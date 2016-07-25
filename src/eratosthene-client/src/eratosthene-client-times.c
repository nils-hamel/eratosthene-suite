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
        if ( ( er_times.tm_tdis = er_client_server_tparam( er_ip, er_port ) ) == _LE_TIME_NULL ) {

            /* Send message */
            return( er_times._status = _LE_FALSE, er_times );

        }

        /* Query times array */
        er_times.tm_times = er_client_server_times( er_ip, er_port );

        /* Check array size */
        if ( le_array_get_size( & er_times.tm_times ) <= 0 ) {

            /* Send message */
            return( er_times._status = _LE_FALSE, er_times );

        }

        /* Retrieve times count */
        er_times.tm_count = le_array_get_record( & er_times.tm_times, LE_ARRAY_64T_LEN );

        /* Parsing times array */
        for ( le_size_t er_parse = 0; er_parse < le_array_get_size( & er_times.tm_times ); er_parse += LE_ARRAY_64T_LEN ) {

            /* Push selection allocation */
            le_array_set_push( & er_times.tm_stack, LE_ARRAY_64T, NULL, _LE_TIME_NULL, NULL );

        }

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
        le_array_delete( & er_times->tm_stack );

        /* Unallocate times array */
        le_array_delete( & er_times->tm_times );

        /* Delete structure */
        * er_times = er_delete;

    }

/*
    source - accessor methods
 */

    le_time_t er_times_get( er_times_t const * const er_times ) {

        /* Static parser variables */
        static le_size_t er_parse = 0;

        /* Array base pointers variables */
        le_time_t * er_sel = ( le_time_t * ) le_array_get_byte( & er_times->tm_stack );

        /* Enumeration of selection array */
        while ( er_parse < er_times->tm_count ) {

            /* Check array element */
            if ( er_sel[er_parse] != _LE_TIME_NULL ) {

                /* Return selected time */
                return( er_sel[er_parse ++] );

            /* Update enumeration parser */
            } else { ++ er_parse; }

        }

        /* Reset enumeration parser */
        er_parse = 0;

        /* Send enumeration end */
        return( _LE_TIME_NULL );

    }

    le_enum_t er_times_get_update( er_times_t * const er_times ) {

        /* Returned value variables */
        le_enum_t er_return = er_times->tm_mtim;

        /* Update modification flag */
        er_times->tm_mtim = _LE_FALSE;

        /* Return modification flag */
        return( er_return );

    }

