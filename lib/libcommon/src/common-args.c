/*
 *  eratosthene-suite - geodetic system
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

    # include "common-args.h"

/*
    source - switches parsers
 */

    int lc_read_flag( int const argc, char ** argv, char const * const er_long, char const * const er_short ) {

        /* parsing arguments and parameters */
        for ( int er_parse = 0 ; er_parse < argc; er_parse ++ ) {

            /* check argument short and long forms */
            if ( ( strcmp( argv[er_parse], er_long ) == 0 ) || ( strcmp( argv[er_parse], er_short ) == 0 ) ) {

                /* return answer */
                return( LC_TRUE );

            }

        }

        /* return answer */
        return( LC_FALSE );

    }

/*
    source - arguments and parameters parsers
 */

    char * lc_read_string( int const argc, char ** argv, char const * const er_long, char const * const er_short ) {

        /* parsing arguments and parameters */
        for ( int er_parse = 0 ; er_parse < argc; er_parse ++ ) {

            /* check argument short and long forms */
            if ( ( strcmp( argv[er_parse], er_long ) == 0 ) || ( strcmp( argv[er_parse], er_short ) == 0 ) ) {

                /* check consistency */
                if ( ( ++ er_parse ) < argc ) {

                    /* return parameter */
                    return( argv[er_parse] );

                /* return default value */
                } else { return( NULL ); }

            }

        }

        /* return default value */
        return( NULL );

    }

    intmax_t lc_read_signed( int const argc, char ** argv, char const * const er_long, char const * const er_short, intmax_t const er_default ) {

        /* parsing arguments and parameters */
        for ( int er_parse = 0; er_parse < argc; er_parse ++ ) {

            /* check argument short and long forms */
            if ( ( strcmp( argv[er_parse], er_long ) == 0 ) || ( strcmp( argv[er_parse], er_short ) == 0 ) ) {

                /* check consistency */
                if ( ( ++ er_parse ) < argc ) {

                    /* convert and return parameter */
                    return( strtoimax( argv[er_parse], NULL, 10 ) );

                /* return default value */
                } else { return( er_default ); }

            }

        }

        /* return default value */
        return( er_default );

    }

    uintmax_t lc_read_unsigned( int const argc, char ** argv, char const * const er_long, char const * const er_short, uintmax_t const er_default ) {

        /* parsing arguments and parameters */
        for ( int er_parse = 0; er_parse < argc; er_parse ++ ) {

            /* check argument short and long forms */
            if ( ( strcmp( argv[er_parse], er_long ) == 0 ) || ( strcmp( argv[er_parse], er_short ) == 0 ) ) {

                /* check consistency */
                if ( ( ++ er_parse ) < argc ) {

                    /* convert and return parameter */
                    return( strtoumax( argv[er_parse], NULL, 10 ) );

                /* return default value */
                } else { return( er_default ); }

            }

        }

        /* return default value */
        return( er_default );

    }

    double lc_read_double( int const argc, char ** argv, char const * const er_long, char const * const er_short, double er_default ) {

        /* parsing arguments and parameters */
        for( int er_parse = 0 ; er_parse < argc; er_parse ++ ) {

            /* check argument short and long forms */
            if ( ( strcmp( argv[er_parse], er_long ) == 0 ) || ( strcmp( argv[er_parse], er_short ) == 0 ) ) {

                /* check consistency */
                if ( ( ++ er_parse ) < argc ) {

                    /* convert and return parameter */
                    return( atof( argv[er_parse] ) );

                /* return default value */
                } else { return( er_default ); }

            }

        }

        /* return default value */
        return( er_default );

    }

