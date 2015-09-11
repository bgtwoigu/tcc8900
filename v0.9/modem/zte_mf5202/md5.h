#ifndef MD5_H
#define MD5_H
/*===========================================================================

 MD5.H - header file for MD5C.C

 Copyright (C) 1991-2, RSA Data Security, Inc. Created 1991. All
 rights reserved.

 License to copy and use this software is granted provided that it
 is identified as the "RSA Data Security, Inc. MD5 Message-Digest
 Algorithm" in all material mentioning or referencing this software
 or this function.

 License is also granted to make and use derivative works provided
 that such works are identified as "derived from the RSA Data
 Security, Inc. MD5 Message-Digest Algorithm" in all material
 mentioning or referencing the derived work.

 RSA Data Security, Inc. makes no representations concerning either
 the merchantability of this software or the suitability of this
 software for any particular purpose. It is provided "as is"
 without express or implied warranty of any kind.

 These notices must be retained in any copies of any part of this
 documentation and/or software.

 ===========================================================================*/

/* <EJECT> */
/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$PVCSPath: L:/src/asw/MSM5100/CP_REL_A/vcs/md5.h_v   1.1.1.0   30 Nov 2001 17:45:28   fchan  $
$Header: //depot/asic/msmshared/services/common/md5.h#1 $ $DateTime: 2006/05/04 09:20:44 $ $Author: ckrishna $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
05/03/06   ck      Removed all the featurizations and mainlined it.
10/01/01   sb      Merged the following from the common archive:
08/15/01   ss      Added FEATURE_DS_MOBILE_IP to include the file for CHAP or 
                   MOBILE_IP.
03/14/00   cah     Added FEATURE_DS_CHAP to include this file for snn.
06/25/98   ck      Checked in the file.

===========================================================================*/

/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
//#include "comdef.h"

/* <EJECT> */
/*=========================================================================*/

/* POINTER defines a generic pointer type */
typedef unsigned char *POINTER;
typedef  unsigned long     dword1; //zxl
//#include "AEEComdef.h" //zxl
/* MD5 context. */
typedef struct {
  dword1 state[4];                                   /* state (ABCD) */
  dword1 count[2];        /* number of bits, modulo 2^64 (lsb first) */
  unsigned char buffer[64];                         /* input buffer */
} MD5_CTX;

extern void MD5Init_UI (MD5_CTX *);
extern void MD5Init_NV (MD5_CTX *);
extern void MD5Update (MD5_CTX *, unsigned char *, unsigned int);
extern void MD5Final (unsigned char [16], MD5_CTX *);
void ASCtoHex(unsigned char *pcHex, char *pcASC,int iLength);
void HextoASC(char *pcASC,unsigned char *pcHex, int iLength);
#endif /* MD5_H */
