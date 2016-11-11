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

        /* created structure variables */
        er_times_t er_times = ER_TIMES_C;

        /* array variables */
        le_array_t er_array  = LE_ARRAY_C;

        /* array access_variables */
        le_array_cf_t er_access = LE_ARRAY_CF_C;

        /* assign server connection parameters */
        er_times.tm_svip = er_ip;
        er_times.tm_port = er_port;

        /* query server configuration array */
        if ( le_client_array( er_ip, er_port, LE_NETWORK_MODE_CMOD, & er_array ) != LE_ERROR_SUCCESS ) {

            /* send message */
            return( er_times._status = _LE_FALSE, er_times );

        } else {

            /* compute configuration array mapping */
            le_array_cf( le_array_get_byte( & er_array ), 0, er_access );

            /* retrieve configuration */
            er_times.tm_tparam = er_access.as_time[0];

        }

        /* query server times array */
        if ( le_client_array( er_ip, er_port, LE_NETWORK_MODE_AMOD, & er_times.tm_tarray ) != LE_ERROR_SUCCESS ) {

            /* send message */
            return( er_times._status = _LE_FALSE, er_times );

        } else {

            /* compute times array size */
            er_times.tm_size = le_array_get_size( & er_times.tm_tarray ) / LE_ARRAY_TFL;

            /* compute times array base */
            er_times.tm_time = ( le_time_t * ) le_array_get_byte( & er_times.tm_tarray );

        }

        /* set default time */
        er_times_set_default( & er_times );

        /* set nearest time */
        er_times_set_nearest( & er_times );

        /* return constructed structure */
        return( er_times );

    }

    le_void_t er_times_delete( er_times_t * const er_times ) {

        /* deleted structure variables */
        er_times_t er_delete = ER_TIMES_C;

        /* unallocate times array */
        le_array_delete( & er_times->tm_tarray );

        /* delete structure */
        * er_times = er_delete;

    }

/*
    source - accessor methods
 */

    le_address_t er_times_get( er_times_t const * const er_times ) {

        /* returned structure variables */
        le_address_t er_addr = LE_ADDRESS_C;

        /* parsing viewable times */
        for ( le_size_t er_parse = 0; er_parse < _LE_USE_TIMES; er_parse ++ ) {

            /* check time allocation state */
            if ( er_times->tm_view[er_parse] != _LE_SIZE_NULL ) {

                /* assign address viewable time */
                le_address_set_time( & er_addr, er_parse, er_times->tm_time[er_times->tm_view[er_parse]] );

            } else {

                /* assign address null time */
                le_address_set_time( & er_addr, er_parse, _LE_TIME_NULL );

            }

        }

        /* return structure */
        return( er_addr );

    }

    le_time_t er_times_get_time( er_times_t * const er_times, le_size_t const er_offset ) {

        /* return time value */
        return( er_times->tm_time[er_times->tm_view[er_offset]] );

    }

