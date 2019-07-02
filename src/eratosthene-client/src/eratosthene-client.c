/*
 *  eratosthene-suite - client
 *
 *      Nils Hamel - nils.hamel@bluewin.ch
 *      Copyright (c) 2016-2019 DHLAB, EPFL
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
    source - constructor/destructor methods
 */

    er_client_t er_client_create( le_char_t * const er_ip, le_sock_t const er_port, le_size_t const er_width, le_size_t const er_height, le_char_t * const er_export, le_size_t const er_frame ) {

        /* created structure variables */
        er_client_t er_client = ER_CLIENT_I( er_width, er_height );

        /* create client socket */
        if ( ( er_client.cl_socket = le_client_create( er_ip, er_port ) ) == _LE_SOCK_NULL ) {

            /* return created structure */
            return( er_client );

        }

        /* query server configuration */
        if ( er_client_set_server( & er_client ) == _LE_FALSE ) {

            /* delete client structure */
            er_client_delete( & er_client );

            /* return created structure */
            return( er_client );

        }

        /* create client model */
        if ( le_get_status( er_client.cl_model = er_model_create( er_client.cl_socket, er_client.cl_scfg, er_client.cl_tcfg ) ) == _LE_FALSE ) {

            /* delete client structure */
            er_client_delete( & er_client );

            /* return created structure */
            return( er_client );

        }

        /* create client times */
         if ( le_get_status( er_client.cl_times = er_times_create( er_width, er_height ) ) == _LE_FALSE ) {

            /* delete client structure */
            er_client_delete( & er_client );

            /* return created structure */
            return( er_client );

        }

        /* create client video */
        if ( le_get_status( er_client.cl_video = er_video_create( er_export, er_frame, er_width, er_height ) ) == _LE_FALSE ) {

            /* delete client structure */
            er_client_delete( & er_client );

            /* return created structure */
            return( er_client );

        }

        /* return created structure */
        return( le_set_status( er_client, _LE_TRUE ) );

    }

    le_void_t er_client_delete( er_client_t * const er_client ) {

        /* deleted structure variables */
        er_client_t er_delete = ER_CLIENT_C;

        /* delete client video */
        er_video_delete( & er_client->cl_video );

        /* delete client times */
        er_times_delete( & er_client->cl_times );

        /* delete client model */
        er_model_delete( & er_client->cl_model );

        /* delete client socket */
        le_client_delete( er_client->cl_socket );

        /* delete structure */
        ( * er_client ) = er_delete;

    }

/*
    source - mutator methods
 */

    le_enum_t er_client_set_server( er_client_t * const er_client ) {

        /* socket-array variable */
        le_array_t er_array = LE_ARRAY_C;

        /* message variable */
        le_enum_t er_message = _LE_TRUE;

        /* update socket-array size */
        le_array_set_size( & er_array, 0 );

        /* write socket-array */
        if ( le_array_io_write( & er_array, LE_MODE_AUTH, er_client->cl_socket ) != LE_MODE_AUTH ) {

            /* push message */
            er_message = _LE_FALSE;

        } else {

            /* read socket-array */
            if ( le_array_io_read( & er_array, er_client->cl_socket ) != LE_MODE_AUTH ) {

                /* push message */
                er_message = _LE_FALSE;

            } else {

                /* check consistency */
                if ( le_array_get_size( & er_array ) != LE_ARRAY_AUTH ) {

                    /* push message */
                    er_message = _LE_FALSE;

                } else {

                    /* server parameter serialisation */
                    le_array_serial( & er_array, & er_client->cl_scfg, sizeof( le_size_t ), 0, _LE_GET );

                    /* server parameter serialisation */
                    le_array_serial( & er_array, & er_client->cl_scfg, sizeof( le_time_t ), sizeof( le_size_t ), _LE_GET );

                }

            }

        }

        /* delete socket-array */
        le_array_delete( & er_array );

        /* send message */
        return( er_message );

    }

