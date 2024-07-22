/* SCE CONFIDENTIAL
 * "PlayStation 2" Programmer Tool Runtime Library NTGUI package (Release 2.5 version) 
 */
/*
 *	GUI Network Setting Application Library
 *
 *                          Version 1.3
 *
 *      Copyright (C) 2001 Sony Computer Entertainment Inc.
 *	                 All Rights Reserved.
 *
 *                          ntguicnf.h
 *
 *        Version       Date            Design     Log
 *  --------------------------------------------------------------------
 *        1.1           2001.05.25      tetsu      Beta Release
 *        1.2           2001.07.19      tetsu      Fist Ver. Release
 *        1.3           2001.11.28      tetsu      add sceNetCnfSelected
 */
#ifndef __sceNetGuiCnf_H_
#define __sceNetGuiCnf_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>

#define SCE_NETGUICNF_STR_SIZE (256)

/* マウスの動作ポイント */
enum
{
    SCE_NETGUICNF_MOUSE_MESSAGE_TYPE__PRESS = 0,
    SCE_NETGUICNF_MOUSE_MESSAGE_TYPE__RELEASE,
    SCE_NETGUICNF_MOUSE_MESSAGE_TYPE__MOVE,
};

/* ソフトウェアキーボードのキー状態設定タイプ */
enum
{
    SCE_NETGUICNF_ENABLE_KEY_TYPE_ENABLE_LISTED_AND_DISABLE_NOTLISTED = 0,
    SCE_NETGUICNF_ENABLE_KEY_TYPE_ENABLE_ALL,
    SCE_NETGUICNF_ENABLE_KEY_TYPE_DISABLE_LISTED,
};

/* マウスのボタン状態 */
enum
{
    SCE_NETGUICNF_MOUSE_BUTTON_LEFT   = 1<<0,
    SCE_NETGUICNF_MOUSE_BUTTON_RIGHT  = 1<<1,
    SCE_NETGUICNF_MOUSE_BUTTON_MIDDLE = 1<<2,
};

/* 起動オプション */
enum
{
    SCE_NETGUICNF_FLAG_USE_HDD           = 1<<0,
    SCE_NETGUICNF_FLAG_USE_USB_MOUSE     = 1<<1,
    SCE_NETGUICNF_FLAG_USE_USB_KB        = 1<<2,
    SCE_NETGUICNF_FLAG_USE_SELECT_OPTION = 1<<3,
    SCE_NETGUICNF_FLAG_SELECT_ONLY       = 1<<4,
    SCE_NETGUICNF_FLAG_MC_SLOT1_ONLY     = 1<<5,
};

/* dialing_type */
enum
{
    SCE_NETGUICNF_DIALINGTYPE_TONE = 0,
    SCE_NETGUICNF_DIALINGTYPE_PULSE,
};

/* phy_config */
enum
{
    SCE_NETGUICNF_PHYCONFIG_AUTO  = 1,
    SCE_NETGUICNF_PHYCONFIG_10    = 2,
    SCE_NETGUICNF_PHYCONFIG_10_FD = 3,
    SCE_NETGUICNF_PHYCONFIG_TX    = 5,
    SCE_NETGUICNF_PHYCONFIG_TX_FD = 6,
};

/* dhcp */
enum
{
    SCE_NETGUICNF_NOUSE_DHCP = 0,
    SCE_NETGUICNF_USE_DHCP,
};

/* pppoe */
enum
{
    SCE_NETGUICNF_NOUSE_PPPOE = 0,
    SCE_NETGUICNF_USE_PPPOE,
};

/* type */
enum
{
    SCE_NETGUICNF_TYPE_ETH = 1,
    SCE_NETGUICNF_TYPE_PPP,
    SCE_NETGUICNF_TYPE_NIC,
};

