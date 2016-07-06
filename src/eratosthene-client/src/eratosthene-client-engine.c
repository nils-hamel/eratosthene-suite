/*
 *  eratosthene-suite - eratosthene indexation server front-end
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

    le_enum_t er_engine_create( le_char_t * const er_ip, le_sock_t const er_port ) {

        /* Create cell model */
        er_engine.eg_model = er_model_create( ER_ENGINE_STACK, er_ip, er_port );

        /* Check model creation - send message */
        if ( er_model_get_sdisc( & er_engine.eg_model ) == _LE_SIZE_NULL ) return( _LE_FALSE );

        /* Initialise display mode */
        glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );

        /* Create rendering window */
        glutCreateWindow( "eratosthene-client" );

        /* Fullscreen rendering window */
        glutFullScreen();

        /* Cursor configuration */
        glutSetCursor( GLUT_CURSOR_NONE );

        /* Graphical thread behavior configuration */
        glutSetOption( GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION );

        /* Buffers clear values */
        glClearColor( 0.0, 0.0, 0.0, 0.0 );
        glClearDepth( 1.0 );

        /* Graphical thread configuration */
        glEnable( GL_DEPTH_TEST );

        /* Declare engine callback functions */
        glutIdleFunc         ( er_engine_loops_render );
        glutKeyboardFunc     ( er_engine_calls_keybd  );
        glutMouseFunc        ( er_engine_calls_mouse  );
        glutMotionFunc       ( er_engine_calls_move   );
        glutPassiveMotionFunc( er_engine_calls_move   );

        /* Enable vertex and color arrays */
        glEnableClientState( GL_VERTEX_ARRAY );
        glEnableClientState( GL_COLOR_ARRAY  );

        /* Send message */
        return( _LE_TRUE );

    }

    le_void_t er_engine_delete( le_void_t ) {

        /* Engine variables */
        er_engine_t er_reset = ER_ENGINE_C;

        /* Delete cell model */
        er_model_delete( & er_engine.eg_model );

        /* Clear engine structure */
        er_engine = er_reset;

    }

/*
    source - engine loops - primary
 */

    le_void_t er_engine_loops( le_void_t ) {
    # ifdef __OPENMP__
    # pragma omp parallel sections
    {

        /* Engine secondary loop - rendering and callbacks */
        # pragma omp section
        glutMainLoop();

        /* Engine secondary loop - model update */
        # pragma omp section
        er_engine_loops_update( NULL );

    }
    # else

        /* Thread variables */
        pthread_t er_secondary;

        /* Engine secondary loops */
        pthread_create( & er_secondary, NULL, & er_engine_loops_update, NULL );

        /* Engine primary loop */
        glutMainLoop();

    # endif
    }

/*
    source - engine loops - secondary
 */

    le_void_t er_engine_loops_render( le_void_t ) {

        /* Update ranges */
        er_engine_calls_range();

        /* Recompute near/far planes */
        er_engine_calls_reshape( glutGet( GLUT_SCREEN_WIDTH ), glutGet( GLUT_SCREEN_HEIGHT ) );

        /* Configure points display */
        glPointSize( er_engine.eg_point );

        /* Clear color and depth buffers */
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        /* Earth wireframe matrix */
        glPushMatrix(); {

            /* Display earth */
            er_model_display_earth( er_engine.eg_vlon, er_engine.eg_vlat, er_engine.eg_valt, er_engine.eg_vazm, er_engine.eg_vgam );

        /* Earth wireframe matrix */
        } glPopMatrix();

        /* Server cells matrix */
        glPushMatrix(); {

            /* Display model */
            er_model_display_cell( & ( er_engine.eg_model ), er_engine.eg_vlon, er_engine.eg_vlat, er_engine.eg_valt, er_engine.eg_vazm, er_engine.eg_vgam );

        /* Server cells matrix */
        } glPopMatrix();

        /* Swap buffers */
        glutSwapBuffers();

    }

    le_void_t * er_engine_loops_update( le_void_t * er_null ) {

        /* Model update loop */
        while ( usleep( 500 ), er_engine.eg_loops == _LE_TRUE ) {

            /* Check model update necessities */
            if ( er_model_get_update( & er_engine.eg_model, er_engine.eg_vtim, er_engine.eg_vlon * ER_D2R, er_engine.eg_vlat * ER_D2R, er_engine.eg_valt ) == _LE_TRUE ) {

                /* Prepare model update */
                er_model_set_update_prepare( & er_engine.eg_model );

                /* Update model cells */
                er_model_set_update_model  ( & er_engine.eg_model, er_engine.eg_vtim, er_engine.eg_vlon * ER_D2R, er_engine.eg_vlat * ER_D2R, er_engine.eg_valt );

                /* Server queries */
                er_model_set_update_query  ( & er_engine.eg_model );

                /* Terminate model update */
                er_model_set_update_destroy( & er_engine.eg_model );

            }

        /* Return null pointer */
        } return( NULL );

    }

