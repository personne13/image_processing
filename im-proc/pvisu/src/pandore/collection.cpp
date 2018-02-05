/* -*- c-basic-offset: 3; mode:c++ -*-
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
 * @file collection.cpp
 *
 * Collection of objects
 */

#include <pandore.h>
using namespace pandore;

#if _MSC_VER
#define snprintf _snprintf
#endif

/*
 * rem: fread "size" is not in LoadAttributes
 * because size is not an attribute
 * but a computed value.
 */
Errc Collection::LoadData( FILE *file ) {
   Long size;
   Long attr[3];

   // number of elements
   if (Fdecode((void*)&size, sizeof(size), 1, file) < 1) {
      return FAILURE;
   }

   for (int i = 0; i < size; ++i) {
      // bytesize of the element.
      if (Fdecode((void*)attr, sizeof(attr[0]), 3, file) < 1) {
	 return FAILURE;
      }

      char *type = new char[attr[1] + 1];
      char *name = new char[attr[2] + 1];

      // Important: keep fread versus Fdecode
      // for chars.
      if ((fread(type, attr[1], 1, file) < 1) ||
	  (fread(name, attr[2], 1, file) < 1)) {
	 return FAILURE;
      }

      type[attr[1]] = 0;
      name[attr[2]] = 0;
      BundledObject *bo = LoadBundledObject(file, type, attr[0], _inversionMode);
      if (!bo) {
	 std::cerr << "ERROR: problem during reading ..." << std::endl;
	 return FAILURE;
      }
      Set(name, bo);
      
      delete[] name;
      delete[] type;
   }
   
   return SUCCESS;
}

Errc Collection::SaveData( FILE *file ) const {
   std::map< std::string, BundledObject * >::const_iterator i;
   Long attr[3];
   std::string s;
   
   for (i = _objs.begin(); i!= _objs.end(); ++i) {
      s = i->second->Type();
      // case of pobject array: use 4 bytes for pointers.
      if (s.find("PArray:") != std::string::npos) {
	 attr[0] = (Long)(i->second->ByteSize() * POINTERSIZE) / sizeof(void*);
      } else {
	 attr[0] = (Long)i->second->ByteSize();
      }
      attr[1] = (Long)s.size();
      attr[2] = (Long)i->first.size();
      
      if ((Fencode((void*)attr, sizeof(attr), 1, file) < 1) ||
	  (Fencode(reinterpret_cast<const void*>(s.c_str()), attr[1], 1, file) < 1) ||
	  (Fencode(reinterpret_cast<const void*>(i->first.c_str()), attr[2], 1, file) < 1) ||
	  (i->second->Save(file) != SUCCESS)) {
	 return FAILURE;
      }
   }
   
   return SUCCESS;
}

