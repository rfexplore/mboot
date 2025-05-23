//<MStar Software>
//******************************************************************************
// MStar Software
// Copyright (c) 2010 - 2012 MStar Semiconductor, Inc. All rights reserved.
// All software, firmware and related documentation herein ("MStar Software") are
// intellectual property of MStar Semiconductor, Inc. ("MStar") and protected by
// law, including, but not limited to, copyright law and international treaties.
// Any use, modification, reproduction, retransmission, or republication of all
// or part of MStar Software is expressly prohibited, unless prior written
// permission has been granted by MStar.
//
// By accessing, browsing and/or using MStar Software, you acknowledge that you
// have read, understood, and agree, to be bound by below terms ("Terms") and to
// comply with all applicable laws and regulations:
//
// 1. MStar shall retain any and all right, ownership and interest to MStar
//    Software and any modification/derivatives thereof.
//    No right, ownership, or interest to MStar Software and any
//    modification/derivatives thereof is transferred to you under Terms.
//
// 2. You understand that MStar Software might include, incorporate or be
//    supplied together with third party`s software and the use of MStar
//    Software may require additional licenses from third parties.
//    Therefore, you hereby agree it is your sole responsibility to separately
//    obtain any and all third party right and license necessary for your use of
//    such third party`s software.
//
// 3. MStar Software and any modification/derivatives thereof shall be deemed as
//    MStar`s confidential information and you agree to keep MStar`s
//    confidential information in strictest confidence and not disclose to any
//    third party.
//
// 4. MStar Software is provided on an "AS IS" basis without warranties of any
//    kind. Any warranties are hereby expressly disclaimed by MStar, including
//    without limitation, any warranties of merchantability, non-infringement of
//    intellectual property rights, fitness for a particular purpose, error free
//    and in conformity with any international standard.  You agree to waive any
//    claim against MStar for any loss, damage, cost or expense that you may
//    incur related to your use of MStar Software.
//    In no event shall MStar be liable for any direct, indirect, incidental or
//    consequential damages, including without limitation, lost of profit or
//    revenues, lost or damage of data, and unauthorized system use.
//    You agree that this Section 4 shall still apply without being affected
//    even if MStar Software has been modified by MStar in accordance with your
//    request or instruction for your use, except otherwise agreed by both
//    parties in writing.
//
// 5. If requested, MStar may from time to time provide technical supports or
//    services in relation with MStar Software to you for your use of
//    MStar Software in conjunction with your or your customer`s product
//    ("Services").
//    You understand and agree that, except otherwise agreed by both parties in
//    writing, Services are provided on an "AS IS" basis and the warranty
//    disclaimer set forth in Section 4 above shall apply.
//
// 6. Nothing contained herein shall be construed as by implication, estoppels
//    or otherwise:
//    (a) conferring any license or right to use MStar name, trademark, service
//        mark, symbol or any other identification;
//    (b) obligating MStar or any of its affiliates to furnish any person,
//        including without limitation, you and your customers, any assistance
//        of any kind whatsoever, or any information; or
//    (c) conferring any license or right under any intellectual property right.
//
// 7. These terms shall be governed by and construed in accordance with the laws
//    of Taiwan, R.O.C., excluding its conflict of law rules.
//    Any and all dispute arising out hereof or related hereto shall be finally
//    settled by arbitration referred to the Chinese Arbitration Association,
//    Taipei in accordance with the ROC Arbitration Law and the Arbitration
//    Rules of the Association by three (3) arbitrators appointed in accordance
//    with the said Rules.
//    The place of arbitration shall be in Taipei, Taiwan and the language shall
//    be English.
//    The arbitration award shall be final and binding to both parties.
//
//******************************************************************************
//<MStar Software>

#include "drvNAND.h"

#if (defined(NAND_DRV_TV_MBOOT)&&NAND_DRV_TV_MBOOT)
#include "MsOS.h"


#if IF_IP_VERIFY
U8  au8_nand_clk_max;
#endif

NAND_DRIVER sg_NandDrv;
static UNFD_ALIGN0 U32 gau32_PartInfo[NAND_PARTITAION_BYTE_CNT/4]UNFD_ALIGN1;

void nand_SysPwrOff(void)
{
    nand_debug(UNFD_DEBUG_LEVEL, 1, " NO power off \n");
    nand_debug(UNFD_DEBUG_LEVEL, 1, " NO power off \n");
}

U32 nand_hw_timer_delay(U32 u32usTick)
{
    #if 0 // Use PIU timer

    U32 u32HWTimer = 0;
    volatile U16 u16TimerLow = 0;
    volatile U16 u16TimerHigh = 0;

    // reset HW timer
    REG_WRITE_UINT16(TIMER0_MAX_LOW, 0xFFFF);
    REG_WRITE_UINT16(TIMER0_MAX_HIGH, 0xFFFF);
    REG_WRITE_UINT16(TIMER0_ENABLE, 0);

    // start HW timer
    REG_SET_BITS_UINT16(TIMER0_ENABLE, 0x0001);

    while( u32HWTimer < 12*u32usTick ) // wait for u32usTick micro seconds
    {
        REG_READ_UINT16(TIMER0_CAP_LOW, u16TimerLow);
        REG_READ_UINT16(TIMER0_CAP_HIGH, u16TimerHigh);

        u32HWTimer = (u16TimerHigh<<16) | u16TimerLow;
    }

    REG_WRITE_UINT16(TIMER0_ENABLE, 0);

    #else // Use kernel udelay
    extern void udelay(unsigned long usec);
    U32 u32_us = u32usTick;

    while(u32_us>1000*20)
    {
        udelay(1000*20);
        u32_us -= 1000*20;
    }

    udelay(u32_us);
    #endif

    return u32usTick+1;
}

U32 nand_pads_release(void)
{
    return UNFD_ST_SUCCESS;
}

U32 nand_pads_init(void)
{
    NAND_DRIVER *pNandDrv = (NAND_DRIVER*)drvNAND_get_DrvContext_address();

    #if defined(CONFIG_MSTAR_TOOL_ROM_PROGRAM_NAND_BIN)
    REG_WRITE_UINT16(NC_BOOT_MODE, 0);
    REG_CLR_BITS_UINT16(NC_FUN_CTL, BIT_BOOT_MODE);
    #endif

    pNandDrv->u16_Reg40_Signal =
            (BIT_NC_WP_AUTO | BIT_NC_WP_H | BIT_NC_CE_AUTO | BIT_NC_CE_H) &
            ~(BIT_NC_CHK_RB_EDGEn | BIT_NC_CE_SEL_MASK);

    return UNFD_ST_SUCCESS;
}

#if defined(DDR_NAND_SUPPORT) && DDR_NAND_SUPPORT
U32 nand_check_DDR_pad(void)
{
    return UNFD_ST_SUCCESS;
}
#endif

