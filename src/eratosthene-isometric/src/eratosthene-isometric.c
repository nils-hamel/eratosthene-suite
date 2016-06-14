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
    source - main function
 */

    int main( int argc, char ** argv ) {

        glutInit( & argc, argv );

        /* Query array variables */
        le_array_t er_array = LE_ARRAY_C;

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

                /* Query on server */
                if ( er_query( er_socket, ( le_char_t * ) lc_read_string( argc, argv, "--query", "-q" ), & er_array ) ) {

                    /* Display message */
                    fprintf( stderr, "eratosthene-suite : error : unable to perform query on server\n" );

                } else {

                    /* Render isometric tile */
                    er_render_isometry( 

                        ( le_char_t * ) lc_read_string( argc, argv, "--output", "-o" ), 
                        ( le_char_t * ) lc_read_string( argc, argv, "--query" , "-q" ), 
                        & er_array, 
                        lc_read_uint( argc, argv, "--width" , "-w", 1024 )

                    );

                }

            }            

            /* Close connection */
            le_client_delete( er_socket );

        }

        /* Return to system */
        return( EXIT_SUCCESS );

    }

