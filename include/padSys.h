#include "common.h"
#include "sdk/ee/libpad2.h"

struct t_scePad2ButtonData
{
    // u8		buttonData[SCE_PAD2_BUTTON_DATA_SIZE];	//00		Team ICO may have had this struct because they changed values in it at 104D60 or a macro was used
    u8 buttonsLo;  // 00	gets iverted and masked by t_padSysData.u100(buttonProfile) Those are either button states or masks. These are button states but are used inverted.
    u8 buttonsHi;  // 01
    u8 analogLVer; // 02	TODO: check which is which
    u8 analogLHor; // 03
    u8 analogRVer; // 04
    u8 analogRHor; // 05
    u8 u02[0xC];   // 06
}; // = 0x12

struct t_scePad2ButtonProfile
{
    // u8		buttonData[SCE_PAD2_BUTTON_PROFILE_SIZE];	//00		Team ICO may have had this struct because they changed values in it at 104D30 or a macro was used
    u8 buttonsMaskLo; // 00	Those are either button states or masks.
    u8 buttonsMaskHi; // 01
    u8 hasAnalog;     // 02	might also be 'noAnalog'
    u8 u03;           // 03	align padding?
}; // = 0x4

// Usually data structures in sotc are called "data" or "info" or "work"(area), and start with the class/lib name.
struct t_padSysData
{
    scePad2SocketParam sockParam; // 00	sz 0x20
    u32 u20[0xE0 / 4];            // 20

    struct t_scePad2ButtonProfile buttonProfile; // 100	used to mask t_scePad2ButtonData.u00	//Likley 16 buttons - one per bit
    struct t_scePad2ButtonData buttonData;       // 104	sz 0x12	//what align?
    // u8		u116_alignPad[2];	//116
    int pad2State;  // 118	 = scePad2GetState()
    int socket;     // 11C
    int gotProfile; // 120	= 0 on init	Semi-certain name; 104D30 increments;
    u32 u124;       // 124
    u32 u128;       // 128
    u32 u12C;       // 12C
    u32 u130;       // 130
    u32 u134;       // 134
    u32 u138;       // 138
    u32 u13C;       // 13C
}; //= 0x140

#define PAD_SYS_NR_PADS 2
struct t_padSysData D_0013D9C0[PAD_SYS_NR_PADS]; // 13D9C0

#define padSysData D_0013D9C0
