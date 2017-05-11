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
        er_cell.ce_data = le_array_create();

        /* return created structure */
        return( er_cell );

    }

    le_void_t er_cell_delete( er_cell_t * const er_cell ) {

        /* deleted structure variables */
        er_cell_t er_reset = ER_CELL_C;

        /* delete array structure */
        le_array_delete( & er_cell->ce_data );

        /* delete structure */
        ( * er_cell ) = er_reset;

    }

/*
    source - accessor methods
 */

    le_byte_t er_cell_get_flag( er_cell_t const * const er_cell, le_byte_t const er_state ) {

        /* return state */
        return( er_cell->ce_flag & er_state );

    }

    le_enum_t er_cell_get_match( er_cell_t const * const er_cell, le_address_t const * const er_addr ) {

        /* compare address */
        return( le_address_get_equal( & er_cell->ce_addr, er_addr ) );

    }

    le_size_t er_cell_get_count( er_cell_t const * const er_cell ) {

        /* return cell point count */
        return( le_array_get_size( & er_cell->ce_data ) / LE_ARRAY_UF3 );

    }

    le_real_t * er_cell_get_pose( er_cell_t * const er_cell ) {

        /* return cell geodetic array pointer */
        return( ( le_real_t * ) le_array_get_byte( & er_cell->ce_data ) );

    }

    le_data_t * er_cell_get_data( er_cell_t * const er_cell ) {

        /* return cell colorimetric array pointer */
        return( ( le_data_t * ) ( le_array_get_byte( & er_cell->ce_data ) + LE_ARRAY_UF3_POSE ) );

    }

    le_real_t * er_cell_get_edge( er_cell_t const * const er_cell ) {

        /* return cell edge array pointer */
        return( ( le_real_t * ) er_cell->ce_edge );

    }

    le_size_t er_cell_get_serial( er_cell_t * const er_cell, le_size_t const er_serial, le_array_t * const er_array ) {

        /* serialise cell address */
        return( le_address_serial( & er_cell->ce_addr, er_array, er_serial, _LE_SET ) );

    }

/*
    source - mutator methods
 */

    le_void_t er_cell_set_flag( er_cell_t * const er_cell, le_byte_t const er_state ) {

        /* update state */
        er_cell->ce_flag |= er_state;

    }

    le_void_t er_cell_set_clear( er_cell_t * const er_cell, le_byte_t const er_state ) {

        /* update state */
        er_cell->ce_flag &= ( ~ er_state );

    }

    le_void_t er_cell_set_push( er_cell_t * const er_cell, le_address_t const * const er_addr ) {

        /* assign address */
        er_cell->ce_addr = ( * er_addr );

    }

/*
    source - i/o methods
 */

    le_void_t er_cell_io_read( er_cell_t * const er_cell, le_sock_t const er_socket ) {

        /* socket-array variables */
        le_byte_t * er_head = NULL;
        le_real_t * er_pose = NULL;

        /* parsing variables */
        le_size_t er_size = 0;

        /* optimisation variables */
        le_real_t er_comp[3] = { 0.0 };

        /* read socket-array */
        le_array_io_read( & er_cell->ce_data, er_socket );

        /* decode socket-array */
        le_array_uf3_decode( & er_cell->ce_data );

        /* compute cell edge */
        le_address_get_pose( & er_cell->ce_addr, er_cell->ce_edge );

        /* convert edge coordinates - geographic to cartesian */
        er_cell->ce_edge[2] = LE_ADDRESS_WGSA * cos( er_cell->ce_edge[1] ) * cos( er_cell->ce_edge[0] );
        er_cell->ce_edge[0] = LE_ADDRESS_WGSA * cos( er_cell->ce_edge[1] ) * sin( er_cell->ce_edge[0] );
        er_cell->ce_edge[1] = LE_ADDRESS_WGSA * sin( er_cell->ce_edge[1] );

        /* socket-array data segment */
        er_size = le_array_get_size( & er_cell->ce_data );
        er_head = le_array_get_byte( & er_cell->ce_data );

        /* parsing socket-array */
        for ( le_size_t er_parse = 0; er_parse < er_size; er_parse += LE_ARRAY_UF3 ) {

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

        }

    }

