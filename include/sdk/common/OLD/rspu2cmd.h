/* SCE CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 1.6
 */
/* 
 *
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                       librspu2 - rspu2cmd.h
 *                    common constant for librspu2
 *
 */

#ifndef _RSPU2CMD_H_
#define _RSPU2CMD_H_

/* ----------------------------------------------------------------
 *	CONSTANT 
 * ---------------------------------------------------------------- */

#define SPU_CB_ST_PREPARE	1
#define SPU_CB_ST_TRANS		2
#define SPU_CB_ST_FINISH	3
#define SPU_CB_DMA0		4
#define SPU_CB_DMA1		5
#define SPU_CB_IRQ		6



/* ----------------------------------------------------------------
 *	COMMAND 
 * ---------------------------------------------------------------- */

//--- Call back
#define rSpuSetTransferCallback			0x8100
#define rSpuSetIRQCallback  			0x8200
#define rSpuStSetPreparationFinishedCallback	0x8300
#define rSpuStSetTransferFinishedCallback	0x8400
#define rSpuStSetStreamFinishedCallback		0x8500
#define rSpuAutoDMASetCallback			0x8600

#define rSpuGetCommonAttr		0x6128
#define rSpuGetVoiceAttr		0x6240
#define rSpuGetReverbModeParam		0x6314
#define rSpuGetAllKeysStatus		0x6418

#define rSpuSetCommonAttr		0x7128
#define rSpuSetVoiceAttr		0x7240
#define rSpuSetReverbModeParam		0x7314
#define rSpuSetKeyOnWithAttr  		0x7440
#define rSpuSetEnv  			0x7508
#define rSpuSetMultiVoiceAttr  		0x7600

#define _rSpuInit   			0x01
#define rSpuSetCore			0x02
#define rSpuSetKey			0x05
#define rSpuSetReverb			0x06
#define rSpuClearReverbWorkArea		0x07
#define rSpuSetReverbEndAddr		0x08
#define rSpuSetReverbModeDepth		0x0a
#define rSpuSetReverbVoice		0x0b
#define rSpuSetIRQ			0x0c
#define rSpuSetIRQAddr			0x0d
#define rSpuSetTransferMode		0x0e
#define rSpuSetTransferStartAddr	0x10
#define rSpuWrite			0x11
#define rSpuWrite0			0x12
#define rSpuIsTransferCompleted		0x13
#define rSpuQuit			0x14
#define rSpuGetReverbEndAddr  		0x17
#define rSpuWritePartly  		0x18
#define rSpuInitHot  			0x19
#define rSpuIsReverbWorkAreaReserved	0x1a
#define rSpuMallocWithStartAddr		0x1b
#define rSpuRead			0x1c
#define rSpuReadDecodedData 		0x1d
#define rSpuReserveReverbWorkArea 	0x1e
#define rSpuSetMute			0x20
#define rSpuSetNoiseClock		0x21
#define rSpuSetNoiseVoice 		0x22
#define rSpuSetPitchLFOVoice 		0x23
#define rSpuStGetStatus			0x24
#define rSpuStGetVoiceStatus 		0x25
#define rSpuInitMalloc			0x0100
#define rSpuMalloc			0x0101
#define rSpuStInit			0x0200
#define rSpuStQuit			0x0201
#define rSpuStTransfer			0x0202
#define rSpuStSetCore			0x0203
#define rSpuGetCore			0x1002
#define rSpuGetIRQAddr			0x100c
#define rSpuFlush			0x1010
#define rSpuFree			0x1011
#define rSpuGetIRQ			0x1013
#define rSpuGetMute			0x1014
#define rSpuGetNoiseClock		0x1015
#define rSpuGetNoiseVoice		0x1016
#define rSpuGetPitchLFOVoice		0x1017
#define rSpuGetReverb			0x1018
#define rSpuGetReverbVoiceb		0x1019
#define rSpuGetTransferMode		0x101a
#define rSpuGetTransferStartAddr	0x101b
#define rSpuGetKeyStatus		0x101c
#define rSpuAutoDMAWrite		0x1020
#define rSpuAutoDMAStop			0x1021
#define rSpuAutoDMAGetStatus		0x1022
#define rSpuSetAutoDMAAttr		0x1023
#define rSpuSetSerialInAttr		0x1024
#define rSpuSetMultiVoiceNum		0x1030
#define rSpuSetDigitalOut		0x1031
#define rSpuSetDigialOut		0x1031
#define rSpuSetDegialOut		0x1031


