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
 * @file errc.cpp
 *
 * The class of exit values.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pandore.h>

#ifdef _MSC_VER
#include <io.h>
#define snprintf _snprintf
#endif

void pandore::Exit (const Errc &e ) { e.Exit(); }

void pandore::Errc::Exit( ) const {
   switch (_ret) {
   case FS_RET : pandore::Exit(_val.fs); 
   case Char_RET : pandore::Exit(_val.v_Char);
   case Uchar_RET : pandore::Exit(_val.v_Uchar);
   case Short_RET : pandore::Exit(_val.v_Short);
   case Ushort_RET : pandore::Exit(_val.v_Ushort);
   case Long_RET : pandore::Exit(_val.v_Long);
   case Ulong_RET : pandore::Exit(_val.v_Ulong);
   case Llong_RET : pandore::Exit(_val.v_Llong);
   case Ullong_RET : pandore::Exit(_val.v_Ullong);
   case Float_RET : pandore::Exit(_val.v_Float);
   case Double_RET : pandore::Exit(_val.v_Double);
   }
}

/*
 * Writes the result of an operator execution into the HOME/.pandore file.
 * This allows to handle Errc values, while the exit command
 * only allows CHAR values.
 */
void pandore::Exit( FS_t statut ) {
   char	filename[256];
   FILE *fp;
   char *directory;
   int processIdentifier = 0;
   
   if (!(directory = getenv(PANDORE_TMP))) {
#if defined _WIN32 || defined WIN32
      if ((directory = getenv("TEMP")) == 0) {
	 directory = const_cast<char *>("c:\\Temp");
      }
#else
      directory = const_cast<char *>("/tmp");
      processIdentifier = (int)getppid();
#endif
   }
#if defined _WIN32 || defined WIN32
   snprintf(filename, sizeof(filename), "%s\\%s%d", directory, STATUS_FILE, processIdentifier);
#else
   snprintf(filename, sizeof(filename), "%s/%s%d", directory, STATUS_FILE, processIdentifier);
#endif
   filename[sizeof(filename) - 1] = '\0';
   if (strlen(filename) > 0) {
      if ((fp = fopen(filename, "wb"))) {
	 if (statut == FAILURE) {
	    fwrite("E", sizeof(Char), 1, fp);
	    fclose(fp);
	    exit(1);
	 } else {
	    fwrite("S", sizeof(Char), 1, fp);
	    fclose(fp);
	    exit(0);
	 }
      }
   }
   exit(1);
}

/*
 * Writes the result of an operator execution into the HOME/.pandore file.
 * This allows to handle Char values, while the exit command
 * only allows CHAR values.
 */
void pandore::Exit( Char statut ) {
   FILE *fp;
   char	filename[256];
   char *directory;
   int processIdentifier = 0;

   if (!(directory = getenv(PANDORE_TMP))) {
#if defined _WIN32 || defined WIN32
      if ((directory = getenv("TEMP")) == 0) {
	 directory = const_cast<char *>("c:\\Temp");
      }
#else
      directory = const_cast<char *>("/tmp");
      processIdentifier = (int)getppid();
#endif
   }
#if defined _WIN32 || defined WIN32
   snprintf(filename, sizeof(filename), "%s\\%s%d", directory, STATUS_FILE, processIdentifier);
#else
   snprintf(filename, sizeof(filename), "%s/%s%d", directory, STATUS_FILE, processIdentifier);
#endif
   filename[sizeof(filename) - 1] = '\0';
   if (strlen(filename) > 0 && (fp = fopen(filename, "wb"))) {
      fwrite("0", sizeof(Char), 1, fp);
      fwrite(&statut, sizeof(Char), 1, fp);
      fclose(fp);
      exit(0);
   }
   exit(1);
}
/*
 * Writes the result of an operator execution into the HOME/.pandore file.
 * This allows to handle Short values, while the exit command
 * only allows CHAR values.
 */
void pandore::Exit( Short statut ) {
   FILE *fp;
   char	filename[256];
   char *directory;
   int processIdentifier = 0;

   if (!(directory = getenv(PANDORE_TMP))) {
#if defined _WIN32 || defined WIN32
      if ((directory = getenv("TEMP")) == 0) {
	 directory = const_cast<char *>("c:\\Temp");
      }
#else
      directory = const_cast<char *>("/tmp");
      processIdentifier = (int)getppid();
#endif
   }
#if defined _WIN32 || defined WIN32
   snprintf(filename, sizeof(filename), "%s\\%s%d", directory, STATUS_FILE, processIdentifier);
#else
   snprintf(filename, sizeof(filename), "%s/%s%d", directory, STATUS_FILE, processIdentifier);
#endif
   filename[sizeof(filename) - 1] = '\0';
   if (strlen(filename) > 0 && (fp = fopen(filename, "wb"))) {
      fwrite("1", sizeof(Char), 1, fp);
      fwrite(&statut, sizeof(Short), 1, fp);
      fclose(fp);
      exit(0);
   }
   exit(1);
}
/*
 * Writes the result of an operator execution into the HOME/.pandore file.
 * This allows to handle Long values, while the exit command
 * only allows CHAR values.
 */
