/* SCE CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library Release 2.5.3
 */
/*
 *                      Emotion Engine Library
 *
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                             libsdr.h
 *
 */

#ifndef _LIBSDR_H_
#define _LIBSDR_H_

#include <sdmacro.h>	/* common/include */
#include <sdrcmd.h>	/* common/include */
#include <sifrpc.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ----------------------------------------------------------------
 *	Proto Types
 * ---------------------------------------------------------------- */

extern int sceSdRemoteInit( void );
extern int sceSdRemote( int arg, ... );
extern int sceSdRemoteCallbackInit( int priority );
extern sceSifEndFunc sceSdCallBack( sceSifEndFunc end_func );
extern int sceSdTransToIOP( void *buff, u_int sendAddr, u_int size, u_int isBlock );

#ifdef __cplusplus
}
#endif

#endif /* _LIBSDR_H_ */
