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
    source - isometry function
 */

    le_enum_t er_isometry( int argc, char ** argv, le_array_t * const er_array ) {

        /* Message variables */
        le_enum_t er_message = _LE_TRUE;

        /* Render structure variables */
        er_render_t er_render = er_render_create(

            /* String parameters */
            ( le_char_t * ) lc_read_string( argc, argv, "--query" , "-q" ),
            ( le_char_t * ) lc_read_string( argc, argv, "--view"  , "-v" ),

            /* Numeric parameters */
            lc_read_double( argc, argv, "--tilt" , "-t", 45.0 ),
            lc_read_uint  ( argc, argv, "--thick", "-k",    2 ),
            lc_read_uint  ( argc, argv, "--width", "-w", 1024 )

        );

        /* Perepare isometry rendering */
        if ( ( er_message = er_render_prepare( & er_render ) ) == _LE_TRUE ) {

            /* Rendering projection configuration */
            er_render_projection( & er_render );

            /* Rendering cell points */
            er_render_primivites( & er_render, er_array );

            /* Rendering exportation */
            er_message = er_render_save( & er_render, lc_read_string( argc, argv, "--output", "-o" ) );

            /* Terminate rendering */
            er_render_terminate( & er_render );

        }

        /* Delete rendering structure */
        er_render = er_render_delete( & er_render );

        /* Send message */
        return( er_message );

    }

/*
    source - main function
 */

    int main( int argc, char ** argv ) {

        /* Query array variables */
        le_array_t er_array = LE_ARRAY_C;

        /* Address variables */
        le_address_t er_address = LE_ADDRESS_C;

        /* Socket variables */
        le_sock_t er_socket = _LE_SOCK_NULL;

        /* Create socket */
        if ( ( er_socket = le_client_create( ( le_char_t * ) lc_read_string( argc, argv, "--ip", "-i" ), lc_read_uint( argc, argv, "--port", "-t", _LE_USE_PORT ) ) ) == _LE_SOCK_NULL ) {

            /* Display message */
            fprintf( stderr, "eratosthene-suite : error : unable to establish connection to server\n" );

        } else {

            /* Query handshake */
            if ( le_client_handshake( er_socket, LE_NETWORK_MODE_QMOD, LE_ARRAY_RFD ) != LE_ERROR_SUCCESS ) {

                /* Display message */
                fprintf( stderr, "eratosthene-suite : error : unable to obtain server authorisation\n" );

            } else {

                /* Convert string query to address */
                le_address_cf_string( & er_address, ( le_char_t * ) lc_read_string( argc, argv, "--query", "-q" ) );

                /* Write query address on socket */
                if ( le_address_io_write( & er_address, er_socket ) != LE_ERROR_SUCCESS ) {

                    /* Display message */
                    fprintf( stderr, "eratosthene-suite : error : unable to send query to server\n" );

                } else {

                    /* Query on server */
                    if ( le_array_io_read( & er_array, er_socket ) != LE_ERROR_SUCCESS ) {

                        /* Display message */
                        fprintf( stderr, "eratosthene-suite : error : unable to read array from server\n" );

                    } else {

                        /* Render function */
                        if ( er_isometry( argc, argv, & er_array ) != _LE_TRUE ) {

                            /* Display message */
                            fprintf( stderr, "eratosthene-suite : error : unable to render isometry\n" );

                        }

                    }

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

