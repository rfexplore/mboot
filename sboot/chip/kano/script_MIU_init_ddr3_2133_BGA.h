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
#define _RV32_2(a, b)    { (a) << 1, b }

#if defined(MIU1_LEN) && (MIU1_LEN != 0)
#define ENABLE_MIU1 1
#else
#define ENABLE_MIU1 0
#endif

    // MIU0 Software reset
    {CHIPCFG_CMD_BASE, 0x1f20},
    _RV32_2(0x121e, 0x0c00),
    _RV32_2(0x121e, 0x0c00),
    _RV32_2(0x121e, 0x0c00),
    _RV32_2(0x121e, 0x0c01),
    
    // MIU0 Request mask 
    _RV32_2(0x1246, 0xFFFE),
    _RV32_2(0x1266, 0xFFFF),
    _RV32_2(0x1286, 0xFFFF),
    _RV32_2(0x12A6, 0xFFFF),
    {CHIPCFG_CMD_BASE, 0x1f2C},
    _RV32_2(0x1506, 0xFFFF),
    _RV32_2(0x1526, 0xFFFF),
    _RV32_2(0x15e6, 0x00F8), // High Way
    
#if (ENABLE_MIU1)
    {CHIPCFG_CMD_BASE, 0x1f20},
    //MIU1 Software reset
    _RV32_2(0x061e, 0x0000),
    _RV32_2(0x061e, 0x0000),
    _RV32_2(0x061e, 0x0000),
    _RV32_2(0x061e, 0x0c01),
    _RV32_2(0x061e, 0x0c00),
    
    //MIU1 Request mask
    _RV32_2(0x0646, 0xFFFE),
    _RV32_2(0x0666, 0xFFFF),
    _RV32_2(0x0686, 0xFFFF),
    _RV32_2(0x06A6, 0xFFFF),
    {CHIPCFG_CMD_BASE, 0x1f2C},
    _RV32_2(0x2206, 0xFFFF),
    _RV32_2(0x2226, 0xFFFF),
    _RV32_2(0x22e6, 0x00F8), // High Way
#endif
    {CHIPCFG_CMD_BASE, 0x1f22},
    _RV32_2(0x0d78, 0x0001),
    _RV32_2(0x0d24, 0x1000),
    _RV32_2(0x0d24, 0x0000),
    _RV32_2(0x0d36, 0x0400),
    _RV32_2(0x0d34, 0x2004),
    _RV32_2(0x0d30, 0x2eeb),
    _RV32_2(0x0d32, 0x001a),

    // DDR LDO power down off
    _RV32_2(0x0d8a, 0x0001),

    //set MIU0 SSC enable
    _RV32_2(0x0d28, 0xc011),
    _RV32_2(0x0d2a, 0x0482),

    {CHIPCFG_CMD_WAIT, 1},       // delay 1ms

    _RV32_2(0x0d22, 0x0004),
    _RV32_2(0x0d2c, 0x0114),

    {CHIPCFG_CMD_WAIT, 1},       // delay 1ms

    {CHIPCFG_STR_IFAC,0x0},
    {CHIPCFG_CMD_BASE, 0x1f20},
    _RV32_2(0x1202, 0x03a7),
    _RV32_2(0x1204, 0x000d),
    _RV32_2(0x1206, 0x1640),
    {CHIPCFG_STR_IFAC_END,0x0},

#if CONFIG_MSTAR_STR_ENABLE
    {CHIPCFG_STR_IFDC,0x0},
    {CHIPCFG_CMD_BASE, 0x1f20},
    _RV32_2(0x1202, 0xf3a7),
    _RV32_2(0x1204, 0x000c),
    _RV32_2(0x1206, 0x1530),
    {CHIPCFG_STR_IFDC_END,0x0},