/*
    source - mutator methods
 */

    le_void_t er_times_set( er_times_t * const er_times, le_enum_t const er_unique ) {

        /* Array base pointers variables */
        le_time_t * er_ref = ( le_time_t * ) le_array_get_byte( & er_times->tm_times );
        le_time_t * er_sel = ( le_time_t * ) le_array_get_byte( & er_times->tm_stack );

        /* Check unicity flag */
        if ( er_unique == _LE_TRUE ) {

            /* Disable every times */
            for ( le_size_t er_parse = 0; er_parse < er_times->tm_count; er_parse ++ ) er_sel[er_parse] = _LE_TIME_NULL;

        }

        /* Check state of time */
        if ( er_sel[er_times->tm_curr] == _LE_TIME_NULL ) {

            /* Update time state */
            er_sel[er_times->tm_curr] = er_ref[er_times->tm_curr];

        } else {

            /* Update time state */
            er_sel[er_times->tm_curr] = _LE_TIME_NULL;

        }

        /* Update modification flag */
        er_times->tm_mtim = _LE_TRUE;

    }

    le_void_t er_times_set_default( er_times_t * const er_times ) {

        /* Array base pointers variables */
        le_time_t * er_ref = ( le_time_t * ) le_array_get_byte( & er_times->tm_times );
        le_time_t * er_sel = ( le_time_t * ) le_array_get_byte( & er_times->tm_stack );

        /* Flag variables */
        le_enum_t er_flag = _LE_FALSE;

        /* Parsing times array */
        for ( le_size_t er_scale = 0; er_scale < er_times->tm_count; er_scale ++ ) {

            /* Check for SRTM data */
            if ( er_ref[er_scale] == ER_TIMES_SRTM( er_times->tm_tdis ) ) {

                /* Enable time at index */
                er_times->tm_pose = ( er_sel[er_scale] = er_ref[er_scale] );

                /* Update flag */
                er_flag = _LE_TRUE;

            }

        }

        /* Check default specification state - enable default time */
        if ( er_flag == _LE_FALSE ) er_times->tm_pose = ( er_sel[0] = er_ref[0] );

    }

    le_void_t er_times_set_nearest( er_times_t * const er_times ) {

        /* Extremum distance variables */
        le_time_t er_distance = _LE_TIME_MAX;

        /* Array base pointers variables */
        le_time_t * er_ref = ( le_time_t * ) le_array_get_byte( & er_times->tm_times );

        /* Parsing times array */
        for ( le_size_t er_parse = 0; er_parse < er_times->tm_count; er_parse ++ ) {

            /* Check distance */
            if ( abs( er_ref[er_parse] - er_times->tm_pose ) < er_distance ) {

                /* Update current selection */
                er_times->tm_curr = er_parse;

                /* Update extremal distance */
                er_distance = abs( er_ref[er_parse] - er_times->tm_pose );

            }

        }

    }

    le_void_t er_times_set_zoom( er_times_t * const er_times, le_size_t const er_mode ) {

        /* Update zoom value */
        er_times->tm_zoom *= ( er_mode == ER_TIMES_DECREASE ? 0.91 : 1.1 );

        /* Check limitation */
        if ( er_times->tm_zoom <= ER_TIMES_ZOOM_MIN ) er_times->tm_zoom = ER_TIMES_ZOOM_MIN;
        if ( er_times->tm_zoom >= ER_TIMES_ZOOM_MAX ) er_times->tm_zoom = ER_TIMES_ZOOM_MAX;

        /* Update nearest times */
        er_times_set_nearest( er_times );

    }

    le_void_t er_times_set_pose( er_times_t * const er_times, le_size_t const er_mode ) {

        /* Update position value */
        er_times->tm_pose += er_times->tm_zoom * ( er_mode == ER_TIMES_DECREASE ? -0.02 : 0.02 );

        /* Update nearest times */
        er_times_set_nearest( er_times );

    }

    le_void_t er_times_set_time( er_times_t * const er_times, le_size_t const er_mode ) {

        /* Check update mode */
        if ( er_mode == ER_TIMES_DECREASE ) {

            /* Update highlighted time */
            er_times->tm_curr = ( er_times->tm_curr - 1 + er_times->tm_count ) % er_times->tm_count;

        } else {

            /* Update highlighted time */
            er_times->tm_curr = ( er_times->tm_curr + 1 ) % er_times->tm_count;

        }

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

        /* Array base pointers variables */
        le_time_t * er_ref = ( le_time_t * ) le_array_get_byte( & er_times->tm_times );
        le_time_t * er_sel = ( le_time_t * ) le_array_get_byte( & er_times->tm_stack );

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
        for ( le_size_t er_parse = 0; er_parse < er_times->tm_count; er_parse ++ ) {

            /* Check time visibility */
            if ( ( er_ref[er_parse] > er_lbound ) && ( er_ref[er_parse] < er_ubound ) ) {

                /* Check time state - assign string color */
                if ( er_sel[er_parse] != _LE_TIME_NULL ) glColor3f( 0.5, 0.5, 0.5 ); else glColor3f( 0.7, 0.7, 0.7 );

                /* Check time state - assign string color */
                if ( er_parse == er_times->tm_curr ) glColor3f( 0.3, 0.5, 0.7 );

                /* Display date string */
                er_times_print_date( er_ref[er_parse], er_xsize * ( ( le_real_t ) ( er_ref[er_parse] - er_lbound ) / er_times->tm_zoom ), 19, ER_TIMES_JUST_CENTER );

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

    le_void_t er_times_print_date( le_time_t const er_time, le_size_t const er_x, le_size_t const er_y, le_enum_t const er_justify ) {

        /* String array varibles */
        le_char_t er_string[32] = { 0 };

        /* Check justification */
        if ( er_justify == ER_TIMES_JUST_LEFT ) {

            /* Assign string position */
            glRasterPos2i( er_x, er_y - 13 );

        } else if ( er_justify == ER_TIMES_JUST_RIGHT ) {

            /* Assign string position */
            glRasterPos2i( er_x - strlen( ( char * ) er_string ) * 8, er_y - 13 );

        } else {

            /* Assign string position */
            glRasterPos2i( er_x - strlen( ( char * ) er_string ) * 4, er_y - 13 );

        }

        /* Display string */
        glutBitmapString( GLUT_BITMAP_8_BY_13, lc_time_to_string( er_time, er_string, 32 ) );

    }

