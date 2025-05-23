////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (��MStar Confidential Information��) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

/*
 * Tuner handling routines
 */

#include <common.h>
#include <command.h>
#include <MsTypes.h>
#include <MsOS.h>
#include <apiDMX.h>
#include <tuner/Tuner.h>
#include <msAPI_Tuner.h>
#include <cofdm/COFDM_Demodulator.h>
#include <CusI2C.h>



#if 1//((CONFIG_COMMANDS&(CFG_CMD_TUNER | CFG_CMD_DEMOD)) == (CFG_CMD_TUNER | CFG_CMD_DEMOD))

static BOOLEAN Tuner_Lock(U32 u32Freq, RF_CHANNEL_BANDWIDTH eBand)
{
    MS_TP_SETTING param;

    U32 i = 0;
    BOOLEAN bLockStatus =0;

    memset(&param, 0, sizeof(MS_TP_SETTING));
    param.u32Frequency = u32Freq;
    param.enBandWidth = eBand;

    msAPI_Tuner_Tune2RfCh(&param);
    msAPI_Check_Lock_State_Init();
    printf("[%s][%d] (Freq, bandth) = (%d %d) Try Lock\n",__FILE__, __LINE__,param.u32Frequency,param.enBandWidth);
    while (1)
    {
        if (10 < i)
        {
            printf("FE un-lock\n");
            return FALSE;
        }
        printf("[%s][%d] Signal quality %d %d \n",__FILE__, __LINE__,msAPI_Tuner_GetSingalSNRPercentage(), msAPI_Tuner_GetSignalQualityPercentage());
        if((msAPI_Tuner_CheckLock(&bLockStatus, FALSE)== TRUE) && (bLockStatus == FE_LOCK))
        {
            printf("[%s][%d] (Freq, bandth) = (%d %d) Lock\n",__FILE__, __LINE__,param.u32Frequency,param.enBandWidth);
            return TRUE;//found a lock channel
        }
        MsOS_DelayTask(500);
        i++;
    }

}

/*
 * Subroutine:  do_tuner_init
 *
 * Description: Handler for 'tuner' command to initialization..
 *
 * Inputs:	argv[1] - NULL
 *
 * Return:      0:success; -1:failure
 *
 */
int do_tuner_init(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    printf ("do_tuner \n");

    //MDrv_IIC_Init();
    msI2C_init(); //jerry-cp

    printf ("msAPI_Tuner_Initialization \n");
    msAPI_Tuner_Initialization(TRUE);
    return 0;
}

/*
 * Subroutine:  do_tuner_setfreq
 *
 * Description: Handler for 'tuner' command to set up the TP parameters
 *
 * Inputs:	argv[1] - Center frequency
 *
 * Inputs:	argv[2] - Bandwidth
 *
 * Return:      0:success; -1:failure
 *
 */
int do_tuner_tuneRF(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    U32 u32Freq;
    RF_CHANNEL_BANDWIDTH eRfBandwidth;


    u32Freq     = simple_strtoul(argv[1], NULL, 10);
    eRfBandwidth  = (RF_CHANNEL_BANDWIDTH)simple_strtoul(argv[2], NULL, 10);

    printf ("do_tuner_setfreq u32Freq = %dKHz, eRfBandwidth = 0x%x \n",u32Freq,eRfBandwidth);

    if (FALSE == Tuner_Lock(u32Freq, eRfBandwidth))
    {
#if ((FRONTEND_DEMOD_TYPE == MSTAR_MSB123x_DEMOD) || (FRONTEND_DEMOD_TYPE == MSTAR_MSB1233c_DEMOD) || (FRONTEND_DEMOD_TYPE == MSTAR_MSB123xc_DEMOD))
        EN_DEVICE_DEMOD_TYPE eDemodType;
        eDemodType = (E_DEVICE_DEMOD_DVB_T == msAPI_GetDemodType()) ? E_DEVICE_DEMOD_DVB_T2 : E_DEVICE_DEMOD_DVB_T;
        if (TRUE == msAPI_SetDemodType(eDemodType))
        {
            devCOFDM_Init();  // re-load dsp code from dram to sram
            return (TRUE == Tuner_Lock(u32Freq, eRfBandwidth))?0:-1;
        }
        else
        {
            return -1;
        }
#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB122x_DEMOD)
        return (TRUE == Tuner_Lock(u32Freq, eRfBandwidth))?0:-1;

#else
        return -1;
#endif
    }
    else
    {
        return 0;
    }
}

/*
 * Subroutine:  do_tuner_setfreq_dvbc
 *
 * Description: Handler for 'tuner' command to set up the TP parameters
 *
 * Inputs:	argv[1] - Center frequency
 *
 * Inputs:	argv[2] - QAM
 *
 * Inputs:	argv[3] - Symble rate	
 *
 * Return:      0:success; -1:failure
 *
 */
int do_tuner_tuneRF_dvbc(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
#if(FRONTEND_DEMOD_TYPE == EMBEDDED_DVBC_DEMOD)
    U32 u32Freq;
	extern MS_U32 u32QAM;
	extern MS_U16 u16SymbolRate;

	u32Freq = simple_strtoul(argv[1], NULL, 10);
	u32QAM = simple_strtoul(argv[2], NULL, 10);
	u16SymbolRate = simple_strtoul(argv[3], NULL, 10);

	if (FALSE == Tuner_Lock(u32Freq, 0))
	{
		return -1;
	}
#endif
	return 0;
}

