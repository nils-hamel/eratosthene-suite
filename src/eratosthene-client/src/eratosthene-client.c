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

    # include "eratosthene-client.h"

/*
    source - global variable (GLUT callbacks)
 */

    er_client_t er_client = ER_CLIENT_C;

/*
    source - constructor/destructor methods
 */

    le_enum_t er_client_create( le_char_t * const er_ip, le_sock_t const er_port ) {

        /* Dominant color variables */
        float er_color[4] = { 0.00, 0.02, 0.04, 0.00 };

        /* Create engine model */
        if ( ( er_client.cl_model = er_model_create( ER_CLIENT_STACK, er_ip, er_port ) )._status == _LE_FALSE ) return( _LE_FALSE );

        /* Create engine times */
        if ( ( er_client.cl_times = er_times_create( er_ip, er_port ) )._status == _LE_FALSE ) return( _LE_FALSE );

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

        /* Color buffer clear values */
        glClearColor( er_color[0], er_color[1], er_color[2], er_color[3] );

        /* Depth buffer clear values */
        glClearDepth( 1.0 );

        /* OpenGL features configuration */
        glEnable( GL_DEPTH_TEST  );
        glEnable( GL_BLEND       );

        /* OpenGL Fog configuration */
        glFogf ( GL_FOG_MODE   , GL_LINEAR );
        glFogf ( GL_FOG_DENSITY, 0.3       );
        glFogfv( GL_FOG_COLOR  , er_color  );
        
        /* Blending function configuration */
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        /* Declare engine callback functions */
        glutIdleFunc         ( er_client_loops_render  );
        glutKeyboardFunc     ( er_client_calls_keybd   );
        glutReshapeFunc      ( er_client_calls_reshape );
        glutMouseFunc        ( er_client_calls_mouse   );
        glutMotionFunc       ( er_client_calls_move    );
        glutPassiveMotionFunc( er_client_calls_move    );

        /* Enable vertex and color arrays */
        glEnableClientState( GL_VERTEX_ARRAY );
        glEnableClientState( GL_COLOR_ARRAY  );

        /* Send message */
        return( _LE_TRUE );

    }

    le_void_t er_client_delete( le_void_t ) {

        /* Engine variables */
        er_client_t er_reset = ER_CLIENT_C;

        /* Delete engine times */
        er_times_delete( & er_client.cl_times );

        /* Delete engine model */
        er_model_delete( & er_client.cl_model );

        /* Clear engine structure */
        er_client = er_reset;

    }

/*
    source - main method/function
 */

    int main( int argc, char ** argv ) {

        /* Server address variables */
        le_char_t * le_addr = ( le_char_t * ) lc_read_string( argc, argv, "--ip"  , "-i" );

        /* Server port variables */
        le_size_t le_port = ( le_size_t   ) lc_read_uint( argc, argv, "--port", "-t", _LE_USE_PORT );

        /* Initialise GL/GLU/GLUT */
        glutInit( & argc, argv );

        /* Create rendering engine */
        if ( er_client_create( le_addr, le_port ) == _LE_FALSE ) {

            /* Display message */
            fprintf( stderr, "eratosthene-suite : error : unable to create model\n" );

        } else {

    # ifdef __OPENMP__
    # pragma omp parallel sections
    {
            /* Model update thread */
            # pragma omp section
            while ( usleep( 500 ), er_client.cl_loops == _LE_TRUE ) er_client_loops_update();

            /* Model display thread */
            # pragma omp section
            glutMainLoop();
    }
    # else
            /* Model update thread */
            pthread_t er_pmodel; pthread_create( & er_pmodel, NULL, & er_client_pthread, NULL );
            
            /* Model display thread */
            glutMainLoop();
    # endif

            /* Delete rendering engine */
            er_client_delete();

        }

        /* Return to system */
        return( EXIT_SUCCESS );

    }

