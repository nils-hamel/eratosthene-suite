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

    /*! \file   eratosthene-client-geodesy.h
     *  \author Nils Hamel <n.hamel@bluewin.ch>
     *
     *  Eratosthene client - Goedesy module
     */

/*
    header - inclusion guard
 */

    # ifndef __ER_CLIENT_GEODESY__
    # define __ER_CLIENT_GEODESY__

/*
    header - C/C++ compatibility
 */

    # ifdef __cplusplus
    extern "C" {
    # endif

/*
    header - includes
 */

    # include <math.h>
    # include <eratosthene-include.h>

/*
    header - preprocessor definitions
 */

    /* Define earth parameters and limits */
    # define ER_ERA ( LE_GEODESY_WGS84_A )
    # define ER_ER2 ( ER_ERA / 2.00 )
    # define ER_ERL ( ER_ERA * 0.75 )
    # define ER_ERU ( ER_ERA * 2.00 )

    /* Define earth motion inertia */
    # define ER_INR ( 0.00100 )
    # define ER_INT ( 0.00005 )
    # define ER_INA ( 0.00500 )

    /* Define conversion factors */
    # define ER_D2R ( 3.1415926535 / 180.0 )
    # define ER_R2D ( 180.0 / 3.1415926535 )

/*
    header - preprocessor macros
 */

/*
    header - type definition
 */

/*
    header - structures
 */

/*
    header - function prototypes
 */

    /*! \brief conversion functions
     */

    le_void_t er_geodesy_cartesian( le_real_t * const er_array, le_size_t const er_count );

    /*! \brief scale functions
     */

    le_real_t er_geodesy_distance( le_real_t const er_distance, le_size_t const er_scale_min, le_size_t const er_scale_max );

    /*! \brief scale functions
     */

    le_real_t er_geodesy_depth( le_real_t const er_distance, le_size_t const er_depth_min, le_size_t const er_depth_max );

    /*! \brief scale functions
     */

    le_real_t er_geodesy_scale( le_real_t const er_altitude );

    /*! \brief scale functions
     */

    le_real_t er_geodesy_near( le_real_t const er_altitude );

    /*! \brief scale functions
     */

    le_real_t er_geodesy_far( le_real_t const er_altitude );
    
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