U32 nand_pads_switch(U32 u32EnableFCIE)
{
    // Disable all pad in
    REG_CLR_BITS_UINT16(reg_allpad_in, BIT15);

    {
        NAND_DRIVER *pNandDrv = (NAND_DRIVER*)drvNAND_get_DrvContext_address();

        REG_CLR_BITS_UINT16(reg_emmc_config, BIT7|BIT6);                //clean EMMC config 0x5D
        REG_CLR_BITS_UINT16(reg_sd_config, BIT9|BIT8);
        REG_CLR_BITS_UINT16(reg_sdio_config, BIT5|BIT4);

        pNandDrv->u16_Reg58_DDRCtrl &= ~(BIT_DDR_MASM|BIT_SDR_DIN_FROM_MACRO|BIT_NC_32B_MODE);

        REG_WRITE_UINT16(reg_emmcpll_rx71, 0xffff);
        REG_SET_BITS_UINT16(reg_emmcpll_rx72, BIT0|BIT1);
        REG_WRITE_UINT16(reg_emmcpll_rx73, 0xffff);
        REG_SET_BITS_UINT16(reg_emmcpll_rx74, BIT0|BIT1|BIT15);
        REG_CLR_BITS_UINT16(reg_emmcpll_rx63, BIT0);

        if(NAND_PAD_BYPASS_MODE == u32EnableFCIE)
        {
            REG_CLR_BITS_UINT16(reg_emmcpll_rx60, BIT0|BIT1|BIT2);
            REG_CLR_BITS_UINT16(reg_emmcpll_rx6d, BIT0);
            REG_CLR_BITS_UINT16(reg_emmcpll_rx70, BIT8);
            NC_DQS_PULL_H();
        }
        else if(NAND_PAD_TOGGLE_MODE == u32EnableFCIE)
        {
            if(pNandDrv->u8_MacroType == MACRO_TYPE_8BIT)
            {
                REG_CLR_BITS_UINT16(reg_emmcpll_rx60, BIT0|BIT1|BIT2);
                REG_SET_BITS_UINT16(reg_emmcpll_rx60, BIT0);
                REG_SET_BITS_UINT16(reg_emmcpll_rx6d, BIT0);
                REG_CLR_BITS_UINT16(reg_emmcpll_rx70, BIT8);
                pNandDrv->u16_Reg58_DDRCtrl |= (BIT_DDR_TOGGLE|BIT_SDR_DIN_FROM_MACRO);
            }
            else if(pNandDrv->u8_MacroType == MACRO_TYPE_32BIT)
            {
                REG_WRITE_UINT16(reg_emmcpll_rx03, 0x2064);
                REG_CLR_BITS_UINT16(reg_emmcpll_rx60, BIT0|BIT1|BIT2);
                REG_SET_BITS_UINT16(reg_emmcpll_rx60, BIT0|BIT1);
                REG_CLR_BITS_UINT16(reg_emmcpll_rx6d, BIT0);
                REG_SET_BITS_UINT16(reg_emmcpll_rx70, BIT8);

                REG_WRITE_UINT16(reg_emmcpll_rx71, 0);
                REG_CLR_BITS_UINT16(reg_emmcpll_rx72, BIT0|BIT1);
                REG_WRITE_UINT16(reg_emmcpll_rx73, 0xFF00);
                REG_CLR_BITS_UINT16(reg_emmcpll_rx74, BIT15);
                REG_SET_BITS_UINT16(reg_emmcpll_rx63, BIT0);

                REG_CLR_BITS_UINT16(reg_emmcpll_rx62, BIT2|BIT1|BIT0);
                REG_CLR_BITS_UINT16(reg_emmcpll_rx6c, BIT7);
                REG_CLR_BITS_UINT16(reg_emmcpll_rx20, BIT9);

                pNandDrv->u16_Reg58_DDRCtrl |= (BIT_DDR_TOGGLE|BIT_NC_32B_MODE);
            }

            NC_DQS_PULL_L();
        }
        else if(NAND_PAD_ONFI_SYNC_MODE == u32EnableFCIE)
        {
            if(pNandDrv->u8_MacroType == MACRO_TYPE_8BIT)
            {
                REG_CLR_BITS_UINT16(reg_emmcpll_rx60, BIT0|BIT1|BIT2);
                REG_SET_BITS_UINT16(reg_emmcpll_rx60, BIT0);
                REG_SET_BITS_UINT16(reg_emmcpll_rx6d, BIT0);
                REG_CLR_BITS_UINT16(reg_emmcpll_rx70, BIT8);
                pNandDrv->u16_Reg58_DDRCtrl |= (BIT_DDR_ONFI|BIT_SDR_DIN_FROM_MACRO);
            }
            else if(pNandDrv->u8_MacroType == MACRO_TYPE_32BIT)
            {
                REG_WRITE_UINT16(reg_emmcpll_rx03, 0x0064);
                REG_CLR_BITS_UINT16(reg_emmcpll_rx60, BIT0|BIT1|BIT2);
                REG_SET_BITS_UINT16(reg_emmcpll_rx60, BIT0|BIT2);
                REG_CLR_BITS_UINT16(reg_emmcpll_rx6d, BIT0);
                REG_SET_BITS_UINT16(reg_emmcpll_rx70, BIT8);

                REG_WRITE_UINT16(reg_emmcpll_rx71, 0);
                REG_CLR_BITS_UINT16(reg_emmcpll_rx72, BIT0|BIT1);
                REG_WRITE_UINT16(reg_emmcpll_rx73, 0xFF00);
                REG_CLR_BITS_UINT16(reg_emmcpll_rx74, BIT15);
                REG_SET_BITS_UINT16(reg_emmcpll_rx63, BIT0);
                REG_CLR_BITS_UINT16(reg_emmcpll_rx62, BIT2|BIT1|BIT0);
                REG_CLR_BITS_UINT16(reg_emmcpll_rx6c, BIT7);
                REG_CLR_BITS_UINT16(reg_emmcpll_rx20, BIT9);

                pNandDrv->u16_Reg58_DDRCtrl |= (BIT_DDR_ONFI|BIT_NC_32B_MODE);
            }
            NC_DQS_PULL_L();
        }
        REG_WRITE_UINT16(NC_DDR_CTRL, pNandDrv->u16_Reg58_DDRCtrl);

    }

    REG_SET_BITS_UINT16(reg_pcm_a_pe, 0xFF);
    REG_SET_BITS_UINT16(reg_nand_ad_pe, 0xFF);
    // Let sboot determine the reg_nand_mode

    nand_debug(UNFD_DEBUG_LEVEL_LOW, 0,"reg_nf_en(%08X)=%04X\n", reg_nf_en, REG(reg_nf_en));
    nand_debug(UNFD_DEBUG_LEVEL_LOW, 0,"reg_allpad_in(%08X)=%04X\n", reg_allpad_in, REG(reg_allpad_in));

    return UNFD_ST_SUCCESS;
}


#if (defined(DDR_EMMC_PLL) && DDR_EMMC_PLL)
void nand_pll_clock_setting(U32 u32EmmcClkParam)
{
    EMMC_PLL_SETTINGS sEmmcPLLSetting[EMMC_PLL_1XCLK_TABLE_CNT] = EMMC_PLL_CLK_TABLE;

    // Reset eMMC_PLL
    REG_SET_BITS_UINT16(REG_EMMC_PLL_RX06, BIT0);
    REG_CLR_BITS_UINT16(REG_EMMC_PLL_RX06, BIT0);

    // Synth clock
    REG_WRITE_UINT16(REG_EMMC_PLL_RX18, sEmmcPLLSetting[u32EmmcClkParam].emmc_pll_1xclk_rx18);
    REG_WRITE_UINT16(REG_EMMC_PLL_RX19, sEmmcPLLSetting[u32EmmcClkParam].emmc_pll_1xclk_rx19);

    // VCO clock
    REG_WRITE_UINT16(REG_EMMC_PLL_RX04, 0x0006);

    // 1X clock
    REG_CLR_BITS_UINT16(REG_EMMC_PLL_RX05, EMMC_PLL_1XCLK_RX05_MASK);
    REG_SET_BITS_UINT16(REG_EMMC_PLL_RX05, sEmmcPLLSetting[u32EmmcClkParam].emmc_pll_1xclk_rx05);

    if( u32EmmcClkParam )
        REG_CLR_BITS_UINT16(REG_EMMC_PLL_RX07, BIT10);
    else
        REG_SET_BITS_UINT16(REG_EMMC_PLL_RX07, BIT10);

    // Wait 100us
    udelay(1000);
}

void nand_skew_clock_setting(void)
{
    // Skew clock setting
//    REG_WRITE_UINT16(REG_EMMC_PLL_RX03, 0x0040);
}

void nand_dll_setting(void)
{
    volatile U16 u16_reg;
    REG_CLR_BITS_UINT16(REG_EMMC_PLL_RX09, BIT0);

    // Reset eMMC_DLL
    REG_SET_BITS_UINT16(REG_EMMC_PLL_RX30, BIT2);
    REG_CLR_BITS_UINT16(REG_EMMC_PLL_RX30, BIT2);

    //DLL pulse width and phase
    REG_WRITE_UINT16(REG_EMMC_PLL_RX01, 0x7F72);

    // DLL code
    REG_WRITE_UINT16(REG_EMMC_PLL_RX32, 0xF200);

    // DLL calibration
    REG_WRITE_UINT16(REG_EMMC_PLL_RX30, 0x3378);
    REG_SET_BITS_UINT16(REG_EMMC_PLL_RX33, BIT15);

    // Wait 100us
    udelay(1000);

    // Get hw dll0 code
    REG_READ_UINT16(REG_EMMC_PLL_RX33, u16_reg);

    REG_CLR_BITS_UINT16(REG_EMMC_PLL_RX34, (BIT10 - 1));
    // Set dw dll0 code
    REG_SET_BITS_UINT16(REG_EMMC_PLL_RX34, u16_reg & 0x03FF);

    // Disable reg_hw_upcode_en
    REG_CLR_BITS_UINT16(REG_EMMC_PLL_RX30, BIT9);

    // Clear reg_emmc_dll_test[7]
    REG_CLR_BITS_UINT16(REG_EMMC_PLL_RX02, BIT15);

    // Enable reg_rxdll_dline_en
    REG_SET_BITS_UINT16(REG_EMMC_PLL_RX09, BIT0);

}
#endif

