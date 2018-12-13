/*
 *  eratosthene-suite - server
 *
 *      Nils Hamel - nils.hamel@bluewin.ch
 *      Copyright (c) 2016-2018 DHLAB, EPFL
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

        /* server variable */
        le_server_t er_server = LE_SERVER_C;

        /* service path variable */
        le_char_t * er_path = ( le_char_t * ) lc_read_string( argc, argv, "--path", "-p" );

        /* service port variable */
        le_enum_t er_port = ( le_enum_t ) lc_read_unsigned( argc, argv, "--port", "-t", _LE_USE_PORT );

        /* message variable */
        le_enum_t er_message = EXIT_SUCCESS;

        /* create server */
        if ( le_get_status( er_server = le_server_create( er_path, er_port ) ) != LE_ERROR_SUCCESS ) {

            /* display message */
            lc_error( "server creation" );

            /* push message */
            er_message = EXIT_FAILURE;

        } else {

            /* display message */
            printf( LC_PROJECT " : listening on TCP/IP with port %" _LE_ENUM_P "\n", er_port );

            /* server process */
            le_server_io( & er_server );

            /* delete server */
            le_server_delete( & er_server );

        }

        /* send message */
        return( er_message );

    }

