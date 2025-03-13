/* SCE CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 1.6
 */
/* 
 *                      Emotion Engine Library
 *                           Shift-JIS
 *
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                       librspu2 - librspu2.h
  *                     header file of librspu2
 *
 */

#ifndef _LIBRSPU2_H_
#define _LIBRSPU2_H_

#include <sifrpc.h>
#include <rspu2cmd.h>

/* ----------------------------------------------------------------
 *	MACRO for libspu
 * ---------------------------------------------------------------- */
#define SPU_SUCCESS	0
#define SPU_INVALID_ARGS (-3)
#define SPU_DIAG	(-2)
#define SPU_CHECK	(-1)
#define SPU_OFF		0
#define SPU_ON		1
#define SPU_CLEAR	2
#define SPU_RESET	3
#define SPU_DONT_CARE	4
#define SPU_ALL		0
#define SPU_CDONLY	5
#define SPU_VOICEONLY	6
#define SPU_CONT	7
#define SPU_BIT		8
#define SPU_NULL	0
/* Macros below will be obsoleted. */
#define SpuDiag		SPU_DIAG
#define SpuCheck	SPU_CHECK
#define SpuOff		SPU_OFF
#define SpuOn		SPU_ON
#define SpuClear	SPU_CLEAR
#define SpuReset	SPU_RESET
#define SpuDontCare	SPU_DONT_CARE
#define SpuALL		SPU_ALL
#define SpuCDOnly	SPU_CDONLY
#define SpuVoiceOnly	SPU_VOICEONLY
#define SpuCont		SPU_CONT
#define SpuNull		SPU_NULL

#define SPU_OFF_ENV_ON	2
#define SPU_ON_ENV_OFF	3
/* Macros below will be obsoleted. */
#define SpuOffEnvOn	SPU_OFF_ENV_ON
#define SpuOnEnvOff	SPU_ON_ENV_OFF

#define SPU_ERROR	(-1)
/* Macros below will be obsoleted. */
#define SpuError	SPU_ERROR

#define SPU_TRANSFER_BY_DMA 0L
#define SPU_TRANSFER_BY_IO 1L
/* Macros below will be obsoleted. */
#define SpuTransferByDMA SPU_TRANSFER_BY_DMA
#define SpuTransferByIO  SPU_TRANSFER_BY_IO
#define SpuTransByDMA SpuTransferByDMA
#define SpuTransByIO SpuTransferByIO

#define SPU_TRANSFER_WAIT 1
#define SPU_TRANSFER_PEEK 0
#define SPU_TRANSFER_GLANCE SPU_TRANSFER_PEEK

#define SPU_AUTODMA_ONESHOT 0  //spu2
#define SPU_AUTODMA_LOOP 1   //spu2
#define SPU_AUTODMA_START_ADDR (1 << 1)   //spu2

/*
 * Voice designate
 */

#ifndef __SPU_VOICE
#define __SPU_VOICE

#define SPU_00CH (0x1L<< 0)
#define SPU_01CH (0x1L<< 1)
#define SPU_02CH (0x1L<< 2)
#define SPU_03CH (0x1L<< 3)
#define SPU_04CH (0x1L<< 4)
#define SPU_05CH (0x1L<< 5)
#define SPU_06CH (0x1L<< 6)
#define SPU_07CH (0x1L<< 7)
#define SPU_08CH (0x1L<< 8)
#define SPU_09CH (0x1L<< 9)
#define SPU_10CH (0x1L<<10)
#define SPU_11CH (0x1L<<11)
#define SPU_12CH (0x1L<<12)
#define SPU_13CH (0x1L<<13)
#define SPU_14CH (0x1L<<14)
#define SPU_15CH (0x1L<<15)
#define SPU_16CH (0x1L<<16)
#define SPU_17CH (0x1L<<17)
#define SPU_18CH (0x1L<<18)
#define SPU_19CH (0x1L<<19)

#define SPU_20CH (0x1L<<20)
#define SPU_21CH (0x1L<<21)
#define SPU_22CH (0x1L<<22)
#define SPU_23CH (0x1L<<23)