U32 nand_clock_setting(U32 u32ClkParam)
{
    NAND_DRIVER *pNandDrv = (NAND_DRIVER*)drvNAND_get_DrvContext_address();

    REG_CLR_BITS_UINT16(reg_ckg_fcie, BIT2|BIT3|BIT4|BIT5);

    REG_SET_BITS_UINT16(reg_ckg_fcie, BIT_CLK_ENABLE);
    REG_CLR_BITS_UINT16(reg_ckg_fcie, BIT1|BIT0);

    if( pNandDrv->u16_Reg58_DDRCtrl & BIT_DDR_MASM )
    {
        switch(u32ClkParam)
        {
            case NFIE_CLK_12M: nand_pll_clock_setting(EMMC_PLL_1XCLK_12M_IDX); break;
            case NFIE_CLK_20M: nand_pll_clock_setting(EMMC_PLL_1XCLK_20M_IDX); break;
            case NFIE_CLK_32M: nand_pll_clock_setting(EMMC_PLL_1XCLK_32M_IDX); break;
            case NFIE_CLK_36M: nand_pll_clock_setting(EMMC_PLL_1XCLK_36M_IDX); break;
            case NFIE_CLK_40M: nand_pll_clock_setting(EMMC_PLL_1XCLK_40M_IDX); break;
            case NFIE_CLK_43M: nand_pll_clock_setting(EMMC_PLL_1XCLK_43M_IDX); break;
            case NFIE_CLK_48M: nand_pll_clock_setting(EMMC_PLL_1XCLK_48M_IDX); break;
            case NFIE_CLK_54M: nand_pll_clock_setting(EMMC_PLL_1XCLK_54M_IDX); break;
            case NFIE_CLK_62M: nand_pll_clock_setting(EMMC_PLL_1XCLK_62M_IDX); break;
            case NFIE_CLK_72M: nand_pll_clock_setting(EMMC_PLL_1XCLK_72M_IDX); break;
            case NFIE_CLK_86M: nand_pll_clock_setting(EMMC_PLL_1XCLK_86M_IDX); break;
            //case NFIE_CLK_100M: nand_pll_clock_setting(EMMC_PLL_1XCLK_100M_IDX); break;
            default:
                nand_die();
                break;
        }

        REG_SET_BITS_UINT16(reg_ckg_fcie, NFIE_CLK_2X_P_DDR);

        nand_skew_clock_setting();
        nand_dll_setting();
    }
    else
        REG_SET_BITS_UINT16(reg_ckg_fcie, u32ClkParam);

    REG_CLR_BITS_UINT16(reg_ckg_fcie_syn, NFIE_CLK_SYNC_MASK);
    REG_SET_BITS_UINT16(reg_ckg_fcie_syn, NFIE_CLK_SYNC_DEFAULT);
    //nand_debug(0,1,"%Xh \n", u32ClkParam);


    /*** ECC Clock ***/
    #ifndef CONFIG_MSTAR_TOOL_ROM_PROGRAM_NAND_BIN
    REG_CLR_BITS_UINT16(reg_ckg_ecc, (ECC_CLK_MASK|ECC_CLK_GATING|ECC_CLK_INVERSE));
    REG_SET_BITS_UINT16(reg_ckg_ecc, ECC_SW_DEFAULT_CLK);
    #endif

    return UNFD_ST_SUCCESS;
}


void nand_DumpPadClk(void)
{
    U16 u16_i;

    nand_debug(0, 1, "clk setting: \n");
    nand_debug(0, 1, "  reg_ckg_fcie(0x%X):0x%x\n", reg_ckg_fcie, REG(reg_ckg_fcie));

    nand_debug(0, 1, "pad setting: \n");
    //fcie pad register
    nand_debug(0, 1, "  NC_DDR_CTRL(0x%X):0x%x\n", NC_DDR_CTRL, REG(NC_DDR_CTRL));

    //chiptop pad register
    nand_debug(0, 1, "  reg_all_pad_in(0x%X):0x%x\n", reg_allpad_in, REG(reg_allpad_in));

    //platform depend reg_sd_use_bypass
    nand_debug(0, 1, "  reg_pcm_a_pe(0x%08X): 0x%04X\n", reg_pcm_a_pe, REG(reg_pcm_a_pe));
    nand_debug(0, 1, "  reg_nand_ad_pe(0x%08X): 0x%04X\n", reg_nand_ad_pe, REG(reg_nand_ad_pe));
    nand_debug(0, 1, "  reg_sd_config(0x%08X): 0x%04X\n", reg_sd_config, REG(reg_sd_config));
    nand_debug(0, 1, "  reg_emmc_config(0x%08X): 0x%04X\n", reg_emmc_config, REG(reg_emmc_config));

    nand_debug(0, 1, "  reg_nf_en(0x%X):0x%x\n\n", reg_nf_en, REG(reg_nf_en));

    // -------------------------------------------
    nand_debug(0, 1, "chiptop:");
    for(u16_i=0 ; u16_i<0x60 ; u16_i++)
    {
        if(0 == u16_i%8)
            nand_debug(0,0, "\n%02Xh:| ", u16_i);

        nand_debug(0, 0, "%04Xh ", REG(GET_REG_ADDR(CHIPTOP_REG_BASE_ADDR, u16_i)));
        if((u16_i & 0x7) == 0x7)
            nand_debug(0,0,"| %02Xh ", u16_i);
    }
    nand_debug(0, 0,"\n\n");

}


#if defined(DECIDE_CLOCK_BY_NAND) && DECIDE_CLOCK_BY_NAND
#define MAX_UNFD(a,b) ((a) > (b) ? (a) : (b))

