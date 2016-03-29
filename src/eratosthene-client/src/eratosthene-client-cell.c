/*
 *  eratosthene-suite - geodetic system
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

    # include "eratosthene-client-cell.h"

/*
    source - constructor/destructor methods
 */

    er_cell_t er_cell_create( le_size_t er_address ) {

        /* Decomposition variables */
        le_size_t er_index = 0;

        /* Cell variables */
        er_cell_t er_cell = ER_CELL_C;

        /* Compute cell address string */
        for ( ; er_index < 5; er_index ++, er_address /= 4 ) {

            /* Extract address digit */
            er_cell.ce_base[4-er_index] = ( er_address % 4 ) + 48;

        }

        /* Return constructed cell */
        return( er_cell );

    }

    le_void_t er_cell_delete( er_cell_t * const er_cell ) {

        /* Check array state */
        if ( er_cell->ce_pose != NULL ) {

            /* Unallocate cell memory */
            free( er_cell->ce_pose );

            /* Invalidate cell pointer */
            er_cell->ce_pose = NULL;

        }

        /* Check array state */
        if ( er_cell->ce_data != NULL ) {

            /* Unallocate cell memory */
            free( er_cell->ce_data );

            /* Invalidate cell pointer */
            er_cell->ce_data = NULL;

        }

    }

/*
    source - accessor methods
 */

    le_size_t er_cell_get_size( er_cell_t const * const er_cell ) {

        /* Return cell size */
        return( er_cell->ce_size );

    }

    le_real_t * er_cell_get_pose( er_cell_t const * const er_cell ) {

        /* Return position array pointer */
        return( er_cell->ce_pose );

    }

    le_data_t * er_cell_get_data( er_cell_t const * const er_cell ) {

        /* Return color array pointer */
        return( er_cell->ce_data );

    }

/*
    source - mutator methods
 */

    le_enum_t er_cell_set_push( er_cell_t * const er_cell, le_size_t const er_block ) {

        /* Memory reallocation */
        if ( ( er_cell->ce_swap = realloc( ( le_void_t * ) er_cell->ce_pose, ( er_cell->ce_size + er_block ) * sizeof( le_real_t ) ) ) == NULL ) {

            /* Send message */
            return( LE_ERROR_MEMORY );

        }

        /* Assign memory segment */
        er_cell->ce_pose = ( le_real_t * ) er_cell->ce_swap;

        /* Memory reallocation */
        if ( ( er_cell->ce_swap = realloc( ( le_void_t * ) er_cell->ce_data, ( er_cell->ce_size + er_block ) * sizeof( le_data_t ) ) ) == NULL ) {

            /* Send message */
            return( LE_ERROR_MEMORY );

        }

        /* Assign memory segment */
        er_cell->ce_data = ( le_data_t * ) er_cell->ce_swap;

        /* Update cell size */
        er_cell->ce_size += er_block;

        /* Send message */
        return( LE_ERROR_SUCCESS );

    }    
