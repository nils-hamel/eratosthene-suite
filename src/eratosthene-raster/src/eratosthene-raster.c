/*
 *  eratosthene-suite - raster
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

    # include "eratosthene-raster.h"

/*
    source - auxiliary methods
 */

    le_char_t * er_raster_path( le_char_t * const er_addr, le_char_t const * const er_path ) {

        /* path variable */
        static le_char_t er_export[_LE_USE_PATH] = { 0 };

        /* parsing pointer variable */
        le_char_t * er_parse = er_addr;

        /* parsing address string */
        while ( * ( ++ er_parse ) != '\0' ) {

            /* search and replace */
            if ( * er_parse == '/' ) * er_parse = '-';
            if ( * er_parse == ',' ) * er_parse = '-';

        }

        /* compose exportation path */
        sprintf( ( char * ) er_export, "%s/%s", er_path, er_addr + 1 );

        /* return exportation path */
        return( er_export );

    }

/*
    source - raster methods
 */

    le_enum_t er_raster( le_char_t const * const er_path, le_address_t const * const er_addr, le_array_t * const er_array ) {

        /* message variable */
        le_enum_t er_message = _LE_TRUE;

        /* output stream variable */
        le_file_t er_stream = NULL;

        /* array pointer variable */
        le_real_t * er_pose = NULL;

        /* array size variable */
        le_size_t er_size = le_array_get_size( er_array );

        /* array base variable */
        le_byte_t * er_byte = le_array_get_byte( er_array );

        /* cell factor variable */
        le_real_t er_fact_p = pow( 2.0, le_address_get_size( er_addr ) ) / LE_2P;

        /* cell factor variable */
        le_real_t er_fact_a = er_fact_p / LE_ADDRESS_WGS_A;

        /* cell edge variable */
        le_real_t er_edge[3] = { 0.0 };

        /* raster index variable */
        le_size_t er_index[3] = { 0 };

        /* raster memory variable */
        le_byte_t * er_raster = NULL;

        /* raster size variable */
        le_size_t er_width = 1 << le_address_get_span( er_addr );

        /* raster size variable */
        le_size_t er_volume = er_width * er_width * er_width;

        /* allocate raster memory */
        if ( ( er_raster = ( le_byte_t * ) calloc( er_volume, sizeof( le_byte_t ) ) ) == NULL ) {

            /* push message */
            er_message = _LE_FALSE;

        } else {

            /* compute raster edge */
            le_address_get_pose( er_addr, er_edge );

            /* parsing array elements */
            for ( le_size_t er_parse = 0; er_parse < er_size; er_parse += LE_ARRAY_UF3 ) {

                /* compute array pointer */
                er_pose = ( le_real_t * ) ( er_byte + er_parse );

                /* convert coordinates to raster index */
                er_index[0] = ( le_size_t ) round( ( ( er_pose[0] - er_edge[0] ) * er_fact_p ) * er_width );
                er_index[1] = ( le_size_t ) round( ( ( er_pose[1] - er_edge[1] ) * er_fact_p ) * er_width );
                er_index[2] = ( le_size_t ) round( ( ( er_pose[2] - er_edge[2] ) * er_fact_a ) * er_width );

                /* assign raster value */
                * ( er_raster + ( ( er_index[2] * er_width ) + er_index[1] ) * er_width + er_index[0] ) = 1;

            }

            /* create output stream */
            if ( ( er_stream = fopen( ( char * ) er_path, "wb" ) ) == NULL ) {

                /* push message */
                er_message = _LE_FALSE;

            } else {

                /* export raster */
                if ( fwrite( er_raster, sizeof( le_byte_t ), er_volume, er_stream ) != er_volume ) {

                    /* push message */
                    er_message = _LE_FALSE;

                }

                /* delete output stream */
                fclose( er_stream );

            }

            /* release raster memory */
            free( er_raster );

        }

        /* send message */
        return( er_message );

    }

