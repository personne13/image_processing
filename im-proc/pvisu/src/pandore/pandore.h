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
 * @file pandore.h
 * @brief The main include file.
 */

#ifndef __PPANDOREH__
#define __PPANDOREH__

#ifndef _WIN32
#include <unistd.h>
#endif

//#include <fcntl.h>
#include <cstdlib>

#include <stdexcept>
#include <iostream>

/*
 * The general constants.
 */
#ifndef STATUS_FILE
#define STATUS_FILE  "pandore"
#endif

#include "ptypes.h"
#include "errc.h"
#include "panfile.h"
#include "pobject.h"
#include "dimension.h"
#include "point.h"
#include "neighbours.h"
#include "image.h"
#include "region.h"
#include "graph.h"

#include "collection.h"
#include "main.h"


// #include "plist.h" //
// #include "plimits.h" //


/*
 *  Some macro functions
 */
#ifndef MIN
#define	MIN(x, y) (((x) < (y)) ? (x) : (y))
#endif
// #ifndef MAX
// #define	MAX(x, y) (((x) > (y)) ? (x) : (y))
// #endif
// #ifndef SQR
// #define	SQR(x) ((x) * (x))
// #endif
// #ifndef CUB
// #define	CUB(x) ((x) * (x) * (x))
// #endif
// #ifndef ABS
// #define	ABS(x) (((x) > 0) ? (x) : (-1 * (x)))
// #endif

#endif // __PPANDOREH__
