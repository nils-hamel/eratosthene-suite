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
        if ( ( er_client.cl_model = er_model_create( er_ip, er_port ) )._status == _LE_FALSE ) return( _LE_FALSE );

        /* Create engine times */
        if ( ( er_client.cl_times = er_times_create( er_ip, er_port ) )._status == _LE_FALSE ) return( _LE_FALSE );

        /* Initialise display mode */
        glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );

        /* Create rendering window */
        glutCreateWindow( "eratosthene-client" );

        /* Rendering window configuration */
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

        /* OpenGL fog configuration */
        glFogf ( GL_FOG_MODE   , GL_LINEAR );
        glFogf ( GL_FOG_DENSITY, 0.3       );
        glFogfv( GL_FOG_COLOR  , er_color  );
        
        /* OpenGL blending configuration */
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

        /* Cleared structure variables */
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
        le_char_t * le_svip = ( le_char_t * ) lc_read_string( argc, argv, "--ip"  , "-i" );

        /* Server port variables */
        le_size_t le_port = ( le_size_t   ) lc_read_uint( argc, argv, "--port", "-t", _LE_USE_PORT );

        /* Initialise GL/GLU/GLUT */
        glutInit( & argc, argv );

        /* Create client */
        if ( er_client_create( le_svip, le_port ) == _LE_FALSE ) {

            /* Display message */
            fprintf( stderr, "eratosthene-suite : error : unable to create client\n" );

            /* Return to system */
            return( EXIT_SUCCESS );

        }

    # ifdef __OPENMP__
    # pragma omp parallel sections
    {
        /* Model update thread */
        # pragma omp section
        while ( er_client.cl_loops == _LE_TRUE ) er_client_loops_update();

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

        /* Delete client */
        er_client_delete();

        /* Return to system */
        return( EXIT_SUCCESS );

    }

/*
    source - loop methods
 */

    le_void_t er_client_loops_render( le_void_t ) {

        /* Ranges management */
        er_client_calls_range();

        /* Clear color and depth buffers */
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        /* Projection : model */
        er_client_proj_model( glutGet( GLUT_SCREEN_WIDTH ), glutGet( GLUT_SCREEN_HEIGHT ) );

        /* Matrix - earth */
        glPushMatrix(); {

            /* Display earth */
            er_model_display_earth( er_client.cl_vlon, er_client.cl_vlat, er_client.cl_valt, er_client.cl_vazm, er_client.cl_vgam );

        } glPopMatrix();

        /* Matrix - cells */
        glPushMatrix(); {

            /* Display cells */
            er_model_display_cell( & ( er_client.cl_model ), er_client.cl_vlon, er_client.cl_vlat, er_client.cl_valt, er_client.cl_vazm, er_client.cl_vgam );

        } glPopMatrix();

        /* Projection : interface */
        er_client_proj_interface( glutGet( GLUT_SCREEN_WIDTH ), glutGet( GLUT_SCREEN_HEIGHT ) );

        /* Matrix - interface */
        glPushMatrix(); {

            /* Display interface */
            er_times_display( & er_client.cl_times );

        } glPopMatrix();

        /* Swap buffers */
        glutSwapBuffers();

    }

    le_void_t er_client_loops_update( le_void_t ) {

        /* Address variables */
        le_address_t er_enum = LE_ADDRESS_C;

        /* Time variables */
        le_time_t er_etime = _LE_TIME_NULL;

        /* Query model update necessities */
        le_enum_t er_model_flag = er_model_get_update( & er_client.cl_model, er_client.cl_vlon, er_client.cl_vlat, er_client.cl_valt );

        /* Query times update necessities */
        le_enum_t er_times_flag = er_times_get_update( & er_client.cl_times );

        /* Check model update necessities */
        if ( ( er_model_flag == _LE_TRUE ) || ( er_times_flag == _LE_TRUE ) ) {

            /* Model times enumeration */
            while ( ( er_etime = er_times_get( & er_client.cl_times ) ) != _LE_TIME_NULL ) {

                /* Reset address size */
                le_address_set_size( & er_enum, 0 );

                /* Reset address time */
                le_address_set_time( & er_enum, er_etime );

                /* Update model cells */
                er_model_set_update_cell( & er_client.cl_model, & er_enum, er_client.cl_vlon * ER_D2R, er_client.cl_vlat * ER_D2R, er_client.cl_valt );

            }

            /* Server queries */
            er_model_set_update_query( & er_client.cl_model );

            /* Terminate model update */
            er_model_set_update_terminate( & er_client.cl_model );

        /* Delaying updates */
        } usleep( 500 );

    }

