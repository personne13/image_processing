/* -*- c-basic-offset: 3 -*-
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

#include <string.h>
#include <pandore.h>
using namespace pandore;

#if _MSC_VER
#define snprintf _snprintf
#endif

/**
 * @file main.cpp
 * Defines some facilities for reading and writing commands arguments.
 */

/**
 * Reads a command argument list (argc, argv).
 * It follows the syntax :: parameter* [-m mask] images_in* images_out*
 * The "masking" parameter specifies if the optional mask is used to realize
 * masking = 0: neither masking nor unmasking.
 * masking = 1: masking and unmaking
 * masking = 2: only the masking operation
 * masking = 3: only unmasking operation
 */
void pandore::ReadArgs( int argc, char* argv[], int parc, int finc, int foutc, Pobject** mask,
			Pobject* objin[], Pobject* objs[], Pobject* objout[], Pobject* objd[],
			char* parv[], const char* usage, char masking ) {
   char error[1024];

   // -s: name of the parameters.
   if (argc >= 2 && !strncmp(argv[1], "-s", 2)) {
      strncpy(error, usage, sizeof(error));
      error[sizeof(error) - 1] = 0;
      char * p = error;
      while (*p && (*p != '%'|| *(p + 1) != 's')) {
	 p++;
      }
      if (*p) {
	 p += 3;
      }
      char *p1 = p, *p2;
      for (int i = 0; i < parc; i++) {
	 p2 = p1;
	 while (*p1 && *p1 != ' ') {
	    p1++;
	 }
	 *(p1++) = '\0';
	 while (*p1 && *p1 == ' ') {
	    p1++;
	 }
	 std::cout << "$" << i << ":" << p2 << ";";
      }
      std::cout << std::endl;
      exit(0);
   }

   // Check the number of arguments or -p option.
   // Print PROTOTYPE : (name - number of parameters - number of inputs - number of output).
   if (argc >= 2 && !strncmp(argv[1], "-p", 2)) {
      snprintf(error, sizeof(error), "%s %d %d %d", argv[0], parc, finc, foutc);
      error[sizeof(error) - 1] = 0;
      std::cout << error << std::endl;
      exit(0);
   }

   // Check the number of arguments or -h option.
   // Print USAGE.
   if (argc <= parc || (argc >= 2 && !strncmp(argv[1], "-h", 2))) {
      snprintf(error, sizeof(error), usage, argv[0]);
      error[sizeof(error) - 1] = 0;
      std::cerr << error << std::endl;
      exit(0);
   }

   // Read parameters (all are floating point values).
   int k = 1;
   for (int i = 0; i < parc; parv[i++] = argv[k++]) ;

   // Read the Mask if any:
   // a Mask is introduced by -m flag,
   // and is supposed to be a region map.
   *mask = NULL;
   if ((argc > k) && (!strncmp("-m", argv[k], 2))) {
      k++; // -m
      *mask = LoadFile(((k >= argc) || (!strncmp(argv[k], "-", 1))) ? NULL : argv[k]);
      k++; // mask file
   }

   if (finc > 0) {
      // Read all input files
      for (int i = 0; i < finc; i++, k++) {
	 objin[i] = LoadFile(((k >= argc) || (!strncmp(argv[k], "-", 1))) ? NULL : argv[k]);
	 if (objin[i] == NULL) {
	    Exit(FAILURE);
	 }
      }

      // If the mask is introduced, mask all input images.
      if ((*mask) && ((masking == 1) || (masking == 2))) {
	 for (int i = 0; i < finc; i++) {
	    objs[i] = objin[i]->Mask(*mask);
	 }
      } else {
	 for (int i = 0; i < finc; i++) {
	    objs[i] = objin[i];
	 }
      }
   }
   
   // Initialize every output objects to NULL value.
   for (int i = 0; i < foutc; i++) {
      objd[i] = objout[i] = NULL;
   }
}

/**
 * Write a command argument list,
 * i.e all the output object in output file.
 * masking = 0: neither masking nor unmasking.
 * masking = 1: masking and unmaking
 * masking = 2: only the masking operation
 * masking = 3: only unmasking operation
 */
void pandore::WriteArgs( int argc, char* argv[], int parc, int finc, int foutc, Pobject** mask,
			 Pobject* objin[], Pobject* [], Pobject* objout[], Pobject* objd[], char masking ) {
   // If only one output file is empty: don't save any file.
   for (int i = 0; i < foutc; i++) {
      if (objd[i] == 0) {
	 return;
      }
   }

   // Search the name among output files.
   int k = (*mask) ? parc + 1 + finc + 2 : parc + 1 + finc;

   // Unfilter output image with the mask and related input image.
   // If output object is not of the same type then the related input object
   // then do not use unmasking operation for this object.
   for (int i = 0; i < foutc; i++) {
      if ((*mask)
	  && ((masking == 1) || (masking == 3))
	  && (i <= finc) && (objd[i]->Type() == objin[i]->Type())) {
	 objout[i] = objd[i]->UnMask(*mask, objin[i]);
      } else {
	 objout[i] = objd[i];
      }
   }

   for (int i = 0; i < foutc; i++, k++) {
      if (SaveFile(objout[i], ((k >= argc) || (!strncmp(argv[k], "-", 1))) ? 0 : argv[k]) == FAILURE) {
 	 Exit(FAILURE);
      }
   }
}

/**
 * Displays a normalized error message for
 * bad input images.
 * @param objin	the array of input images.
 * @param finc	the number of input images.
 */
void pandore::PrintErrorFormat( Pobject* objin[], int finc, char *argv[] ) {
   if (argv != NULL) {
      std::cerr << argv[0] << ": ";
   }
   std::cerr << "Error: input types not supported by this operator: ";
   for (int _i = 0; _i < finc; _i++) {
      if (_i > 0) {
	 std::cerr << " x ";
      }
      std::cerr << objin[_i]->Name();
   }
   std::cerr << std::endl;
}
