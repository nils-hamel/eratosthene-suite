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

    # include "eratosthene-client-times.h"

/*
    source - constructor/destructor methods
 */

    er_times_t er_times_create( le_char_t * const er_ip, le_sock_t const er_port ) {

        /* Returned structure variables */
        er_times_t er_times = ER_TIMES_C;

        /* Assign server connection parameters */
        er_times.tm_svip = er_ip;
        er_times.tm_port = er_port;

        /* Retrieve server configuration */
        if ( ( er_times.tm_tparam = er_client_server_tparam( er_ip, er_port ) ) == _LE_TIME_NULL ) {

            /* Send message */
            return( er_times._status = _LE_FALSE, er_times );

        }

        /* Query times array */
        er_times.tm_tarray = er_client_server_times( er_ip, er_port );

        /* Check array size */
        if ( le_array_get_size( & er_times.tm_tarray ) <= 0 ) {

            /* Send message */
            return( er_times._status = _LE_FALSE, er_times );

        }

        /* Retrieve times count */
        er_times.tm_size = le_array_get_record( & er_times.tm_tarray, LE_ARRAY_64T_LEN );

        /* Retrieve times array */
        er_times.tm_time = ( le_time_t * ) le_array_get_byte( & er_times.tm_tarray );

        /* Parsing times array */
        //for ( le_size_t er_parse = 0; er_parse < le_array_get_size( & er_times.tm_tarray ); er_parse += LE_ARRAY_64T_LEN ) {

            /* Push selection allocation */
        //    le_array_set_push( & er_times.tm_stack, LE_ARRAY_64T, NULL, _LE_TIME_NULL, NULL );

        //}

        /* Set default time */
        er_times_set_default( & er_times );

        /* Set nearest time */
        er_times_set_nearest( & er_times );

        /* Return constructed structure */
        return( er_times );

    }

    le_void_t er_times_delete( er_times_t * const er_times ) {

        /* Deleted structure variables */
        er_times_t er_delete = ER_TIMES_C;

        /* Unallocate stack array */
        //le_array_delete( & er_times->tm_stack );

        /* Unallocate times array */
        le_array_delete( & er_times->tm_tarray );

        /* Delete structure */
        * er_times = er_delete;

    }

/*
    source - accessor methods
 */

    le_time_t er_times_get( er_times_t const * const er_times ) {

        /* Static parser variables */
        static le_size_t er_parse = 0;

        /* Enumeration on enable time array */
        while ( er_parse < 2 ) {

            /* Check enable time array */
            if ( er_times->tm_view[er_parse] != _LE_TIME_NULL ) {

                /* Return enable time */
                return( er_times->tm_view[er_parse ++] );

            /* Continue enumeration */
            } else { ++ er_parse; }

        }

        /* Reset enumeration and send time */
        return( er_parse = 0, _LE_TIME_NULL );

    }

    le_enum_t er_times_get_update( er_times_t * const er_times ) {

        /* Returned value variables */
        le_enum_t er_return = er_times->tm_update;

        /* Update modification flag */
        er_times->tm_update = _LE_FALSE;

        /* Return modification flag */
        return( er_return );

    }

