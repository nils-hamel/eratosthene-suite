/*
 *  eratosthene-suite - geodetic system
 *
 *      Nils Hamel - nils.hamel@bluewin.ch
 *      Copyright (c) 2016 EPFL CDH DHLAB
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

    # include "eratosthene-client-engine.h"

/*
    source - ugly global variable (GLUT callbacks)
 */

    er_engine_t er_engine = ER_ENGINE_C;

/*
    source - constructor/destructor methods
 */

    le_void_t er_engine_create( le_size_t const er_stack, le_char_t const * const er_ip, le_sock_t const er_port ) {

        /* Create rendering window */
        glutCreateWindow( "eratosthene-client" );

        /* Fullscreen rendering window */
        glutFullScreen();

        /* Cursor configuration */
        glutSetCursor( GLUT_CURSOR_NONE );

        /* Initialise display mode */
        glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );

        /* Graphical thread behavior configuration */
        glutSetOption( GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION );

        /* Buffers clear values */
        glClearColor( 0.0, 0.0, 0.0, 0.0 );
        glClearDepth( 1.0 );

        /* Graphical thread configuration */
        glEnable( GL_DEPTH_TEST );

        /* Declare engine callback functions */
        glutDisplayFunc      ( er_engine_render  );
        glutIdleFunc         ( er_engine_render  );
        glutReshapeFunc      ( er_engine_reshape );
        glutKeyboardFunc     ( er_engine_keybd   );
        glutMouseFunc        ( er_engine_mouse   );
        glutMotionFunc       ( er_engine_move    );
        glutPassiveMotionFunc( er_engine_move    );

        /* Enable vertex and color arrays */
        glEnableClientState( GL_VERTEX_ARRAY );
        glEnableClientState( GL_COLOR_ARRAY  );

        /* Create cell model */
        er_engine.eg_model = er_model_create( er_stack, er_ip, er_port );

    }

    le_void_t er_engine_delete( le_void_t ) {

        /* Engine variables */
        er_engine_t er_reset = ER_ENGINE_C;

        /* Delete model */
        er_model_delete( & er_engine.eg_model );

        /* Enable vertex and color arrays */
        glEnableClientState( GL_VERTEX_ARRAY );
        glEnableClientState( GL_COLOR_ARRAY  );

        /* Clear engine structure */
        er_engine = er_reset;

    }

/*
    source - rendering engine
 */

    le_void_t er_engine_loops( le_void_t ) {

        /* Thread variables */
        pthread_t er_secondary;

        /* Engine secondary loop */
        pthread_create( & er_secondary, NULL, & er_engine_update, NULL );

        /* Engine primary loop */
        glutMainLoop();

        /* Engine secondary loop */
        pthread_cancel( er_secondary );

    }

/*
    source - engine callbacks - primary
 */

    le_void_t er_engine_render( le_void_t ) {

        /* Update ranges */
        er_engine_range();

        /* Recompute near/far planes */
        er_engine_reshape( glutGet( GLUT_SCREEN_WIDTH ), glutGet( GLUT_SCREEN_HEIGHT ) );

        /* Clear color and depth buffers */
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        /* Configure points display */
        glPointSize( er_engine.eg_point );

        /* Push matrix */
        glPushMatrix(); {

            /* Motion management - translation */
            glTranslated( 0.0, +sin( er_engine.eg_vgam * ER_D2R ) * er_engine.eg_valt, -cos( er_engine.eg_vgam * ER_D2R ) * er_engine.eg_valt );

            /* Motion management - rotations */
            glRotated( +er_engine.eg_vgam, 1.0, 0.0, 0.0 );
            glRotated( +er_engine.eg_vazm, 0.0, 0.0, 1.0 );
            glRotated( +er_engine.eg_vlat, 1.0, 0.0, 0.0 );
            glRotated( -er_engine.eg_vlon, 0.0, 1.0, 0.0 );

            /* Display model */
            er_model_display_cell( & ( er_engine.eg_model ) );

            /* Display earth */
            er_model_display_earth();

        /* Pop matrix */
        } glPopMatrix();

        /* Swap buffers */
        glutSwapBuffers();

    }

    le_void_t * er_engine_update( le_void_t * er_null ) {

        /* Engine update loop */
        for ( ; ; sleep( 0.5 ) ) {

            /* Check engine model state */
            if ( er_engine.eg_sflag == _LE_TRUE ) {

                /* Update model */
                er_model_set_model( & ( er_engine.eg_model ), er_engine.eg_vtim, er_engine.eg_vlon * ER_D2R, er_engine.eg_vlat * ER_D2R, er_engine.eg_valt );

                /* Reset engine model state */
                er_engine.eg_sflag = _LE_FALSE;

            }

        /* Return null pointer */
        } return( NULL );

    }

