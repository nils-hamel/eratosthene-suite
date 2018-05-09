/*
 *  eratosthene-suite - client
 *
 *      Nils Hamel - nils.hamel@bluewin.ch
 *      Copyright (c) 2016-2018 DHLAB, EPFL
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
     *  eratosthene-suite - client - view
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
    # define ER_VIEW_C { 0.0, 0.0, 0.0, 0.0, 0.0, 0, 0, 0, 0, ER_COMMON_SPD }

    /* define pseudo-constructor - default point of view */
    # define ER_VIEW_D { 12.335435, 45.438531, LE_ADDRESS_WGS_A * 1.5, 0.0, 0.0, 1, 950486422, 0, 31536000, 31536000, ER_COMMON_SPD }

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
     *  comparison modes :
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
        le_time_t vw_tia;
        le_time_t vw_tib;
        le_time_t vw_zta;
        le_time_t vw_ztb;

        le_size_t vw_spn;

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
     *  This function computes and return the inertia factor that is used to
     *  modulate the point of view modification. In addition to the position
     *  coordinates, the function also checks the keyboard modifiers to adapt
     *  the inertia value.
     *
     *  As the point of view get closer to the ground surface, the motion has
     *  to be reduced in order to keep relevant displacement speed. In addition,
     *  if left-control key is pressed, the inertia is augmented to speed up
     *  the point of view. If the left-shift key is pressed, the inertia is
     *  decreased to allow fine tune of the point of view.
     *
     *  \param er_view     View structure
     *  \param er_modifier Keyboard modifier flags
     *
     *  \return Returns the computed inertia value
     */

    le_real_t er_view_get_inertia( er_view_t const * const er_view, le_enum_t const er_modifier );

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
     *  This function returns the span value of the provided view structure.
     *
     *  \param er_view View structure
     * 
     *  \return Returns view structure span value
     */

    le_size_t er_view_get_span( er_view_t const * const er_view );

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
     *  This function is used for alignment of the two times of the provided
     *  view structure.
     *
     *  If the mode is 1, the second time is aligned on the first one. If the
     *  mode is two, the first time is aligned on the second one. This also
     *  align the respective area fields of each time.
     *
     *  \param er_view View structure
     */

    le_void_t er_view_set_times( er_view_t * const er_view );

    /*! \brief mutator methods
     *
     *  This function updates the time value of the provided view structure using
     *  its respective area. The provided value is used as a factor applied to
     *  the area for the time update.
     *
     *  As the structure holds two times, the time that correspond to the active
     *  mode is updated through a call to this function.
     *
     *  \param er_view  View structure
     *  \param er_value Time value
     */

    le_void_t er_view_set_time( er_view_t * const er_view, le_real_t const er_value );

    /*! \brief mutator methods
     *
     *  This function updates the area value of the time corresponding to the
     *  active mode.
     *
     *  \param er_view  View structure
     *  \param er_value Time area size value
     */

    le_void_t er_view_set_area( er_view_t * const er_view, le_real_t const er_value );

    /*! \brief mutator methods
     *
     *  This function updates the span value of the provided view structure. It
     *  adds the provided value, that can be negative or positive, to the span
     *  value of the view before to clamp the result in the predefined range.
     *
     *  \param er_view  View structure
     *  \param er_value Span shift value
     */

    le_void_t er_view_set_span( er_view_t * const er_view, le_size_t const er_value );

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

