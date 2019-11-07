/*
 *  eratosthene-suite - client
 *
 *      Nils Hamel - nils.hamel@bluewin.ch
 *      Copyright (c) 2016-2019 DHLAB, EPFL
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
     *  eratosthene-suite - client - goedesy
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

    # include "eratosthene-client-cell.h"
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
     *  provided as parameter to the pseudo-centre of the provided cell.
     *
     *  To compute the distance, the function considers the spatial index hold
     *  by the cell address. It determines the parametric centre of the cell and
     *  converts it into Earth-attached Cartesian coordinates. The coordinates
     *  of the point of view are also converted in Cartesian coordinates.
     *
     *  The distance is then computed between these two points using a simple
     *  euclidean distance. The distance, expressed in metres, is then returned
     *  by the function.
     *
     *  \param er_cell Cell structure
     *  \param er_view View structure
     *
     *  \return Returns the distance between the cell and the point of view
     */

    le_real_t er_geodesy_distance( le_address_t const * const er_cell, er_view_t const * const er_view );

    /*! \brief geodetic methods
     *
     *  This function computes the angles of view of the Earth according to the
     *  altitude of the viewpoint. This angle separates the direction toward the
     *  Earth center and toward the tangent of the Earth sphere according to
     *  the provided altitude.
     *
     *  \param er_altitude Height above WGS84 ellipsoid (in metres)
     */

    le_real_t er_geodesy_angle( le_real_t const er_altitude );

    /*! \brief geodetic methods
     *
     *  This function computes and returns the maximum cell selection radius
     *  according to the provided viewpoint altitude. It is used during cell
     *  enumeration process to decide if a given cell has to be considered as
     *  part of the 'seen' Earth face.
     *
     *  As the altitude of the point of view decreases, the Earth face is formed
     *  of a smaller portion of Earth surface. This allows to take into account
     *  that far cells become irrelevant from the user point of view.
     *
     *  \param er_altitude Height above WGS84 ellipsoid (in metres)
     *
     *  \return Returns Earth face render distance
     */

    le_real_t er_geodesy_face( le_real_t const er_altitude );

    /*! \brief geodetic methods (revoked)
     *
     *  This function is used to detect cells that are relevant for display
     *  according to the distance between the cell center and the position of
     *  the viewpoint.
     *
     *  The function applies a selection criterion based on the distance to the
     *  cell in order to detect render relevancy. As a cell is detected as
     *  relevant, the function returns \b _LE_TRUE value and the \b _LE_FALSE
     *  value otherwise.
     *
     *  \param er_distance Distance between the cell and the point of view
     *  \param er_view     View structure (point of view)
     *  \param er_scfg     Server spatial parameter
     *  \param er_scale    Cell address length (enumeration scale)
     *
     *  \return Returns \b _LE_TRUE on selection, \b _LE_FALSE otherwise
     */

    le_enum_t er_geodesy_select( le_real_t const er_distance, er_view_t const * const er_view, le_size_t const er_scfg, le_size_t const er_scale );

    /*! \brief geodetic functions
     *
     *  This function computes the scale factor applied to the model according
     *  to the point of view height.
     *
     *  As the height of the point of view gets greater, the scale factor is
     *  reduced. This allows, using simultaneously the near and far planes
     *  modulation functions, to consider the entire Earth within a single
     *  continuous model.
     *
     *  \param er_altitude Height above WGS84 ellipsoid (in metres)
     *
     *  \return Returns the model scale factor
     */

    le_real_t er_geodesy_scale( le_real_t const er_altitude );

    /*! \brief geodetic functions
     *
     *  This function computes the position of the near plane used by OpenGL.
     *
     *  The near plane position is computed taking into account the current
     *  scale of the rendering. It computes an optimal position of the near
     *  plane by reducing its position as the altitude goes down.
     *
     *  The value of both near and far planes are optimised to avoid saturation
     *  of the OpenGL depth buffer.
     *
     *  \param er_altitude Height above WGS84 ellipsoid (in metres)
     *  \param er_scale    Current scale value
     *
     *  \return Near plane distance
     */

    le_real_t er_geodesy_near( le_real_t const er_altitude, le_real_t const er_scale );

    /*! \brief geodetic functions
     *
     *  This function computes the position of the far plane used by OpenGL.
     *
     *  The far plane position is computed taking into account the current scale
     *  factor of the rendering. It also takes into account the viewpoint tilt
     *  angle allowing to decrease the position of the far plane as the tilt
     *  angle goes up.
     *
     *  The far plane position is also used to specify the fog parameters of the
     *  rendering.
     *
     *  The value of both near and far planes are optimised to avoid saturation
     *  of the OpenGL depth buffer.
     *
     *  \param er_altitude Height above WGS84 ellipsoid (in metres)
     *  \param er_gamma    Point of view tilt (gamma) angle (in decimal degree)
     *  \param er_scale    Current scale value
     *
     *  \return Far plane distance
     */

    le_real_t er_geodesy_far( le_real_t const er_altitude, le_real_t const er_tilt, le_real_t const er_scale );

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