/*
    source - main method
 */

    int main( int argc, char ** argv ) {

        /* display mode variable */
        SDL_DisplayMode er_display;

        /* window handle variable */
        SDL_Window * er_window = NULL;

        /* opengl context variable */
        SDL_GLContext er_context;

        /* server address variables */
        le_char_t * er_svip = ( le_char_t * ) lc_read_string( argc, argv, "--ip"  , "-i" );

        /* server service variables */
        le_sock_t er_port = ( le_sock_t ) lc_read_signed( argc, argv, "--port", "-t", _LE_USE_PORT );

        /* video path variable */
        le_char_t * er_export = ( le_char_t * ) lc_read_string( argc, argv, "--export", "-e" );

        /* video frame variable */
        le_size_t er_frame = ( le_size_t ) lc_read_unsigned( argc, argv, "--frame", "-f", 1800 );

        /* client structure variable */
        er_client_t er_client = ER_CLIENT_C;

        /* messasge variable */
        le_enum_t er_message = EXIT_SUCCESS;

        /* initialise sdl */
        if ( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_EVENTS ) < 0 ) {

            /* display message */
            lc_error( "SDL library initialisation" );

            /* push message */
            er_message = EXIT_FAILURE;

        } else {

            /* retrieve display mode */
            SDL_GetCurrentDisplayMode( 0, & er_display );

            /* hide mouse cursor */
            SDL_ShowCursor( SDL_DISABLE );

            /* color buffer bits */
            SDL_GL_SetAttribute( SDL_GL_RED_SIZE  , 8 );
            SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 );
            SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE , 8 );
            SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, 8 );

            /* depth buffer bits */
            SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 24 );

            /* enable double buffer */
            SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

            /* create window */
            if ( ( er_window = SDL_CreateWindow( LC_PROJECT, 0, 0, er_display.w, er_display.h, ER_SDL_FLAGS ) ) == NULL ) {

                /* display message */
                lc_error( "window creation" );

                /* push message */
                er_message = EXIT_FAILURE;

            } else {

                /* create opengl context */
                if ( ( er_context = SDL_GL_CreateContext( er_window ) ) == NULL ) {

                    /* display message */
                    lc_error( "OpenGL context creation" );

                    /* push message */
                    er_message = EXIT_FAILURE;

                } else {

                    /* create client */
                    if ( ( er_client = er_client_create( er_svip, er_port, er_display.w, er_display.h, er_export, er_frame ) )._status == _LE_FALSE ) {

                        /* display message */
                        lc_error( "client creation" );

                        /* push message */
                        er_message = EXIT_FAILURE;

                    } else {

                        /* principal loop */
                        er_client_loops( & er_client, er_window );

                        /* delete client */
                        er_client_delete( & er_client );

                    }

                    /* delete opengl context */
                    SDL_GL_DeleteContext( er_context );

                }

                /* delete window */
                SDL_DestroyWindow( er_window );

            }

            /* terminate sdl */
            SDL_Quit();

        }

        /* send message */
        return( er_message );

    }