#endif //CONFIG_MSTAR_STR_ENABLE

    _RV32_2(0x1208, 0x24ee),
    _RV32_2(0x120a, 0x3288),
    _RV32_2(0x120c, 0xd80a),
    _RV32_2(0x120e, 0xc118),
    _RV32_2(0x1210, 0x1124),
    _RV32_2(0x1212, 0x4004),
    _RV32_2(0x1214, 0x8028),
    _RV32_2(0x1216, 0xc000),
    _RV32_2(0x1228, 0x0050),
    _RV32_2(0x12d2, 0xa000),
    {CHIPCFG_CMD_BASE, 0x1f2c},
    _RV32_2(0x1560, 0x0000),
    _RV32_2(0x1562, 0x0000),
    _RV32_2(0x1564, 0x0000),
    _RV32_2(0x1566, 0x0030),
    _RV32_2(0x1568, 0x5000),
    _RV32_2(0x156a, 0x0028),

    {CHIPCFG_CMD_BASE, 0x1f22},
    _RV32_2(0x0d02, 0xaaaa),
    _RV32_2(0x0d04, 0x0080),
    _RV32_2(0x0d0a, 0x2200),
    _RV32_2(0x0d0e, 0x008b),
    _RV32_2(0x0d2e, 0x2211),
    _RV32_2(0x0d38, 0x0077),
    _RV32_2(0x0d3a, 0x000c),
    _RV32_2(0x0d3c, 0x9744),
    _RV32_2(0x0d3e, 0xb077),
    _RV32_2(0x0d48, 0x1177),
    _RV32_2(0x0d4a, 0xa0a0),
    _RV32_2(0x0d4c, 0x0044),
    _RV32_2(0x0d4e, 0x0011),
    _RV32_2(0x0d50, 0x1122),
    _RV32_2(0x0d52, 0x0075),
    _RV32_2(0x0d6c, 0x0808),
    _RV32_2(0x0d6e, 0x0808),

    _RV32_2(0x0da0, 0x2423),
    _RV32_2(0x0da2, 0x2402),
    _RV32_2(0x0da4, 0x1211),
    _RV32_2(0x0da6, 0x4130),
    _RV32_2(0x0da8, 0x3231),
    _RV32_2(0x0daa, 0x2310),
    _RV32_2(0x0dac, 0x1101),
    _RV32_2(0x0dae, 0x2203),

    _RV32_2(0x0db6, 0x0000),
    _RV32_2(0x0db8, 0x7777),
    _RV32_2(0x0dba, 0x0777),
    _RV32_2(0x0dbc, 0x0777),
    _RV32_2(0x0dbe, 0x0777),

    // TX DQ
    _RV32_2(0x0dd0, 0x5555),
    _RV32_2(0x0dd2, 0x5555),
    _RV32_2(0x0dd4, 0x5555),
    _RV32_2(0x0dd6, 0x5555),
    _RV32_2(0x0dd8, 0x0054),
    _RV32_2(0x0de0, 0x4444),
    _RV32_2(0x0de2, 0x4444),
    _RV32_2(0x0de4, 0x4444),
    _RV32_2(0x0de6, 0x4444),
    _RV32_2(0x0de8, 0x0044),
    _RV32_2(0x0d94, 0x0206),
    _RV32_2(0x0d96, 0x0205),

    // program DLL
    _RV32_2(0x0d62, 0x007f),
    _RV32_2(0x0d64, 0xf000),
    _RV32_2(0x0d60, 0x00cb),
    _RV32_2(0x0d60, 0x00cf),
    _RV32_2(0x0d60, 0x00cb),
    _RV32_2(0x0d60, 0x00c3),
    _RV32_2(0x0d60, 0x00cb),
    _RV32_2(0x0d60, 0x00c3),
    _RV32_2(0x0d60, 0x00cb),
    _RV32_2(0x0d60, 0x00c2),
    _RV32_2(0x0d60, 0x00c0),
    _RV32_2(0x0d60, 0x33c8),
    _RV32_2(0x0d70, 0x0000),
    _RV32_2(0x0d98, 0x0000),
    _RV32_2(0x0d9a, 0x0000),
    _RV32_2(0x0d90, 0xf0f1),
    _RV32_2(0x0d70, 0x0800),

#if (ENABLE_MIU1)
    {CHIPCFG_CMD_BASE, 0x1f20},
    _RV32_2(0x062c, 0x8221),
    _RV32_2(0x06fc, 0x951a),
#endif

    // program dig general setting 
    {CHIPCFG_CMD_BASE, 0x1f20},
    _RV32_2(0x122c, 0x8221),
    _RV32_2(0x12fc, 0x951a),
    _RV32_2(0x1252, 0xffff),
    _RV32_2(0x1272, 0xffff),
    _RV32_2(0x1292, 0xffff),
    _RV32_2(0x12b2, 0xffff),
    {CHIPCFG_CMD_BASE, 0x1f2C},
    _RV32_2(0x1512, 0xffff),
    _RV32_2(0x1532, 0xffff),

    // Default BW setting
    {CHIPCFG_CMD_BASE, 0x1f20},
    _RV32_2(0x1240, 0x8015),
    _RV32_2(0x1260, 0x8015),
    _RV32_2(0x1280, 0x8015),
    _RV32_2(0x12a0, 0x8015),
    {CHIPCFG_CMD_BASE, 0x1f2C},
    _RV32_2(0x1500, 0x8015),
    _RV32_2(0x1520, 0x8015),
    
    // program ana general setting
    {CHIPCFG_CMD_BASE, 0x1f22},
    _RV32_2(0x0d70, 0x0800),
    // driving setting
    _RV32_2(0x0d58, 0x0a0a),
    _RV32_2(0x0d5a, 0xaaaa),
    _RV32_2(0x0d5c, 0xaaaa),
    _RV32_2(0x0d5e, 0xaaaa),
    _RV32_2(0x0dde, 0x1010),
    // trig. lvl setting
    _RV32_2(0x0d74, 0x0808),
    _RV32_2(0x0d76, 0x0808),
    _RV32_2(0x0d1a, 0x8222), //2015.11.11  For U02 RX receiver control 
    _RV32_2(0x0d1c, 0x0020),

    //---------------------------
    //turn off delay chain for power saving 2015.12.08
    //---------------------------
    _RV32_2(0x0dfc, 0x0f0f),
    _RV32_2(0x0dfe, 0x0f0f),
    
    // MIU1 Intial 
