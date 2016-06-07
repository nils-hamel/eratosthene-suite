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

    # include "eratosthene-isometric.h"

/*
    source - server query
 */

    void er_query( le_sock_t const er_socket, le_char_t const * const er_query ) {

        /* Parsing variables */
        le_size_t er_parse = 0;

        /* Socket i/o count variables */
        le_size_t er_count = 0;

        /* Array pointer variables */
        le_real_t * er_ptrp = NULL;
        le_time_t * er_ptrt = NULL;
        le_data_t * er_ptrd = NULL;

        /* Socket i/o buffer variables */
        le_byte_t er_buffer[LE_NETWORK_BUFFER_SYNC] = LE_NETWORK_BUFFER_C;

        /* Check socket */
        if ( er_socket == _LE_SOCK_NULL ) {

            /* Display message */
            fprintf( stderr, "eratosthene-suite : error : invalid socket\n" );

            /* Abort query */
            return;

        }

        /* Query string to socket buffer */
        strcpy( ( char * ) er_buffer, ( char * ) er_query );

        /* Write query address */
        if ( write( er_socket, er_buffer, LE_NETWORK_BUFFER_ADDR ) != LE_NETWORK_BUFFER_ADDR ) {

            /* Display message */
            fprintf( stderr, "eratosthene-suite : error : unable to send query address\n" );

            /* Abort query */
            return;

        }

        /* Reading query elements */
        while( ( er_count = read( er_socket, er_buffer, LE_NETWORK_BUFFER_SYNC ) ) > 0 ) {

            /* Parsing received elements */
            for ( er_parse = 0; er_parse < er_count; er_parse += LE_ARRAY_LINE ) {

                /* Compute pointers */
                er_ptrp = ( le_real_t * ) ( er_buffer + er_parse );
                er_ptrt = ( le_time_t * ) ( er_ptrp + 3 );
                er_ptrd = ( le_data_t * ) ( er_ptrt + 1 );

                /* Display read element */
                fprintf( stderr, ER_FORMAT, er_ptrp[0], er_ptrp[1], er_ptrp[2], er_ptrt[0], er_ptrd[0], er_ptrd[1], er_ptrd[2] );

            }

        }

    }

/*
    source - main function
 */

    int main( int argc, char ** argv ) {

        /* Socket variables */
        le_sock_t er_socket = _LE_SOCK_NULL;

        /* Create socket */
        if ( ( er_socket = le_client_create( ( le_char_t * ) lc_read_string( argc, argv, "--ip", "-i" ), lc_read_uint( argc, argv, "--port", "-t", _LE_USE_PORT ) ) ) == _LE_SOCK_NULL ) {

            /* Display message */
            fprintf( stderr, "eratosthene-suite : error : unable to establish connection to server\n" );

        } else {

                /* Query handshake */
                if ( le_client_handshake_mode( er_socket, LE_NETWORK_MODE_QMOD ) != LE_ERROR_SUCCESS ) {

                    /* Display message */
                    fprintf( stderr, "eratosthene-suite : error : unable to obtain server authorisation\n" );

                } else {

                    /* Injection process */
                    er_query( er_socket, ( le_char_t * ) lc_read_string( argc, argv, "--query", "-q" ) );

                }            

            /* Close connection */
            le_client_delete( er_socket );

        }

        /* Return to system */
        return( EXIT_SUCCESS );

    }