/*
    source - loop methods
 */

    le_void_t er_client_loops(  er_client_t * const er_client, SDL_Window * const er_window ) {

        /* color array variable */
        GLfloat er_vector[4] = { 0.0 };

        /* shade model */
        glShadeModel( GL_SMOOTH );

        /* face culling */
        glCullFace( GL_BACK );

        /* enable culling */
        glEnable( GL_CULL_FACE );

        /* enable depth test */
        glEnable( GL_DEPTH_TEST );

        /* enable light */
        glEnable( GL_LIGHT0 );

        /* face culling */
        glCullFace( GL_BACK );

        /* opengl blending configuration */
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

        /* permanent enable vertex array */
        glEnableClientState( GL_VERTEX_ARRAY );

        /* permanent enable color array */
        glEnableClientState( GL_COLOR_ARRAY );

        /* opengl point size */
        glPointSize( 2.0 );

        /* depth buffer clear values */
        glClearDepth( 1.0 );

        /* assign background color */
        er_vector[0] = 0.03;
        er_vector[1] = 0.04;
        er_vector[2] = 0.05;
        er_vector[3] = 1.00;

        /* color buffer clear values */
        glClearColor( er_vector[0], er_vector[1], er_vector[2], er_vector[3] );

        /* opengl fog configuration */
        glFogf ( GL_FOG_MODE   , GL_LINEAR );
        glFogf ( GL_FOG_DENSITY, 0.3       );
        glFogfv( GL_FOG_COLOR  , er_vector );

        /* assign ambient color */
        er_vector[0] = 0.4;
        er_vector[1] = 0.4;
        er_vector[2] = 0.4;
        er_vector[3] = 1.0;

        /* light ambient color */
        glLightfv( GL_LIGHT0, GL_AMBIENT, er_vector );

        /* assign diffuse color */
        er_vector[0] = 1.0;
        er_vector[1] = 1.0;
        er_vector[2] = 1.0;
        er_vector[3] = 1.0;

        /* light diffuse color */
        glLightfv( GL_LIGHT0, GL_DIFFUSE, er_vector );

        /* assign light direction */
        er_vector[0] = 0.0;
        er_vector[1] = 0.0;
        er_vector[2] = 1.0;
        er_vector[3] = 0.0;

        /* light direction */
        glLightfv( GL_LIGHT0, GL_POSITION, er_vector );

        /* disable dynamic thread */
        omp_set_dynamic( 0 );

        /* execution threads */
        # pragma omp parallel num_threads( 2 )
        {

        /* master swicth switch */
        if ( omp_get_thread_num() == 0 ) {

            /* critical region variable */
            le_enum_t er_loop = _LE_TRUE;

            /* execution loop */
            while ( er_loop == _LE_TRUE ) {

                if ( er_client->cl_loops == ER_COMMON_AUTO ) {

                    // set view point //
                    er_client->cl_view = er_video_get( & er_client->cl_video );

                    // full model update with parameter //
                    er_client_loops_update( er_client, ER_COMMON_SIZE, 0 );

                } else {

                    /* interface events procedure */
                    er_client_loops_event( er_client );

                }

                /* model display procedure */
                er_client_loops_render( er_client );

                /* swap buffers */
                SDL_GL_SwapWindow( er_window );

                if ( er_client->cl_loops == ER_COMMON_AUTO ) {

                    // export //
                    er_client->cl_loops = er_video_set( & er_client->cl_video );

                }

                /* execution mode synchronisation */
                # pragma omp critical
                er_loop = er_client->cl_loops == ER_COMMON_EXIT ? _LE_FALSE : _LE_TRUE;

            }

        } else {

            /* critical region variable */
            le_enum_t er_loop = _LE_TRUE;

            /* execution loop */
            while ( er_loop == _LE_TRUE ) {

                /* switch on execution mode */
                if ( er_client->cl_loops == ER_COMMON_VIEW ) {

                    /* model update procedure */
                    er_client_loops_update( er_client, ER_COMMON_PACK, CLOCKS_PER_SEC >> 2 );

                }

                /* execution mode synchronisation */
                # pragma omp critical
                er_loop = er_client->cl_loops == ER_COMMON_EXIT ? _LE_FALSE : _LE_TRUE;

            }

        }

        } /* openmp */

    }

    le_void_t er_client_loops_render( er_client_t * const er_client ) {

        /* clear color and depth buffers */
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        /* projection : model */
        er_client_proj_model( er_client );

        /* matrix - cells */
        glPushMatrix(); {

            /* display cells */
            er_model_display_cell( & er_client->cl_model, & er_client->cl_view );

        } glPopMatrix();

        /* matrix - earth */
        glPushMatrix(); {

            /* display earth */
            er_model_display_earth( & er_client->cl_view );

        } glPopMatrix();

        /* projection : interface */
        er_client_proj_interface( er_client );

        /* matrix - interface */
        glPushMatrix(); {

            /* display interface */
            er_times_display( & er_client->cl_times, & er_client->cl_view );

        } glPopMatrix();

    }

    le_void_t er_client_loops_event( er_client_t * const er_client ) {

        /* event variable */
        SDL_Event er_event;

        /* pop events from stack */
        while ( SDL_PollEvent( & er_event ) > 0 ) {

            /* switch on event type */
            switch ( er_event.type ) {

                /* event type : keydown */
                case( SDL_KEYDOWN ) : {

                    /* event-specific callback */
                    er_client_callback_keydown( er_event.key, er_client );

                } break;

                /* event type : wheel */
                case( SDL_MOUSEWHEEL ) : {

                    /* event-specific callback */
                    er_client_callback_wheel( er_event.wheel, er_client );

                } break;

                /* event type : button */
                case ( SDL_MOUSEBUTTONDOWN ) : {

                    /* event-specific callback */
                    er_client_callback_button( er_event.button, er_client );

                } break;

                /* event type : motion */
                case ( SDL_MOUSEMOTION ) : {

                    /* event-specific callback */
                    er_client_callback_motion( er_event.motion, er_client );

                } break;

            }

        }

    }

    le_void_t er_client_loops_update( er_client_t * const er_client, le_size_t const er_pack, le_size_t const er_delay ) {

        /* address variable */
        le_address_t er_address = LE_ADDRESS_C;

        /* motion detection */
        if ( er_view_get_equal( & er_client->cl_push, & er_client->cl_view ) == _LE_FALSE ) {

            /* push view */
            er_client->cl_push = er_client->cl_view;

            /* update motion time */
            er_client->cl_last = clock();

        }

        /* model update delay */
        if ( ( clock() - er_client->cl_last ) > er_delay ) {

            /* retreive address times */
            er_address = er_view_get_times( & er_client->cl_view );

            /* prepare model update */
            er_model_set_prep( & er_client->cl_model );

            /* update model target */
            er_model_set_enum( & er_client->cl_model, & er_address, 0, & er_client->cl_view );

            /* model/target fast synchronisation */
            er_model_set_fast( & er_client->cl_model );

            /* reset motion time */
            er_client->cl_last = _LE_TIME_MAX;

        }

        /* check synchronisation flag */
        if ( er_model_get_sync( & er_client->cl_model ) == _LE_FALSE ) {

            /* model synchronisation process */
            er_model_set_sync( & er_client->cl_model, er_pack );

        } else {

            /* check synchronisation flag */
            if ( er_model_get_tail( & er_client->cl_model ) == _LE_FALSE ) {

                /* model synchronisation tail process */
                er_model_set_tail( & er_client->cl_model );

            }

        }

    }

