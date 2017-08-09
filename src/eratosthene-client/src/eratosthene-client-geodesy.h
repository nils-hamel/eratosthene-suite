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
     *  eratosthene-suite - client - goedesic module
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
     *  provided as parameter to the pseudo-centre of the provided cell.
     *
     *  To compute the distance, the function considers the spatial index hold
     *  by the cell address. It determines the parametric centre of the cell and
     *  converts it into earth-attached Cartesian coordinates. The coordinates
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

    /*! \brief model methods
     *
     *  This function computes and returns the distance to the point of view at
     *  which earth surface elements have to be considered. It then defines, for
     *  any altitude, which portion of earth surface is to be considered as
     *  earth face seen by the user at the point of view.
     *
     *  As the altitude of the point of view decreases, the earth face is formed
     *  of a smaller portion of earth surface. This allows to take into account
     *  that far elements become irrelevant, from the display point of view, as
     *  the point of view gets closer to earth.
     *
     *  This function is both used to drive to position of the OpenGL far plane
     *  and to preselect earth model cells during cells enumeration at model
     *  update.
     *
     *  \param er_altitude Point of view height above WGS84 ellipsoid
     *
     *  \return Returns earth face render distance
     */

    le_real_t er_geodesy_face( le_real_t const er_altitude );

    /*! \brief model methods
     *
     *  This function, similar to the \b er_geodesy_face() function, computes
     *  and returns the maximum render distance of cells according to the height
     *  of the point of view.
     *
     *  As the point of view height decreases, the cell render distance becomes
     *  smaller. This allows to take into account that distant cells are not
     *  relevant for display as the point of view get closer to the earth
     *  surface.
     *
     *  This function is both used to drive to position of the OpenGL far plane
     *  and also during model update. As a cell is detected relevant for display
     *  this function is called to discard cells that are too far away according
     *  to point of view height.
     *
     *  \param er_altitude Point of view height above WGS84 ellipsoid
     *
     *  \return Returns cells render distance
     */

    le_real_t er_geodesy_radius( le_real_t const er_altitude );

    /*! \brief model methods
     *
     *  This function computes, according to the distance between the cell and
     *  the point of view, the ideal scale (i.e. the ideal number of digits of
     *  is address spatial index) at which a cell is relevant to display. The
     *  two other provided values are used to clamp the result.
     *
     *  This function is mainly used by the model update process. As it parses
     *  the possible cells it checks, using this function, if a given cell is
     *  relevant for display. Otherwise, the enumeration continues considering
     *  the cell daughters.
     *
     *  \param er_distance Distance between the cell and the point of view
     *  \param er_scfg     Server maximum number of scale (space parameter)
     *  \param er_scale    Enumeration scale - cell address size
     *
     *  \return Returns the ideal depth value
     */

    le_enum_t er_geodesy_select( le_real_t const er_distance, le_real_t const er_scfg, le_real_t const er_scale );

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
     *  This function computes the position of the near plane used by OpenGL.
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
     *  This function computes the position of the far plane used by OpenGL.
     *
     *  The position of the near and far planes are computed according to the
     *  point of view height that drives the model scale factor. These two
     *  planes are adapted to the scale factor and earth size to decrease as
     *  much as possible the precision load applied on the depth buffer.
     *
     *  In addition, the far plane position computation is linked to the model
     *  cells render limit as the altitude of the point of view goes toward
     *  zeros. This allows to fade (fog) the model view at limit of display.
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

