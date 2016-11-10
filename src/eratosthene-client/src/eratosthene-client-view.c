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

/*
    source - mutator methods
 */

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
