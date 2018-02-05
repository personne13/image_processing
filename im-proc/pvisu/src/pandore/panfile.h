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
 * @file panfile.h
 * @brief This file contains the definition of the general
 * Pandore file header structure.
 */

#ifndef __PPANFILEH__
#define __PPANFILEH__

namespace pandore {

/** @def PO_MAGIC defines the magic number of the current version of Pandore. */
#define	PO_MAGIC	"PANDORE04"

/**
 * @num Typobj
 * This enumeration contains all the identifiers of
 * the available Pandore objects.
 * This value is mainly used to identify the object
 * stored in a given file.
 * The name of a type is build as follows:
 * <br>Po_<typename> where typename is the name of the type.
 * <br>For example: Po_Collection, Po_Imx3duc, Po_Reg2d ...
 */
enum Typobj { // DO NOT CHANGE THIS ORDERING !
   object,      /*< Unknown object */
   
   /*
    * DO NOT DELETE the following inline comments (// POBJECT)!
    * (`POBJECT' is a flag for the preprocessor macro `forall')
    */
   Po_Collection,	// POBJECT
   
   Po_Img1duc,	// POBJECT
   Po_Img1dsl,	// POBJECT
   Po_Img1dsf,	// POBJECT
   
   Po_Img2duc,	// POBJECT
   Po_Img2dsl,	// POBJECT
   Po_Img2dsf,	// POBJECT

   Po_Img3duc,	// POBJECT
   Po_Img3dsl,	// POBJECT
   Po_Img3dsf,	// POBJECT

   Po_Reg1d,	// POBJECT
   Po_Reg2d,	// POBJECT
   Po_Reg3d,	// POBJECT

   Po_Graph2d,	// POBJECT
   Po_Graph3d,	// POBJECT

   Po_Imc2duc,	// POBJECT
   Po_Imc2dsl,	// POBJECT
   Po_Imc2dsf,	// POBJECT

   Po_Imc3duc,	// POBJECT
   Po_Imc3dsl,	// POBJECT
   Po_Imc3dsf,	// POBJECT

   Po_Imx1duc,	// POBJECT
   Po_Imx1dsl,	// POBJECT
   Po4_Imx1dul,	// POBJECT
   Po_Imx1dsf,	// POBJECT

   Po_Imx2duc,	// POBJECT
   Po_Imx2dsl,	// POBJECT
   Po4_Imx2dul,	// POBJECT
   Po_Imx2dsf,	// POBJECT

   Po_Imx3duc,	// POBJECT
   Po_Imx3dsl,	// POBJECT
   Po4_Imx3dul,	// POBJECT
   Po_Imx3dsf,	// POBJECT

   Po_Point1d,	// POBJECT
   Po_Point2d,	// POBJECT
   Po_Point3d,	// POBJECT

   Po_Dimension1d,	// POBJECT
   Po_Dimension2d,	// POBJECT
   Po_Dimension3d	// POBJECT
};

/** @brief The general Pandore file header structure.
 *
 * Each pandore file uses at least this general header,
 * and then adds its proper header.<br>
 * The length of the header is 36 bytes.
 * It is organized as follows:
 * <ul>
 * <li>12 bytes for the magic number.
 * <li>4 bytes for the identifier number.
 * <li>9 bytes for the name of the creator.
 * <li>10 bytes for the date of the creation.
 * <li>+1 byte for complement to 32 bits.
 * </ul>
 */
struct Po_header {
   /** The magic number @ref PO_MAGIC. */
   char	magic[12];
   /** The object type. */
   char potype[4];
   /** The autor name. */
   char	ident[9];
   /** The creation date. */
   char	date[10];
   /** Unused */
   char	unused[1];
   
   /**
    * Returns the current type of the object. The value
    * is one value of the predefined set of type @ref Typobj.
    * @return the object type id.
    */
   Typobj Type() {
      Typobj toto;
      memcpy(&toto, potype, sizeof(potype));
//       Typobj *p =  (Typobj*)toto;
//       return *p;
      return toto;
   }

   /**
    * Sets the object type with the specified type
    * from one value of the predefined set of type @ref Typobj.
    * @param type	the type value.
    */
   void Type( const Typobj type ) {
      memcpy(potype, &type, sizeof(Typobj));
   }
};

} //End of pandore:: namespace

#endif // __PPANFILEH__