#define rSsAllocateVoices  		0x400a
#define rSsBlockVoiceAllocation    	0x4001
#define rSsEnd				0x4002
#define rSsChannelMute   		0x4003
#define rSsGetActualProgFromProg    	0x4004
#define rSsGetChannelMute    		0x4005
#define rSsGetCurrentPoint    		0x4006
#define rSsGetVoiceMask    		0x4007
#define rSsInit   			0x4008
#define rSsIsEos    			0x4009
#define rSsPitchCorrect			0x4010
#define rSsPitchFromNote    		0x4011
#define rSsPlayBack    			0x4012
#define rSsQueueKeyOn    		0x4013
#define rSsQueueReverb    		0x4014
#define rSsQuit				0x4015
#define rSsSetTableSize			0x4017
#define rSsSetTickMode			0x4018
#define rSsSepClose    			0x4019
#define rSsSepOpen 			0x4020
#define rSsSepPause			0x4021
#define rSsSepPlay 			0x4022
#define rSsSepReplay 			0x4023
#define rSsSepSetAccelerando 		0x4024
#define rSsSepSetCrescendo  		0x4025
#define rSsSepSetDecrescendo 		0x4026
#define rSsSepSetRitardando 		0x4027
#define rSsSepSetVol			0x4028
#define rSsSepStop 			0x4029
#define rSsSeqGetVol			0x4030
#define rSsSeqOpen			0x4031
#define rSsSeqPause 			0x4032
#define rSsSeqPlayPtoP 			0x4033
#define rSsSeqReplay 			0x4034
#define rSsSeqSetAccelerando 		0x4035
#define rSsSeqSetCrescendo 		0x4036
#define rSsSeqSetDecrescendo		0x4037
#define rSsSeqSetRitardando		0x4038
#define rSsSeqSetNext			0x4039
#define rSsSeqCalledTbyT		0x4040
#define rSsSeqClose			0x4041
#define rSsSeqPlay			0x4042
#define rSsSeqSetVol			0x4043
#define rSsSeqSkip 			0x4044
#define rSsSeqStop 			0x4045
#define rSsSetAutoKeyOffMode    	0x4046
#define rSsSetCurrentPoint		0x4047
#define rSsSetLoop    			0x4048
#define rSsSetMono   			0x4049
#define rSsSetMVol  			0x404a
#define rSsSetNext  			0x4050
#define rSsSetReservedVoice  		0x4051
#define rSsSetStereo    		0x4052
#define rSsSetTempo    			0x4053
#define rSsSetVoiceMask    		0x4054
#define rSsStart			0x4055
#define rSsStart2    			0x4056
#define rSsUnBlockVoiceAllocation    	0x4057
#define rSsUtFlush    			0x4058
#define rSsUtGetVagAddr    		0x4059
#define rSsUtGetVagAddrFromTone    	0x4060
#define rSsUtGetVBaddrInSB    		0x4061
#define rSsVabClose			0x4062
#define rSsVabOpenHead			0x4063
#define rSsVabOpenHeadSticky		0x4070
#define rSsVabTransBodyPartly		0x4064
#define rSsVabTransCompleted		0x4065
#define rSsVabTransBody			0x4066
#define rSsVoiceCheck   		0x4067
#define rSsVoKeyOff    			0x4068
#define rSsVoKeyOn    			0x4069

/* ----------------------------------------------------------------
 *	End on File
 * ---------------------------------------------------------------- */
#endif /* _RSPU2CMD_H_ */
/* DON'T ADD STUFF AFTER THIS */
