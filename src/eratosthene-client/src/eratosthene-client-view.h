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

    /*! \file   eratosthene-client-view.h
     *  \author Nils Hamel <nils.hamel@bluewin.ch>
     *
     *  eratosthene-suite - client - views module
     */

/*
    header - inclusion guard
 */

    # ifndef __ER_CLIENT_VIEW__
    # define __ER_CLIENT_VIEW__

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

/*
    header - external includes
 */

/*
    header - preprocessor definitions
 */

    /* define pseudo-constructor */
    # define ER_VIEW_C { 0.0, 0.0, 0.0, 0.0, 0.0, 0, 0, 0, 0, 0 }

    /* define pseudo-constructor - default point of view */
    # define ER_VIEW_D { 12.335435, 45.438531, LE_ADDRESS_WGSA * 1.5, 0.0, 0.0, 1, 0, 950486422, 0, 31536000, 31536000 }

/*
    header - preprocessor macros
 */

/*
    header - type definition
 */

/*
    header - structures
 */

    /*! \struct er_view_struct
     *  \brief View structure
     *
     *  This structure contains the information describing a model point of
     *  view. It includes the point of view position and angles of sight, and
     *  also fields related to the position in times and time management.
     *
     *  The position and orientation of the point of view is stored through
     *  fives fields that gives the longitude, the latitude, the height above
     *  WGS84 ellipsoid and the two angles of sight.
     *
     *  A structure field holds the times comparison mode. As the graphical
     *  client allows to browse two different times simultaneously, the mode
     *  describes how their are handled. The remote server implements five
     *   comparison modes :
     *
     *      mode = 1 : first time only
     *      mode = 2 : second time only
     *      mode = 3 : first time (logical or) second time
     *      mode = 4 : first time (logical and) second time
     *      mode = 5 : first time (logical xor) second time
     *
     *  These modes allow to emphasise the similarities and differences through
     *  time of the earth model.
     *
     *  As two different times can be browsed the time interface, that allows
     *  to choose the value of each time, has to know on which one the focus is
     *  set. This information is stored in the \b vw_act structure field.
     *
     *  The two times are then part of the point of view and their value is kept
     *  in this structure through two fields. In addition to their value, each
     *  of the two times comes with an temporal area size. This size is used by
     *  the time interface during graduation display. Changing the size of the
     *  time area affects the way the time is modified through the time
     *  interface. A large area size induces large modification of the related
     *  time.
     *
     *  \var er_view_struct::vw_lon
     *  Point of view longitude angle, in decimal degrees
     *  \var er_view_struct::vw_lat
     *  Point of view latitude angle, in decimal degrees
     *  \var er_view_struct::vw_alt
     *  Point of view height above WGS84 ellipsoid, in metres
     *  \var er_view_struct::vw_azm
     *  Point of view azimuthal angle, in decimal degrees
     *  \var er_view_struct::vw_gam
     *  Point of view tilt (gamma) angle, in decimal degrees
     *  \var er_view_struct::vw_mod
     *  Point of view times comparison mode
     *  \var er_view_struct::vw_act
     *  Point of view focused time (according to time interface)
     *  \var er_view_struct::vw_tia
     *  Point of view first time value
     *  \var er_view_struct::vw_tib
     *  Point of view second time value
     *  \var er_view_struct::vw_zta
     *  Point of view first time area size (according to time interface)
     *  \var er_view_struct::vw_ztb
     *  Point of view second time area size (according to time interface)
     */

    typedef struct er_view_struct {

        le_real_t vw_lon;
        le_real_t vw_lat;
        le_real_t vw_alt;
        le_real_t vw_azm;
        le_real_t vw_gam;

        le_enum_t vw_mod;
        le_enum_t vw_act;
        le_time_t vw_tia;
        le_time_t vw_tib;
        le_time_t vw_zta;
        le_time_t vw_ztb;

    } er_view_t;


