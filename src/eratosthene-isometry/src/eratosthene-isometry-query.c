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

    # include "eratosthene-isometry-query.h"

/*
    source - server query
 */

    le_enum_t er_query( le_sock_t const er_socket, le_char_t const * const er_query, le_array_t * const er_array ) {

        /* Parsing variables */
        le_size_t er_parse = 0;

        /* Socket i/o count variables */
        le_size_t er_count = 0;

        /* Array pointer variables */
        le_real_t * er_ptrp = NULL;
        le_time_t * er_ptrt = NULL;
        le_data_t * er_ptrd = NULL;

        /* Socket i/o buffer variables */
        le_byte_t er_buffer[LE_NETWORK_BUFFER_SYNC] = LE_NETWORK_BUFFER_C;

        /* Check socket */
        if ( er_socket == _LE_SOCK_NULL ) {

            /* Send message */
            return( _LE_FALSE );

        }

        /* Query string to socket buffer */
        strcpy( ( char * ) er_buffer, ( char * ) er_query );

        /* Write query address */
        if ( write( er_socket, er_buffer, LE_NETWORK_BUFFER_ADDR ) != LE_NETWORK_BUFFER_ADDR ) {

            /* Send message */
            return( _LE_FALSE );

        }

        /* Reading query elements */
        while( ( er_count = read( er_socket, er_buffer, LE_NETWORK_BUFFER_SYNC ) ) > 0 ) {

            /* Parsing received elements */
            for ( er_parse = 0; er_parse < er_count; er_parse += LE_ARRAY_LINE ) {

                /* Compute pointers */
                er_ptrp = ( le_real_t * ) ( er_buffer + er_parse );
                er_ptrt = ( le_time_t * ) ( er_ptrp + 3 );
                er_ptrd = ( le_data_t * ) ( er_ptrt + 1 );

                /* Push element */
                le_array_set_push( er_array, er_ptrp, * er_ptrt, er_ptrd );

            }

        }

        /* Send message */
        return( _LE_TRUE );

    }

