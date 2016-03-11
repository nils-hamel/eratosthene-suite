/*
 *  liberatosthene - geodetic system
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

    # include "eratosthene-server.h"

/*
    source - arguments and parameters parser
 */

    char * er_read_string( int const argc, char ** argv, char const * const er_long, char const * const er_short ) {

        /* Parsing variables */
        int er_parse = 0;

        /* Parses arguments and parameters */
        for ( ; er_parse < argc; er_parse ++ ) {

            /* Check argument */
            if ( ( strcmp( argv[er_parse], er_long ) == 0 ) || ( strcmp( argv[er_parse], er_short ) == 0 ) ) {

                /* Check consistency */
                if ( ( ++ er_parse ) < argc ) {

                    /* Return pointer */
                    return( argv[er_parse] );

                } else {

                    /* Return pointer */
                    return( NULL );

                }

            }

        }

        /* Return pointer */
        return( NULL );

    }

    unsigned int er_read_uint( int const argc, char ** argv, char const * const er_long, char const * const er_short, unsigned int er_default ) {

        /* Parsing variables */
        int er_parse = 0;

        /* Parses arguments and parameters */
        for( ; er_parse < argc; er_parse ++ ) {

            /* Check argument */
            if ( ( strcmp( argv[er_parse], er_long ) == 0 ) || ( strcmp( argv[er_parse], er_short ) == 0 ) ) {

                /* Check consistency */
                if ( ( ++ er_parse ) < argc ) {

                    /* Return read value */
                    return( strtoul( argv[er_parse], NULL, 10 ) );

                } else {

                    /* Return default value */
                    return( er_default );

                }

            }

        }

        /* Return default value */
        return( er_default );

    }

/*
    source - main function
 */

    int main( int argc, char ** argv ) {

        /* Server port variables */
        unsigned int er_port = er_read_uint( argc, argv, "--port", "-t", _LE_USE_PORT );

        /* Socket handle variables */
        le_sock_t er_server = _LE_SOCK_NULL;

        /* System handle variables */
        le_system_t er_system = LE_SYSTEM_C;

        /* Create system handle */
        if ( le_system_create( & er_system, ( le_char_t * ) er_read_string( argc, argv, "--path", "-p" ) ) != LE_ERROR_SUCCESS ) {

            /* Display message */
            fprintf( stderr, "eratosthene-server : error : unable to create system handle\n" );

        } else {

            /* Create server handle */
            if ( ( er_server = le_server_create( er_port ) ) == _LE_SOCK_NULL ) {

                /* Display message */
                fprintf( stderr, "eratosthene-server : error : unable to create server handle\n" );

            } else {

                /* Display message */
                fprintf( stderr, "eratosthene-server : server listening on TCP/IP port %u\n", er_port );

                /* Server handle and idle */
                le_server( er_server, & er_system );

                /* Delete server handle */
                le_server_delete( er_server );

            }

            /* Delete system handle */
            le_system_delete( & er_system );

        }

        /* Return to system */
        return( EXIT_SUCCESS );

    }

