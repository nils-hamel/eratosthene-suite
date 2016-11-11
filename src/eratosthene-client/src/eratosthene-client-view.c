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

