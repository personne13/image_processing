/* -*- mode: c++; c-basic-offset: 3 -*-
 *
 * Copyright (c), GREYC.
 * All rights reserved
 *
 * You may use this file under the terms of the BSD license as follows:
 *
 * "Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in
 *     the documentation and/or other materials provided with the
 *     distribution.
 *   * Neither the name of the GREYC, nor the name of its
 *     contributors may be used to endorse or promote products
 *     derived from this software without specific prior written
 *     permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
 *
 *
 * For more information, refer to:
 * https://clouard.users.greyc.fr/Pandore
 */

/**
 * @file neighbours.h
 * @brief The definition of the arrays for neighbour accesses.
 */

#ifndef __PNEIGHBOURSH__
#define __PNEIGHBOURSH__

namespace pandore {

   const Long v2x []={ -1, 1, -1};

   /** The predefined array for y-shift for 4-connexity neighbours. */
   const Long v4y []={  0, -1,  0,  1,  0};
   /** The predefined array for x-shift for 4-connexity neighbours. */
   const Long v4x []={ -1,  0,  1,  0, -1};

   /** The predefined array for y-shift for 8-connexity neighbours. */
   const Long v8y []={  0, -1, -1, -1,  0,  1,  1,  1,  0};
   /** The predefined array for x-shift for 8-connexity neighbours. */
   const Long v8x []={ -1, -1,  0,  1,  1,  1,  0, -1, -1};

   /** The predefined array for z-shift for 6-connexity neighbours. */
   const Long v6z[]={ -1,  0,  0,  1,  0,  0, -1};
   /** The predefined array for y-shift for 6-connexity neighbours. */
   const Long v6y[]={  0,  0, -1,  0,  0,  1,  0};
   /** The predefined array for x-shift for 6-connexity neighbours. */
   const Long v6x[]={  0, -1,  0,  0,  1,  0,  0};

   /** The predefined array for z-shift for 26-connexity neighbours. */
   const Long v26z[]={ -1, -1, -1, -1, -1, -1, -1, -1, -1,  0, 0, 0, 0,  +1, +1, +1, +1, +1, +1, +1, +1, +1,   0, 0, 0, 0,  -1};

   /** The predefined array for y-shift for 26-connexity neighbours. */
   const Long v26y[]={  0, 0, -1, -1, -1, 0, 1, 1, 1,  0, -1, -1, -1,   0, 0, 1, 1, 1, 0, -1, -1, -1,   0, 1, 1, 1, 0};

   /** The predefined array for x-shift for 26-connexity neighbours. */
   const Long v26x[]={  0, -1, -1, 0, 1, 1, 1, 0, -1, -1, -1, 0, 1,   0, 1, 1, 0, -1, -1, -1, 0, 1,   1, 1, 0, -1, 0};

   /** The predefined array for point shift for 2-connexity neighbours. */
   const Point1d v2[]= { Point1d(-1),
			 Point1d( 1),
			 Point1d(-1) };

   /** The predefined array for point shift for 4-connexity neighbours. */
   const Point2d v4[]= { Point2d( 0, -1), 
			 Point2d(-1,  0), 
			 Point2d( 0,  1), 
			 Point2d( 1,  0),
			 Point2d( 0, -1) };

   /** The predefined array for point shift for 8-connexity neighbours. */
   const Point2d v8[]= { Point2d( 0, -1),
			 Point2d(-1, -1),
			 Point2d(-1,  0),
			 Point2d(-1,  1),
			 Point2d( 0,  1),
			 Point2d( 1,  1),
			 Point2d( 1,  0),
			 Point2d( 1, -1),
			 Point2d( 0, -1) };

   /** The predefined array for point shift for 6-connexity neighbours. */
   const Point3d v6[]= { Point3d(-1,  0,  0),
			 Point3d( 0,  0, -1),
			 Point3d( 0, -1,  0),
			 Point3d( 1,  0,  0),
			 Point3d( 0,  0,  1),
			 Point3d( 0,  1,  0),
			 Point3d(-1,  0,  0) };

   /** The predefined array for point shift for 26-connexity neighbours. */
   const Point3d v26[]={ Point3d(-1,  0,  0),
			 Point3d(-1,  0, -1),
			 Point3d(-1, -1, -1),
			 Point3d(-1, -1,  0),
			 Point3d(-1, -1,  1),
			 Point3d(-1,  0,  1),
			 Point3d(-1,  1,  1),
			 Point3d(-1,  1,  0),
			 Point3d(-1,  1, -1),

			 Point3d( 0,  0, -1),
			 Point3d( 0, -1, -1),
			 Point3d( 0, -1,  0),
			 Point3d( 0, -1,  1),

			 Point3d(+1,  0,  0),
			 Point3d(+1,  0,  1),
			 Point3d(+1,  1,  1),
			 Point3d(+1,  1,  0),
			 Point3d(+1,  1, -1),
			 Point3d(+1,  0, -1),
			 Point3d(+1, -1, -1),
			 Point3d(+1, -1,  0),
			 Point3d(+1, -1,  1),

			 Point3d( 0, 0,  1),
			 Point3d( 0, 1,  1),
			 Point3d( 0, 1,  0),
			 Point3d( 0, 1, -1),

			 Point3d(-1, 0, 0) };

   /**
    * The predefined array for x-shift for a given connexity neighbours.
    * Indirection with connexity. vc[connexity][neighbour]
    */
   const Long* const vcx[]={0, 0, v2x, 0, v4x, 0, v6x, 0, v8x,
			    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			    0, 0, 0, 0, 0, v26x};
   /**
    * The predefined array for y-shift for a given connexity neighbours.
    * Indirection with connexity. vc[connexity][neighbour]
    */
   const Long* const vcy[]={0, 0, 0, 0, v4y, 0, v6y, 0, v8y,
			    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			    0, 0, 0, 0, 0, v26y};
   /**
    * The predefined array for z-shift for a given connexity neighbours.
    * Indirection with connexity. vc[connexity][neighbour]
    */
   const Long* const vcz[]={0, 0, 0, 0, 0, 0, v6z, 0, 0,
			    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			    0, 0, 0, 0, 0, v26z};

   /**
    * The predefined array for point shift for a given connexity neighbours.
    * Indirection with connexity. vc[connexity][neighbour]
    */
   const Point* const vc[]={0, 0, v2, 0, v4, 0, v6, 0, v8,
			    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			    0, 0, 0, 0, 0, v26};

} //End of pandore:: namespace

#endif // __PNEIGHBOURSH__
