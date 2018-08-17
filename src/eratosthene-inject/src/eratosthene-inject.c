/*
 *  eratosthene-suite - inject
 *
 *      Nils Hamel - nils.hamel@bluewin.ch
 *      Copyright (c) 2016-2018 DHLAB, EPFL
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
    source - injection method - uv3
 */

    le_enum_t er_inject_uv3_beta( le_char_t const * const er_path, le_time_t er_time, le_sock_t const er_socket ) {

        /* stream variable */
        le_file_t er_stream = NULL;

        /* reading variable */
        le_size_t er_read = 0;

        /* array variable */
        le_array_t er_array = LE_ARRAY_C;

        /* message variable */
        le_enum_t er_message = EXIT_SUCCESS;

        /* create and check stream */
        if ( ( er_stream = fopen( ( char * ) er_path, "rb" ) ) == NULL ) {

            /* display message */
            lc_error( "stream access" );

            /* push message */
            er_message = EXIT_FAILURE;

        } else {

            /* update socket-array size */
            le_array_set_size( & er_array, LE_ARRAY_INJE );

            /* serialise time */
            le_array_serial( & er_array, & er_time, sizeof( le_time_t ), 0, _LE_SET );

            /* write socket-array */
            if ( le_array_io_write( & er_array, LE_MODE_INJE, er_socket ) != LE_MODE_INJE ) {

                /* display message */
                lc_error( "request dispatch" );

                /* push message */
                er_message = EXIT_FAILURE;

            } else {

                /* update socket-array size */
                le_array_set_size( & er_array, ER_INJECT );

                /* stream chunk dispatch */
                while ( ( ( er_read = fread( le_array_get_byte( & er_array ), sizeof( le_byte_t ), ER_INJECT, er_stream ) ) > 0 ) && ( er_message == EXIT_SUCCESS ) ) {

                    /* check read size */
                    if ( er_read < ER_INJECT ) {

                        /* update socket-array size */
                        le_array_set_size( & er_array, er_read );

                    }

                    /* write socket array */
                    if ( le_array_io_write( & er_array, LE_MODE_INJE, er_socket ) != LE_MODE_INJE ) {

                        /* display message */
                        lc_error( "data dispatch" );

                        /* push message */
                        er_message = EXIT_FAILURE;

                    }

                }

            }

        }

        /* send message */
        return( er_message );

    }

/*
    source - injection method - optimisation
 */

    le_enum_t er_inject_optimise( le_time_t er_time, le_sock_t const er_socket ) {

        /* socket array variable */
        le_array_t er_array = LE_ARRAY_C;

        /* message variable */
        le_enum_t er_message = EXIT_SUCCESS;

        /* update socket-array size */
        le_array_set_size( & er_array, LE_ARRAY_OPTM );

        /* serialise time */
        le_array_serial( & er_array, & er_time, sizeof( le_time_t ), 0, _LE_SET );

        /* write socket-array */
        if ( le_array_io_write( & er_array, LE_MODE_OPTM, er_socket ) != LE_MODE_OPTM ) {

            /* display message */
            lc_error( "request dispatch" );

            /* push message */
            er_message = EXIT_FAILURE;

        } else {

            /* read socket-array */
            if ( le_array_io_read( & er_array, er_socket ) != LE_MODE_OPTM ) {

                /* display message */
                lc_error( "service confirmation" );

                /* push message */
                er_message = EXIT_FAILURE;

            }

        }

        /* delete socket-array */
        le_array_delete( & er_array );

        /* send message */
        return( er_message );

    }

/*
    source - main method
 */

    int main( int argc, char ** argv ) {

        /* socket variable */
        le_sock_t er_socket = _LE_SOCK_NULL;

        /* data path variable */
        le_char_t * er_path = NULL;

        /* server address variable */
        le_char_t * er_host = ( le_char_t * ) lc_read_string( argc, argv, "--ip", "-i" );

        /* server service variable */
        le_enum_t er_port = lc_read_unsigned( argc, argv, "--port", "-p", _LE_USE_PORT );

        /* time variable */
        le_time_t er_time = lc_read_signed( argc, argv, "--time", "-t", _LE_TIME_NULL );

        /* message variable */
        le_enum_t er_message = EXIT_SUCCESS;

        /* create socket */
        if ( ( er_socket = le_client_create( er_host, er_port ) ) == _LE_SOCK_NULL ) {

            /* display message */
            lc_error( "service unavailable" );

            /* push message */
            er_message = EXIT_FAILURE;

        } else {

            /* flag detection */
            if ( lc_read_flag( argc, argv, "--inject", "-j" ) == LC_TRUE ) {

                /* switch on format */
                if ( ( er_path = ( le_char_t * ) lc_read_string( argc, argv, "--uv3", "" ) ) != NULL ) {

                    /* injection process - uf3 */
                    //if ( ( er_message = er_inject_uf3( er_path, er_time, er_socket ) ) == EXIT_SUCCESS ) {
                    if ( ( er_message = er_inject_uv3_beta( er_path, er_time, er_socket ) ) == EXIT_SUCCESS ) {

                        /* flag detection */
                        if ( lc_read_flag( argc, argv, "--optimise", "-o" ) == LC_TRUE ) {

                            /* optimisation process */
                            er_message = er_inject_optimise( er_time, er_socket );

                        }

                    }

                } else {

                    /* display messsage */
                    lc_error( "un-supported format" );

                    /* push message */
                    er_message = EXIT_FAILURE;

                }

            } else {

                /* flag detection */
                if ( lc_read_flag( argc, argv, "--optimise", "-o" ) == LC_TRUE ) {

                    /* optimisation process */
                    er_message = er_inject_optimise( er_time, er_socket );

                }

            }

            /* delete socket */
            le_client_delete( er_socket );

        }

        /* send message */
        return( er_message );

    }