/*
    source - mutator methods
 */

    le_void_t er_times_set( er_times_t * const er_times, le_size_t const er_index ) {

        /* Check consistency */
        if ( ( er_index >= 0 ) && ( er_index < 2 ) ) {

            /* Check enable time state */
            if ( er_times->tm_view[er_index] != er_times->tm_time[er_times->tm_near] ) {
            
                /* Enable highlighted time */
                er_times->tm_view[er_index] = er_times->tm_time[er_times->tm_near];

            } else {

                /* Disable highlighted time */
                er_times->tm_view[er_index] = _LE_TIME_NULL;

            }

            /* Trigger model update */
            er_times->tm_update = _LE_TRUE;

        }

    }

    le_void_t er_times_set_default( er_times_t * const er_times ) {

        /* State flag variables */
        le_enum_t er_flag = _LE_FALSE;

        /* Parsing times array */
        for ( le_size_t er_parse = 0; er_parse < er_times->tm_size; er_parse ++ ) {

            /* Check for SRTM data */
            if ( er_times->tm_time[er_parse] == ER_TIMES_SRTM( er_times->tm_tparam ) ) {

                /* Update flag and enable default time */
                er_flag = _LE_TRUE, er_times->tm_pose = er_times->tm_view[0] = er_times->tm_time[er_parse];

            }

        }

        /* Check state flag - enable first time as default */
        if ( er_flag == _LE_FALSE ) er_times->tm_pose = er_times->tm_view[0] = er_times->tm_time[0];

    }

    le_void_t er_times_set_nearest( er_times_t * const er_times ) {

        /* Distance variables */
        le_time_t er_distance = _LE_TIME_MAX, er_current = _LE_TIME_NULL;

        /* Parsing times array */
        for ( le_size_t er_parse = 0; er_parse < er_times->tm_size; er_parse ++ ) {

            /* Check current distance */
            if ( ( er_current = le_time_abs( er_times->tm_time[er_parse] - er_times->tm_pose ) ) < er_distance ) {

                /* Update nearest index and distance */
                er_times->tm_near = er_parse, er_distance = er_current;

            }

        }

    }

    le_void_t er_times_set_zoom( er_times_t * const er_times, le_size_t const er_mode ) {

        /* Update zoom value */
        er_times->tm_zoom *= ( er_mode == ER_TIMES_DECREASE ? 1.0 / 1.1 : 1.1 );

        /* Check limitation */
        if ( er_times->tm_zoom <= ER_TIMES_ZOOM_MIN ) er_times->tm_zoom = ER_TIMES_ZOOM_MIN;
        if ( er_times->tm_zoom >= ER_TIMES_ZOOM_MAX ) er_times->tm_zoom = ER_TIMES_ZOOM_MAX;

    }

    le_void_t er_times_set_pose( er_times_t * const er_times, le_size_t const er_mode ) {

        /* Update position value */
        er_times->tm_pose += er_times->tm_zoom * ( er_mode == ER_TIMES_DECREASE ? -0.02 : 0.02 );

        /* Update nearest times */
        er_times_set_nearest( er_times );

    }

