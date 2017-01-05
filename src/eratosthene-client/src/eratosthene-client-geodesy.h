/*
 *  eratosthene-suite - client
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
     *  eratosthene-suite client - goedesy module
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

    /*! \brief distance methods
     *
     *  This function computes the distance that separates the point of view
     *  provided as parameter to the pseudo-center of the provided cell.
     *
     *  To compute the distance, the function considers the spatial index hold
     *  by the cell address. It determines the parametric center of the cell and
     *  converts it into earth-attached cartesian coordinates. The coordinates
     *  of the point of view are also converted in cartesian coordinates.
     *
     *  The distance is then computed between these two points using a simple
     *  euclidian distance. The distance, expressed in metres, is then returned
     *  by the function.
     *
     *  \param er_cell Cell structure
     *  \param er_view View structure
     *
     *  \return Returns the distance between the cell and the point of view
     */

    le_real_t er_geodesy_distance( le_address_t const * const er_cell, er_view_t const * const er_view );

    /*! \brief model methods
     *
     *  This function computes and returns the highest distance around the point
     *  of view beyond that a cell is not considered by the model.
     *
     *  The computation is made considering only the height of the point of view
     *  following the logic : as the height gets greater, the view limit of cell
     *  has to be increased.
     *
     *  \param er_altitude Point of view height above WGS84 ellipsoid
     *
     *  \return Returns cell maximum distance
     */

    le_real_t er_geodesy_limit( le_real_t const er_altitude );

    /*! \brief model methods
     *
     *  This function computes, accroding to the distance between the cell and
     *  the point of view, the ideal scale (i.e. the ideal number of digits of
     *  is address statial index) at which a cell is relevant to display. The
     *  two other provided values are used to clamp the result.
     *
     *  This function is mainly used by the model update process. As it parses
     *  the possible cells, it checks using this function if a given cell is
     *  relevant for display of it the enumeration of the possible cells has to
     *  continue.
     *
     *  \param er_distance Distance between the cell and the point of view
     *  \param er_scale    Server maximum number of scale (space parameter)
     *  \param er_depth    Cell address additionnal depth (span)
     *
     *  \return Returns the ideal depth value
     */

    le_real_t er_geodesy_depth( le_real_t const er_distance, le_size_t const er_scale, le_size_t const er_depth );

    /*! \brief model functions
     *
     *  This function computes the scale factor applied to the model according
     *  to the point of view height.
     *
     *  As the height of the point of view gets greater, the scale factor is
     *  reduced. This allows, using simultaneously the near and far planes
     *  modulation functions, to consider the entire earth within a single
     *  continuous model.
     *
     *  \param er_altitude Point of view height above WGS84 ellipsoid
     *
     *  \return Returns the model scale factor
     */

    le_real_t er_geodesy_scale( le_real_t const er_altitude );

    /*! \brief model functions
     *
     *  This function computes the position of the near plane used by opengl.
     *
     *  The position of the near and far planes are computed according to the
     *  point of view height that drives the model scale factor. These two
     *  planes are adapted to the scale factor and earth size to decrease as
     *  much as possible the precision load applied on the depth buffer.
     *
     *  \param er_altitude Point of view height above WGS84 ellipsoid
     *
     *  \return Near plane distance
     */

    le_real_t er_geodesy_near( le_real_t const er_altitude );

    /*! \brief model functions
     *
     *  This function computes the position of the far plane used by opengl.
     *
     *  The position of the near and far planes are computed according to the
     *  point of view height that drives the model scale factor. These two
     *  planes are adapted to the scale factor and earth size to decrease as
     *  much as possible the precision load applied on the depth buffer.
     *
     *  \param er_altitude Point of view height above WGS84 ellipsoid
     *
     *  \return Far plane distance
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