/*
    source - projection methods
 */

    le_void_t er_client_proj_model( er_client_t * const er_client ) {

        /* clipping plane variables */
        le_real_t er_neac = er_geodesy_near( er_view_get_alt( & er_client->cl_view ) );
        le_real_t er_farc = er_geodesy_far ( er_view_get_alt( & er_client->cl_view ) );

        /* compute model scale factor */
        er_client->cl_scale = er_geodesy_scale( er_view_get_alt( & er_client->cl_view ) );

        /* matrix mode to projection */
        glMatrixMode( GL_PROJECTION );

        /* set matrix to identity */
        glLoadIdentity();

        /* compute projection matrix */
        gluPerspective( 45.0, ( GLdouble ) er_client->cl_width / er_client->cl_height, er_neac, er_farc );

        /* matrix mode to modelview */
        glMatrixMode( GL_MODELVIEW );

        /* set matrix to identity */
        glLoadIdentity();

        /* apply scale factor to projection matrix */
        glScaled( er_client->cl_scale, er_client->cl_scale, er_client->cl_scale );

        /* enable depth test */
        glEnable( GL_DEPTH_TEST );

        /* fog configuration */
        glFogf( GL_FOG_START, er_farc * 0.8 ), glFogf( GL_FOG_END, er_farc );

        /* enable fog features */
        glEnable( GL_FOG );

    }

    le_void_t er_client_proj_interface( er_client_t * const er_client ) {

        /* matrix mode to projection */
        glMatrixMode( GL_PROJECTION );

        /* set matrix to identity */
        glLoadIdentity();

        /* compute projection matrix */
        glOrtho( 0, er_client->cl_width, 0, er_client->cl_height, -1.0, 1.0 );

        /* matrix mode to modelview */
        glMatrixMode( GL_MODELVIEW );

        /* set matrix to identity */
        glLoadIdentity();

        /* disable depth test */
        glDisable( GL_DEPTH_TEST );

        /* disable fog feature */
        glDisable( GL_FOG );

    }

