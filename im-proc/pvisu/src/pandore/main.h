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
 * @file main.h
 * @brief The definition of all main utilities.
 */

#ifndef __PMAINH__
#define __PMAINH__

namespace pandore {
   /**
    * Reads and parses the input argument list.
    * Arguments list follows the syntax ::<br>
    * <tt>operator [-hp]</tt><br>
    * in case of documentation usage, where
    * <ul>
    * <li>-h: display the usage of the operator.
    * <li>-p: display the prototype of the operator.
    * </ul>
    * <br>or <tt>operator parameter* [-m mask] images_in* images_out*</tt><br>
    * for a normal execution with input and output files and parameters.<br>
    * The "masking" parameter <tt>-m mask</tt> specifies
    * if the optional mask is used to realize:
    * <ul>
    * <li>masking = 0: neither masking nor unmasking.
    * <li>masking = 1: masking and unmaking
    * <li>masking = 2: only the masking operation
    * <li>masking = 3: only unmasking operation
    * </ul>
    * @param argc	the number of input arguments.
    * @param argv	the list of input arguments.
    * @param parc	the number of parameters.
    * @param finc	the number of input images.
    * @param foutc	the number of output images.
    * @param mask	the mask (a region map) or NULL.
    * @param objin	the pointer to the input Pandore object.
    * @param objs	the pointer to the input masked Pandore object.
    * @param objout	the pointer to the output Pandore object.
    * @param objd	the pointer to the output masked Pandore object.
    * @param parv	the list of parameters.
    * @param usage	the text describing the usage.
    * @param masking	the masking level in [0..3]
    */
   void ReadArgs( int argc, char* argv[], int parc, int finc, int foutc,
		  Pobject** mask, Pobject* objin[], Pobject* objs[],
		  Pobject* objout[], Pobject* objd[],
		  char* parv[], const char* usage, char masking = 1 );
   /**
    * Writes the output arguments list.<br>
    * The "masking" parameter <tt>-m mask</tt> specifies
    * if the optional mask is used to realize:
    * <ul>
    * <li>masking = 0: neither masking nor unmasking.
    * <li>masking = 1: masking and unmaking
    * <li>masking = 2: only the masking operation
    * <li>masking = 3: only unmasking operation
    * </ul>
    * @param argc	the number of input arguments.
    * @param argv	the list of input arguments.
    * @param parc	the number of parameters.
    * @param finc	the number of input images.
    * @param foutc	the number of output images.
    * @param mask	the mask (a region map) or NULL.
    * @param objin	the pointer to the input Pandore object.
    * @param objs	the pointer to the input masked Pandore object.
    * @param objout	the pointer to the output Pandore object.
    * @param objd	the pointer to the output masked Pandore object.
    * @param masking	the masking level in [0..3]
    */
   void WriteArgs( int argc, char* argv[], int parc, int finc, int foutc,
		   Pobject** mask, Pobject* objin[], Pobject* objs[],
		   Pobject* objout[], Pobject* objd[], char masking = 1 );
   
   /**
    * Prints the error message for bad argument list.
    * Displays a normalized error message for
    * bad input images.
    * @param objin	the array of input images.
    * @param finc	the number of input images.
    */
   void PrintErrorFormat( Pobject* objin[], int finc, char *argv[] = NULL );
   
} //End of pandore:: namespace

#endif  // __PMAINH__
