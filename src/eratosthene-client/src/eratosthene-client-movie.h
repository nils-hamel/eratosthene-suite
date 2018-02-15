/*
 *  eratosthene-suite - client
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

    /*! \file   eratosthene-client-movie.h
     *  \author Nils Hamel <nils.hamel@bluewin.ch>
     *
     *  eratosthene-suite - client - movie module
     */

/*
    header - inclusion guard
 */

    # ifndef __ER_CLIENT_MOVIE__
    # define __ER_CLIENT_MOVIE__

/*
    header - C/C++ compatibility
 */

    # ifdef __cplusplus
    extern "C" {
    # endif

/*
    header - internal includes
 */

    # include "eratosthene-client-common.h"
    # include "eratosthene-client-view.h"

/*
    header - external includes
 */

/*
    header - preprocessor definitions
 */

    /* define pseudo-constructor */
    # define ER_MOVIE_C     { 0, 0, NULL, 0, 0, 0.0, 0, { { 0.0 } }, _LE_TRUE }

    /* define stack size */
    # define ER_MOVIE_STACK ( 64 )

/*
    header - preprocessor macros
 */

/*
    header - type definition
 */

/*
    header - structures
 */

    /*! \struct er_movie_struct
     *  \brief Movie structure
     *
     *  This structure holds the required fields used to compute and export
     *  movie frames. Movie frames are computed using the points of view stack
     *  and an interpolation algorithm to create an image sequences showing the
     *  motion of the point of view.
     *
     *  Three fields are used to store the movie frame size and the frames image
     *  bytes array. The bytes array is filled using OpenGL routines to ask the
     *  content of the rendered colour buffer.
     *
     *  Three fields are used to drive the computation and the exportation of
     *  the movie frames. An index, starting at zero, is used to name the
     *  exported frames. This index is increased at each frame exportation. The
     *  two other fields drive the points of view interpolation. The first one,
     *  an integer, is used to parse the points of view stack, the second one,
     *  a floating point value, is used for the intermediates steps between two
     *  points of view.
     *
     *  Finally, the structure hold the points of view stack. The views pushed
     *  in the stack, considering their order of arrival, are used as reference
     *  points to drive the trajectory of the point of view during movie frames
     *  computation through interpolation.
     *
     *  \var er_movie_struct::mv_wbuffer
     *  Movie frames width, in pixels
     *  \var er_movie_struct::mv_hbuffer
     *  Movie frames height, in pixels
     *  \var er_movie_struct::mv_pbuffer
     *  Movie frames image bytes array
     *  \var er_movie_struct::mv_index
     *  Movie frames exportation index
     *  \var er_movie_struct::mv_parse
     *  Views interpolation parameter - integer
     *  \var er_movie_struct::mv_param
     *  Views interpolation parameter - float
     *  \var er_movie_struct::mv_stack
     *  Model views stack size
     *  \var er_movie_struct::mv_views
     *  Model views stack
     */

    typedef struct er_movie_struct {

        le_size_t   mv_wbuffer;
        le_size_t   mv_hbuffer;
        le_byte_t * mv_pbuffer;

        le_size_t   mv_index;
        le_size_t   mv_parse;
        le_real_t   mv_param;

        le_size_t   mv_stack;
        er_view_t   mv_views[ER_MOVIE_STACK];

    le_enum_t _status; } er_movie_t;


/*
    header - function prototypes
 */

    /*! \brief constructor/destructor methods
     *
     *  This function creates and returns a movie structure. It initialises the
     *  structure fields and retrieve the screen sizes.
     *
     *  Using the obtained screen resolution, the function allocate the memory
     *  of the colour buffer that receives the OpenGL colour buffer. The buffer
     *  is used to export movie frames.
     *
     *  This function returning the created structure, the status is stored in
     *  the structure itself using the reserved \b _status field.
     *
     *  \return Returns the created movie structure
     */

    er_movie_t er_movie_create( le_size_t const er_width, le_size_t const er_height );
    //er_movie_t er_movie_create( le_void_t );

    /*! \brief constructor/destructor methods
     *
     *  This function deletes the provided movie structure. It releases memory
     *  allocation of the colour buffer and clears the structure fields.
     *
     *  \param er_movie Movie structure
     */

    le_void_t er_movie_delete( er_movie_t * const er_movie );

    /*! \brief accessor methods
     *
     *  This function returns the interpolated point of view that corresponds to
     *  the points of view stack and the interpolation parameters hold in the
     *  provided movie structure.
     *
     *  The function uses the view module function to compute the interpolated
     *  point of view before to return it.
     *
     *  \param er_movie Movie structure
     *
     *  \return Returns the interpolated point of view
     */

    er_view_t er_movie_get( er_movie_t const * const er_movie );

    /*! \brief mutator methods
     *
     *  This function pushes the provided point of view on the stack hold by the
     *  movie structure. The points of view stack being limited in size by the
     *  \b ER_MOVIE_STACK constant, the function checks the stack size before to
     *  push the provided view.
     *
     *  The function checks also the size of the stack for interpolation values
     *  management. As the stack is of size zero, the function resets the two
     *  interpolation parameters. This allows the movie structure to be easily
     *  reset by emptying the points of view stack.
     *
     *  \param er_movie Movie structure
     *  \param er_view  View structure
     */

    le_void_t er_movie_set( er_movie_t * const er_movie, er_view_t const * const er_view );

    /*! \brief mutator methods
     *
     *  This function empties the points of view stack of the provided movie
     *  structure. This also triggers the reset of the provided movie structure
     *  interpolation parameters.
     *
     *  \param er_movie Movie structure
     */

    le_void_t er_movie_set_clear( er_movie_t * const er_movie );

    /*! \brief movie methods
     *
     *  This function retrieves the OpenGL colour buffer in the movie structure
     *  buffer and exports it in a portable network graphic file using the frame
     *  exportation index of the structure. The index is then incremented.
     *
     *  As the frame is exported, the function updates the interpolation values
     *  of the movie structure. The function and the \b er_movie_get() being
     *  called by the main function loops as the \b ER_COMMON_MOVIE execution
     *  mode is active, the update of the interpolation parameters affects the
     *  result of the \b er_movie_get().
     *
     *  After interpolation values update, the function checks their state to
     *  determine if interpolation is complete. This indicates the function to
     *  return the \b ER_COMMON_MOVIE execution mode to tell the main function
     *  loop to continue movie frame computation ; the \b ER_COMMON_VIEW code
     *  is returned to tell the main function loop to return back to the model
     *  view execution mode, the movie frames being computed.
     *
     *  The function also checks if the points of view stacks contains at least
     *  two points of view, allowing interpolation to take place. Otherwise, the
     *  function returns the \b ER_COMMON_VIEW execution mode to tell the main
     *  loop to abort movie frames computation.
     *
     *  \param er_movie Movie structure
     *
     *  \return Returns loop execution mode
     */

    le_enum_t er_movie( er_movie_t * const er_movie );

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