/*
    source - mutator methods
 */

    le_void_t er_times_set( er_times_t * const er_times, le_size_t const er_index ) {

        /* check consistency */
        if ( ( er_index >= 0 ) && ( er_index < ER_TIMES_VIEW ) ) {

            /* check enable time state */
            if ( er_times->tm_view[er_index] != er_times->tm_near ) {
            
                /* enable highlighted time */
                er_times->tm_view[er_index] = er_times->tm_near;

            } else {

                /* disable highlighted time */
                er_times->tm_view[er_index] = _LE_SIZE_NULL;

            }

        }

    }

    le_void_t er_times_set_default( er_times_t * const er_times ) {

        /* flag variables */
        le_enum_t er_flag = _LE_FALSE;

        /* parsing times array */
        for ( le_size_t er_parse = 0; er_parse < er_times->tm_size; er_parse ++ ) {

            /* search for SRTM data */
            if ( er_times->tm_time[er_parse] == ER_TIMES_SRTM( er_times->tm_tparam ) ) {
                
                /* enable default time */
                er_times->tm_pose = er_times->tm_time[er_times->tm_view[0] = er_parse];

            /* update state flag */
            er_flag = _LE_TRUE; }

        }

        /* check flag - enable first time as default */
        if ( er_flag == _LE_FALSE ) er_times->tm_pose = er_times->tm_time[er_times->tm_view[0] = 0];

    }

    le_void_t er_times_set_nearest( er_times_t * const er_times ) {

        /* distance variables */
        le_time_t er_distance = _LE_TIME_MAX, er_current = _LE_TIME_NULL;

        /* parsing times array */
        for ( le_size_t er_parse = 0; er_parse < er_times->tm_size; er_parse ++ ) {

            /* check current distance */
            if ( ( er_current = le_time_abs( er_times->tm_time[er_parse] - er_times->tm_pose ) ) < er_distance ) {

                /* update nearest index and distance */
                er_times->tm_near = er_parse;

            /* update distance */
            er_distance = er_current; }

        }

    }

    le_void_t er_times_set_zoom( er_times_t * const er_times, le_real_t const er_factor ) {

        /* update zoom value */
        er_times->tm_zoom = lc_clamp( er_times->tm_zoom * er_factor, ER_TIMES_ZOOM_MIN, ER_TIMES_ZOOM_MAX );

    }

    le_void_t er_times_set_pose( er_times_t * const er_times, le_real_t const er_factor ) {

        /* update position value */
        er_times->tm_pose += er_times->tm_zoom * er_factor;

        /* update nearest times */
        er_times_set_nearest( er_times );

    }

    le_void_t er_times_set_reset( er_times_t * const er_times, le_size_t const er_index ) {

        /* check consistency */
        if ( ( er_index >= 0 ) && ( er_index < ER_TIMES_VIEW ) ) {

            /* check time state */
            if ( er_times->tm_view[er_index] != _LE_SIZE_NULL ) {

                /* set time position to enable time */
                er_times->tm_pose = er_times->tm_time[er_times->tm_view[er_index]];

                /* assign implicit nearest */
                er_times->tm_near = er_times->tm_view[er_index];

            }

        }

    }