/*
    source - event methods
 */

    le_void_t er_client_callback_keydown( SDL_KeyboardEvent er_event, er_client_t * const er_client ) {

        /* switch on keycode */
        switch ( er_event.keysym.sym ) {

            case ( SDLK_ESCAPE ) : {

                /* update execution mode */
                er_client->cl_loops = ER_COMMON_EXIT;

            } break;

            case ( SDLK_p ) : {

                /* check model update state */
                if ( er_model_get_tail( & er_client->cl_model ) == _LE_TRUE ) {

                    /* check stack state */
                    if ( er_video_get_state( & er_client->cl_video ) == _LE_TRUE ) {

                        /* update execution mode */
                        er_client->cl_loops = ER_COMMON_AUTO;

                    }

                }

            } break;

            case ( SDLK_1 ) :
            case ( SDLK_2 ) :
            case ( SDLK_3 ) :
            case ( SDLK_4 ) : {

                /* check modifier */
                if ( ( SDL_GetModState() & KMOD_LSHIFT ) != 0 ) {

                    /* update model parameter */
                    glLineWidth( er_event.keysym.sym - SDLK_1 + 1 );

                } else {

                    /* update model parameter */
                    glPointSize( er_event.keysym.sym - SDLK_1 + 1 );

                }

            } break;

            case ( SDLK_SPACE ) : {

                /* equal times */
                er_view_set_times( & er_client->cl_view );

            } break;

            case ( SDLK_q ) : {

                /* update dual-model mode */
                er_view_set_mode( & er_client->cl_view, 1 );

            } break;

            case ( SDLK_w ) : {

                /* update dual-model mode */
                er_view_set_mode( & er_client->cl_view, 2 );

            } break;

            case ( SDLK_e ) : {

                /* update dual-model mode */
                er_view_set_mode( & er_client->cl_view, 3 );

            } break;

            case ( SDLK_r ) : {

                /* update dual-model mode */
                er_view_set_mode( & er_client->cl_view, 4 );

            } break;

            case ( SDLK_t ) : {

                /* update dual-model mode */
                er_view_set_mode( & er_client->cl_view, 5 );

            } break;

            case ( SDLK_s ) :
            case ( SDLK_KP_PLUS ) : {

                /* update span value */
                er_view_set_span( & er_client->cl_view, +1 );

            } break;

            case ( SDLK_a ) :
            case ( SDLK_KP_MINUS ) : {

                /* update span value */
                er_view_set_span( & er_client->cl_view, -1 );

            } break;

            case ( SDLK_i ) : {

                /* push current view on stack */
                er_video_set_push( & er_client->cl_video, & er_client->cl_view );

            } break;

            case ( SDLK_o ) : {

                /* reset current view stack */
                er_video_set_reset( & er_client->cl_video );

            } break;

        }

    }

    le_void_t er_client_callback_wheel( SDL_MouseWheelEvent er_event, er_client_t * const er_client ) {

        /* modifier variable */
        le_enum_t er_modif = SDL_GetModState() & ER_COMMON_KMALL;

        /* check modifiers */
        if ( er_modif == ER_COMMON_KMAAC ) {

            /* check wheel direction */
            if ( er_event.y > 0 ) {

                /* update time area */
                er_view_set_comb( & er_client->cl_view, 1.0990 );

            } else {

                /* update time area */
                er_view_set_comb( & er_client->cl_view, 0.9099 );

            }

        } else if ( er_modif == ER_COMMON_KMALT ) {

            /* check wheel direction */
            if ( er_event.y > 0 ) {

                /* update time position */
                er_view_set_time( & er_client->cl_view, + 0.02 );

            } else {

                /* update time position */
                er_view_set_time( & er_client->cl_view, - 0.02 );

            }

        } else {

            /* compute inertia */
            er_client->cl_inertia = er_view_get_inertia( & er_client->cl_view, er_modif );

            /* check wheel direction */
            if ( er_event.y > 0 ) {

                /* update view position */
                er_view_set_alt( & er_client->cl_view, + er_client->cl_inertia );

            } else {

                /* update view position */
                er_view_set_alt( & er_client->cl_view, - er_client->cl_inertia );

            }

        }

    }

    le_void_t er_client_callback_button( SDL_MouseButtonEvent er_event, er_client_t * const er_client ) {

        /* check button state */
        if ( ( er_event.button == SDL_BUTTON_LEFT ) || ( er_event.button == SDL_BUTTON_RIGHT ) ) {

            /* push motion origin */
            er_client->cl_x = er_client->cl_width  >> 1;
            er_client->cl_y = er_client->cl_height >> 1;

            /* reset mouse position */
            SDL_WarpMouseGlobal( er_client->cl_x, er_client->cl_y );

        }

    }

    le_void_t er_client_callback_motion( SDL_MouseMotionEvent er_event, er_client_t * const er_client ) {

        /* motion variable */
        le_size_t er_dx = er_event.x - er_client->cl_x;
        le_size_t er_dy = er_event.y - er_client->cl_y;

        /* motion variable */
        le_real_t er_mx = er_dx;
        le_real_t er_my = er_dy;

        /* position variable */
        le_size_t er_cx = er_client->cl_width  >> 1;
        le_size_t er_cy = er_client->cl_height >> 1;

        /* switch on button */
        if ( ( er_event.state & SDL_BUTTON_LMASK ) != 0 ) {

            /* compute inertia */
            er_client->cl_inertia = er_view_get_inertia( & er_client->cl_view, SDL_GetModState() );

            /* apply inertia */
            er_mx *= ER_COMMON_INP * er_client->cl_inertia;
            er_my *= ER_COMMON_INP * er_client->cl_inertia;

            /* update view position */
            er_view_set_plan( & er_client->cl_view, er_mx, er_my );

        } else if ( ( er_event.state & SDL_BUTTON_RMASK ) != 0 ) {

            /* apply inertia */
            er_mx *= ER_COMMON_INR;
            er_my *= ER_COMMON_INR;

            /* update view direction */
            er_view_set_azm( & er_client->cl_view, - er_mx );
            er_view_set_gam( & er_client->cl_view, + er_my );

        }

        /* continuous motion management */
        er_client->cl_x = er_cx - er_dx;
        er_client->cl_y = er_cy - er_dy;

        /* reset mouse position */
        SDL_WarpMouseGlobal( er_cx, er_cy );

    }

