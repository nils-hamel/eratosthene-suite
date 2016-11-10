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

    er_client_t er_client_create( le_char_t * const er_ip, le_sock_t const er_port ) {

        /* created structure variables */
        er_client_t er_client = ER_CLIENT_C;

        /* create client model */
        if ( ( er_client.cl_model = er_model_create( er_ip, er_port ) )._status == _LE_FALSE ) {

            /* return created structure */
            return( er_client._status = _LE_FALSE, er_client );

        }

        /* create client times */
        if ( ( er_client.cl_times = er_times_create( er_ip, er_port ) )._status == _LE_FALSE ) {

            /* return created structure */
            return( er_client._status = _LE_FALSE, er_client );

        }

        /* create client movie */
        if ( ( er_client.cl_movie = er_movie_create() )._status == _LE_FALSE ) {

            /* return created structure */
            return( er_client._status = _LE_FALSE, er_client );

        }

        /* return created structure */
        return( er_client );

    }

    le_void_t er_client_delete( er_client_t * const er_client ) {

        /* Cleared structure variables */
        er_client_t er_reset = ER_CLIENT_C;

        /* delete client movie */
        er_movie_delete( & er_client->cl_movie );

        /* delete client times */
        er_times_delete( & er_client->cl_times );

        /* delete client model */
        er_model_delete( & er_client->cl_model );

        /* delete client structure */
        ( * er_client ) = er_reset;

    }

/*
    source - main method/function
 */

    int main( int argc, char ** argv ) {

        /* server address variables */
        le_char_t * le_svip = ( le_char_t * ) lc_read_string( argc, argv, "--ip"  , "-i" );

        /* server port variables */
        le_sock_t le_port = ( le_sock_t ) lc_read_signed( argc, argv, "--port", "-t", _LE_USE_PORT );

        /* color array variables */
        float er_color[4] = { 0.02, 0.04, 0.06, 0.00 };

        /* initialise gl/glu/glut */
        glutInit( & argc, argv );

        /* initialise display mode */
        glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );

        /* create rendering window */
        glutCreateWindow( "eratosthene-client" );

        /* rendering window configuration */
        glutFullScreen();

        /* cursor configuration */
        glutSetCursor( GLUT_CURSOR_NONE );

        /* graphical thread behavior configuration */
        glutSetOption( GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION );

        /* color buffer clear values */
        glClearColor( er_color[0], er_color[1], er_color[2], er_color[3] );

        /* depth buffer clear values */
        glClearDepth( 1.0 );

        /* opengl features configuration */
        glEnable( GL_DEPTH_TEST );
        glEnable( GL_BLEND      );

        /* opengl fog configuration */
        glFogf ( GL_FOG_MODE   , GL_LINEAR );
        glFogf ( GL_FOG_DENSITY, 0.3       );
        glFogfv( GL_FOG_COLOR  , er_color  );
        
        /* opengl blending configuration */
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        /* declare client callback functions */
        glutKeyboardFunc     ( er_client_calls_keybd   );
        glutReshapeFunc      ( er_client_calls_reshape );
        glutMouseFunc        ( er_client_calls_mouse   );
        glutMotionFunc       ( er_client_calls_move    );
        glutPassiveMotionFunc( er_client_calls_move    );

        /* enable vertex and color arrays */
        glEnableClientState( GL_VERTEX_ARRAY );
        glEnableClientState( GL_COLOR_ARRAY  );

        /* create client */
        if ( ( er_client = er_client_create( le_svip, le_port ) )._status == _LE_FALSE ) {

            /* display message */
            fprintf( stderr, "eratosthene-suite : error : unable to create client\n" );

            /* send message */
            return( EXIT_FAILURE );

        }

        /* principale execution loop */
        while ( er_client.cl_loops != ER_COMMON_EXIT ) {

            /* switch on execution mode */
            if ( er_client.cl_loops == ER_COMMON_VIEW ) {

                # pragma omp parallel sections
                {

                /* model display procedure - master thread */
                # pragma omp section
                while ( er_client.cl_loops == ER_COMMON_VIEW ) { 

                    /* interface event procedure */
                    glutMainLoopEvent();

                    /* model display procedure */
                    er_client_loops_render();

                    /* swap buffers */
                    glutSwapBuffers();

                }

                /* model update procedure - secondary thread */
                # pragma omp section
                while ( er_client.cl_loops == ER_COMMON_VIEW ) { 

                    /* model update procedure */
                    er_client_loops_update();

                }

                }

            } else if ( er_client.cl_loops == ER_COMMON_MOVIE ) {

                /* movie computation procedure - master thread */
                while ( er_client.cl_loops == ER_COMMON_MOVIE ) {

                    /* motion update procedure */
                    er_client_loops_movie();

                    /* model update procedure */
                    er_client_loops_update();

                    /* model display procedure */
                    er_client_loops_render();

                    /* movie procedure */
                    er_client.cl_loops = er_movie( & er_client.cl_movie );

                    /* swap buffers */
                    glutSwapBuffers();

                }

            }

        }

        /* delete client */
        er_client_delete( & er_client );

        /* uninitialise glut */
        glutFinish();

        /* send message */
        return( EXIT_SUCCESS );

    }

