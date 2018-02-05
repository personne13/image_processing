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

#include <pandore.h>
using namespace pandore;

/**
 * @file region.cpp
 */

/*
 * Load object attributes from the file.
 */
Errc Reg1d::LoadAttributes( FILE *df ) {
   Img1d<Long>::LoadAttributes(df);
   if (Fdecode((void*)&nlabels, sizeof(nlabels), 1, df) < 1) {
      return FAILURE;
   }
   return SUCCESS;
}

/*
 * Save object attributes and object data into the file.
 */
Errc Reg1d::SaveAttributes( FILE *df ) const {
   Img1d<Long>::SaveAttributes(df);
   if (Fencode(reinterpret_cast<const void*>(&nlabels), sizeof(nlabels), 1, df) < 1) {
      return FAILURE;
   }
   return SUCCESS;
}

/*
 * Save region data.
 */
Errc Reg1d::SaveData( FILE *df ) const {
   if (Labels() <= MAXUCHAR) {
      for (Reg1d::ValueType *p = Vector(); p < Vector() + VectorSize(); p++) {
	 Uchar x = (Uchar)*p;
	 if (Fencode((void*)&x, sizeof(x), 1, df) < 1) {
	    return FAILURE;
	 }
      }
   } else if (Labels() <= MAXUSHORT) {
      for (Reg1d::ValueType *p = Vector(); p < Vector() + VectorSize(); p++) {
	 Ushort x = (Ushort)*p;
	 if (Fencode((void*)&x, sizeof(x), 1, df) < 1) {
	    return FAILURE;
	 }
      }
   } else if (Fencode((void*)Vector(), sizeof(Reg1d::ValueType), (int)(ncol), df) < (size_t)(ncol)) {
	 return FAILURE;
   }
   return SUCCESS;
}

/*
 * Load object data.
 */
Errc Reg1d::LoadData( FILE *df ) {
   if (Labels() <= MAXUCHAR) {
      for (Reg1d::ValueType *p = Vector(); p < Vector() + VectorSize(); p++) {
	 Uchar x;
	 if (Fdecode((void*)&x, sizeof(x), 1, df) < 1) {
	    return FAILURE;
	 }
	 *p = (Reg1d::ValueType)x;
      }
   } else if (Labels() <= MAXUSHORT) {
      for (Reg1d::ValueType *p = Vector(); p < Vector() + VectorSize(); p++) {
	 Ushort x;
	 if (Fdecode((void*)&x, sizeof(x), 1, df) < 1) {
	    return FAILURE;
	 }
	 *p = (Reg1d::ValueType)x;
      }
   } else if (Fdecode((void*)Vector(), sizeof(Reg1d::ValueType), (int)(ncol), df) < (size_t)(ncol)) {
      return FAILURE;
   }
   return SUCCESS;
}

/*
 * Create a new region map with the only unmasked labels.
 * A mask is a region map.
 * -> nlabels is still the same.
 */
Pobject *Reg1d::Mask( const Pobject *mask ) {
   if ((!mask)
       || (mask->Type() != Po_Reg1d)
       || (reinterpret_cast<const Reg1d *>(mask)->Size() != Size())) {
      std::cerr << "Warning: bad mask format... ignored" << std::endl;
      return this;
   }
   Reg1d *objd = new Reg1d(Props());
   const Reg1d *m = reinterpret_cast<const Reg1d *>(mask);
   Reg1d::ValueType *pm = m->Vector();
   Reg1d::ValueType *pp = objd->Vector();
   Reg1d::ValueType *pq = Vector();
   
   for (int i = 0; i < ncol; i++, pp++, pq++, pm++) {
      *pp = (*pm == 0 ) ? 0 : *pq;
   }

   objd->Labels(Labels());
   return objd;
}

/*
 * Create a new region map with the unmasked labels and new labels.
 * A mask is a region map.
 * -> nlabels is changed to the new label count.
 * -> Labels are recompute. -> (1 .. nlabels).
 */
