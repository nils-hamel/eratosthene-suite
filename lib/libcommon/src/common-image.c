/*
 *  eratosthene-suite - common library
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

    # include "common-image.h"

/*
    source - portable network graphics i/o function
 */

    int lc_image_write( char const * const lc_path, int const lc_width, int const lc_height, unsigned char * const lc_bytes ) {

        /* portable network graphics variable */
        png_structp lc_pngs = NULL;
        png_infop   lc_info = NULL;

        /* stream variable */
        FILE * lc_stream = NULL;

        /* check consistency */
        if ( lc_bytes == NULL ) {

            /* send message */
            return( LC_FALSE );

        }

        /* create output stream */
        if ( ( lc_stream = fopen( ( char * ) lc_path, "wb" ) ) == NULL ) {

            /* send message */
            return( LC_FALSE );

        }

        /* create portable network graphics structure */
        if ( ( lc_pngs = png_create_write_struct( PNG_LIBPNG_VER_STRING, NULL, NULL, NULL ) ) == NULL ) {

            /* delete output stream */
            fclose( lc_stream );

            /* send message */
            return( LC_FALSE );

        }

        /* create portable network graphics info */
        if ( ( lc_info = png_create_info_struct( lc_pngs ) ) == NULL ) {

            /* delete portable network graphics structure */
            png_destroy_write_struct( & lc_pngs, ( png_infopp ) NULL );

            /* delete output stream */
            fclose( lc_stream );

            /* send message */
            return( LC_FALSE );

        }

        /* setup exception management */
        if ( setjmp( png_jmpbuf( lc_pngs ) ) ) {

            /* create portable network graphics info */
            png_free_data( lc_pngs, lc_info, PNG_FREE_ALL, -1 );

            /* delete portable network graphics structure */
            png_destroy_write_struct( & lc_pngs, ( png_infopp ) NULL );

            /* delete output stream */
            fclose( lc_stream );

            /* send message */
            return( LC_FALSE );

        }

        /* bind output stream */
        png_init_io( lc_pngs, lc_stream );

        /* setup portable network format */
        png_set_IHDR( lc_pngs, lc_info, lc_width, lc_height, 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

        /* write portable network graphics header */
        png_write_info( lc_pngs, lc_info );

        /* export portable network graphics pixels rows */
        for ( int lc_parse = 0; lc_parse < lc_height; lc_parse ++ ) {

            /* export current pixels row */
            png_write_row( lc_pngs, lc_bytes + ( lc_height - lc_parse - 1 ) * ( lc_width * 3 ) );

        }

        /* write portable network graphics footer */
        png_write_end( lc_pngs, NULL );

        /* create portable network graphics info */
        png_free_data( lc_pngs, lc_info, PNG_FREE_ALL, -1 );

        /* delete portable network graphics structure */
        png_destroy_write_struct( & lc_pngs, ( png_infopp ) NULL );

        /* delete output stream */
        fclose( lc_stream );

        /* send message */
        return( LC_TRUE );

    }

