/*
 *  eratosthene-suite - client
 *
 *      Nils Hamel - nils.hamel@alumni.epfl.ch
 *      Copyright (c) 2016-2020 DHLAB, EPFL
 *      Copyright (c) 2020 Republic and Canton of Geneva
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

    le_enum_t er_model_get_sync( er_model_t const * const er_model ) {

        /* return synchronisation flag */
        return( er_model->md_sync );

    }

/*
    source - mutator methods
 */

    le_void_t er_model_set_next( er_model_t * const er_model ) {

        /* search available d-cell */
        while ( ( ++ er_model->md_free ) < er_model->md_size ) {

            /* check cell state */
            if ( er_cell_get_flag( er_model->md_cell + er_model->md_free, ER_CELL_SYN ) != ER_CELL_SYN ) {

                /* d-cell found */
                return;

            }

        }

    }

    le_void_t er_model_set_prep( er_model_t * const er_model ) {

        /* reset synchronisation index */
        er_model->md_syna = +0;

        /* reset synchronisation index */
        er_model->md_synb = +0;

        /* reset synchronisation index */
        er_model->md_push = +0;

        /* reset synchronisation index */
        er_model->md_free = -1;

        /* reset synchronisation flag */
        er_model->md_sync = _LE_FALSE;

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

        /* depth limit condition */
        le_byte_t er_limit = _LE_FALSE;

        /* scale digit enumeration */
        for ( le_size_t er_digit = 0; er_digit < er_base; er_digit ++ ) {

            /* update address size */
            le_address_set_size( er_enum, er_scale + 1 );

            /* assign address digit */
            le_address_set_digit( er_enum, er_scale, er_digit );

            /* enumeration constraint */
            if ( er_scale >= ER_COMMON_ENUM ) {

                /* compute and check distance */
                if ( ( er_line = er_geodesy_distance( er_enum, er_view ) ) < er_geodesy_face( er_view_get_alt( er_view ) ) ) {

                    /* compute depth limit condition */
                    if ( ( er_scale + 3 + er_view_get_span( er_view ) ) < er_model->md_scfg ) {

                        /* update depth limit condition */
                        er_limit = _LE_FALSE;

                    } else {

                        /* update depth limit condition */
                        er_limit = _LE_TRUE;

                    }

                    /* check selection criterion */
                    if ( ( er_geodesy_select( er_line, er_view, er_model->md_scfg, er_scale ) == _LE_TRUE ) || ( er_limit == _LE_TRUE ) ) {

                        /* check target size - fail-safe */
                        if ( er_model->md_push < er_model->md_size ) {

                            /* push address to v-cell array */
                            er_cell_set_push( er_model->md_virt + ( er_model->md_push ++ ), er_enum );

                            /* search for deepest scale */
                            if ( er_scale >= er_model->md_synb ) {

                                /* update deepest scale */
                                er_model->md_synb = er_scale + 1; /* devnote : to check */

                            }

                        }

                    } else {

                        /* continue enumeration */
                        er_model_set_enum( er_model, er_enum, er_scale + 1, er_view );

                    }

                }

            } else {

                /* continue enumeration */
                er_model_set_enum( er_model, er_enum, er_scale + 1, er_view );

            }

        }

    }

    le_void_t er_model_set_fast( er_model_t * const er_model ) {

        /* parsing variables */
        le_size_t er_parse = 0;

        /* parsing variables */
        le_size_t er_index = 0;

        /* parsing d-cells array */
        while ( er_parse < er_model->md_size ) {

            /* check d-cell state */
            if ( er_cell_get_flag( er_model->md_cell + er_parse, ER_CELL_DIS ) == ER_CELL_DIS ) {

                /* reset index */
                er_index = 0;

                /* parsing v-cells array */
                while ( er_index < er_model->md_push ) {

                    /* address identity detection */
                    if ( er_cell_get_equal( er_model->md_cell + er_parse, er_model->md_virt + er_index ) == _LE_TRUE ) {

                        /* update d-cell state */
                        er_cell_set_flag( er_model->md_cell + er_parse, ER_CELL_SYN );

                        /* update v-cell state */
                        er_cell_set_flag( er_model->md_virt + er_index, ER_CELL_SYN );

                        /* resume detection */
                        er_index = er_model->md_push;

                    /* update index */
                    } else { er_index ++; }

                }

            }

            /* update index */
            er_parse ++;

        }

    }

    le_void_t er_model_set_sync( er_model_t * const er_model ) {

        /* address serialisation */
        le_size_t er_serial = 0;

        /* jump value */
        le_size_t er_jump = 0;

        /* reset address pack array */
        le_array_set_size( & er_model->md_addr, ER_COMMON_PACK );

        /* packing address */
        while ( ( er_serial < ER_COMMON_PACK ) && ( er_model->md_sync == _LE_FALSE ) ) {

            /* scale-based v-cell selection */
            if ( er_cell_get_size( er_model->md_virt + er_model->md_syna ) == er_model->md_synb ) {

                /* select un-synchronised v-cell */
                if ( er_cell_get_flag( er_model->md_virt + er_model->md_syna, ER_CELL_SYN ) != ER_CELL_SYN ) {

                    /* serialise cell address */
                    er_serial = er_cell_get_sync( er_model->md_virt + er_model->md_syna, & er_model->md_addr, er_serial );

                }

            }

            /* update synchronisation index */
            if ( ( ++ er_model->md_syna ) == er_model->md_push ) {

                /* d-cell stack tail management */
                er_model_set_hide( er_model );

                /* update synchronisation index */
                if ( ( -- er_model->md_synb ) >= ER_COMMON_ENUM ) {

                    /* reset synchronisation index */
                    er_model->md_syna = 0;

                } else {

                    /* update synchronisation flag */
                    er_model->md_sync = _LE_TRUE;

                }

            }

        }

        /* incomplete address pack manegement */
        if ( er_serial < ER_COMMON_PACK ) {

            /* resize address pack array */
            le_array_set_size( & er_model->md_addr, er_serial );

        }

        /* write socket-array on socket */
        if ( le_array_io_write( & er_model->md_addr, LE_MODE_QUER, er_model->md_sock ) == LE_MODE_QUER ) {

            /* parsing address pack */
            for ( le_size_t er_parse = 0; er_parse < er_serial; er_parse += LE_ARRAY_ADDR ) {

                /* read jump information array */
                if ( le_array_io_read( & er_model->md_jump, er_model->md_sock ) == LE_MODE_JUMP ) {

                    /* serialise jump value */
                    le_array_serial( & er_model->md_jump, & er_jump, sizeof( le_size_t ), 0, _LE_GET );

                    /* apply jump value */
                    if ( ( er_parse += ( er_jump * LE_ARRAY_ADDR ) ) < er_serial ) {

                        /* search available d-cell */
                        er_model_set_next( er_model );

                        /* update d-cell state */
                        er_cell_set_zero( er_model->md_cell + er_model->md_free, ER_CELL_DIS );

                        /* read d-cell data */
                        if ( er_cell_set_read( er_model->md_cell + er_model->md_free, er_model->md_sock ) == LE_ERROR_SUCCESS ) {

                            /* process d-cell */
                            if ( er_cell_set_data( er_model->md_cell + er_model->md_free ) != 0 ) {

                                /* synchronise d-cell address */
                                er_cell_set_sync( er_model->md_cell + er_model->md_free, & er_model->md_addr, er_parse );

                                /* update d-cell state */
                                er_cell_set_flag( er_model->md_cell + er_model->md_free, ER_CELL_SYN | ER_CELL_DIS );

                            }

                        } else {

                        }

                    }

                } else { 

                }

            }

        }

    }

    le_void_t er_model_set_hide( er_model_t * const er_model ) {

        /* parsing d-cell array tail */
        for ( le_size_t er_parse = er_model->md_free + 1; er_parse < er_model->md_size; er_parse ++ ) {

            /* scale-based d-cell selection */
            if ( er_cell_get_size( er_model->md_cell + er_parse ) >= er_model->md_synb ) {

                /* check d-cell state */
                if ( er_cell_get_flag( er_model->md_cell + er_parse, ER_CELL_SYN | ER_CELL_DIS ) == ER_CELL_DIS ) {

                    /* update d-cell state */
                    er_cell_set_zero( er_model->md_cell + er_parse, ER_CELL_DIS );

                }

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
        le_real_t er_cosl = cos( - er_lon * LE_D2R );
        le_real_t er_sinl = sin( - er_lon * LE_D2R );
        le_real_t er_cosa = cos( + er_lat * LE_D2R );
        le_real_t er_sina = sin( + er_lat * LE_D2R );

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
                    er_cell_get_render( er_model->md_cell + er_parse, er_lon, er_lat, er_cosl, er_sinl, er_cosa, er_sina );

                /* d-cell matrix */
                glPopMatrix();

            }

        }

    }

    le_void_t er_model_display_earth( er_view_t const * const er_view ) {

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
        glDisable( GL_BLEND );

        /* delete quadric */
        gluDeleteQuadric( er_earth );

    }

/*
    source - research methods (temporary)
 */

    le_void_t er_model_research( er_model_t const * const er_model ) {

        /* string buffer */
        le_char_t er_string[1024] = { 0 };

        /* parsing cell array */
        for ( le_size_t er_parse = 0; er_parse < er_model->md_size; er_parse ++ ) {

            /* check d-cell flag */
            if ( er_cell_get_flag( er_model->md_cell + er_parse, ER_CELL_DIS ) == ER_CELL_DIS ) {

                /* reset string */
                memset( er_string, 0x00, 1024 );

                /* convert address to string */
                le_address_ct_string( & ( er_model->md_cell + er_parse )->ce_addr, er_string );

                /* display address */
                fprintf( stderr, "%s\n", er_string );

            }

        }

    }