/*
    source - loop methods
 */

    le_void_t er_client_loops_render( le_void_t ) {

        /* Update ranges */
        er_client_calls_range();

        /* Projection : model */
        er_client_proj_model( glutGet( GLUT_SCREEN_WIDTH ), glutGet( GLUT_SCREEN_HEIGHT ) );

        /* Clear color and depth buffers */
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        /* Push matrix - model */
        glPushMatrix(); {

            /* Display earth */
            er_model_display_earth( er_client.cl_vlon, er_client.cl_vlat, er_client.cl_valt, er_client.cl_vazm, er_client.cl_vgam );

        /* Pop matrix - model */
        } glPopMatrix();

        /* Push matrix - model */
        glPushMatrix(); {

            /* Display model */
            er_model_display_cell( & ( er_client.cl_model ), er_client.cl_vlon, er_client.cl_vlat, er_client.cl_valt, er_client.cl_vazm, er_client.cl_vgam );

        /* Pop matrix - model */
        } glPopMatrix();

        /* Projection : interface */
        er_client_proj_interface( glutGet( GLUT_SCREEN_WIDTH ), glutGet( GLUT_SCREEN_HEIGHT ) );

        /* Push matrix - interface */
        glPushMatrix(); {

            /* Display time manager */
            er_times_display( & er_client.cl_times );

        } glPopMatrix();

        /* Swap buffers */
        glutSwapBuffers();

    }

    le_void_t er_client_loops_update( le_void_t ) {

        /* Time variables */
        le_time_t er_etime = _LE_TIME_NULL;

        /* Query model update necessities */
        le_enum_t er_model_flag = er_model_get_update( & er_client.cl_model, er_client.cl_vlon, er_client.cl_vlat, er_client.cl_valt );

        /* Query times update necessities */
        le_enum_t er_times_flag = er_times_get_update( & er_client.cl_times );

        /* Check model update necessities */
        if ( ( er_model_flag == _LE_TRUE ) || ( er_times_flag == _LE_TRUE ) ) {

            /* Prepare model update */
            er_model_set_update_prepare( & er_client.cl_model );

            /* Enable times enumeration */
            while ( ( er_etime = er_times_get( & er_client.cl_times ) ) != _LE_TIME_NULL ) {

                /* Update model cells */
                er_model_set_update_model( & er_client.cl_model, er_etime, er_client.cl_vlon * ER_D2R, er_client.cl_vlat * ER_D2R, er_client.cl_valt );

            }

            /* Server queries */
            er_model_set_update_query( & er_client.cl_model );

            /* Terminate model update */
            er_model_set_update_destroy( & er_client.cl_model );

        }

    }

/*
    source - projection methods
 */

    le_void_t er_client_proj_model( int er_width, int er_height ) {

        /* Clipping plane variables */
        le_real_t er_neac = er_geodesy_near( er_client.cl_valt );
        le_real_t er_farc = er_geodesy_far ( er_client.cl_valt );

        /* Matrix mode to projection */
        glMatrixMode( GL_PROJECTION );

        /* Set projection matrix to identity */
        glLoadIdentity();

        /* Compute projectio matrix */
        gluPerspective( 45.0, ( double ) er_width / er_height, er_neac, er_farc );

        /* Matrix mode to modelview */
        glMatrixMode( GL_MODELVIEW );

        /* Set model view matrix to identity */
        glLoadIdentity();

        /* Compute scale factor */
        er_client.cl_vscl = er_geodesy_scale( er_client.cl_valt );

        /* Apply scale factor to projection matrix */
        glScaled( er_client.cl_vscl, er_client.cl_vscl, er_client.cl_vscl );

        /* Apply fog configuration */
        glFogf( GL_FOG_START, er_farc * 0.8 );
        glFogf( GL_FOG_END  , er_farc       );

        /* Enable fog feature */
        glEnable( GL_FOG );

    }

    le_void_t er_client_proj_interface( int er_width, int er_height ) {

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

        /* Disable fog feature */
        glDisable( GL_FOG );

    }

