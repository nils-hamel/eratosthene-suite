/*
 *  eratosthene-suite - info
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

    # include "eratosthene-info.h"

/*
    source - information method
 */

    le_enum_t er_information( le_sock_t const er_socket, le_char_t const * const er_addr, le_enum_t const er_port ) {

        /* socket-array variable */
        le_array_t er_array = LE_ARRAY_C;

        /* server parameter variable */
        le_size_t er_space = _LE_SIZE_NULL;

        /* server parameter variable */
        le_time_t er_time = _LE_TIME_NULL;

        /* serialisation variable */
        le_size_t er_serial = 0;

        /* initialise array */
        le_array_set_size( & er_array, 0 );

        /* write socket-array */
        le_array_io_put( & er_array, NULL, LE_MODE_AUTH, er_socket );

        /* read socket-array */
        if ( le_array_io_get( & er_array, NULL, er_socket ) != LE_MODE_AUTH ) {

            /* send message */
            return( _LE_FALSE );

        }

        /* serialise parameter */
        er_serial = le_array_serial( & er_array, & er_space, sizeof( le_size_t ), er_serial, _LE_GET );
        er_serial = le_array_serial( & er_array, & er_time , sizeof( le_time_t ), er_serial, _LE_GET );

        /* display information */
        printf( "%s@%" _LE_ENUM_P " : %" _LE_SIZE_P ", %" _LE_TIME_P "\n", er_addr, er_port, er_space, er_time );

        /* send message */
        return( _LE_TRUE );

    }

/*
    source - main method
 */

    int main( int argc, char ** argv ) {

        /* socket variable */
        le_sock_t er_socket = _LE_SOCK_NULL;

        /* address variable */
        le_char_t * er_addr = ( le_char_t * ) lc_read_string( argc, argv, "--ip", "-i" );

        /* service variable */
        le_enum_t er_port = lc_read_unsigned( argc, argv, "--port", "-p", _LE_USE_PORT );

        /* message variable */
        le_enum_t er_message = EXIT_SUCCESS;

        /* create socket */
        if ( ( er_socket = le_client_create( er_addr, er_port ) ) == _LE_SOCK_NULL ) {

            /* display message */
            fprintf( stderr, "eratosthene-suite : error : unable to establish connection\n" );

            /* push message */
            er_message = EXIT_FAILURE;

        } else {

            /* query and display information */
            if ( er_information( er_socket, er_addr, er_port ) != _LE_TRUE ) {

                /* display message */
                fprintf( stderr, "eratosthene-suite : error : unable to query information\n" );

                /* push message */
                er_message = EXIT_FAILURE;

            }

            /* delete client */
            er_socket = le_client_delete( er_socket );

        }

        /* send message */
        return( er_message );

    }

