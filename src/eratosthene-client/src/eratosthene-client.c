/*
 *  eratosthene-suite - client
 *
 *      Nils Hamel - nils.hamel@bluewin.ch
 *      Copyright (c) 2016-2017 EPFL CDH DHLAB
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

        /* array variables */
        le_array_t er_array = LE_ARRAY_C;

        /* agreement variables */
        le_size_t er_agree = LE_AUTH_QUER;

        /* server configuration variables */
        le_size_t er_space = 0;
        le_time_t er_times = 0;

        /* serialisation variables */
        le_size_t er_head = 0;

        /* created structure variables */
        er_client_t er_client = ER_CLIENT_C;

        /* create client socket */
        if ( ( er_client.cl_socket = le_client_create( er_ip, er_port ) ) == _LE_SOCK_NULL ) {

            /* return created structure */
            return( er_client._status = _LE_FALSE, er_client );

        }

        /* socket-array size */
        le_array_set_size( & er_array, sizeof( le_size_t ) );

        /* compose socket-array  */
        le_array_serial( & er_array, & er_agree, sizeof( le_size_t ), 0, _LE_SET );

        /* write socket-array - agreement */
        le_array_io_write( & er_array, LE_MODE_AUTH, er_client.cl_socket );

        /* read socket-array */
        if ( le_array_io_read( & er_array, er_client.cl_socket ) != LE_MODE_AUTH ) {

            /* delete client socket */
            le_client_delete( er_client.cl_socket );

            /* return created structure */
            return( er_client._status = _LE_FALSE, er_client );

        }

        /* check array consistency */
        if ( le_array_get_size( & er_array ) != LE_ARRAY_AUTH ) {

            /* delete client socket */
            le_client_delete( er_client.cl_socket );

            /* return created structure */
            return( er_client._status = _LE_FALSE, er_client );

        }

        /* decompose socket-array */
        er_head = le_array_serial( & er_array, & er_agree, sizeof( le_size_t ), er_head, _LE_GET );
        er_head = le_array_serial( & er_array, & er_space, sizeof( le_size_t ), er_head, _LE_GET );
        er_head = le_array_serial( & er_array, & er_times, sizeof( le_time_t ), er_head, _LE_GET );

        /* check agreement value */
        if ( er_agree != LE_AUTH_AUTH ) {

            /* delete client socket */
            le_client_delete( er_client.cl_socket );

            /* return created structure */
            return( er_client._status = _LE_FALSE, er_client );

        }

        /* create client model */
        if ( ( er_client.cl_model = er_model_create_( er_client.cl_socket, er_space, er_times ) )._status == _LE_FALSE ) {

            /* delete client socket */
            le_client_delete( er_client.cl_socket );

            /* return created structure */
            return( er_client._status = _LE_FALSE, er_client );

        }

        /* create client times */
        if ( ( er_client.cl_times = er_times_create() )._status == _LE_FALSE ) {

            /* delete client socket */
            le_client_delete( er_client.cl_socket );

            /* return created structure */
            return( er_client._status = _LE_FALSE, er_client );

        }

        /* create client movie */
        if ( ( er_client.cl_movie = er_movie_create() )._status == _LE_FALSE ) {

            /* delete client socket */
            le_client_delete( er_client.cl_socket );

            /* return created structure */
            return( er_client._status = _LE_FALSE, er_client );

        }

        /* return created structure */
        return( er_client );

    }

    le_void_t er_client_delete( er_client_t * const er_client ) {

        /* socket-array variables */
        le_array_t er_array = LE_ARRAY_C;

        /* deleted structure variables */
        er_client_t er_delete = ER_CLIENT_C;

        /* delete client movie */
        er_movie_delete( & er_client->cl_movie );

        /* delete client times */
        er_times_delete( & er_client->cl_times );

        /* delete client model */
        //er_model_delete( & er_client->cl_model );
        er_model_delete_( & er_client->cl_model );

        /* socket-array size */
        le_array_set_size( & er_array, 0 );

        /* write socket-array - resiliation */
        le_array_io_write( & er_array, LE_MODE_RESI, er_client->cl_socket );

        /* delete client socket */
        le_client_delete( er_client->cl_socket );

        /* delete structure */
        ( * er_client ) = er_delete;

    }

