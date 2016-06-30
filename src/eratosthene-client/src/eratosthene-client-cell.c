/*
 *  eratosthene-suite - eratosthene indexation server front-end
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

    er_cell_t er_cell_create( le_void_t ) {

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

        /* Clear cell structure */
        * ( er_cell ) = er_reset;

    }

/*
    source - accessor methods
 */

    le_enum_t er_cell_get_flag( er_cell_t const * const er_cell ) {

        /* Return cell flag */
        return( er_cell->ce_flag );

    }

    le_enum_t er_cell_get_push( er_cell_t const * const er_cell ) {

        /* Check pushed address */
        if ( ( er_cell->ce_push )[0] != '\0' ) {

            /* Return positive answer */
            return( _LE_TRUE );

        } else {

            /* Return negative answer */
            return( _LE_FALSE );

        }

    }

    le_enum_t er_cell_get_match( er_cell_t const * const er_cell, er_cell_t const * const er_push ) {

        /* Compare address and pushed address */
        if ( strcmp( ( char * ) er_cell->ce_addr, ( char * ) er_push->ce_push ) == 0 ) {

            /* Return positive answer */
            return( _LE_TRUE );

        } else {

            /* Return negative answer */
            return( _LE_FALSE );

        }

    }

    le_size_t er_cell_get_size( er_cell_t const * const er_cell ) {

        /* Return cell size */
        return( er_cell->ce_size );

    }

    le_real_t * er_cell_get_pose( er_cell_t const * const er_cell ) {

        /* Return cell geodetic array pointer */
        return( er_cell->ce_pose );

    }

    le_data_t * er_cell_get_data( er_cell_t const * const er_cell ) {

        /* Return cell colorimetric array pointer */
        return( er_cell->ce_data );

    }

/*
    source - mutator methods
 */

    le_void_t er_cell_set_flag( er_cell_t * const er_cell, le_enum_t const er_flag ) {

        /* Assign cell flag */
        er_cell->ce_flag = er_flag;

    }

    le_void_t er_cell_set_addr( er_cell_t * const er_cell, le_address_t const * const er_address ) {

        /* Compute and assign cell address */
        le_address_cvas( er_address, er_cell->ce_addr );

    }

    le_void_t er_cell_set_push( er_cell_t * const er_cell, le_address_t const * const er_address ) {

        /* Compute and assign cell address */
        le_address_cvas( er_address, er_cell->ce_push );

    }

    le_void_t er_cell_set_pop( er_cell_t * const er_cell ) {

        /* Clear pushed address */
        ( er_cell->ce_push )[0] = '\0';

    }

    le_void_t er_cell_set_swap( er_cell_t * const er_addr, er_cell_t * const er_push ) {

        /* Swap address and pushed address */
        strcpy( ( char * ) er_addr->ce_addr, ( char * ) er_push->ce_push );

    }

    le_void_t er_cell_set_empty( er_cell_t * const er_cell ) {

        /* Empty array */
        er_cell->ce_size = 0;

        /* Clear address */
        ( er_cell->ce_addr )[0] = '\0';

    }

    le_void_t er_cell_set_query( er_cell_t * const er_cell, le_sock_t const er_socket ) {

        /* Parsing variables */
        le_size_t er_parse = 0;

        /* Tracking variables */
        le_size_t er_track = 0;

        /* Socket i/o count variables */
        le_size_t er_count = 0;

        /* Optimisation variables */
        le_size_t er_size = 0;

        /* Socket i/o variables */
        le_size_t er_read = 0;
        le_size_t er_bridge = 0;

        /* Array pointer variables */
        le_real_t * er_ptrp = NULL;
        le_data_t * er_ptrd = NULL;

        /* Socket i/o buffer variables */
        le_byte_t er_buffer[LE_NETWORK_SB_STRM] = LE_NETWORK_C;

        /* Reset cell size */
        er_cell->ce_size = 0;

        /* Convert query string */
        le_address_cvsa( & er_cell->ce_cell, er_cell->ce_addr );

        /* Retrieve cell position */
        le_address_get_pose( & er_cell->ce_cell, er_cell->ce_edge );

        /* Optimised edge computation */
        er_cell->ce_edge[1] = er_cell->ce_edge[1];

        /* Optimised edge computation */
        er_cell->ce_edge[2] = ER_ERA * cos( er_cell->ce_edge[1] ) * cos( er_cell->ce_edge[0] );
        er_cell->ce_edge[0] = ER_ERA * cos( er_cell->ce_edge[1] ) * sin( er_cell->ce_edge[0] );

        /* Optimised edge computation */
        er_cell->ce_edge[1] = ER_ERA * sin( er_cell->ce_edge[1] );

        /* Client/server query handshake */
        if ( le_client_handshake_mode( er_socket, LE_NETWORK_MODE_QMOD, LE_ARRAY_64R ) != LE_ERROR_SUCCESS ) {

            /* Abort update */
            return;

        }

        /* Write query address */
        if ( write( er_socket, er_cell->ce_addr, LE_NETWORK_SB_ADDR ) != LE_NETWORK_SB_ADDR ) {

            /* Abort update */
            return;

        }

        /* Reading query elements */
        while ( er_read < _LE_USE_RETRY ) {

            /* Read bloc from socket */
            if ( ( er_count = read( er_socket, er_buffer + er_bridge, _LE_USE_MTU ) + er_bridge ) >= LE_ARRAY_64R_LEN ) {

                /* Check cell limitation */
                if ( ( er_size = er_cell->ce_size + ( er_count / LE_ARRAY_64R_LEN ) * 3 ) < ER_CELL_ARRAY ) {

                    /* Parsing received bloc */
                    for ( er_parse = 0; er_parse < (er_count/LE_ARRAY_64R_LEN)*LE_ARRAY_64R_LEN; er_parse += LE_ARRAY_64R_LEN, er_track += 3 ) {

                        /* Compute pointers */
                        er_ptrp = ( le_real_t * ) ( er_buffer + er_parse );
                        er_ptrd = ( le_data_t * ) ( er_ptrp + 3 );

                        /* Optimised vertex computation */
                        er_ptrp[2] += ER_ERA;

                        /* Optimised vertex computation */
                        er_cell->ce_pose[er_track + 1] = er_ptrp[2] * sin( er_ptrp[1] ) - er_cell->ce_edge[1];

                        /* Optimised vertex computation */
                        er_ptrp[1] = cos( er_ptrp[1] );

                        /* Optimised vertex computation */
                        er_cell->ce_pose[er_track    ] = er_ptrp[2] * er_ptrp[1] * sin( er_ptrp[0] ) - er_cell->ce_edge[0];
                        er_cell->ce_pose[er_track + 2] = er_ptrp[2] * er_ptrp[1] * cos( er_ptrp[0] ) - er_cell->ce_edge[2];

                        /* Assign color */
                        er_cell->ce_data[er_track    ] = er_ptrd[0];
                        er_cell->ce_data[er_track + 1] = er_ptrd[1];
                        er_cell->ce_data[er_track + 2] = er_ptrd[2];

                    }

                    /* Cell size management */
                    er_cell->ce_size = er_size;

                }

                /* Bridge management */
                if ( ( er_bridge = ( er_count % LE_ARRAY_64R_LEN ) ) != 0 ) {

                    /* Displace bridge data */
                    memcpy( er_buffer, er_buffer + ( er_count - er_bridge ), er_bridge );

                }
                
            /* Update reading value */
            er_read = 0; } else { er_read ++; }
            
        }

    }

