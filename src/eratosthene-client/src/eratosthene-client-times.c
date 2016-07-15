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
            er_times._status = _LE_FALSE; return( er_times );

        }

        /* Query times array */
        er_times.tm_times = er_client_server_times( er_ip, er_port );

        /* Check array size */
        if ( le_array_get_size( & er_times.tm_times ) <= 0 ) {

            /* Send message */
            er_times._status = _LE_FALSE; return( er_times );

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

        /* Array base pointers variables */
        le_time_t * er_sel = ( le_time_t * ) le_array_get_byte( & er_times->tm_stack );

        /* Static parser variables */
        static le_size_t er_parse = 0;

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

    le_void_t er_times_set( er_times_t * const er_times ) {

        /* Array base pointers variables */
        le_time_t * er_ref = ( le_time_t * ) le_array_get_byte( & er_times->tm_times );
        le_time_t * er_sel = ( le_time_t * ) le_array_get_byte( & er_times->tm_stack );

        /* Check state at current */
        if ( er_sel[er_times->tm_curr] == _LE_TIME_NULL ) {

            /* Enable time at current */
            er_sel[er_times->tm_curr] = er_ref[er_times->tm_curr];

        } else {

            /* Disable time at current */
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
        for ( le_size_t er_index = 0; er_index < er_times->tm_count; er_index ++ ) {

            /* Check for SRTM data */
            if ( er_ref[er_index] == ER_TIMES_SRTM( er_times->tm_tdis ) ) {

                /* Enable time at index */
                er_times->tm_pose = ( er_sel[er_index] = er_ref[er_index] );

                /* Update flag */
                er_flag = _LE_TRUE;

            }

        }

        /* Check default specification state - enable default time */
        if ( er_flag == _LE_FALSE ) er_times->tm_pose = ( er_sel[0] = er_ref[0] );

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

        /* Check mode */
        if ( er_mode == ER_TIMES_DECREASE ) {

            /* Update pointed time */
            er_times->tm_curr = ( er_times->tm_curr == 0 ? er_times->tm_count - 1 : er_times->tm_curr - 1 );

        } else {

            /* Update pointed time */
            er_times->tm_curr = ( er_times->tm_curr + 1 ) % er_times->tm_count;

        }

    }

    le_void_t er_times_set_nearest( er_times_t * const er_times ) {

        /* Array base pointers variables */
        le_time_t * er_ref = ( le_time_t * ) le_array_get_byte( & er_times->tm_times );

        /* Extremum distance variables */
        le_time_t er_distance = _LE_TIME_MAX;

        /* Parsing times array */
        for ( le_size_t er_parse = 0; er_parse < er_times->tm_count; er_parse ++ ) {

            /* Check distance */
            if ( abs( er_ref[er_parse] - er_times->tm_pose ) < er_distance ) {

                /* Update extremal distance */
                er_distance = abs( er_ref[er_parse] - er_times->tm_pose );

                /* Update current selection */
                er_times->tm_curr = er_parse;

            }

        }

    }

/*
    source - display methods
 */

    le_void_t er_times_display( er_times_t const * const er_times ) {

        /* Static buffer configuration variables */
        static le_size_t er_cwidth  = 0;
        static le_size_t er_cheight = 0;
        static le_size_t er_csize   = 0;
        static le_size_t er_coffset = 0;

        /* Static buffer variables */
        static le_byte_t * er_buffer = NULL;

        /* Graduation scale variables */
        le_time_t er_grad = ER_TIMES_GRAD_SCALE;

        le_time_t er_point = 0;
        le_size_t er_egde = 0;

        /* Boundaries variables */
        le_time_t er_lbound = er_times->tm_pose - ( er_times->tm_zoom >> 1 );
        le_time_t er_ubound = er_times->tm_pose + ( er_times->tm_zoom >> 1 );

        /* Array base pointers variables */
        le_time_t * er_ref = ( le_time_t * ) le_array_get_byte( & er_times->tm_times );
        le_time_t * er_sel = ( le_time_t * ) le_array_get_byte( & er_times->tm_stack );

        /* Check buffer state */
        if ( er_buffer == NULL ) {

            /* Compute buffer sizes */
            er_cwidth  = glutGet( GLUT_SCREEN_WIDTH  );
            er_cheight = glutGet( GLUT_SCREEN_HEIGHT ) * 0.07;
            er_coffset = glutGet( GLUT_SCREEN_HEIGHT ) * 0.07;

            /* Compute buffer byte count */
            er_csize = er_cwidth * er_cheight * 4;

            /* Allocate buffer memory - clear buffer */
            if ( ( er_buffer  = ( le_byte_t * ) malloc( er_csize ) ) != NULL ) memset( er_buffer, 255, er_csize );

        }

        /* Reset graphical buffer */
        for ( le_size_t er_parse = 3; er_parse < er_csize; er_parse += 4 ) {

            /* Reset alpha channel component */
            er_buffer[er_parse] = 208;

        }

        /* Information string position */
        glRasterPos2i( 16 , er_coffset + er_cheight - 12 );

        /* Information string color */
        glColor3f( 0.3, 0.3, 0.3 );

        /* Display information string */
        glutBitmapString( GLUT_BITMAP_HELVETICA_10, er_times_range( er_times, er_lbound, er_ubound ) );

        /* Display times from stack */
        for ( le_size_t er_parse = 0; er_parse < er_times->tm_count; er_parse ++ ) {

            /* Check time visibility */
            if ( ( er_ref[er_parse] > er_lbound ) && ( er_ref[er_parse] < er_ubound ) ) {

                /* Set time string color */
                if ( er_sel[er_parse] != _LE_TIME_NULL ) glColor3f( 0.3, 0.3, 0.3 ); else glColor3f( 0.7, 0.7, 0.7 );

                /* Check highlighted time string */
                if ( er_parse == er_times->tm_curr ) glColor3f( 0.3, 0.5, 0.7 );

                /* Time string position */
                glRasterPos2i( er_cwidth * ( ( le_real_t ) ( er_ref[er_parse] - er_lbound ) / er_times->tm_zoom ), er_coffset + 4 );

                /* Display time string */
                glutBitmapString( GLUT_BITMAP_HELVETICA_10, er_times_string( er_ref[er_parse] )  );

            }

        }

        /* Display graduation */
        for ( le_size_t er_index = 0; er_index < ER_TIMES_GRAP_DEPTH; er_grad /= 10, er_index ++ ) {

            /* Check scale displayability */
            if ( ( ( ( ( le_real_t ) er_grad ) / er_times->tm_zoom ) * er_cwidth ) > 4 ) {

                /* Display graduation at current scale */
                for ( le_time_t er_parse = ER_TIMES_RUD( er_lbound, er_grad ); er_parse < er_ubound; er_parse += er_grad ) {

                    /* Compute position of graduation */
                    er_point = ( ( ( ( le_real_t ) er_parse ) - er_lbound ) / er_times->tm_zoom ) * er_cwidth;

                    /* Compute graduation egdes */
                    er_egde = 18 + ( er_index << 2 );

                    /* Display graduation */
                    for ( le_size_t er_pixel = er_egde; er_pixel < er_cheight - er_egde; er_pixel ++ ) {

                        /* Update alpha channel */
                        er_buffer[(er_point + er_pixel * er_cwidth) * 4 + 3] -= 52;

                    }

                }

            }

        }

        /* Buffer display position */
        glRasterPos2i( 0, er_coffset );

        /* Display buffer */
        glDrawPixels( er_cwidth, er_cheight, GL_RGBA, GL_UNSIGNED_BYTE, er_buffer );

    }

/*
    source - auxiliary methods
 */

    le_char_t * er_times_string( le_time_t const er_time ) {

        /* Static string array variables */
        static le_char_t er_string[256] = { 0 };

        /* Decomposed time variables */
        struct tm er_struct = * localtime( & er_time );

        /* Compose date string */
        strftime( ( char * ) er_string, sizeof( er_string ), "%Y-%m-%d-%H:%M:%S", & er_struct );

        /* Return composed string */
        return( er_string );

    }

    le_char_t * er_times_range( er_times_t const * const er_times, le_time_t const er_lbound, le_time_t const er_ubound ) {

        /* Static string array variables */
        static le_char_t er_string[256] = { 0 };

        /* Compose range string */
        sprintf( ( char * ) er_string, "%s", er_times_string( er_lbound ) );

        /* Compose range string */
        sprintf( ( char * ) er_string, "%s ( %.1fy )", er_string, ( float ) er_times->tm_zoom / ER_TIMES_YEAR );

        /* Compose range string */
        sprintf( ( char * ) er_string, "%s %s", er_string, er_times_string( er_ubound ) );

        /* Return composed string */
        return( er_string );

    }

