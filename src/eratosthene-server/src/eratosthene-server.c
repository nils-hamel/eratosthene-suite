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

    # include "eratosthene-server.h"

/*
    source - main function
 */

    int main( int argc, char ** argv ) {

        /* Socket handle variables */
        le_sock_t er_server = _LE_SOCK_NULL;

        /* System handle variables */
        le_system_t er_system = LE_SYSTEM_C;

        /* Create system handle */
        if ( ( er_system = le_system_create( ( le_char_t * ) lc_read_string( argc, argv, "--path", "-p" ) ) )._status != LE_ERROR_SUCCESS ) {

            /* Display message */
            fprintf( stderr, "eratosthene-server : error : unable to create system handle\n" );

        } else {

            /* Create server handle */
            if ( ( er_server = le_server_create( lc_read_uint( argc, argv, "--port", "-t", _LE_USE_PORT ) ) ) == _LE_SOCK_NULL ) {

                /* Display message */
                fprintf( stderr, "eratosthene-server : error : unable to create server handle\n" );

            } else {

                /* Display message */
                fprintf( stderr, "eratosthene-server : server listening on TCP/IP\n" );

                /* Server handle and idle */
                le_server( er_server, & er_system );

                /* Delete server handle */
                er_server = le_server_delete( er_server );

            }

            /* Delete system handle */
            er_system = le_system_delete( & er_system );

        }

        /* Return to system */
        return( EXIT_SUCCESS );

    }

