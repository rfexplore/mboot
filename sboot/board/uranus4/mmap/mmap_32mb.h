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

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    mmap_32mb.h
/// @brief  Memory mapping for 32MB RAM
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////
//#include <linux/undefconf.h>
//#include "../../../../../include/linux/undefconf.h"
#ifndef _MS_MMAP_32MB_H_
#define _MS_MMAP_32MB_H_

// Memory alignment
#define MemAlignUnit                64UL
#define MemAlign(n, unit)           ((((n)+(unit)-1)/(unit))*(unit))

#define MIU_DRAM_LEN                    (0x2000000)

//----------------------------------------------------------------------
// 32M
//----------------------------------------------------------------------
#if 0
#define EMAC_MEM_LEN                0x100000
#define EMAC_MEM_ADR                0x00200000UL

#define NULL_MEM_LEN                0x100000
// Linux kernel space
#define LINUX_MEM_AVAILABLE			(EMAC_MEM_ADR+EMAC_MEM_LEN+NULL_MEM_LEN)
#define LINUX_MEM_BASE_ADR 			(LINUX_MEM_AVAILABLE)
#define LINUX_MEM_GAP_CHK  			(LINUX_MEM_BASE_ADR-LINUX_MEM_AVAILABLE)
#define LINUX_MEM_LEN               (0x900000)    // 12MB 
#else

// Linux kernel space
#define LINUX_MEM_AVAILABLE			0x00000000UL
#define LINUX_MEM_BASE_ADR 			(LINUX_MEM_AVAILABLE)
#define LINUX_MEM_GAP_CHK  			(LINUX_MEM_BASE_ADR-LINUX_MEM_AVAILABLE)
#define LINUX_MEM_LEN               (0xC00000)     // 12MB
#endif

// VD_3DCOMB
#define VD_3DCOMB_AVAILABLE          (LINUX_MEM_BASE_ADR+LINUX_MEM_LEN)
#define VD_3DCOMB_BASE_ADR           MemAlign(VD_3DCOMB_AVAILABLE, 8)
#define VD_3DCOMB_GAP_CHK            (VD_3DCOMB_BASE_ADR-VD_3DCOMB_AVAILABLE)
#define VD_3DCOMB_LEN                (0x400000)                        // 4MB

// must start at nnn0000 - 512k alignment   --[Dean] be careful allocate this big alignment area
#define MAD_BASE_AVAILABLE          (VD_3DCOMB_BASE_ADR+VD_3DCOMB_LEN)
#define MAD_BASE_BUFFER_ADR         MemAlign(MAD_BASE_AVAILABLE, 0x80000)
#define MAD_BASE_BUFFER_ADR_GAP_CHK (MAD_BASE_BUFFER_ADR-MAD_BASE_AVAILABLE)
#define MAD_BASE_BUFFER_LEN         0x80000                      // 352K

// need 512 byte alignment SD
#define MVD_FRAMEBUFFER_AVAILABLE   (MAD_BASE_BUFFER_ADR + MAD_BASE_BUFFER_LEN)
#define MVD_FRAMEBUFFER_ADR         MemAlign(MVD_FRAMEBUFFER_AVAILABLE, 512)
#define MVD_FRAMEBUFFER_ADR_GAP_CHK (MVD_FRAMEBUFFER_ADR-MVD_FRAMEBUFFER_AVAILABLE)
#define MVD_FRAMEBUFFER_LEN         0x1DB000 // 1900KB, 3 Buffers=720*576*1.5*3

// need 8 byte alignment
#define MVD_BITSTREAM_AVAILABLE     (MVD_FRAMEBUFFER_ADR + MVD_FRAMEBUFFER_LEN)
#define MVD_BITSTREAM_ADR           MemAlign(MVD_BITSTREAM_AVAILABLE, 8)
#define MVD_BITSTREAM_ADR_GAP_CHK   (MVD_BITSTREAM_ADR-MVD_BITSTREAM_AVAILABLE)
#define MVD_BITSTREAM_LEN           0x80000                     // 512KB

#if 0
#define GE_FRAMEBUFFER_AVAILABLE    (MVD_BITSTREAM_ADR + MVD_BITSTREAM_LEN)
#define GE_FRAMEBUFFER_ADR          MemAlign(GE_FRAMEBUFFER_AVAILABLE, 8)
#define GE_FRAMEBUFFER_ADR_GAP_CHK  (GE_FRAMEBUFFER_ADR-GE_FRAMEBUFFER_AVAILABLE)
#define GE_FRAMEBUFFER_LEN          (0x426800+0x80000+0x100000)         // 5786KB

//------------------------------------------------------------------
// GOP  --- the same with GE_FRAMEBUFFER
#define GOP_GWIN_RB_AVAILABLE   GE_FRAMEBUFFER_ADR
#define GOP_GWIN_RB_BASE_ADR    (GOP_GWIN_RB_AVAILABLE)
#define GOP_GWIN_RB_GAP_CHK     (GOP_GWIN_RB_BASE_ADR-GOP_GWIN_RB_AVAILABLE)
#define GOP_GWIN_RB_BASE_LEN    GE_FRAMEBUFFER_LEN

// SUBTITLE ???
#define SUBTITLE_AVAILABLE      ((GE_FRAMEBUFFER_ADR + GE_FRAMEBUFFER_LEN) - 0x189000)  //reserved 1610KB
#define SUBTITLE_START_ADR      MemAlign(SUBTITLE_AVAILABLE, 0x10000)
#define SUBTITLE_GAP_CHK        (SUBTITLE_START_ADR-SUBTITLE_AVAILABLE)
#define SUBTITLE_LEN            0x178000                     // 1510KB
//------------------------------------------------------------------
#endif

