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

        /* Create engine model */
        if ( ( er_engine.eg_model = er_model_create( ER_ENGINE_STACK, er_ip, er_port ) )._status == _LE_FALSE ) return( _LE_FALSE );

        /* Create engine times */
        if ( ( er_engine.eg_times = er_times_create( er_ip, er_port ) )._status == _LE_FALSE ) return( _LE_FALSE );

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

        /* OpenGL features configuration */
        glEnable( GL_DEPTH_TEST );
        glEnable( GL_BLEND );

        /* Blending function configuration */
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        /* Declare engine callback functions */
        glutIdleFunc         ( er_engine_loops_render  );
        glutKeyboardFunc     ( er_engine_calls_keybd   );
        glutReshapeFunc      ( er_engine_calls_reshape );
        glutMouseFunc        ( er_engine_calls_mouse   );
        glutMotionFunc       ( er_engine_calls_move    );
        glutPassiveMotionFunc( er_engine_calls_move    );

        /* Enable vertex and color arrays */
        glEnableClientState( GL_VERTEX_ARRAY );
        glEnableClientState( GL_COLOR_ARRAY  );

        /* Send message */
        return( _LE_TRUE );

    }

    le_void_t er_engine_delete( le_void_t ) {

        /* Engine variables */
        er_engine_t er_reset = ER_ENGINE_C;

        /* Delete engine times */
        er_times_delete( & er_engine.eg_times );

        /* Delete engine model */
        er_model_delete( & er_engine.eg_model );

        /* Clear engine structure */
        er_engine = er_reset;

    }

/*
    source - engine loops - primary methods
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
    source - engine loops - secondary methods
 */

    le_void_t er_engine_loops_render( le_void_t ) {

        /* Update ranges */
        er_engine_calls_range();

        /* Projection : model */
        er_engine_proj_model( glutGet( GLUT_SCREEN_WIDTH ), glutGet( GLUT_SCREEN_HEIGHT ) );

        /* Clear color and depth buffers */
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        /* Configure points display */
        glPointSize( er_engine.eg_point );

        /* Push matrix - model */
        glPushMatrix(); {

            /* Display earth */
            er_model_display_earth( er_engine.eg_vlon, er_engine.eg_vlat, er_engine.eg_valt, er_engine.eg_vazm, er_engine.eg_vgam );

        /* Pop matrix - model */
        } glPopMatrix();

        /* Push matrix - model */
        glPushMatrix(); {

            /* Display model */
            er_model_display_cell( & ( er_engine.eg_model ), er_engine.eg_vlon, er_engine.eg_vlat, er_engine.eg_valt, er_engine.eg_vazm, er_engine.eg_vgam );

        /* Pop matrix - model */
        } glPopMatrix();

        /* Projection : interface */
        er_engine_proj_interface( glutGet( GLUT_SCREEN_WIDTH ), glutGet( GLUT_SCREEN_HEIGHT ) );

        /* Push matrix - interface */
        glPushMatrix(); {

            /* Display time manager */
            er_times_display( & er_engine.eg_times );

        } glPopMatrix();

        /* Swap buffers */
        glutSwapBuffers();

    }

    le_void_t * er_engine_loops_update( le_void_t * er_null ) {

        /* Time variables */
        le_time_t er_etime = _LE_TIME_NULL;

        /* Update variables */
        le_enum_t er_model_flag = _LE_FALSE;
        le_enum_t er_times_flag = _LE_FALSE;

        /* Model update loop */
        while ( usleep( 500 ), er_engine.eg_loops == _LE_TRUE ) {

            /* Query model update necessities */
            er_model_flag = er_model_get_update( & er_engine.eg_model, er_engine.eg_vlon, er_engine.eg_vlat, er_engine.eg_valt );

            /* Query times update necessities */
            er_times_flag = er_times_get_update( & er_engine.eg_times );

            /* Check model update necessities */
            if ( ( er_model_flag == _LE_TRUE ) || ( er_times_flag == _LE_TRUE ) ) {

                /* Prepare model update */
                er_model_set_update_prepare( & er_engine.eg_model );

                /* Enable times enumeration */
                while ( ( er_etime = er_times_get( & er_engine.eg_times ) ) != _LE_TIME_NULL ) {

                    /* Update model cells */
                    er_model_set_update_model( & er_engine.eg_model, er_etime, er_engine.eg_vlon * ER_D2R, er_engine.eg_vlat * ER_D2R, er_engine.eg_valt );

                }

                /* Server queries */
                er_model_set_update_query( & er_engine.eg_model );

                /* Terminate model update */
                er_model_set_update_destroy( & er_engine.eg_model );

            }

        /* Return null pointer */
        } return( NULL );

    }