#define SPU_0CH SPU_00CH
#define SPU_1CH SPU_01CH
#define SPU_2CH SPU_02CH
#define SPU_3CH SPU_03CH
#define SPU_4CH SPU_04CH
#define SPU_5CH SPU_05CH
#define SPU_6CH SPU_06CH
#define SPU_7CH SPU_07CH
#define SPU_8CH SPU_08CH
#define SPU_9CH SPU_09CH

#define SPU_ALLCH (SPU_00CH | SPU_01CH | SPU_02CH | SPU_03CH | SPU_04CH | SPU_05CH | SPU_06CH | SPU_07CH | SPU_08CH | SPU_09CH |SPU_10CH | SPU_11CH | SPU_12CH | SPU_13CH | SPU_14CH | SPU_15CH | SPU_16CH | SPU_17CH | SPU_18CH | SPU_19CH | SPU_20CH | SPU_21CH | SPU_22CH | SPU_23CH)

#define SPU_KEYCH(x) (0x1L<<(x))
#define SPU_VOICECH(x) SPU_KEYCH(x)

#endif /* __SPU_VOICE */

/* for Voice setting */

#define	SPU_VOICE_VOLL		(0x01 <<  0) /* 音量(左)                 */
#define	SPU_VOICE_VOLR		(0x01 <<  1) /* 音量(右)                 */
#define	SPU_VOICE_VOLMODEL	(0x01 <<  2) /* 音量モード(左)           */
#define	SPU_VOICE_VOLMODER	(0x01 <<  3) /* 音量モード(右)           */
#define	SPU_VOICE_PITCH		(0x01 <<  4) /* 音程 (ピッチ指定)        */
#define	SPU_VOICE_NOTE		(0x01 <<  5) /* 音程 (ノート指定)        */
#define	SPU_VOICE_SAMPLE_NOTE	(0x01 <<  6) /* 波形データサンプルノート */
#define	SPU_VOICE_WDSA		(0x01 <<  7) /* 波形データ先頭アドレス   */
#define	SPU_VOICE_ADSR_AMODE	(0x01 <<  8) /* ADSR Attack rate モード  */
#define	SPU_VOICE_ADSR_SMODE	(0x01 <<  9) /* ADSR Sustain rate モード */
#define	SPU_VOICE_ADSR_RMODE	(0x01 << 10) /* ADSR Release rate モード */
#define	SPU_VOICE_ADSR_AR	(0x01 << 11) /* ADSR Attack rate         */
#define	SPU_VOICE_ADSR_DR	(0x01 << 12) /* ADSR Decay rate          */
#define	SPU_VOICE_ADSR_SR	(0x01 << 13) /* ADSR Sustain rate        */
#define	SPU_VOICE_ADSR_RR	(0x01 << 14) /* ADSR Release rate        */
#define	SPU_VOICE_ADSR_SL	(0x01 << 15) /* ADSR Sustain level       */
#define	SPU_VOICE_LSAX		(0x01 << 16) /* ループ開始アドレス       */
#define	SPU_VOICE_ADSR_ADSR1	(0x01 << 17) /* ADSR adsr1 for `VagAtr'  */
#define	SPU_VOICE_ADSR_ADSR2	(0x01 << 18) /* ADSR adsr2 for `VagAtr'  */

#define	SPU_VOICE_DIRECT	0
#define	SPU_VOICE_LINEARIncN	1
#define	SPU_VOICE_LINEARIncR	2
#define	SPU_VOICE_LINEARDecN	3
#define	SPU_VOICE_LINEARDecR	4
#define	SPU_VOICE_EXPIncN	5
#define	SPU_VOICE_EXPIncR	6
#define	SPU_VOICE_EXPDec	7
#define	SPU_VOICE_EXPDecN	SPU_VOICE_EXPDec
#define	SPU_VOICE_EXPDecR	SPU_VOICE_EXPDec

#define	SPU_DECODED_FIRSTHALF	0
#define	SPU_DECODED_SECONDHALF	1
#define	SPU_DECODE_FIRSTHALF	SPU_DECODED_FIRSTHALF
#define	SPU_DECODE_SECONDHALF	SPU_DECODED_SECONDHALF


