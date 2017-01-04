/*
 *  eratosthene-suite - eratosthene indexation server front-end
 *
 *      Nils Hamel - nils.hamel@bluewin.ch
 *      Copyright (c) 2016-2017 EPFL CDH DHLAB
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
     *  \author Nils Hamel <nils.hamel@bluewin.ch>
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
    header - internal includes
 */

    # include "eratosthene-client-common.h"
    # include "eratosthene-client-view.h"

/*
    header - external includes
 */

/*
    header - preprocessor definitions
 */

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

    le_real_t er_geodesy_distance( le_address_t const * const er_cell, er_view_t const * const er_view );

    le_real_t er_geodesy_limit( le_real_t const er_altitude );

    le_real_t er_geodesy_depth( le_real_t const er_distance, le_size_t const er_scale, le_size_t const er_depth );

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