/*
    source - display methods
 */

    le_void_t er_times_display( er_times_t const * const er_times ) {

        /* Static buffer variables */
        static le_byte_t * er_buffer = NULL;

        /* Static buffer configuration variables */
        static le_size_t er_xsize = 0;
        static le_size_t er_ysize = 0;
        static le_size_t er_bsize = 0;

        /* Graduation display variables */
        le_time_t er_xgrad = 0;
        le_size_t er_ygrad = 0;
        le_time_t er_dgrad = ER_TIMES_GRAD_SCALE;

        /* Boundaries variables */
        le_time_t er_lbound = er_times->tm_pose - ( er_times->tm_zoom >> 1 );
        le_time_t er_ubound = er_times->tm_pose + ( er_times->tm_zoom >> 1 );

        /* Check buffer state */
        if ( er_buffer == NULL ) {

            /* Compute buffer dimensions */
            er_xsize = glutGet( GLUT_SCREEN_WIDTH  );
            er_ysize = glutGet( GLUT_SCREEN_HEIGHT ) * 0.1;

            /* Compute buffer size */
            er_bsize = er_xsize * er_ysize * 4;

            /* Allocate buffer memory - initialise memory */
            if ( ( er_buffer = ( le_byte_t * ) malloc( er_bsize ) ) != NULL ) memset( er_buffer, 255, er_bsize );

        }

        /* Reset graphical buffer */
        for ( le_size_t er_parse = 3; er_parse < er_bsize; er_parse += 4 ) er_buffer[er_parse] = 208;

        /* Information string color */
        glColor3f( 0.5, 0.5, 0.5 );

        /* Display indicative dates - lower */
        er_times_print_date( er_lbound, 16, er_ysize - 2, ER_TIMES_JUST_LEFT );

        /* Display indicative dates - upper */
        er_times_print_date( er_ubound, er_xsize - 16, er_ysize - 2, ER_TIMES_JUST_RIGHT );
        
        /* Display times */
        for ( le_size_t er_parse = 0; er_parse < er_times->tm_size; er_parse ++ ) {

            /* Check time visibility */
            if ( ( er_times->tm_time[er_parse] > er_lbound ) && ( er_times->tm_time[er_parse] < er_ubound ) ) {

                /* Check time state - assign color */
                if ( er_times->tm_time[er_parse] == er_times->tm_view[0] ) glColor3f( 0.3, 0.5, 0.7 ); else glColor3f( 0.5, 0.5, 0.5 );

                /* Check time state - assign color */
                if ( er_times->tm_time[er_parse] == er_times->tm_view[1] ) glColor3f( 0.3, 0.7, 0.5 );

                /* Check time state - assign color */
                if ( er_parse == er_times->tm_near ) glColor3f( 0.7, 0.3, 0.5 );

                /* Display date string */
                er_times_print_date( er_times->tm_time[er_parse], er_xsize * ( ( le_real_t ) ( er_times->tm_time[er_parse] - er_lbound ) / er_times->tm_zoom ), 19, ER_TIMES_JUST_CENTER );

            }

        }

        /* Display graduation scales */
        for ( le_size_t er_scale = 0; er_scale < ER_TIMES_GRAP_DEPTH; er_dgrad /= 10, er_scale ++ ) {

            /* Check graduation spacing */
            if ( ( ( ( ( le_real_t ) er_dgrad ) / er_times->tm_zoom ) * er_xsize ) > 4 ) {

                /* Display graduation */
                for ( le_time_t er_parse = ER_TIMES_ROUND( er_lbound, er_dgrad ); er_parse < er_ubound; er_parse += er_dgrad ) {

                    /* Compute graduation increment x-position */
                    er_xgrad = ( ( ( ( le_real_t ) er_parse ) - er_lbound ) / er_times->tm_zoom ) * er_xsize;

                    /* Check interface boundaries */
                    if ( ( er_xgrad >= 16 ) && ( er_xgrad <= er_xsize - 16 ) ) {

                        /* Compute graduation y-position */
                        er_ygrad = 20 + ( er_scale << 2 );

                        /* Display graduation increment */
                        for ( le_size_t er_pixel = er_ygrad; er_pixel < er_ysize - er_ygrad; er_pixel ++ ) {

                            /* Update interface buffer alpha channel */
                            er_buffer[(er_xgrad + er_pixel * er_xsize) * 4 + 3] -= 52;

                        }

                    }

                }

            }

        }

        /* Buffer display position */
        glRasterPos2i( 0, 0 );

        /* Display buffer */
        glDrawPixels( er_xsize, er_ysize, GL_RGBA, GL_UNSIGNED_BYTE, er_buffer );

    }

/*
    source - auxiliary methods
 */

    le_void_t er_times_print_date( le_time_t const er_time, le_size_t er_x, le_size_t er_y, le_enum_t const er_justify ) {

        /* String array varibles */
        le_char_t er_string[32] = { 0 };

        /* Compose date string */
        lc_time_to_string( er_time, er_string, 32 );

        /* Check justification */
        if ( er_justify == ER_TIMES_JUST_RIGHT  ) er_x -= strlen( ( char * ) er_string ) * 8;
        if ( er_justify == ER_TIMES_JUST_CENTER ) er_x -= strlen( ( char * ) er_string ) * 4;

        /* Assign string position */
        glRasterPos2i( er_x, er_y - 13 );

        /* Display string */
        glutBitmapString( GLUT_BITMAP_8_BY_13, er_string );

    }