/*
    source - loop methods
 */

    le_void_t er_client_loops_render( le_void_t ) {

        /* ranges management */
        er_client_calls_range();

        /* clear color and depth buffers */
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        /* projection : model */
        er_client_proj_model( glutGet( GLUT_SCREEN_WIDTH ), glutGet( GLUT_SCREEN_HEIGHT ) );

        /* matrix - earth */
        glPushMatrix(); {

            /* display earth */
            er_model_display_earth( er_client.cl_vlon, er_client.cl_vlat, er_client.cl_valt, er_client.cl_vazm, er_client.cl_vgam );

        } glPopMatrix();

        /* matrix - cells */
        glPushMatrix(); {

            /* display cells */
            er_model_display_cell( & ( er_client.cl_model ), er_client.cl_vlon, er_client.cl_vlat, er_client.cl_valt, er_client.cl_vazm, er_client.cl_vgam );

        } glPopMatrix();

        /* check execution mode */
        if ( er_client.cl_loops == ER_COMMON_MOVIE ) return;

        /* projection : interface */
        er_client_proj_interface( glutGet( GLUT_SCREEN_WIDTH ), glutGet( GLUT_SCREEN_HEIGHT ) );

        /* matrix - interface */
        glPushMatrix(); {

            /* display interface */
            er_times_display( & er_client.cl_times );

        } glPopMatrix();

    }

    le_void_t er_client_loops_update( le_void_t ) {

        /* address variables */
        le_address_t er_enum = LE_ADDRESS_C;

        /* query model update necessities */
        le_enum_t er_model_flag = er_model_get_update( & er_client.cl_model, er_client.cl_vlon, er_client.cl_vlat, er_client.cl_valt );

        /* query times update necessities */
        le_enum_t er_times_flag = er_times_get_update( & er_client.cl_times );

        /* check model update necessities */
        if ( ( er_model_flag == _LE_TRUE ) || ( er_times_flag == _LE_TRUE ) ) {

            /* compose query times */
            er_enum = er_times_get( & er_client.cl_times );

            /* update model cells */
            er_model_set_update_cell( & er_client.cl_model, & er_enum, er_client.cl_vlon * ER_D2R, er_client.cl_vlat * ER_D2R, er_client.cl_valt );

            /* server queries */
            er_model_set_update_query( & er_client.cl_model );

            /* terminate model update */
            er_model_set_update_terminate( & er_client.cl_model );

        /* delaying updates */
        } usleep( 500 );

    }

    le_void_t er_client_loops_movie( le_void_t ) {

        /* compute point of view position */
        er_client.cl_vlon = er_movie_get_value( & er_client.cl_movie, 0 );
        er_client.cl_vlat = er_movie_get_value( & er_client.cl_movie, 1 );
        er_client.cl_valt = er_movie_get_value( & er_client.cl_movie, 2 );
        er_client.cl_vazm = er_movie_get_value( & er_client.cl_movie, 3 );
        er_client.cl_vgam = er_movie_get_value( & er_client.cl_movie, 4 );

    }

