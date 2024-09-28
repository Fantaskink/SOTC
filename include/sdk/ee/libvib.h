/* SCE CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library Release 2.5
 */
/*
 *                Controller Vibration Library
 *                          Version 2.01
 *                           Shift-JIS
 *
 *         Copyright (C) 2001 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                         libvib - libvib.h
 *                     header file of libvib
 *
 *       Version        Date            Design      Log
 *  --------------------------------------------------------------------
 *      2.01          2001-03-09        nozomu      the first version
 *
 */

#ifndef _LIBVIB_H_
#define _LIBVIB_H_

#if defined(__LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
extern "C"
{
#endif
    int sceVibGetProfile(int, unsigned char *);
    int sceVibSetActParam(int, int, unsigned char *, int, unsigned char *);
#if defined(__LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif /* _LIBVIB_H_ */