#if (ENABLE_MIU1)
   {CHIPCFG_CMD_BASE, 0x1f2c},
    _RV32_2(0x1678, 0x0001),
    _RV32_2(0x1624, 0x1000),
    _RV32_2(0x1624, 0x0000),
    _RV32_2(0x1636, 0x0400),
    _RV32_2(0x1634, 0x2004),
    _RV32_2(0x1630, 0x2eeb),
    _RV32_2(0x1632, 0x001a),

    // DDR LDO power down off
    _RV32_2(0x168a, 0x0001),
    
    //set MIU1 SSC enable
    _RV32_2(0x1628, 0xc011),
    _RV32_2(0x162a, 0x0482),
 
    {CHIPCFG_CMD_WAIT, 1},       // delay 1ms

    _RV32_2(0x1622, 0x0004),
    _RV32_2(0x162c, 0x0114),

    {CHIPCFG_CMD_WAIT, 1},       // delay 1ms

    // MIU1 Initial Table
    {CHIPCFG_STR_IFAC,0x0},
    {CHIPCFG_CMD_BASE, 0x1f20},
    _RV32_2(0x0602, 0x03a7),
    _RV32_2(0x0604, 0x000d),
    _RV32_2(0x0606, 0x1640),
    {CHIPCFG_STR_IFAC_END,0x0},

#if CONFIG_MSTAR_STR_ENABLE
    {CHIPCFG_STR_IFDC,0x0},
    {CHIPCFG_CMD_BASE, 0x1f20},
    _RV32_2(0x0602, 0xf3a7),
    _RV32_2(0x0604, 0x000c),
    _RV32_2(0x0606, 0x1530),
    {CHIPCFG_STR_IFDC_END,0x0},
