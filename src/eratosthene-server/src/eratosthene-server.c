/*
 *  eratosthene-suite - eratosthene indexation server front-end
 *
 *      Nils Hamel - nils.hamel@bluewin.ch
 *      Copyright (c) 2016-2017 EPFL CDH DHLAB
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

        /* server handle variables */
        le_server_t er_server = LE_SERVER_C;

        /* create server */
        if ( ( er_server = le_server_create( ( le_char_t * ) lc_read_string( argc, argv, "--path", "-p" ), lc_read_signed( argc, argv, "--port", "-t", _LE_USE_PORT ) ) )._status != LE_ERROR_SUCCESS ) {

            /* display message */
            fprintf( stderr, "eratosthene-suite : error : unable to create server\n" );

        } else {

            /* display message */
            fprintf( stderr, "eratosthene-suite : state : server listening on TCP/IP\n" );

            /* server process */
            le_server_io( & er_server );

            /* delete server */
            le_server_delete( & er_server );

        }

        /* send message */
        return( EXIT_SUCCESS );

    }

