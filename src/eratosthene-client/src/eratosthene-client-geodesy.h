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

    /*! \file   eratosthene-client-geodesy.h
     *  \author Nils Hamel <n.hamel@bluewin.ch>
     *
     *  Eratosthene client - goedesy module
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
    header - external includes
 */

    # include <math.h>
    # include <eratosthene-include.h>

/*
    header - preprocessor definitions
 */

    /* Define geodetic parameters */
    # define ER_ERA ( LE_GEODESY_WGS84_A )
    # define ER_ER2 ( ER_ERA / 2.00 )
    # define ER_ERL ( ER_ERA * 0.75 )
    # define ER_ERU ( ER_ERA * 3.00 )
    # define ER_ERD ( ER_ERA * 2.00 )

    /* Define motion inertia */
    # define ER_INB ( 5.0 / 1000.0 )
    # define ER_INR ( 1.0 / 1000.0 )
    # define ER_INT ( LE_PI / ( LE_GEODESY_WGS84_A * 180.0 ) )

    /* Define motion inertia multipliers */
    # define ER_IML (  0.1 )
    # define ER_IMU ( 10.0 )

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
     *
     *  This function converts spherical coordinates into cartesian coordinates
     *  of the element contained in the array \b er_array. The \b er_count
     *  parameter has to give the number of elements of the array, being of
     *  dimension one, rather than the number of points.
     *
     *  The results of the conversion are place in the array, replacing the
     *  spherical elements.
     *
     *  The spherical coordinates expects the direct frame defined by the
     *  longitude, latitude and radial vector, in this order.
     *
     *  \param er_array Array structure
     *  \param er_count Number of elements in the array
     */

    le_void_t er_geodesy_cartesian( le_real_t * const er_array, le_size_t const er_count );

    /*! \brief cell functions
     *
     *  This function computes the cartesian distance between the center of the
     *  class represented by the address structure to the position provided as
     *  parameter, typically a point of view.
     *
     *  \param er_addr Cell address structure
     *  \param er_lon  Longitude
     *  \param er_lat  Latitude
     *  \param er_alt  Altitude (radial distance)
     *
     *  \return Distance between cell center and the provided position
     */

    le_real_t er_geodesy_cell( le_address_t * const er_addr, le_real_t const er_lon, le_real_t const er_lat, le_real_t const er_alt );

    /*! \brief model functions
     *
     *  This function computes the floating point scale index of a provided
     *  equivalence class (represented by its distance to the model point of
     *  view) that determine the scale at which class have to be queried.
     *
     *  \param er_distance  Distance between class center and point of view
     *  \param er_scale_min Minimum authorised scale
     *  \param er_scale_max Maximum authorised scale
     *
     *  \return Class gathering initial scale
     */

    le_real_t er_geodesy_distance( le_real_t const er_distance, le_size_t const er_scale_min, le_size_t const er_scale_max );

    /*! \brief model functions
     *
     *  This function computes the scale factor applied to the model in function
     *  of the distance of the point of view. The altitude, here the radial
     *  distance to the center of earth, is considered as the only parameter
     *  that drive the scale factor.
     *
     *  \param er_altitude Radial position of the point of view
     *
     *  \return Scale factor
     */

    le_real_t er_geodesy_scale( le_real_t const er_altitude );

    /*! \brief model functions
     *
     *  This function computes the position of the near plane used by OpenGL
     *  according to the radial position of the point of view.
     *
     *  \param er_altitude Radial position of the point of view
     *
     *  \return Near plane distance
     */

    le_real_t er_geodesy_near( le_real_t const er_altitude );

    /*! \brief model functions
     *
     *  This function computes the position of the far plane used by OpenGL
     *  according to the radial position of the point of view.
     *
     *  \param er_altitude Radial position of the point of view
     *
     *  \return Fat plane distance
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

