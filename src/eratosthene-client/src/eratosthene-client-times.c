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

        /* Set defaults */
        er_times_set_default( & er_times );

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

    le_void_t er_times_set_enable( er_times_t * const er_times, le_size_t const er_index ) {

        /* Array base pointers variables */
        le_time_t * er_ref = ( le_time_t * ) le_array_get_byte( & er_times->tm_times );
        le_time_t * er_sel = ( le_time_t * ) le_array_get_byte( & er_times->tm_stack );

        /* Enable time at index */
        er_ref[er_index] = er_sel[er_index];

    }

    le_void_t er_times_set_disable( er_times_t * const er_times, le_size_t const er_index ) {

        /* Array base pointers variables */
        le_time_t * er_sel = ( le_time_t * ) le_array_get_byte( & er_times->tm_stack );

        /* Enable time at index */
        er_sel[er_index] = _LE_TIME_NULL;

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
        er_times->tm_zoom *= ( er_mode == ER_TIMES_IZOOM ? 0.91 : 1.1 );

        /* Check limitation */
        if ( er_times->tm_zoom < ER_TIMES_DAY * 5 ) er_times->tm_zoom = ER_TIMES_DAY * 5;
        if ( er_times->tm_zoom > ER_TIMES_YEAR * 1000 ) er_times->tm_zoom = ER_TIMES_YEAR * 1000;

        /* Update nearest times */
        er_times_set_nearest( er_times );

    }

    le_void_t er_times_set_pose( er_times_t * const er_times, le_size_t const er_mode ) {

        /* Update position value */
        er_times->tm_pose += er_times->tm_zoom * ( er_mode == ER_TIMES_DPOSE ? -0.02 : 0.02 );

        /* Update nearest times */
        er_times_set_nearest( er_times );

    }

    le_void_t er_times_set_time( er_times_t * const er_times, le_size_t const er_mode ) {

        /* Check mode */
        if ( er_mode == ER_TIMES_DTIME ) {

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
        static le_size_t er_coffset = 0;

        /* Static buffer variables */
        static le_byte_t * er_buffer = NULL;

        /* Check buffer state */
        if ( er_buffer == NULL ) {

            /* Compute buffer sizes */
            er_cwidth  = glutGet( GLUT_SCREEN_WIDTH  );
            er_cheight = glutGet( GLUT_SCREEN_HEIGHT ) * 0.07;
            er_coffset = glutGet( GLUT_SCREEN_HEIGHT ) * 0.07;

            /* Allocate buffer memory */
            er_buffer  = ( le_byte_t * ) malloc( er_cwidth * er_cheight * 4 );

        return; }

        /* String buffer variables */
        le_char_t er_string[256] = { 0 };

        le_size_t er_count = 0;
        le_size_t er_point = 0;

        /* Boundaries variables */
        le_time_t er_lbound = er_times->tm_pose - ( er_times->tm_zoom >> 1 );
        le_time_t er_ubound = er_times->tm_pose + ( er_times->tm_zoom >> 1 );

        /* Array base pointers variables */
        le_time_t * er_ref = ( le_time_t * ) le_array_get_byte( & er_times->tm_times );
        le_time_t * er_sel = ( le_time_t * ) le_array_get_byte( & er_times->tm_stack );

        /* Initialise buffer bytes */
        for ( le_size_t er_parse = 0; er_parse < er_cwidth * er_cheight * 4; er_parse ++ ) {

            /* Check congurence */
            if ( ( ( er_parse + 1 ) % 4 ) == 0 ) er_buffer[er_parse] = 192; else er_buffer[er_parse] = 255;

        }

        /* Display times stack */
        for ( le_size_t er_parse = 0; er_parse < er_times->tm_count; er_parse ++ ) {

            /* Check time visibility */
            if ( ( er_ref[er_parse] > er_lbound ) && ( er_ref[er_parse] < er_ubound ) ) {

                /* Check colorisation */
                if ( er_parse == er_times->tm_curr ) {

                    /* Push color */
                    glColor3f( 1.0, 0.2, 0.1 );

                } else
                if ( er_sel[er_parse] != _LE_TIME_NULL ) {

                    /* Push color */
                    glColor3f( 0.5, 0.5, 0.5 );

                } else {

                    /* Push color */
                    glColor3f( 0.75, 0.75, 0.75 );

                }

                /* Compute time string */
                sprintf( ( char * ) er_string, "%" _LE_TIME_P, er_ref[er_parse] / 31536000 + 1970 );

                /* Time string position */
                glRasterPos2i( er_cwidth * ( ( le_real_t ) ( er_ref[er_parse] - er_lbound ) / er_times->tm_zoom ), er_coffset + 4 );

                /* Display time string */
                glutBitmapString( GLUT_BITMAP_8_BY_13, er_string );

            }

        }

        le_size_t er_edge = 16;

        for ( le_size_t er_parse = ( er_lbound / ER_TIMES_DAY ) * ER_TIMES_DAY + ER_TIMES_DAY; er_parse < er_ubound; er_parse += ER_TIMES_DAY ) {

            /* Compute position */
            er_point = ( ( le_real_t ) ( er_parse - er_lbound ) / er_times->tm_zoom ) * er_cwidth;

            /* Display bar */
            for ( le_size_t er_pixel = 20 + er_edge; er_pixel < er_cheight - er_edge; er_pixel ++ ) {

                er_buffer[(er_point+er_pixel*er_cwidth)*4  ] = 128;
                er_buffer[(er_point+er_pixel*er_cwidth)*4+1] = 128;
                er_buffer[(er_point+er_pixel*er_cwidth)*4+2] = 128;

            }

        }

        er_edge = 8;

        for ( le_size_t er_parse = ( er_lbound / ER_TIMES_MONTH ) * ER_TIMES_MONTH + ER_TIMES_MONTH; er_parse < er_ubound; er_parse += ER_TIMES_MONTH ) {

            /* Compute position */
            er_point = ( ( le_real_t ) ( er_parse - er_lbound ) / er_times->tm_zoom ) * er_cwidth;

            /* Display bar */
            for ( le_size_t er_pixel = 20 + er_edge; er_pixel < er_cheight - er_edge; er_pixel ++ ) {

                er_buffer[(er_point+er_pixel*er_cwidth)*4  ] = 192;
                er_buffer[(er_point+er_pixel*er_cwidth)*4+1] = 192;
                er_buffer[(er_point+er_pixel*er_cwidth)*4+2] = 192;

            }

        }

        er_edge = 2;

        for ( le_size_t er_parse = ( er_lbound / ER_TIMES_YEAR ) * ER_TIMES_YEAR + ER_TIMES_YEAR; er_parse < er_ubound; er_parse += ER_TIMES_YEAR ) {

            /* Compute position */
            er_point = ( ( le_real_t ) ( er_parse - er_lbound ) / er_times->tm_zoom ) * er_cwidth;

            /* Display bar */
            for ( le_size_t er_pixel = 20 + er_edge; er_pixel < er_cheight - er_edge; er_pixel ++ ) {

                er_buffer[(er_point+er_pixel*er_cwidth)*4  ] = 220;
                er_buffer[(er_point+er_pixel*er_cwidth)*4+1] = 220;
                er_buffer[(er_point+er_pixel*er_cwidth)*4+2] = 220;

            }

        }

        /* Buffer display position */
        glRasterPos2i( 0, er_coffset );

        /* Display buffer */
        glDrawPixels( er_cwidth, er_cheight, GL_RGBA, GL_UNSIGNED_BYTE, er_buffer );

    }

