////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2008 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (; MStar; Confidential; Information; ) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//


//****************************************************
// DDR3-1866
//****************************************************
code U8 BWTABLE_COM[][REG_BW_ADDR_SIZE+REG_BW_BANK_SIZE+REG_BW_MASK_SIZE+1] =
{//Reg          Bank  Mask  Value
 { REG(0x31021C), 0xFF, 0xFF, 0x00 },
 { REG(0x31021D), 0xFF, 0xFF, 0x00 },
 { REG(0x31025C), 0xFF, 0xFF, 0x00 },
 { REG(0x31025D), 0xFF, 0xFF, 0x00 },
 { REG(0x31029C), 0xFF, 0xFF, 0x00 },
 { REG(0x31029D), 0xFF, 0xFF, 0x00 },
 { REG(0x3102DC), 0xFF, 0xFF, 0x00 },
 { REG(0x3102DD), 0xFF, 0xFF, 0x00 },
 { REG(0x31031C), 0xFF, 0xFF, 0x00 },
 { REG(0x31031D), 0xFF, 0xFF, 0x00 },
 { REG(0x31035C), 0xFF, 0xFF, 0x00 },
 { REG(0x31035D), 0xFF, 0xFF, 0x00 },
 { REG(0x31039C), 0xFF, 0xFF, 0x00 },
 { REG(0x31039D), 0xFF, 0xFF, 0x00 },
 { REG(0x31061C), 0xFF, 0xFF, 0x00 },
 { REG(0x31061D), 0xFF, 0xFF, 0x00 },
 { REG(0x31065C), 0xFF, 0xFF, 0x00 },
 { REG(0x31065D), 0xFF, 0xFF, 0x00 },
 { REG(0x31069C), 0xFF, 0xFF, 0x00 },
 { REG(0x31069D), 0xFF, 0xFF, 0x00 },
 { REG(0x3106DC), 0xFF, 0xFF, 0x00 },
 { REG(0x3106DD), 0xFF, 0xFF, 0x00 },
 { REG(0x31071C), 0xFF, 0xFF, 0x00 },
 { REG(0x31071D), 0xFF, 0xFF, 0x00 },
 { REG(0x31075C), 0xFF, 0xFF, 0x00 },
 { REG(0x31075D), 0xFF, 0xFF, 0x00 },
 { REG(0x31079C), 0xFF, 0xFF, 0x00 },
 { REG(0x31079D), 0xFF, 0xFF, 0x00 },
 { REG(0x310776), 0xFF, 0xFF, 0x00 },
 { REG(0x310777), 0xFF, 0xFF, 0x00 },
 { REG(0x3107B6), 0xFF, 0xFF, 0x00 },
 { REG(0x3107B6), 0xFF, 0xFF, 0x00 },
 { REG(0x152062), 0xFF, 0xFF, 0x80 },
 { REG(0x15205C), 0xFF, 0xFF, 0x00 },
 { REG(0x15204A), 0xFF, 0xFF, 0xE1 },
 { REG(0x15204B), 0xFF, 0xFF, 0xD2 },
 { REG(0x15206E), 0xFF, 0xFF, 0x0A },
 { REG(0x152070), 0xFF, 0xFF, 0x0A },
 { REG(0x152162), 0xFF, 0xFF, 0x80 },
 { REG(0x15215C), 0xFF, 0xFF, 0x00 },
 { REG(0x15214A), 0xFF, 0xFF, 0xE1 },
 { REG(0x15214B), 0xFF, 0xFF, 0xD2 },
 { REG(0x15216E), 0xFF, 0xFF, 0x0A },
 { REG(0x152170), 0xFF, 0xFF, 0x0A },
 { REG(0x161521), 0xFF, 0xFF, 0x80 },
 { REG(0x161522), 0xFF, 0xFF, 0x10 },
 { REG(0x161523), 0xFF, 0xFF, 0x08 },
 { REG(0x161524), 0xFF, 0xFF, 0x08 },
 { REG(0x161526), 0xFF, 0xFF, 0x20 },
 { REG(0x161530), 0xFF, 0xFF, 0xE7 },
 { REG(0x161531), 0xFF, 0xFF, 0xDF },
 { REG(0x1615E2), 0xFF, 0xFF, 0xFF },
 { REG(0x162221), 0xFF, 0xFF, 0x80 },
 { REG(0x162222), 0xFF, 0xFF, 0x10 },
 { REG(0x162223), 0xFF, 0xFF, 0x08 },
 { REG(0x162224), 0xFF, 0xFF, 0x08 },
 { REG(0x162226), 0xFF, 0xFF, 0x20 },
 { REG(0x162230), 0xFF, 0xFF, 0xE7 },
 { REG(0x162231), 0xFF, 0xFF, 0xDF },
 { REG(0x1622E2), 0xFF, 0xFF, 0xFF },
 { REG(0x01), 0xFF, 0x00, 0x00 },//Same mark
 { REG(REG_TABLE_END), 0x00, 0x00 }
};

code U8 BWTABLE[][REG_BW_ADDR_SIZE+REG_BW_BANK_SIZE+REG_BW_MASK_SIZE+BWTABLE_NUMS]=
{//Reg          Bank  Mask  Value
 { REG(REG_TABLE_END), 0x00, 0x00 }
};