U32 nand_config_timing(U16 u16_1T)
{
    NAND_DRIVER *pNandDrv = (NAND_DRIVER*)drvNAND_get_DrvContext_address();
    U16 u16_DefaultTRR;
    U16 u16_DefaultTCS;
    U16 u16_DefaultTWW;
    U16 u16_DefaultRX40Cmd;
    U16 u16_DefaultRX40Adr;
    U16 u16_DefaultRX56;
    U16 u16_DefaultTADL;
    U16 u16_DefaultTCWAW;
    #if defined(NC_TWHR_TCLHZ) && NC_TWHR_TCLHZ
    U16 u16_DefaultTCLHZ = 4;
    #endif
    U16 u16_DefaultTWHR;
    U16 u16_Tmp, u16_Cnt;
    U16 u16_Tmp2, u16_Cnt2;


    #if defined(DDR_NAND_SUPPORT) && DDR_NAND_SUPPORT
    if(pNandDrv->u16_Reg58_DDRCtrl&BIT_DDR_ONFI)
    {
        u16_DefaultTRR = NC_ONFI_DEFAULT_TRR;
        u16_DefaultTCS = NC_ONFI_DEFAULT_TCS;
        u16_DefaultTWW = NC_ONFI_DEFAULT_TWW;
        u16_DefaultRX40Cmd = NC_ONFI_DEFAULT_RX40CMD;
        u16_DefaultRX40Adr = NC_ONFI_DEFAULT_RX40ADR;
        u16_DefaultRX56 = NC_ONFI_DEFAULT_RX56;
        u16_DefaultTADL = NC_ONFI_DEFAULT_TADL;
        u16_DefaultTCWAW = NC_ONFI_DEFAULT_TCWAW;
        u16_DefaultTWHR = NC_ONFI_DEFAULT_TWHR;
    }
    else if(pNandDrv->u16_Reg58_DDRCtrl&BIT_DDR_TOGGLE)
    {
        u16_DefaultTRR = NC_TOGGLE_DEFAULT_TRR;
        u16_DefaultTCS = NC_TOGGLE_DEFAULT_TCS;
        u16_DefaultTWW = NC_TOGGLE_DEFAULT_TWW;
        u16_DefaultRX40Cmd = NC_TOGGLE_DEFAULT_RX40CMD;
        u16_DefaultRX40Adr = NC_TOGGLE_DEFAULT_RX40ADR;
        u16_DefaultRX56 = NC_TOGGLE_DEFAULT_RX56;
        u16_DefaultTADL = NC_TOGGLE_DEFAULT_TADL;
        u16_DefaultTCWAW = NC_TOGGLE_DEFAULT_TCWAW;
        u16_DefaultTWHR = NC_TOGGLE_DEFAULT_TWHR;
    }
    else
    #endif
    {
        u16_DefaultTRR = NC_SDR_DEFAULT_TRR;
        u16_DefaultTCS = NC_SDR_DEFAULT_TCS;
        u16_DefaultTWW = NC_SDR_DEFAULT_TWW;
        u16_DefaultRX40Cmd = NC_SDR_DEFAULT_RX40CMD;
        u16_DefaultRX40Adr = NC_SDR_DEFAULT_RX40ADR;
        u16_DefaultRX56 = NC_SDR_DEFAULT_RX56;
        u16_DefaultTADL = NC_SDR_DEFAULT_TADL;
        u16_DefaultTCWAW = NC_SDR_DEFAULT_TCWAW;
        u16_DefaultTWHR = NC_SDR_DEFAULT_TWHR;
    }

    #if defined(NC_INST_DELAY) && NC_INST_DELAY
    // Check CMD_END
    u16_Tmp = MAX_UNFD(pNandDrv->u16_tWHR, pNandDrv->u16_tCWAW);
    u16_Cnt = (u16_Tmp+u16_1T-1)/u16_1T;

    if(u16_DefaultRX40Cmd >= u16_Cnt)
        u16_Cnt = 0;
    else if(u16_Cnt-u16_DefaultRX40Cmd > 0xFF)
        return UNFD_ST_ERR_INVALID_PARAM;
    else
        u16_Cnt -= u16_DefaultRX40Cmd;

    // Check ADR_END
    u16_Tmp2 = MAX_UNFD(MAX_UNFD(pNandDrv->u16_tWHR, pNandDrv->u16_tADL), pNandDrv->u16_tCCS);
    u16_Cnt2 = (u16_Tmp2+u16_1T-1)/u16_1T;

    if(u16_DefaultRX40Adr >= u16_Cnt2)
        u16_Cnt2 = 0;
    else if(u16_Cnt2-u16_DefaultRX40Adr > 0xFF)
        return UNFD_ST_ERR_INVALID_PARAM;
    else
        u16_Cnt2 -= u16_DefaultRX40Adr;

    // get the max cnt
    u16_Cnt = MAX_UNFD(u16_Cnt, u16_Cnt2);

    pNandDrv->u16_Reg40_Signal &= ~(0x00FF<<8);
    pNandDrv->u16_Reg40_Signal |= (u16_Cnt<<8);
    nand_debug(UNFD_DEBUG_LEVEL_HIGH,1, "u16_Reg40_Signal =  %X\n",pNandDrv->u16_Reg40_Signal);
    #endif

    #if defined(NC_HWCMD_DELAY) && NC_HWCMD_DELAY
    u16_Cnt = (pNandDrv->u16_tRHW+u16_1T-1)/u16_1T + 2;

    if(u16_DefaultRX56 >= u16_Cnt)
        u16_Cnt = 0;
    else if(u16_Cnt-u16_DefaultRX56 > 0xFF)
        return UNFD_ST_ERR_INVALID_PARAM;
    else
        u16_Cnt -= u16_DefaultRX56;

    pNandDrv->u16_Reg56_Rand_W_Cmd &= ~(0x00FF<<8);
    pNandDrv->u16_Reg56_Rand_W_Cmd |= (u16_Cnt<<8);
    nand_debug(UNFD_DEBUG_LEVEL_HIGH,1, "u16_Reg56_Rand_W_Cmd =  %X\n",pNandDrv->u16_Reg56_Rand_W_Cmd);
    #endif

    #if defined(NC_TRR_TCS) && NC_TRR_TCS
    u16_Cnt = (pNandDrv->u8_tRR+u16_1T-1)/u16_1T + 2;

    if(u16_DefaultTRR >= u16_Cnt)
        u16_Cnt = 0;
    else if(u16_Cnt-u16_DefaultTRR > 0x0F)
        return UNFD_ST_ERR_INVALID_PARAM;
    else
        u16_Cnt -= u16_DefaultTRR;

    u16_Tmp = (pNandDrv->u8_tCS+u16_1T-1)/u16_1T + 2;

    if(u16_DefaultTCS >= u16_Tmp)
        u16_Tmp = 0;
    else if(u16_Tmp-u16_DefaultTCS > 0x0F)
        return UNFD_ST_ERR_INVALID_PARAM;
    else
        u16_Tmp -= u16_DefaultTCS;

    u16_Tmp2 = (pNandDrv->u16_tWW+u16_1T-1)/u16_1T + 2;

    if(u16_DefaultTWW >= u16_Tmp2)
        u16_Tmp2 = 0;
    else if(u16_Tmp2-u16_DefaultTWW > 0x0F)
        return UNFD_ST_ERR_INVALID_PARAM;
    else
        u16_Tmp2 -= u16_DefaultTWW;

    u16_Cnt2 = MAX_UNFD(u16_Tmp, u16_Tmp2);

    pNandDrv->u16_Reg59_LFSRCtrl &= ~(0x00FF);
    pNandDrv->u16_Reg59_LFSRCtrl |= (u16_Cnt|(u16_Cnt2<<4));
    nand_debug(UNFD_DEBUG_LEVEL_HIGH,1, "u16_Reg59_LFSRCtrl =  %X\n",pNandDrv->u16_Reg59_LFSRCtrl);
    #endif

    #if defined(NC_TCWAW_TADL) && NC_TCWAW_TADL
    u16_Cnt = (pNandDrv->u16_tADL + u16_1T - 1) / u16_1T + 2;

    if(u16_DefaultTADL > u16_Cnt)
        u16_Cnt = 0;
    else if(u16_Cnt-u16_DefaultTADL > 0xFF)
        return UNFD_ST_ERR_INVALID_PARAM;
    else
        u16_Cnt -= u16_DefaultTADL;

    u16_Cnt2 = (pNandDrv->u16_tCWAW + u16_1T - 1) / u16_1T + 2;

    if(u16_DefaultTADL > u16_Cnt2)
        u16_Cnt2 = 0;
    else if(u16_Cnt2-u16_DefaultTCWAW > 0xFF)
        return UNFD_ST_ERR_INVALID_PARAM;
    else
        u16_Cnt2 -= u16_DefaultTCWAW;

    pNandDrv->u16_Reg5D_tCWAW_tADL &= ~(0xFFFF);
    pNandDrv->u16_Reg5D_tCWAW_tADL |= (u16_Cnt|(u16_Cnt2<<8));
    nand_debug(UNFD_DEBUG_LEVEL_HIGH,1, "u16_Reg5D_tCWAW_tADL =  %X\n",pNandDrv->u16_Reg5D_tCWAW_tADL);
    #endif

    #if defined(NC_TWHR_TCLHZ) && NC_TWHR_TCLHZ
    u16_Cnt = (pNandDrv->u8_tCLHZ + u16_1T - 1) / u16_1T + 2;

    if(u16_DefaultTCLHZ > u16_Cnt)
        u16_Cnt = 0;
    else if(u16_Cnt-u16_DefaultTCLHZ > 0xF)
        return UNFD_ST_ERR_INVALID_PARAM;
    else
        u16_Cnt -= u16_DefaultTCLHZ;

    u16_Cnt2 = (pNandDrv->u16_tWHR + u16_1T - 1) / u16_1T + 2;

    if(u16_DefaultTWHR > u16_Cnt2)
        u16_Cnt2 = 0;
    else if(u16_Cnt2-u16_DefaultTWHR > 0xFF)
        return UNFD_ST_ERR_INVALID_PARAM;
    else
        u16_Cnt2 -= u16_DefaultTWHR;

    pNandDrv->u16_Reg5A_tWHR_tCLHZ &= ~(0xFFFF);
    pNandDrv->u16_Reg5A_tWHR_tCLHZ |= ((u16_Cnt&0xF)|(u16_Cnt2<<8));
    nand_debug(UNFD_DEBUG_LEVEL_HIGH,1, "u16_Reg5A_tWHR_tCLHZ =  %X\n",pNandDrv->u16_Reg5A_tWHR_tCLHZ);
    #endif

    NC_Config();
    return UNFD_ST_SUCCESS;
}