#endif

    _RV32_2(0x0608, 0x24ee),
    _RV32_2(0x060a, 0x3288),
    _RV32_2(0x060c, 0xd80a),
    _RV32_2(0x060e, 0xc118),
    _RV32_2(0x0610, 0x1124),
    _RV32_2(0x0612, 0x4004),
    _RV32_2(0x0614, 0x8028),
    _RV32_2(0x0616, 0xc000),
    _RV32_2(0x0628, 0x0050),
    _RV32_2(0x06d2, 0xa000),
   {CHIPCFG_CMD_BASE, 0x1f2c},
    _RV32_2(0x2260, 0x0000),
    _RV32_2(0x2262, 0x0000),
    _RV32_2(0x2264, 0x0000),
    _RV32_2(0x2266, 0x0030),
    _RV32_2(0x2268, 0x5000),
    _RV32_2(0x226a, 0x0028),
   {CHIPCFG_CMD_BASE, 0x1f2c},
    _RV32_2(0x1602, 0xaaaa),
    _RV32_2(0x1604, 0x0080),
    _RV32_2(0x160a, 0x2200),
    _RV32_2(0x160e, 0x008b),
    _RV32_2(0x162e, 0x3321),
    _RV32_2(0x1638, 0x0077),
    _RV32_2(0x163a, 0x404c),
    _RV32_2(0x163c, 0x9744),
    _RV32_2(0x163e, 0xb077),
    _RV32_2(0x1648, 0x4477),
    _RV32_2(0x164a, 0xa0a0),
    _RV32_2(0x164c, 0x0044),
    _RV32_2(0x164e, 0x0011),
    _RV32_2(0x1650, 0x1122),
    _RV32_2(0x1652, 0x0075),
    _RV32_2(0x166c, 0x0808),
    _RV32_2(0x166e, 0x0808),

    // RX DQs
    _RV32_2(0x16a0, 0x3516),
    _RV32_2(0x16a2, 0x0837),
    _RV32_2(0x16a4, 0x4331),
    _RV32_2(0x16a6, 0x7053),
    _RV32_2(0x16a8, 0x2022),
    _RV32_2(0x16aa, 0x0713),
    _RV32_2(0x16ac, 0x5345),
    _RV32_2(0x16ae, 0x3050),

    _RV32_2(0x16b6, 0x0000),
    _RV32_2(0x16b8, 0x7777),
    _RV32_2(0x16ba, 0x0777),
    _RV32_2(0x16bc, 0x0777),
    _RV32_2(0x16be, 0x0777),

    // TX DQ
    _RV32_2(0x16d0, 0x5555),
    _RV32_2(0x16d2, 0x5555),
    _RV32_2(0x16d4, 0x4444),
    _RV32_2(0x16d6, 0x4444),
    _RV32_2(0x16d8, 0x0044),
    _RV32_2(0x16e0, 0x4444),
    _RV32_2(0x16e2, 0x4444),
    _RV32_2(0x16e4, 0x4444),
    _RV32_2(0x16e6, 0x4444),
    _RV32_2(0x16e8, 0x0045),

    _RV32_2(0x1694, 0x0409),
    _RV32_2(0x1696, 0x0606),

    // program DLL 
    _RV32_2(0x1662, 0x007f),
    _RV32_2(0x1664, 0xf000),
    _RV32_2(0x1660, 0x00cb),
    _RV32_2(0x1660, 0x00cf),
    _RV32_2(0x1660, 0x00cb),
    _RV32_2(0x1660, 0x00c3),
    _RV32_2(0x1660, 0x00cb),
    _RV32_2(0x1660, 0x00c3),
    _RV32_2(0x1660, 0x00cb),
    _RV32_2(0x1660, 0x00c2),
    _RV32_2(0x1660, 0x00c0),
    _RV32_2(0x1660, 0x33c8),
    _RV32_2(0x1670, 0x0000),
    _RV32_2(0x1698, 0x0000),
    _RV32_2(0x169a, 0x0000),
    _RV32_2(0x1690, 0xf0f1),
    _RV32_2(0x1670, 0x0800),

    // program dig general setting
    {CHIPCFG_CMD_BASE, 0x1f20},
    _RV32_2(0x0652, 0xffff),
    _RV32_2(0x0672, 0xffff),
    _RV32_2(0x0692, 0xffff),
    _RV32_2(0x06b2, 0xffff),
    {CHIPCFG_CMD_BASE, 0x1f2c},
    _RV32_2(0x2212, 0xffff),
    _RV32_2(0x2232, 0xffff),

    // Default BW setting
    {CHIPCFG_CMD_BASE, 0x1f20},
    _RV32_2(0x0640, 0x8015),
    _RV32_2(0x0660, 0x8015),
    _RV32_2(0x0680, 0x8015),
    _RV32_2(0x06a0, 0x8015),
    {CHIPCFG_CMD_BASE, 0x1f2c},
    _RV32_2(0x2200, 0x8015),
    _RV32_2(0x2220, 0x8015),

    // program ana general setting
    _RV32_2(0x1670, 0x0800),
    // driving setting
    _RV32_2(0x1658, 0x0a0a),
    _RV32_2(0x165a, 0xaaaa),
    _RV32_2(0x165c, 0xaaaa),
    _RV32_2(0x165e, 0xaaaa),
    _RV32_2(0x16de, 0x1010),
    // trig. lvl setting
    _RV32_2(0x1674, 0x0808),
    _RV32_2(0x1676, 0x0808),
    _RV32_2(0x161a, 0x8222), //2015.11.11  For U02 RX receiver control 
    _RV32_2(0x161c, 0x0020),

    //---------------------------
    //turn off delay chain for power saving 2015.12.08
    //---------------------------
    _RV32_2(0x16fc, 0x0f0f),
    _RV32_2(0x16fe, 0x0f0f),
#endif

    // Toggle reset & disable GPIO
    {CHIPCFG_CMD_BASE, 0x1f20},
    _RV32_2(0x121e, 0x8c01),
    _RV32_2(0x121e, 0x8c00),

#if (ENABLE_MIU1)
    // Toggle reset & disable GPIO
    {CHIPCFG_CMD_BASE, 0x1f20},
    _RV32_2(0x061e, 0x8c01),
    _RV32_2(0x061e, 0x8c00),
