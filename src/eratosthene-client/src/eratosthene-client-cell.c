/*
 *  eratosthene-suite - client
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

    # include "eratosthene-client-cell.h"

/*
    source - constructor/destructor methods
 */

    er_cell_t er_cell_create( le_void_t ) {

        /* created structure variables */
        er_cell_t er_cell = ER_CELL_C;

        /* create array structure */
        er_cell.ce_array = le_array_create();

        /* return created structure */
        return( er_cell );

    }

    le_void_t er_cell_delete( er_cell_t * const er_cell ) {

        /* deleted structure variables */
        er_cell_t er_reset = ER_CELL_C;

        /* delete array structure */
        le_array_delete( & er_cell->ce_array );

        /* delete structure */
        ( * er_cell ) = er_reset;

    }

/*
    source - accessor methods
 */

    le_enum_t er_cell_get_flag( er_cell_t const * const er_cell ) {

        /* return cell flag */
        return( er_cell->ce_flag );

    }

    le_enum_t er_cell_get_draw( er_cell_t const * const er_cell ) {

        /* return cell flag */
        return( er_cell->ce_draw );

    }

    le_enum_t er_cell_get_push( er_cell_t const * const er_cell ) {

        /* return pushed address state */
        return( le_address_get_size( & er_cell->ce_push ) != 0 ? _LE_TRUE : _LE_FALSE );

    }

    le_enum_t er_cell_get_match( er_cell_t const * const er_addr, er_cell_t const * const er_push ) {

        /* return comparison result */
        return( le_address_get_equal( & er_addr->ce_addr, & er_push->ce_push ) );

    }

    le_size_t er_cell_get_size( er_cell_t const * const er_cell ) {

        /* return cell size */
        return( er_cell->ce_size );

    }

    le_real_t * er_cell_get_pose( er_cell_t const * const er_cell ) {

        /* return cell geodetic array pointer */
        //return( ( le_real_t * ) le_array_get_byte( & er_cell->ce_array ) );
        return( ( le_real_t * ) ( le_array_get_byte( & er_cell->ce_array ) + LE_ARRAY_ADDR ) );

    }

    le_data_t * er_cell_get_data( er_cell_t const * const er_cell ) {

        /* return cell colorimetric array pointer */
        //return( ( le_data_t * ) ( le_array_get_byte( & er_cell->ce_array ) + LE_ARRAY_SD_1 ) );
        return( ( le_data_t * ) ( le_array_get_byte( & er_cell->ce_array )  + LE_ARRAY_ADDR + LE_ARRAY_SD_1 ) );

    }

    le_real_t * er_cell_get_edge( er_cell_t const * const er_cell ) {

        /* return cell edge array pointer */
        return( ( le_real_t * ) er_cell->ce_edge );

    }

/*
    source - mutator methods
 */

    le_void_t er_cell_set_flag( er_cell_t * const er_cell, le_enum_t const er_flag ) {

        /* assign cell flag */
        er_cell->ce_flag = er_flag;

    }

    le_void_t er_cell_set_draw( er_cell_t * const er_cell, le_enum_t const er_draw ) {

        /* assign cell flag */
        er_cell->ce_draw = er_draw;

    }

    le_void_t er_cell_set_addr( er_cell_t * const er_cell, le_address_t const * const er_address ) {

        /* assign cell address */
        er_cell->ce_addr = ( * er_address );

    }

    le_void_t er_cell_set_push( er_cell_t * const er_cell, er_cell_t const * const er_push ) {

        /* assign cell address */
        er_cell->ce_push = er_push->ce_addr;

    }

    le_void_t er_cell_set_pop( er_cell_t * const er_cell ) {

        /* clear pushed address */
        le_address_set_size( & er_cell->ce_push, 0 );

    }

    le_void_t er_cell_set_swap( er_cell_t * const er_addr, er_cell_t * const er_push ) {

        /* swap address and pushed address */
        er_addr->ce_addr = er_push->ce_push;

    }