U32 nand_find_timing(U8 *pu8_ClkIdx, U8 u8_find_DDR_timg)
{
    NAND_DRIVER *pNandDrv = (NAND_DRIVER*)drvNAND_get_DrvContext_address();
    U32 au32_1TTable[NFIE_CLK_TABLE_CNT] = NFIE_1T_TABLE;
    U32 au32_ClkValueTable[NFIE_CLK_TABLE_CNT] = NFIE_CLK_VALUE_TABLE;

    #if defined(DDR_NAND_SUPPORT) && DDR_NAND_SUPPORT
    U32 au32_4Clk1TTable[NFIE_4CLK_TABLE_CNT] = NFIE_4CLK_1T_TABLE;
    U32 au32_4ClkValueTable[NFIE_4CLK_TABLE_CNT] = NFIE_4CLK_VALUE_TABLE;
    #endif
    U16 u16_i, u16_j, u16_pass_begin = 0xFF, u16_pass_cnt = 0;
    U16 u16_nandstatus;
    U32 u32_Err;
    U8 au8_ID[NAND_ID_BYTE_CNT];
    U32 au32_ClkTable[NFIE_CLK_TABLE_CNT] = NFIE_CLK_TABLE;
    U32 u32_Clk;
    U16 u16_SeqAccessTime, u16_Tmp, u16_Tmp2, u16_1T, u16_RE_LATCH_DELAY;
    S8 s8_ClkIdx;

    s8_ClkIdx = 0;
    u16_1T = 0;

    if(pNandDrv->u16_Reg58_DDRCtrl&BIT_DDR_ONFI)
    {
        u16_SeqAccessTime = 10;
    }
    else if(pNandDrv->u16_Reg58_DDRCtrl&BIT_DDR_TOGGLE)
    {
        u16_Tmp = MAX_UNFD(MAX_UNFD(2*pNandDrv->u8_tRP, 2*pNandDrv->u8_tREH), pNandDrv->u16_tRC);
        u16_Tmp2 = MAX_UNFD(MAX_UNFD(pNandDrv->u8_tWP, pNandDrv->u8_tWH), (pNandDrv->u16_tWC+1)/2);
        u16_SeqAccessTime = MAX_UNFD(u16_Tmp, u16_Tmp2);
    }
    else
    {
        u16_Tmp = MAX_UNFD(MAX_UNFD(pNandDrv->u8_tRP, pNandDrv->u8_tREH), (pNandDrv->u16_tRC+1)/2);
        u16_Tmp2 = MAX_UNFD(MAX_UNFD(pNandDrv->u8_tWP, pNandDrv->u8_tWH), (pNandDrv->u16_tWC+1)/2);
        u16_SeqAccessTime = MAX_UNFD(u16_Tmp, u16_Tmp2);

        u16_Tmp = (pNandDrv->u8_tREA + NAND_SEQ_ACC_TIME_TOL)/2;
        u16_Tmp2 = u16_SeqAccessTime;
        u16_SeqAccessTime = MAX_UNFD(u16_Tmp, u16_Tmp2);
    }

    u32_Clk = 1000000000/((U32)u16_SeqAccessTime); //FCIE5 needs to be update .....

    if(!u8_find_DDR_timg)
    {
        for(s8_ClkIdx =  0; s8_ClkIdx <= NFIE_CLK_TABLE_CNT - 1; s8_ClkIdx ++)
        {
            if(u32_Clk <= au32_ClkValueTable[s8_ClkIdx])
            {
                break;
            }
        }
    }
    else
    {
        #if defined(DDR_NAND_SUPPORT) && DDR_NAND_SUPPORT
        for(s8_ClkIdx =  0; s8_ClkIdx <= NFIE_4CLK_TABLE_CNT - 1; s8_ClkIdx ++)
        {
            if(u32_Clk < au32_4ClkValueTable[s8_ClkIdx])
            {
                break;
            }
        }
        #endif
    }
    s8_ClkIdx --;


RETRY:
    if(s8_ClkIdx<0)
        return UNFD_ST_ERR_INVALID_PARAM;
    if(!u8_find_DDR_timg)
        u16_1T = au32_1TTable[s8_ClkIdx];
    #if defined(DDR_NAND_SUPPORT) && DDR_NAND_SUPPORT
    else
        u16_1T = au32_4Clk1TTable[s8_ClkIdx];
    #endif

    if(nand_config_timing(u16_1T) != UNFD_ST_SUCCESS)
    {
        s8_ClkIdx--;
        goto RETRY;
    }

    if (!u8_find_DDR_timg)
    {
        NC_ReadStatus();
        REG_READ_UINT16(NC_ST_READ, u16_nandstatus);

        pNandDrv->u32_Clk = au32_ClkTable[s8_ClkIdx];
        nand_clock_setting(pNandDrv->u32_Clk);

        //using read id to detect relatch
        memcpy(au8_ID, pNandDrv->au8_ID, NAND_ID_BYTE_CNT);
        for(u16_i = 0 ; u16_i < 16 ; u16_i ++)
        {

            pNandDrv->u16_Reg57_RELatch &= ~(BIT_NC_LATCH_DATA_MASK|BIT_NC_PAD_SEL_FAILLING);
            if(u16_i &1)    //select falling edge otherwise rising edge is selected
                pNandDrv->u16_Reg57_RELatch|=BIT_NC_PAD_SEL_FAILLING;
            pNandDrv->u16_Reg57_RELatch |= ((u16_i/2) << 1) &BIT_NC_LATCH_DATA_MASK;

            NC_Config();
            u32_Err = NC_ReadID();
            if(u32_Err != UNFD_ST_SUCCESS)
            {
                nand_debug(0, 1, "ReadID Error with ErrCode 0x%lX\n", u32_Err);
                nand_die();
            }
            for(u16_j = 0; u16_j < NAND_ID_BYTE_CNT; u16_j++)
            {
                if(pNandDrv->au8_ID[u16_j] != au8_ID[u16_j])
                {
                    break;
                }
            }
            if(u16_j == NAND_ID_BYTE_CNT)
            {
                if(u16_pass_begin == 0xFF)
                    u16_pass_begin = u16_i;
                u16_pass_cnt ++;
            }
            //  break;
        }

        if(u16_pass_cnt == 0)
        {
            nand_debug(0, 1, "Read ID detect timing fails\n");
            pNandDrv->u16_Reg57_RELatch = BIT_NC_LATCH_DATA_2_0_T|BIT_NC_LATCH_STS_2_0_T;
            NC_Config();
            s8_ClkIdx = 0;
            *pu8_ClkIdx = (U8)s8_ClkIdx;
            memcpy(pNandDrv->au8_ID, au8_ID, NAND_ID_BYTE_CNT);
            return UNFD_ST_SUCCESS;
        }
        else
        {
            u16_i = u16_pass_begin + u16_pass_cnt/2;
            pNandDrv->u16_Reg57_RELatch &= ~(BIT_NC_LATCH_DATA_MASK|BIT0);
            if(u16_i &1)    //select falling edge otherwise rising edge is selected
                pNandDrv->u16_Reg57_RELatch|=BIT0;
            pNandDrv->u16_Reg57_RELatch |= ((u16_i/2) << 1) &BIT_NC_LATCH_DATA_MASK;
            memcpy(pNandDrv->au8_ID, au8_ID, NAND_ID_BYTE_CNT);
        }

        //detect read status

        u16_pass_begin = 0xFF;
        u16_pass_cnt = 0;

        for(u16_i = 0 ; u16_i < 8 ; u16_i ++)
        {
            U16 u16_tmpStatus;
            pNandDrv->u16_Reg57_RELatch &= ~(BIT_NC_LATCH_STS_MASK);
            pNandDrv->u16_Reg57_RELatch |= ((u16_i) << 5) & BIT_NC_LATCH_STS_MASK;

            NC_Config();
            NC_ReadStatus();
            REG_READ_UINT16(NC_ST_READ, u16_tmpStatus);

            if(u16_tmpStatus == u16_nandstatus)
            {
                if(u16_pass_begin == 0xFF)
                    u16_pass_begin = u16_i;
                u16_pass_cnt ++;
            }
        }
        if(u16_pass_cnt == 0)
        {
            nand_debug(0, 1, "Read status detect timing fails\n");
            pNandDrv->u16_Reg57_RELatch = BIT_NC_LATCH_DATA_2_0_T|BIT_NC_LATCH_STS_2_0_T;
            NC_Config();
            s8_ClkIdx = 0;
            *pu8_ClkIdx = (U8)s8_ClkIdx;

            return UNFD_ST_SUCCESS;
        }
        else
        {
            u16_i = u16_pass_begin + u16_pass_cnt/2;
            pNandDrv->u16_Reg57_RELatch &= ~(BIT_NC_LATCH_STS_MASK);
            pNandDrv->u16_Reg57_RELatch |= ((u16_i) << 5) & BIT_NC_LATCH_STS_MASK;
        }
        u16_RE_LATCH_DELAY = pNandDrv->u16_Reg57_RELatch;
    }

    *pu8_ClkIdx = (U8)s8_ClkIdx;

    return UNFD_ST_SUCCESS;

}

#if (defined(DDR_NAND_SUPPORT) && DDR_NAND_SUPPORT)

U8 gu8_unfd_disable_uart = 0;

static const U8 aau8_max_delay_cell[NFIE_CLK_TABLE_CNT][DQS_MODE_TABLE_CNT] =
{
    {31, 31, 31, 0}, // 12M
    {31, 31, 12, 12}, // 20M
    {31, 17, 17, 0}, // 27M
    {27, 17, 0, 0}, // 36M
    {24, 15, 0, 0}, // 40M
    {19,  4, 4, 0}, // 48M
    {4, 1, 1, 7}, // 54M
    {16, 0, 0, 0}, // 72M
    {16, 0, 0, 0}, // 108M

};
static const U8 aau8_half_delay_cell[NFIE_CLK_TABLE_CNT][DQS_MODE_TABLE_CNT] =
{
    {18, 15, 15, 0}, // 12M
    {18, 15, 6, 6}, // 20M
    {18, 9, 9, 0}, // 27M
    {16, 9, 0, 0}, // 36M
    {15, 6, 6, 0}, // 40M
    {13, 8, 0, 0}, // 48M
    {12, 2, 2, 0}, // 54M
    {10, 0, 0, 0}, // 72M
    {7,  0, 0, 0}, // 108M
};

#define MIN_PASS_CNT    3

static U16 *au16_WritePageBuf;
static U16 *au16_ReadPageBuf;
static U8  au8_ReadSpareBuf[512];

static U32 u32_TestBlk;