#define	SPU_COMMON_MVOLL		(0x01 <<  0) /* マスタ音量(左)                     */
#define	SPU_COMMON_MVOLR		(0x01 <<  1) /* マスタ音量(右)                     */
#define	SPU_COMMON_MVOLMODEL		(0x01 <<  2) /* マスタ音量モード(左)               */
#define	SPU_COMMON_MVOLMODER		(0x01 <<  3) /* マスタ音量モード(右)               */
#define	SPU_COMMON_RVOLL		(0x01 <<  4) /* リバーブ音量(左)                   */
#define	SPU_COMMON_RVOLR		(0x01 <<  5) /* リバーブ音量(右)                   */
#define	SPU_COMMON_CDVOLL		(0x01 <<  6) /* CD 入力 音量 (左)                  */
#define	SPU_COMMON_CDVOLR		(0x01 <<  7) /* CD 入力 音量 (右)                  */
#define	SPU_COMMON_CDREV		(0x01 <<  8) /* CD 入力 リバーブオン/オフ          */
#define	SPU_COMMON_CDMIX		(0x01 <<  9) /* CD 入力 オン/オフ                  */
#define	SPU_COMMON_EXTVOLL		(0x01 << 10) /* 外部デジタル入力 音量 (左)         */
#define	SPU_COMMON_EXTVOLR		(0x01 << 11) /* 外部デジタル入力 音量 (右)         */
#define	SPU_COMMON_EXTREV		(0x01 << 12) /* 外部デジタル入力 リバーブオン/オフ */
#define	SPU_COMMON_EXTMIX		(0x01 << 13) /* 外部デジタル入力 オン/オフ         */

/* for Reverb setting */

#define	SPU_REV_MODE		(0x01 <<  0) /* モード設定                     */
#define	SPU_REV_DEPTHL		(0x01 <<  1) /* リバーブの深さ(左)             */
#define	SPU_REV_DEPTHR		(0x01 <<  2) /* リバーブの深さ(右)             */
#define	SPU_REV_DELAYTIME	(0x01 <<  3) /* Delay Time (ECHO, DELAY のみ)  */
#define	SPU_REV_FEEDBACK	(0x01 <<  4) /* Feedback   (ECHO のみ)   */

#define SPU_REV_MODE_CHECK	(-1)
#define SPU_REV_MODE_OFF	0
#define SPU_REV_MODE_ROOM	1
#define SPU_REV_MODE_STUDIO_A	2
#define SPU_REV_MODE_STUDIO_B	3
#define SPU_REV_MODE_STUDIO_C	4
#define SPU_REV_MODE_HALL	5
#define SPU_REV_MODE_SPACE	6
#define SPU_REV_MODE_ECHO	7
#define SPU_REV_MODE_DELAY	8
#define SPU_REV_MODE_PIPE	9
#define SPU_REV_MODE_MAX	10

#define SPU_REV_MODE_CLEAR_WA	0x100

/* ----------------------------------------------------------------
 *	Event flushing
 * ---------------------------------------------------------------- */

#define SPU_EVENT_KEY      (0x01 << 0)
#define SPU_EVENT_PITCHLFO (0x01 << 1)
#define SPU_EVENT_NOISE    (0x01 << 2)
#define SPU_EVENT_REVERB   (0x01 << 3)

#define SPU_EVENT_ALL 0

/* ----------------------------------------------------------------
 *	Digital Out
 * ---------------------------------------------------------------- */

#define SPU_SPDIF_OUT_OFF        0
#define SPU_SPDIF_OUT_PCM        1
#define SPU_SPDIF_OUT_BITSTREAM  2

#define SPU_SPDIF_COPY_NORMAL    0x00
#define SPU_SPDIF_COPY_PROHIBIT  0x80

#define SPU_SPDIF_MEDIA_CD       0x000
#define SPU_SPDIF_MEDIA_DVD      0x800

