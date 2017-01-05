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

    le_enum_t er_view_get_equal( er_view_t const * const er_viewa, er_view_t const * const er_viewb ) {

        /* check spatial components - return answer */
        if ( er_viewa->vw_lon != er_viewb->vw_lon ) return( _LE_FALSE );
        if ( er_viewa->vw_lat != er_viewb->vw_lat ) return( _LE_FALSE );
        if ( er_viewa->vw_alt != er_viewb->vw_alt ) return( _LE_FALSE );

        /* check temporal components - return answer */
        if ( er_viewa->vw_tia != er_viewb->vw_tia ) return( _LE_FALSE );
        if ( er_viewa->vw_tib != er_viewb->vw_tib ) return( _LE_FALSE );

        /* check mode component - return answer */
        if ( er_viewa->vw_mod != er_viewb->vw_mod ) return( _LE_FALSE );

        /* return answer */
        return( _LE_TRUE );

    }

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

    le_enum_t er_view_get_mode( er_view_t const * const er_view ) {

        /* return time mode */
        return( er_view->vw_mod );

    }


    le_enum_t er_view_get_active( er_view_t const * const er_view ) {

        /* return active index */
        return( er_view->vw_act );

    }

    le_address_t er_view_get_times( er_view_t const * const er_view ) {

        /* returned structure variables */
        le_address_t er_addr = LE_ADDRESS_C;

        /* assign address mode */
        le_address_set_mode( & er_addr, er_view->vw_mod );

        /* set address span */
        le_address_set_span( & er_addr, ER_COMMON_SPAN );

        /* check mode */
        if ( er_view->vw_mod != 2 ) {

            /* assign address times */
            le_address_set_time( & er_addr, 0, er_view->vw_tia );

        }

        /* check mode */
        if ( er_view->vw_mod != 1 ) {

            /* assign address time */
            le_address_set_time( & er_addr, 1, er_view->vw_tib );

        }

        /* return address structure */
        return( er_addr );

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

    er_view_t er_view_get_inter( er_view_t const * const er_views, le_size_t const er_size, le_size_t const er_index, le_real_t const er_param ) {

        /* returned value variables */
        er_view_t er_view = ER_VIEW_C;

        /* interpolation variables */
        le_real_t er_value = 0.0;

        /* accumulation variables */
        le_real_t er_accum = 0.0;

        /* parsing control points */
        for ( le_size_t er_parse = 0; er_parse < er_size; er_parse ++ ) {

            /* compute interpolation parameter */
            er_value = ( ( le_real_t ) er_index + er_param ) - ( le_real_t ) er_parse;

            /* compute interpolation weight - accumulate interpolation weight */
            er_accum += ( er_value = exp( - 3.0 * er_value * er_value ) );

            /* update interpolated values */
            er_view.vw_lon += er_value * ( er_views + er_parse )->vw_lon;
            er_view.vw_lat += er_value * ( er_views + er_parse )->vw_lat;
            er_view.vw_alt += er_value * ( er_views + er_parse )->vw_alt;
            er_view.vw_azm += er_value * ( er_views + er_parse )->vw_azm;
            er_view.vw_gam += er_value * ( er_views + er_parse )->vw_gam;

        }

        /* compute interpolated values - simple smooth interpolation */
        er_view.vw_lon /= er_accum;
        er_view.vw_lat /= er_accum;
        er_view.vw_alt /= er_accum;
        er_view.vw_azm /= er_accum;
        er_view.vw_gam /= er_accum;

        /* compute interpolated values - step interpolation */
        er_view.vw_mod = ( er_views + er_index )->vw_mod;
        er_view.vw_act = ( er_views + er_index )->vw_act;

        /* compute interpolated values - linear interpolation */
        er_view.vw_tia = er_param * ( er_views + er_index + 1 )->vw_tia + ( 1.0 - er_param ) * ( er_views + er_index )->vw_tia;
        er_view.vw_tib = er_param * ( er_views + er_index + 1 )->vw_tib + ( 1.0 - er_param ) * ( er_views + er_index )->vw_tib;
        er_view.vw_zta = er_param * ( er_views + er_index + 1 )->vw_zta + ( 1.0 - er_param ) * ( er_views + er_index )->vw_zta;
        er_view.vw_ztb = er_param * ( er_views + er_index + 1 )->vw_ztb + ( 1.0 - er_param ) * ( er_views + er_index )->vw_ztb;

        /* return computed view */
        return( er_view );

    }

/*
    source - mutator methods
 */

    le_void_t er_view_set_plan( er_view_t * const er_view, le_real_t const er_xvalue, le_real_t const er_yvalue ) {

        /* azimuthal angles variables */
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

    le_void_t er_view_set_mode( er_view_t * const er_view, le_enum_t const er_mode ) {

        /* update mode */
        er_view->vw_mod = er_mode;

    }

    le_void_t er_view_set_swap( er_view_t * const er_view ) {

        /* swap active time */
        er_view->vw_act = 1 - er_view->vw_act;

    }

    le_void_t er_view_set_times( er_view_t * const er_view ) {

        /* align secondary time */
        er_view->vw_tib = er_view->vw_tia;

        /* align secondary area */
        er_view->vw_ztb = er_view->vw_zta;

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

            /* update area value */
            er_view->vw_zta *= er_value;

            /* clamp area value */
            er_view->vw_zta = lc_clamp( er_view->vw_zta, 60, 32314982400 );

        } else {

            /* update area value */
            er_view->vw_ztb *= er_value;

            /* clamp area value */
            er_view->vw_ztb = lc_clamp( er_view->vw_ztb, 60, 32314982400 );

        }

    }

