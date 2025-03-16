/* SCE CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library Release 3.0
 $Id: sdhd.h,v 1.24 2003/09/12 05:21:50 tokiwa Exp $
 */
/* 
 * Sound Data .HD Library
 *
 * Copyright (C) 2002 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 *
 *      Date            Design      Log
 *  ----------------------------------------------------
 *      2002-09-26      toki        initial
 *      2003-03-27      toki        change unload module
 */

#ifndef _SCE_SDHD_H
#define _SCE_SDHD_H

#ifdef __cplusplus
extern "C" {
#endif

#include <format/sound/hd.h>


/*******************************************************************************
 * define
 ******************************************************************************/

#define SCESDHD_NOT_IGNORE       ( 0 )    /*$B%5%s%W%k%;%C%H>pJs$N%Y%m%7%F%#$rL5;k(B
                                            $B$7$J$$(B*/
#define SCESDHD_IGNORE           ( 1 )    /*$B%5%s%W%k%;%C%H>pJs$N%Y%m%7%F%#$rL5;k(B
                                            $B$9$k(B*/

/*******************************************************************************
 * struct
 ******************************************************************************/

/* ***** ProgramParam ***** */
typedef struct{
    unsigned int   volume;      /*$B%W%m%0%i%`%\%j%e!<%`(B    */
    int            panpot;      /*$B%W%m%0%i%`%Q%s%]%C%H(B    */
    int            transpose;   /*$B%W%m%0%i%`%H%i%s%9%]!<%:(B*/
    int            detune;      /*$B%W%m%0%i%`%G%#%A%e!<%s(B  */
} SceSdHdProgramCommon;

/*
 * volume     0~128
 * panpot     0~64~127 = $B:8(B~$B??Cf(B~$B1&(B $B%^%$%J%9$O5UAj(B
 * transpose  $BC10L$O%N!<%H(B
 * detune     $BC10L$OH>2;$N#1#2#8EyJ,(B
 */


typedef struct{
    int            pan;        /*$B#1%*%/%?!<%VKh$KJQ2=$9$k%Q%s%]%C%H$NJQ2=I}(B*/
    unsigned int   panCenter;  /*pan$B$N8z2L$,#0$K$J$k%N!<%H%J%s%P!<(B*/
} SceSdHdProgramKeyFollow;


typedef struct{
    unsigned int   wavePitch;         /*LFO$BGH7A!J%T%C%A!K"(Cm#2(B*/
    unsigned int   waveAmp;           /*LFO$BGH7A!J%"%s%W!K"(Cm#2(B*/
    unsigned int   startPhasePitch;   /*LFO$B%9%?!<%H%U%'%$%:!J%T%C%A!K"(Cm#3(B*/
    unsigned int   startPhaseAmp;     /*LFO$B%9%?!<%H%U%'%$%:!J%"%s%W!K"(Cm#3(B*/
    unsigned int   phaseRandomPitch;  /*LFO$B%9%?!<%H%U%'%$%:%i%s%@%`!J%T%C%A!K(B*/
    unsigned int   phaseRandomAmp;    /*LFO$B%9%?!<%H%U%'%$%:%i%s%@%`!J%"%s%W!K(B*/
    unsigned int   cyclePitch;        /*LFO$B<~4|!J%T%C%A!K(B*/
    unsigned int   cycleAmp;          /*LFO$B<~4|!J%"%s%W!K(B*/
    int            pitchDepthUp;      /*$B%T%C%A%b%8%e%l!<%7%g%s?6I}!\"(Cm#1(B*/
    int            pitchDepthDown;    /*$B%T%C%A%b%8%e%l!<%7%g%s?6I}!]"(Cm#1(B*/
    int            midiPitchDepthUp;  /*MIDI$B%T%C%A%b%8%e%l!<%7%g%s?6I}!\"(Cm#1(B*/
    int            midiPitchDepthDown;/*MIDI$B%T%C%A%b%8%e%l!<%7%g%s?6I}!]"(Cm#1(B*/
    int            ampDepthUp;        /*$B%"%s%W%b%8%e%l!<%7%g%s?6I}!\(B*/
    int            ampDepthDown;      /*$B%"%s%W%b%8%e%l!<%7%g%s?6I}!](B*/
    int            midiAmpDepthUp;    /*MIDI$B%b%8%e%l!<%7%g%s?6I}!\(B*/
    int            midiAmpDepthDown;  /*MIDI$B%b%8%e%l!<%7%g%s?6I}!](B*/
} SceSdHdProgramLFO;

/*
 * $B"(Cm#1!'C10L$OH>2;$N#1#2#8EyJ,(B
 * $B"(Cm#2!'(Bhd.h$B;2>H$N;v(B
 *         SCEFORMAT_SOUND_HD_LFO_WAVEFORM_NON
 *         SCEFORMAT_SOUND_HD_LFO_WAVEFORM_SAWUP
 *         SCEFORMAT_SOUND_HD_LFO_WAVEFORM_SAWDOWN
 *         SCEFORMAT_SOUND_HD_LFO_WAVEFORM_TRIANGLE
 *         SCEFORMAT_SOUND_HD_LFO_WAVEFORM_SQUEARE
 *         SCEFORMAT_SOUND_HD_LFO_WAVEFORM_NOISE
 *         SCEFORMAT_SOUND_HD_LFO_WAVEFORM_SIN
 *         SCEFORMAT_SOUND_HD_LFO_WAVEFORM_USER
 * $B"(Cm#3!'CM$O#0!A#2#5#5$G!"#0!A#3#5#9EY$HI=$9(B
 */


typedef struct{
    unsigned int              nSplit;      /*$BFbJq$9$k(BSplitBlock$B$N8D?t(B*/
    unsigned int              progAttr;    /*$B%W%m%0%i%`%"%I%j%S%e!<%H(B*/
    SceSdHdProgramCommon      common;      /*$B%W%m%0%i%`4pK\>pJs(B*/
    SceSdHdProgramKeyFollow   keyFollow;   /*$B%W%m%0%i%`(BkeyFollow$B>pJs(B*/
    SceSdHdProgramLFO         LFO;         /*$B%W%m%0%i%`(BLFO$B>pJs(B*/
} SceSdHdProgramParam;

/*
 * progAttr   SCE_FORMAT_SOUND_HD_ROUND_PAN(0x01) : $B5UAj2D(B
 */


/* ***** SplitBlock ***** */
typedef struct{
    unsigned int    low;        /*$B2<8B%N!<%H%J%s%P!<(B*/
    unsigned int    crossFade;  /*$B%/%m%9%U%'%$%I3+;O%N!<%H%J%s%P!<(B*/
    unsigned int    high;       /*$B>e8B%N!<%H%J%s%P!<(B*/
} SceSdHdSplitRange;

typedef struct{
    unsigned int   low;   /*$B%Y%s%I%l%s%8!](B $B"((B*/
    unsigned int   high;  /*$B%Y%s%I%l%s%8!\(B $B"((B*/
} SceSdHdSplitBendRange;

/*
 * $B"(C10L$OH>2;$N#1#2#8EyJ,(B
 */

typedef struct{
    int             pitch;      /*$B#1%*%/%?!<%VKh$KJQ2=$9$k%T%C%A(BLFO$B$NJQ2=I}(B $B"((B*/
    unsigned int    pitchCenter;/*pitch$B$N8z2L$,#0$K$J$k%N!<%H%J%s%P!<(B*/
    int             amp;        /*$B#1%*%/%?!<%VKh$KJQ2=$9$k%"%s%W(BLFO$B$NJQ2=I}(B*/
    unsigned int    ampCenter;  /*amp$B$N8z2L$,#0$K$J$k%N!<%H%J%s%P!<(B*/
    int             pan;        /*$B#1%*%/%?!<%VKh$KJQ2=$9$k%Q%s%]%C%H$NJQ2=I}(B*/
    unsigned int    panCenter;  /*pan$B$N8z2L$,#0$K$J$k%N!<%H%J%s%P!<(B*/
} SceSdHdSplitKeyFollow;

typedef struct{
    unsigned int    volume;     /*$B%9%W%j%C%H%\%j%e!<%`(B*/
    int             panpot;     /*$B%9%W%j%C%H%Q%s%]%C%H(B*/
    int             transpose;  /*$B%9%W%j%C%H%H%i%s%9%]!<%:(B*/
    int             detune;     /*$B%9%W%j%C%H%G%#%A%e!<%s(B*/
} SceSdHdSplitCommon;

/*
 * volume     0~128
 * panpot     0~64~127 = $B:8(B~$B??Cf(B~$B1&(B $B%^%$%J%9$O5UAj(B
 * transpose  $BC10L$O%N!<%H(B
 * detune     $BC10L$OH>2;$N#1#2#8EyJ,(B
 */

typedef struct{
    unsigned int            sampleSetIndex; /*$B%j%s%/$9$k%5%s%W%k%;%C%H%J%s%P!<(B*/
    unsigned int            splitNumber;    /*$B%9%W%j%C%H%J%s%P!<(B*/
    SceSdHdSplitRange       range;          /*$B%9%W%j%C%H%l%s%8>pJs(B*/
    SceSdHdSplitBendRange   bendRange;      /*$B%9%W%j%C%H%Y%s%I>pJs(B*/
    SceSdHdSplitKeyFollow   keyFollow;      /*$B%9%W%j%C%H(BkeyFollow$B>pJs(B*/
    SceSdHdSplitCommon      common;         /*$B%9%W%j%C%H4pK\>pJs(B*/
} SceSdHdSplitBlock;


/* ***** SampleSetParam ***** */
typedef struct{
    unsigned int   velCurve;      /*$B%Y%m%7%F%#%+!<%V%?%$%W(B*/
    unsigned int   velLimitLow;   /*$BH/2;$9$k%Y%m%7%F%#$N2<8BCM(B*/
    unsigned int   velLimitHigh;  /*$BH/2;$9$k%Y%m%7%F%#$N>e8BCM(B*/
    unsigned int   nSample;       /*$BFbJq$9$k%5%s%W%k>pJs$N8D?t(B*/
} SceSdHdSampleSetParam;


/* ***** sample ***** */
typedef struct{
    unsigned int    low;        /*$BH/2;2<8B%Y%m%7%F%#(B*/
    unsigned int    crossFade;  /*$B%/%m%9%U%'%$%I3+;O%Y%m%7%F%#(B*/
    unsigned int    high;       /*$BH/2;>e8B%Y%m%7%F%#(B*/
} SceSdHdSampleVelRange;

typedef struct{
    int             pitch;           /*$B%Y%m%7%F%#$K$h$k%T%C%A(BLFO$B$NJQ2=I}(B $B"((B*/
    unsigned int    pitchCenter;     /*pitch$B$N8z2L$,#0$K$J$k%Y%m%7%F%#CM(B*/
    unsigned int    pitchVelCurve;   /*$B%T%C%A(BLFO$B$KBP$9$k%Y%m%7%F%#%+!<%V%?%$%W(B*/
    int             amp;             /*$B%Y%m%7%F%#$K$h$k%"%s%W(BLFO$B$NJQ2=I}(B*/
    unsigned int    ampCenter;       /*amp$B$K$h$k8z2L$,#0$K$J$k%Y%m%7%F%#CM(B*/
    unsigned int    ampVelCurve;     /*$B%"%s%W(BLFO$B$KBP$9$k%Y%m%7%F%#%+!<%V%?%$%W(B*/
} SceSdHdSampleVelFollow;

/*
 * $B"(C10L$OH>2;$N#1#2#8EyJ,(B
 */

typedef struct{
    unsigned int    baseNote;  /*$B%Y!<%9$H$J$k%N!<%H%J%s%P!<(B*/
    int             detune;    /*$B%5%s%W%k%G%#%A%e!<%s(B $B"((B*/
    int             panpot;    /*$B%5%s%W%k%Q%s%]%C%H(B*/
    unsigned int    group;     /*$B%0%k!<%W(B*/
    unsigned int    priority;  /*$B%W%i%$%*%j%F%#(B*/
    unsigned int    volume;    /*$B%5%s%W%k%\%j%e!<%`(B*/
} SceSdHdSampleCommon;

/*
 * $B"(C10L$OH>2;$N#1#2#8EyJ,(B
 */

typedef struct{
    unsigned int   ADSR1;  /*$B%(%s%Y%m!<%W(BADSR1 $B"((B*/
    unsigned int   ADSR2;  /*$B%(%s%Y%m!<%W(BADSR2 $B"((B*/
} SceSdHdSampleADSR;

/*
 * $B"(%G!<%?7A<0$O(BSystem Manual[SPU2 Overview]$B$r;2>H$N;v(B
 */

typedef struct{
    int             ar;        /*AR$B$N#1%*%/%?!<%VKh$NJQ2=I}(B*/
    unsigned int    arCenter;  /*ar$B$N8z2L$,#0$K$J$k%N!<%H%J%s%P!<(B*/
    int             dr;        /*DR$B$N#1%*%/%?!<%VKh$NJQ2=I}(B*/
    unsigned int    drCenter;  /*dr$B$N8z2L$,#0$K$J$k%N!<%H%J%s%P!<(B*/
    int             sr;        /*SR$B$N#1%*%/%?!<%VKh$NJQ2=I}(B*/
    unsigned int    srCenter;  /*sr$B$N8z2L$,#0$K$J$k%N!<%H%J%s%P!<(B*/
    int             rr;        /*RR$B$N#1%*%/%?!<%VKh$NJQ2=I}(B*/
    unsigned int    rrCenter;  /*RR$B$N8z2L$,#0$K$J$k%N!<%H%J%s%P!<(B*/
    int             sl;        /*SL$B$N#1%*%/%?!<%VKh$NJQ2=I}(B*/
    unsigned int    slCenter;  /*sl$B$N8z2L$,#0$K$J$k%N!<%H%J%s%P!<(B*/
} SceSdHdSampleKeyFollow;

typedef struct{
    unsigned int   pitchLFODelay;  /*PitchLFO$B%G%#%l%$%?%$%`(B*/
    unsigned int   pitchLFOFade;   /*PitchLFO$B%U%'%$%I%?%$%`(B*/
    unsigned int   ampLFODelay;    /*AmpLFO$B%G%#%l%$%?%$%`(B*/
    unsigned int   ampLFOFade;     /*AmpLFO$B%U%'%$%I%?%$%`(B*/
} SceSdHdSampleLFO;

typedef struct{
    int                      vagIndex;   /*$B%j%s%/$9$k(BVAGInfo$B%J%s%P!<(B*/
    unsigned int             spuAttr;    /*SPU$B$K4X$9$k(BAttribute $B"(Cm#1(B*/
    unsigned int             lfoAttr;    /*LFO$B$K4X$9$k(BAttribute $B"(Cm#2(B*/
    SceSdHdSampleVelRange    velRange;   /*$B%Y%m%7%F%#%l%s%8>pJs(B*/
    SceSdHdSampleVelFollow   velFollow;  /*$B%Y%m%7%F%#(BFollow$B>pJs(B*/
    SceSdHdSampleCommon      common;     /*$B%5%s%W%k4pK\>pJs(B*/
    SceSdHdSampleADSR        ADSR;       /*ADSR$B>pJs(B*/
    SceSdHdSampleKeyFollow   keyFollow;  /*KyuFollow$B>pJs(B*/
    SceSdHdSampleLFO         LFO;        /*LFO$B>pJs(B*/
} SceSdHdSampleParam;

/* $B"(Cm#1(B .hd$B;2>H$N;v(B
 * SCEFORMAT_SOUND_HD_LFO_KEY_ON_PITCH
 * SCEFORMAT_SOUND_HD_LFO_KEY_OFF_PITCH
 * SCEFORMAT_SOUND_HD_LFO_BOTH_PITCH
 * SCEFORMAT_SOUND_HD_LFO_KEY_ON_AMP
 * SCEFORMAT_SOUND_HD_LFO_KEY_OFF_AMP
 * SCEFORMAT_SOUND_HD_LFO_BOTH_AMP
 */

/* $B"(Cm#2(B .hd$B;2>H$N;v(B
 * SCEFORMAT_SOUND_HD_SPU_DIRECTSEND_L
 * SCEFORMAT_SOUND_HD_SPU_DIRECTSEND_R
 * SCEFORMAT_SOUND_HD_SPU_EFFECTSEND_L
 * SCEFORMAT_SOUND_HD_SPU_EFFECTSEND_R
 * SCEFORMAT_SOUND_HD_SPU_CORE_0
 * SCEFORMAT_SOUND_HD_SPU_CORE_1
 */


/* vaginfo */
typedef struct{
    unsigned int   vagOffsetAddr;  /*$BGH7A%G!<%?$N(BBD$B%U%!%$%kFb$N0LCV(B*/
    unsigned int   vagSize;        /*$BGH7A%G!<%?$N%5%$%:(B*/
    unsigned int   vagSampleRate;  /*$BGH7A%G!<%?$N%5%s%W%j%s%0%l!<%H(B*/
    unsigned int   vagAttribute;   /*$B%k!<%W%U%i%0(B $B"((B*/
} SceSdHdVAGInfoParam;

/* $B"((B.hd$B;2>H$N;v(B
 * SCE_FORMAT_SOUND_HD_VAG_ATTR_1SHOT = 1 shot
 * SCE_FORMAT_SOUND_HD_VAG_ATTR_LOOP  = Loop
 */

/* Proto Type */
int sceSdHdGetMaxProgramNumber(void *buffer);

int sceSdHdGetMaxSampleSetNumber(void *buffer);

int sceSdHdGetMaxSampleNumber(void *buffer);

int sceSdHdGetMaxVAGInfoNumber(void *buffer);

int sceSdHdGetProgramParamAddr(void *buffer, unsigned int programNumber,
			       sceHardSynthProgramParam **ptr);

int sceSdHdGetProgramParam(void *buffer, unsigned int programNumber,
			   SceSdHdProgramParam *param);

int sceSdHdGetSplitBlockAddr(void *buffer, unsigned int programNumber,
			     unsigned int splitBlockNumber,
			     sceHardSynthSplitBlock **theParamPtr);

int sceSdHdGetSplitBlock(void *buffer, unsigned int programNumber,
			 unsigned int splitBlockNumber,
			 SceSdHdSplitBlock *param);

int sceSdHdGetSampleSetParamAddr(void *buffer, unsigned int sampleSetNumber,
				 sceHardSynthSampleSetParam **ptr);

int sceSdHdGetSampleSetParam(void *buffer, unsigned int sampleSetNumber,
			     SceSdHdSampleSetParam *param);

int sceSdHdGetSampleParamAddr(void *buffer, unsigned int sampleNumber,
			      sceHardSynthSampleParam **ptr);

int sceSdHdGetSampleParam(void *buffer, unsigned int sampleNumber,
			  SceSdHdSampleParam *param);

int sceSdHdGetVAGInfoParamAddr(void *buffer, unsigned int vagInfoNumber,
			       sceHardSynthVagParam **ptr);

int sceSdHdGetVAGInfoParam(void *buffer, unsigned int vagInfoNumber,
			   SceSdHdVAGInfoParam *param);

int sceSdHdCheckProgramNumber(void *buffer, unsigned int programNumber);

int sceSdHdGetSplitBlockCountByNote(void *buffer, unsigned int programNumber,
				    unsigned int noteNumber);

int sceSdHdGetSplitBlockAddrByNote(void *buffer, unsigned int programNumber,
				   unsigned int noteNumber,
				   sceHardSynthSplitBlock **ptr);

int sceSdHdGetSplitBlockByNote(void *buffer, unsigned int programNumber,
			       unsigned int noteNumber,
			       SceSdHdSplitBlock *param);

int sceSdHdGetSampleSetParamCountByNote(void *buffer,
					unsigned int programNumber,
					unsigned int noteNumber);

int sceSdHdGetSampleSetParamAddrByNote(void *buffer, unsigned int programNumber,
				       unsigned int noteNumber, 
				       sceHardSynthSampleSetParam **ptr);

int sceSdHdGetSampleSetParamByNote(void *buffer, unsigned int programNumber, 
				   unsigned int noteNumber, 
				   SceSdHdSampleSetParam *param);

int sceSdHdGetSampleParamCountByNoteVelocity(void *buffer, 
					     unsigned int programNumber, 
					     unsigned int noteNumber, 
					     unsigned int velocity, 
					     unsigned int mode);

int sceSdHdGetSampleParamAddrByNoteVelocity(void *buffer, 
					    unsigned int programNumber, 
					    unsigned int noteNumber, 
					    unsigned int velocity, 
					    unsigned int mode, 
					    sceHardSynthSampleParam **ptr);

int sceSdHdGetSampleParamByNoteVelocity(void *buffer, 
					unsigned int programNumber, 
					unsigned int noteNumber, 
					unsigned int velocity, 
					unsigned int mode, 
					SceSdHdSampleParam *param);

int sceSdHdGetVAGInfoParamCountByNoteVelocity(void *buffer, 
					      unsigned int programNumber, 
					      unsigned int noteNumber, 
					      unsigned int velocity, 
					      unsigned int mode);

int sceSdHdGetVAGInfoParamAddrByNoteVelocity(void *buffer, 
					     unsigned int programNumber, 
					     unsigned int noteNumber, 
					     unsigned int velocity, 
					     unsigned int mode, 
					     sceHardSynthVagParam **ptr);

int sceSdHdGetVAGInfoParamByNoteVelocity(void *buffer, 
					 unsigned int programNumber, 
					 unsigned int noteNumber, 
					 unsigned int velocity, 
					 unsigned int mode, 
					 SceSdHdVAGInfoParam *param);

int sceSdHdGetSampleParamCountByVelocity(void *buffer, 
					 unsigned int sampleSetNumber, 
					 unsigned int velocity, 
					 unsigned int mode);

int sceSdHdGetSampleParamAddrByVelocity(void *buffer, 
					unsigned int sampleSetNumber, 
					unsigned int velocity, 
					unsigned int mode, 
					sceHardSynthSampleParam **ptr);

int sceSdHdGetSampleParamByVelocity(void *buffer, unsigned int sampleSetNumber,
				    unsigned int velocity, unsigned int mode, 
				    SceSdHdSampleParam *param);

int sceSdHdGetVAGInfoParamCountByVelocity(void *buffer, 
					  unsigned int sampleSetNumber, 
					  unsigned int velocity, 
					  unsigned int mode);

int sceSdHdGetVAGInfoParamAddrByVelocity(void *buffer, 
					 unsigned int sampleSetNumber, 
					 unsigned int velocity, 
					 unsigned int mode, 
					 sceHardSynthVagParam **ptr);

int sceSdHdGetVAGInfoParamByVelocity(void *buffer, unsigned int sampleSetNumber,
				     unsigned int velocity, unsigned int mode, 
				     SceSdHdVAGInfoParam *param);

int sceSdHdGetVAGInfoParamAddrBySampleNumber(void *buffer, 
					     unsigned int sampleNumber, 
					     sceHardSynthVagParam **ptr);

int sceSdHdGetVAGInfoParamBySampleNumber(void *buffer, 
					 unsigned int sampleNumber, 
					 SceSdHdVAGInfoParam *param);

int sceSdHdGetSplitBlockNumberBySplitNumber(void *buffer, 
					    unsigned int programNumber, 
					    unsigned int splitNumber);

int sceSdHdGetVAGSize(void *buffer, unsigned int vagInfoNumber);

int sceSdHdGetSplitBlockCount(void *buffer, unsigned int programNumber);

int sceSdHdGetMaxSplitBlockCount(void *buffer);

int sceSdHdGetMaxSampleSetParamCount(void *buffer);

int sceSdHdGetMaxSampleParamCount(void *buffer);

int sceSdHdGetMaxVAGInfoParamCount(void *buffer);

int sceSdHdModifyVelocity(unsigned int curveType, int velocity);

int sceSdHdModifyVelocityLFO
(unsigned int curveType, int velocity, int center);

int sceSdHdGetValidProgramNumberCount(void *buffer);

int sceSdHdGetValidProgramNumber(void *buffer, unsigned int *ptr);

int sceSdHdGetSampleNumberBySampleIndex(void *buffer,
					unsigned int sampleSetNumber,
					unsigned int sampleIndexNumber);

#ifdef __cplusplus
}
#endif

#endif /* !_SCE_SDHD_H */
