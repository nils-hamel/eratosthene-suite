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

    # include "eratosthene-isometry.h"

/*
    source - main function
 */

    int main( int argc, char ** argv ) {

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
            if ( le_client_handshake_mode( er_socket, LE_NETWORK_MODE_QMOD, LE_ARRAY_64S ) != LE_ERROR_SUCCESS ) {

                /* Display message */
                fprintf( stderr, "eratosthene-suite : error : unable to obtain server authorisation\n" );

            } else {

                /* Query on server */
                if ( le_client_system_query_array( er_socket, & er_array, ( le_char_t * ) lc_read_string( argc, argv, "--query", "-q" ) ) != LE_ERROR_SUCCESS ) {

                    /* Display message */
                    fprintf( stderr, "eratosthene-suite : error : unable to perform query on server\n" );

                } else {

                    /* Render structure variables */
                    er_render_t er_render = er_render_create(

                        /* String parameters */
                        ( le_char_t * ) lc_read_string( argc, argv, "--output", "-o" ),
                        ( le_char_t * ) lc_read_string( argc, argv, "--query" , "-q" ),
                        ( le_char_t * ) lc_read_string( argc, argv, "--view"  , "-v" ),

                        /* Numeric parameters */
                        lc_read_double( argc, argv, "--tilt" , "-t", 45.0 ),
                        lc_read_uint  ( argc, argv, "--thick", "-k",    2 ),
                        lc_read_uint  ( argc, argv, "--width", "-w", 1024 ),

                        /* Indexation server array */
                        & er_array

                    );

                    /* Prepare rendering */
                    if ( er_render_prepare( & er_render ) == _LE_FALSE ) {

                        /* Display message */
                        fprintf( stderr, "eratosthene-suite : error : unable to create graphical context\n" );

                    } else {

                        /* Rendering projection configuration */
                        er_render_projection( & er_render );

                        /* Rendering cell points */
                        er_render_cell( & er_render );

                        /* Rendering cell bounds */
                        er_render_bound( & er_render );

                        /* Rendering exportation */
                        if ( er_render_save( & er_render ) == _LE_FALSE ) {

                            /* Display message */
                            fprintf( stderr, "erathosthene-suite : error : unable to export graphical buffer\n" );

                        }

                        /* Terminate rendering */
                        er_render_terminate( & er_render );

                    }

                    /* Delete rendering structure */
                    er_render = er_render_delete( & er_render );

                }

                /* Delete array */
                le_array_delete( & er_array );

            }       

            /* Close connection */
            le_client_delete( er_socket );

        }

        /* Return to system */
        return( EXIT_SUCCESS );

    }

