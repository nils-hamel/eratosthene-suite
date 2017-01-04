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

    # include "eratosthene-inject.h"

/*
    source - injection method - uf3
 */

    le_enum_t er_inject_uf3( int argc, char ** argv ) {

        /* segment variables */
        le_size_t er_count = 0;

        /* time variables */
        le_time_t er_time = lc_read_signed( argc, argv, "--time", "-t", time( NULL ) );

        /* stream variables */
        le_file_t er_stream = NULL;

        /* array variables */
        le_array_t er_array = LE_ARRAY_C;

        /* socket variables */
        le_sock_t er_client = _LE_SOCK_NULL;

        /* setting array size */
        if ( le_array_set_size( & er_array, LE_ARRAY_SD * ER_INJECT ) == _LE_FALSE ) {

            /* display message */
            fprintf( stderr, "eratosthene-suite : error : unable to allocate memory\n" );

            /* send message */
            return( EXIT_FAILURE );

        }

        /* create stream */
        if ( ( er_stream = fopen( lc_read_string( argc, argv, "--uf3", "" ), "rb" ) ) == NULL ) {

            /* display message */
            fprintf( stderr, "eratosthene-suite : error : unable to access uf3 file\n" );

            /* delete array - send message */
            return( le_array_delete( & er_array ), EXIT_FAILURE );

        }

        /* read stream by segment */
        while ( ( er_count = fread( le_array_get_byte( & er_array ), sizeof( le_byte_t ), LE_ARRAY_SD * ER_INJECT, er_stream ) ) > 0 ) {

            /* create socket */
            if ( ( er_client = le_client_create( ( le_char_t * ) lc_read_string( argc, argv, "--ip", "-i" ), lc_read_signed( argc, argv, "--port", "-t", _LE_USE_PORT ) ) ) == _LE_SOCK_NULL ) {

                /* display message */
                fprintf( stderr, "eratosthene-suite : error : unable to connect to server\n" );

                /* delete array - send message */
                return( le_array_delete( & er_array ), EXIT_FAILURE );

            }

            /* client/server handshake */
            if ( le_client_handshake( er_client, LE_MODE_IMOD ) != LE_ERROR_SUCCESS ) {

                /* display message */
                fprintf( stderr, "eratosthene-suite : error : server authorisation failed\n" );

                /* delete array - send message */
                return( le_array_delete( & er_array ), EXIT_FAILURE );

            }

            /* write time on sokcet */
            if ( write( er_client, & er_time, sizeof( le_time_t ) ) != sizeof( le_time_t ) ) {

                /* display message */
                fprintf( stderr, "eratosthene-suite : error : unable to write on socket\n" );

                /* delete array - send message */
                return( le_array_delete( & er_array ), EXIT_FAILURE );

            }

            /* update array size */
            le_array_set_size( & er_array, er_count );

            /* write array on socket */
            if ( le_array_io_write( & er_array, er_client ) != LE_ERROR_SUCCESS ) {

                /* display message */
                fprintf( stderr, "eratosthene-suite : error : unable to write on socket\n" );

                /* delete array - send message */
                return( le_array_delete( & er_array ), EXIT_FAILURE );

            }

            /* close socket */
            le_client_delete( er_client );

        }

        /* delete array */
        le_array_delete( & er_array );

        /* delete stream */
        fclose( er_stream );

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

