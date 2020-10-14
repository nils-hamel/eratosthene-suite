/*
 *  eratosthene-suite - client
 *
 *      Nils Hamel - nils.hamel@bluewin.ch
 *      Copyright (c) 2016-2020 DHLAB, EPFL
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

        /* check comb components - return answer */
        if ( er_viewa->vw_cmb != er_viewb->vw_cmb ) return( _LE_FALSE );

        /* check mode component - return answer */
        if ( er_viewa->vw_mod != er_viewb->vw_mod ) return( _LE_FALSE );

        /* check mode component - return answer */
        if ( er_viewa->vw_qry != er_viewb->vw_qry ) return( _LE_FALSE );

        /* check span component - return answer */
        if ( er_viewa->vw_spn != er_viewb->vw_spn ) return( _LE_FALSE );

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
        er_pose[0] = er_view->vw_lon * LE_D2R;
        er_pose[1] = er_view->vw_lat * LE_D2R;
        er_pose[2] = er_view->vw_alt;

    }

    le_real_t er_view_get_inertia( er_view_t const * const er_view, le_enum_t const er_modifier ) {

        /* inertia variable */
        le_real_t er_inertia = fabs( er_view->vw_alt - LE_ADDRESS_WGS_A ) * ER_COMMON_INE;

        /* range clamping */
        er_inertia = er_inertia < 5.0 ? 5.0 : er_inertia;

        /* check modifier */
        if ( er_modifier & ER_COMMON_KMCTL ) {

            /* modify inertia */
            er_inertia *=  ER_COMMON_IMU;

        } else if ( er_modifier & ER_COMMON_KMSHF ) {

            /* modify inertia */
            er_inertia *= ER_COMMON_IML;

        }

        /* return inertia */
        return( er_inertia );

    }

    le_enum_t er_view_get_mode( er_view_t const * const er_view ) {

        /* return convolution mode */
        return( er_view->vw_mod );

    }

    le_enum_t er_view_get_query( er_view_t const * const er_view ) {

        /* return query mode */
        return( er_view->vw_qry );

    }

    le_address_t er_view_get_times( er_view_t const * const er_view ) {

        /* returned structure variables */
        le_address_t er_addr = LE_ADDRESS_C;

        /* assign address convolution mode */
        le_address_set_mode( & er_addr, er_view->vw_mod );

        /* assign address query mode */
        le_address_set_query( & er_addr, er_view->vw_qry );

        /* set address span */
        le_address_set_span( & er_addr, er_view->vw_spn );

        /* check mode */
        //if ( er_view->vw_mod != 2 ) {

            /* assign address times */
            le_address_set_time( & er_addr, 0, er_view->vw_tia );

        //}

        /* check mode */
        //if ( er_view->vw_mod != 1 ) {

            /* assign address time */
            le_address_set_time( & er_addr, 1, er_view->vw_tib );

        //}

        /* set address comb */
        le_address_set_time( & er_addr, 2, er_view->vw_cmb );

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

    le_time_t er_view_get_comb( er_view_t const * const er_view ) {

        /* return view comb */
        return( er_view->vw_cmb );

    }

    le_size_t er_view_get_span( er_view_t const * const er_view ) {

        /* return view span */
        return( er_view->vw_spn );

    }

/*
    source - mutator methods
 */

    le_void_t er_view_set( er_view_t * const er_view, le_real_t const er_lon, le_real_t const er_lat, le_real_t const er_alt, le_real_t const er_azm, le_real_t const er_gam, le_time_t const er_tia, le_time_t const er_tib, le_time_t const er_cmb ) {

        /* assign spatial position */
        er_view->vw_lon = er_lon;
        er_view->vw_lat = er_lat;
        er_view->vw_alt = er_alt;
        er_view->vw_azm = er_azm;
        er_view->vw_gam = er_gam;

        /* assign time position */
        er_view->vw_tia = er_tia;
        er_view->vw_tib = er_tib;
        er_view->vw_cmb = er_cmb;

    }

    le_void_t er_view_set_plan( er_view_t * const er_view, le_real_t const er_xvalue, le_real_t const er_yvalue ) {

        /* azimuths angles variables */
        le_real_t er_cos = cos( er_view->vw_azm * LE_D2R );
        le_real_t er_sin = sin( er_view->vw_azm * LE_D2R );

        /* update planimetric coordinates */
        er_view->vw_lon += er_yvalue * er_sin - er_xvalue * er_cos;
        er_view->vw_lat += er_yvalue * er_cos + er_xvalue * er_sin;

    }

    le_void_t er_view_set_alt( er_view_t * const er_view, le_real_t const er_value ) {

        /* update altitude */
        er_view->vw_alt += er_value;

        /* clamp altitude value */
        er_view->vw_alt = lc_clamp( er_view->vw_alt, ER_COMMON_LALTI, ER_COMMON_UALTI );

    }

    le_void_t er_view_set_azm( er_view_t * const er_view, le_real_t const er_value ) {

        /* update azimuth */
        er_view->vw_azm += er_value;

        /* clamp azimuth value */
        er_view->vw_azm = lc_angle( er_view->vw_azm );

    }

    le_void_t er_view_set_gam( er_view_t * const er_view, le_real_t const er_value ) {

        /* update gamma */
        er_view->vw_gam += er_value;

        /* clamp gamma value */
        er_view->vw_gam = lc_clamp( er_view->vw_gam, 0.0, 140.0 );

    }

    le_void_t er_view_set_mode( er_view_t * const er_view, le_enum_t const er_mode ) {

        /* update convolution mode */
        er_view->vw_mod = er_mode;

    }

    le_void_t er_view_set_query( er_view_t * const er_view, le_enum_t const er_query ) {

        /* update query mode */
        er_view->vw_qry = er_query;

    }

    le_void_t er_view_set_times( er_view_t * const er_view ) {

        /* check mode */
        if ( er_view->vw_mod == 1 ) {

            /* align secondary time */
            er_view->vw_tib = er_view->vw_tia;

        } else if ( er_view->vw_mod == 2 ) {

            /* align secondary time */
            er_view->vw_tia = er_view->vw_tib;

        }

    }

    le_void_t er_view_set_time( er_view_t * const er_view, le_real_t const er_value ) {

        /* increment variable */
        le_real_t er_increment = ( le_real_t ) er_view->vw_cmb * er_value;

        /* check increment value */
        if ( fabs( er_increment ) < 1.0 ) {

            /* avoid position lock */
            er_increment = ( er_increment < 0.0 ) ? -1 : +1;

        }

        /* check mode */
        if ( er_view->vw_mod == 2 ) {

            /* update time value */
            er_view->vw_tib += er_increment;

            /* clamp time range */
            er_view->vw_tib = lc_clamp( er_view->vw_tib, -ER_COMMON_UTIME, +ER_COMMON_UTIME );

        } else {

            /* update time value */
            er_view->vw_tia += er_increment;

            /* clamp time range */
            er_view->vw_tia = lc_clamp( er_view->vw_tia, -ER_COMMON_UTIME, +ER_COMMON_UTIME );

        }

    }

    le_void_t er_view_set_swap_times( er_view_t * const er_view ) {

        /* push variable */
        le_time_t er_swap = er_view->vw_tib;

        /* swap time */
        er_view->vw_tib = er_view->vw_tia;

        /* pop time */
        er_view->vw_tia = er_swap;

    }

    le_void_t er_view_set_comb( er_view_t * const er_view, le_real_t er_value ) {

        /* update comb value */
        er_view->vw_cmb = lc_clamp( er_view->vw_cmb * er_value, ER_COMMON_LCOMB, ER_COMMON_UCOMB );

    }

    le_void_t er_view_set_span( er_view_t * const er_view, le_size_t const er_value ) {

        /* update and clamp span value */
        er_view->vw_spn = lc_clamp( er_view->vw_spn + er_value, ER_COMMON_LSPAN, ER_COMMON_USPAN );

    }

