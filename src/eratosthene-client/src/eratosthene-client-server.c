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
    source - server times access methods
 */

    le_array_t er_client_server_times( le_char_t const * const er_ip, le_sock_t const er_port ) {

        /* Returned structure variables */
        le_array_t er_array = LE_ARRAY_C;

        /* Socket variables */
        le_sock_t er_socket = _LE_SOCK_NULL;

        /* Establish server connection */
        if ( ( er_socket = le_client_create( er_ip, er_port ) ) != _LE_SOCK_NULL ) {

            /* Client/server query handshake */
            if ( le_client_handshake( er_socket, LE_NETWORK_MODE_AMOD, LE_ARRAY_64T ) == LE_ERROR_SUCCESS ) {

                /* Retrieve times array */
                le_array_io_read( & er_array, er_socket );

            }

            /* Close server connection */
            er_socket = le_client_delete( er_socket );

        }

        /* Return times array */
        return( er_array );

    }

/*
    source - server parameters access methods
 */

    le_size_t er_client_server_sparam( le_char_t const * const er_ip, le_sock_t const er_port ) {

        /* Returned value variables */
        le_size_t er_return = _LE_SIZE_NULL;

        /* Socket variables */
        le_sock_t er_socket = _LE_SOCK_NULL;

        /* Establish server connection */
        if ( ( er_socket = le_client_create( er_ip, er_port ) ) != _LE_SOCK_NULL ) {

            /* Client/server query handshake */
            if ( le_client_handshake( er_socket, LE_NETWORK_MODE_SMOD, LE_ARRAY_NULL ) == LE_ERROR_SUCCESS ) {

                /* Retrieve server parameter */
                er_return = le_client_system_sparam( er_socket );

            }

            /* Close server connection */
            er_socket = le_client_delete( er_socket );

        }

        /* Return server parameter */
        return( er_return );

    }

    le_time_t er_client_server_tparam( le_char_t const * const er_ip, le_sock_t const er_port ) {

        /* Returned value variables */
        le_time_t er_return = _LE_SIZE_NULL;

        /* Socket variables */
        le_sock_t er_socket = _LE_SOCK_NULL;

        /* Establish server connection */
        if ( ( er_socket = le_client_create( er_ip, er_port ) ) != _LE_SOCK_NULL ) {

            /* Client/server query handshake */
            if ( le_client_handshake( er_socket, LE_NETWORK_MODE_TMOD, LE_ARRAY_NULL ) == LE_ERROR_SUCCESS ) {

                /* Retrieve server parameter */
                er_return = le_client_system_tparam( er_socket );

            }

            /* Close server connection */
            er_socket = le_client_delete( er_socket );

        }

        /* Return server parameter */
        return( er_return );

    }