/*
    source - main method
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
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

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

        /* execution threads */
        # pragma omp parallel sections
        {

        /* master thread - display procedure */
        # pragma omp section
        while ( er_client.cl_loops != ER_COMMON_EXIT ) {

            /* switch on execution mode */
            if ( er_client.cl_loops == ER_COMMON_VIEW ) {

                /* principale execution loop */
                while ( er_client.cl_loops == ER_COMMON_VIEW ) {

                    /* interface events procedure */
                    glutMainLoopEvent();

                    /* model display procedure */
                    er_client_loops_render_();

                    /* swap buffers */
                    glutSwapBuffers();

                }

            } else if ( er_client.cl_loops == ER_COMMON_MOVIE ) {

                /* principale execution loop */
                while ( er_client.cl_loops == ER_COMMON_MOVIE ) {

                    /* motion management procedure */
                    er_client.cl_view = er_movie_get( & er_client.cl_movie );

                    /* model update procedure */
                    er_client_loops_update_();

                    /* model display procedure */
                    er_client_loops_render_();

                    /* movie procedure */
                    er_client.cl_loops = er_movie( & er_client.cl_movie );

                    /* swap buffers */
                    glutSwapBuffers();

                }

            /* thread idle mechanism */
            } else { sleep( 1 ); }

        }

        /* secondary thread - model procedure */
        # pragma omp section
        while ( er_client.cl_loops != ER_COMMON_EXIT ) {

            /* switch on execution mode */
            if ( er_client.cl_loops == ER_COMMON_VIEW ) {

                /* principale execution loop */
                while ( er_client.cl_loops == ER_COMMON_VIEW ) {

                    /* model update procedure */
                    //er_client_loops_update();
                    er_client_loops_update_();

                }

            /* thread idle mechanism */
            } else { sleep( 1 ); }

        }

        }

        /* delete client */
        er_client_delete( & er_client );

        /* uninitialise glut - see stability methods */
        glutFinish();

        /* send message */
        return( EXIT_SUCCESS );

    }

/*
    source - loop methods
 */

    le_void_t er_client_loops_render_( le_void_t ) {

        /* clear color and depth buffers */
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        /* projection : model */
        er_client_proj_model( glutGet( GLUT_SCREEN_WIDTH ), glutGet( GLUT_SCREEN_HEIGHT ) );

        /* matrix - cells */
        glPushMatrix(); {

            /* display cells */
            er_model_display_cell_( & er_client.cl_model, & er_client.cl_view );

        } glPopMatrix();

        /* matrix - earth */
        glPushMatrix(); {

            /* display earth */
            er_model_display_earth( & er_client.cl_view );

        } glPopMatrix();

        /* projection : interface */
        er_client_proj_interface( glutGet( GLUT_SCREEN_WIDTH ), glutGet( GLUT_SCREEN_HEIGHT ) );

        /* matrix - interface */
        glPushMatrix(); {

            /* display interface */
            er_times_display( & er_client.cl_times, & er_client.cl_view );

        } glPopMatrix();

    }

    le_void_t er_client_loops_update_( le_void_t ) {

        /* address variables */
        le_address_t er_enum = LE_ADDRESS_C;

        /* motion detection */
        if ( er_view_get_equal( & er_client.cl_push, & er_client.cl_view ) == _LE_FALSE ) {

            /* retreive address times */
            er_enum = er_view_get_times( & er_client.cl_view );

            /* prepare model update */
            er_model_set_prep_( & er_client.cl_model );

            /* update model target */
            er_model_set_enum_( & er_client.cl_model, & er_enum, 0, & er_client.cl_view );

            /* model/target fast synchronisation */
            er_model_set_fast_( & er_client.cl_model );

            /* push considered view */
            er_client.cl_push = er_client.cl_view;

        }

        /* synchronisation process */
        er_model_set_sync_( & er_client.cl_model );

    }