/* ----------------------------------------------------------------
 *	Structure
 * ---------------------------------------------------------------- */

typedef struct {
    short left;	       /* Lch */
    short right;       /* Rch */
} SpuVolume;

typedef struct {
    unsigned int	voice;		/* 設定ボイス:
					   SpuSetVoiceAttr: 各ボイスは bit 列
					   SpuGetVoiceAttr: ボイスは bit 値
					   */
    unsigned int	mask;		/* 設定属性ビット (Get では無効)	*/
    SpuVolume		volume;		/* 音量					*/
    SpuVolume		volmode;	/* 音量モード				*/
    SpuVolume		volumex;	/* 現在の音量 (Set では無効)		*/
    unsigned short	pitch;		/* 音程 (ピッチ指定)			*/
    unsigned short	note;		/* 音程 (ノート指定)			*/
    unsigned short	sample_note;	/* 音程 (ノート指定)			*/
    short		envx;		/* 現在のエンベロープ値 (Set では無効)  */
    unsigned int	addr;		/* 波形データ先頭アドレス		*/
    unsigned int	loop_addr;	/* ループ開始アドレス			*/
    int		a_mode;		/* Attack rate mode			*/
    int		s_mode;		/* Sustain rate mode			*/
    int		r_mode;		/* Release rate mode			*/
    unsigned short	ar;		/* Attack rate				*/
    unsigned short	dr;		/* Decay rate				*/
    unsigned short	sr;		/* Sustain rate				*/
    unsigned short	rr;		/* Release rate				*/
    unsigned short	sl;		/* Sustain level			*/
    unsigned short	adsr1;		/* adsr1 for `VagAtr' */
    unsigned short	adsr2;		/* adsr2 for `VagAtr' */
} SpuVoiceAttr;

typedef struct {
    short	voiceNum;		/* voice number */
    short	pad;			/* padding */
    SpuVoiceAttr	attr;		/* voice attribute */
} SpuLVoiceAttr;

typedef struct {
    unsigned int	mask;	  /* 設定マスク		  */
    
    int		mode;	  /* リバーブモード       */
    SpuVolume		depth;	  /* リバーブの深さ	  */
    int                delay;	  /* Delay Time (ECHO, DELAY のみ)  */
    int                feedback; /* Feedback   (ECHO のみ)         */
} SpuReverbAttr;

#define SPU_DECODEDDATA_SIZE 0x200
#define SPU_DECODEDATA_SIZE SPU_DECODEDDATA_SIZE
typedef struct {
    short cd_left  [SPU_DECODEDDATA_SIZE];
    short cd_right [SPU_DECODEDDATA_SIZE];
    short voice1   [SPU_DECODEDDATA_SIZE];
    short voice3   [SPU_DECODEDDATA_SIZE];
} SpuDecodedData;
typedef SpuDecodedData SpuDecodeData;

typedef struct {
    SpuVolume	volume;		  /* 音量		  */
    int	reverb;		  /* リバーブオン/オフ	  */
    int	mix;		  /* ミキシングオン/オフ  */
} SpuExtAttr;

typedef struct {
    unsigned int	mask;	  /* 設定マスク		  */
    
    SpuVolume		mvol;	  /* マスタ音量		  */
    SpuVolume		mvolmode; /* マスタ音量モード	  */
    SpuVolume		mvolx;	  /* 現在のマスタ音量	  */
    SpuExtAttr		cd;	  /* CD 入力属性	  */
    SpuExtAttr		ext;	  /* 外部デジタル入力属性 */
} SpuCommonAttr;

#ifndef __SPU_IRQCALLBACK_PROC
#define __SPU_IRQCALLBACK_PROC
typedef void (*SpuIRQCallbackProc)(void);
#endif /* __SPU_IRQCALLBACK_PROC */
#ifndef __SPU_TRANSFERCALLBACK_PROC
#define __SPU_TRANSFERCALLBACK_PROC
typedef void (*SpuTransferCallbackProc)(void);
#endif /* __SPU_TRANSFERCALLBACK_PROC */