/*
    source - projection methods
 */

    le_void_t er_client_proj_model( int er_width, int er_height ) {

        /* clipping plane variables */
        le_real_t er_neac = er_geodesy_near( er_client.cl_valt );
        le_real_t er_farc = er_geodesy_far ( er_client.cl_valt );

        /* compute model scale factor */
        er_client.cl_vscl = er_geodesy_scale( er_client.cl_valt );

        /* matrix mode to projection */
        glMatrixMode( GL_PROJECTION );

        /* set matrix to identity */
        glLoadIdentity();

        /* compute projection matrix */
        gluPerspective( 45.0, ( double ) er_width / er_height, er_neac, er_farc );

        /* matrix mode to modelview */
        glMatrixMode( GL_MODELVIEW );

        /* set matrix to identity */
        glLoadIdentity();

        /* apply scale factor to projection matrix */
        glScaled( er_client.cl_vscl, er_client.cl_vscl, er_client.cl_vscl );

        /* adapt and enable fog feature */
        glFogf( GL_FOG_START, er_farc * 0.85 ), glFogf( GL_FOG_END, er_farc ), glEnable( GL_FOG );

    }

    le_void_t er_client_proj_interface( int er_width, int er_height ) {

        /* matrix mode to projection */
        glMatrixMode( GL_PROJECTION );

        /* set matrix to identity */
        glLoadIdentity();

        /* compute projection matrix */
        glOrtho( 0, glutGet( GLUT_SCREEN_WIDTH ), 0, glutGet( GLUT_SCREEN_HEIGHT ), -1.0, 1.0 );

        /* matrix mode to modelview */
        glMatrixMode( GL_MODELVIEW );

        /* set matrix to identity */
        glLoadIdentity();

        /* disable fog feature */
        glDisable( GL_FOG );

    }