/*
    source - i/o methods
 */

    /* resize array at structure creation - will only grow up */

    le_size_t er_cell_io_reduce( er_cell_t * const er_cell, le_sock_t const er_socket ) {

        /* array size management */
        le_array_set_size( & er_cell->ce_array, LE_ARRAY_ADDR );

        /* serialise address */
        le_address_serial( & er_cell->ce_addr, & er_cell->ce_array, 0, _LE_SET );

        /* write socket-array */
        le_array_io_write( & er_cell->ce_array, LE_MODE_REDU, er_socket );

        /* read socket-array */
        le_array_io_read( & er_cell->ce_array, er_socket );

        /* serialise address */
        le_address_serial( & er_cell->ce_addr, & er_cell->ce_array, 0, _LE_GET );

        /* check reduction results */
        if ( le_address_get_time( & er_cell->ce_addr, 0 ) != _LE_TIME_NULL ) return( 1 );

        /* check reduction results */
        if ( le_address_get_time( & er_cell->ce_addr, 1 ) != _LE_TIME_NULL ) return( 1 );

        /* send results */
        return( 0 );

    }

    /* resize array at structure creation - will only grow up */

    le_size_t er_cell_io_query( er_cell_t * const er_cell, le_sock_t const er_socket ) {

        /* socket-array variables */
        le_byte_t * er_head = NULL;
        le_real_t * er_pose = NULL;

        /* parsing variables */
        le_size_t er_size = 0;

        /* optimisation variables */
        le_real_t er_comp[3] = { 0.0 };

        /* socket-array size management */
        le_array_set_size( & er_cell->ce_array, LE_ARRAY_ADDR );

        /* serialise address */
        le_address_serial( & er_cell->ce_addr, & er_cell->ce_array, 0, _LE_SET );

        /* writre socket-array */
        le_array_io_write( & er_cell->ce_array, LE_MODE_QUER, er_socket );

        /* read socket-array */
        le_array_io_read( & er_cell->ce_array, er_socket );

        /* serialise address */
        le_address_serial( & er_cell->ce_addr, & er_cell->ce_array, 0, _LE_GET );

        /* compute cell edge */
        le_address_get_pose( & er_cell->ce_addr, er_cell->ce_edge );

        /* convert edge coordinates - geographic to cartesian */
        er_cell->ce_edge[2] = LE_ADDRESS_WGSA * cos( er_cell->ce_edge[1] ) * cos( er_cell->ce_edge[0] );
        er_cell->ce_edge[0] = LE_ADDRESS_WGSA * cos( er_cell->ce_edge[1] ) * sin( er_cell->ce_edge[0] );
        er_cell->ce_edge[1] = LE_ADDRESS_WGSA * sin( er_cell->ce_edge[1] );

        /* empty cell size */
        er_cell->ce_size = 0;

        /* socket-array data segment */
        er_size = le_array_get_size( & er_cell->ce_array ) - LE_ARRAY_ADDR;
        er_head = le_array_get_byte( & er_cell->ce_array ) + LE_ARRAY_ADDR;

        /* parsing socket-array */
        for ( le_size_t er_parse = 0; er_parse < er_size; er_parse += LE_ARRAY_SD ) {

            /* compute data pointer */
            er_pose = ( le_real_t * ) ( er_head + er_parse );

            /* convert coordinates - geographic to cartesian */
            er_comp[0] = - er_cell->ce_edge[1] + sin( er_pose[1] ) * ( er_pose[2] += LE_ADDRESS_WGSA );
            er_comp[1] = - er_cell->ce_edge[0] + er_pose[2] * sin( er_pose[0] ) * ( er_pose[1] = cos( er_pose[1] ) );
            er_comp[2] = - er_cell->ce_edge[2] + er_pose[2] * er_pose[1] * cos( er_pose[0] );

            /* assign converted coordinates */
            er_pose[1] = er_comp[0];
            er_pose[0] = er_comp[1];
            er_pose[2] = er_comp[2];

            /* update cell size */
            er_cell->ce_size += 3;

        }

        /* return cell size */
        return( er_cell->ce_size );

    }