Pobject *Reg1d::UnMask( const Pobject *mask, const Pobject *im ) {
   if ((!mask)
       || (mask->Type() != Po_Reg1d)
       || (reinterpret_cast<const Reg1d *>(mask)->Size() != Size())
       || (im->Type() != Type())
       || (reinterpret_cast<const Reg1d *>(im)->Size() != Size())) {
      std::cerr << "Warning: bad unmask format... ignored" << std::endl;
      return this;
   }
   if ((mask == NULL)



       || (mask->Type() != Po_Reg1d)
       || (im->Type() != Type())) {
      return this;
   }
   const Reg1d *objs = reinterpret_cast<const Reg1d *>(im);
   Reg1d *objd = new Reg1d(Props());
   const Reg1d *m = reinterpret_cast<const Reg1d *>(mask);
   Reg1d::ValueType *pm = m->Vector();
   Reg1d::ValueType *pp = objd->Vector();
   Reg1d::ValueType *pq = Vector();
   Reg1d::ValueType *ps = objs->Vector();
   
   for (int i = 0; i<ncol; i++, pp++, pq++, pm++, ps++) {
      *pp = (*pm == 0) ? *ps : *pq;
   }
   objd->Labels(Labels());
   return objd;
}

/*
 * Load object attributes and object data from the file.
 */
Errc Reg2d::LoadAttributes( FILE *df ) {
   Img2d<Long>::LoadAttributes(df);
   if (Fdecode((void*)&nlabels, sizeof(nlabels), 1, df) < 1) {
      return FAILURE;
   }
   return SUCCESS;
}

/*
 * Save object attributes and object data into the file.
 */
Errc Reg2d::SaveAttributes( FILE *df ) const {
   Img2d<Long>::SaveAttributes(df);
   if (Fencode(reinterpret_cast<const void*>(&nlabels), sizeof(nlabels), 1, df) < 1) {
      return FAILURE;
   }
   return SUCCESS;
}

/*
 * Save region data.
 */
Errc Reg2d::SaveData( FILE *df ) const {
   if (Labels() <= MAXUCHAR) {
      for (Reg2d::ValueType *p = Vector(); p < Vector() + VectorSize(); p++) {
	 Uchar x = (Uchar)*p;
	 if (Fencode((void*)&x, sizeof(x), 1, df) < 1) {
	    return FAILURE;
	 }
      }
   } else if (Labels() <= MAXUSHORT) {
      for (Reg2d::ValueType *p = Vector(); p < Vector() + VectorSize(); p++) {
	 Ushort x = (Ushort)*p;
	 if (Fencode((void*)&x, sizeof(x), 1, df) < 1) {
	    return FAILURE;
	 }
      }
   } else {
      if (Fencode((void*)Vector(), sizeof(Reg2d::ValueType), (size_t)(nrow * ncol), df) < (size_t)(nrow * ncol)) {
	 return FAILURE;
      }
   }

   return SUCCESS;
}

/*
 * Load object data.
 */
Errc Reg2d::LoadData( FILE *df ) {
   if (Labels() <= MAXUCHAR) {
      for (Reg2d::ValueType *p = Vector(); p < Vector() + VectorSize(); p++) {
	 Uchar x;
	 if (Fdecode((void*)&x, sizeof(x), 1, df) < 1) {
	    return FAILURE;
	 }
	 *p = (Reg2d::ValueType)x;
      }
   } else if (Labels() <= MAXUSHORT) {
      for (Reg2d::ValueType *p = Vector(); p < Vector() + VectorSize(); p++) {
	 Ushort x;
	 if (Fdecode((void*)&x, sizeof(x), 1, df) < 1) {
	    return FAILURE;
	 }
	 *p = (Reg2d::ValueType)x;
      }
   } else if (Fdecode((void*)Vector(), sizeof(Reg2d::ValueType), (int)(nrow * ncol), df) < (size_t)(nrow * ncol)) {
	 return FAILURE;
   }
   return SUCCESS;
}

/*
 * Create a new region map with the only unmasked labels.
 * A mask is a region map.
 * -> nlabels is changed to the new label count.
 * -> Labels are recompute. -> (1 .. nlabels).
 */