/* ----------------------------------------------------------------
 *	for SPU Malloc (used in SpuInitMalloc())
 * ---------------------------------------------------------------- */

#define SPU_MALLOC_RECSIZ 8

/* ----------------------------------------------------------------
 *	User specifiable global environment
 * ---------------------------------------------------------------- */

typedef struct {
    unsigned int mask;
    unsigned int queueing;
} SpuEnv;

#define SPU_ENV_EVENT_QUEUEING			(0x01 << 0)


/* ----------------------------------------------------------------
 *	MACRO for stream
 * ---------------------------------------------------------------- */

#define SPU_ST_NOT_AVAILABLE 0
#define SPU_ST_ACCEPT  1

#define SPU_ST_ERROR (-1)
#define SPU_ST_INVALID_ARGUMENT (-2)
#define SPU_ST_WRONG_STATUS (-3)

#define SPU_ST_STOP     2
#define SPU_ST_IDLE     3
#define SPU_ST_PREPARE  4
#define SPU_ST_START    5
#define SPU_ST_PLAY     6
#define SPU_ST_TRANSFER 7
#define SPU_ST_FINAL    8


/* VAG's header size */
#define SPU_ST_VAG_HEADER_SIZE 0x30

typedef struct {
    char status;		/* stream status */
    char pad1;			/* padding */
    char pad2;			/* padding */
    char pad3;			/* padding */
    int last_size;		/* the size of last transferring
				   (last_size <= (size / 2)) */
    unsigned int buf_addr;	/* The start address of stream buffer */
    unsigned int data_addr;	/* The start address of SPU streaming
				   data in main memory */
} SpuStVoiceAttr;

typedef struct {
    int size;			/* The size of stream buffer */
    int low_priority;		/* transfer priority */
    SpuStVoiceAttr voice [24];
} SpuStEnv;

#ifndef __SPU_ST_TRANSFERCALLBACK_PROC
#define __SPU_ST_TRANSFERCALLBACK_PROC
typedef void (*SpuStCallbackProc)(unsigned int, int);
#endif /* __SPU_TRANSFERCALLBACK_PROC */


/* ----------------------------------------------------------------
 *	MACRO for libsnd
 * ---------------------------------------------------------------- */

#define SSPLAY_INFINITY  0 
#define SS_NOTICK	 0x1000
#define SS_NOTICK0	 0
#define SS_TICK60	 1
#define SS_TICK240	 2
#define SS_TICK120	 3
#define SS_TICK50	 4
#define SS_TICKVSYNC	 5
#define SS_TICKMODE_MAX  6
#define SSPLAY_PAUSE     0
#define SSPLAY_PLAY      1
#define SS_SOFF          0      
#define SS_SON           1     
#define SS_MIX           0    
#define SS_REV           1   
#define SS_SERIAL_A      0 
#define SS_SERIAL_B      1     
#define SS_MUTE_OFF      0     
#define SS_MUTE_ON       1     

#define SS_IMEDIATE 	    0
#define SS_IMMEDIATE        0
#define SS_WAIT_COMPLETED   1

#define SS_REV_TYPE_OFF        0
#define SS_REV_TYPE_ROOM       1
#define SS_REV_TYPE_STUDIO_A   2
#define SS_REV_TYPE_STUDIO_B   3
#define SS_REV_TYPE_STUDIO_C   4
#define SS_REV_TYPE_HALL       5
#define SS_REV_TYPE_SPACE      6
#define SS_REV_TYPE_ECHO       7
#define SS_REV_TYPE_DELAY      8
#define SS_REV_TYPE_PIPE       9
#define SSSKIP_TICK    0  
#define SSSKIP_NOTE4   1  
#define SSSKIP_NOTE8   2  
#define SSSKIP_BAR     3 

#define SS_SEQ_TABSIZ     176

#define SND_VOLL  1
#define SND_VOLR  2
#define SND_ADSR1 4
#define SND_ADSR2 8
#define SND_ADDR  16
#define SND_PITCH 32

#ifndef NULL
#define NULL 0
#endif


/*
 * Vag & Vab Structure 
 */ 