static U32 nand_prepare_test_pattern(void)
{
    NAND_DRIVER *pNandDrv = (NAND_DRIVER*)drvNAND_get_DrvContext_address();
    U32 u32_Err;
    U16 i, j, u16_pattern_size;
    U32 au32_ClkTable[NFIE_4CLK_TABLE_CNT] = NFIE_4CLK_TABLE;
    U32 au32_1TTable[NFIE_4CLK_TABLE_CNT] = NFIE_4CLK_1T_TABLE;

    // prepare test pattern
    u16_pattern_size = pNandDrv->u16_SectorByteCnt/8/2;
    for(i=0 ; i<8 ; i++)
    {
        switch(i)
        {
            case 0:
                for(j=0 ; j<u16_pattern_size ; j++)
                    au16_WritePageBuf[i*u16_pattern_size + j] = 0x0000;
                break;
            case 1:
                for(j=0 ; j<u16_pattern_size ; j++)
                    au16_WritePageBuf[i*u16_pattern_size + j] = 0xFFFF;
                break;
            case 2:
                for(j=0 ; j<u16_pattern_size ; j++)
                    au16_WritePageBuf[i*u16_pattern_size + j] = 0x0FF0;
                break;
            case 3:
                for(j=0 ; j<u16_pattern_size ; j++)
                    au16_WritePageBuf[i*u16_pattern_size + j] = 0xFF00;
                break;
            case 4:
                for(j=0 ; j<u16_pattern_size ; j++)
                    au16_WritePageBuf[i*u16_pattern_size + j] = 0x55AA;
                break;
            case 5:
                for(j=0 ; j<u16_pattern_size ; j++)
                    au16_WritePageBuf[i*u16_pattern_size + j] = 0x5A5A;
                break;
            case 6:
                for(j=0 ; j<u16_pattern_size*2 ; j++)
                    ((U8 *)au16_WritePageBuf)[i*u16_pattern_size*2 + j] = j;
                break;
            case 7:
                for(j=0 ; j<u16_pattern_size*2 ; j++)
                    ((U8 *)au16_WritePageBuf)[i*u16_pattern_size*2 + j] = 0xFF-j;
                break;
            default:
                break;
        }
    }
    //nand_debug(UNFD_DEBUG_LEVEL, 0, "dump write buffer-------------------------------\n");
    //dump_mem((unsigned char *) au16_WritePageBuf, pNandDrv->u16_SectorByteCnt);

    pNandDrv->u32_Clk = au32_ClkTable[pNandDrv->tMinDDR.u8_ClkIdx];
    nand_clock_setting(pNandDrv->u32_Clk);

    NC_FCIE5SetInterface(1, pNandDrv->tMinDDR.u8_DqsMode, pNandDrv->tMinDDR.u8_DelayCell, pNandDrv->tMinDDR.u8_DdrTiming);

    nand_config_timing(au32_1TTable[pNandDrv->tMinDDR.u8_ClkIdx]);

    // write pattern to test blk
    for(u32_TestBlk=9 ; u32_TestBlk>=5 ; u32_TestBlk--)
    {
        #if defined(FCIE_LFSR) && FCIE_LFSR
        if(pNandDrv->u8_RequireRandomizer == 1)
            NC_DisableLFSR();
        #endif

        if(!drvNAND_IsGoodBlk(u32_TestBlk))
        {
            continue;
        }

        #if defined(FCIE_LFSR) && FCIE_LFSR
        if(pNandDrv->u8_RequireRandomizer == 1)
            NC_EnableLFSR();
        #endif

        u32_Err = NC_EraseBlk(u32_TestBlk*pNandDrv->u16_BlkPageCnt);
        if(u32_Err != UNFD_ST_SUCCESS)
        {
            nand_debug(UNFD_DEBUG_LEVEL_HIGH, 0, "Err, NAND, bad blk=%lX\n", u32_TestBlk);
            drvNAND_MarkBadBlk(u32_TestBlk);
            continue;
        }

        u32_Err = NC_WriteSectors(u32_TestBlk*pNandDrv->u16_BlkPageCnt, 0, (U8 *)au16_WritePageBuf, NULL, 1);
        if(u32_Err != UNFD_ST_SUCCESS)
        {
            nand_debug(UNFD_DEBUG_LEVEL_HIGH, 0, "Err, NAND, write fail, blk=%lX\n", u32_TestBlk);
            continue;
        }

        u32_Err = NC_ReadSectors(u32_TestBlk*pNandDrv->u16_BlkPageCnt, 0, (U8 *)au16_ReadPageBuf, au8_ReadSpareBuf,1);//NULL, 1);
        if(u32_Err != UNFD_ST_SUCCESS)
        {
            nand_debug(UNFD_DEBUG_LEVEL_HIGH, 0, "Err, NAND, read fail, blk=%lX\n", u32_TestBlk);
            continue;
        }

        if(memcmp((const void *) au16_ReadPageBuf, (const void *) au16_WritePageBuf, pNandDrv->u16_SectorByteCnt) == 0)
            break;

    }
    //nand_debug(UNFD_DEBUG_LEVEL, 0, "\n");
    //dump_mem((unsigned char *) au8_ReadSpareBuf, pNandDrv->u16_SectorSpareByteCnt);
    if(u32_TestBlk<5)
    {
        nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Err, NAND, too many bad blk\n");
        nand_die();
        return UNFD_ST_ERR_LACK_BLK;
    }

    return UNFD_ST_SUCCESS;
}

static U32 nand_probe_ddr(void)
{
    NAND_DRIVER *pNandDrv = (NAND_DRIVER*)drvNAND_get_DrvContext_address();
    U32 u32_Err;
    Disable_Err_log = 1;
    u32_Err = NC_ReadSectors(u32_TestBlk*pNandDrv->u16_BlkPageCnt, 0, (U8 *)au16_ReadPageBuf, au8_ReadSpareBuf,1);//NULL, 1);

    if(u32_Err == UNFD_ST_SUCCESS)
    {
        if(memcmp((const void *) au16_ReadPageBuf, (const void *) au16_WritePageBuf, pNandDrv->u16_SectorByteCnt) == 0)
        {
            Disable_Err_log = 0;
            return UNFD_ST_SUCCESS;
        }
    }
    Disable_Err_log = 0;

    return UNFD_ST_ERR_NO_TOGGLE_DDR_TIMING;
}

U32 NC_FCIE5SetInterface(U8 u8_IfDDR, U8 u8_dqs_mode, U8 u8_dqs_delaycell, U8 u8_rd_ddr_timing)
{
    NAND_DRIVER *pNandDrv = (NAND_DRIVER*)drvNAND_get_DrvContext_address();
    volatile U32 u32_Err = UNFD_ST_SUCCESS;

    if(u8_IfDDR)
    {
        //for DS Mode
        pNandDrv->u16_EmmcPllReg09_PhaseSel &= ~(BIT_EMMC_RXDLL_PHASE_SEL_MASK|BIT1);
        if(u8_dqs_mode & 1)
            pNandDrv->u16_EmmcPllReg09_PhaseSel |= BIT1;

        pNandDrv->u16_EmmcPllReg09_PhaseSel |= (u8_dqs_mode/2)<<BIT_EMMC_RXDLL_PHASE_SEL_SHIFT;
        REG_CLR_BITS_UINT16(REG_EMMC_PLL_RX09, BIT_EMMC_RXDLL_PHASE_SEL_MASK|BIT1);
        REG_SET_BITS_UINT16(REG_EMMC_PLL_RX09, pNandDrv->u16_EmmcPllReg09_PhaseSel & (BIT_EMMC_RXDLL_PHASE_SEL_MASK|BIT1));

        // walk around timing bug
        pNandDrv->u16_Reg57_RELatch &= ~(BIT_NC_LATCH_DATA_MASK|BIT_NC_LATCH_STS_MASK|BIT_RE_SEC_TURN_CNT_MASK);
        pNandDrv->u16_Reg57_RELatch |= 0xE << BIT_RE_SEC_TURN_CNT_SHIFT;
        REG_WRITE_UINT16(NC_LATCH_DATA, pNandDrv->u16_Reg57_RELatch);
    }
    else
    {
        nand_pads_switch(NAND_PAD_BYPASS_MODE);
        u32_Err = NC_ResetNandFlash(); // switch ONFI to ASync Mode
    }

    return u32_Err;
}