/*
    source - projection methods
 */

    le_void_t er_client_proj_model( int er_width, int er_height ) {

        /* Clipping plane variables */
        le_real_t er_neac = er_geodesy_near( er_client.cl_valt );
        le_real_t er_farc = er_geodesy_far ( er_client.cl_valt );

        /* Compute model scale factor */
        er_client.cl_vscl = er_geodesy_scale( er_client.cl_valt );

        /* Matrix mode to projection */
        glMatrixMode( GL_PROJECTION );

        /* Set matrix to identity */
        glLoadIdentity();

        /* Compute projection matrix */
        gluPerspective( 45.0, ( double ) er_width / er_height, er_neac, er_farc );

        /* Matrix mode to modelview */
        glMatrixMode( GL_MODELVIEW );

        /* Set matrix to identity */
        glLoadIdentity();

        /* Apply scale factor to projection matrix */
        glScaled( er_client.cl_vscl, er_client.cl_vscl, er_client.cl_vscl );

        /* Adapt and enable fog feature */
        glFogf( GL_FOG_START, er_farc * 0.85 ), glFogf( GL_FOG_END, er_farc ), glEnable( GL_FOG );

    }

    le_void_t er_client_proj_interface( int er_width, int er_height ) {

        /* Matrix mode to projection */
        glMatrixMode( GL_PROJECTION );

        /* Set matrix to identity */
        glLoadIdentity();

        /* Compute projection matrix */
        glOrtho( 0, glutGet( GLUT_SCREEN_WIDTH ), 0, glutGet( GLUT_SCREEN_HEIGHT ), -1.0, 1.0 );

        /* Matrix mode to modelview */
        glMatrixMode( GL_MODELVIEW );

        /* Set matrix to identity */
        glLoadIdentity();

        /* Disable fog feature */
        glDisable( GL_FOG );

    }

/*
    source - callback methods
 */

    le_void_t er_client_calls_reshape( int er_width, int er_height ) {

        /* Configurate viewport */
        glViewport( 0, 0, er_width, er_height );

    }

    le_void_t er_client_calls_keybd( unsigned char er_keycode, int er_x, int er_y ) {

        /* Switch on keycode */
        switch( er_keycode ) {

            /* Interrupt engine loops */
            case ( 0x1b ) : { glutLeaveMainLoop(), er_client.cl_loops = _LE_FALSE; } break;

            /* Update point size */
            case ( 0x31 ) :
            case ( 0x32 ) :
            case ( 0x33 ) : 
            case ( 0x34 ) : { glPointSize( er_keycode - 0x30 ); } break;

            /* Update point of view */
            case ( 0x63 ) : { er_client.cl_valt = ER_ERD; }
            case ( 0x78 ) : { er_client.cl_vazm = 0.0; }
            case ( 0x79 ) : { er_client.cl_vgam = 0.0; } break;

            /* Update point of view */
            case ( 0x61 ) : { er_times_set( & er_client.cl_times, 0 ); } break;
            case ( 0x73 ) : { er_times_set( & er_client.cl_times, 1 ); } break;

            /* Update point of view */
            case ( 0x71 ) : { er_times_set_unset( & er_client.cl_times, 0 ); } break;
            case ( 0x77 ) : { er_times_set_unset( & er_client.cl_times, 1 ); } break;

        };

    }

    le_void_t er_client_calls_mouse( int er_button, int er_state, int er_x, int er_y ) {

        /* Keyboard modifiers variables */
        le_enum_t er_modifiers = glutGetModifiers();

        /* Check mouse state */
        if ( ( er_client.cl_state = er_state ) != GLUT_DOWN ) return;

        /* Update client interface */
        er_client.cl_button = er_button;
        er_client.cl_x      = er_x;
        er_client.cl_y      = er_y;

        /* Compute inertial factor */
        er_client.cl_inertia = abs( er_client.cl_valt - ER_ERA ) * ER_INB;

        /* Clamp inertial value */
        if ( er_client.cl_inertia < 5.0 ) er_client.cl_inertia = 5.0;

        /* Inertial multiplier application */
        if ( er_modifiers == GLUT_ACTIVE_CTRL  ) er_client.cl_inertia *= ER_IMU;
        if ( er_modifiers == GLUT_ACTIVE_SHIFT ) er_client.cl_inertia *= ER_IML;

        /* Interface switch */
        if ( er_modifiers == ( GLUT_ACTIVE_CTRL | GLUT_ACTIVE_ALT ) ) {

            /* Mouse event switch - update time zoom */
            if ( er_client.cl_button == 3 ) er_times_set_zoom( & er_client.cl_times, 1.0990 );
            if ( er_client.cl_button == 4 ) er_times_set_zoom( & er_client.cl_times, 0.9099 );

        } else
        if ( er_modifiers == GLUT_ACTIVE_ALT ) {

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

        /* Range clamping - cyclic */
        er_client.cl_vlon = lc_angle( er_client.cl_vlon );
        er_client.cl_vazm = lc_angle( er_client.cl_vazm );

        /* Range clamping - clamped */
        er_client.cl_vlat = lc_clamp( er_client.cl_vlat,  -90.0,  +90.0 );
        er_client.cl_vgam = lc_clamp( er_client.cl_vgam, -120.0, +120.0 );
        er_client.cl_valt = lc_clamp( er_client.cl_valt, ER_ERL, ER_ERU );

    }

/*
    source - auxiliary methods
 */

    # ifndef __OPENMP__
    void * er_client_pthread( void * er_null ) {

        /* Model update thread - pthread specific derivation */
        while ( er_client.cl_loops == _LE_TRUE ) er_client_loops_update();

        /* Return null pointer */
        return( NULL );

    }
    # endif