void pandore::Exit( Long statut ) {
   FILE *fp;
   char	filename[256];
   char *directory;
   int processIdentifier = 0;

   if (!(directory = getenv(PANDORE_TMP))) {
#if defined _WIN32 || defined WIN32
      if ((directory = getenv("TEMP")) == 0) {
	 directory = const_cast<char *>("c:\\Temp");
      }
#else
      directory = const_cast<char *>("/tmp");
      processIdentifier = (int)getppid();
#endif
   }
#if defined _WIN32 || defined WIN32
   snprintf(filename, sizeof(filename), "%s\\%s%d", directory, STATUS_FILE, processIdentifier);
#else
   snprintf(filename, sizeof(filename), "%s/%s%d", directory, STATUS_FILE, processIdentifier);
#endif
   filename[sizeof(filename) - 1] = '\0';
   if (strlen(filename) > 0 && (fp = fopen(filename, "wb"))) {
      fwrite("2", sizeof(Char), 1, fp);
      fwrite(&statut, sizeof(Long), 1, fp);
      fclose(fp);
      exit(0);
   }
   exit(1);
}
/*
 * Writes the result of an operator execution into the HOME/.pandore file.
 * This allows to handle Uchar values, while the exit command
 * only allows CHAR values.
 */
void pandore::Exit( Uchar statut ) {
   FILE *fp;
   char	filename[256];
   char *directory;
   int processIdentifier = 0;

   if (!(directory = getenv(PANDORE_TMP))) {
#if defined _WIN32 || defined WIN32
      if ((directory = getenv("TEMP")) == 0) {
	 directory = const_cast<char *>("c:\\Temp");
      }
#else
      directory = const_cast<char *>("/tmp");
      processIdentifier = (int)getppid();
#endif
   }
#if defined _WIN32 || defined WIN32
   snprintf(filename, sizeof(filename), "%s\\%s%d", directory, STATUS_FILE, processIdentifier);
#else
   snprintf(filename, sizeof(filename), "%s/%s%d", directory, STATUS_FILE, processIdentifier);
#endif
   filename[sizeof(filename) - 1] = '\0';
   if (strlen(filename) > 0 && (fp = fopen(filename, "wb"))) {
      fwrite("3", sizeof(Char), 1, fp);
      fwrite(&statut, sizeof(Uchar), 1, fp);
      fclose(fp);
      exit(0);
   }
   exit(1);
}
/*
 * Writes the result of an operator execution into the HOME/.pandore file.
 * This allows to handle Ushort values, while the exit command
 * only allows CHAR values.
 */
void pandore::Exit( Ushort statut ) {
   FILE *fp;
   char	filename[256];
   char *directory;
   int processIdentifier = 0;

   if (!(directory = getenv(PANDORE_TMP))) {
#if defined _WIN32 || defined WIN32
      if ((directory = getenv("TEMP")) == 0) {
	 directory = const_cast<char *>("c:\\Temp");
      }
#else
      directory = const_cast<char *>("/tmp");
      processIdentifier = (int)getppid();
#endif
   }
#if defined _WIN32 || defined WIN32
   snprintf(filename, sizeof(filename), "%s\\%s%d", directory, STATUS_FILE, processIdentifier);
#else
   snprintf(filename, sizeof(filename), "%s/%s%d", directory, STATUS_FILE, processIdentifier);
#endif
   filename[sizeof(filename) - 1] = '\0';
   if (strlen(filename) > 0 && (fp = fopen(filename, "wb"))) {
      fwrite("4", sizeof(Char), 1, fp);
      fwrite(&statut, sizeof(Ushort), 1, fp);
      fclose(fp);
      exit(0);
   }
   exit(1);
}
/*
 * Writes the result of an operator execution into the HOME/.pandore file.
 * This allows to handle Ulong values, while the exit command
 * only allows CHAR values.
 */
void pandore::Exit( Ulong statut ) {
   FILE *fp;
   char	filename[256];
   char *directory;
   int processIdentifier = 0;

   if (!(directory = getenv(PANDORE_TMP))) {
#if defined _WIN32 || defined WIN32
      if ((directory = getenv("TEMP")) == 0) {
	 directory = const_cast<char *>("c:\\Temp");
      }
#else
      directory = const_cast<char *>("/tmp");
      processIdentifier = (int)getppid();
#endif
   }
#if defined _WIN32 || defined WIN32
   snprintf(filename, sizeof(filename), "%s\\%s%d", directory, STATUS_FILE, processIdentifier);
#else
   snprintf(filename, sizeof(filename), "%s/%s%d", directory, STATUS_FILE, processIdentifier);
#endif
   filename[sizeof(filename) - 1] = '\0';
   if (strlen(filename) > 0 && (fp = fopen(filename, "wb"))) {
      fwrite("5", sizeof(Char), 1, fp);
      fwrite(&statut, sizeof(Ulong), 1, fp);
      fclose(fp);
      exit(0);
   }
   exit(1);
}
/*
 * Writes the result of an operator execution into the HOME/.pandore file.
 * This allows to handle Float values, while the exit command
 * only allows CHAR values.
 */
