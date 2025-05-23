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
    //************************************************************
    //***             RIU Base address of each IP              ***
    //***                (Byte address [23:0])                 ***
    //************************************************************

#define    RIUBASE_RIU_DBG             0x100100    //0x100100 ~ 0x1001ff
#define    RIUBASE_VD_MHEG5            0x100300    //0x100300 ~ 0x1003ff
#define    RIUBASE_POR_STATUS          0x100500    //0x100500 ~ 0x10053f
#define    RIUBASE_INTR_CPUINT         0x100540    //0x100540 ~ 0x10057f
#define    RIUBASE_NORPF               0x100580    //0x100580 ~ 0x1005ff
#define    RIUBASE_MIU2                0x100600    //0x100600 ~ 0x1006ff
#define    RIUBASE_USB0                0x100700    //0x100700 ~ 0x10077f
#define    RIUBASE_USB1                0x100780    //0x100780 ~ 0x1007ff
#define    RIUBASE_UPLLO               0x100800    //0x100800 ~ 0x10080f
#define    RIUBASE_UPLL1               0x100820    //0x100810 ~ 0x10081f
#define    RIUBASE_BDMA                0x100900    //0x100900 ~ 0x10097f
#define    RIUBASE_UART0               0x100980    //0x100980 ~ 0x1009ff
#define    RIUBASE_CLKGEN2             0x100a00    //0x100a00 ~ 0x100aff
#define    RIUBASE_CLKGEN0             0x100b00    //0x100b00 ~ 0x100bff
#define    RIUBASE_MSPI1               0x100c00    //0x100c00 ~ 0x100cff
#define    RIUBASE_UHC1                0x100d00    //0x100d00 ~ 0x100dff
#define    RIUBASE_ACP                 0x100e00    //0x100e00 ~ 0x100eff
#define    RIUBASE_SPI_PRE_ARB         0x100f00    //0x100f00 ~ 0x100fff
#define    RIUBASE_INTR_CTRL_1         0x101000    //0x101000 ~ 0x1010ff
#define    RIUBASE_MVD                 0x101100    //0x101100 ~ 0x1011ff
#define    RIUBASE_MIU                 0x101200    //0x101200 ~ 0x1012ff
#define    RIUBASE_MSPI0               0x101300    //0x101300 ~ 0x1013ff
#define    RIUBASE_MVOP                0x101400    //0x101400 ~ 0x1014ff
#define    RIUBASE_TSP0                0x101500    //0x101500 ~ 0x1015ff
#define    RIUBASE_TSP1                0x101600    //0x101600 ~ 0x1016ff
#define    RIUBASE_HVD1                0x101700    //0x101700 ~ 0x1017ff
#define    RIUBASE_SEMAPH              0x101800    //0x101800 ~ 0x10187f
#define    RIUBASE_I3_AXI              0x101880    //0x101880 ~ 0x1018ff
#define    RIUBASE_INTR_CTRL           0x101900    //0x101900 ~ 0x1019ff
#define    RIUBASE_IDAC                0x101a00    //0x101a00 ~ 0x101aff
#define    RIUBASE_HVD                 0x101b00    //0x101b00 ~ 0x101bff
#define    RIUBASE_EVD                 0x101c00    //0x101c00 ~ 0x101cff
#define    RIUBASE_ARM                 0x101d00    //0x101d00 ~ 0x101dff
#define    RIUBASE_CHIP                0x101e00    //0x101e00 ~ 0x101eff
#define    RIUBASE_GOP                 0x101f00    //0x101f00 ~ 0x101fff
#define    RIUBASE_USB30PHY_DTOP       0x102000    //0x102000 ~ 0x1020ff
#define    RIUBASE_USB30PHY_ATOP       0x102100    //0x102100 ~ 0x1021ff
#define    RIUBASE_USB30PHY1_DTOP      0x102200    //0x102200 ~ 0x1022ff
#define    RIUBASE_USB30PHY1_ATOP      0x102300    //0x102300 ~ 0x1023ff
#define    RIUBASE_UHC0                0x102400    //0x102400 ~ 0x1024ff
#define    RIUBASE_PADTOP0             0x102500    //0x102500 ~ 0x1025ff
#define    RIUBASE_PADTOP1             0x102600    //0x102600 ~ 0x1026ff
#define    RIUBASE_EVD1                0x102700    //0x102700 ~ 0x1027ff
#define    RIUBASE_GE0                 0x102800    //0x102800 ~ 0x1028ff
#define    RIUBASE_SMART               0x102900    //0x102900 ~ 0x1029ff
#define    RIUBASE_SMART2              0x102a00    //0x102a00 ~ 0x102aff
#define    RIUBASE_SATA_GHC            0x102b00    //0x102b00 ~ 0x102bff
#define    RIUBASE_SATA_P0             0x102c00    //0x102c00 ~ 0x102cff
#define    RIUBASE_SATA_MISC           0x102d00    //0x102d00 ~ 0x102dff
#define    RIUBASE_SC0                 0x102e00    //0x102e00 ~ 0x102eff
#define    RIUBASE_SC1                 0x102f00    //0x102f00 ~ 0x102fff
#define    RIUBASE_SC2                 0x103000    //0x103000 ~ 0x1030ff
#define    RIUBASE_SC3                 0x103100    //0x103100 ~ 0x1031ff
#define    RIUBASE_SC4                 0x103200    //0x103200 ~ 0x1032ff
#define    RIUBASE_CLKGEN1             0x103300    //0x103300 ~ 0x10337f
#define    RIUBASE_MAILBOX             0x103380    //0x103380 ~ 0x1033ff
#define    RIUBASE_PCM                 0x103440    //0x103440 ~ 0x10345f
#define    RIUBASE_VDMCU51_IF          0x103460    //0x103460 ~ 0x10347f
#define    RIUBASE_PM                  0x1034a0    //0x1034a0 ~ 0x1034bf
#define    RIUBASE_URDMA               0x1034c0    //0x1034c0 ~ 0x1034ff
#define    RIUBASE_SEC_R2              0x103500    //0x103500 ~ 0x1035ff
#define    RIUBASE_SEC_MAU0            0x103600    //0x103600 ~ 0x1036ff
#define    RIUBASE_SATA_PHY            0x103700    //0x103700 ~ 0x1037ff
#define    RIUBASE_SATA_MIUPORT        0x1037c0    //0x103700 ~ 0x1037ff
#define    RIUBASE_UTMISS0             0x103800    //0x103800 ~ 0x1038ff
#define    RIUBASE_UTMISS1             0x103880    //0x103800 ~ 0x1038ff
#define    RIUBASE_UTMI2               0x103900    //0x103900 ~ 0x10397f
#define    RIUBASE_UTMI1               0x103a00    //0x103a00 ~ 0x103a7f
#define    RIUBASE_UTMI                0x103a80    //0x103a80 ~ 0x103aff
#define    RIUBASE_VE_0                0x103b00    //0x103b00 ~ 0x103bff
#define    RIUBASE_REG_PIU_NONPM       0x103c00    //0x103c00 ~ 0x103cff
#define    RIUBASE_MVOP_SUB            0x103d00    //0x103d00 ~ 0x103dff
#define    RIUBASE_VE_1                0x103e00    //0x103e00 ~ 0x103eff
#define    RIUBASE_VE_2                0x103f00    //0x103f00 ~ 0x103fff
#define    RIUBASE_INTR_CTRL4_0        0x110200    //0x110200 ~ 0x1102ff
#define    RIUBASE_INTR_CTRL4_1        0x110300    //0x110300 ~ 0x1103ff
#define    RIUBASE_UART1               0x110600    //0x110600 ~ 0x11063f
#define    RIUBASE_UART2               0x110640    //0x110640 ~ 0x11067f
#define    RIUBASE_FUART               0x110680    //0x110680 ~ 0x1106bf
#define    RIUBASE_GE1                 0x110700    //0x110700 ~ 0x1107ff
#define    RIUBASE_GPU                 0x110800    //0x110800 ~ 0x1108ff
#define    RIUBASE_ANA_MISC_EVD_MIUPLL 0x110900    //0x110900 ~ 0x1109ff
#define    RIUBASE_ANA_MISC_MIPSPLL    0x110a00    //0x110a00 ~ 0x110a7f
#define    RIUBASE_ANA_MISC_MPLLTOP    0x110b00    //0x110b00 ~ 0x110b7f
#define    RIUBASE_ANA_MISC_GMAC       0x110c00    //0x110c00 ~ 0x110c7f
#define    RIUBASE_MIU_ATOP            0x110d00    //0x110d00 ~ 0x110dff
#define    RIUBASE_GPD0                0x110e00    //0x110e00 ~ 0x110eff
#define    RIUBASE_GPD1                0x110f00    //0x110f00 ~ 0x110fff
#define    RIUBASE_MFE0                0x111000    //0x111000 ~ 0x1110ff
#define    RIUBASE_MFE1                0x111100    //0x111100 ~ 0x1111ff
#define    RIUBASE_ADC_DTOPB           0x111200    //0x111200 ~ 0x1112ff
#define    RIUBASE_VIVALDI0            0x112a00    //0x112a00 ~ 0x112aff
#define    RIUBASE_VIVALDI1            0x112b00    //0x112b00 ~ 0x112bff
#define    RIUBASE_VIVALDI2            0x112c00    //0x112c00 ~ 0x112cff
#define    RIUBASE_VIVALDI3            0x112d00    //0x112d00 ~ 0x112dff
#define    RIUBASE_VIVALDI4            0x112e00    //0x112e00 ~ 0x112eff
#define    RIUBASE_VIVALDI5            0x112f00    //0x112f00 ~ 0x112fff
#define    RIUBASE_VIVALDIC            0x113000    //0x113000 ~ 0x1130ff
