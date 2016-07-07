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

        /* Return constructed structure */
        return( er_times );

    }

    le_void_t er_times_delete( er_times_t * const er_times ) {

        /* Deleted structure variables */
        er_times_t er_delete = ER_TIMES_C;

        /* Unallocate times array */
        le_array_delete( & er_times->tm_times );

        /* Delete structure */
        * er_times = er_delete;

    }

/*
    source - mutator methods
 */

    le_void_t er_times_set_enable( er_times_t * const er_times, le_size_t const er_index ) {

        /* Enable times */
        * ( ( ( le_time_t * ) er_times->tm_stack ) + er_index ) = * ( ( ( le_time_t * ) er_times->tm_times ) + er_index );

    }