#endif

    {CHIPCFG_STR_IFAC,0x0},
    // disable GPIO
    {CHIPCFG_CMD_BASE, 0x1f22},
    _RV32_2(0x0d00, 0x2010),
    _RV32_2(0x0d00, 0x0000),
    _RV32_2(0x0d18, 0x0000),
    _RV32_2(0x0d7c, 0x0000),
    // Release ATOP PD mode
    _RV32_2(0x0d54, 0xc000),
    _RV32_2(0x0d08, 0x003f),
    
    // DQSM RST
    _RV32_2(0x0d1e, 0x0005),
    _RV32_2(0x0d1e, 0x000f),
    _RV32_2(0x0d1e, 0x0005),

    // select Mapping
    _RV32_2(0x0d00, 0x0000),
    {CHIPCFG_CMD_BASE, 0x1f20},
    _RV32_2(0x1200, 0x0000),
    {CHIPCFG_STR_IFAC_END,0x0},

#if CONFIG_MSTAR_STR_ENABLE
    {CHIPCFG_STR_IFDC,0x0},
    // disable GPIO
    {CHIPCFG_CMD_BASE, 0x1f22},
    _RV32_2(0x0d00, 0x2018),
    _RV32_2(0x0d00, 0x0008),
    _RV32_2(0x0d18, 0x0000),
    _RV32_2(0x0d7c, 0x0000),
    // Release ATOP PD mode
    _RV32_2(0x0d54, 0xc070),
    _RV32_2(0x0d08, 0x0000),

    _RV32_2(0x0d00, 0x0008),
    {CHIPCFG_CMD_BASE, 0x1f20},
    _RV32_2(0x1200, 0x0000),
    {CHIPCFG_STR_IFDC_END,0x0},
#endif


#if (ENABLE_MIU1)
    {CHIPCFG_STR_IFAC,0x0},
    // disable GPIO
    {CHIPCFG_CMD_BASE, 0x1f2c},
    _RV32_2(0x1600, 0x2010),
    _RV32_2(0x1600, 0x0000),
    _RV32_2(0x1618, 0x0000),
    _RV32_2(0x167c, 0x0000),
    // Release ATOP PD mode
    _RV32_2(0x1654, 0xc000),
    _RV32_2(0x1608, 0x003f),
    
    // DQSM RST
    _RV32_2(0x161e, 0x0005),
    _RV32_2(0x161e, 0x000f),
    _RV32_2(0x161e, 0x0005),

    // select Mapping
    _RV32_2(0x1600, 0x0000),
    {CHIPCFG_CMD_BASE, 0x1f20},
    _RV32_2(0x0600, 0x0000),
    {CHIPCFG_STR_IFAC_END,0x0},

#if CONFIG_MSTAR_STR_ENABLE
    {CHIPCFG_STR_IFDC,0x0},
    // disable GPIO
    {CHIPCFG_CMD_BASE, 0x1f2c},
    _RV32_2(0x1600, 0x2018),
    _RV32_2(0x1600, 0x0008),
    _RV32_2(0x1618, 0x0000),
    _RV32_2(0x167c, 0x0000),
    // Release ATOP PD mode
    _RV32_2(0x1654, 0xc070),
    _RV32_2(0x1608, 0x0000),

    // select Mapping
    _RV32_2(0x1600, 0x0008),
    {CHIPCFG_CMD_BASE, 0x1f20},
    _RV32_2(0x0600, 0x0000),
    {CHIPCFG_STR_IFDC_END,0x0},
#endif

#endif //ENABLE_MIU1

    // MIU0 Init Cycle
    {CHIPCFG_CMD_WAIT, 1},       // delay 1ms
    _RV32_2(0x1200, 0x0008),
    _RV32_2(0x1200, 0x000c),
    {CHIPCFG_CMD_WAIT, 1},       // delay 1ms
    _RV32_2(0x1200, 0x000e),
    {CHIPCFG_CMD_WAIT, 1},       // delay 1ms
    _RV32_2(0x1200, 0x001f),
    
#if (ENABLE_MIU1)
    // MIU1 Init Cycle
    {CHIPCFG_CMD_WAIT, 1},       // delay 1ms
    _RV32_2(0x0600, 0x0008),
    _RV32_2(0x0600, 0x000c),
    {CHIPCFG_CMD_WAIT, 1},       // delay 1ms
    _RV32_2(0x0600, 0x000e),
    {CHIPCFG_CMD_WAIT, 1},       // delay 1ms
    _RV32_2(0x0600, 0x001f),
#endif
    // unmask High Way
    {CHIPCFG_CMD_BASE, 0x1f2C},
    _RV32_2(0x15e6, 0x0000),
#if (ENABLE_MIU1)
    _RV32_2(0x22e6, 0x0000),
#endif