U32 nand_detect_ddr_timing(void)
{
    NAND_DRIVER *pNandDrv = (NAND_DRIVER*)drvNAND_get_DrvContext_address();
    U32 u32_Err;
    DDR_TIMING_GROUP_t *ptDDR;
    U8 u8_2Ch_dqsmode, /*u8_2Ch_dqsmode_retry,*/ u8_delaycell;
    U8 u8_pass_cnt;
    const U8 au8_dqsmode[DQS_MODE_TABLE_CNT] = DQS_MODE_SEARCH_TABLE;   // defined in platform-depedent .h
    U32 au32_ClkTable[NFIE_4CLK_TABLE_CNT] = NFIE_4CLK_TABLE;
    U32 au32_1TTable[NFIE_4CLK_TABLE_CNT] = NFIE_4CLK_1T_TABLE;
    char *Clk4StrTable[NFIE_4CLK_TABLE_CNT] = NFIE_4CLK_TABLE_STR;
    U8 u8_DqsPassCnt;
    U32 u32_DqsMatch;
    S8 s8_ClkIdx;

    ptDDR = (DDR_TIMING_GROUP_t *)malloc(DQS_MODE_TABLE_CNT*sizeof(DDR_TIMING_GROUP_t));
    au16_WritePageBuf = (U16 *)malloc(pNandDrv->u16_SectorByteCnt);
    au16_ReadPageBuf = (U16 *)malloc(pNandDrv->u16_SectorByteCnt);
    if(!ptDDR || !au16_WritePageBuf || !au16_ReadPageBuf)
    {
        free(ptDDR);
        free(au16_WritePageBuf);
        free(au16_ReadPageBuf);
        nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Err, NAND, cannot allocate memory\n");
        nand_die();
        return UNFD_ST_ERR_INVALID_ADDR;
    }

    u32_Err = nand_prepare_test_pattern();
    if(u32_Err < UNFD_ST_SUCCESS)
    {
        NC_EraseBlk(u32_TestBlk*pNandDrv->u16_BlkPageCnt);
        free(ptDDR);
        free(au16_WritePageBuf);
        free(au16_ReadPageBuf);
        nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Err, NAND, cannot prepare test pattern\n");
        nand_die();
        return UNFD_ST_ERR_LACK_BLK;
    }

    /**************** search default/retry ddr timing *********************/
    nand_debug(UNFD_DEBUG_LEVEL, 0,"*** Try to detect default/retry ddr timing ***\r\n");
    u32_Err = nand_find_timing((U8*)&s8_ClkIdx, 1);

    if(u32_Err != UNFD_ST_SUCCESS)
    {
        free(ptDDR);
        free(au16_WritePageBuf);
        free(au16_ReadPageBuf);
        nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Err, NAND, Cannot config nand timing\n");
        nand_die();
        return(u32_Err);
    }
    #if IF_IP_VERIFY
    s8_ClkIdx =au8_nand_clk_max;
    #endif
    for( ; s8_ClkIdx>=0 ; s8_ClkIdx--) // search from high-speed clk
    {
        u32_DqsMatch = 0;
        u8_DqsPassCnt = 0;
        pNandDrv->u32_Clk = au32_ClkTable[s8_ClkIdx];
        nand_debug(UNFD_DEBUG_LEVEL, 0," **********************************************\r\n");
        nand_debug(UNFD_DEBUG_LEVEL, 0," CLK[%lX]:%sHz\r\n", au32_ClkTable[s8_ClkIdx], Clk4StrTable[s8_ClkIdx]);
        nand_debug(UNFD_DEBUG_LEVEL, 0," **********************************************\r\n");
        nand_clock_setting(pNandDrv->u32_Clk);
        if(nand_config_timing(au32_1TTable[s8_ClkIdx]) != UNFD_ST_SUCCESS)
        {
            nand_debug(UNFD_DEBUG_LEVEL, 0,"Cannot match nand timing @clk=%lX\r\n", pNandDrv->u32_Clk);
            continue;
        }

        u8_delaycell = 0;
        for(u8_2Ch_dqsmode=0 ; u8_2Ch_dqsmode<DQS_MODE_TABLE_CNT ; u8_2Ch_dqsmode++)
        {
            NC_FCIE5SetInterface(1, au8_dqsmode[u8_2Ch_dqsmode], u8_delaycell, 0);
            if(UNFD_ST_SUCCESS == nand_probe_ddr())
            {
                ptDDR[u8_2Ch_dqsmode].u8_ClkIdx = s8_ClkIdx;
                ptDDR[u8_2Ch_dqsmode].u8_DqsMode = au8_dqsmode[u8_2Ch_dqsmode];
                ptDDR[u8_2Ch_dqsmode].u8_DelayCell = u8_delaycell;
                ptDDR[u8_2Ch_dqsmode].u8_DdrTiming = 0;
                u32_DqsMatch |= (1<<u8_2Ch_dqsmode);
                u8_DqsPassCnt++;
                nand_debug(UNFD_DEBUG_LEVEL, 0,"clk[%lX], Dll_Phase_Sel[%u], DelayCell[%u], ddr_timing[%u]\r\n",
                            pNandDrv->u32_Clk, u8_2Ch_dqsmode, u8_delaycell, 0);
            }
            else
            {
                NC_ResetFCIE();
                NC_ReConfig();
            }
        }
        Disable_Err_log=0;
        u8_2Ch_dqsmode = 0;

        if(u8_DqsPassCnt >= 2)
        {
            U8 u8_DelayCellLen, u8_DelayCellMaxLen;
            U8  u8_DelayCellBegin, u8_DelayCellMaxBegin;
            //find longest sequence
            u8_DelayCellLen = 0;
            u8_DelayCellBegin = 0;
            u8_DelayCellMaxBegin = 0;
            u8_DelayCellMaxLen = 0;
            for(u8_2Ch_dqsmode = 1; u8_2Ch_dqsmode <  DQS_MODE_TABLE_CNT; u8_2Ch_dqsmode ++)
            {
                if(((u32_DqsMatch >> u8_2Ch_dqsmode) & 0x1) == 0x1)
                {
                    u8_pass_cnt = 1;
                    if(u8_DelayCellLen == 0)
                        u8_DelayCellBegin = u8_2Ch_dqsmode;
                    u8_DelayCellLen ++;
                    if(u8_2Ch_dqsmode == (DQS_MODE_TABLE_CNT - 1))
                    {
                        if(u8_DelayCellMaxLen < u8_DelayCellLen)
                        {
                            u8_DelayCellMaxBegin = u8_DelayCellBegin;
                            u8_DelayCellMaxLen = u8_DelayCellLen;
                        }
                    }
                }
                else
                {
                    if(u8_DelayCellMaxLen == 0)
                    {
                        u8_DelayCellMaxBegin = u8_DelayCellBegin;
                        u8_DelayCellMaxLen = u8_DelayCellLen;
                    }
                    else
                    {
                        if(u8_DelayCellMaxLen < u8_DelayCellLen)
                        {
                            u8_DelayCellMaxBegin = u8_DelayCellBegin;
                            u8_DelayCellMaxLen = u8_DelayCellLen;
                        }
                    }
                    u8_DelayCellLen = 0;
                }
            }

            u8_2Ch_dqsmode = u8_DelayCellMaxBegin + (u8_DelayCellMaxLen >> 1);
        }
        else
        {
            u8_pass_cnt = 0;
            for(u8_2Ch_dqsmode = 0; u8_2Ch_dqsmode <  DQS_MODE_TABLE_CNT; u8_2Ch_dqsmode ++)
                if(((u32_DqsMatch >> u8_2Ch_dqsmode) & 0x1) == 0x1)
                {
                    u8_pass_cnt = 1;
                    break;
                }
        }

        if(u8_pass_cnt != 0)
        {
            pNandDrv->tDefaultDDR.u8_ClkIdx = ptDDR[u8_2Ch_dqsmode].u8_ClkIdx;
            pNandDrv->tDefaultDDR.u8_DqsMode = ptDDR[u8_2Ch_dqsmode].u8_DqsMode;
            pNandDrv->tDefaultDDR.u8_DelayCell = ptDDR[u8_2Ch_dqsmode].u8_DelayCell;
            pNandDrv->tDefaultDDR.u8_DdrTiming = ptDDR[u8_2Ch_dqsmode].u8_DdrTiming;

            nand_debug(UNFD_DEBUG_LEVEL,1,"ok, get default DDR timing: clk[%X], Dll_Phase_Sel[%u], DelayCell[%u], ddr_timing[%u]\r\n",
                    pNandDrv->tDefaultDDR.u8_ClkIdx, pNandDrv->tDefaultDDR.u8_DqsMode, pNandDrv->tDefaultDDR.u8_DelayCell,
                    pNandDrv->tDefaultDDR.u8_DdrTiming);

            nand_debug(UNFD_DEBUG_LEVEL,1,"ok, get retry DDR timing: clk[%X], Dll_Phase_Sel[%u], DelayCell[%u], ddr_timing[%u]\r\n",
                    pNandDrv->tMinDDR.u8_ClkIdx, pNandDrv->tMinDDR.u8_DqsMode, pNandDrv->tMinDDR.u8_DelayCell,
                    pNandDrv->tMinDDR.u8_DdrTiming);
            break;
        }
    }

    if(s8_ClkIdx < 0)
    {
        nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Warning, NAND, can't detect any DDR timing\n");
        nand_die();
    }

    pNandDrv->u32_Clk = au32_ClkTable[pNandDrv->tDefaultDDR.u8_ClkIdx];
    pNandDrv->u32_minClk = au32_ClkTable[pNandDrv->tMinDDR.u8_ClkIdx];
    nand_clock_setting(pNandDrv->u32_Clk);
    nand_config_timing(au32_1TTable[pNandDrv->tDefaultDDR.u8_ClkIdx]);

    NC_FCIE5SetInterface(1, pNandDrv->tDefaultDDR.u8_DqsMode, pNandDrv->tDefaultDDR.u8_DelayCell , pNandDrv->tDefaultDDR.u8_DdrTiming);

    #if defined(FCIE_LFSR) && FCIE_LFSR
    // nand_prepare_test_pattern enable randomizer
    if(pNandDrv->u8_RequireRandomizer == 1)
        NC_DisableLFSR();
    #endif

    NC_EraseBlk(u32_TestBlk*pNandDrv->u16_BlkPageCnt);
    free(ptDDR);
    free(au16_WritePageBuf);
    free(au16_ReadPageBuf);

    return UNFD_ST_SUCCESS;
}
#endif
#endif

