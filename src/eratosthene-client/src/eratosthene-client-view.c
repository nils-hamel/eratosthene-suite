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

    # include "eratosthene-client-view.h"

/*
    source - constructor/destructor methods
 */

    er_view_t er_view_create( le_void_t ) {

        /* created structure variables */
        er_view_t er_view = ER_VIEW_C;

        /* return created structure */
        return( er_view );

    }

    le_void_t er_view_delete( er_view_t * const er_view ) {

        /* deleted structure variables */
        er_view_t er_delete = ER_VIEW_C;

        /* delete structure */
        ( * er_view ) = er_delete;

    }

/*
    source - accessor methods
 */

    le_real_t er_view_get_lon( er_view_t const * const er_view ) {

        /* return view value */
        return( er_view->vw_lon );

    }

    le_real_t er_view_get_lat( er_view_t const * const er_view ) {

        /* return view value */
        return( er_view->vw_lat );

    }

    le_real_t er_view_get_alt( er_view_t const * const er_view ) {

        /* return view value */
        return( er_view->vw_alt );

    }

    le_real_t er_view_get_azm( er_view_t const * const er_view ) {

        /* return view value */
        return( er_view->vw_azm );

    }

    le_real_t er_view_get_gam( er_view_t const * const er_view ) {

        /* return view value */
        return( er_view->vw_gam );

    }

    le_void_t er_view_get_pose( er_view_t const * const er_view, le_real_t * const er_pose ) {

        /* assign position */
        er_pose[0] = er_view->vw_lon * ER_COMMON_D2R;
        er_pose[1] = er_view->vw_lat * ER_COMMON_D2R;
        er_pose[2] = er_view->vw_alt;

    }

    le_time_t er_view_get_time( er_view_t const * const er_view, le_enum_t const er_time ) {

        /* check asked time */
        if ( er_time == 0 ) {

            /* return asked time */
            return( er_view->vw_tia );

        } else {

            /* return asked time */
            return( er_view->vw_tib );

        }

    }

    le_address_t er_view_get_times( er_view_t const * const er_view ) {

        /* returned structure variables */
        le_address_t er_addr = LE_ADDRESS_C;

        /* assign address mode */
        le_address_set_mode( & er_addr, er_view->vw_mod );

        /* assign address times */
        le_address_set_time( & er_addr, 0, er_view->vw_tia );
        le_address_set_time( & er_addr, 1, er_view->vw_tib );

        /* return address structure */
        return( er_addr );

    }

    le_time_t er_view_get_area( er_view_t const * const er_view, le_enum_t const er_time ) {

        /* check asked zoom */
        if ( er_time == 0 ) {

            /* return asked zoom */
            return( er_view->vw_zta );

        } else {

            /* return asked zoom */
            return( er_view->vw_ztb );

        }

    }

    le_enum_t er_view_get_active( er_view_t const * const er_view ) {

        /* return active index */
        return( er_view->vw_act );

    }

    le_enum_t er_view_get_mode( er_view_t const * const er_view ) {

        /* return time mode */
        return( er_view->vw_mod );

    }

/*
    source - mutator methods
 */

    le_void_t er_view_set_plan( er_view_t * const er_view, le_real_t const er_xvalue, le_real_t const er_yvalue ) {

        /* azimuthal anagles variables */
        le_real_t er_cos = cos( er_view->vw_azm * ER_COMMON_D2R );
        le_real_t er_sin = sin( er_view->vw_azm * ER_COMMON_D2R );

        /* update planimetric coordinates */
        er_view->vw_lon += er_yvalue * er_sin - er_xvalue * er_cos;
        er_view->vw_lat += er_yvalue * er_cos + er_xvalue * er_sin;

    }

    le_void_t er_view_set_alt( er_view_t * const er_view, le_real_t const er_value ) {

        /* update altitude */
        er_view->vw_alt += er_value;

        /* clamp altitude value */
        if ( er_view->vw_alt < ER_COMMON_ALL ) er_view->vw_alt = ER_COMMON_ALL;
        if ( er_view->vw_alt > ER_COMMON_ALU ) er_view->vw_alt = ER_COMMON_ALU;

    }

    le_void_t er_view_set_azm( er_view_t * const er_view, le_real_t const er_value ) {

        /* update azimuth */
        er_view->vw_azm += er_value;

    }

    le_void_t er_view_set_gam( er_view_t * const er_view, le_real_t const er_value ) {

        /* update gamma */
        er_view->vw_gam += er_value;

        /* clamp gamma value */
        if ( er_view->vw_gam > + 120.0 ) er_view->vw_gam = + 120.0;
        if ( er_view->vw_gam < - 120.0 ) er_view->vw_gam = - 120.0;

    }

    le_void_t er_view_set_weight( er_view_t * const er_view, le_real_t const er_weight, er_view_t const * const er_value ) {

        /* weighted view addition */
        er_view->vw_lon += er_weight * er_value->vw_lon;
        er_view->vw_lat += er_weight * er_value->vw_lat;
        er_view->vw_alt += er_weight * er_value->vw_alt;
        er_view->vw_azm += er_weight * er_value->vw_azm;
        er_view->vw_gam += er_weight * er_value->vw_gam;

    }

    le_void_t er_view_set_devide( er_view_t * const er_view, le_real_t const er_value ) {

        /* devide view parameters */
        er_view->vw_lon /= er_value;
        er_view->vw_lat /= er_value;
        er_view->vw_alt /= er_value;
        er_view->vw_azm /= er_value;
        er_view->vw_gam /= er_value;

    }

    le_void_t er_view_set_time( er_view_t * const er_view, le_real_t const er_value ) {

        /* check active time */
        if ( er_view->vw_act == 0 ) {

            /* update time value */
            er_view->vw_tia += ( le_real_t ) er_view->vw_zta * er_value;

        } else {

            /* update time value */
            er_view->vw_tib += ( le_real_t ) er_view->vw_ztb * er_value;

        }

    }

    le_void_t er_view_set_area( er_view_t * const er_view, le_real_t const er_value ) {

        /* check active time */
        if ( er_view->vw_act == 0 ) {

            /* update zoom value */
            er_view->vw_zta *= er_value;

        } else {

            /* update zoom value */
            er_view->vw_ztb *= er_value;

        }

    }

    le_void_t er_view_set_swap( er_view_t * const er_view ) {

        /* swap active time */
        er_view->vw_act = 1 - er_view->vw_act;

    }

    le_void_t er_view_set_mode( er_view_t * const er_view, le_enum_t const er_mode ) {

        /* update mode */
        er_view->vw_mod = er_mode;

    }

