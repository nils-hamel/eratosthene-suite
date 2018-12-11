/*
 *  eratosthene-suite - client
 *
 *      Nils Hamel - nils.hamel@bluewin.ch
 *      Copyright (c) 2016-2018 DHLAB, EPFL
 *
 *  Contributors
 *
 *      Ciprian Tomoiaga - ciprian.tomoiaga@gmail.com
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

    # include "eratosthene-client-model.h"

/*
    source - constructor/destructor methods
 */

    er_model_t er_model_create( le_sock_t const er_socket, le_size_t const er_scfg, le_time_t const er_tcfg ) {

        /* created structure variables */
        er_model_t er_model = ER_MODEL_I( er_socket, er_scfg, er_tcfg );

        /* array size variables */
        le_size_t er_size = er_model.md_size << 1;

        /* create cell arrays */
        if ( ( er_model.md_cell = ( er_cell_t * ) malloc( er_size * sizeof( er_cell_t ) ) ) == NULL ) {

            /* return created structure */
            return( er_model );

        }

        /* assign target stack pointer */
        er_model.md_virt = er_model.md_cell + er_model.md_size;

        /* initialise arrays cells */
        for ( le_size_t er_parse = 0; er_parse < er_size; er_parse ++ ) {

            /* initialise cell */
            er_model.md_cell[er_parse] = er_cell_create();

        }

        /* return created structure */
        return( le_set_status( er_model, _LE_TRUE ) );

    }

    le_void_t er_model_delete( er_model_t * const er_model ) {

        /* deleted structure variables */
        er_model_t er_delete = ER_MODEL_C;

        /* check cell array */
        if ( er_model->md_cell != NULL ) {

            /* update array size */
            er_model->md_size <<= 1;

            /* delete arrays cells */
            for ( le_size_t er_parse = 0; er_parse < er_model->md_size; er_parse ++ ) {

                /* delete cell */
                er_cell_delete( er_model->md_cell + er_parse );

            }

            /* release array memory */
            free( er_model->md_cell );

        }

        /* delete structure */
        ( * er_model ) = er_delete;

    }

/*
    source - accessor methods
 */

    le_size_t er_model_get_drop( er_model_t const * const er_model, le_address_t const * const er_addr ) {

        /* parsing variable */
        le_size_t er_parse = 0;

        /* parsing d-cell array */
        while ( er_parse < er_model->md_size ) {

            /* compare to cell address */
            if ( er_cell_get_drop( er_model->md_cell + er_parse, er_addr ) == _LE_TRUE ) {

                /* return index */
                return( er_parse );

            /* update index */
            } else { er_parse ++; }

        }

        /* return index */
        return( er_parse );

    }

