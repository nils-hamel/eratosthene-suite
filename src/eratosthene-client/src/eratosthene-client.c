/*
 *  eratosthene-suite - client
 *
 *      Nils Hamel - nils.hamel@bluewin.ch
 *      Copyright (c) 2016-2018 DHLAB, EPFL
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

    er_client_t er_client_create( le_char_t * const er_ip, le_sock_t const er_port, le_size_t const er_width, le_size_t const er_height ) {

        /* array variables */
        le_array_t er_array = LE_ARRAY_C;

        /* server configuration variables */
        le_size_t er_space = 0;
        le_time_t er_times = 0;

        /* serialisation variables */
        le_size_t er_serial = 0;

        /* created structure variables */
        er_client_t er_client = ER_CLIENT_C;

        /* assign display parameters */
        er_client.cl_width  = er_width;
        er_client.cl_height = er_height;

        /* create client socket */
        if ( ( er_client.cl_socket = le_client_create( er_ip, er_port ) ) == _LE_SOCK_NULL ) {

            /* return created structure */
            return( er_client._status = _LE_FALSE, er_client );

        }

        /* update socket-array size */
        le_array_set_size( & er_array, 0 );

        /* write socket-array */
        le_array_io_write( & er_array, LE_MODE_AUTH, er_client.cl_socket );

        /* read socket array */
        if ( le_array_io_read( & er_array, er_client.cl_socket ) != LE_MODE_AUTH ) {

            /* delete client socket */
            le_client_delete( er_client.cl_socket );

            /* return created structure */
            return( er_client._status = _LE_FALSE, er_client );

        }

        /* check consistency */
        if ( le_array_get_size( & er_array ) != LE_ARRAY_AUTH ) {

            /* delete client socket */
            le_client_delete( er_client.cl_socket );

            /* return created structure */
            return( er_client._status = _LE_FALSE, er_client );

        }

        /* serialise server configuration */
        er_serial = le_array_serial( & er_array, & er_space, sizeof( le_size_t ), er_serial, _LE_GET );
        er_serial = le_array_serial( & er_array, & er_times, sizeof( le_time_t ), er_serial, _LE_GET );

        /* create client model */
        if ( ( er_client.cl_model = er_model_create( er_client.cl_socket, er_space, er_times ) )._status == _LE_FALSE ) {

            /* delete client socket */
            le_client_delete( er_client.cl_socket );

            /* return created structure */
            return( er_client._status = _LE_FALSE, er_client );

        }

        /* create client times */
        if ( ( er_client.cl_times = er_times_create( er_width, er_height ) )._status == _LE_FALSE ) {

            /* delete client socket */
            le_client_delete( er_client.cl_socket );

            /* return created structure */
            return( er_client._status = _LE_FALSE, er_client );

        }

        /* create client movie */
        if ( ( er_client.cl_movie = er_movie_create( er_width, er_height ) )._status == _LE_FALSE ) {

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
        er_model_delete( & er_client->cl_model );

        /* socket-array size */
        le_array_set_size( & er_array, 0 );

        /* write socket-array */
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

        /* messasge variable */
        int er_message = EXIT_SUCCESS;

        /* display mode variable */
        SDL_DisplayMode er_display;

        /* window handle variable */
        SDL_Window * er_window = NULL;

        /* opengl context variable */
        SDL_GLContext er_context;

        /* client structure variable */
        er_client_t er_client = ER_CLIENT_C;

        /* server address variables */
        le_char_t * er_svip = NULL;

        /* server service variables */
        le_sock_t er_port = 0;

        /* initialise sdl */
        if ( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {

            /* display message */
            fprintf( stderr, ER_COMMON_PREFIX " : error : unable to initialise sdl library\n" );

            /* push message */
            er_message = EXIT_FAILURE;

        } else {

            /* retrieve display mode */
            SDL_GetCurrentDisplayMode( 0, & er_display );

            /* hide mouse cursor */
            SDL_ShowCursor( SDL_DISABLE );

            /* enable double buffer */
            SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

            /* color buffer bits */
            SDL_GL_SetAttribute( SDL_GL_RED_SIZE  , 8 );
            SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 );
            SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE , 8 );
            SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, 8 );

            /* depth buffer bits */
            SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 24 );

            /* create window */
            if ( ( er_window = SDL_CreateWindow( ER_COMMON_NAME, 0, 0, er_display.w, er_display.h, ER_SDL_FLAGS ) ) == NULL ) {

                /* display message */
                fprintf( stderr, ER_COMMON_PREFIX " : error : unable to create window\n" );

                /* push message */
                er_message = EXIT_FAILURE;

            } else {

                /* create opengl context */
                if ( ( er_context = SDL_GL_CreateContext( er_window ) ) == NULL ) {

                    /* display message */
                    fprintf( stderr, ER_COMMON_PREFIX " : error : unable to create context\n" );

                    /* push message */
                    er_message = EXIT_FAILURE;

                } else {

                    /* read server address */
                    er_svip = ( le_char_t * ) lc_read_string( argc, argv, "--ip"  , "-i" );

                    /* read server service */
                    er_port = ( le_sock_t ) lc_read_signed( argc, argv, "--port", "-t", _LE_USE_PORT );

                    /* create client */
                    if ( ( er_client = er_client_create( er_svip, er_port, er_display.w, er_display.h ) )._status == _LE_FALSE ) {

                        /* display message */
                        fprintf( stderr, ER_COMMON_PREFIX " : error : unable to create client\n" );

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

        /* color array variables */
        float er_color[4] = ER_MODEL_SPACE;

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

        /* enable vertex and color arrays */
        glEnableClientState( GL_VERTEX_ARRAY );
        glEnableClientState( GL_COLOR_ARRAY  );

        /* opengl point size */
        glPointSize( 2.0 );

        /* disable dynamic thread */
        omp_set_dynamic( 0 );

        /* execution threads */
        # pragma omp parallel num_threads( 2 )
        {

        /* master swicth switch */
        if ( omp_get_thread_num() == 0 ) {

            /* execution loop */
            while ( er_client->cl_loops != ER_COMMON_EXIT ) {

                /* switch on execution mode */
                if ( er_client->cl_loops == ER_COMMON_VIEW ) {

                    /* principale execution loop */
                    while ( er_client->cl_loops == ER_COMMON_VIEW ) {

                        /* interface events procedure */
                        er_client_loops_event( er_client );

                        /* model display procedure */
                        er_client_loops_render( er_client );

                        /* swap buffers */
                        SDL_GL_SwapWindow( er_window );

                    }

                } else if ( er_client->cl_loops == ER_COMMON_MOVIE ) {

                    /* principale execution loop */
                    while ( er_client->cl_loops == ER_COMMON_MOVIE ) {

                        /* motion management procedure */
                        er_client->cl_view = er_movie_get( & er_client->cl_movie );

                        /* model update procedure */
                        er_client_loops_update( er_client );

                        /* model display procedure */
                        er_client_loops_render( er_client );

                        /* swap buffers */
                        SDL_GL_SwapWindow( er_window );

                        /* movie procedure */
                        er_client->cl_loops = er_movie( & er_client->cl_movie );

                    }

                }

            }

        } else {

            /* execution loop */
            while ( er_client->cl_loops != ER_COMMON_EXIT ) {

                /* switch on execution mode */
                if ( er_client->cl_loops == ER_COMMON_VIEW ) {

                    /* principale execution loop */
                    while ( er_client->cl_loops == ER_COMMON_VIEW ) {

                        /* model update procedure */
                        er_client_loops_update( er_client );

                    }

                }

            }

        }

        }

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

    le_void_t er_client_loops_update( er_client_t * const er_client ) {

        /* address variables */
        le_address_t er_enum = LE_ADDRESS_C;

        /* motion detection */
        if ( er_view_get_equal( & er_client->cl_push, & er_client->cl_view ) == _LE_FALSE ) {

            /* retreive address times */
            er_enum = er_view_get_times( & er_client->cl_view );

            /* prepare model update */
            er_model_set_prep( & er_client->cl_model );

            /* update model target */
            er_model_set_enum( & er_client->cl_model, & er_enum, 0, & er_client->cl_view );

            /* model/target fast synchronisation */
            er_model_set_fast( & er_client->cl_model );

            /* push considered view */
            er_client->cl_push = er_client->cl_view;

        }

        /* check exectution mode */
        if ( er_client->cl_loops == ER_COMMON_MOVIE ) {

            /* synchronisation process - full-process */
            while ( er_model_set_sync( & er_client->cl_model ) == _LE_FALSE );

        } else {

            /* synchronisation process - step-process */
            er_model_set_sync( & er_client->cl_model );

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

                /* update execution mode */
                er_client->cl_loops = ER_COMMON_MOVIE;

            } break;

            case ( SDLK_1 ) :
            case ( SDLK_2 ) :
            case ( SDLK_3 ) :
            case ( SDLK_4 ) : {

                /* update model parameter */
                glPointSize( er_event.keysym.sym - SDLK_1 + 1 );

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

            case ( SDLK_i ) : {

                /* flush control point */
                er_movie_set_clear( & er_client->cl_movie );

            } break;

            case ( SDLK_o ) : {

                /* push view as control point */
                er_movie_set( & er_client->cl_movie, & er_client->cl_view );

            } break;

            case ( SDLK_COMMA ) : {

                /* update model cell span */
                er_view_set_red( & er_client->cl_view, +1 );

            } break;

            case ( SDLK_PERIOD ) : {

                /* update model cell span */
                er_view_set_red( & er_client->cl_view, -1 );

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
                er_view_set_area( & er_client->cl_view, 1.0990 );

            } else {

                /* update time area */
                er_view_set_area( & er_client->cl_view, 0.9099 );

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

            /* compute inertia */
            er_client->cl_inertia = er_view_get_inertia( & er_client->cl_view, SDL_GetModState() );

            /* update click position */
            er_client->cl_x = er_event.x;
            er_client->cl_y = er_event.y;

        }

    }

    le_void_t er_client_callback_motion( SDL_MouseMotionEvent er_event, er_client_t * const er_client ) {

        /* motion variable */
        le_real_t er_dx = ( le_real_t ) er_event.x - er_client->cl_x;
        le_real_t er_dy = ( le_real_t ) er_event.y - er_client->cl_y;

        /* switch on button */
        if ( ( er_event.state & SDL_BUTTON_LMASK ) != 0 ) {

            /* apply inertia */
            er_dx *= ER_COMMON_INP * er_client->cl_inertia;
            er_dy *= ER_COMMON_INP * er_client->cl_inertia;

            /* update view position */
            er_view_set_plan( & er_client->cl_view, er_dx, er_dy );

        } else if ( ( er_event.state & SDL_BUTTON_RMASK ) != 0 ) {

            /* apply inertia */
            er_dx *= ER_COMMON_INR;
            er_dy *= ER_COMMON_INR;

            /* update view direction */
            er_view_set_azm( & er_client->cl_view, - er_dx );
            er_view_set_gam( & er_client->cl_view, + er_dy );

        }

    }

