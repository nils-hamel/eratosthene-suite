/*
 *  eratosthene-suite - raster
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

    # include "eratosthene-raster.h"

/*
    source - raster method
 */

    le_enum_t er_raster( le_char_t const * const er_path, le_address_t const * const er_addr, le_array_t * const er_encode ) {

        /* output stream variables */
        le_file_t er_stream = NULL;

        /* array pointer variables */
        le_real_t * er_pose = NULL;

        /* array size variables */
        le_size_t er_size = le_array_get_size( er_encode );

        /* array base variables */
        le_byte_t * er_byte = le_array_get_byte( er_encode );

        /* cell factor variables */
        le_real_t er_fact_p = pow( 2.0, le_address_get_size( er_addr ) ) / LE_2P;

        /* cell factor variables */
        le_real_t er_fact_a = er_fact_p / LE_ADDRESS_WGS_A;

        /* cell edge variables */
        le_real_t er_edge[3] = { 0.0 };

        /* raster index variables */
        le_size_t er_index[3] = { 0 };

        /* raster memory variables */
        le_byte_t * er_raster = NULL;

        /* raster size variables */
        le_size_t er_segment = 1 << le_address_get_span( er_addr );

        /* raster size variables */
        le_size_t er_volume = er_segment * er_segment * er_segment;

        /* allocate raster memory */
        if ( ( er_raster = ( le_byte_t * ) malloc( er_volume ) ) == NULL ) {

            /* send message */
            return( _LE_FALSE );

        }

        /* compute cell edge */
        le_address_get_pose( er_addr, er_edge );

        /* parsing array elements */
        for ( le_size_t er_parse = 0; er_parse < er_size; er_parse += LE_ARRAY_UF3 ) {

            /* compute array pointer */
            er_pose = ( le_real_t * ) ( er_byte + er_parse );

            /* convert coordinates to raster index */
            er_index[0] = ( le_size_t ) ( ( ( er_pose[0] - er_edge[0] ) * er_fact_p ) * er_segment );
            er_index[1] = ( le_size_t ) ( ( ( er_pose[1] - er_edge[1] ) * er_fact_p ) * er_segment );
            er_index[2] = ( le_size_t ) ( ( ( er_pose[2] - er_edge[2] ) * er_fact_a ) * er_segment );

            /* assign raster value */
            * ( er_raster + ( ( er_index[2] * er_segment ) + er_index[1] ) * er_segment + er_index[0] ) = 1;

        }

        /* create output stream */
        if ( ( er_stream = fopen( ( char * ) er_path, "wb" ) ) != NULL ) {

            /* release raster memory */
            free( er_raster );

            /* send message */
            return( _LE_FALSE );

        }

        /* export raster content */
        if ( fwrite( er_raster, sizeof( le_byte_t ), er_volume, er_stream ) != er_volume ) {

            /* delete output stream */
            fclose( er_stream );

            /* release raster memory */
            free( er_raster );

            /* send message */
            return( _LE_FALSE );

        }

        /* delete output stream */
        fclose( er_stream );

        /* release raster memory */
        free( er_raster );

        /* send message */
        return( _LE_TRUE );

    }

/*
    source - main function
 */

    int main( int argc, char ** argv ) {

        /* address variables */
        le_address_t er_address = LE_ADDRESS_C;

        /* query array variables */
        le_array_t er_encode = LE_ARRAY_C;
        le_array_t er_decode = LE_ARRAY_C;

        /* socket variables */
        le_sock_t er_socket = _LE_SOCK_NULL;

        /* create socket */
        if ( ( er_socket = le_client_create( ( le_char_t * ) lc_read_string( argc, argv, "--ip", "-i" ), lc_read_signed( argc, argv, "--port", "-t", _LE_USE_PORT ) ) ) == _LE_SOCK_NULL ) {

            /* display message */
            fprintf( stderr, "eratosthene-suite : error : unable to establish connection to server\n" );

            /* send message */
            return( EXIT_FAILURE );

        }

        /* socket array size */
        le_array_set_size( & er_encode, 0 );

        /* write socket array */
        le_array_io_write( & er_encode, LE_MODE_AUTH, er_socket );

        /* read socket array */
        if ( le_array_io_read( & er_encode, er_socket ) != LE_MODE_AUTH ) {

            /* display message */
            fprintf( stderr, "eratosthene-suite : warning : bad server response\n" );

        }

        /* check array consistency */
        if ( le_array_get_size( & er_encode ) != LE_ARRAY_AUTH ) {

            /* display message */
            fprintf( stderr, "eratosthene-suite : warining : bad server response\n" );

        }

        /* socket array size */
        le_array_set_size( & er_encode, LE_ARRAY_ADDR );

        /* convert string to address */
        le_address_cf_string( & er_address, ( le_char_t * ) lc_read_string( argc, argv, "--query", "-q" ) );

        /* serialise address */
        le_address_serial( & er_address, & er_encode, 0, _LE_SET );

        /* write socket-array */
        le_array_io_write( & er_encode, LE_MODE_QUER, er_socket );

        /* read socket-array */
        le_array_io_read( & er_encode, er_socket );

        /* decode array */
        le_array_uf3_decode( & er_encode, & er_decode );

        /* delete socket */
        le_client_delete( er_socket );

        /* compute and export raster */
        if ( er_raster( ( le_char_t * ) lc_read_string( argc, argv, "--raster", "-r" ), & er_address, & er_decode ) == _LE_FALSE ) {

            /* display message */
            fprintf( stderr, "eratosthene-suite : error : unable to create raster\n" );

            /* send message */
            return( EXIT_FAILURE );

        }

        /* send message */
        return( EXIT_SUCCESS );

    }