/*
    source - mutator methods
 */

    le_void_t er_model_set_prep( er_model_t * const er_model ) {

        /* reset synchronisation index */
        er_model->md_syna = 0;

        /* reset synchronisation index */
        er_model->md_synb = 0;

        /* reset synchronisation index */
        er_model->md_free = 0;

        /* reset synchronisation index */
        er_model->md_push = 0;

        /* reset cells state */
        for ( le_size_t er_parse = 0; er_parse < er_model->md_size; er_parse ++ ) {

            /* reset d-cell state */
            er_cell_set_zero( er_model->md_cell + er_parse, ER_CELL_SYN );

            /* reset v-cell state */
            er_cell_set_zero( er_model->md_virt + er_parse, ER_CELL_SYN );

        }

    }

    le_void_t er_model_set_enum( er_model_t * const er_model, le_address_t * const er_enum, le_size_t const er_scale, er_view_t const * const er_view ) {

        /* scale base variables */
        le_size_t er_base = le_address_base( er_scale );

        /* distance variables */
        le_real_t er_line = 0.0;

        /* push condition variable */
        le_size_t er_index = 0;

        /* scale digit enumeration */
        for ( le_size_t er_digit = 0; er_digit < er_base; er_digit ++ ) {

            /* update address size */
            le_address_set_size( er_enum, er_scale + 1 );

            /* assign address digit */
            le_address_set_digit( er_enum, er_scale, er_digit );

            /* enumeration constraint */
            if ( er_scale < ER_COMMON_ENUM ) {

                /* continue enumeration */
                er_model_set_enum( er_model, er_enum, er_scale + 1, er_view );

            } else {

                /* compute and check distance */
                if ( ( er_line = er_geodesy_distance( er_enum, er_view ) ) < er_geodesy_face( er_view_get_alt( er_view ) ) ) {

                    /* check selection criterion */
                    if ( er_geodesy_select( er_line, er_view, er_model->md_scfg, er_scale ) == _LE_TRUE ) {

                        /* check target size - failsafe */
                        if ( er_model->md_push < er_model->md_size ) {

                            // DEBUG : drop un-necessary (performed by the _fast process) //

                            /* check push condition */
                            //if ( ( er_index = er_model_get_drop( er_model, er_enum ) ) == er_model->md_size ) {

                                /* push address to v-cell array */
                                er_cell_set_push( er_model->md_virt + er_model->md_push, er_enum );

                                /* search for deepest scale */
                                if ( er_scale >= er_model->md_synb ) {

                                    /* update deepest scale */
                                    er_model->md_synb = er_scale + 1;

                                }

                                /* update synchronisation index */
                                er_model->md_push ++;

                            //} else {

                                /* update d-cell state */
                            //    er_cell_set_flag( er_model->md_cell + er_index, ER_CELL_SYN | ER_CELL_DIS );

                                // possible missing flag on virtual stack (ER_CELL_SYN) //

                            //    er_cell_set_flag( er_model->md_virt + er_model->md_push, ER_CELL_SYN );

                            //}

                        }

                    } else {

                        /* check enumeration boundary */
                        if ( ( er_scale + 2 + er_view_get_span( er_view ) ) < er_model->md_scfg ) {

                            /* continue enumeration */
                            er_model_set_enum( er_model, er_enum, er_scale + 1, er_view );

                        }

                    }

                }

            }

        }

    }

    le_void_t er_model_set_fast( er_model_t * const er_model ) {

        /* parsing variables */
        le_size_t er_index = 0;

        /* parsing v-cells array */
        for ( le_size_t er_parse = 0; er_parse < er_model->md_push; er_parse ++ ) {

            /* reset parsing index */
            er_index = 0;

            /* parsing d-cells array */
            while ( er_index < er_model->md_size ) {

                // DEBUG  : possible missing condition on display //

                if ( er_cell_get_flag( er_model->md_cell + er_index, ER_CELL_DIS ) == ER_CELL_DIS ) {

                /* detect cell identity */
                if ( er_cell_get_equal( er_model->md_virt + er_parse, er_model->md_cell + er_index ) == _LE_TRUE ) {

                    /* update v-cell flag */
                    er_cell_set_flag( er_model->md_virt + er_parse, ER_CELL_SYN );

                    /* update d-cell flag */
                    er_cell_set_flag( er_model->md_cell + er_index, ER_CELL_SYN | ER_CELL_DIS );

                    /* resume search */
                    er_index = er_model->md_size;

                /* update index */
                } else { er_index ++; }

                } else { er_index ++; }

            }

        }

    }

    le_enum_t er_model_set_sync( er_model_t * const er_model ) {

        /* package size variable */
        le_size_t er_serial = 0;

        /* segment parsing variable */
        le_size_t er_parse = 0;

        /* check synchronisation state */
        if ( er_model->md_synb < ER_COMMON_ENUM ) {

            /* send message */
            return( _LE_TRUE );

        }

        /* update socket-array size */
        le_array_set_size( & er_model->md_addr, 0 );

        /* compose address pack */
        if ( ( er_serial = er_model_set_sync_pack( er_model ) ) > 0 ) {

            /* write socket-array on socket */
            le_array_io_write( & er_model->md_addr, LE_MODE_QUER, er_model->md_sock );

            /* parsing v-cell array segment */
            while ( er_parse < er_serial ) {

                /* search usable d-cell */
                while ( er_cell_get_flag( er_model->md_cell + er_model->md_free, ER_CELL_SYN ) == ER_CELL_SYN ) {

                    /* update index */
                    er_model->md_free ++;

                }

                /* update d-cell state */
                er_cell_set_zero( er_model->md_cell + er_model->md_free, ER_CELL_DIS );

                /* read socket-array */
                er_cell_set_array( er_model->md_cell + er_model->md_free, er_model->md_sock );

                /* sychronise cell address */
                er_parse = er_cell_set_sync( er_model->md_cell + er_model->md_free, & er_model->md_addr, er_parse );

                /* process d-cell */
                if ( er_cell_set_data( er_model->md_cell + er_model->md_free ) != 0 ) {

                    /* update d-cell state */
                    er_cell_set_flag( er_model->md_cell + er_model->md_free, ER_CELL_SYN | ER_CELL_DIS );

                }

            }

        }

        /* check synchronisation state */
        if ( er_model->md_synb < ER_COMMON_ENUM ) {

            /* processs d-cell array tail */
            er_model_set_sync_tail( er_model );

            /* send message */
            return( _LE_TRUE );

        /* send message */
        } else { return( _LE_FALSE ); }

    }

    le_size_t er_model_set_sync_pack( er_model_t * const er_model ) {

        /* returned value variable */
        le_size_t er_serial = 0;

        /* parsing v-cell array segment */
        while ( ( er_model->md_synb >= ER_COMMON_ENUM ) && ( er_serial < LE_ARRAY_ADDR * ER_COMMON_PACK ) ) {

            /* select unsynchronised v-cell */
            if ( er_cell_get_flag( er_model->md_virt + er_model->md_syna, ER_CELL_SYN ) != ER_CELL_SYN ) {

                /* scale-based v-cell selection */
                if ( er_cell_get_size( er_model->md_virt + er_model->md_syna ) == er_model->md_synb ) {

                    /* update socket-array size */
                    le_array_set( & er_model->md_addr, LE_ARRAY_ADDR );

                    /* serialise address */
                    er_serial = er_cell_get_sync( er_model->md_virt + er_model->md_syna, & er_model->md_addr, er_serial );

                    /* update v-cell state */
                    er_cell_set_flag( er_model->md_virt + er_model->md_syna, ER_CELL_SYN );

                }

            }

            /* update synchronisation index */
            if ( ( ++ er_model->md_syna ) == er_model->md_push ) {

                /* update synchronisation index */
                er_model->md_synb --;

                /* reset synchronisation index */
                er_model->md_syna = 0;

            }

        }

        /* retrun offset */
        return( er_serial );

    }

    le_void_t er_model_set_sync_tail( er_model_t * const er_model ) {

        // DEBUG : possible remaining cell in the first segment of the actual stack //

        /* parsing d-cell array tail */
        for ( le_size_t er_parse = er_model->md_free + 1; er_parse < er_model->md_size; er_parse ++ ) {

            /* check d-cell state */
            if ( er_cell_get_flag( er_model->md_cell + er_parse, ER_CELL_SYN | ER_CELL_DIS ) == ER_CELL_DIS ) {

                /* update d-cell state */
                er_cell_set_zero( er_model->md_cell + er_parse, ER_CELL_DIS );

            }

        }

    }

