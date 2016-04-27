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

        /* Cell variables */
        er_cell_t er_cell = ER_CELL_C;

        /* Allocate geodetic array memory */
        er_cell.ce_pose = malloc( sizeof( le_real_t ) * ER_CELL_ARRAY );

        /* Allocate colorimetric array memory */
        er_cell.ce_data = malloc( sizeof( le_data_t ) * ER_CELL_ARRAY );

        /* Return constructed cell */
        return( er_cell );

    }

    le_void_t er_cell_delete( er_cell_t * const er_cell ) {

        /* Cell variables */
        er_cell_t er_reset = ER_CELL_C;

        /* Check array state and memory unallocation */
        if ( er_cell->ce_pose != NULL ) free( er_cell->ce_pose );

        /* Check array state and memory unallocation */
        if ( er_cell->ce_data != NULL ) free( er_cell->ce_data );

        /* Clear cell fields */
        * er_cell = er_reset;

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

    le_void_t er_cell_set_query( er_cell_t * const er_cell, le_sock_t const er_socket ) {

        /* Parsing variables */
        le_size_t er_parse = 0;

        /* Tracking variables */
        le_size_t er_track = 0;

        /* Socket i/o count variables */
        le_size_t er_count = 0;

        /* Socket i/o buffer variables */
        le_byte_t er_buffer[LE_NETWORK_BUFFER_SYNC] = LE_NETWORK_BUFFER_C;

        /* Array pointer variables */
        le_real_t * er_ptrp = NULL;
        le_data_t * er_ptrd = NULL;

        /* Client/server query handshake */
        if ( le_client_handshake_mode( er_socket, LE_NETWORK_MODE_QMOD ) != LE_ERROR_SUCCESS ) {

            /* Abort update */
            return;

        }

        /* Query string to socket buffer */
        strcpy( ( char * ) er_buffer, ( char * ) er_cell->ce_push );

        /* Write query address */
        if ( write( er_socket, er_buffer, LE_NETWORK_BUFFER_ADDR ) != LE_NETWORK_BUFFER_ADDR ) {

            /* Abort update */
            return;

        }

        /* Query string to cell address */
        strcpy( ( char * ) er_cell->ce_addr, ( char * ) er_cell->ce_push );

        /* Reset cell size */
        er_cell->ce_size = 0;

        /* Reading query elements */
        while( ( er_count = read( er_socket, er_buffer, LE_NETWORK_BUFFER_SYNC ) ) > 0 ) {
                
            /* Parsing received elements */
            for ( er_parse = 0; er_parse < er_count; er_parse += LE_ARRAY_LINE, er_track += 3 ) {

                /* Compute pointers */
                er_ptrp = ( le_real_t * ) ( er_buffer + er_parse      );
                er_ptrd = ( le_data_t * ) ( er_buffer + er_parse + 32 );

                /* Assign vertex */
                er_cell->ce_pose[er_track + 2] = ( ( er_ptrp[2] * 0.001 ) + ER_ERA ) * cos( er_ptrp[1] ) * cos( er_ptrp[0] );
                er_cell->ce_pose[er_track    ] = ( ( er_ptrp[2] * 0.001 ) + ER_ERA ) * cos( er_ptrp[1] ) * sin( er_ptrp[0] );
                er_cell->ce_pose[er_track + 1] = ( ( er_ptrp[2] * 0.001 ) + ER_ERA ) * sin( er_ptrp[1] );

                /* Assign color */
                er_cell->ce_data[er_track    ] = er_ptrd[0];
                er_cell->ce_data[er_track + 1] = er_ptrd[1];
                er_cell->ce_data[er_track + 2] = er_ptrd[2];

            }

            /* Update cell size */
            er_cell->ce_size += ( er_count / LE_ARRAY_LINE ) * 3;

        }

    }