/*
    source - enumeration methods
 */

    le_enum_t er_raster_enum( le_address_t * const er_addr, le_size_t const er_scale, le_size_t const er_target, le_size_t const er_limit, le_char_t const * const er_path, le_sock_t const er_socket ) {

        /* message variable */
        le_enum_t er_message = _LE_TRUE;

        /* size variable */
        le_size_t er_size = 0;

        /* span variable */
        le_size_t er_span = 0;

        /* digit value variable */
        le_size_t er_digit = 0;

        /* digit span variable */
        le_size_t er_base = le_address_base( er_scale );

        /* display variable */
        le_char_t er_string[_LE_USE_PATH] = { 0 };

        /* array variable */
        static le_array_t er_data = LE_ARRAY_C;
        static le_array_t er_dual = LE_ARRAY_C;

        /* check scale */
        if ( er_scale < er_target ) {

            /* update array size */
            le_array_set_size( & er_data, LE_ARRAY_ADDR );

            /* push address span */
            er_span = le_address_get_span( er_addr );

            /* set span for fast query */
            le_address_set_span( er_addr, 0 );

            /* serialise address */
            le_address_serial( er_addr, & er_data, 0, _LE_SET );

            /* pop address span */
            le_address_set_span( er_addr, er_span );

            /* write socket-array */
            le_array_io_put( & er_data, & er_dual, LE_MODE_QUER, er_socket );

            /* read socket-array */
            if ( le_array_io_get( & er_data, & er_dual, er_socket ) != LE_MODE_QUER ) {

                /* push message */
                er_message = _LE_FALSE;

            } else {

                /* check array size */
                if ( le_array_get_size( & er_data ) > 0 ) {

                    /* enumeration process */
                    while ( ( er_digit < er_base ) && ( er_message == _LE_TRUE ) ) {

                        /* update address digit */
                        le_address_set_digit( er_addr, er_scale, er_digit );

                        /* update address size */
                        le_address_set_size( er_addr, er_scale + 1 );

                        /* continue enumeration */
                        er_message = er_raster_enum( er_addr, er_scale + 1, er_target, er_limit, er_path, er_socket );

                        /* update digit value */
                        er_digit ++;

                    }

                }

            }

        } else {

            /* compose display string */
            le_address_ct_string( er_addr, er_string );

            /* update array size */
            le_array_set_size( & er_data, LE_ARRAY_ADDR );

            /* serialise address */
            le_address_serial( er_addr, & er_data, 0, _LE_SET );

            /* write socket-array */
            le_array_io_put( & er_data, & er_dual, LE_MODE_QUER, er_socket );

            /* read socket-array */
            if ( le_array_io_get( & er_data, & er_dual, er_socket ) != LE_MODE_QUER ) {

                /* push message */
                er_message = _LE_FALSE;

            } else {

                /* check limitation value */
                if ( ( er_size = ( le_array_get_size( & er_data ) / LE_ARRAY_UF3 ) ) < er_limit ) {

                    /* display message */
                    fprintf( stdout, "%s rejected with %" _LE_SIZE_P "\n", er_string, er_size );

                } else {

                    /* display message */
                    fprintf( stdout, "%s selected with %" _LE_SIZE_P "\n", er_string, er_size );

                    /* compute and export raster */
                    er_raster( er_raster_path( er_string, er_path ), er_addr, & er_data );

                }

            }

        }

        /* send message */
        return( er_message );

    }

/*
    source - main function
 */

    int main( int argc, char ** argv ) {

        /* messagae variable */
        le_enum_t er_message = EXIT_SUCCESS;

        /* socket variable */
        le_sock_t er_socket = _LE_SOCK_NULL;

        /* address size variable */
        le_size_t er_size = 0;

        /* enumeration depth variable */
        le_size_t er_depth = 0;

        /* limit value variable */
        le_size_t er_limit = 0;

        /* exportation path variable */
        le_char_t * er_path = ( le_char_t * ) lc_read_string( argc, argv, "--export", "-e" );;

        /* array variable */
        le_array_t er_auth = LE_ARRAY_C;

        /* query address variable */
        le_address_t er_addr = LE_ADDRESS_C;

        /* create socket */
        if ( ( er_socket = le_client_create( ( le_char_t * ) lc_read_string( argc, argv, "--ip", "-i" ), lc_read_signed( argc, argv, "--port", "-p", _LE_USE_PORT ) ) ) == _LE_SOCK_NULL ) {

            /* display message */
            fprintf( stderr, "eratosthene-suite : error : unable to establish connection to server\n" );

            /* push message */
            er_message = EXIT_FAILURE;

        } else {

            /* compose authorisation array */
            le_array_set_size( & er_auth, 0 );

            /* write authorisation array */
            le_array_io_put( & er_auth, NULL, LE_MODE_AUTH, er_socket );

            /* read authorisation response */
            if ( le_array_io_get( & er_auth, NULL, er_socket ) != LE_MODE_AUTH ) {

                /* display message */
                fprintf( stderr, "eratosthene-suite : error : bad server response\n" );

                /* push message */
                er_message = EXIT_FAILURE;

            } else {

                /* read enumeration depth */
                er_depth = lc_read_signed( argc, argv, "--depth", "-d", 0 );

                /* read raster limit value */
                er_limit = lc_read_signed( argc, argv, "--limit", "-l", 0 );

                /* read query address */
                le_address_cf_string( & er_addr, ( le_char_t * ) lc_read_string( argc, argv, "--query", "-q" ) );

                /* retrieve address size */
                er_size = le_address_get_size( & er_addr );

                /* address enumeration process */
                if ( er_raster_enum( & er_addr, er_size, er_size + er_depth, er_limit, er_path, er_socket ) != _LE_TRUE ) {

                    /* display message */
                    fprintf( stderr, "eratostehen-suite : error : raster query/exportation\n" );

                    /* push message */
                    er_message = EXIT_FAILURE;

                }

            }

            /* delete socket */
            er_socket = le_client_delete( er_socket );

        }

        /* send message */
        return( er_message );

    }

