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

        /* Return constructed structure */
        return( er_cell );

    }

    le_void_t er_cell_delete( er_cell_t * const er_cell ) {

        /* Cell variables */
        er_cell_t er_reset = ER_CELL_C;

        /* Check array state and memory unallocation */
        if ( er_cell->ce_pose != NULL ) free( er_cell->ce_pose );

        /* Check array state and memory unallocation */
        if ( er_cell->ce_data != NULL ) free( er_cell->ce_data );

        /* Return deleted structure */
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

        /* Return pushed address state */
        return( er_cell->ce_push[0] != '\0' ? _LE_TRUE : _LE_FALSE );

    }

    le_enum_t er_cell_get_match( er_cell_t const * const er_cell, er_cell_t const * const er_push ) {

        /* Return matching state */
        return( strcmp( ( char * ) er_cell->ce_addr, ( char * ) er_push->ce_push ) == 0 ? _LE_TRUE : _LE_FALSE );

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
        le_address_ct_string( er_address, er_cell->ce_addr );

    }

    le_void_t er_cell_set_push( er_cell_t * const er_cell, le_address_t const * const er_address ) {

        /* Compute and assign cell address */
        le_address_ct_string( er_address, er_cell->ce_push );

    }

    le_void_t er_cell_set_pop( er_cell_t * const er_cell ) {

        /* Clear pushed address */
        er_cell->ce_push[0] = '\0';

    }

    le_void_t er_cell_set_swap( er_cell_t * const er_addr, er_cell_t * const er_push ) {

        /* Swap address and pushed address */
        strcpy( ( char * ) er_addr->ce_addr, ( char * ) er_push->ce_push );

    }

    le_void_t er_cell_set_empty( er_cell_t * const er_cell ) {

        /* Empty array */
        er_cell->ce_size = 0;

        /* Clear address */
        er_cell->ce_addr[0] = '\0';

    }

/*
    source - i/o methods
 */

    le_size_t er_cell_io_query( er_cell_t * const er_cell, le_char_t const * const er_ip, le_sock_t const er_port ) {

        /* Socket variables */
        le_sock_t er_socket = _LE_SOCK_NULL;

        /* Returned value variables */
        le_size_t er_return = 0;

        /* Check socket state */
        if ( ( er_socket = le_client_create( er_ip, er_port ) ) != _LE_SOCK_NULL ) {

            /* Server/client handshake */
            if ( le_client_handshake( er_socket, LE_NETWORK_MODE_QMOD, ER_CELL_TFMT ) == LE_ERROR_SUCCESS ) {

                /* Read cell from server */
                er_return = er_cell_io_read( er_cell, er_socket );

            } 

            /* Delete client handle */
            le_client_delete( er_socket );

        }

        /* Send cell size */
        return( er_return );

    }

    le_size_t er_cell_io_read( er_cell_t * const er_cell, le_sock_t const er_socket ) {

        /* Redundant reading variables */
        le_size_t er_read = 0;

        /* Reading variables */
        le_size_t er_parse = 0;
        le_size_t er_count = 0;
        le_size_t er_csize = 0;
        le_size_t er_track = 0;

        /* Socket i/o bridge variables */
        le_size_t er_bridge = 0;

        /* Array pointer variables */
        le_real_t * er_pap = NULL;
        le_data_t * er_dap = NULL;

        /* Address variables */
        static le_address_t er_address = LE_ADDRESS_C;

        /* Socket i/o buffer variables */
        static le_byte_t er_buffer[LE_NETWORK_SB_STRM] = LE_NETWORK_C;

        /* Empty cell */
        er_cell->ce_size = 0;

        /* Write query on socket - send cell size */
        if ( write( er_socket, er_cell->ce_addr, LE_NETWORK_SB_ADDR ) != LE_NETWORK_SB_ADDR ) return( er_cell->ce_size );

        /* Create address from query */
        le_address_cf_string( & er_address, er_cell->ce_addr );

        /* Compute cell edge */
        le_address_get_pose( & er_address, er_cell->ce_edge );

        /* Compute edge cartesian coordinates */
        er_cell->ce_edge[2] = ER_ERA * cos( er_cell->ce_edge[1] ) * cos( er_cell->ce_edge[0] );
        er_cell->ce_edge[0] = ER_ERA * cos( er_cell->ce_edge[1] ) * sin( er_cell->ce_edge[0] );
        er_cell->ce_edge[1] = ER_ERA * sin( er_cell->ce_edge[1] );

        /* Reading query array */
        while ( er_read < _LE_USE_RETRY ) {

            /* Read array from socket */
            if ( ( er_count = read( er_socket, er_buffer + er_bridge, _LE_USE_MTU ) + er_bridge ) >= ER_CELL_TLEN ) {

                /* Check cell limitation */
                if ( ( er_csize = er_cell->ce_size + ( er_count / ER_CELL_TLEN ) * 3 ) < ER_CELL_ARRAY ) {

                    /* Parsing received bloc */
                    for ( er_parse = 0; er_parse < ( er_count / ER_CELL_TLEN ) * ER_CELL_TLEN; er_parse += ER_CELL_TLEN, er_track += 3 ) {

                        /* Compute array pointers */
                        er_pap = ( le_real_t * ) ( er_buffer + er_parse );
                        er_dap = ( le_data_t * ) ( er_pap + 3 );

                        /* Extract element data */
                        er_cell->ce_data[er_track    ] = er_dap[0];
                        er_cell->ce_data[er_track + 1] = er_dap[1];
                        er_cell->ce_data[er_track + 2] = er_dap[2];                        

                        /* Optimised element vertex extraction */
                        er_cell->ce_pose[er_track + 1] = ( er_pap[2] += ER_ERA ) * sin( er_pap[1] ) - er_cell->ce_edge[1];
                        er_cell->ce_pose[er_track    ] = ( er_pap[2] ) * ( er_pap[1] = cos( er_pap[1] ) ) * sin( er_pap[0] ) - er_cell->ce_edge[0];
                        er_cell->ce_pose[er_track + 2] = ( er_pap[2] ) * ( er_pap[1] ) * cos( er_pap[0] ) - er_cell->ce_edge[2];

                    }

                    /* Cell size management */
                    er_cell->ce_size = er_csize;

                    /* Bridge management */
                    if ( ( er_bridge = ( er_count % ER_CELL_TLEN ) ) != 0 ) memcpy( er_buffer, er_buffer + ( er_count - er_bridge ), er_bridge );

                /* Update redundancy */
                er_read = 0; } else { er_read = _LE_USE_RETRY; }
                
            /* Update redundancy */
            } else { er_read ++; }
            
        }        

        /* Send cell size */
        return( er_cell->ce_size );

    }

