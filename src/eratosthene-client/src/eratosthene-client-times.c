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

        /* Query times array */
        er_times.tm_times = er_client_server_times( er_ip, er_port );

        /* Retrieve server configuration */
        if ( ( er_times.tm_tdis = er_client_server_tparam( er_ip, er_port ) ) == _LE_TIME_NULL ) {

            /* Send message */
            er_times._status = _LE_FALSE; return( er_times );

        }

        /* Check array size */
        if ( le_array_get_size( & er_times.tm_times ) <= 0 ) {

            /* Send message */
            er_times._status = _LE_FALSE; return( er_times );

        }

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
    source - mutator methods
 */

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

        /* Parsing times array */
        for ( le_size_t er_index = 0; er_index < le_array_get_record( & er_times->tm_times, LE_ARRAY_64T_LEN ); er_index ++ ) {

            /* Check for SRTM data */
            if ( er_ref[er_index] == ER_TIMES_SRTM( er_times->tm_tdis ) ) {

                /* Enable time at index */
                er_sel[er_index] = er_ref[er_index];

            }

        }

    }

/*
    source - display methods
 */

    le_void_t er_times_display( er_times_t const * const er_times ) {

        /* Screen variables */
        le_size_t er_wscr = glutGet( GLUT_SCREEN_WIDTH  );
        le_size_t er_hscr = glutGet( GLUT_SCREEN_HEIGHT );

        /* Initialise position */
        le_size_t er_x = er_wscr * 0.1;
        le_size_t er_y = er_hscr * 0.9;

        /* Array base pointers variables */
        le_time_t * er_ref = ( le_time_t * ) le_array_get_byte( & er_times->tm_times );
        le_time_t * er_sel = ( le_time_t * ) le_array_get_byte( & er_times->tm_stack );

        /* String variables */
        le_char_t er_value[256] = { 0 };

        /* Parsing selection array */
        for ( le_size_t er_parse = 0; er_parse < le_array_get_record( & er_times->tm_times, LE_ARRAY_64T_LEN ); er_parse ++ ) {

            /* Check selection state */
            if ( er_sel[er_parse] != _LE_TIME_NULL ) glColor3f( 1.0, 1.0, 1.0 ); else glColor3f( 0.5, 0.5, 0.5 );

            sprintf( ( char * ) er_value, "%" _LE_TIME_P, er_ref[er_parse] / 31536000 + 1970 );

            /* Position */
            glWindowPos2i( er_x, er_y ); er_y += 10;

            /* Display time */
            glutBitmapString( GLUT_BITMAP_HELVETICA_10, er_value );

        }

        er_times_buffer( er_times );

    }

    le_void_t er_times_buffer( er_times_t const * const er_times ) {

        /* Graphical buffer */
        static le_byte_t er_buffer[ER_TIMES_CW * ER_TIMES_CH * 4] = { 0 };

        if ( er_buffer[0] == 0 ) memset( er_buffer, 255, ER_TIMES_CW * ER_TIMES_CH * 4 );

        /* Draw buffer */
        for ( le_size_t er_parse = 3; er_parse < ER_TIMES_CW * ER_TIMES_CH * 4; er_parse += 4 ) {

            le_size_t er_x = ( er_parse - 3 ) / 4 % ER_TIMES_CW;

            if ( er_x % 10 == 0 ) er_buffer[er_parse] = 255; else
            er_buffer[er_parse] = 128;

        }

        glEnable( GL_BLEND );
        glWindowPos2i( ( glutGet( GLUT_SCREEN_WIDTH  ) - ER_TIMES_CW ) / 2, ER_TIMES_CP );
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDrawPixels( ER_TIMES_CW, ER_TIMES_CH, GL_RGBA, GL_UNSIGNED_BYTE, er_buffer );

    }