/*
    source - engine callbacks - reshape
 */

    le_void_t er_engine_calls_reshape( int er_width, int er_height ) {

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

    le_void_t er_engine_calls_keybd( unsigned char er_keycode, int er_x, int er_y ) {

        /* Switch on keycode */
        switch( er_keycode ) {

            /* Interrupt engine loops */
            case ( 27 ) : { glutLeaveMainLoop(), er_engine.eg_loops = _LE_FALSE; } break;

            /* Update point size */
            case ( 'w' ) : { er_engine.eg_point ++; } break;
            case ( 'q' ) : { er_engine.eg_point --; } break;

            /* Update point of view */
            case ( 'r' ) : { er_engine.eg_valt = 1.5 * ER_ERA; }
            case ( 'a' ) : { er_engine.eg_vazm = 0.0; }
            case ( 'g' ) : { er_engine.eg_vgam = 0.0; } break;

            /* Display position */
            case ( 'p' ) : {

                /* Display position and orientation */
                printf( "%" _LE_REAL_P "\n", er_engine.eg_vlon );
                printf( "%" _LE_REAL_P "\n", er_engine.eg_vlat );
                printf( "%" _LE_REAL_P "\n", er_engine.eg_valt );
                printf( "%" _LE_REAL_P "\n", er_engine.eg_vazm );
                printf( "%" _LE_REAL_P "\n", er_engine.eg_vgam );

            } break;

        };

    }

/*
    source - engine callbacks - mouse
 */

    le_void_t er_engine_calls_mouse( int er_button, int er_state, int er_x, int er_y ) {

        /* Update engine handle */
        er_engine.eg_button = er_button;
        er_engine.eg_state  = er_state;
        er_engine.eg_x      = er_x;
        er_engine.eg_y      = er_y;
        er_engine.eg_mult   = ER_INB * fabs( er_engine.eg_valt - ER_ERA );

        /* Clamp inertial value */
        if ( er_engine.eg_mult < 5.0 ) er_engine.eg_mult = 5.0;

        /* Inertial multiplier */
        if ( glutGetModifiers() == GLUT_ACTIVE_CTRL  ) er_engine.eg_mult *= ER_IMU;
        if ( glutGetModifiers() == GLUT_ACTIVE_SHIFT ) er_engine.eg_mult *= ER_IML;

        /* Check mouse state */
        if ( er_engine.eg_state != GLUT_DOWN ) return;

        /* Mouse event switch - update altitude */
        if ( er_engine.eg_button == 3 ) er_engine.eg_valt += er_engine.eg_mult;
        if ( er_engine.eg_button == 4 ) er_engine.eg_valt -= er_engine.eg_mult;

    }

    le_void_t er_engine_calls_move( int er_x, int er_y ) {

        /* Check mouse state */
        if ( er_engine.eg_state != GLUT_DOWN ) return;

        /* Mouse event switch */
        if ( er_engine.eg_button == GLUT_LEFT_BUTTON ) {

            /* Update longitude and latitude */
            er_engine.eg_vlon += ( er_y - er_engine.eg_y ) * ( ER_INT * er_engine.eg_mult ) * sin( er_engine.eg_vazm * ER_D2R );
            er_engine.eg_vlat += ( er_y - er_engine.eg_y ) * ( ER_INT * er_engine.eg_mult ) * cos( er_engine.eg_vazm * ER_D2R );
            er_engine.eg_vlon -= ( er_x - er_engine.eg_x ) * ( ER_INT * er_engine.eg_mult ) * cos( er_engine.eg_vazm * ER_D2R );
            er_engine.eg_vlat += ( er_x - er_engine.eg_x ) * ( ER_INT * er_engine.eg_mult ) * sin( er_engine.eg_vazm * ER_D2R );

        } else 
        if ( er_engine.eg_button == GLUT_RIGHT_BUTTON ) {

            /* Update azimuth and gamma */
            er_engine.eg_vazm -= ( er_x - er_engine.eg_x ) * ER_INR;
            er_engine.eg_vgam += ( er_y - er_engine.eg_y ) * ER_INR;

        }

    }

/*
    source - engine callbacks - ranges
 */

    le_void_t er_engine_calls_range() {

        /* Angle ranges - cyclic */
        if ( er_engine.eg_vlon > +180.0 ) er_engine.eg_vlon -= +360.0;
        if ( er_engine.eg_vlon < -180.0 ) er_engine.eg_vlon += +360.0;
        if ( er_engine.eg_vlat > + 90.0 ) er_engine.eg_vlat  = + 90.0;
        if ( er_engine.eg_vlat < - 90.0 ) er_engine.eg_vlat  = - 90.0;
        if ( er_engine.eg_vazm > +360.0 ) er_engine.eg_vazm -= +360.0;
        if ( er_engine.eg_vazm < -360.0 ) er_engine.eg_vazm += +360.0;

        /* Angles ranges - clamp */
        if ( er_engine.eg_vgam < -120.0 ) er_engine.eg_vgam = -120.0;
        if ( er_engine.eg_vgam > +120.0 ) er_engine.eg_vgam = +120.0;

        /* Radius ranges - clamp */
        if ( er_engine.eg_valt < ER_ERL ) er_engine.eg_valt = ER_ERL;
        if ( er_engine.eg_valt > ER_ERU ) er_engine.eg_valt = ER_ERU;

        /* Point size ranges - clamp */
        if ( er_engine.eg_point < 1 ) er_engine.eg_point = 1;
        if ( er_engine.eg_point > 8 ) er_engine.eg_point = 8;

    }