/*
    source - display methods
 */

    le_void_t er_times_display( er_times_t const * const er_times ) {

        /* static buffer display variables */
        static le_size_t er_xsize = 0;
        static le_size_t er_ysize = 96;
        static le_size_t er_wsize = 0;
        static le_size_t er_bsize = 0;

        static le_size_t er_heig0 = 32;
        static le_size_t er_heig1 = 24;
        static le_size_t er_heig2 = 60;
        static le_size_t er_heig3 = 68;
        static le_size_t er_heig4 = 76;

        /* static buffer variables */
        static le_byte_t * er_buffer = NULL;

        /* graduation display variables */
        le_time_t er_xgrad = 0;
        le_size_t er_ygrad = 0;
        le_time_t er_dgrad = ER_TIMES_GRAD_SCALE;

        /* boundaries variables */
        le_time_t er_lbound = er_times->tm_pose - ( er_times->tm_zoom >> 1 );
        le_time_t er_ubound = er_times->tm_pose + ( er_times->tm_zoom >> 1 );

        /* check buffer state */
        if ( er_buffer == NULL ) {

            /* compute buffer dimensions */
            er_xsize = glutGet( GLUT_SCREEN_WIDTH );

            /* compute buffer size */
            er_wsize = er_xsize << 2;
            er_bsize = er_wsize * er_ysize;

            /* allocate buffer memory - return */
            if ( ( er_buffer = malloc( er_bsize ) ) == NULL ) return; 

            /* initialise buffer bytes */
            memset( er_buffer, 255, er_bsize );

        }

        /* reset buffer bytes */
        for ( le_size_t er_i = 3, er_j = 0; er_i < er_bsize; er_i += 4, er_j = er_i / er_wsize ) {

            /* check heights */
            if ( er_j == er_heig2 ) {

                /* reset pixel alpha channel */
                er_buffer[er_i] = 128;

            } else if ( er_j < er_heig1 ) {

                /* reset pixel alpha channel */
                er_buffer[er_i] = 224;

            } else {

                /* reset pixel alpha channel */
                er_buffer[er_i] = 216;

            }

        }

        /* display graduations */
        for ( le_size_t er_scale = 0; er_scale < ER_TIMES_GRAP_DEPTH; er_dgrad /= 10, er_scale ++ ) {

            /* check graduation spacing */
            if ( ( ( ( le_real_t ) er_dgrad ) / er_times->tm_zoom ) * er_xsize < 6 ) continue;

            /* display graduation */
            for ( le_time_t er_parse = ER_TIMES_ROUND( er_lbound, er_dgrad ); er_parse < er_ubound; er_parse += er_dgrad ) {

                /* compute graduation position x-position */
                er_xgrad = ( ( ( ( le_real_t ) er_parse ) - er_lbound ) / er_times->tm_zoom ) * er_xsize;

                /* compute graduation position y-position */
                er_ygrad = er_heig1 + ( er_scale << 2 ) + 2;

                /* display graduation increment */
                for ( le_size_t er_pixel = er_ygrad; er_pixel < er_heig2; er_pixel ++ ) {

                    /* update interface buffer alpha channel */
                    er_buffer[( ( er_xgrad + er_pixel * er_xsize ) << 2 ) + 3] -= 32;

                }

            }

        }

        /* display boundaries times */
        er_times_display_date( er_lbound, 16, er_heig0 + 20, ER_TIMES_JUST_LEFT );

        /* display boundaries times */
        er_times_display_date( er_ubound, er_xsize - 16, er_heig0 + 20, ER_TIMES_JUST_RIGHT );

        /* display selected times */
        if ( er_times->tm_view[0] != _LE_SIZE_NULL ) er_times_display_date( er_times->tm_time[er_times->tm_view[0]], ( er_xsize >> 1 ) - 16, er_heig0 + 20, ER_TIMES_JUST_RIGHT );

        /* display selected times */
        if ( er_times->tm_view[1] != _LE_SIZE_NULL ) er_times_display_date( er_times->tm_time[er_times->tm_view[1]], ( er_xsize >> 1 ) + 16, er_heig0 + 20, ER_TIMES_JUST_LEFT );
        
        /* display dynamic times */
        for ( le_size_t er_parse = 0; er_parse < er_times->tm_size; er_parse ++ ) {

            /* check time visibility */
            if ( ( er_times->tm_time[er_parse] < er_lbound ) || ( er_times->tm_time[er_parse] > er_ubound ) ) continue;

            /* compute time x-position */
            er_xgrad = er_xsize * ( ( le_real_t ) ( er_times->tm_time[er_parse] - er_lbound ) / er_times->tm_zoom );

            /* check time highlighting */
            if ( er_parse == er_times->tm_near ) {

                /* display date string */
                er_times_display_date( er_times->tm_time[er_parse], er_xgrad, er_heig0 + er_ysize - 2, ER_TIMES_JUST_LEFT );

            /* time marker configuration */
            er_ygrad = er_heig4; } else { er_ygrad = er_heig3; }

            /* display time marker */
            for ( le_size_t er_pixel = er_heig2 + 1; er_pixel < er_ygrad; er_pixel ++ ) {

                /* update interface buffer alpha channel */
                er_buffer[( ( er_xgrad + er_pixel * er_xsize ) << 2 ) + 3] = 128;

            }

        }

        /* position buffer */
        glRasterPos2i( 0, er_heig0 );

        /* display buffer */
        glDrawPixels( er_xsize, er_ysize, GL_RGBA, GL_UNSIGNED_BYTE, er_buffer );

    }

    le_void_t er_times_display_date( le_time_t const er_time, le_size_t er_x, le_size_t er_y, le_enum_t const er_justify ) {

        /* string array variables */
        static le_char_t er_string[32] = { 0 };

        /* compose date string */
        lc_time_to_string( er_time, er_string, 32 );

        /* check justification - assign shift */
        if ( er_justify == ER_TIMES_JUST_RIGHT  ) er_x -= strlen( ( char * ) er_string ) * 8;
        if ( er_justify == ER_TIMES_JUST_CENTER ) er_x -= strlen( ( char * ) er_string ) * 4;

        /* assign string color */
        glColor3f( 0.18, 0.22, 0.28 );

        /* assign string position */
        glRasterPos2i( er_x, er_y - 13 );

        /* display string */
        glutBitmapString( GLUT_BITMAP_8_BY_13, er_string );

    }