/*
    source - projection methods
 */

    le_void_t er_client_proj_model( int er_width, int er_height ) {

        /* clipping plane variables */
        le_real_t er_neac = er_geodesy_near( er_view_get_alt( & er_client.cl_view ) );
        le_real_t er_farc = er_geodesy_far ( er_view_get_alt( & er_client.cl_view ) );

        /* compute model scale factor */
        er_client.cl_scale = er_geodesy_scale( er_view_get_alt( & er_client.cl_view ) );

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
        glScaled( er_client.cl_scale, er_client.cl_scale, er_client.cl_scale );

        /* fog configuration */
        glFogf( GL_FOG_START, er_farc * 0.8 ), glFogf( GL_FOG_END, er_farc );

        /* enable fog features */
        glEnable( GL_FOG );

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

            /* execution mode - key [esc] */
            case ( 0x1b ) : {

                /* update execution mode */
                er_client.cl_loops = ER_COMMON_EXIT;

            } break;

            /* execution mode - key [p] */
            case ( 0x70 ) : {

                /* update execution mode */
                er_client.cl_loops = ER_COMMON_MOVIE;

            } break;

            /* graphical options - key [1-4] */
            case ( 0x31 ) :
            case ( 0x32 ) :
            case ( 0x33 ) :
            case ( 0x34 ) : {

                /* update opengl point size */
                glPointSize( er_keycode - 0x30 );

            } break;

            /* time management - key [TAB] */
            case ( 0x09 ) : {

                /* swap active time */
                er_view_set_swap( & er_client.cl_view );

            } break;

            /* time management - key [SPACE] */
            case ( 0x20 ) : {

                /* align times */
                er_view_set_times( & er_client.cl_view );

            } break;

            /* mode management - key [q] */
            case ( 0x71 ) : {

                /* update mode */
                er_view_set_mode( & er_client.cl_view, 1 );

            } break;

            /* mode management - key [w] */
            case ( 0x77 ) : {

                /* update mode */
                er_view_set_mode( & er_client.cl_view, 2 );

            } break;

            /* mode management - key [e] */
            case ( 0x65 ) : {

                /* update mode */
                er_view_set_mode( & er_client.cl_view, 3 );

            } break;

            /* mode management - key [r] */
            case ( 0x72 ) : {

                /* update mode */
                er_view_set_mode( & er_client.cl_view, 4 );

            } break;

            /* mode management - key [t] */
            case ( 0x74 ) : {

                /* update mode */
                er_view_set_mode( & er_client.cl_view, 5 );

            } break;

            /* movie creation - key [i] */
            case ( 0x69 ) : {

                /* unstacking control points */
                er_movie_set_clear( & er_client.cl_movie );

            } break;

            /* movie creation - key [o] */
            case ( 0x6f ) : {

                /* stack view as control point */
                er_movie_set( & er_client.cl_movie, & er_client.cl_view );

            } break;

        };

    }

    le_void_t er_client_calls_mouse( int er_button, int er_state, int er_x, int er_y ) {

        /* switch on button */
        if ( ( er_button != ER_COMMON_WUP ) && ( er_button != ER_COMMON_WDOWN ) ) {

            /* assign state */
            er_client.cl_state = er_state;

            /* assign button */
            er_client.cl_button = er_button;

            /* assign position */
            er_client.cl_x = er_x;
            er_client.cl_y = er_y;

        }

        /* check state - return */
        if ( er_state != GLUT_DOWN ) return;

        /* compute inertial coefficient */
        er_client.cl_inertia = abs( er_view_get_alt( & er_client.cl_view ) - LE_ADDRESS_WGSA ) * ER_COMMON_INE;

        /* clamp inertial coefficient */
        if ( er_client.cl_inertia < 5.0 ) er_client.cl_inertia = 5.0;

        /* switch on modifiers - apply inertial coefficient */
        if ( glutGetModifiers() == GLUT_ACTIVE_CTRL  ) er_client.cl_inertia *= ER_COMMON_IMU;
        if ( glutGetModifiers() == GLUT_ACTIVE_SHIFT ) er_client.cl_inertia *= ER_COMMON_IML;

        /* switch on modifiers */
        if ( glutGetModifiers() == ( GLUT_ACTIVE_CTRL | GLUT_ACTIVE_ALT ) ) {

            /* switch on button - update time zoom */
            if ( er_button == ER_COMMON_WUP   ) er_view_set_area( & er_client.cl_view, 1.0990 );
            if ( er_button == ER_COMMON_WDOWN ) er_view_set_area( & er_client.cl_view, 0.9099 );

        } else if ( glutGetModifiers() == GLUT_ACTIVE_ALT ) {

            /* switch on button - update time position */
            if ( er_button == ER_COMMON_WUP   ) er_view_set_time( & er_client.cl_view, + 0.02 );
            if ( er_button == ER_COMMON_WDOWN ) er_view_set_time( & er_client.cl_view, - 0.02 );

        } else {

            /* switch on button - update altitude */
            if ( er_button == ER_COMMON_WUP   ) er_view_set_alt( & er_client.cl_view, + er_client.cl_inertia );
            if ( er_button == ER_COMMON_WDOWN ) er_view_set_alt( & er_client.cl_view, - er_client.cl_inertia );

        }

    }

    le_void_t er_client_calls_move( int er_x, int er_y ) {

        /* displacement variables */
        le_real_t er_mdx = ( le_real_t ) er_x - er_client.cl_x;
        le_real_t er_mdy = ( le_real_t ) er_y - er_client.cl_y;

        /* check state - return */
        if ( er_client.cl_state != GLUT_DOWN ) return;

        /* switch on button */
        if ( er_client.cl_button == GLUT_LEFT_BUTTON ) {

            /* apply inertial coefficients and multipliers */
            er_mdx *= ER_COMMON_INP * er_client.cl_inertia;
            er_mdy *= ER_COMMON_INP * er_client.cl_inertia;

            /* update longitude and latitude */
            er_view_set_plan( & er_client.cl_view, er_mdx, er_mdy );

        } else if ( er_client.cl_button == GLUT_RIGHT_BUTTON ) {

            /* apply inetrial coefficients and multipliers */
            er_mdx *= ER_COMMON_INR;
            er_mdy *= ER_COMMON_INR;

            /* update azimuth and gamma */
            er_view_set_azm( & er_client.cl_view, - er_mdx );
            er_view_set_gam( & er_client.cl_view, + er_mdy );

        }

    }

/*
    source - stability methods
 */

    le_void_t glutFinish( le_void_t ) { glutIdleFunc( glutLeaveMainLoop ), glutMainLoop(); }

