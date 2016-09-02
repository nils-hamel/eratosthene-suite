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

        /* Server handle variables */
        le_server_t er_server = LE_SERVER_C;

        /* Create server handle */
        if ( ( er_server = le_server_create( lc_read_signed( argc, argv, "--port", "-t", _LE_USE_PORT ), ( le_char_t * ) lc_read_string( argc, argv, "--path", "-p" ) ) )._status != LE_ERROR_SUCCESS ) {

            /* Display message */
            fprintf( stderr, "eratosthene-suite : error : unable to create server\n" );

        } else {

            /* Display message */
            fprintf( stderr, "eratosthene-suite : state : server listening on TCP/IP\n" );

            /* Server connections management */
            le_server( & er_server );

            /* Delete server handle */
            le_server_delete( & er_server );

        }

        /* Return to system */
        return( EXIT_SUCCESS );

    }