Pobject *Reg2d::Mask( const Pobject *mask ) {
   if ((!mask)
       || (mask->Type() != Po_Reg2d)
       || (reinterpret_cast<const Reg2d *>(mask)->Size()!= Size())) {
      std::cerr << "Warning: bad mask format... ignored" << std::endl;
      return this;
   }

   Reg2d *objd = new Reg2d(Props());
   const Reg2d *m = reinterpret_cast<const Reg2d *>(mask);
   Reg2d::ValueType *pm = m->Vector();
   Reg2d::ValueType *pp = objd->Vector();
   Reg2d::ValueType *pq = Vector();

   for (int i = 0; i < nrow * ncol; i++, pp++, pq++, pm++) {
      *pp = (*pm == 0) ? 0 : *pq;
   }

   objd->Labels(Labels());
   return objd;
}

/*
 * Create a new region map with the unmasked labels and new labels.
 * A mask is a region map.
 * -> nlabels is still the same.
 */
Pobject *Reg2d::UnMask( const Pobject *mask, const Pobject *im ) {
   if ((!mask)
       || (mask->Type() != Po_Reg2d)
       || (reinterpret_cast<const Reg2d *>(mask)->Size() != Size())
       || (im->Type() != Type())
       || (reinterpret_cast<const Reg2d *>(im)->Size() != Size())) {
      std::cerr << "Warning: bad unmask format... ignored" << std::endl;
      return this;
   }
   if ((mask == NULL)
       || (mask->Type() != Po_Reg2d)
       || (im->Type() != Type())) {
      return this;
   }

   const Reg2d *objs = reinterpret_cast<const Reg2d *>(im);
   Reg2d *objd = new Reg2d(Props());
   const Reg2d *m = reinterpret_cast<const Reg2d *>(mask);
   Reg2d::ValueType *pm = m->Vector();
   Reg2d::ValueType *pp = objd->Vector();
   Reg2d::ValueType *pq = Vector();
   Reg2d::ValueType *ps = objs->Vector();

   for (int i = 0; i < nrow * ncol; i++, pp++, pq++, pm++, ps++) {
      *pp = (*pm == 0) ? *ps : *pq;
   }
   objd->Labels(Labels());

   return objd;
}

/*
 * Load object attributes and object data from the file.
 */
Errc Reg3d::LoadAttributes( FILE *df ) {
   Img3d<Long>::LoadAttributes(df);
   if (Fdecode((void*)&nlabels, sizeof(nlabels), 1, df) < 1) {
      return FAILURE;
   }
   return SUCCESS;
}

/*
 * Save object attributes and object data into the file.
 */
Errc Reg3d::SaveAttributes( FILE *df ) const {
   Img3d<Long>::SaveAttributes(df);
   if (Fencode(reinterpret_cast<const void*>(&nlabels), sizeof(nlabels), 1, df) < 1) {
      return FAILURE;
   }
   return SUCCESS;
}

/*
 * Save object data.
 */
Errc Reg3d::SaveData( FILE *df ) const {
   if (Labels() <= MAXUCHAR) {
      for (Reg3d::ValueType *p = Vector(); p < Vector() + VectorSize(); p++) {
	 Uchar x = (Uchar)*p;
	 if (Fencode((void*)&x, sizeof(x), 1, df) < 1) {
	    return FAILURE;
	 }
      }
   } else if (Labels() <= MAXUSHORT) {
	 for (Reg3d::ValueType *p = Vector(); p < Vector() + VectorSize(); p++) {
	    Ushort x = (Ushort)*p;
	    if (Fencode((void*)&x, sizeof(x), 1, df) < 1) {
	       return FAILURE;
	    }
	 }
   } else {
      size_t s = ndep * nrow * ncol;
      // Problem : Visual C++ 2005 cannot call the fwrite function to write to a buffer that is larger than 64 MB in 
      // http://support.microsoft.com/default.aspx?scid=kb;en-us;899149
#ifdef _MSC_VER
      if (s * sizeof(Reg3d::ValueType) < (size_t)67076095) {
#endif
	 if (Fencode((void*)Vector(), sizeof(Reg3d::ValueType), s, df) < s) {
	    return FAILURE;
	 }
#ifdef _MSC_VER
      } else {
	 ValueType *data = Vector();
	 s = nrow * ncol;
	 for (int z = 0; z < ndep; z++) {
	    if (Fencode((void*)data, sizeof(Reg3d::ValueType), s, df) < s) {
	       return FAILURE;
	    }
	    data += s;
	 }
      }
#endif      
   }
   return SUCCESS;
}

