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

    le_enum_t er_cell_set_query( er_cell_t * const er_cell, le_char_t const * const er_query, le_sock_t const er_socket ) {

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
        le_time_t * er_ptrt = NULL;
        le_data_t * er_ptrd = NULL;

        /* Cell pointer variables */
        le_real_t * er_celp = NULL;
        le_data_t * er_celd = NULL;

        /* Client/server query handshake */
        if ( le_client_handshake_mode( er_socket, LE_NETWORK_MODE_QMOD ) != LE_ERROR_SUCCESS ) {

            /* Send message */
            return( LE_ERROR_AUTH );

        }

        /* Query string to socket buffer */
        strcpy( ( char * ) er_buffer, ( char * ) er_query );

        /* Write query address */
        if ( write( er_socket, er_buffer, LE_NETWORK_BUFFER_ADDR ) != LE_NETWORK_BUFFER_ADDR ) {

            /* Send message */
            return( LE_ERROR_IO_WRITE );

        }

        /* Reading query elements */
        while( ( er_count = read( er_socket, er_buffer, LE_NETWORK_BUFFER_SYNC ) ) > 0 ) {

            /* Retrieve cell size */
            er_track = er_cell->ce_size;

            /* Resize cell arrays */
            if ( er_cell_set_push( er_cell, ( er_count / LE_ARRAY_LINE ) * 3 ) == LE_ERROR_SUCCESS ) {

                /* Retrieve array pointers */
                er_celp = er_cell_get_pose( er_cell );
                er_celd = er_cell_get_data( er_cell );
                
                /* Parsing received elements */
                for ( er_parse = 0; er_parse < er_count; er_parse += LE_ARRAY_LINE, er_track += 3 ) {

                    /* Compute pointers */
                    er_ptrp = ( le_real_t * ) ( er_buffer + er_parse );
                    er_ptrt = ( le_time_t * ) ( er_ptrp + 3 );
                    er_ptrd = ( le_data_t * ) ( er_ptrt + 1 );

                    /* Assign vertex */
                    er_celp[ er_track + 2 ] = ( ( er_ptrp[2] * 0.001 ) + ER_ERA ) * cos( er_ptrp[1] ) * cos( er_ptrp[0] );
                    er_celp[ er_track     ] = ( ( er_ptrp[2] * 0.001 ) + ER_ERA ) * cos( er_ptrp[1] ) * sin( er_ptrp[0] );
                    er_celp[ er_track + 1 ] = ( ( er_ptrp[2] * 0.001 ) + ER_ERA ) * sin( er_ptrp[1] );

                    /* Assign color */
                    er_celd[ er_track     ] = er_ptrd[0];
                    er_celd[ er_track + 1 ] = er_ptrd[1];
                    er_celd[ er_track + 2 ] = er_ptrd[2];

                }

            }

        }

        /* Send message */
        return( LE_ERROR_SUCCESS );

    }