void pandore::Exit( Float statut ) {
   FILE *fp;
   char	filename[256];
   char *directory;
   int processIdentifier = 0;

   if (!(directory = getenv(PANDORE_TMP))) {
#if defined _WIN32 || defined WIN32
      if ((directory = getenv("TEMP")) == 0) {
	 directory = const_cast<char *>("c:\\Temp");
      }
#else
      directory = const_cast<char *>("/tmp");
      processIdentifier = (int)getppid();
#endif
   }
#if defined _WIN32 || defined WIN32
   snprintf(filename, sizeof(filename), "%s\\%s%d", directory, STATUS_FILE, processIdentifier);
#else
   snprintf(filename, sizeof(filename), "%s/%s%d", directory, STATUS_FILE, processIdentifier);
#endif
   filename[sizeof(filename) - 1] = '\0';
   if (strlen(filename) > 0 && (fp = fopen(filename, "wb"))) {
      fwrite("6", sizeof(Char), 1, fp);
      fwrite(&statut, sizeof(Float), 1, fp);
      fclose(fp);
      exit(0);
   }
   exit(1);
}
/*
 * Writes the result of an operator execution into the HOME/.pandore file.
 * This allows to handle Double values, while the exit command
 * only allows CHAR values.
 */
void pandore::Exit( Double statut ) {
   FILE *fp;
   char	filename[256];
   char *directory;
   int processIdentifier = 0;

   if (!(directory = getenv(PANDORE_TMP))) {
#if defined _WIN32 || defined WIN32
      if ((directory = getenv("TEMP")) == 0) {
	 directory = const_cast<char *>("c:\\Temp");
      }
#else
      directory = const_cast<char *>("/tmp");
      processIdentifier = (int)getppid();
#endif
   }
#if defined _WIN32 || defined WIN32
   snprintf(filename, sizeof(filename), "%s\\%s%d", directory, STATUS_FILE, processIdentifier);
#else
   snprintf(filename, sizeof(filename), "%s/%s%d", directory, STATUS_FILE, processIdentifier);
#endif
   filename[sizeof(filename) - 1] = '\0';
   if (strlen(filename) > 0 && (fp = fopen(filename, "wb"))) {
      fwrite("7", sizeof(Char), 1, fp);
      fwrite(&statut, sizeof(Double), 1, fp);
      fclose(fp);
      exit(0);
   }
   exit(1);
}
/*
 * Writes the result of an operator execution into the HOME/.pandore file.
 * This allows to handle Llong values, while the exit command
 * only allows CHAR values.
 */
void pandore::Exit( Llong statut ) {
   FILE *fp;
   char	filename[256];
   char *directory;
   int processIdentifier = 0;

   if (!(directory = getenv(PANDORE_TMP))) {
#if defined _WIN32 || defined WIN32
      if ((directory = getenv("TEMP")) == 0) {
	 directory = const_cast<char *>("c:\\Temp");
      }
#else
      directory = const_cast<char *>("/tmp");
      processIdentifier = (int)getppid();
#endif
   }
#if defined _WIN32 || defined WIN32
   snprintf(filename, sizeof(filename), "%s\\%s%d", directory, STATUS_FILE, processIdentifier);
#else
   snprintf(filename, sizeof(filename), "%s/%s%d", directory, STATUS_FILE, processIdentifier);
#endif
   filename[sizeof(filename) - 1] = '\0';
   if (strlen(filename) > 0 && (fp = fopen(filename, "wb"))) {
      fwrite("8", sizeof(Char), 1, fp);
      fwrite(&statut, sizeof(Llong), 1, fp);
      fclose(fp);
      exit(0);
   }
   exit(1);
}
/*
 * Writes the result of an operator execution into the HOME/.pandore file.
 * This allows to handle Ullong values, while the exit command
 * only allows CHAR values.
 */
void pandore::Exit( Ullong statut ) {
   FILE *fp;
   char	filename[256];
   char *directory;
   int processIdentifier = 0;

   if (!(directory = getenv(PANDORE_TMP))) {
#if defined _WIN32 || defined WIN32
      if ((directory = getenv("TEMP")) == 0) {
	 directory = const_cast<char *>("c:\\Temp");
      }
#else
      directory = const_cast<char *>("/tmp");
      processIdentifier = (int)getppid();
#endif
   }
#if defined _WIN32 || defined WIN32
   snprintf(filename, sizeof(filename), "%s\\%s%d", directory, STATUS_FILE, processIdentifier);
#else
   snprintf(filename, sizeof(filename), "%s/%s%d", directory, STATUS_FILE, processIdentifier);
#endif
   filename[sizeof(filename) - 1] = '\0';
   if (strlen(filename) > 0 && (fp = fopen(filename, "wb"))) {
      fwrite("9", sizeof(Char), 1, fp);
      fwrite(&statut, sizeof(Ullong), 1, fp);
      fclose(fp);
      exit(0);
   }
   exit(1);
}