/*
    source - engine callbacks - reshape
 */

    le_void_t er_engine_reshape( int er_width, int er_height ) {

        /* Compute scale factor */
        er_engine.eg_vscl = er_geodesy_scale( er_engine.eg_valt );

        /* Reset viewport */
        glViewport( 0, 0, er_width, er_height );

        /* Matrix mode to projection */
        glMatrixMode( GL_PROJECTION );

        /* Set projection matrix to identity */
        glLoadIdentity();

        /* Compute projectio matrix */
        gluPerspective( 45.0, ( double ) er_width / er_height, er_geodesy_near( er_engine.eg_valt ), er_geodesy_far( er_engine.eg_valt ) );

        /* Matrix mode to modelview */
        glMatrixMode( GL_MODELVIEW );

        /* Set model view matrix to identity */
        glLoadIdentity();

        /* Apply scale factor to projection matrix */
        glScaled( er_engine.eg_vscl, er_engine.eg_vscl, er_engine.eg_vscl );

    }

/*
    source - engine callbacks - keyboard
 */

    le_void_t er_engine_keybd( unsigned char er_keycode, int er_x, int er_y ) {

        /* Switch on keycode */
        switch( er_keycode ) {

            /* Escape */
            case ( 27 ) : {

                /* Leave GLUT events management loop */
                glutLeaveMainLoop();

            } break;

            /* Point size control */
            case ( 'w' ) : {

                /* Update point size */
                er_engine.eg_point = ( er_engine.eg_point < 8 ) ? er_engine.eg_point + 1 : 8;

            } break;

            /* Point size control */
            case ( 'q' ) : {

                /* Update point size */
                er_engine.eg_point = ( er_engine.eg_point > 1 ) ? er_engine.eg_point - 1 : 1;

            } break;

            /* Reset azimuth and gamma */
            case ( 'e' ) : {

                /* Reset variables */
                er_engine.eg_vazm = 0.0;
                er_engine.eg_vgam = 0.0;

            } break;

            /* Reset point of view */
            case ( 'r' ) : {

                /* Reset variables */
                er_engine.eg_vlon = 0.0;
                er_engine.eg_vlat = 0.0;
                er_engine.eg_valt = 1.5 * ER_ERA;
                er_engine.eg_vazm = 0.0;
                er_engine.eg_vgam = 0.0;

                /* Push motion flag */
                er_engine.eg_sflag = _LE_TRUE;

            } break;

            /* Time management */
            case ( 'a' ) : {

                /* Update time */
                er_engine.eg_vtim -= er_model_get_tdisc( & ( er_engine.eg_model ) );

                /* Push motion flag */
                er_engine.eg_sflag = _LE_TRUE;

            } break;

            /* Time management */
            case ( 's' ) : {

                /* Update time */
                er_engine.eg_vtim += er_model_get_tdisc( & ( er_engine.eg_model ) );

                /* Push motion flag */
                er_engine.eg_sflag = _LE_TRUE;

            } break;

            /* Temporary */
            case ( 'v' ) : er_engine.eg_vtim = 16917 * er_model_get_tdisc( & ( er_engine.eg_model ) ) + 1; er_engine.eg_sflag = _LE_TRUE; break;
            case ( 'b' ) : er_engine.eg_vtim = 11001 * er_model_get_tdisc( & ( er_engine.eg_model ) ) + 1; er_engine.eg_sflag = _LE_TRUE; break;

        };

    }