// [nand_config_clock] the entry for clock auto-config (by DECIDE_CLOCK_BY_NAND)
// [nand_find_timing]   find a FCIE clock
// [nand_config_timing] set other auxiliary parameters (cycle count)
//                                if timing parameters in nni are 0, would use default cycle count.
U32 nand_config_clock(U16 u16_SeqAccessTime)
{
    #if defined(DECIDE_CLOCK_BY_NAND) && DECIDE_CLOCK_BY_NAND
    NAND_DRIVER * pNandDrv = drvNAND_get_DrvContext_address();
    U32 u32_Err = 0;
    U32 au32_ClkTable[NFIE_CLK_TABLE_CNT] = NFIE_CLK_TABLE;
    char *ClkStrTable[NFIE_CLK_TABLE_CNT] = NFIE_CLK_TABLE_STR;

    #if defined(DDR_NAND_SUPPORT) && DDR_NAND_SUPPORT
    U32 au32_4ClkTable[NFIE_4CLK_TABLE_CNT] = NFIE_4CLK_TABLE;
    char *Clk4StrTable[NFIE_4CLK_TABLE_CNT] = NFIE_4CLK_TABLE_STR;
    U32 au32_1TTable[NFIE_4CLK_TABLE_CNT] = NFIE_4CLK_1T_TABLE;
    #endif

    U8 u8_ClkIdx = 0;

    #if (defined(DDR_NAND_SUPPORT) && DDR_NAND_SUPPORT)     //DDR enable of FCIE5
    if(pNandDrv->u16_Reg58_DDRCtrl&BIT_DDR_MASM)
    {
        if(pNandDrv->tDefaultDDR.u8_DqsMode == 0 && pNandDrv->tDefaultDDR.u8_ClkIdx == 0)
        {
            nand_detect_ddr_timing();
        }
        else
        {
            pNandDrv->u32_minClk = au32_4ClkTable[pNandDrv->tMinDDR.u8_ClkIdx];
            pNandDrv->u32_Clk = au32_4ClkTable[pNandDrv->tDefaultDDR.u8_ClkIdx];
            NC_FCIE5SetInterface(1, pNandDrv->tDefaultDDR.u8_DqsMode,
                pNandDrv->tDefaultDDR.u8_DelayCell, pNandDrv->tDefaultDDR.u8_DdrTiming);

            if(nand_config_timing(au32_1TTable[pNandDrv->tDefaultDDR.u8_ClkIdx]) != UNFD_ST_SUCCESS)
            {
                nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Err, NAND, Cannot config nand timing\n");
                nand_die();
                return(u32_Err);
            }
        }
        nand_debug(UNFD_DEBUG_LEVEL,1,"ok, get default DDR timing: RXdll :%X, 57h:%X\n",
                    pNandDrv->tDefaultDDR.u8_DqsMode, pNandDrv->u16_Reg57_RELatch);
        u8_ClkIdx = pNandDrv->tDefaultDDR.u8_ClkIdx;
        printf("FCIE is set to %sHz\n", Clk4StrTable[u8_ClkIdx]);
    }
    else
    #endif
    {
        u32_Err = nand_find_timing(&u8_ClkIdx, 0);
        if(u32_Err != UNFD_ST_SUCCESS)
        {
            nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Err, NAND, Cannot config nand timing\n");
            nand_die();
            return(u32_Err);
        }
        pNandDrv->u32_Clk = au32_ClkTable[u8_ClkIdx];
        printf("FCIE is set to %sHz\n", ClkStrTable[u8_ClkIdx]);
    }

    nand_clock_setting(pNandDrv->u32_Clk);
    REG_WRITE_UINT16(NC_LATCH_DATA, pNandDrv->u16_Reg57_RELatch);
    //nand_debug(UNFD_DEBUG_LEVEL, 0,"reg_ckg_fcie(%08X)=%08X\n", reg_ckg_fcie, REG(reg_ckg_fcie));
    //printf("RE LATCH is set to %X\n", pNandDrv->u16_Reg57_RELatch);

  // timing defined by
  //  - nand_clock_setting
  //  - default auxiliary timing parameters
  #endif

  return UNFD_ST_SUCCESS;
}


void nand_set_WatchDog(U8 u8_IfEnable)
{
  // do nothing in Linux
}

void nand_reset_WatchDog(void)
{
  // do nothing in Linux
}

U32 nand_translate_DMA_address_Ex(U32 u32_DMAAddr, U32 u32_ByteCnt, int mode)
{
    flush_cache(u32_DMAAddr, u32_ByteCnt);

    REG_CLR_BITS_UINT16(NC_MIU_CTRL, BIT_MIU_SEL_MASK);

    if((MsOS_VA2PA(u32_DMAAddr) & CONFIG_SYS_MIU_INTERVAL) == CONFIG_SYS_MIU_INTERVAL) //MIU0
    {
        //printf("MIU1: %lXh\n", u32_DMAAddr);
        // MIU1
        REG_SET_BITS_UINT16(NC_MIU_CTRL, BIT_MIU1_SELECT);
        return MsOS_VA2PA(u32_DMAAddr) - CONFIG_SYS_MIU_INTERVAL;
    }
    else
    {   //printf("MIU0: %lXh\n", u32_DMAAddr);
        // MIU0
        return MsOS_VA2PA(u32_DMAAddr);
    }
}

U32 nand_translate_Spare_DMA_address_Ex(U32 u32_DMAAddr, U32 u32_ByteCnt, int mode)
{
    flush_cache(u32_DMAAddr, u32_ByteCnt);

    REG_CLR_BITS_UINT16(NC_MIU_CTRL, BIT_SPARE_MIU_SEL_MASK);

    if((MsOS_VA2PA(u32_DMAAddr) & CONFIG_SYS_MIU_INTERVAL) == CONFIG_SYS_MIU_INTERVAL)
    {
        // MIU1
        REG_SET_BITS_UINT16(NC_MIU_CTRL, BIT_SPARE_MIU1_SELECT);
        return MsOS_VA2PA(u32_DMAAddr) - CONFIG_SYS_MIU_INTERVAL;
    }
    else // MIU0
    {
        return MsOS_VA2PA(u32_DMAAddr);
    }
}

U32 nand_translate_DMA_MIUSel(U32 u32_DMAAddr, U32 u32_ByteCnt)
{
    if((MsOS_VA2PA(u32_DMAAddr) & CONFIG_SYS_MIU_INTERVAL) == CONFIG_SYS_MIU_INTERVAL)      //miu1
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void nand_flush_miu_pipe(void)
{

}

void nand_read_dma_post_flush(U32 u32_DMAAddr, U32 u32_ByteCnt)
{

}

void *drvNAND_get_DrvContext_address(void) // exposed API
{
  return &sg_NandDrv;
}

void *drvNAND_get_DrvContext_PartInfo(void)
{
    return (void*)((U32)gau32_PartInfo);
}

static UNFD_ALIGN0 U32 drvNandPageBuf[8192] UNFD_ALIGN1; /* 8192*4 Bytes */
static UNFD_ALIGN0 U32 drvNandSpareBuf[640] UNFD_ALIGN1; /*   640*4 Bytes */

U32 NC_PlatformResetPre(void)
{
  //HalMiuMaskReq(MIU_CLT_FCIE);
  return UNFD_ST_SUCCESS;
}

U32 NC_PlatformResetPost(void)
{
    U16 u16_Reg;

    REG_WRITE_UINT16(NC_FUN_CTL, BIT_NC_EN);

    REG_READ_UINT16(FCIE_NC_WBUF_CIFD_BASE, u16_Reg); // dummy read for WCIFD clock
    REG_READ_UINT16(FCIE_NC_WBUF_CIFD_BASE, u16_Reg); // dummy read for WCIFD clock

    REG_READ_UINT16(FCIE_NC_RBUF_CIFD_BASE, u16_Reg); // dummy read for RCIFD clock twice said by designer
    REG_READ_UINT16(FCIE_NC_RBUF_CIFD_BASE, u16_Reg); // dummy read for RCIFD clock twice said by designer

    return UNFD_ST_SUCCESS;
}


U32 NC_PlatformInit(void)
{
    NAND_DRIVER *pNandDrv = (NAND_DRIVER*)drvNAND_get_DrvContext_address();

    nand_pads_init();
    pNandDrv->u8_WordMode = 0; // TV/Set-Top Box projects did not support x16 NAND flash
    nand_pads_switch(NAND_PAD_BYPASS_MODE);
    pNandDrv->u8_PadMode = NAND_PAD_BYPASS_MODE;

    pNandDrv->u8_MacroType = MACRO_TYPE_32BIT;
    pNandDrv->u32_Clk =FCIE3_SW_SLOWEST_CLK;
    nand_clock_setting(FCIE3_SW_SLOWEST_CLK);
    pNandDrv->u16_Reg57_RELatch = BIT_NC_LATCH_DATA_2_0_T|BIT_NC_LATCH_STS_2_0_T;
    REG_WRITE_UINT16(NC_LATCH_DATA, pNandDrv->u16_Reg57_RELatch);

    pNandDrv->pu8_PageDataBuf= (U8*)drvNandPageBuf;
    pNandDrv->pu8_PageSpareBuf = (U8*)drvNandSpareBuf;

    return UNFD_ST_SUCCESS;
}

void HWTimer_Start(void)
{
    // reset HW timer
    REG_WRITE_UINT16(TIMER0_MAX_LOW, 0xFFFF);
    REG_WRITE_UINT16(TIMER0_MAX_HIGH, 0xFFFF);
    REG_WRITE_UINT16(TIMER0_ENABLE, 0);

    // start HW timer
    REG_SET_BITS_UINT16(TIMER0_ENABLE, 0x0001);

}

U32 HWTimer_End(void)
{
    U32 u32HWTimer = 0;
    U16 u16TimerLow = 0;
    U16 u16TimerHigh = 0;

    // Get HW timer
    REG_READ_UINT16(TIMER0_CAP_LOW, u16TimerLow);
    REG_READ_UINT16(TIMER0_CAP_HIGH, u16TimerHigh);

    u32HWTimer = (u16TimerHigh<<16) | u16TimerLow;

    return u32HWTimer;
}

#else
  #error "Error! no platform functions."
#endif