Errc Collection::NbOf( const std::string &s, std::string &type_out, Long &number_out, Long &minsize_out ) const {
   char name[512];
   Long nbrcomp = 0;
   Long min = MAXLONG;
   
   snprintf(name, sizeof(name), "%s.%d", s.c_str(), nbrcomp + 1);
   name[sizeof(name) - 1] = '\0';
   std::string type = GetType(name);
   if (type == "Array:Char") {
      do {
	 int n_ = (int)this->GETARRAYSIZE(name, Char);
	 if (n_ < min) {
	    min = n_;
	 }
	 ++nbrcomp;
	 snprintf(name, sizeof(name), "%s.%d", s.c_str(), nbrcomp + 1);
	 name[sizeof(name) - 1] = '\0';
      } while (Exists(name) && type == GetType(name));
   } else if (type == "Char") {
      do {
	 int n_ = 1;
	 if (n_ < min) {
	    min = n_;
	 }
	 ++nbrcomp;
	 snprintf(name, sizeof(name), "%s.%d", s.c_str(), nbrcomp + 1);
	 name[sizeof(name) - 1] = '\0';
      } while (Exists(name) && type == GetType(name));
   } else
   if (type == "Array:Uchar") {
      do {
	 int n_ = (int)this->GETARRAYSIZE(name, Uchar);
	 if (n_ < min) {
	    min = n_;
	 }
	 ++nbrcomp;
	 snprintf(name, sizeof(name), "%s.%d", s.c_str(), nbrcomp + 1);
	 name[sizeof(name) - 1] = '\0';
      } while (Exists(name) && type == GetType(name));
   } else if (type == "Uchar") {
      do {
	 int n_ = 1;
	 if (n_ < min) {
	    min = n_;
	 }
	 ++nbrcomp;
	 snprintf(name, sizeof(name), "%s.%d", s.c_str(), nbrcomp + 1);
	 name[sizeof(name) - 1] = '\0';
      } while (Exists(name) && type == GetType(name));
   } else
   if (type == "Array:Short") {
      do {
	 int n_ = (int)this->GETARRAYSIZE(name, Short);
	 if (n_ < min) {
	    min = n_;
	 }
	 ++nbrcomp;
	 snprintf(name, sizeof(name), "%s.%d", s.c_str(), nbrcomp + 1);
	 name[sizeof(name) - 1] = '\0';
      } while (Exists(name) && type == GetType(name));
   } else if (type == "Short") {
      do {
	 int n_ = 1;
	 if (n_ < min) {
	    min = n_;
	 }
	 ++nbrcomp;
	 snprintf(name, sizeof(name), "%s.%d", s.c_str(), nbrcomp + 1);
	 name[sizeof(name) - 1] = '\0';
      } while (Exists(name) && type == GetType(name));
   } else
   if (type == "Array:Ushort") {
      do {
	 int n_ = (int)this->GETARRAYSIZE(name, Ushort);
	 if (n_ < min) {
	    min = n_;
	 }
	 ++nbrcomp;
	 snprintf(name, sizeof(name), "%s.%d", s.c_str(), nbrcomp + 1);
	 name[sizeof(name) - 1] = '\0';
      } while (Exists(name) && type == GetType(name));
   } else if (type == "Ushort") {
      do {
	 int n_ = 1;
	 if (n_ < min) {
	    min = n_;
	 }
	 ++nbrcomp;
	 snprintf(name, sizeof(name), "%s.%d", s.c_str(), nbrcomp + 1);
	 name[sizeof(name) - 1] = '\0';
      } while (Exists(name) && type == GetType(name));
   } else
   if (type == "Array:Long") {
      do {
	 int n_ = (int)this->GETARRAYSIZE(name, Long);
	 if (n_ < min) {
	    min = n_;
	 }
	 ++nbrcomp;
	 snprintf(name, sizeof(name), "%s.%d", s.c_str(), nbrcomp + 1);
	 name[sizeof(name) - 1] = '\0';
      } while (Exists(name) && type == GetType(name));
   } else if (type == "Long") {
      do {
	 int n_ = 1;
	 if (n_ < min) {
	    min = n_;
	 }
	 ++nbrcomp;
	 snprintf(name, sizeof(name), "%s.%d", s.c_str(), nbrcomp + 1);
	 name[sizeof(name) - 1] = '\0';
      } while (Exists(name) && type == GetType(name));
   } else
   if (type == "Array:Ulong") {
      do {
	 int n_ = (int)this->GETARRAYSIZE(name, Ulong);
	 if (n_ < min) {
	    min = n_;
	 }
	 ++nbrcomp;
	 snprintf(name, sizeof(name), "%s.%d", s.c_str(), nbrcomp + 1);
	 name[sizeof(name) - 1] = '\0';
      } while (Exists(name) && type == GetType(name));
   } else if (type == "Ulong") {
      do {
	 int n_ = 1;
	 if (n_ < min) {
	    min = n_;
	 }
	 ++nbrcomp;
	 snprintf(name, sizeof(name), "%s.%d", s.c_str(), nbrcomp + 1);
	 name[sizeof(name) - 1] = '\0';
      } while (Exists(name) && type == GetType(name));
   } else
   if (type == "Array:Llong") {
      do {
	 int n_ = (int)this->GETARRAYSIZE(name, Llong);
	 if (n_ < min) {
	    min = n_;
	 }
	 ++nbrcomp;
	 snprintf(name, sizeof(name), "%s.%d", s.c_str(), nbrcomp + 1);
	 name[sizeof(name) - 1] = '\0';
      } while (Exists(name) && type == GetType(name));
   } else if (type == "Llong") {
      do {
	 int n_ = 1;
	 if (n_ < min) {
	    min = n_;
	 }
	 ++nbrcomp;
	 snprintf(name, sizeof(name), "%s.%d", s.c_str(), nbrcomp + 1);
	 name[sizeof(name) - 1] = '\0';
      } while (Exists(name) && type == GetType(name));
   } else
   if (type == "Array:Ullong") {
      do {
	 int n_ = (int)this->GETARRAYSIZE(name, Ullong);
	 if (n_ < min) {
	    min = n_;
	 }
	 ++nbrcomp;
	 snprintf(name, sizeof(name), "%s.%d", s.c_str(), nbrcomp + 1);
	 name[sizeof(name) - 1] = '\0';
      } while (Exists(name) && type == GetType(name));
   } else if (type == "Ullong") {
      do {
	 int n_ = 1;
	 if (n_ < min) {
	    min = n_;
	 }
	 ++nbrcomp;
	 snprintf(name, sizeof(name), "%s.%d", s.c_str(), nbrcomp + 1);
	 name[sizeof(name) - 1] = '\0';
      } while (Exists(name) && type == GetType(name));
   } else
   if (type == "Array:Float") {
      do {
	 int n_ = (int)this->GETARRAYSIZE(name, Float);
	 if (n_ < min) {
	    min = n_;
	 }
	 ++nbrcomp;
	 snprintf(name, sizeof(name), "%s.%d", s.c_str(), nbrcomp + 1);
	 name[sizeof(name) - 1] = '\0';
      } while (Exists(name) && type == GetType(name));
   } else if (type == "Float") {
      do {
	 int n_ = 1;
	 if (n_ < min) {
	    min = n_;
	 }
	 ++nbrcomp;
	 snprintf(name, sizeof(name), "%s.%d", s.c_str(), nbrcomp + 1);
	 name[sizeof(name) - 1] = '\0';
      } while (Exists(name) && type == GetType(name));
   } else
   if (type == "Array:Double") {
      do {
	 int n_ = (int)this->GETARRAYSIZE(name, Double);
	 if (n_ < min) {
	    min = n_;
	 }
	 ++nbrcomp;
	 snprintf(name, sizeof(name), "%s.%d", s.c_str(), nbrcomp + 1);
	 name[sizeof(name) - 1] = '\0';
      } while (Exists(name) && type == GetType(name));
   } else if (type == "Double") {
      do {
	 int n_ = 1;
	 if (n_ < min) {
	    min = n_;
	 }
	 ++nbrcomp;
	 snprintf(name, sizeof(name), "%s.%d", s.c_str(), nbrcomp + 1);
	 name[sizeof(name) - 1] = '\0';
      } while (Exists(name) && type == GetType(name));
   } else
      {
	 return FAILURE;
      }
   minsize_out = min;
   number_out = nbrcomp;
   type_out = type;
   return SUCCESS;
}