/*
    header - function prototypes
 */

    /*! \brief constructor/destructor methods
     *
     *  This function creates and returns a view structure. It initialises the
     *  structure fields using default values and returns it.
     *
     *  \return Returns the created view structure
     */

    er_view_t er_view_create( le_void_t );

    /*! \brief constructor/destructor methods
     *
     *  This function deletes the view structure provided as parameter. It
     *  simply clears the structure fields.
     *
     *  \param er_view View structure
     */

    le_void_t er_view_delete( er_view_t * const er_view );

    /*! \brief accessor methods
     *
     *  This function compare the content of the two provided view structures
     *  and returns _LE_TRUE if the proper point of view values, those able to
     *  trigger a model update, are identical. The _LE_FALSE value is returned
     *  otherwise.
     *
     *  \param  er_viewa View structure
     *  \param  er_viewb View structure
     *
     *  \return Returns _LE_TRUE on identity, _LE_FALSE otherwise
     */

    le_enum_t er_view_get_equal( er_view_t const * const er_viewa, er_view_t const * const er_viewb );

    /*! \brief accessor methods
     *
     *  This function returns the longitude value contained in the provided view
     *  structure.
     *
     *  \param  er_view View structure
     *
     *  \return Returns the view longitude, in decimal degrees
     */

    le_real_t er_view_get_lon( er_view_t const * const er_view );

    /*! \brief accessor methods
     *
     *  This function returns the latitude value contained in the provided view
     *  structure.
     *
     *  \param  er_view View structure
     *
     *  \return Returns the view latitude, in decimal degrees
     */

    le_real_t er_view_get_lat( er_view_t const * const er_view );

    /*! \brief accessor methods
     *
     *  This function returns the height above WGS84 ellipsoid value contained
     *  in the provided view structure.
     *
     *  \param  er_view View structure
     *
     *  \return Returns the view height above WGS84 ellipsoid, in metres
     */

    le_real_t er_view_get_alt( er_view_t const * const er_view );

    /*! \brief accessor methods
     *
     *  This function returns the azimuthal angle value contained in the view
     *  structure provided as parameter.
     *
     *  \param  er_view View structure
     *
     *  \return Returns the view azimuthal angle, in decimal degrees
     */

    le_real_t er_view_get_azm( er_view_t const * const er_view );

    /*! \brief accessor methods
     *
     *  This function returns the tilt (gamma) angle value contained in the view
     *  structure provided as parameter.
     *
     *  \param  er_view View structure
     *
     *  \return Returns the view tilt (gamma) angle, in decimal degrees
     */

    le_real_t er_view_get_gam( er_view_t const * const er_view );

    /*! \brief accessor methods
     *
     *  This function copies the point of view position of the provided view
     *  structure to the provided position array. In addition to the copy, it
     *  also converts the two parametric angles from degrees to radian.
     *
     *  \param  er_view View structure
     *  \param  er_pose Position array
     */

    le_void_t er_view_get_pose( er_view_t const * const er_view, le_real_t * const er_pose );

    /*! \brief accessor methods
     *
     *  This function returns the times comparison mode value contained in the
     *  provided view structure.
     *
     *  \param  er_view View structure
     *
     *  \return Returns the view times comparison mode
     */

    le_enum_t er_view_get_mode( er_view_t const * const er_view );

    /*! \brief accessor methods
     *
     *  This function returns the index of the focused time pointed by the view
     *  structure provided as parameter.
     *
     *  \param  er_view View structure
     *
     *  \return Returns view focused time index
     */

    le_enum_t er_view_get_active( er_view_t const * const er_view );

    /*! \brief accessor methods
     *
     *  This function creates and returns an address structure initialised using
     *  the provided view structure.
     *
     *  The two times of the view structure are respectively copied in the
     *  address structure depending on the structure times comparison mode. The
     *  first time is copied as the mode value is different of 2 (second time
     *  only). The second time is copied as the mode value is not 1 (first time
     *  only).
     *
     *  In addition, the function also sets the address additional depth (span)
     *  using the graphical client default value \b ER_COMMON_SPAN.
     *
     *  This function is usually used by the model update process to initialise
     *  the enumeration address.
     *
     *  \param  er_view View structure
     *
     *  \return Returns initialised address structure
     */

    le_address_t er_view_get_times( er_view_t const * const er_view );

    /*! \brief accessor methods
     *
     *  This structure returns the time value contained in the provided view
     *  structure. According to the provided index, the function returns the
     *  first or the second time contained in the view structure.
     *
     *  \param  er_view View structure
     *  \param  er_time View structure time index
     *
     *  \return Returns view time
     */

    le_time_t er_view_get_time( er_view_t const * const er_view, le_enum_t const er_time );

    /*! \brief accessor methods
     *
     *  This function returns the time area size contained in the provided view
     *  structure. According to the provided index, the function returns the
     *  first or second time area size contained in the view structure.
     *
     *  \param er_view View structure
     *  \param er_time View structure time index
     *
     *  \return Returns view structure time area size
     */

    le_time_t er_view_get_area( er_view_t const * const er_view, le_enum_t const er_time );

    /*! \brief accessor methods
     *
     *  This function computes an interpolated view structure using the provided
     *  views array and the interpolation parameters.
     *
     *  The integer interpolation parameter gives the array index of the view
     *  that is smaller or equal to the interpolation position. The floating
     *  point parameter gives the position form the previous view, assuming that
     *  the addition of the two parameters is always smaller than the index of
     *  the next array view.
     *
     *  For the positions coordinates and angles of sight of the point of view,
     *  a non linear interpolation is used. The times comparison mode value and
     *  the time focus are set using the values of the array view pointed by the
     *  integer interpolation parameter. The times and time area sizes values
     *  are set using a simple linear interpolation between the two views
     *  surrounding the interpolation position.
     *
     *  \param  er_views View structures array
     *  \param  er_size  View structures array size
     *  \param  er_index Integer interpolation parameter
     *  \param  er_param Floating point interpolation parameter
     *
     *  \return Returns interpolated view
     */

    er_view_t er_view_get_inter( er_view_t const * const er_views, le_size_t const er_size, le_size_t const er_index, le_real_t const er_param );

    /*! \brief mutator methods
     *
     *  This function updates the position of the point of view provided by the
     *  view structure.
     *
     *  It uses the x motion amplitude value to move the point of view along the
     *  direction defined by the azimuthal angle. The y motion amplitude is used
     *  to move the point of view along the perpendicular direction to the
     *  azimuthal direction.
     *
     *  \param er_view   View structure
     *  \param er_xvalue Motion amplitude - prograde
     *  \param er_yvalue Motion amplitude - orthograde
     */

    le_void_t er_view_set_plan( er_view_t * const er_view, le_real_t const er_xvalue, le_real_t const er_yvalue );

    /*! \brief mutator methods
     *
     *  This function adds the provided motion amplitude value to the height of
     *  the view provided as parameter.
     *
     *  After height modification, the function checks its value and correct it
     *  as it goes beyond the authorised range.
     *
     *  \param er_view  View structure
     *  \param er_value Motion amplitude
     */

    le_void_t er_view_set_alt( er_view_t * const er_view, le_real_t const er_value );

    /*! \brief mutator methods
     *
     *  This function adds the provided motion amplitude value to the azimuthal
     *  angle of the provided view structure.
     *
     *  \param er_view  View structure
     *  \param er_value Motion amplitude
     */

    le_void_t er_view_set_azm( er_view_t * const er_view, le_real_t const er_value );

    /*! \brief mutator methods
     *
     *  This function adds the provided motion amplitude value to the tilt angle
     *  of the provided view structure.
     *
     *  In addition, the function checks the updated value of the angle and
     *  correct it as it goes beyond the authorised range.
     *
     *  \param er_view  View structure
     *  \param er_value Motion amplitude
     */

    le_void_t er_view_set_gam( er_view_t * const er_view, le_real_t const er_value );

    /*! \brief mutator methods
     *
     *  This function erases the mode field of the provided view structure using
     *  the provided mode value.
     *
     *  \param er_view View structure
     *  \param er_mode Mode value
     */

    le_void_t er_view_set_mode( er_view_t * const er_view, le_enum_t const er_mode );

    /*! \brief mutator methods
     *
     *  This function swaps the focus between the two times of the provided view
     *  structure. In other words, it simply exchange the activity of the times
     *  of the structure (from the time interface point of view).
     *
     *  \param er_view View structure
     */

    le_void_t er_view_set_swap( er_view_t * const er_view );

    /*! \brief mutator methods
     *
     *  This function aligns the second time of the provided view structure on
     *  the first one. It simply copies the time and time area size values of
     *  the first time to the second.
     *
     *  \param er_view View structure
     */

    le_void_t er_view_set_times( er_view_t * const er_view );

    /*! \brief mutator methods
     *
     *  This function assign the provided time to the time of the provided view
     *  structure. The index of the time to set is retrieved through the focus
     *  value hold in the structure. In other word, this function only allows
     *  to specify the structure active time (from the time interface point of
     *  view).
     *
     *  \param er_view  View structure
     *  \param er_value Time value
     */

    le_void_t er_view_set_time( er_view_t * const er_view, le_real_t const er_value );

    /*! \brief mutator methods
     *
     *  This function assign the provided time area size to the time area size
     *  of the provided view structure. As for \b er_view_set_time() function,
     *  this function uses the structure focus to choose the time. In other
     *  words, this function only allows to set the time area size of the active
     *  time (from the time interface point of view).
     *
     *  \param er_view  View structure
     *  \param er_value Time area size value
     */

    le_void_t er_view_set_area( er_view_t * const er_view, le_real_t const er_value );

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

