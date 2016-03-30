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

    /*! \file   eratosthene-client-cell.h
     *  \author Nils Hamel <n.hamel@bluewin.ch>
     *
     *  Eratosthene client - Cell module
     */

/*
    header - inclusion guard
 */

    # ifndef __ER_CLIENT_CELL__
    # define __ER_CLIENT_CELL__

/*
    header - C/C++ compatibility
 */

    # ifdef __cplusplus
    extern "C" {
    # endif

/*
    header - includes
 */

    # include "eratosthene-client-geodesy.h"
    # include <math.h>
    # include <eratosthene-include.h>

/*
    header - preprocessor definitions
 */

    /* Define pseudo-constructor */
    # define ER_CELL_C      { ER_CELL_IDLE, { 0 }, { 0 }, { 0 }, 0, NULL, NULL }

    /* Define cell states */
    # define ER_CELL_IDLE   ( 0x00 )
    # define ER_CELL_UPDATE ( 0x01 )
    # define ER_CELL_DRAW   ( 0x02 )

/*
    header - preprocessor macros
 */

/*
    header - type definition
 */

/*
    header - structures
 */

    typedef struct er_cell_struct {

        le_enum_t   ce_stat;
        le_char_t   ce_base[_LE_USE_DEPTH];
        le_char_t   ce_push[_LE_USE_DEPTH];
        le_char_t   ce_addr[_LE_USE_DEPTH];
        le_size_t   ce_size;
        le_real_t * ce_pose;
        le_data_t * ce_data;
        
    } er_cell_t;

/*
    header - function prototypes
 */

    /*! \brief constructor/destructor methods
     */

    er_cell_t er_cell_create( le_size_t er_address );

    /*! \brief constructor/destructor methods
     */

    le_void_t er_cell_delete( er_cell_t * const er_cell );

    /*! \brief accessor methods
     */

    le_enum_t er_cell_get_state( er_cell_t const * const er_cell );

    /*! \brief accessor methods
     */

    le_enum_t er_cell_get_update( er_cell_t const * const er_cell );

    /*! \brief accessor methods
     */

    le_size_t er_cell_get_size( er_cell_t const * const er_cell );

    /*! \brief accessor methods
     */

    le_real_t * er_cell_get_pose( er_cell_t const * const er_cell );

    /*! \brief accessor methods
     */

    le_data_t * er_cell_get_data( er_cell_t const * const er_cell );

    /*! \brief mutator methods
     */

    le_enum_t er_cell_set_addr( er_cell_t * const er_cell, le_char_t const * const er_push );

    /*! \brief mutator methods
     */

    le_enum_t er_cell_set_push( er_cell_t * const er_cell, le_size_t const er_block );

    /*! \brief mutator methods
     */

    le_void_t er_cell_set_query( er_cell_t * const er_cell, le_sock_t const er_socket );

/*
    header - C/C++ compatibility
 */

    # ifdef __cplusplus
    }
    # endif

/*
    header - inclusion guard
 */

    # endif

