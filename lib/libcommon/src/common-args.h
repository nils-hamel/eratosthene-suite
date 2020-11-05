/*
 *  eratosthene-suite - common library
 *
 *      Nils Hamel - nils.hamel@alumni.epfl.ch
 *      Copyright (c) 2016-2020 DHLAB, EPFL
 *      Copyright (c) 2020 Republic and Canton of Geneva
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

    /*! \file   common-args.h
     *  \author Nils Hamel <nils.hamel@bluewin.ch>
     *
     *  eratosthene-suite - common library - args
     */

/*
    header - inclusion guard
 */

    # ifndef __LC_ARGS__
    # define __LC_ARGS__

/*
    header - C/C++ compatibility
 */

    # ifdef __cplusplus
    extern "C" {
    # endif

/*
    header - internal includes
 */

    # include "common.h"

/*
    header - external includes
 */

/*
    header - preprocessor definitions
 */

/*
    header - preprocessor macros
 */

/*
    header - type definition
 */

/*
    header - structures
 */

/*
    header - function prototypes
 */

    /*! \brief switches parsers
     *
     *  This function searches in the provided arguments list if the specified
     *  flag is present, in short or long form.
     *
     *  \param argc     Main function parameters
     *  \param argv     Main function parameters
     *  \param er_long  Argument string - long form
     *  \param er_short Argument string - short form
     *
     *  \return Returns LC_TRUE if switch is found, LC_FALSE otherwise
     */

    int lc_read_flag( int const argc, char ** argv, char const * const er_long, char const * const er_short );

    /*! \brief arguments and parameters parsers
     *
     *  This function searches in the provided arguments list if the specified
     *  argument is present. As it is, the function returns the pointer to the
     *  string containing the parameter of the found argument.
     *
     *  \param argc     Main function parameters
     *  \param argv     Main function parameters
     *  \param er_long  Argument string - long form
     *  \param er_short Argument string - short form
     *
     *  \return Pointer to parameter string on success, NULL otherwise
     */

    char * lc_read_string( int const argc, char ** argv, char const * const er_long, char const * const er_short );

    /*! \brief arguments and parameters parsers
     *
     *  This function searches in the provided argument list if the specified
     *  argument is present. As it is, the function converts the parameter into
     *  the largest signed int type and returns it.
     *
     *  When the argument is not found in the list or on missing parameter, the
     *  function simply returns the provided default value.
     *
     *  \param argc       Main function parameters
     *  \param argv       Main function parameters
     *  \param er_long    Argument string - long form
     *  \param er_short   Argument string - short form
     *  \param er_default Parameter default value
     *
     *  \return Parameter value on success, the default value otherwise
     */

    intmax_t lc_read_signed( int const argc, char ** argv, char const * const er_long, char const * const er_short, intmax_t const er_default );

    /*! \brief arguments and parameters parsers
     *
     *  This function searches in the provided argument list if the specified
     *  argument is present. As it is, the function converts the parameter into
     *  the largest unsigned int type and returns it.
     *
     *  When the argument is not found in the list or on missing parameter, the
     *  function simply returns the provided default value.
     *
     *  \param argc       Main function parameters
     *  \param argv       Main function parameters
     *  \param er_long    Argument string - long form
     *  \param er_short   Argument string - short form
     *  \param er_default Parameter default value
     *
     *  \return Parameter value on success, the default value otherwise
     */

    uintmax_t lc_read_unsigned( int const argc, char ** argv, char const * const er_long, char const * const er_short, uintmax_t const er_default );

    /*! \brief arguments and parameters parsers
     *
     *  This function searches in the provided argument list if the specified
     *  argument is present. As it is, the function converts the parameter into
     *  double type and returns it.
     *
     *  When the argument is not found in the list or on missing parameter, the
     *  function simply returns the provided default value.
     *
     *  \param argc       Main function parameters
     *  \param argv       Main function parameters
     *  \param er_long    Argument string - long form
     *  \param er_short   Argument string - short form
     *  \param er_default Parameter default value
     *
     *  \return Parameter value on success, the default value otherwise
     */

    double lc_read_double( int const argc, char ** argv, char const * const er_long, char const * const er_short, double er_default );

/*
    header - C/C++ compatibility
 */

    # ifdef __cplusplus
    }
    # endif

/*
    header - inclusion guard
 */

    # endif