/*
    source - callback methods
 */

    le_void_t er_client_calls_reshape( int er_width, int er_height ) {

        /* configurate viewport */
        glViewport( 0, 0, er_width, er_height );

    }

    le_void_t er_client_calls_keybd( unsigned char er_keycode, int er_x, int er_y ) {

        /* switch on keycode */
        switch( er_keycode ) {

            /* execution mode - exit */
            case ( 0x1b ) : { er_client.cl_loops = ER_COMMON_EXIT; } break;

            /* execution mode - movie */
            case ( 0x70 ) : { er_client.cl_loops = ER_COMMON_MOVIE; } break;

            /* update point size */
            case ( 0x31 ) :
            case ( 0x32 ) :
            case ( 0x33 ) : 
            case ( 0x34 ) : { glPointSize( er_keycode - 0x30 ); } break;

            /* update point of view */
            case ( 0x63 ) : { er_client.cl_valt = ER_ERD; }
            case ( 0x78 ) : { er_client.cl_vazm = 0.0; }
            case ( 0x79 ) : { er_client.cl_vgam = 0.0; } break;

            /* update point of view */
            case ( 0x61 ) : { er_times_set( & er_client.cl_times, 0 ); } break;
            case ( 0x73 ) : { er_times_set( & er_client.cl_times, 1 ); } break;

            /* update point of view */
            case ( 0x71 ) : { er_times_set_reset( & er_client.cl_times, 0 ); } break;
            case ( 0x77 ) : { er_times_set_reset( & er_client.cl_times, 1 ); } break;

            /* update movie stack */
            case ( 0x69 ) : { er_movie_set_empty( & er_client.cl_movie ); } break;
            case ( 0x6f ) : { er_movie_set_point( & er_client.cl_movie, er_client.cl_vlon, er_client.cl_vlat, er_client.cl_valt, er_client.cl_vazm, er_client.cl_vgam ); } break;

        };

    }

    le_void_t er_client_calls_mouse( int er_button, int er_state, int er_x, int er_y ) {

        /* keyboard modifiers variables */
        le_enum_t er_modifiers = glutGetModifiers();

        /* check mouse state */
        if ( ( er_client.cl_state = er_state ) != GLUT_DOWN ) return;

        /* update client interface */
        er_client.cl_button = er_button;
        er_client.cl_x      = er_x;
        er_client.cl_y      = er_y;

        /* compute inertial factor */
        er_client.cl_inertia = abs( er_client.cl_valt - ER_ERA ) * ER_INB;

        /* clamp inertial value */
        if ( er_client.cl_inertia < 5.0 ) er_client.cl_inertia = 5.0;

        /* inertial multiplier application */
        if ( er_modifiers == GLUT_ACTIVE_CTRL  ) er_client.cl_inertia *= ER_IMU;
        if ( er_modifiers == GLUT_ACTIVE_SHIFT ) er_client.cl_inertia *= ER_IML;

        /* interface switch */
        if ( er_modifiers == ( GLUT_ACTIVE_CTRL | GLUT_ACTIVE_ALT ) ) {

            /* mouse event switch - update time zoom */
            if ( er_client.cl_button == 3 ) er_times_set_zoom( & er_client.cl_times, 1.0990 );
            if ( er_client.cl_button == 4 ) er_times_set_zoom( & er_client.cl_times, 0.9099 );

        } else
        if ( er_modifiers == GLUT_ACTIVE_ALT ) {

            /* mouse event switch - update time position */
            if ( er_client.cl_button == 3 ) er_times_set_pose( & er_client.cl_times, + 0.02 );
            if ( er_client.cl_button == 4 ) er_times_set_pose( & er_client.cl_times, - 0.02 );

        } else {

            /* mouse event switch - update altitude */
            if ( er_client.cl_button == 3 ) er_client.cl_valt += er_client.cl_inertia;
            if ( er_client.cl_button == 4 ) er_client.cl_valt -= er_client.cl_inertia;

        }

    }

    le_void_t er_client_calls_move( int er_x, int er_y ) {

        /* check mouse state */
        if ( er_client.cl_state != GLUT_DOWN ) return;

        /* mouse event switch */
        if ( er_client.cl_button == GLUT_LEFT_BUTTON ) {

            /* update longitude and latitude */
            er_client.cl_vlon += ( er_y - er_client.cl_y ) * ( ER_INT * er_client.cl_inertia ) * sin( er_client.cl_vazm * ER_D2R );
            er_client.cl_vlat += ( er_y - er_client.cl_y ) * ( ER_INT * er_client.cl_inertia ) * cos( er_client.cl_vazm * ER_D2R );
            er_client.cl_vlon -= ( er_x - er_client.cl_x ) * ( ER_INT * er_client.cl_inertia ) * cos( er_client.cl_vazm * ER_D2R );
            er_client.cl_vlat += ( er_x - er_client.cl_x ) * ( ER_INT * er_client.cl_inertia ) * sin( er_client.cl_vazm * ER_D2R );

        } else 
        if ( er_client.cl_button == GLUT_RIGHT_BUTTON ) {

            /* update azimuth and gamma */
            er_client.cl_vazm -= ( er_x - er_client.cl_x ) * ER_INR;
            er_client.cl_vgam += ( er_y - er_client.cl_y ) * ER_INR;

        }

    }

    le_void_t er_client_calls_range() {

        /* range clamping - cyclic */
        er_client.cl_vlon = lc_angle( er_client.cl_vlon );
        er_client.cl_vazm = lc_angle( er_client.cl_vazm );

        /* range clamping - clamped */
        er_client.cl_vlat = lc_clamp( er_client.cl_vlat,  -90.0,  +90.0 );
        er_client.cl_vgam = lc_clamp( er_client.cl_vgam, -120.0, +120.0 );
        er_client.cl_valt = lc_clamp( er_client.cl_valt, ER_ERL, ER_ERU );

    }

/*
    source - stability methods
 */

    le_void_t glutFinish( le_void_t ) { glutIdleFunc( glutLeaveMainLoop ), glutMainLoop(); }

