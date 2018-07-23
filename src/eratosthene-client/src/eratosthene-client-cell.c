/*
 *  eratosthene-suite - client
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
        er_cell_t er_delete = ER_CELL_C;

        /* delete array structure */
        le_array_delete( & er_cell->ce_data );

        /* delete structure */
        ( * er_cell ) = er_delete;

    }

/*
    source - accessor methods
 */


    le_byte_t er_cell_get_flag( er_cell_t const * const er_cell, le_byte_t const er_state ) {

        /* return state */
        return( er_cell->ce_flag & er_state );

    }

    le_enum_t er_cell_get_equal( er_cell_t const * const er_cell, er_cell_t const * const er_targ ) {

        /* compare addresses */
        return( le_address_get_equal( & er_cell->ce_addr, & er_targ->ce_addr ) );

    }

    le_enum_t er_cell_get_drop( er_cell_t const * const er_cell, le_address_t const * const er_addr ) {

        /* compare addresses */
        return( le_address_get_equal( & er_cell->ce_addr, er_addr ) );

    }

    le_size_t er_cell_get_record( er_cell_t const * const er_cell ) {

        /* return number of uf3 records */
        return( le_array_get_size( & er_cell->ce_data ) / LE_ARRAY_UV3 );

    }

    le_size_t er_cell_get_size( er_cell_t const * const er_cell ) {

        /* return address length */
        return( le_address_get_size( & er_cell->ce_addr ) );

    }

    le_real_t * er_cell_get_pose( er_cell_t * const er_cell ) {

        /* return position pointer */
        return( ( le_real_t * ) le_array_get_byte( & er_cell->ce_data ) );

    }

    le_data_t * er_cell_get_data( er_cell_t * const er_cell ) {

        /* return data pointer */
        return( ( le_data_t * ) ( le_array_get_byte( & er_cell->ce_data ) + LE_ARRAY_UV3_POSE + LE_ARRAY_UV3_TYPE ) );

    }

    le_real_t * er_cell_get_edge( er_cell_t * const er_cell ) {

        /* return edge pointer */
        return( er_cell->ce_edge );

    }

    le_size_t er_cell_get_sync( er_cell_t * const er_cell, le_array_t * const er_array, le_size_t const er_offset ) {

        /* de-serialise address */
        return( le_address_serial( & er_cell->ce_addr, er_array, er_offset, _LE_SET ) );

    }

    le_array_t * er_cell_get_array( er_cell_t const * const er_cell ) {

        /* return array */
        return( ( le_array_t * ) & er_cell->ce_data );

    }

/*
    source - mutator methods
 */

    le_void_t er_cell_set_flag( er_cell_t * const er_cell, le_byte_t const er_state ) {

        /* update cell state */
        er_cell->ce_flag |= er_state;

    }

    le_void_t er_cell_set_zero( er_cell_t * const er_cell, le_byte_t const er_state ) {

        /* update cell state */
        er_cell->ce_flag &= ~ er_state;

    }

    le_void_t er_cell_set_push( er_cell_t * const er_cell, le_address_t const * const er_addr ) {

        /* assign address */
        er_cell->ce_addr = ( * er_addr );

    }

    le_size_t er_cell_set_sync( er_cell_t * const er_cell, le_array_t * const er_array, le_size_t const er_offset ) {

        /* serialise address */
        return( le_address_serial( & er_cell->ce_addr, er_array, er_offset, _LE_GET ) );

    }

    le_size_t er_cell_set_data( er_cell_t * const er_cell ) {

        /* pointer variables */
        le_byte_t * er_head = NULL;
        le_byte_t * er_base = NULL;

        /* size variables */
        le_size_t er_size = 0;

        /* optimisation variables */
        le_real_t er_opta = 0.0;
        le_real_t er_optb = 0.0;

        /* retrieve array size */
        if ( ( er_size = le_array_get_size( & er_cell->ce_data ) ) == 0 ) {

            /* return cell size */
            return( er_size );

        }

        /* retrieve array pointer */
        er_head = er_base = le_array_get_byte( & er_cell->ce_data );

        /* coordinates conversion - edge */
        er_cell->ce_edge[2] = ( ( le_real_t * ) er_head )[2] + LE_ADDRESS_WGS_A;

        /* coordinates conversion - edge */
        er_cell->ce_edge[1] = er_cell->ce_edge[2] * sin( ( ( le_real_t * ) er_head )[1] );
        er_cell->ce_edge[2] = er_cell->ce_edge[2] * cos( ( ( le_real_t * ) er_head )[1] );
        er_cell->ce_edge[0] = er_cell->ce_edge[2] * sin( ( ( le_real_t * ) er_head )[0] );
        er_cell->ce_edge[2] = er_cell->ce_edge[2] * cos( ( ( le_real_t * ) er_head )[0] );

        /* inital points coordinates */
        ( ( le_real_t * ) er_head )[0] = 0.0;
        ( ( le_real_t * ) er_head )[1] = 0.0;
        ( ( le_real_t * ) er_head )[2] = 0.0;

        /* parsing socket array */
        while ( ( ( er_head += LE_ARRAY_UV3 ) - er_base ) < er_size ) {

            /* coordinates conversion - points */
            ( ( le_real_t * ) er_head )[2] += LE_ADDRESS_WGS_A;

            /* coordinates conversion - points */
            er_opta = ( ( le_real_t * ) er_head )[0];
            er_optb = ( ( le_real_t * ) er_head )[1];

            /* coordinates conversion - points */
            ( ( le_real_t * ) er_head )[1] = ( ( le_real_t * ) er_head )[2] * sin( er_optb ) - er_cell->ce_edge[1];
            ( ( le_real_t * ) er_head )[2] = ( ( le_real_t * ) er_head )[2] * cos( er_optb );
            ( ( le_real_t * ) er_head )[0] = ( ( le_real_t * ) er_head )[2] * sin( er_opta ) - er_cell->ce_edge[0];
            ( ( le_real_t * ) er_head )[2] = ( ( le_real_t * ) er_head )[2] * cos( er_opta ) - er_cell->ce_edge[2];

        }

        /* return cell size */
        return( er_size );

    }