/*
 * Subroutine:  do_tuner_setfreq_dvbs
 *
 * Description: Handler for 'tuner' command to set up the TP parameters
 *
 * Inputs:	argv[1] - Center frequency
 *
 * Inputs:	argv[2] - Symble rate	
 *
 * Return:      0:success; -1:failure
 *
 */
int do_tuner_tuneRF_dvbs(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    U32 u32Freq;
	extern MS_U16 u16SymbolRate;

	u32Freq = simple_strtoul(argv[1], NULL, 10);
	u16SymbolRate = simple_strtoul(argv[2], NULL, 10);
	
	if (FALSE == Tuner_Lock(u32Freq, 0))
	{
		return -1;
	}

	return 0;
}

/*
 * Subroutine:  do_tuner_setdemodtype
 *
 * Description: Handler for 'tuner' command to set front-end type..
 *
 * Inputs:	argv[1] - demodulator type
 *
 * Return:      0:success; -1:failure
 *
 */
int do_tuner_setdemodtype(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    EN_DEVICE_DEMOD_TYPE u32DemodType;


    u32DemodType     = (EN_DEVICE_DEMOD_TYPE)simple_strtoul(argv[1], NULL, 10);

    printf ("do_tuner_setdemodtype u32DemodType = 0x%x \n",u32DemodType);
#if (FRONTEND_DEMOD_TYPE == EMBEDDED_DVBT2_DEMOD)
    if(u32DemodType == E_DEVICE_DEMOD_DVB_T2)
    {
        MS_U32 u32Addr = 0;
        if(get_value_from_env(E_MMAP_ID_PHOTO_INTER_ADR, MMAP_E_MMAP_ID_PHOTO_INTER_ADDR, &u32Addr) != 0)
        {
            return;
        }
        devCOFDM_SetDemodBuffer(u32Addr);
    }
#endif

#if ((FRONTEND_DEMOD_TYPE == MSTAR_MSB123x_DEMOD) || (FRONTEND_DEMOD_TYPE == MSTAR_MSB1233c_DEMOD) || (FRONTEND_DEMOD_TYPE == MSTAR_MSB123xc_DEMOD)||(FRONTEND_DEMOD_TYPE == MSTAR_MSB124x_DEMOD)||(FRONTEND_DEMOD_TYPE == EMBEDDED_DVBT2_DEMOD))
    return (TRUE==msAPI_SetDemodType(u32DemodType)) ? 0 : -1;
#else
    return 0;
#endif
}

int do_init_frontend(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{

    EN_DEVICE_DEMOD_TYPE u32DemodType;

    u32DemodType = (EN_DEVICE_DEMOD_TYPE)simple_strtoul(argv[1], NULL, 10);
    MS_U32 u32Frequency = (MS_U32)simple_strtoul(argv[2], NULL, 10);
    MS_U8 u8BandWidth  = (MS_U8 )simple_strtoul(argv[3], NULL, 10);

    //printf ("do_tuner_bringup u32DemodType = 0x%x u32Frequency:%ld u8BandWidth:%d\n",u32DemodType, u32Frequency, u8BandWidth+5);

#if 1
    return (TRUE == msAPI_InitFrontend(u32DemodType, u32Frequency, u8BandWidth)) ? 0 : -1;
#else
    // Test msAPI_Tuner_CheckLock API for OAD
    msAPI_InitFrontend(u32DemodType, u32Frequency, u8BandWidth);

    BOOLEAN bLockStatus =0;
    BOOLEAN bRet = msAPI_Tuner_CheckLock(&bLockStatus, FALSE);
    printf("msAPI_Tuner_CheckLock ret:%d, bLockStatus:%d\n", bRet, bLockStatus);
    return 0;
#endif
}


#include <command.h>


U_BOOT_CMD(
	tuner_init,	1,	1,	do_tuner_init,
	"tuner_init     - frondend initialization\n",
	"command - tuner_init\n"
);

U_BOOT_CMD(
	tuner_tune,	3,	1,	do_tuner_tuneRF,
	"tuner_tune     - tune RF to check lock or not\n",
	"arg[1]: center frequency(KHz)\n"
	"arg[2]: bandwidth>>1: 6MHz; 2: 7MHz; 3: 8MHz\n"
);

U_BOOT_CMD(
	tuner_tune_dvbc,	4,	1,	do_tuner_tuneRF_dvbc,
	"tuner_tune     - tune RF to check lock or not for dvbc\n",
	"arg[1]: center frequency(KHz)\n"
	"arg[2]: 16: QAM16; 32: QAM32; 64: QAM64; 128: QAM128; 256: QAM256; others: AUTO; \n"
	"arg[3]: symble rate (KHz)\n"
);

U_BOOT_CMD(
	tuner_tune_dvbs,	3,	1,	do_tuner_tuneRF_dvbs,
	"tuner_tune     - tune RF to check lock or not for dvbs\n",
	"arg[1]: center frequency(KHz)\n"
	"arg[2]: symble rate (KHz)\n"
);

U_BOOT_CMD(
	tuner_demodtype,	2,	1,	do_tuner_setdemodtype,
	"tuner_demodtype     - set frontend type\n",
	"arg[1]: 1:dvb-t; 2:dvb-t2\n"
);

U_BOOT_CMD(
    init_frontend,  4,  1,  do_init_frontend,
    "init_frontend     - init frontend\n",
    "arg[1]: 1:dvb-t; 2:dvb-t2\n"
    "arg[2]: frequency\n"
    "arg[3]: bandwidth\n"
);

#endif

