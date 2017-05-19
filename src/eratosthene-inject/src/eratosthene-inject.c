/*
 *  eratosthene-suite - inject
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

    # include "eratosthene-inject.h"

/*
    source - injection method - uf3
 */

    le_enum_t er_inject_uf3( int argc, char ** argv ) {

        /* socket variables */
        le_sock_t er_socket = _LE_SOCK_NULL;

        /* stream variables */
        le_file_t er_stream = NULL;

        /* injection time variables */
        le_time_t er_time = lc_read_signed( argc, argv, "--time", "-t", _LE_TIME_NULL );

        /* reading variables */
        le_size_t er_read = 0;

        /* array pointer variables */
        le_byte_t * er_base = NULL;

        /* socket-array variables */
        le_array_t er_head = LE_ARRAY_C;
        le_array_t er_data = LE_ARRAY_C;
        le_array_t er_dual = LE_ARRAY_C;

        /* check consistency */
        if ( er_time == _LE_TIME_NULL ) {

            /* display message */
            fprintf( stderr, "eratosthene-suite : error : injection time\n" );

            /* send message */
            return( EXIT_FAILURE );

        }

        /* create socket-array */
        if ( le_array_set_size( & er_head, LE_ARRAY_INJE_HEAD ) == _LE_FALSE ) {

            /* display message */
            fprintf( stderr, "eratosthene-suite : error : memory allocation\n" );

            /* send message */
            return( EXIT_FAILURE );

        }

        /* create socket-array */
        if ( le_array_set_size( & er_data, LE_ARRAY_STEP ) == _LE_FALSE ) {

            /* display message */
            fprintf( stderr, "eratosthene-suite : error : memory allocation\n" );

            /* send message */
            return( EXIT_FAILURE );

        }

        /* create client socket */
        if ( ( er_socket = le_client_create( ( le_char_t * ) lc_read_string( argc, argv, "--ip", "-i" ), lc_read_signed( argc, argv, "--port", "-t", _LE_USE_PORT ) ) ) == _LE_SOCK_NULL ) {

            /* display message */
            fprintf( stderr, "eratosthene-suite : error : server connection\n ");

            /* send message */
            return( EXIT_FAILURE );

        }

        /* create stream */
        if ( ( er_stream = fopen( lc_read_string( argc, argv, "--uf3", "" ), "rb" ) ) == NULL ) {

            /* display message */
            fprintf( stderr, "eratosthene-suite : error : unable to create input stream\n" );

            /* send message */
            return( EXIT_FAILURE );

        }

        /* serialise time */
        le_array_serial( & er_head, & er_time, sizeof( le_time_t ), 0, _LE_SET );

        /* retreive array pointer */
        er_base = le_array_get_byte( & er_data );

        /* read stream */
        while ( ( er_read = fread( er_base, sizeof( le_byte_t ), ER_INJECT * LE_ARRAY_UF3, er_stream ) ) > 0 ) {

            /* write socket-array - injection head */
            le_array_io_write( & er_head, LE_MODE_INJE, er_socket );

            /* update array size */
            le_array_set_size( & er_data, er_read );

            /* encode socket-array */
            le_array_uf3_encode( & er_data, & er_dual );

            /* write socket array */
            le_array_io_write( & er_dual, LE_MODE_INJE, er_socket );

        }

        /* delete stream */
        fclose( er_stream );

        /* delete client socket */
        le_client_delete( er_socket );

        /* delete socket-array */
        le_array_delete( & er_head );
        le_array_delete( & er_data );
        le_array_delete( & er_dual );

        /* send message */
        return( EXIT_SUCCESS );

    }

/*
    source - main method
 */

    int main( int argc, char ** argv ) {

        /* detect input stream format */
        if ( lc_read_flag( argc, argv, "--uf3", "" ) == LC_TRUE ) {

            /* call specialised method */
            return( er_inject_uf3( argc, argv ) );

        } else {

            /* display message */
            fprintf( stderr, "eratosthene-suite : error : unsupported format\n" );

            /* return to system */
            return( EXIT_FAILURE );

        }

    }