typedef void * (*sceNetGuiCnfCallback_Malloc)( size_t size );
typedef void * (*sceNetGuiCnfCallback_Memalign)( size_t align, size_t size );
typedef void * (*sceNetGuiCnfCallback_Realloc)( void * old_ptr, size_t new_size );
typedef void   (*sceNetGuiCnfCallback_Free)( void * ptr );
typedef void   (*sceNetGuiCnfCallback_SKBInit)(void);
typedef void   (*sceNetGuiCnfCallback_SKBDestroy)(void);
typedef void * (*sceNetGuiCnfCallback_SKBGetVif1PktTopAddr)(void);
typedef void   (*sceNetGuiCnfCallback_SKBGetStatus)( int * w, int * h );
typedef int    (*sceNetGuiCnfCallback_SKBSendMouseMessage)( int type, int x, int y );
typedef void   (*sceNetGuiCnfCallback_SKBEnableKey)( int type, unsigned char * keynames[], int keynames_size );
typedef void   (*sceNetGuiCnfCallback_SKBEveryFrame)(void);
typedef void   (*sceNetGuiCnfCallback_SJIStoUTF8 )( unsigned char * dst, size_t dst_size, unsigned char const* src );
typedef void   (*sceNetGuiCnfCallback_UTF8toSJIS )( unsigned char * dst, size_t dst_size, unsigned char const* src );
typedef void   (*sceNetGuiCnfCallback_UsbMouseRead )( int * delta_x, int * delta_y, int * buttons, int * wheel );
typedef void   (*sceNetGuiCnfCallback_UsbKbRead )(void);
typedef void   (*sceNetGuiCnfCallback_PadRead )( unsigned int * paddata );

typedef struct sceNetGuiCnf_Color {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
} sceNetGuiCnf_Color_t;

typedef struct sceNetGuiCnf_Color4 {
    sceNetGuiCnf_Color_t aColor[4];
} sceNetGuiCnf_Color4_t;

typedef struct sceNetGuiCnfEnvData {
    char attach_ifc[SCE_NETGUICNF_STR_SIZE];      /* (0x0100) */
    char attach_dev[SCE_NETGUICNF_STR_SIZE];      /* (0x0200) */
    char address[SCE_NETGUICNF_STR_SIZE];         /* (0x0300) */
    char netmask[SCE_NETGUICNF_STR_SIZE];         /* (0x0400) */
    char gateway[SCE_NETGUICNF_STR_SIZE];         /* (0x0500) */
    char dns1_address[SCE_NETGUICNF_STR_SIZE];    /* (0x0600) */
    char dns2_address[SCE_NETGUICNF_STR_SIZE];    /* (0x0700) */
    char phone_numbers1[SCE_NETGUICNF_STR_SIZE];  /* (0x0800) */
    char phone_numbers2[SCE_NETGUICNF_STR_SIZE];  /* (0x0900) */
    char phone_numbers3[SCE_NETGUICNF_STR_SIZE];  /* (0x0a00) */
    char auth_name[SCE_NETGUICNF_STR_SIZE];       /* (0x0b00) */
    char auth_key[SCE_NETGUICNF_STR_SIZE];        /* (0x0c00) */
    char vendor[SCE_NETGUICNF_STR_SIZE];          /* (0x0d00) */
    char product[SCE_NETGUICNF_STR_SIZE];         /* (0x0e00) */
    char chat_additional[SCE_NETGUICNF_STR_SIZE]; /* (0x0f00) */
    char outside_number[SCE_NETGUICNF_STR_SIZE];  /* (0x1000) */
    char outside_delay[SCE_NETGUICNF_STR_SIZE];   /* (0x1100) */
    char dhcp_host_name[SCE_NETGUICNF_STR_SIZE];  /* (0x1200) */
    char peer_name[SCE_NETGUICNF_STR_SIZE];       /* (0x1300) */
    int dialing_type;                             /* (0x1304) */
    int type;                                     /* (0x1308) */
    int phy_config;                               /* (0x130c) */
    int idle_timeout;                             /* (0x1310) */
    unsigned char dhcp;                           /* (0x1311) */
    unsigned char dns1_nego;                      /* (0x1312) */
    unsigned char dns2_nego;                      /* (0x1313) */
    unsigned char f_auth;                         /* (0x1314) */
    unsigned char auth;                           /* (0x1315) */
    unsigned char pppoe;                          /* (0x1316) */
    unsigned char prc_nego;                       /* (0x1317) */
    unsigned char acc_nego;                       /* (0x1318) */
    unsigned char accm_nego;                      /* (0x1319) */
    unsigned char p0;                             /* (0x131a) */
    unsigned char p1;                             /* (0x131b) */
    unsigned char p2;                             /* (0x131c) */
    int mtu;                                      /* (0x1320) */
} sceNetGuiCnfEnvData_t;

