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

    # include "eratosthene-client-server.h"

/*
    source - server array methods
 */

    le_enum_t er_server_array( le_char_t const * const er_ip, le_sock_t const er_port, le_enum_t const er_mode, le_array_t * const er_array ) {

        /* Socket variables */
        le_sock_t er_socket = _LE_SOCK_NULL;

        /* Establish connection to server - send message */
        if ( ( er_socket = le_client_create( er_ip, er_port ) ) == _LE_SOCK_NULL ) return( LE_ERROR_IO_SOCKET );

        /* Server/client handshake */
        if ( le_client_handshake( er_socket, er_mode, LE_ARRAY_TFD ) == LE_ERROR_SUCCESS ) {

            /* Retreive server array */
            le_array_io_read( er_array, er_socket );

        }

        /* Close connection to server */
        le_client_delete( er_socket );

        /* Send message */
        return( LE_ERROR_SUCCESS );

    }