/*
    source - display methods
 */

    le_void_t er_model_display_cell( er_model_t const * const er_model, er_view_t const * const er_view ) {

        /* angle variable */
        le_real_t er_lon = er_view_get_lon( er_view );
        le_real_t er_lat = er_view_get_lat( er_view );

        /* trigonometric variable */
        le_real_t er_cl = cos( - er_lon * LE_D2R );
        le_real_t er_sl = sin( - er_lon * LE_D2R );
        le_real_t er_ca = cos( + er_lat * LE_D2R );
        le_real_t er_sa = sin( + er_lat * LE_D2R );

        /* motion management - tilt rotation */
        glRotated( - er_view_get_gam( er_view ), 1.0, 0.0, 0.0 );

        /* motion management - altimetric translation */
        glTranslated( 0.0, 0.0, - er_view_get_alt( er_view ) + LE_ADDRESS_WGS_A );

        /* motion management - azimuth rotation */
        glRotated( + er_view_get_azm( er_view ), 0.0, 0.0, 1.0 );

        /* parsing d-cells array */
        for ( le_size_t er_parse = 0; er_parse < er_model->md_size; er_parse ++ ) {

            /* check d-cell flag */
            if ( er_cell_get_flag( er_model->md_cell + er_parse, ER_CELL_DIS ) == ER_CELL_DIS ) {

                /* d-cell matrix */
                glPushMatrix();

                    /* cell rendering */
                    er_cell_get_render( er_model->md_cell + er_parse, er_lon, er_lat, er_cl, er_sl, er_ca, er_sa );

                /* d-cell matrix */
                glPopMatrix();

                // DEBUG //
                glPushMatrix();

                    // DEBUG : cell edges rendering //
                    er_cell_get_shell( er_model->md_cell + er_parse, er_lon, er_lat );

                // DEBUG //
                glPopMatrix();

            }

        }

    }

    le_void_t er_model_display_earth( er_view_t const * const er_view ) { /* dev-note : weak link with model module */

        /* tilt rotation */
        glRotated( - er_view_get_gam( er_view ), 1.0, 0.0, 0.0 );

        /* altimetric translation */
        glTranslated( 0.0, 0.0, - er_view_get_alt( er_view ) + LE_ADDRESS_WGS_A );

        /* azimuth rotation */
        glRotated( + er_view_get_azm( er_view ), 0.0, 0.0, 1.0 );

        /* earth centering */
        glTranslated( 0.0, 0.0, - LE_ADDRESS_WGS_A );

        /* planimetric rotation */
        glRotated( + er_view_get_lat( er_view ), 1.0, 0.0, 0.0 );
        glRotated( - er_view_get_lon( er_view ), 0.0, 1.0, 0.0 );

        /* earth orientation */
        glRotated( 90.0, 1.0, 0.0, 0.0 );

        /* create quadric */
        GLUquadricObj * er_earth = gluNewQuadric();

        /* configure quadric */
        gluQuadricDrawStyle( er_earth, GLU_LINE );

        /* enable blending */
        glEnable( GL_BLEND );

        /* earth color */
        glColor4f( 0.6, 0.6, 0.6, 0.09 );

        /* display quadric */
        gluSphere( er_earth, LE_ADDRESS_WGS_A, 360, 180 );

        /* disable blending */
        glEnable( GL_BLEND );

        /* delete quadric */
        gluDeleteQuadric( er_earth );

    }

