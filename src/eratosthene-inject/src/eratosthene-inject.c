/*
 *  eratosthene-suite - geodetic system
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

    void er_injection( le_sock_t const er_client, FILE * const er_stream ) {

        /* Write status variables */
        le_enum_t er_flag = _LE_TRUE;

        /* Array pointer variables */
        le_real_t * er_ptrp = NULL;
        le_time_t * er_ptrt = NULL;
        le_data_t * er_ptrd = NULL;

        /* Socket i/o count variables */
        le_size_t er_parse = 0;
        le_size_t er_count = 0;

        /* Socket i/o buffer */
        le_byte_t er_buffer[LE_NETWORK_BUFFER_SYNC] = LE_NETWORK_BUFFER_C;

        /* Stream data injection */
        while ( er_flag == _LE_TRUE ) {

            /* Compute array pointers */
            er_ptrp = ( le_real_t * ) ( er_buffer + er_parse );
            er_ptrt = ( le_time_t * ) ( er_ptrp + 3 );
            er_ptrd = ( le_data_t * ) ( er_ptrt + 1 );

            /* Read stream element and update i/o parser */
            if ( fscanf( er_stream, ER_INJECT_FORMAT, er_ptrp, er_ptrp + 1, er_ptrp + 2, er_ptrt, er_ptrd, er_ptrd + 1, er_ptrd + 2 ) == 7 ) {

                er_parse += LE_ARRAY_LINE;

            }

            /* Check i/o buffer and stream state */
            if ( ( er_parse == LE_NETWORK_BUFFER_SYNC ) || ( ( er_flag = feof( er_stream ) ? _LE_FALSE : _LE_TRUE ) == _LE_FALSE ) ) {

                /* Write buffer to socket */
                if ( write( er_client, er_buffer, er_parse ) != er_parse ) {

                    /* Display message */
                    fprintf( stderr, "eratosthene-inject : error : partial buffer writing on socket\n" );

                    /* Abort injection */
                    return;

                }

                /* Update i/o counter */
                er_count += er_parse / LE_ARRAY_LINE;

                /* Reset i/o parser */
                er_parse = 0;

            }

        }

        /* Display message */
        fprintf( stderr, "eratosthene-inject : injected %" _LE_SIZE_P " element(s)\n", er_count );

    }

/*
    source - main function
 */

    int main( int argc, char ** argv ) {

        /* Server port variables */
        unsigned int er_port = lc_read_uint( argc, argv, "--port", "-t", _LE_USE_PORT );

        /* Stream handle variables */
        FILE * er_stream = NULL;

        /* Socket handle variables */
        le_sock_t er_client = _LE_SOCK_NULL;

        /* Create input stream */
        if ( ( er_stream = fopen( lc_read_string( argc, argv, "--file", "-f" ), "r" ) ) == NULL ) {

            /* Display message */
            fprintf( stderr, "eratosthene-inject : error : unable to access file\n" );

        } else {

            /* Create client handle */
            if ( ( er_client = le_client_create( ( le_char_t * ) lc_read_string( argc, argv, "--ip", "-i" ), er_port ) ) == _LE_SOCK_NULL ) {

                /* Display message */
                fprintf( stderr, "eratosthene-inject : error : unable to connect to server\n" );

            } else {

                /* Client-server injection handshake */
                if ( le_client_handshake_mode( er_client, LE_NETWORK_MODE_IMOD ) != LE_ERROR_SUCCESS ) {

                    /* Display message */
                    fprintf( stderr, "eratosthene-inject : error : authorisation failed\n" );

                } else {

                    /* Injection procedure */
                    er_injection( er_client, er_stream );

                }

                /* Delete client handle */
                er_client = le_client_delete( er_client );

            }

            /* Delete input stream */
            fclose( er_stream );

        }

        /* Return to system */
        return( EXIT_SUCCESS );

    }

