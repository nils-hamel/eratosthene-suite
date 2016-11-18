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

        /* created structure variables */
        er_cell_t er_cell = ER_CELL_C;

        /* allocate geodetic array memory */
        er_cell.ce_pose = malloc( sizeof( le_real_t ) * ER_CELL_ARRAY );

        /* allocate colorimetric array memory */
        er_cell.ce_data = malloc( sizeof( le_data_t ) * ER_CELL_ARRAY );

        /* return created structure */
        return( er_cell );

    }

    le_void_t er_cell_delete( er_cell_t * const er_cell ) {

        /* deleted structure variables */
        er_cell_t er_reset = ER_CELL_C;

        /* check array state - memory unallocation */
        if ( er_cell->ce_pose != NULL ) free( er_cell->ce_pose );

        /* check array state - memory unallocation */
        if ( er_cell->ce_data != NULL ) free( er_cell->ce_data );

        /* delete structure */
        * ( er_cell ) = er_reset;

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
        return( ( le_real_t * ) er_cell->ce_pose );

    }

    le_data_t * er_cell_get_data( er_cell_t const * const er_cell ) {

        /* return cell colorimetric array pointer */
        return( ( le_data_t * ) er_cell->ce_data );

    }

    le_real_t * er_cell_get_edge( er_cell_t const * const er_cell ) {

        /* return cell edge array pointer */
        return( ( le_real_t * ) er_cell->ce_edge );

    }

    le_address_t er_cell_get_addr( er_cell_t const * const er_cell ) {

        /* return cell address */
        return( er_cell->ce_addr );

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

    le_void_t er_cell_set_push( er_cell_t * const er_cell, le_address_t const * const er_address ) {

        /* compute and assign cell address */
        er_cell->ce_push = ( * er_address );

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

    le_size_t er_cell_io_query( er_cell_t * const er_cell, le_char_t const * const er_ip, le_sock_t const er_port ) {

        /* returned value variables */
        le_size_t er_return = 0;

        /* socket variables */
        le_sock_t er_socket = _LE_SOCK_NULL;

        /* check socket state */
        if ( ( er_socket = le_client_create( er_ip, er_port ) ) != _LE_SOCK_NULL ) {

            /* server/client handshake */
            if ( le_client_handshake( er_socket, LE_NETWORK_MODE_QMOD ) == LE_ERROR_SUCCESS ) {

                /* read cell from server */
                er_return = er_cell_io_read( er_cell, er_socket );

            } 

            /* delete client handle */
            le_client_delete( er_socket );

        }

        /* send cell size */
        return( er_return );

    }

    le_size_t er_cell_io_read( er_cell_t * const er_cell, le_sock_t const er_socket ) {

        /* redundant reading variables */
        le_size_t er_read = 0;

        /* reading variables */
        le_size_t er_parse = 0;
        le_size_t er_count = 0;
        le_size_t er_csize = 0;
        le_size_t er_track = 0;

        /* socket i/o bridge variables */
        le_size_t er_bridge = 0;

        /* array pointer variables */
        le_real_t * er_pap = NULL;
        le_data_t * er_dap = NULL;

        /* socket i/o buffer variables */
        static le_byte_t er_buffer[LE_NETWORK_SB_STRM] = LE_NETWORK_C;

        /* write query address on socket */
        le_address_io_write( & er_cell->ce_addr, er_socket );

        /* read query address on socket */
        le_address_io_read( & er_cell->ce_addr, er_socket );

        /* extract cell edge components */
        le_address_get_pose( & er_cell->ce_addr, er_cell->ce_edge );

        /* compute edge cartesian coordinates */
        er_cell->ce_edge[2] = LE_GEODESY_WGS84_A * cos( er_cell->ce_edge[1] ) * cos( er_cell->ce_edge[0] );
        er_cell->ce_edge[0] = LE_GEODESY_WGS84_A * cos( er_cell->ce_edge[1] ) * sin( er_cell->ce_edge[0] );
        er_cell->ce_edge[1] = LE_GEODESY_WGS84_A * sin( er_cell->ce_edge[1] );

        /* reset cell array size */
        er_cell->ce_size = 0;

        /* reading query array */
        while ( er_read < _LE_USE_RETRY ) {

            /* read array from socket */
            if ( ( er_count = read( er_socket, er_buffer + er_bridge, _LE_USE_MTU ) + er_bridge ) >= LE_ARRAY_RFL ) {

                /* check cell limitation */
                if ( ( er_csize = er_cell->ce_size + ( er_count / LE_ARRAY_RFL ) * 3 ) < ER_CELL_ARRAY ) {

                    /* parsing received bloc */
                    for ( er_parse = 0; er_parse < ( er_count / LE_ARRAY_RFL ) * LE_ARRAY_RFL; er_track += 3, er_parse += LE_ARRAY_RFL ) {

                        /* compute array pointers */
                        er_pap = ( le_real_t * ) ( er_buffer + er_parse );
                        er_dap = ( le_data_t * ) ( er_pap + 3 );

                        /* extract element data */
                        er_cell->ce_data[er_track    ] = er_dap[0];
                        er_cell->ce_data[er_track + 1] = er_dap[1];
                        er_cell->ce_data[er_track + 2] = er_dap[2];                        

                        /* optimised element vertex extraction */
                        er_cell->ce_pose[er_track + 1] = - er_cell->ce_edge[1] + sin( er_pap[1] ) * ( er_pap[2] += LE_GEODESY_WGS84_A );
                        er_cell->ce_pose[er_track    ] = - er_cell->ce_edge[0] + er_pap[2] * sin( er_pap[0] ) * ( er_pap[1] = cos( er_pap[1] ) );
                        er_cell->ce_pose[er_track + 2] = - er_cell->ce_edge[2] + er_pap[2] * er_pap[1] * cos( er_pap[0] );

                    }

                    /* cell size management */
                    er_cell->ce_size = er_csize;

                    /* bridge management */
                    if ( ( er_bridge = ( er_count % LE_ARRAY_RFL ) ) != 0 ) memcpy( er_buffer, er_buffer + ( er_count - er_bridge ), er_bridge );

                /* reset redundancy */
                er_read = 0; } else { er_read = _LE_USE_RETRY; }
                
            /* update redundancy */
            } else { er_read ++; }

        }

        /* send cell size */
        return( er_cell->ce_size );

    }

