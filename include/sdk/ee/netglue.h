/* SCE CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library Release 2.5.3
 */
/*
 *                      Emotion Engine Library
 *                          Version 1.00
 *                           Shift-JIS
 *
 *      Copyright (C) 2002 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                       <netglue - netglue.h>
 *               <glue library header for protocol stack >
 *
 *       Version        Date            Design      Log
 *  --------------------------------------------------------------------
 *       1.00           Sep,21,2001     komaki      first version
 *       1.10           Jan,29,2002     komaki      move errno function
 *                                                  inside extern "C"{}
 */

#include <netglue/netdb.h>
#include <netglue/sys/socket.h>
#include <netglue/netinet/in.h>
#include <netglue/netinet/tcp.h>
#include <netglue/arpa/inet.h>

#ifdef __cplusplus
extern "C" {
#endif

int *__sceNetGlueErrnoLoc(void);
int *__sceNetGlueHErrnoLoc(void);

#define	sceNetGlueErrno		(*__sceNetGlueErrnoLoc())
#define	sceNetGlueHErrno	(*__sceNetGlueHErrnoLoc())

int sceNetGlueAbort(int sockfd);
int sceNetGlueThreadInit(int thread_id);
int sceNetGlueThreadTerminate(int thread_id);

#ifdef __cplusplus
}
#endif