/* select_device */
enum
{
    SCE_NETGUICNF_SELECT_DEVICE_NO_DEVICE,
    SCE_NETGUICNF_SELECT_DEVICE_MC0,
    SCE_NETGUICNF_SELECT_DEVICE_MC1,
    SCE_NETGUICNF_SELECT_DEVICE_HDD,
};

typedef struct sceNetGuiCnfSelected {
    char select_env[SCE_NETGUICNF_STR_SIZE]; /* 選択された組み合わせ */
    char select_ifc[SCE_NETGUICNF_STR_SIZE]; /* 選択された接続プロバイダ設定 */
    char select_dev[SCE_NETGUICNF_STR_SIZE]; /* 選択された接続機器設定 */
    int env_device;                          /* 選択された組み合わせのデバイス */
    int ifc_device;                          /* 選択された接続プロバイダ設定のデバイス */
    int dev_device;                          /* 選択された接続機器設定のデバイス */
} sceNetGuiCnfSelected_t;

typedef struct sceNetGuiCnf_Arg
{
    /* 起動オプション */
    int flag;

    /* v-blank 開始を待つセマフォ */
    int _sema_vsync;

    /* 追加時のデフォルトデータへのポインタ */
    sceNetGuiCnfEnvData_t *default_env_data;

    /* 選択結果を返すバッファへのポインタ */
    sceNetGuiCnfEnvData_t *result_env_data;

    /* 選択された設定名とそのデバイスを返すバッファへのポインタ */
    sceNetGuiCnfSelected_t *selected_configuration;

    /* メモリアロケーション コールバック関数ポインタ */
    sceNetGuiCnfCallback_Malloc               cb_malloc;
    sceNetGuiCnfCallback_Memalign             cb_memalign;
    sceNetGuiCnfCallback_Realloc              cb_realloc;
    sceNetGuiCnfCallback_Free                 cb_free;

    /* ソフトウェアキーボード コールバック関数ポインタ */
    sceNetGuiCnfCallback_SKBInit              cb_skb_init;
    sceNetGuiCnfCallback_SKBDestroy           cb_skb_destroy;
    sceNetGuiCnfCallback_SKBGetVif1PktTopAddr cb_skb_getvif1pkttopaddr;
    sceNetGuiCnfCallback_SKBGetStatus         cb_skb_getstatus;
    sceNetGuiCnfCallback_SKBSendMouseMessage  cb_skb_sendmousemessage;
    sceNetGuiCnfCallback_SKBEnableKey         cb_skb_enablekey;
    sceNetGuiCnfCallback_SKBEveryFrame        cb_skb_everyframe;

    /* SJIS <-> UTF8 変換する関数へのポインタ */
    sceNetGuiCnfCallback_SJIStoUTF8           cb_sjis_to_utf8;
    sceNetGuiCnfCallback_UTF8toSJIS           cb_utf8_to_sjis;

    /* USBマウスの 入力を受ける関数へのポインタ */
    sceNetGuiCnfCallback_UsbMouseRead         cb_mouse_read;

    /* パッドの 入力を受ける関数へのポインタ */
    sceNetGuiCnfCallback_PadRead              cb_pad_read;

    /* USBキーボードの入力を受ける関数へのポインタ */
    sceNetGuiCnfCallback_UsbKbRead            cb_kb_read;

    /* 背景をファイルからから読み込むためのパス */
    char * str_path_bg;

    /* 色 */
    sceNetGuiCnf_Color4_t                     color_titlebar;
    sceNetGuiCnf_Color4_t                     color_window;
    sceNetGuiCnf_Color4_t                     color_pagebutton;
    sceNetGuiCnf_Color4_t                     color_msgbox_ok;
    sceNetGuiCnf_Color4_t                     color_msgbox_yesno;
    sceNetGuiCnf_Color4_t                     color_msgbox_warning;
    sceNetGuiCnf_Color4_t                     color_msgbox_wait;

} sceNetGuiCnf_Arg_t;

void sceNetGuiCnf_Do( sceNetGuiCnf_Arg_t * arg );


/* SendKBMessage の 要因 */
enum
{
    SCE_NETGUICNF_KBMSG_TYPE_SOFTKB,
    SCE_NETGUICNF_KBMSG_TYPE_HARDKB,
};
void sceNetGuiCnf_SendKBMessage( int type, unsigned char * keyname );

#ifdef __cplusplus
}
#endif


#endif //__sceNetGuiCnf_H_