/*
    source - engine callbacks - mouse
 */

    le_void_t er_engine_mouse( int er_button, int er_state, int er_x, int er_y ) {

        /* Update engine handle */
        er_engine.eg_button = er_button;
        er_engine.eg_state  = er_state;
        er_engine.eg_x      = er_x;
        er_engine.eg_y      = er_y;
        er_engine.eg_u      = er_x;
        er_engine.eg_v      = er_y;

        /* Inertial multiplier */
        er_engine.eg_mult = glutGetModifiers() == GLUT_ACTIVE_CTRL ? 10.0 : 1.0;

        /* Mouse event switch */
        if ( ( er_engine.eg_button == 3 ) && ( er_engine.eg_state == GLUT_DOWN ) ) {

            /* Update altitude */
            er_engine.eg_valt += ( ER_INA * er_engine.eg_mult ) * ( er_engine.eg_valt - ER_ERA - LE_GEODESY_HMIN );

            /* Push motion flag */
            er_engine.eg_sflag = _LE_TRUE;

        }

        /* Mouse event switch */
        if ( ( er_engine.eg_button == 4 ) && ( er_engine.eg_state == GLUT_DOWN ) ) {

            /* Update altitude */
            er_engine.eg_valt -= ( ER_INA * er_engine.eg_mult ) * ( er_engine.eg_valt - ER_ERA - LE_GEODESY_HMIN );

            /* Push motion flag */
            er_engine.eg_sflag = _LE_TRUE;

        }

    }

    le_void_t er_engine_move( int er_x, int er_y ) {

        /* Check mouse state */
        if ( er_engine.eg_state == GLUT_DOWN ) {

            /* Update engine handle */
            er_engine.eg_u = er_x;
            er_engine.eg_v = er_y;

        }

        /* Mouse event switch */
        if ( ( er_engine.eg_button == GLUT_LEFT_BUTTON ) && ( er_engine.eg_state == GLUT_DOWN ) ) {

            /* Update longitude and latitude */
            er_engine.eg_vlon -= ( ER_INT * er_engine.eg_mult ) * ( er_engine.eg_u - er_engine.eg_x );
            er_engine.eg_vlat += ( ER_INT * er_engine.eg_mult ) * ( er_engine.eg_v - er_engine.eg_y );

            /* Push motion flag */
            er_engine.eg_sflag = _LE_TRUE;

        }

        /* Mouse switch event */
        if ( ( er_engine.eg_button == GLUT_MIDDLE_BUTTON ) && ( er_engine.eg_state == GLUT_DOWN ) ) {

            /* Update longitude and latitude */
            er_engine.eg_vlon += ( ER_INT * er_engine.eg_mult ) * ( er_engine.eg_v - er_engine.eg_y ) * sin( er_engine.eg_vazm * ER_D2R );
            er_engine.eg_vlat += ( ER_INT * er_engine.eg_mult ) * ( er_engine.eg_v - er_engine.eg_y ) * cos( er_engine.eg_vazm * ER_D2R );

            /* Push motion flag */
            er_engine.eg_sflag = _LE_TRUE;

        }

        /* Mouse event switch */
        if ( ( er_engine.eg_button == GLUT_RIGHT_BUTTON ) && ( er_engine.eg_state == GLUT_DOWN ) ) {

            /* Update azimuth and gamma angles */
            er_engine.eg_vazm -= ( ER_INR * er_engine.eg_mult ) * ( er_engine.eg_u - er_engine.eg_x );
            er_engine.eg_vgam -= ( ER_INR * er_engine.eg_mult ) * ( er_engine.eg_v - er_engine.eg_y ) * 2.0;

        }

    }

/*
    source - engine callbacks - ranges
 */

    le_void_t er_engine_range() {

        /* Angle ranges - cyclic */
        if ( er_engine.eg_vlon > +180.0 ) er_engine.eg_vlon -= +360.0;
        if ( er_engine.eg_vlon < -180.0 ) er_engine.eg_vlon += +360.0;
        if ( er_engine.eg_vlat > + 90.0 ) er_engine.eg_vlat  = + 90.0;
        if ( er_engine.eg_vlat < - 90.0 ) er_engine.eg_vlat  = - 90.0;
        if ( er_engine.eg_vazm > +360.0 ) er_engine.eg_vazm -= +360.0;
        if ( er_engine.eg_vazm < -360.0 ) er_engine.eg_vazm += +360.0;

        /* Angles ranges - clamp */
        if ( er_engine.eg_vgam < -120.0 ) er_engine.eg_vgam = -120.0;
        if ( er_engine.eg_vgam > +  0.0 ) er_engine.eg_vgam = +  0.0;

        /* Parameter ranges - clamp */
        if ( er_engine.eg_valt < ER_ERL ) er_engine.eg_valt = ER_ERL;
        if ( er_engine.eg_valt > ER_ERU ) er_engine.eg_valt = ER_ERU;

    }