/*
    source - callback methods
 */

    le_void_t er_client_calls_reshape( int er_width, int er_height ) {

        /* Reset viewport */
        glViewport( 0, 0, er_width, er_height );

    }

    le_void_t er_client_calls_keybd( unsigned char er_keycode, int er_x, int er_y ) {

        /* Switch on keycode */
        switch( er_keycode ) {

            /* Interrupt engine loops */
            case (  27 ) : { glutLeaveMainLoop(), er_client.cl_loops = _LE_FALSE; } break;

            /* Update point size */
            case (  49 ) :
            case (  50 ) :
            case (  51 ) : 
            case (  52 ) : { glPointSize( er_keycode - 48 ); } break;

            /* Update point of view */
            case (  99 ) : { er_client.cl_valt = ER_ERD; }
            case ( 120 ) : { er_client.cl_vazm = 0.0; }
            case ( 121 ) : { er_client.cl_vgam = 0.0; } break;

            /* Update times */
            case (  97 ) : { er_times_set( & er_client.cl_times, 0 ); } break;
            case ( 115 ) : { er_times_set( & er_client.cl_times, 1 ); } break;

        };

    }

    le_void_t er_client_calls_mouse( int er_button, int er_state, int er_x, int er_y ) {

        /* Update engine handle */
        er_client.cl_button  = er_button;
        er_client.cl_state   = er_state;
        er_client.cl_x       = er_x;
        er_client.cl_y       = er_y;
        er_client.cl_inertia = ER_INB * fabs( er_client.cl_valt - ER_ERA );

        /* Clamp inertial value */
        if ( er_client.cl_inertia < 5.0 ) er_client.cl_inertia = 5.0;

        /* Inertial multiplier */
        if ( glutGetModifiers() == GLUT_ACTIVE_CTRL  ) er_client.cl_inertia *= ER_IMU;
        if ( glutGetModifiers() == GLUT_ACTIVE_SHIFT ) er_client.cl_inertia *= ER_IML;

        /* Check mouse state */
        if ( er_client.cl_state != GLUT_DOWN ) return;

        /* Interface switch */
        if ( glutGetModifiers() == ( GLUT_ACTIVE_CTRL | GLUT_ACTIVE_ALT ) ) {

            /* Mouse event switch - update time zoom */
            if ( er_client.cl_button == 3 ) er_times_set_zoom( & er_client.cl_times, 1.0990 );
            if ( er_client.cl_button == 4 ) er_times_set_zoom( & er_client.cl_times, 0.9099 );

        } else
        if ( glutGetModifiers() == GLUT_ACTIVE_ALT ) {

            /* Mouse event switch - update time position */
            if ( er_client.cl_button == 3 ) er_times_set_pose( & er_client.cl_times, + 0.02 );
            if ( er_client.cl_button == 4 ) er_times_set_pose( & er_client.cl_times, - 0.02 );

        } else {

            /* Mouse event switch - update altitude */
            if ( er_client.cl_button == 3 ) er_client.cl_valt += er_client.cl_inertia;
            if ( er_client.cl_button == 4 ) er_client.cl_valt -= er_client.cl_inertia;

        }

    }

    le_void_t er_client_calls_move( int er_x, int er_y ) {

        /* Check mouse state */
        if ( er_client.cl_state != GLUT_DOWN ) return;

        /* Mouse event switch */
        if ( er_client.cl_button == GLUT_LEFT_BUTTON ) {

            /* Update longitude and latitude */
            er_client.cl_vlon += ( er_y - er_client.cl_y ) * ( ER_INT * er_client.cl_inertia ) * sin( er_client.cl_vazm * ER_D2R );
            er_client.cl_vlat += ( er_y - er_client.cl_y ) * ( ER_INT * er_client.cl_inertia ) * cos( er_client.cl_vazm * ER_D2R );
            er_client.cl_vlon -= ( er_x - er_client.cl_x ) * ( ER_INT * er_client.cl_inertia ) * cos( er_client.cl_vazm * ER_D2R );
            er_client.cl_vlat += ( er_x - er_client.cl_x ) * ( ER_INT * er_client.cl_inertia ) * sin( er_client.cl_vazm * ER_D2R );

        } else 
        if ( er_client.cl_button == GLUT_RIGHT_BUTTON ) {

            /* Update azimuth and gamma */
            er_client.cl_vazm -= ( er_x - er_client.cl_x ) * ER_INR;
            er_client.cl_vgam += ( er_y - er_client.cl_y ) * ER_INR;

        }

    }

    le_void_t er_client_calls_range() {

        /* Angle ranges - cyclic */
        if ( er_client.cl_vlon > +180.0 ) er_client.cl_vlon -= +360.0;
        if ( er_client.cl_vlon < -180.0 ) er_client.cl_vlon += +360.0;
        if ( er_client.cl_vazm > +360.0 ) er_client.cl_vazm -= +360.0;
        if ( er_client.cl_vazm < -360.0 ) er_client.cl_vazm += +360.0;

        /* Angles ranges - clamp */
        if ( er_client.cl_vlat > + 90.0 ) er_client.cl_vlat = + 90.0;
        if ( er_client.cl_vlat < - 90.0 ) er_client.cl_vlat = - 90.0;
        if ( er_client.cl_vgam < -120.0 ) er_client.cl_vgam = -120.0;
        if ( er_client.cl_vgam > +120.0 ) er_client.cl_vgam = +120.0;

        /* Radius ranges - clamp */
        if ( er_client.cl_valt < ER_ERL ) er_client.cl_valt = ER_ERL;
        if ( er_client.cl_valt > ER_ERU ) er_client.cl_valt = ER_ERU;

    }

/*
    source - auxiliary methods
 */

    void * er_client_pthread( void * er_null ) {

        /* Model update thread - pthread specific derivation */
        while ( usleep( 500 ), er_client.cl_loops == _LE_TRUE ) er_client_loops_update();

        /* Return null pointer */
        return( NULL );

    }

