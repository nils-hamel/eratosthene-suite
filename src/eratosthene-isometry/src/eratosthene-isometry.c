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

    # include "eratosthene-isometry.h"

/*
    source - isometry function
 */

    le_enum_t er_isometry( int argc, char ** argv, le_array_t * const er_array ) {

        /* message variables */
        le_enum_t er_message = _LE_TRUE;

        /* render structure variables */
        er_render_t er_render = er_render_create(

            /* string parameters */
            ( le_char_t * ) lc_read_string( argc, argv, "--query" , "-q" ),
            ( le_char_t * ) lc_read_string( argc, argv, "--view"  , "-v" ),

            /* numeric parameters */
            lc_read_double  ( argc, argv, "--tilt" , "-l", 45.0 ),
            lc_read_unsigned( argc, argv, "--thick", "-k",    2 ),
            lc_read_unsigned( argc, argv, "--width", "-w", 1024 )

        );

        /* perepare isometry rendering */
        if ( ( er_message = er_render_prepare( & er_render ) ) == _LE_TRUE ) {

            /* rendering projection configuration */
            er_render_projection( & er_render );

            /* rendering cell points */
            er_render_primivites( & er_render, er_array );

            /* rendering exportation */
            er_message = er_render_save( & er_render, lc_read_string( argc, argv, "--output", "-o" ) );

            /* terminate rendering */
            er_render_terminate( & er_render );

        }

        /* delete rendering structure */
        er_render_delete( & er_render );

        /* send message */
        return( er_message );

    }

/*
    source - main function
 */

    int main( int argc, char ** argv ) {

        /* address variables */
        le_address_t er_address = LE_ADDRESS_C;

        /* query array variables */
        le_array_t er_array = LE_ARRAY_C;

        /* socket variables */
        le_sock_t er_socket = _LE_SOCK_NULL;

        /* create socket */
        if ( ( er_socket = le_client_create( ( le_char_t * ) lc_read_string( argc, argv, "--ip", "-i" ), lc_read_signed( argc, argv, "--port", "-t", _LE_USE_PORT ) ) ) == _LE_SOCK_NULL ) {

            /* display message */
            fprintf( stderr, "eratosthene-suite : error : unable to establish connection to server\n" );

        } else {

            /* query handshake */
            if ( le_client_handshake( er_socket, LE_MODE_QMOD ) != LE_ERROR_SUCCESS ) {

                /* display message */
                fprintf( stderr, "eratosthene-suite : error : unable to obtain server authorisation\n" );

            } else {

                /* convert string query to address */
                le_address_cf_string( & er_address, ( le_char_t * ) lc_read_string( argc, argv, "--query", "-q" ) );

                /* write query address on socket */
                if ( le_address_io_write( & er_address, er_socket ) != LE_ERROR_SUCCESS ) {

                    /* display message */
                    fprintf( stderr, "eratosthene-suite : error : unable to send query to server\n" );

                } else {

                    /* read reduced address */
                    le_address_io_read( & er_address, er_socket );

                    /* query on server */
                    if ( le_array_io_read( & er_array, er_socket ) != LE_ERROR_SUCCESS ) {

                        /* display message */
                        fprintf( stderr, "eratosthene-suite : error : unable to read array from server\n" );

                    } else {

                        /* render function */
                        if ( er_isometry( argc, argv, & er_array ) != _LE_TRUE ) {

                            /* display message */
                            fprintf( stderr, "eratosthene-suite : error : unable to render isometry\n" );

                        }

                    }

                }

                /* delete array */
                le_array_delete( & er_array );

            }

            /* close connection */
            le_client_delete( er_socket );

        }

        /* return to system */
        return( EXIT_SUCCESS );

    }