typedef struct VabHdr {         /* VAB Bank Headdings */

	int           form;          /* always 'VABp' */
	int           ver;           /* VAB file version number */
	int           id;            /* VAB id */
	unsigned int  fsize;         /* VAB file size */
	unsigned short reserved0;     /* system reserved */
	unsigned short ps;            /* # of the programs in this bank */
	unsigned short ts;            /* # of the tones in this bank */
	unsigned short vs;            /* # of the vags in this bank */
	unsigned char  mvol;          /* master volume for this bank */
	unsigned char  pan;           /* master panning for this bank */
	unsigned char  attr1;         /* bank attributes1 */
	unsigned char  attr2;         /* bank attributes2 */
	unsigned int  reserved1;     /* system reserved */

} VabHdr;			/* 32 byte */


typedef struct ProgAtr {        /* Program Headdings */

	unsigned char tones;          /* # of tones */
	unsigned char mvol;           /* program volume */
	unsigned char prior;          /* program priority */
	unsigned char mode;           /* program mode */
	unsigned char mpan;           /* program pan */
	char          reserved0;      /* system reserved */
	short         attr;           /* program attribute */
	unsigned int reserved1;      /* system reserved */
	unsigned int reserved2;      /* system reserved */

} ProgAtr;			/* 16 byte */


typedef struct VagAtr {         /* VAG Tone Headdings */

	unsigned char  prior;         /* tone priority */
	unsigned char  mode;          /* play mode */
	unsigned char  vol;           /* tone volume*/
	unsigned char  pan;           /* tone panning */
	unsigned char  center;        /* center note */
	unsigned char  shift;         /* center note fine tune */
	unsigned char  min;           /* minimam note limit */
	unsigned char  max;           /* maximam note limit */
	unsigned char  vibW;          /* vibrate depth */
	unsigned char  vibT;          /* vibrate duration */
	unsigned char  porW;          /* portamento depth */
	unsigned char  porT;          /* portamento duration */
	unsigned char  pbmin;         /* under pitch bend max */
	unsigned char  pbmax;         /* upper pitch bend max */
	unsigned char  reserved1;     /* system reserved */
	unsigned char  reserved2;     /* system reserved */
	unsigned short adsr1;         /* adsr1 */
	unsigned short adsr2;         /* adsr2 */
	short          prog;          /* parent program*/
	short          vag;           /* vag reference */
	short          reserved[4];   /* system reserved */

} VagAtr;			/* 32 byte */


/*
 * Volume Structure
 */
typedef struct {
        unsigned short left;    /* L Channel */
        unsigned short right;   /* R Channel */
} SndVolume;

typedef struct SndVolume2 {
	short       left;
	short       right;
} SndVolume2;

typedef struct SndRegisterAttr {
	SndVolume2       volume;
	short            pitch;
	short            mask;
	short            addr;
	short            adsr1;
	short            adsr2;
} SndRegisterAttr;

typedef struct SndVoiceStats {
	short            vagId;
	short            vabId;
	unsigned short   pitch;
	short            note;
	short            tone;
	short            prog_num;
	short            prog_actual;
	short 		 vol;
	short 		 pan;
} SndVoiceStats;

/*
 * CallBack
 */
typedef void (*SsMarkCallbackProc)(short, short, short);


/* ----------------------------------------------------------------
 *	Proto Types
 * ---------------------------------------------------------------- */

#ifdef __cplusplus
extern "C" {
#endif
extern int sceSpu2RemoteInit( void );
extern int sceSpu2Remote( int arg, ... );
extern int sceSpu2CallbackInit( int priority );
extern int sceSpu2StreamEnvInit( SpuStEnv *st );
extern int sceSpu2StreamEnvSet( SpuStEnv *st );
extern sceSifEndFunc sceSpu2RemoteCallBack( sceSifEndFunc end_func );
#ifdef __cplusplus
}
#endif


/* ----------------------------------------------------------------
 *	End on File
 * ---------------------------------------------------------------- */
#endif /* _LIBRSPU2_H_ */
/* DON'T ADD STUFF AFTER THIS */