/*
    source - projection methods
 */

    le_void_t er_engine_proj_model( int er_width, int er_height ) {

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

        /* Compute scale factor */
        er_engine.eg_vscl = er_geodesy_scale( er_engine.eg_valt );

        /* Apply scale factor to projection matrix */
        glScaled( er_engine.eg_vscl, er_engine.eg_vscl, er_engine.eg_vscl );

    }

    le_void_t er_engine_proj_interface( int er_width, int er_height ) {

        /* Matrix mode to projection */
        glMatrixMode( GL_PROJECTION );

        /* Set projection matrix to identity */
        glLoadIdentity();

        /* Compute projectio matrix */
        glOrtho( 0, glutGet( GLUT_SCREEN_WIDTH ), 0, glutGet( GLUT_SCREEN_HEIGHT ), -1.0, 1.0 );

        /* Matrix mode to modelview */
        glMatrixMode( GL_MODELVIEW );

        /* Set model view matrix to identity */
        glLoadIdentity();

    }

/*
    source - engine callbacks - reshape methods
 */

    le_void_t er_engine_calls_reshape( int er_width, int er_height ) {

        /* Reset viewport */
        glViewport( 0, 0, er_width, er_height );

    }

/*
    source - engine callbacks - keyboard methods
 */

    le_void_t er_engine_calls_keybd( unsigned char er_keycode, int er_x, int er_y ) {

        /* Switch on keycode */
        switch( er_keycode ) {

            /* Interrupt engine loops */
            case (  27 ) : { glutLeaveMainLoop(), er_engine.eg_loops = _LE_FALSE; } break;

            /* Update point size */
            case ( 'w' ) : { er_engine.eg_point ++; } break;
            case ( 'q' ) : { er_engine.eg_point --; } break;

            /* Update point of view */
            case ( 'r' ) : { er_engine.eg_valt = 1.5 * ER_ERA; }
            case ( 'a' ) : { er_engine.eg_vazm = 0.0; }
            case ( 'g' ) : { er_engine.eg_vgam = 0.0; } break;

            /* Update times */
            case ( 't' ) : { er_times_set( & er_engine.eg_times ); } break;

            /* Update times */
            case ( 'n' ) : { er_times_set_time( & er_engine.eg_times, ER_TIMES_DTIME ); } break;
            case ( 'm' ) : { er_times_set_time( & er_engine.eg_times, ER_TIMES_ITIME ); } break;

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
    source - engine callbacks - mouse methods
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

        /* Interface switch */
        if ( glutGetModifiers() == GLUT_ACTIVE_ALT ) {

            /* Mouse event switch - update time zoom */
            if ( er_engine.eg_button == 3 ) er_times_set_zoom( & er_engine.eg_times, ER_TIMES_IZOOM );
            if ( er_engine.eg_button == 4 ) er_times_set_zoom( & er_engine.eg_times, ER_TIMES_DZOOM );

        } else
        if ( glutGetModifiers() == ( GLUT_ACTIVE_CTRL | GLUT_ACTIVE_ALT ) ) {

            /* Mouse event switch - update time position */
            if ( er_engine.eg_button == 3 ) er_times_set_pose( & er_engine.eg_times, ER_TIMES_IPOSE );
            if ( er_engine.eg_button == 4 ) er_times_set_pose( & er_engine.eg_times, ER_TIMES_DPOSE );

        } else {

            /* Mouse event switch - update altitude */
            if ( er_engine.eg_button == 3 ) er_engine.eg_valt += er_engine.eg_mult;
            if ( er_engine.eg_button == 4 ) er_engine.eg_valt -= er_engine.eg_mult;

        }

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
    source - engine callbacks - ranges methods
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

