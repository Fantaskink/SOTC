/* SCE CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library Release 2.5.3
 */
/* 
 *                          Libnet Library
 *
 *      Copyright (C) 2000-2002 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *       This header is used in Application(EE) & Libnet(EE).
 *
 *       Version      Date        Design      Log
 *  --------------------------------------------------------------------
 *       1.1.0        2002/01/24  ksh         sample to library
 */

#if ( !defined __LIBNET_LIBNET_H )
#define	__LIBNET_LIBNET_H

/*** Header ***/
#include <libmrpc.h>

/*** Libnet header ***/
#include <libnet/libnetdefs.h>

/*** IOP header ***/
#define	__LIBNET__
#include <../../iop/install/include/inet/inet.h>
#include <../../iop/install/include/inet/in.h>
#include <../../iop/install/include/inet/netdev.h>

#if defined(__cplusplus)
extern "C" {
#endif

/*** Libnet ***/
int 	sceLibnetInitialize( sceSifMClientData *cd, u_int buffersize, u_int stacksize, int priority );
int 	sceLibnetTerminate( sceSifMClientData *cd );
int 	sceLibnetRegisterHandler( sceSifMClientData *cd, u_int *net_buf );
int 	sceLibnetUnregisterHandler( sceSifMClientData *cd, u_int *net_buf );
int 	sceLibnetSetConfiguration( sceSifMClientData *cd, u_int *net_buf, u_int	env_addr );
int 	sceLibnetWaitGetAddress( sceSifMClientData *cd, u_int *net_buf, int *if_id, int n, struct sceInetAddress *addr, u_int flags );
int 	sceLibnetWaitGetInterfaceID( sceSifMClientData *cd, u_int *net_buf, int *if_id, int n );

/*** INET ***/
int 	sceInetPoll( sceSifMClientData *cd, u_int *net_buf, sceInetPollFd_t *fds, int nfds, int ms );
int 	sceInetName2Address( sceSifMClientData *cd, u_int *net_buf, int flags, struct sceInetAddress *paddr, const char *name, int ms, int nretry );
int 	sceInetAddress2String( sceSifMClientData *cd, u_int *net_buf, char *name, int len, struct sceInetAddress *paddr );
int 	sceInetAddress2Name( sceSifMClientData *cd, u_int *net_buf, int flags, char *name, int len, struct sceInetAddress *paddr, int ms, int nretry );
int 	sceInetCreate( sceSifMClientData *cd, u_int *net_buf, struct sceInetParam *param );
int 	sceInetOpen( sceSifMClientData *cd, u_int *net_buf, int cid, int ms );
int 	sceInetClose( sceSifMClientData *cd, u_int *net_buf, int cid, int ms );
int 	sceInetRecv( sceSifMClientData *cd, u_int *net_buf, int cid, void *ptr, int count, int *pflags, int ms );
int 	sceInetRecvFrom( sceSifMClientData *cd, u_int *net_buf, int cid, void *ptr, int count, int *pflags, struct sceInetAddress *remote_adr, int *premote_port, int ms );
int 	sceInetSend( sceSifMClientData *cd, u_int *net_buf, int cid, const void *ptr, int count, int *pflags, int ms );
int 	sceInetSendTo( sceSifMClientData *cd, u_int *net_buf, int cid, void *ptr, int count, int *pflags, struct sceInetAddress *remote_adr, int remote_port, int ms );
int 	sceInetAbort( sceSifMClientData *cd, u_int *net_buf, int cid, int flags );
int 	sceInetControl( sceSifMClientData *cd, u_int *net_buf, int cid, int code, void *ptr, int len );
int 	sceInetGetInterfaceList( sceSifMClientData *cd, u_int *net_buf, int *interface_id_list, int n );
int 	sceInetInterfaceControl( sceSifMClientData *cd, u_int *net_buf, int interface_id, int code, void *ptr, int len );
int 	sceInetGetRoutingTable( sceSifMClientData *cd, u_int *net_buf, struct sceInetRoutingEntry *p, int n );
int 	sceInetGetNameServers( sceSifMClientData *cd, u_int *net_buf, struct sceInetAddress *paddr, int n );
int 	sceInetChangeThreadPriority( sceSifMClientData *cd, u_int *net_buf, int prio );
int 	sceInetGetLog( sceSifMClientData *cd, u_int *net_buf, char *log_buf, int len, int ms );
int 	sceInetAbortLog( sceSifMClientData *cd, u_int *net_buf );

/*** INETCTL ***/
int 	sceInetCtlUpInterface( sceSifMClientData *cd, u_int *net_buf, int id );
int 	sceInetCtlDownInterface( sceSifMClientData *cd, u_int *net_buf, int id );
int 	sceInetCtlSetAutoMode( sceSifMClientData *cd, u_int *net_buf, int f_auto );
int 	sceInetCtlGetState( sceSifMClientData *cd, u_int *net_buf, int id, int *pstate );


/*** disable ***/
#if	defined	sceLibnetDisableAll
#undef	sceLibnetDisableCompatible
#define	sceLibnetDisableCompatible
#undef	sceLibnetDisableNoExtra
#define	sceLibnetDisableNoExtra
#undef	sceLibnetDisableAliases
#define	sceLibnetDisableAliases
#endif	/*** sceLibnetDisableAll ***/

/*** compatible ***/
#if ( !defined sceLibnetDisableCompatible )
int 	load_set_conf_extra( sceSifMClientData *cd, u_int *net_buf, char *fname, char *usr_name, u_int flags );
#endif	/*** sceLibnetDisableCompatible ***/

/*** extra( reserved ) ***/
#if ( !defined sceLibnetDisableNoExtra )
#endif	/*** sceLibnetDisableNoExtra ***/

/*** alias ***/
#if ( !defined sceLibnetDisableAliases )
#define	libnet_init( xcd, x1, x2, x3 )	sceLibnetInitialize( xcd, x1, x2, x3 )
#define	libnet_term( xcd )	sceLibnetTerminate( xcd )
#define	reg_handler( xcd, xnet_buf )	sceLibnetRegisterHandler( xcd, xnet_buf )
#define	unreg_handler( xcd, xnet_buf )	sceLibnetUnregisterHandler( xcd, xnet_buf )
#define	load_set_conf( xcd, xnet_buf, x1, x2 )	load_set_conf_extra( xcd, xnet_buf, x1, x2, sceLIBNETF_AUTO_LOADMODULE | sceLIBNETF_AUTO_UPIF )
#define	load_set_conf_only( xcd, xnet_buf, x1, x2 )	load_set_conf_extra( xcd, xnet_buf, x1, x2, sceLIBNETF_AUTO_LOADMODULE )
#define	wait_get_addr( xcd, xnet_buf, x1, x2 )	sceLibnetWaitGetAddress( xcd, xnet_buf, x1, 1, x2, sceLIBNETF_AUTO_UPIF )
#define	wait_get_addr_only( xcd, xnet_buf, x1, x2 )	sceLibnetWaitGetAddress( xcd, xnet_buf, x1, 1, x2, 0 )
#define	get_interface_id( xcd, xnet_buf, x1 )	sceLibnetWaitGetInterfaceID( xcd, xnet_buf, x1, 1 )
#define	up_interface( xcd, xnet_buf, x1 )	sceInetCtlUpInterface( xcd, xnet_buf, x1 )
#define	down_interface( xcd, xnet_buf, x1 )	sceInetCtlDownInterface( xcd, xnet_buf, x1 )
#endif	/*** sceLibnetDisableAliases ***/

#if defined(__cplusplus)
}
#endif

#endif	/*** __LIBNET_LIBNET_H ***/

/*** End of file ***/