// need 4 byte alignment
#define EMAC_BUF_AVAILABLE     (MVD_BITSTREAM_ADR + MVD_BITSTREAM_LEN)
#define EMAC_BUF_ADR           MemAlign(EMAC_BUF_AVAILABLE, 4)
#define EMAC_BUF_ADR_GAP_CHK   (EMAC_BUF_ADR-EMAC_BUF_AVAILABLE)
#define EMAC_BUF_LEN           0x180000                     // 1.5MB

#define VE_FRAMEBUFFER_AVAILABLE    (EMAC_BUF_ADR + EMAC_BUF_LEN)
#define VE_FRAMEBUFFER_ADR          MemAlign(VE_FRAMEBUFFER_AVAILABLE, 8)
#define VE_FRAMEBUFFER_ADR_GAP_CHK  (VE_FRAMEBUFFER_ADR-VE_FRAMEBUFFER_AVAILABLE)
#define VE_FRAMEBUFFER_LEN          0x195000UL            // 720*576*2*2

#define EMAC_MEM_LEN                0x100000
#define EMAC_MEM_ADR                (VE_FRAMEBUFFER_ADR+VE_FRAMEBUFFER_LEN)//0x00200000UL
#define MIU0_END_ADR                (EMAC_MEM_ADR+EMAC_MEM_LEN)//(MPOOL_ADR+MPOOL_LEN)

//======================================================================
// Can not add any buffer here (in between VE_FRAMEBUFFER_ADR and SCALER_DNR_BUF_ADR)
// Because USB/OAD download use the buffer from VE_FRAMEBUFFER_ADR to the end of SCALER_DNR_BUF_ADR
//======================================================================

// For Maximum is 1920x1088x3x2 about 12MB in HDMI and YPbPr
// Let it overwrite EVENTDB and MHEG5 buffer(These buffers is DTV only)
#define SCALER_DNR_AVAILABLE        (VE_FRAMEBUFFER_ADR + VE_FRAMEBUFFER_LEN)
#define SCALER_DNR_BUF_ADR          MemAlign(SCALER_DNR_AVAILABLE, 8)
#define SCALER_DNR_GAP_CHK          (SCALER_DNR_BUF_ADR-SCALER_DNR_AVAILABLE)
//#define SCALER_DNR_BUF_LEN          (((736UL*3+0x0F) & ~0x0F) * 581UL *2)  //(720UL*576*3*2)   // SCALER_MEM_SIZE // 2565696
#define SCALER_DNR_BUF_LEN          5711424

#define SCALER_DNR_W_AVAILABLE      (SCALER_DNR_BUF_ADR + SCALER_DNR_BUF_LEN)
#define SCALER_DNR_W_BARRIER_ADR    (SCALER_DNR_W_AVAILABLE)
#define SCALER_DNR_W_GAP_CHK        (SCALER_DNR_W_BARRIER_ADR-SCALER_DNR_W_AVAILABLE)
#define SCALER_DNR_W_BARRIER_LEN    80//16  // DNR submit 2 64-bit data before compare limit

#define SCALER_DNR_BUF_LEN_EXT      ((((896UL-736UL)*3+0x0F) & ~0x0F) * 581UL *2) // the output size of VD will be 848 * 581

#define RLD_BUF_AVAILABLE      (SCALER_DNR_W_BARRIER_ADR + SCALER_DNR_W_BARRIER_LEN)
#define RLD_BUF_ADR                 MemAlign(RLD_BUF_AVAILABLE, 8)
#define RLD_BUF_GAP_CHK        (RLD_BUF_ADR-RLD_BUF_AVAILABLE)
#define RLD_BUF_LEN    905216

#define TTX_BUF_AVAILABLE      (RLD_BUF_ADR + RLD_BUF_LEN)
#define TTX_BUF_ADR                 MemAlign(TTX_BUF_AVAILABLE, 8)
#define TTX_BUF_GAP_CHK        (TTX_BUF_ADR-TTX_BUF_AVAILABLE)
#define TTX_BUF_LEN    65536

#define MPOOL_AVAILABLE      (TTX_BUF_ADR + TTX_BUF_LEN)
#define MPOOL_ADR             MemAlign(MPOOL_AVAILABLE, 4096)
#define MPOOL_GAP_CHK        (MPOOL_ADR-MPOOL_AVAILABLE)
#define MPOOL_LEN            (MIU_DRAM_LEN - MPOOL_ADR + 1)

//#define END_OF_MEMORY_MAP           (DUMMY_SPACE+DUMMY_SPACE_LEN)
//#define END_MEMORY_GAP              (MIU_DRAM_LEN-END_OF_MEMORY_MAP)

#define END_OF_MEMORY_MAP			(MPOOL_GAP_CHK+MPOOL_LEN)//(DUMMY_SPACE+DUMMY_SPACE_LEN)
#define END_MEMORY_GAP				(MIU_DRAM_LEN-END_OF_MEMORY_MAP)

#define MIU0_END_ADR                (MPOOL_ADR+MPOOL_LEN)

#if (END_OF_MEMORY_MAP > MIU_DRAM_LEN)
#error "Memory MAP Overflow!!!"
#endif

#endif

