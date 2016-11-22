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

    # include "eratosthene-inject.h"

/*
    source - injection procedure
 */

    le_enum_t er_injection( le_sock_t const er_client, FILE * const er_stream, le_time_t const er_time ) {

        /* socket i/o count variables */
        le_size_t er_fread = 1;
        le_size_t er_write = 1;

        /* socket i/o buffer variables */
        le_byte_t er_buffer[LE_BUFFER_SYNC] = LE_BUFFER_C;

        /* sending time on socket */
        if ( write( er_client, & er_time, sizeof( le_time_t ) ) != sizeof( le_time_t ) ) {

            /* send message */
            return( LE_ERROR_IO_WRITE );

        }

        /* stream data injection */
        while ( ( er_fread > 0 ) && ( er_fread == er_write ) ) {

            /* read stream elements */
            if ( ( er_fread = fread( ( le_void_t * ) er_buffer, sizeof( le_byte_t ), _LE_USE_MTU, er_stream ) ) > 0 ) {

                /* write buffer to socket */
                if ( ( er_write = write( er_client, er_buffer, er_fread ) ) != er_fread ) {

                    /* send message */
                    return( LE_ERROR_IO_WRITE );

                }

            }

        }

        /* send message */
        return( LE_ERROR_SUCCESS );

    }

/*
    source - main function
 */

    int main( int argc, char ** argv ) {

        /* stream handle variables */
        FILE * er_stream = NULL;

        /* socket handle variables */
        le_sock_t er_client = _LE_SOCK_NULL;

        /* time variables */
        le_time_t er_time = lc_read_signed( argc, argv, "--time", "-t", time( NULL ) );

        /* create input stream */
        if ( ( er_stream = fopen( lc_read_string( argc, argv, "--file", "-f" ), "rb" ) ) == NULL ) {

            /* display message */
            fprintf( stderr, "eratosthene-suite : error : unable to access file\n" );

        } else {

            /* create client handle */
            if ( ( er_client = le_client_create( ( le_char_t * ) lc_read_string( argc, argv, "--ip", "-i" ), lc_read_signed( argc, argv, "--port", "-t", _LE_USE_PORT ) ) ) == _LE_SOCK_NULL ) {

                /* display message */
                fprintf( stderr, "eratosthene-suite : error : unable to connect to server\n" );

            } else {

                /* client-server injection handshake */
                if ( le_client_handshake( er_client, LE_MODE_IMOD ) != LE_ERROR_SUCCESS ) {

                    /* display message */
                    fprintf( stderr, "eratosthene-suite : error : authorisation failed\n" );

                } else {

                    /* injection procedure */
                    if ( er_injection( er_client, er_stream, er_time ) != LE_ERROR_SUCCESS ) {

                        /* display message */
                        fprintf( stderr, "eratosthene-suite : error : write on socket failure\n" );

                    }

                }

                /* delete client handle */
                er_client = le_client_delete( er_client );

            }

            /* delete input stream */
            fclose( er_stream );

        }

        /* return to system */
        return( EXIT_SUCCESS );

    }