/*
 * Load object data.
 */
Errc Reg3d::LoadData( FILE *df ) {
   if (Labels() <= MAXUCHAR) {
      for (Reg3d::ValueType *p = Vector(); p < Vector() + VectorSize(); p++) {
	 Uchar x;
	 if (Fdecode((void*)&x, sizeof(x), 1, df) < 1) {
	    return FAILURE;
	 }
	 *p = (Reg3d::ValueType)x;
      }
   } else if (Labels() <= MAXUSHORT) {
      for (Reg3d::ValueType *p = Vector(); p < Vector() + VectorSize(); p++) {
	 Ushort x;
	 if (Fdecode((void*)&x, sizeof(x), 1, df) < 1) {
	    return FAILURE;
	 }
	 *p = (Reg3d::ValueType)x;
      }
   } else {
      size_t s = ndep * nrow * ncol;
      // Problem : Visual C++ 2005 cannot call the fread function to read from a buffer that is larger than 64 MB in 
      // http://support.microsoft.com/default.aspx?scid=kb;en-us;899149
#ifdef _MSC_VER
      if (s * sizeof(Reg3d::ValueType) < (size_t)67076095) {
#endif
	 if (Fdecode((void*)Vector(), sizeof(Reg3d::ValueType), s, df) < s) {
	    return FAILURE;
	 }
#ifdef _MSC_VER
      } else {
	 ValueType *data = Vector();
	 s = nrow * ncol;
	 for (int z = 0; z < ndep; z++) {
	    if (Fdecode((void*)data, sizeof(Reg3d::ValueType), s, df) < s) {
	       return FAILURE;
	    }
	    data += s;
	 }
      }
#endif
   }
   return SUCCESS;
}

/*
 * Create a new region map with the only unmasked labels.
 * A mask is a region map.
 * -> nlabels is changed to the new label count.
 * -> Labels are recompute. -> (1 .. nlabels).
 */
Pobject *Reg3d::Mask( const Pobject *mask ) {
   if ((!mask)
       || (mask->Type() != Po_Reg3d)
       || (reinterpret_cast<const Reg3d *>(mask)->Size() != Size())) {
      std::cerr << "Warning: bad mask format... ignored" << std::endl;
      return this;
   }
   Reg3d *objd = new Reg3d(Props());
   const Reg3d *m = reinterpret_cast<const Reg3d *>(mask);
   Reg3d::ValueType *pm = m->Vector();
   Reg3d::ValueType *pp = objd->Vector();
   Reg3d::ValueType *pq = Vector();

   for (int i = 0; i < ndep * nrow * ncol; i++, pq++, pm++, pp++) {
      *pp = (*pm == 0) ? 0 : *pq;
   }

   objd->Labels(Labels());

   return objd;
}

/*
 * Create a new region map with the unmasked labels and new labels.
 * A mask is a region map.
 * -> nlabels is changed to the new label count.
 * -> Labels are recompute. -> (1 .. nlabels).
 */
Pobject *Reg3d::UnMask( const Pobject *mask, const Pobject *im ) {
   if ((!mask)
       ||(mask->Type() != Po_Reg3d)
       || (reinterpret_cast<const Reg3d *>(mask)->Size() != Size())
       || (im->Type() != Type())
       || (reinterpret_cast<const Reg3d *>(im)->Size() != Size())) {
      std::cerr << "Warning: bad unmask format... ignored" << std::endl;
      return this;
   }
   if ((mask == NULL)
       || (mask->Type() != Po_Reg3d)
       || (im->Type() != Type())) {
      return this;
   }
   const Reg3d *objs = reinterpret_cast<const Reg3d *>(im);
   Reg3d *objd = new Reg3d(Props());
   const Reg3d *m = reinterpret_cast<const Reg3d *>(mask);
   Reg3d::ValueType *pm = m->Vector();
   Reg3d::ValueType *pp = objd->Vector();
   Reg3d::ValueType *pq = Vector();
   Reg3d::ValueType *ps = objs->Vector();

   for (int i = 0; i < ndep * nrow * ncol; i++, pp++, pq++, pm++, ps++) {
      *pp = (*pm == 0) ? *ps : *pq;
   }

   objd->Labels(Labels());

   return objd;
}
