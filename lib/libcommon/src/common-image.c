/*
 *  eratosthene-suite - geodetic system
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

    # include "common-image.h"

/*
    source - i/o function
 */

    int lc_image_png_write( char const * const lc_path, int const lc_width, int const lc_height, int const lc_depth, unsigned char * const lc_bytes ) {

        /* Stream variables */
        FILE * lc_stream = NULL;

        /* Components variables */
        int lc_component = 0;
        int lc_colortype = 0;

        /* Portable network graphics variables */
        png_structp lc_pngs = NULL;
        png_infop   lc_info = NULL;

        /* Check image depth */
        if ( lc_depth == LC_IMAGE_RGB ) {

            /* Assign png color type */
            lc_component = 3;
            lc_colortype = PNG_COLOR_TYPE_RGB;

        } else if ( lc_depth == LC_IMAGE_RGBA ) {

            /* Assign png color type */
            lc_component = 4;
            lc_colortype = PNG_COLOR_TYPE_RGBA;

        } else {

            /* Send message */
            return( LC_FALSE );

        }

        /* Create output stream */
        if ( ( lc_stream = fopen( ( char * ) lc_path, "wb" ) ) == NULL ) {

            /* Send message */
            return( LC_FALSE );

        }

        /* Create portable network graphics structure */
        if ( ( lc_pngs = png_create_write_struct( PNG_LIBPNG_VER_STRING, NULL, NULL, NULL ) ) == NULL ) {

            /* Delete output stream */
            fclose( lc_stream );

            /* Send message */
            return( LC_FALSE );

        }

        /* Create portable network graphics info */
        if ( ( lc_info = png_create_info_struct( lc_pngs ) ) == NULL ) {

            /* Delete portable network graphics structure */
            png_destroy_write_struct( & lc_pngs, ( png_infopp ) NULL );

            /* Delete output stream */
            fclose( lc_stream );

            /* Send message */
            return( LC_FALSE );

        }

        /* Setup exception management */
        if ( setjmp( png_jmpbuf( lc_pngs ) ) ) {

            /* Create portable network graphics info */
            png_free_data( lc_pngs, lc_info, PNG_FREE_ALL, -1 );

            /* Delete portable network graphics structure */
            png_destroy_write_struct( & lc_pngs, ( png_infopp ) NULL );

            /* Delete output stream */
            fclose( lc_stream );

            /* Send message */
            return( LC_FALSE );

        }

        /* Bind output stream */
        png_init_io( lc_pngs, lc_stream );

        /* Setup portable network format */
        png_set_IHDR( lc_pngs, lc_info, lc_width, lc_height, 8, lc_colortype, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

        /* Write portable network graphics header */
        png_write_info( lc_pngs, lc_info );

        /* Export portable network graphics rows */
        for ( int lc_parse = 0; lc_parse < lc_height; lc_parse ++ ) png_write_row( lc_pngs, lc_bytes + ( lc_height - lc_parse - 1 ) * ( lc_width * lc_component ) );

        /* Write portable network graphics footer */
        png_write_end( lc_pngs, NULL );

        /* Create portable network graphics info */
        png_free_data( lc_pngs, lc_info, PNG_FREE_ALL, -1 );

        /* Delete portable network graphics structure */
        png_destroy_write_struct( & lc_pngs, ( png_infopp ) NULL );

        /* Delete output stream */
        fclose( lc_stream );

        /* Send message */
        return( LC_TRUE );

    }

