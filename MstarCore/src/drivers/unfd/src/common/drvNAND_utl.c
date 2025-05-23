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
#include <linux/string.h>
#include "drvNAND.h"
#include "drvNAND_utl.h"

extern int Disable_Err_log;

typedef struct {
	const char *msg;
	U8  u8_OpCode_RW_AdrCycle;
	U8  u8_OpCode_Erase_AdrCycle;
	U16 u16_Reg48_Spare;
} drvNAND_ReadSeq;

static const drvNAND_ReadSeq seq[] = {
	{"R_SEQUENCE_003A", ADR_C3TFS0, ADR_C2TRS0, BIT_NC_ONE_COL_ADDR},
	{"R_SEQUENCE_004A", ADR_C4TFS0,	ADR_C3TRS0,	BIT_NC_ONE_COL_ADDR},
	{"R_SEQUENCE_004A30", ADR_C4TFS0, ADR_C2TRS0, 0},
	{"R_SEQUENCE_005A30", ADR_C5TFS0, ADR_C3TRS0, 0},
	{"R_SEQUENCE_006A30", ADR_C6TFS0, ADR_C3TRS0, 0}, // FIXME
	{NULL, 0, 0, 0},
};

U32 drvNAND_ProbeReadSeq(void)
{
	NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();
	U8 u8_seq_idx = 0;
	U32 u32_Err;
	drvNAND_ReadSeq const *pSeq = seq;

	pNandDrv->u16_BlkCnt = 0x400;
	pNandDrv->u16_BlkPageCnt = 0x40;
	pNandDrv->u16_PageByteCnt = 0x200;
	pNandDrv->u16_SpareByteCnt = 0x10;
    pNandDrv->u16_ECCType = ECC_TYPE_4BIT;
    pNandDrv->u8_Flag_004A30 = 0;

	NC_ConfigNandFlashContext();
	u32_Err = NC_Init();
	if (u32_Err != UNFD_ST_SUCCESS)
		return u32_Err;

	if(pNandDrv->u16_Reg58_DDRCtrl & BIT_DDR_TOGGLE)
	{
		pSeq +=3;
		u8_seq_idx += 3;
	}
	
	/* Probe read sequence */
	while (pSeq->msg) {

		nand_debug(UNFD_DEBUG_LEVEL_HIGH, 1, "%s\n", pSeq->msg);
		pNandDrv->u8_OpCode_RW_AdrCycle = pSeq->u8_OpCode_RW_AdrCycle;
		pNandDrv->u8_OpCode_Erase_AdrCycle = pSeq->u8_OpCode_Erase_AdrCycle;
		pNandDrv->u16_Reg48_Spare &= ~BIT_NC_ONE_COL_ADDR;
		pNandDrv->u16_Reg48_Spare |= pSeq->u16_Reg48_Spare;

        NC_Config();

		u32_Err = NC_ProbeReadSeq();
		if (u32_Err == UNFD_ST_SUCCESS)
        {
            // if R_SEQUENCE_004A30
            if(ADR_C4TFS0 == pNandDrv->u8_OpCode_RW_AdrCycle &&
               0 == (pNandDrv->u16_Reg48_Spare & BIT_NC_ONE_COL_ADDR))
            {
                pNandDrv->u8_Flag_004A30 = 1;
            }
			pNandDrv->u8_AddrCycleIdx = u8_seq_idx;
			return UNFD_ST_SUCCESS;
        }
		NC_ResetFCIE();
		pSeq++;
		u8_seq_idx ++;
	}

	nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "ERROR: Unknown RSEQ\n");

	return UNFD_ST_ERR_UNKNOWN_RSEQ;
}

U32 drvNAND_CheckPartInfo(U32 u32_PageIdx)
{
	NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();
	U8 *au8_SectorBuf = (U8*)pNandDrv->pu8_PageDataBuf;
	U8 *au8_SpareBuf = (U8*)pNandDrv->pu8_PageSpareBuf;
	U32 u32_Err;
	U32 u32_ChkSum;
	U32 u32_ByteCnt;

	PARTITION_INFO_t *pPartInfo = (PARTITION_INFO_t*)au8_SectorBuf;

	//u32_Err = NC_ReadSector_RIUMode(u32_PageIdx, 0, au8_SectorBuf, au8_SpareBuf);
	u32_Err = NC_ReadSectors_Retry(u32_PageIdx, 0, au8_SectorBuf, au8_SpareBuf, 1);
	if (u32_Err != UNFD_ST_SUCCESS)
	{
		return u32_Err;
	}

	u32_ChkSum = drvNAND_CheckSum(au8_SectorBuf + 0x04, 0x200 - 0x04);
	if (u32_ChkSum != pPartInfo->u32_ChkSum) {
		nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Skip PartInfo (ChkSum: 0x%08lx != 0x%08lx)\n",
				   u32_ChkSum, pPartInfo->u32_ChkSum);

		return UNFD_ST_ERR_CIS_PART_ERR;
	}

	u32_ByteCnt = pPartInfo->u16_PartCnt * pPartInfo->u16_UnitByteCnt;
	memcpy(pNandDrv->pPartInfo, pPartInfo, sizeof(*pPartInfo) + u32_ByteCnt);

	//dump_part_info(pPartInfo);
	return UNFD_ST_SUCCESS;
}

void drvNAND_ParseNandInfo(NAND_FLASH_INFO_t *pNandInfo)
{
	int i;
	NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();

	pNandDrv->u16_PageByteCnt  = pNandInfo->u16_PageByteCnt;
	pNandDrv->u16_SpareByteCnt = pNandInfo->u16_SpareByteCnt;
	pNandDrv->u16_BlkPageCnt   = pNandInfo->u16_BlkPageCnt;
	pNandDrv->u16_BlkCnt       = pNandInfo->u16_BlkCnt;
	pNandDrv->u16_ECCType      = pNandInfo->u16_ECCType;
	pNandDrv->u8_IDByteCnt     = pNandInfo->u8_IDByteCnt;
	pNandDrv->u32_Config       = pNandInfo->u32_Config;
	pNandDrv->u16_tRC          = pNandInfo->u16_tRC;
	pNandDrv->u8_tRP           = pNandInfo->u8_tRP;
	pNandDrv->u8_tREH          = pNandInfo->u8_tREH;
	pNandDrv->u8_tREA          = pNandInfo->u8_tREA;
	pNandDrv->u8_tRR           = pNandInfo->u8_tRR;
	pNandDrv->u16_tADL         = pNandInfo->u16_tADL;
	pNandDrv->u16_tRHW         = pNandInfo->u16_tRHW;
	pNandDrv->u16_tWHR         = pNandInfo->u16_tWHR;
	pNandDrv->u16_tCCS         = pNandInfo->u16_tCCS;
	pNandDrv->u8_tCS           = pNandInfo->u8_tCS;
	pNandDrv->u16_tWC          = pNandInfo->u16_tWC;
	pNandDrv->u8_tWP           = pNandInfo->u8_tWP;
	pNandDrv->u8_tWH           = pNandInfo->u8_tWH;
	pNandDrv->u16_tCWAW        = pNandInfo->u16_tCWAW;
	pNandDrv->u8_tCLHZ		   = pNandInfo->u8_tCLHZ;
	pNandDrv->u16_tWW		   = pNandInfo->u16_tWW;
	pNandDrv->u8_AddrCycleIdx  = pNandInfo->u8_AddrCycleIdx;

	if(pNandDrv->u8_AddrCycleIdx != 0)
	{
		pNandDrv->u8_OpCode_Erase_AdrCycle = seq[pNandDrv->u8_AddrCycleIdx].u8_OpCode_Erase_AdrCycle;
		pNandDrv->u8_OpCode_RW_AdrCycle = seq[pNandDrv->u8_AddrCycleIdx].u8_OpCode_RW_AdrCycle;
		pNandDrv->u16_One_Col_Addr = seq[pNandDrv->u8_AddrCycleIdx].u16_Reg48_Spare;
		pNandDrv->u16_Reg48_Spare &= ~BIT_NC_ONE_COL_ADDR;
		pNandDrv->u16_Reg48_Spare |= seq[pNandDrv->u8_AddrCycleIdx].u16_Reg48_Spare;
	}
	
	memcpy(pNandDrv->u8_Vendor, pNandInfo->u8_Vendor, 16);
	memcpy(pNandDrv->u8_PartNumber, pNandInfo->u8_PartNumber, 16);

	pNandDrv->u8_CellType      = pNandDrv->u32_Config&0x1;
	pNandDrv->u8_BadBlkMarker      = (pNandDrv->u32_Config>>1)&0x7;
	pNandDrv->u8_PlaneCnt      = ((pNandDrv->u32_Config>>4)&0x7)+1;
	pNandDrv->u8_RequireRandomizer		= (pNandDrv->u32_Config>>8) & 0x1;
	pNandDrv->u8_NANDInterface	= ((pNandDrv->u32_Config>>9) & 3);
	pNandDrv->u8_CacheProgram	= ((pNandDrv->u32_Config >> 11) & 3);
	pNandDrv->u8_CacheRead		= ((pNandDrv->u32_Config >> 13) & 3);
	pNandDrv->u8_RequireReadRetry = ((pNandDrv->u32_Config>>15) & 1);
	pNandDrv->u8_MaxCENum       = ((pNandDrv->u32_Config >> 18) & 3);
	pNandDrv->u8_PairPageMapLoc = pNandInfo->u8_PairPageMapLoc;
	pNandDrv->u8_ReadRetryType =	pNandInfo->u8_ReadRetryType;
	pNandDrv->u16_BitflipThreshold = pNandInfo->u8_BitflipThreshold;
	#if defined(FCIE4_DDR) && FCIE4_DDR || defined(DDR_NAND_SUPPORT) && DDR_NAND_SUPPORT
	
	if(pNandInfo->tDefaultDDR.u8_DqsMode != 0 || pNandInfo->tDefaultDDR.u8_ClkIdx != 0)
	{
	    memcpy((void *) &pNandDrv->tDefaultDDR, (const void *) &pNandInfo->tDefaultDDR, sizeof(DDR_TIMING_GROUP_t));
	    memcpy((void *) &pNandDrv->tMaxDDR, (const void *) &pNandInfo->tMaxDDR, sizeof(DDR_TIMING_GROUP_t));
	    memcpy((void *) &pNandDrv->tMinDDR, (const void *) &pNandInfo->tMinDDR, sizeof(DDR_TIMING_GROUP_t));
	}
	#endif

	pNandDrv->u8_BL0PBA = pNandInfo->u8_BL0PBA;
	pNandDrv->u8_BL1PBA = pNandInfo->u8_BL1PBA;
	pNandDrv->u8_UBOOTPBA = pNandInfo->u8_UBOOTPBA;
	pNandDrv->u8_HashPBA[0][0] = pNandInfo->u8_HashPBA[0][0];
	pNandDrv->u8_HashPBA[0][1] = pNandInfo->u8_HashPBA[0][1];
	pNandDrv->u8_HashPBA[1][0] = pNandInfo->u8_HashPBA[1][0];
	pNandDrv->u8_HashPBA[1][1] = pNandInfo->u8_HashPBA[1][1];
	pNandDrv->u8_HashPBA[2][0] = pNandInfo->u8_HashPBA[2][0];
	pNandDrv->u8_HashPBA[2][1] = pNandInfo->u8_HashPBA[2][1];
	pNandDrv->u8_Hash0PageIdx = pNandInfo->u8_Hash0PageIdx;
	pNandDrv->u8_Hash1PageIdx = pNandInfo->u8_Hash1PageIdx;	
	pNandDrv->u32_BootSize = pNandInfo->u32_BootSize;

	pNandDrv->u16_BBtPageCheckCount = pNandInfo->u16_BBtPageCheckCount;
	memcpy(pNandDrv->u16_BBtPageIdx, pNandInfo->u16_BBtPageIdx, NAND_BBT_PAGE_COUNT*sizeof(U16));
	memcpy(pNandDrv->u16_BBtMarker, pNandInfo->u16_BBtMarker, NAND_BBT_PAGE_COUNT*sizeof(U16));
	
	nand_debug(UNFD_DEBUG_LEVEL_LOW, 1,"----------------------------------------------------\n");
	nand_debug(UNFD_DEBUG_LEVEL_LOW, 1,"BBtPageCount= %d\n", pNandDrv->u16_BBtPageCheckCount);
	for(i = 0; i < NAND_BBT_PAGE_COUNT; i++)
		nand_debug(UNFD_DEBUG_LEVEL_LOW, 1,"BBtPageIdx[%d] = %d, BBtMarker[%d] = %d\n", i, pNandDrv->u16_BBtPageIdx[i], i, pNandDrv->u16_BBtMarker[i]);

	pNandDrv->u16_PhySpareByteCnt = pNandInfo->u16_PhySpareByteCnt;
	if(pNandDrv->u16_PhySpareByteCnt == 0)
		pNandDrv->u16_PhySpareByteCnt = pNandDrv->u16_SpareByteCnt;
}

void drvNAND_DumpReadRetry(void)
{
	U16 u16_i, u16_j, u16_k;
	NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();

	nand_debug(UNFD_DEBUG_LEVEL_WARNING, 1,"u8_CustRegNo = %X\n", pNandDrv->ReadRetry_t.u8_CustRegNo);

	nand_debug(UNFD_DEBUG_LEVEL_WARNING, 1,"Cust Register: %X\n", pNandDrv->ReadRetry_t.u8_CustRegNo);
	for(u16_i = 0; u16_i < pNandDrv->ReadRetry_t.u8_CustRegNo; u16_i++)
		nand_debug(UNFD_DEBUG_LEVEL_WARNING, 0,"%X ", pNandDrv->ReadRetry_t.pu8_CustRegTable[u16_i]);
	nand_debug(UNFD_DEBUG_LEVEL_WARNING, 0,"\n");

	nand_debug(UNFD_DEBUG_LEVEL_WARNING, 1,"u8_DefaultValueOffset = %X\n", pNandDrv->ReadRetry_t.u8_DefaultValueOffset);

	nand_debug(UNFD_DEBUG_LEVEL_WARNING, 1,"u8_MaxRetryTime = %X\n", pNandDrv->ReadRetry_t.u8_MaxRetryTime);

	nand_debug(UNFD_DEBUG_LEVEL_WARNING, 1,"u8_ByteLenPerCmd = %X\n", pNandDrv->ReadRetry_t.u8_ByteLenPerCmd);

	nand_debug(UNFD_DEBUG_LEVEL_WARNING, 1,"Retry Register Value Table\n ");

	for(u16_i = 0; u16_i < pNandDrv->ReadRetry_t.u8_CustRegNo; u16_i ++)
		nand_debug(UNFD_DEBUG_LEVEL_WARNING, 0,"Reg%X\t", u16_i);
	nand_debug(UNFD_DEBUG_LEVEL_WARNING, 0,"\n");

	for(u16_i = 0; u16_i < pNandDrv->ReadRetry_t.u8_MaxRetryTime; u16_i ++)
	{
		for(u16_j = 0; u16_j < pNandDrv->ReadRetry_t.u8_CustRegNo; u16_j ++)
		{
			for(u16_k = 0; u16_k < pNandDrv->ReadRetry_t.u8_ByteLenPerCmd+1; u16_k ++)
				nand_debug(UNFD_DEBUG_LEVEL_WARNING, 0,"%02X", pNandDrv->ReadRetry_t.pppu8_RetryRegValue[u16_i][u16_j][u16_k]);
			nand_debug(UNFD_DEBUG_LEVEL_WARNING, 0,"\t");
		}
		nand_debug(UNFD_DEBUG_LEVEL_WARNING, 0,"\n");
	}
	nand_debug(UNFD_DEBUG_LEVEL_WARNING, 1,"u8_DefaultValueRestore = %X\n", pNandDrv->ReadRetry_t.u8_DefaultValueRestore);

	nand_debug(UNFD_DEBUG_LEVEL_WARNING, 1,"u8_SetCmdLen = %X\n", pNandDrv->ReadRetry_t.u8_SetCmdLen);
	
	if(pNandDrv->ReadRetry_t.u8_SetCmdLen)
	{
		for(u16_i = 0; u16_i < pNandDrv->ReadRetry_t.u8_SetCmdLen; u16_i ++)
			nand_debug(UNFD_DEBUG_LEVEL_WARNING, 0,"%X\t", pNandDrv->ReadRetry_t.pu8_SetCmdTypeSeq[u16_i]);
		nand_debug(UNFD_DEBUG_LEVEL_WARNING, 0,"\n");
		
		for(u16_i = 0; u16_i < pNandDrv->ReadRetry_t.u8_SetCmdLen; u16_i ++)
			nand_debug(UNFD_DEBUG_LEVEL_WARNING, 0,"%X\t", pNandDrv->ReadRetry_t.pu8_SetCmdValueSeq[u16_i]);
		nand_debug(UNFD_DEBUG_LEVEL_WARNING, 0,"\n");
	}
	nand_debug(UNFD_DEBUG_LEVEL_WARNING, 1,"u8_GetCmdLen = %X\n", pNandDrv->ReadRetry_t.u8_GetCmdLen);

	if(pNandDrv->ReadRetry_t.u8_GetCmdLen)
	{
		for(u16_i = 0; u16_i < pNandDrv->ReadRetry_t.u8_GetCmdLen; u16_i ++)
			nand_debug(UNFD_DEBUG_LEVEL_WARNING, 0,"%X\t", pNandDrv->ReadRetry_t.pu8_GetCmdTypeSeq[u16_i]);
		nand_debug(UNFD_DEBUG_LEVEL_WARNING, 0,"\n");
		
		for(u16_i = 0; u16_i < pNandDrv->ReadRetry_t.u8_GetCmdLen; u16_i ++)
			nand_debug(UNFD_DEBUG_LEVEL_WARNING, 0,"%X\t", pNandDrv->ReadRetry_t.pu8_GetCmdValueSeq[u16_i]);
		nand_debug(UNFD_DEBUG_LEVEL_WARNING, 0,"\n");
	}
}

U32 drvNAND_ParseReadRetryCmd(U8 *pu8_DataBuf)
{
	NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();
	U16 u16_Idx = 0, u16_i, u16_j;

	pNandDrv->u8_RequireReadRetry = 1;
	pNandDrv->ReadRetry_t.u8_CustRegNo = pu8_DataBuf[u16_Idx];
	u16_Idx ++;
	//Number of Custom Register

	pNandDrv->ReadRetry_t.pu8_CustRegTable = (U8*)malloc(pNandDrv->ReadRetry_t.u8_CustRegNo * sizeof(U8));
	if(!pNandDrv->ReadRetry_t.pu8_CustRegTable)
		return UNFD_ST_ERR_INVALID_ADDR;
	//Reg Table
	memcpy(pNandDrv->ReadRetry_t.pu8_CustRegTable, pu8_DataBuf+u16_Idx, pNandDrv->ReadRetry_t.u8_CustRegNo);
	u16_Idx += pNandDrv->ReadRetry_t.u8_CustRegNo;
	
	pNandDrv->ReadRetry_t.u8_DefaultValueOffset = pu8_DataBuf[u16_Idx];
	u16_Idx ++;
	pNandDrv->ReadRetry_t.u8_MaxRetryTime = pu8_DataBuf[u16_Idx];
	u16_Idx ++;
	pNandDrv->ReadRetry_t.u8_ByteLenPerCmd = pu8_DataBuf[u16_Idx];
	u16_Idx ++;

	pNandDrv->ppu8_ReadRetryDefault = (U8**)malloc(pNandDrv->ReadRetry_t.u8_CustRegNo * sizeof(U8*) +
										pNandDrv->ReadRetry_t.u8_CustRegNo * (pNandDrv->ReadRetry_t.u8_ByteLenPerCmd + 1) *sizeof(U8));
	if(!pNandDrv->ppu8_ReadRetryDefault)
		return UNFD_ST_ERR_INVALID_ADDR;

	for(u16_i = 0; u16_i < pNandDrv->ReadRetry_t.u8_CustRegNo; u16_i ++)
		pNandDrv->ppu8_ReadRetryDefault[u16_i] = ((U8*)(pNandDrv->ppu8_ReadRetryDefault + pNandDrv->ReadRetry_t.u8_CustRegNo) 
													+ u16_i * (pNandDrv->ReadRetry_t.u8_ByteLenPerCmd + 1) *sizeof(U8));

	//alloc Retry Register value table and parse Retry Register value table
	pNandDrv->ReadRetry_t.pppu8_RetryRegValue = 
		(U8***) malloc( pNandDrv->ReadRetry_t.u8_MaxRetryTime *sizeof(U8**) 
			+ pNandDrv->ReadRetry_t.u8_MaxRetryTime * pNandDrv->ReadRetry_t.u8_CustRegNo *sizeof(U8*) 
			+ pNandDrv->ReadRetry_t.u8_MaxRetryTime * pNandDrv->ReadRetry_t.u8_CustRegNo * (pNandDrv->ReadRetry_t.u8_ByteLenPerCmd + 1) *sizeof(U8) );
	if(!pNandDrv->ReadRetry_t.pppu8_RetryRegValue)
		return UNFD_ST_ERR_INVALID_ADDR;

	for(u16_i = 0 ; u16_i < pNandDrv->ReadRetry_t.u8_MaxRetryTime; u16_i ++)
		pNandDrv->ReadRetry_t.pppu8_RetryRegValue[u16_i] = ((U8**) pNandDrv->ReadRetry_t.pppu8_RetryRegValue + 
											pNandDrv->ReadRetry_t.u8_MaxRetryTime) + u16_i * pNandDrv->ReadRetry_t.u8_CustRegNo;
	
	for(u16_i = 0; u16_i < pNandDrv->ReadRetry_t.u8_MaxRetryTime; u16_i ++)
		for(u16_j = 0; u16_j < pNandDrv->ReadRetry_t.u8_CustRegNo; u16_j ++)
			pNandDrv->ReadRetry_t.pppu8_RetryRegValue[u16_i][u16_j] = 
				((U8*) (pNandDrv->ReadRetry_t.pppu8_RetryRegValue + pNandDrv->ReadRetry_t.u8_MaxRetryTime + 
					pNandDrv->ReadRetry_t.u8_MaxRetryTime * pNandDrv->ReadRetry_t.u8_CustRegNo)) + 
						(u16_i*pNandDrv->ReadRetry_t.u8_CustRegNo + u16_j)* (pNandDrv->ReadRetry_t.u8_ByteLenPerCmd + 1) *sizeof(U8);
	
	memcpy(&pNandDrv->ReadRetry_t.pppu8_RetryRegValue[0][0][0], pu8_DataBuf + u16_Idx,
            pNandDrv->ReadRetry_t.u8_MaxRetryTime * pNandDrv->ReadRetry_t.u8_CustRegNo * (pNandDrv->ReadRetry_t.u8_ByteLenPerCmd + 1) *sizeof(U8));
	
	u16_Idx+= pNandDrv->ReadRetry_t.u8_MaxRetryTime * pNandDrv->ReadRetry_t.u8_CustRegNo * (pNandDrv->ReadRetry_t.u8_ByteLenPerCmd + 1) *sizeof(U8);
	pNandDrv->ReadRetry_t.u8_DefaultValueRestore = pu8_DataBuf[u16_Idx];
	u16_Idx ++;
	pNandDrv->ReadRetry_t.u8_SetCmdLen = pu8_DataBuf[u16_Idx];
	u16_Idx ++;
	if(pNandDrv->ReadRetry_t.u8_SetCmdLen)
	{
		//parse Set CMD Type Sequence
		pNandDrv->ReadRetry_t.pu8_SetCmdTypeSeq = (U8*)malloc(pNandDrv->ReadRetry_t.u8_SetCmdLen * sizeof(U8));
		if(!pNandDrv->ReadRetry_t.pu8_SetCmdTypeSeq)
			return UNFD_ST_ERR_INVALID_ADDR;
		memcpy(pNandDrv->ReadRetry_t.pu8_SetCmdTypeSeq, pu8_DataBuf + u16_Idx, pNandDrv->ReadRetry_t.u8_SetCmdLen);
		u16_Idx += pNandDrv->ReadRetry_t.u8_SetCmdLen;
		//parse Set CMD Value Sequence
    	pNandDrv->ReadRetry_t.pu8_SetCmdValueSeq = (U8*)malloc(pNandDrv->ReadRetry_t.u8_SetCmdLen * sizeof(U8));
		if(!pNandDrv->ReadRetry_t.pu8_SetCmdValueSeq)
			return UNFD_ST_ERR_INVALID_ADDR;
    	memcpy(pNandDrv->ReadRetry_t.pu8_SetCmdValueSeq, pu8_DataBuf + u16_Idx, pNandDrv->ReadRetry_t.u8_SetCmdLen);
   		u16_Idx += pNandDrv->ReadRetry_t.u8_SetCmdLen;
	}
	pNandDrv->ReadRetry_t.u8_GetCmdLen = pu8_DataBuf[u16_Idx];
	u16_Idx ++;
	if(pNandDrv->ReadRetry_t.u8_GetCmdLen)
	{	
		//parse Get CMD Type Sequence
		pNandDrv->ReadRetry_t.pu8_GetCmdTypeSeq = (U8*)malloc(pNandDrv->ReadRetry_t.u8_GetCmdLen * sizeof(U8));
		if(!pNandDrv->ReadRetry_t.pu8_GetCmdTypeSeq)
			return UNFD_ST_ERR_INVALID_ADDR;
		memcpy(pNandDrv->ReadRetry_t.pu8_GetCmdTypeSeq, pu8_DataBuf + u16_Idx, pNandDrv->ReadRetry_t.u8_GetCmdLen);
		u16_Idx += pNandDrv->ReadRetry_t.u8_GetCmdLen;
		//parse Get CMD Value Sequence
		pNandDrv->ReadRetry_t.pu8_GetCmdValueSeq = (U8*)malloc(pNandDrv->ReadRetry_t.u8_GetCmdLen * sizeof(U8));
		if(!pNandDrv->ReadRetry_t.pu8_GetCmdValueSeq)
			return UNFD_ST_ERR_INVALID_ADDR;
		memcpy(pNandDrv->ReadRetry_t.pu8_GetCmdValueSeq, pu8_DataBuf + u16_Idx, pNandDrv->ReadRetry_t.u8_GetCmdLen);
		u16_Idx += pNandDrv->ReadRetry_t.u8_GetCmdLen;
	}
	return UNFD_ST_SUCCESS;
}

U32 drvNAND_SearchCIS(void)
{
	NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();
	U8 *au8_PageBuf = (U8*)pNandDrv->pu8_PageDataBuf;
	U8 *au8_SpareBuf = (U8*)pNandDrv->pu8_PageSpareBuf;
	NAND_FLASH_INFO_t *pNandInfo = (NAND_FLASH_INFO_t*)au8_PageBuf;
	BLK_INFO_t *pBlkInfo = (BLK_INFO_t*)au8_SpareBuf;
	U16 u16_PBA;
	U32 u32_PageIdx;
	U32 u32_Err;
	U32 u32_ChkSum;
	U32 u16_i;
	U8 u8_BlkPageCntBits = 6; /* 64 pages per block */

	pNandDrv->u8_CISBlk = 0xFF;
	
	for (u16_PBA = 0; u16_PBA < 10; u16_PBA++)
	{
		pNandDrv->u16_BlkCnt = 0x400;
		pNandDrv->u16_BlkPageCnt = 0x20;
		pNandDrv->u16_PageByteCnt = 0x800;
		pNandDrv->u16_SpareByteCnt = 0x100;
		pNandDrv->u16_ECCType = NANDINFO_ECC_TYPE;
		pNandDrv->u16_BitflipThreshold = 0;

		NC_ConfigNandFlashContext();
		NC_RegInit();
		pNandDrv->u16_Reg48_Spare |= BIT_NC_HW_AUTO_RANDOM_CMD_DISABLE;
		//force disable Randomizer
		#if defined(FCIE_LFSR) && FCIE_LFSR	
		NC_DisableLFSR();
		#endif
		NC_Config();
		u32_PageIdx = u16_PBA << u8_BlkPageCntBits;
		nand_debug(UNFD_DEBUG_LEVEL_HIGH, 0, "32Pages: %d\n", u16_PBA);

		// ------------------------------------
		if(pNandDrv->u8_Flag_004A30)
	    {
		    pNandDrv->u8_OpCode_RW_AdrCycle = ADR_C4TFS0;
		    pNandDrv->u8_OpCode_Erase_AdrCycle = ADR_C2TRS0;
	    }
		LABEL_TRY_005A30:
		// ------------------------------------
			
		#if defined(ENABLE_NAND_RIU_MODE)&&ENABLE_NAND_RIU_MODE
		u32_Err = NC_ReadSector_RIUMode(u32_PageIdx, 0, au8_PageBuf, au8_SpareBuf);
		#else
		u32_Err = NC_ReadSectors(u32_PageIdx, 0, au8_PageBuf, au8_SpareBuf, 1);
		#endif
		#if 0		//WordMode is unsupported
		if (u32_Err != UNFD_ST_SUCCESS && 0==(pNandDrv->u16_Reg58_DDRCtrl & BIT_DDR_MASM))
		{
			//nand_debug(0, 0, "NAND, change WordMode, now: %X\n", pNandDrv->u8_WordMode);

			// If read sector fail, change u8_WordMode and try again
			if (pNandDrv->u8_WordMode == 0)
			{
				pNandDrv->u8_WordMode = 1;
				pNandDrv->u16_Reg50_EccCtrl |= BIT_NC_WORD_MODE;
			}
			else
			{
				pNandDrv->u8_WordMode = 0;
				pNandDrv->u16_Reg50_EccCtrl &= (~BIT_NC_WORD_MODE);
			}

			#if defined(ENABLE_NAND_RIU_MODE)&&ENABLE_NAND_RIU_MODE
			u32_Err = NC_ReadSector_RIUMode(u32_PageIdx, 0, au8_PageBuf, au8_SpareBuf);
			#else
			u32_Err = NC_ReadSectors(u32_PageIdx, 0, au8_PageBuf, au8_SpareBuf, 1);
			#endif
			//nand_debug(UNFD_DEBUG_LEVEL_HIGH, 0, "NC_ReadSectors2=%x\n", u32_Err);
		}
		#endif

		if (u32_Err != UNFD_ST_SUCCESS || pBlkInfo->u8_BadBlkMark != 0xFF)
		{
			if(1==pNandDrv->u8_Flag_004A30 && ADR_C5TFS0!=pNandDrv->u8_OpCode_RW_AdrCycle)
			{
				//nand_debug(0,1,"switch to 005A30, 0 \n");
				pNandDrv->u8_OpCode_RW_AdrCycle = ADR_C5TFS0;
				pNandDrv->u8_OpCode_Erase_AdrCycle = ADR_C3TRS0;
				goto LABEL_TRY_005A30;
			}

			continue;
		}

		/* Valid CIS has block address 0 or 1 */
		//if (pBlkInfo->u16_BlkAddr > 1)
		//	continue;

		if (drvNAND_CompareCISTag(pNandInfo->au8_Tag))
		{
			if(1==pNandDrv->u8_Flag_004A30 && ADR_C5TFS0!=pNandDrv->u8_OpCode_RW_AdrCycle)
			{
				//nand_debug(0,1,"switch to 005A30, 1 \n");
				pNandDrv->u8_OpCode_RW_AdrCycle = ADR_C5TFS0;
				pNandDrv->u8_OpCode_Erase_AdrCycle = ADR_C3TRS0;
				goto LABEL_TRY_005A30;
			}

			continue;
		}

		u32_ChkSum = drvNAND_CheckSum(au8_PageBuf + 0x24, 0x32 - 0x24);
		if (u32_ChkSum != pNandInfo->u32_ChkSum)
		{
			nand_debug(UNFD_DEBUG_LEVEL_WARNING, 1, "Skip Blk 0x%08x, ChkSum: 0x%08lx != 0x%08lx\n",
				u16_PBA, u32_ChkSum, pNandInfo->u32_ChkSum);

			if(1==pNandDrv->u8_Flag_004A30 && ADR_C5TFS0!=pNandDrv->u8_OpCode_RW_AdrCycle)
			{
				//nand_debug(0,1,"switch to 005A30, 2 \n");
				pNandDrv->u8_OpCode_RW_AdrCycle = ADR_C5TFS0;
				pNandDrv->u8_OpCode_Erase_AdrCycle = ADR_C3TRS0;
				goto LABEL_TRY_005A30;
			}

			continue;
		}

		//u8_CISIdx = pBlkInfo->u16_BlkAddr;

		nand_debug(UNFD_DEBUG_LEVEL_HIGH, 0, "Complete auto-checking 8/16 bits:0x%X\n", pNandDrv->u8_WordMode);
		dump_nand_info(pNandInfo);

		drvNAND_ParseNandInfo(pNandInfo);

		//Read Retry
		if(pNandDrv->u8_RequireReadRetry)
		{
			NC_SetupReadRetryCmd();
			NC_GetRegDefaultValue();
		}
        
		NC_ConfigNandFlashContext();
		NC_RegInit();
		#if defined(FCIE_LFSR) && FCIE_LFSR 
		if(pNandDrv->u8_RequireRandomizer)
			nand_debug(0,1,"Enable Randomizer\n");
		//	NC_EnableLFSR();	//Compatible with ROM Code
		#endif
		NC_Config();
		if(pNandDrv->u8_CellType == NAND_CellType_SLC)
		{
			for(u16_i=0;u16_i<pNandDrv->u16_BlkPageCnt;u16_i++)
			{
				ga_tPairedPageMap[u16_i].u16_LSB = u16_i;
				ga_tPairedPageMap[u16_i].u16_MSB = u16_i;				
			}
		}
		else
		{
			nand_debug(UNFD_DEBUG_LEVEL, 1, "Paired Page Map @0x%08x\n", pNandDrv->u8_PairPageMapLoc);
			u32_Err = NC_ReadPages(u32_PageIdx+pNandDrv->u8_PairPageMapLoc, au8_PageBuf, au8_SpareBuf, 1);
			if (u32_Err != UNFD_ST_SUCCESS)
			{
				nand_debug(UNFD_DEBUG_LEVEL_WARNING, 1, "Skip Blk 0x%08x, Read PPM fail:0x%lX\n",
						   u16_PBA, u32_Err);

				continue;
			}
			else
			{
				memcpy(&ga_tPairedPageMap[0], au8_PageBuf, 2048);
			}
		}

		if((u32_Err == UNFD_ST_SUCCESS))
		{
			nand_debug(UNFD_DEBUG_LEVEL_HIGH, 1, "PartInfo @0x%08x\n", ga_tPairedPageMap[1].u16_LSB);
			pNandDrv->u8_HasPNI = 1;
			u32_Err = drvNAND_CheckPartInfo(u32_PageIdx + ga_tPairedPageMap[1].u16_LSB);
			if (u32_Err != UNFD_ST_SUCCESS) {
				pNandDrv->u8_HasPNI = 0;
				nand_debug(UNFD_DEBUG_LEVEL, 1, "No PartInfo\n");
			}
		}

		pNandDrv->u8_CISBlk = (u16_PBA<<u8_BlkPageCntBits)>>pNandDrv->u8_BlkPageCntBits;
		nand_debug(UNFD_DEBUG_LEVEL_WARNING, 0, "CIS is found @Blk%d\n", pNandDrv->u8_CISBlk);
		
		return UNFD_ST_SUCCESS;
	}

	nand_debug(UNFD_DEBUG_LEVEL, 1, "No CIS found\n");
	return UNFD_ST_ERR_NO_CIS;
}

U32 drvNAND_SearchCIS1(U8 *u8_CIS1Blk)
{
	NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();
	U8 *au8_PageBuf = (U8*)pNandDrv->pu8_PageDataBuf;
	U8 *au8_SpareBuf = (U8*)pNandDrv->pu8_PageSpareBuf;
	NAND_FLASH_INFO_t *pNandInfo = (NAND_FLASH_INFO_t*)au8_PageBuf;
	BLK_INFO_t *pBlkInfo = (BLK_INFO_t*)au8_SpareBuf;
	U16 u16_PBA;
	U32 u32_PageIdx;
	U32 u32_Err;
	U32 u32_ChkSum;
	U16 u16_PageByteCnt;
	U16 u16_SpareByteCnt;
	U16 u16_ECCType;

	for (u16_PBA = pNandDrv->u8_CISBlk+1 ; u16_PBA < 10; u16_PBA++)
	{
		u16_PageByteCnt = pNandDrv->u16_PageByteCnt;
		u16_SpareByteCnt = pNandDrv->u16_SpareByteCnt;
		u16_ECCType = pNandDrv->u16_ECCType;
		
		pNandDrv->u16_PageByteCnt = 2048;
		pNandDrv->u16_SpareByteCnt = 256;
		pNandDrv->u16_ECCType = NANDINFO_ECC_TYPE;
		
		NC_ConfigNandFlashContext();
		NC_RegInit();
		pNandDrv->u16_Reg48_Spare |= BIT_NC_HW_AUTO_RANDOM_CMD_DISABLE;
		//force disable Randomizer
		#if defined(FCIE_LFSR) && FCIE_LFSR	
		NC_DisableLFSR();
		#endif
		NC_Config();
		u32_PageIdx = u16_PBA << pNandDrv->u8_BlkPageCntBits;

		#if defined(ENABLE_NAND_RIU_MODE)&&ENABLE_NAND_RIU_MODE
		u32_Err = NC_ReadSector_RIUMode(u32_PageIdx, 0, au8_PageBuf, au8_SpareBuf);
		#else
		u32_Err = NC_ReadSectors(u32_PageIdx, 0, au8_PageBuf, au8_SpareBuf, 1);
		#endif
		#if 0		//WordMode is unsupported
		if (u32_Err != UNFD_ST_SUCCESS && 0==(pNandDrv->u16_Reg58_DDRCtrl & BIT_DDR_MASM))
		{
			//nand_debug(0, 0, "NAND, change WordMode, now: %X\n", pNandDrv->u8_WordMode);

			// If read sector fail, change u8_WordMode and try again
			if (pNandDrv->u8_WordMode == 0)
			{
				pNandDrv->u8_WordMode = 1;
				pNandDrv->u16_Reg50_EccCtrl |= BIT_NC_WORD_MODE;
			}
			else
			{
				pNandDrv->u8_WordMode = 0;
				pNandDrv->u16_Reg50_EccCtrl &= (~BIT_NC_WORD_MODE);
			}

			#if defined(ENABLE_NAND_RIU_MODE)&&ENABLE_NAND_RIU_MODE
			u32_Err = NC_ReadSector_RIUMode(u32_PageIdx, 0, au8_PageBuf, au8_SpareBuf);
			#else
			u32_Err = NC_ReadSectors(u32_PageIdx, 0, au8_PageBuf, au8_SpareBuf, 1);
			#endif
			//nand_debug(UNFD_DEBUG_LEVEL_HIGH, 0, "NC_ReadSectors2=%x\n", u32_Err);
		}
		#endif
	
		if (u32_Err != UNFD_ST_SUCCESS || pBlkInfo->u8_BadBlkMark != 0xFF)
		{
			continue;
		}

		/* Valid CIS has block address 0 or 1 */
		//if (pBlkInfo->u16_BlkAddr > 1)
		//	continue;

		if (drvNAND_CompareCISTag(pNandInfo->au8_Tag))
		{
			continue;
		}

		u32_ChkSum = drvNAND_CheckSum(au8_PageBuf + 0x24, 0x32 - 0x24);
		if (u32_ChkSum != pNandInfo->u32_ChkSum)
		{
			nand_debug(UNFD_DEBUG_LEVEL_WARNING, 1, "Skip Blk 0x%08x, ChkSum: 0x%08lx != 0x%08lx\n",
				u16_PBA, u32_ChkSum, pNandInfo->u32_ChkSum);

			continue;
		}

		pNandDrv->u16_PageByteCnt = u16_PageByteCnt;
		pNandDrv->u16_SpareByteCnt = u16_SpareByteCnt;
		pNandDrv->u16_ECCType = u16_ECCType;
		
		NC_ConfigNandFlashContext();
		NC_RegInit();
		pNandDrv->u16_Reg48_Spare &= ~BIT_NC_HW_AUTO_RANDOM_CMD_DISABLE;

		#if defined(FCIE_LFSR) && FCIE_LFSR 
		if(pNandDrv->u8_RequireRandomizer)
			nand_debug(0,1,"Enable Randomizer\n");
		//	NC_EnableLFSR();							//Compatible with ROM Code
		#endif
		NC_Config();
		if(pNandDrv->u8_CellType == NAND_CellType_SLC)
		{
			// do nothing
		}
		else
		{
			nand_debug(UNFD_DEBUG_LEVEL_HIGH, 1, "Paired Page Map @0x%08x\n", pNandDrv->u8_PairPageMapLoc);
			u32_Err = NC_ReadPages(u32_PageIdx+pNandDrv->u8_PairPageMapLoc, au8_PageBuf, au8_SpareBuf, 1);
			if (u32_Err != UNFD_ST_SUCCESS)
			{
				nand_debug(UNFD_DEBUG_LEVEL_WARNING, 1, "Skip Blk 0x%08x, Read PPM fail:0x%lX\n",
						   u16_PBA, u32_Err);

				continue;
			}
			else
			{
				if(memcmp((const void *) &ga_tPairedPageMap[0], (const void *) au8_PageBuf, 2048) != 0)
					continue;
			}
		}

		if((u32_Err == UNFD_ST_SUCCESS))
		{
			nand_debug(UNFD_DEBUG_LEVEL_HIGH, 1, "PartInfo @0x%08x\n", ga_tPairedPageMap[1].u16_LSB);
			if(pNandDrv->u8_HasPNI == 1)
			{
				u32_Err = drvNAND_CheckPartInfo(u32_PageIdx + ga_tPairedPageMap[1].u16_LSB);
				if (u32_Err != UNFD_ST_SUCCESS)
					continue;
			}
		}

		nand_debug(UNFD_DEBUG_LEVEL_WARNING, 0, "CIS1 is found @Blk%d\n", u16_PBA);
		*u8_CIS1Blk = u16_PBA;

		return UNFD_ST_SUCCESS;
	}

	nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "No CIS1 found\n");
	return UNFD_ST_ERR_NO_CIS;
}

U32 drvNAND_InitBBT(INIT_BBT_t *pInitBBT)
{
	NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();
	U8 *au8_PageBuf = (U8*)pNandDrv->pu8_PageDataBuf;
	U8 *au8_SpareBuf = (U8*)pNandDrv->pu8_PageSpareBuf;
	U32 u32_PageIdx;
	U32 u32_Err;
	U32 u32_i, u32_k;
	U8 u8_IsBadBlk;
	U8 u8_BadBlkMarker = (pNandDrv->u32_Config>>1)&0x07;
	U8 u8_BusWidth = (pNandDrv->u32_Config>>7)&0x01;

	nand_debug(UNFD_DEBUG_LEVEL_HIGH, 1, "Bad Blk Col Addr=0x%X\n", u8_BadBlkMarker);
	nand_debug(UNFD_DEBUG_LEVEL_HIGH, 1, "Bus Width=0x%X\n", u8_BusWidth);
    //pInitBBT->u16_Cnt = 0;
	for(u32_i=0; u32_i<pNandDrv->u16_BlkCnt; u32_i++)
	{
		u8_IsBadBlk = 0;
		#if 0
		if(drvNAND_ErasePhyBlk(u32_i))
		{
			u8_IsBadBlk = 1;
			nand_debug(0,1,"bad blk: %X from erase \n", u32_i);
		}
		#endif
		for(u32_k=0; u32_k<3; u32_k++)
		{
			u32_PageIdx = (u32_i << pNandDrv->u8_BlkPageCntBits) + u32_k;
			u32_Err = NC_ReadSectors(u32_PageIdx, 0, au8_PageBuf, au8_SpareBuf, 1);
			if (u32_Err != UNFD_ST_SUCCESS )
				nand_debug(UNFD_DEBUG_LEVEL_HIGH, 1, "NC_ReadSectors(0x%lX)=0x%lX\n", u32_PageIdx, u32_Err);
			if(u8_BusWidth == 0) // 8-bit mode
			{
				if(au8_SpareBuf[u8_BadBlkMarker] != 0xFF)
				{
					u8_IsBadBlk = 1;
					break;
				}
			}
			else // 16-bit mode
			{
				if(au8_SpareBuf[(u8_BadBlkMarker<<1)] != 0xFF || au8_SpareBuf[(u8_BadBlkMarker<<1)+1] != 0xFF)
				{
					u8_IsBadBlk = 1;
					break;
				}
			}

			u32_PageIdx = ((u32_i + 1) << pNandDrv->u8_BlkPageCntBits) - 1 - u32_k;
			u32_Err = NC_ReadSectors(u32_PageIdx, 0, au8_PageBuf, au8_SpareBuf, 1);
			if (u32_Err != UNFD_ST_SUCCESS )
				nand_debug(UNFD_DEBUG_LEVEL_HIGH, 1, "NC_ReadSectors(0x%lX)=0x%lX\n", u32_PageIdx, u32_Err);
			if(u8_BusWidth == 0) // 8-bit mode
			{
				if(au8_SpareBuf[u8_BadBlkMarker] != 0xFF)
				{
					u8_IsBadBlk = 1;
					break;
				}
			}
			else // 16-bit mode
			{
				if(au8_SpareBuf[(u8_BadBlkMarker<<1)] != 0xFF || au8_SpareBuf[(u8_BadBlkMarker<<1)+1] != 0xFF)
				{
					u8_IsBadBlk = 1;
					break;
				}
			}
		}

        if (u8_IsBadBlk == 1)
        {
            nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Bad Blk = 0x%lX\n", u32_i);

            if (pInitBBT->u16_Cnt <= 250)
            {
                pInitBBT->au8_BadBlkTbl[(pInitBBT->u16_Cnt<<1)] = (u32_i & 0xFF);
                pInitBBT->au8_BadBlkTbl[(pInitBBT->u16_Cnt<<1)+1] = ((u32_i >> 8) & 0xFF);
            }
			pInitBBT->u16_Cnt++;
            drvNAND_MarkBadBlk(u32_i);
        }
    }

    nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Total Bad Blk = 0x%X\n", pInitBBT->u16_Cnt);

	pInitBBT->u32_ChkSum = drvNAND_CheckSum((U8*)(&(pInitBBT->u16_Cnt)), (512-4));
	return UNFD_ST_SUCCESS;
}


//--------------------------------------------------------------------------
static UNFD_ALIGN0 TEST_ALIGN_PACK_t g_TestAlignPack_t UNFD_ALIGN1;

U32 drvNAND_CheckAlignPack(U8 u8_AlignByteCnt)
{
	// check alignment
	if((U32)&(g_TestAlignPack_t.u8_0) & (u8_AlignByteCnt-1))
	{
		nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "ERROR, not aliged. expect %X but %lX \n",
			u8_AlignByteCnt, (U32)&(g_TestAlignPack_t.u8_0));
		nand_die();
		return UNFD_ST_ERR_NOT_ALIGN;
	}
	// check packed - 0
	if((U32)&(g_TestAlignPack_t.u16_0)-(U32)&(g_TestAlignPack_t.u8_0) != 1 ||
	   (U32)&(g_TestAlignPack_t.u32_0)-(U32)&(g_TestAlignPack_t.u8_0) != 3 ||
	   (U32)&(g_TestAlignPack_t.u32_1)-(U32)&(g_TestAlignPack_t.u8_0) != 7)
	{
		nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "ERROR, not packed. check err.0 \n");
		nand_die();
		return UNFD_ST_ERR_NOT_PACKED;
	}
	// check packed - 1
	if((U32)&(g_TestAlignPack_t.u16_0)-(U32)&(g_TestAlignPack_t.u8_0) != 1 ||
	   (U32)&(g_TestAlignPack_t.u32_0)-(U32)&(g_TestAlignPack_t.u16_0)!= 2 ||
	   (U32)&(g_TestAlignPack_t.u32_1)-(U32)&(g_TestAlignPack_t.u32_0)!= 4)
	{
		nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "ERROR, not packed. check err.1 \n");
		nand_die();
		return UNFD_ST_ERR_NOT_PACKED;
	}

	nand_debug(UNFD_DEBUG_LEVEL_HIGH, 1, "ok\n");
	return UNFD_ST_SUCCESS;
}
//--------------------------------------------------------------------------

static __inline void dump_mem_line(unsigned char *buf, int cnt)
{
	int i;

	nand_debug(UNFD_DEBUG_LEVEL_WARNING, 0, " 0x%08lx: ", (U32)buf);
	
	for (i= 0; i < cnt; i++)
		nand_debug(UNFD_DEBUG_LEVEL_WARNING, 0, "%02X ", buf[i]);

	nand_debug(UNFD_DEBUG_LEVEL_WARNING, 0, " | ");

	for (i = 0; i < cnt; i++)
		nand_debug(UNFD_DEBUG_LEVEL_WARNING, 0, "%c", (buf[i] >= 32 && buf[i] < 128) ? buf[i] : '.');

	nand_debug(UNFD_DEBUG_LEVEL_WARNING, 0, "\n");
}

void dump_mem(unsigned char *buf, int cnt)
{
	int i;

	for (i= 0; i < cnt; i+= 16)
		dump_mem_line(buf + i, 16);
}

U32 nand_CompareData(U8 *pu8_Buf0, U8 *pu8_Buf1, U32 u32_ByteCnt)
{
	U32 u32_i;

	for(u32_i=0; u32_i<u32_ByteCnt; u32_i++)
	{
		if(pu8_Buf0[u32_i] != pu8_Buf1[u32_i])
		{
			nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "eMMC Err: data mismatch: Buf0[%lXh]: %02Xh, Buf1[%lXh]: %02Xh\n",
				u32_i, pu8_Buf0[u32_i], u32_i, pu8_Buf1[u32_i]);
            
            nand_debug(0, 0, "buf0[%lXh]:\n", u32_i&(~0xF));
            dump_mem((U8*)&pu8_Buf0[u32_i&(~0xF)], 0x20);
            nand_debug(0, 0, "buf1[%lXh]:\n", u32_i&(~0xF));
            dump_mem((U8*)&pu8_Buf1[u32_i&(~0xF)], 0x20);
			break;
		}
	}
    if(u32_i != u32_ByteCnt)
		return UNFD_ST_ERR_DATA_CMP_FAIL;

	return UNFD_ST_SUCCESS;
}

void dump_nand_info(NAND_FLASH_INFO_t *pNandInfo)
{
	int i;
	nand_debug(UNFD_DEBUG_LEVEL_LOW, 1, "###############################################\n");
	nand_debug(UNFD_DEBUG_LEVEL_LOW, 1, "#        NAND INFO                            #\n");
	nand_debug(UNFD_DEBUG_LEVEL_LOW, 1, "###############################################\n");

	nand_debug(UNFD_DEBUG_LEVEL_LOW, 1, "pNandInfo: 0x%08lx\n", (U32)pNandInfo);
	nand_debug(UNFD_DEBUG_LEVEL_LOW, 1, "au8_Tag          : [");
	for (i = 0; i < 16; i++)
		nand_debug(UNFD_DEBUG_LEVEL_LOW, 0, "%c", pNandInfo->au8_Tag[i]);
	nand_debug(UNFD_DEBUG_LEVEL_LOW, 0, "]\n");

	nand_debug(UNFD_DEBUG_LEVEL_LOW, 1, "u8_IDByteCnt     : 0x%04x\n", pNandInfo->u8_IDByteCnt);

	nand_debug(UNFD_DEBUG_LEVEL_LOW, 1, "au8_ID           : 0x[ ");
	for (i = 0; i < pNandInfo->u8_IDByteCnt; i++)
		nand_debug(UNFD_DEBUG_LEVEL_LOW, 0, "%02X ", pNandInfo->au8_ID[i]);
	nand_debug(UNFD_DEBUG_LEVEL_LOW, 0, "]\n");

	nand_debug(UNFD_DEBUG_LEVEL_LOW, 1, "u32_ChkSum       : 0x%04lx\n", pNandInfo->u32_ChkSum);
	nand_debug(UNFD_DEBUG_LEVEL_LOW, 1, "u16_SpareByteCnt : 0x%04x\n", pNandInfo->u16_SpareByteCnt);
	nand_debug(UNFD_DEBUG_LEVEL_LOW, 1, "u16_PageByteCnt  : 0x%04x\n", pNandInfo->u16_PageByteCnt);
	nand_debug(UNFD_DEBUG_LEVEL_LOW, 1, "u16_BlkPageCnt   : 0x%04x\n", pNandInfo->u16_BlkPageCnt);
	nand_debug(UNFD_DEBUG_LEVEL_LOW, 1, "u16_BlkCnt       : 0x%04x\n", pNandInfo->u16_BlkCnt);
	nand_debug(UNFD_DEBUG_LEVEL_LOW, 1, "u32_Config       : 0x%08lx\n", pNandInfo->u32_Config);
	nand_debug(UNFD_DEBUG_LEVEL_LOW, 1, "u16_ECCType      : 0x%04x\n", pNandInfo->u16_ECCType);
	nand_debug(UNFD_DEBUG_LEVEL_LOW, 1, "u16_tRC: 0x%04x\n\n", pNandInfo->u16_tRC);

	//dump_mem_line((U8*)pNandInfo, sizeof(NAND_FLASH_INFO_t));
	nand_debug(UNFD_DEBUG_LEVEL_LOW, 1, "\n");
	
}

void dump_part_records(PARTITION_RECORD_t *records, int cnt)
{
	int i;

	if (cnt > 1024) {
		nand_debug(0, 1, "broken\n");
		return;
	}

	for (i = 0; i < cnt; i++) {
		nand_debug(0, 1, "record[%d]       : 0x%04x, 0x%04x, 0x%04x, 0x%04x\n",
				i,
				records[i].u16_StartBlk, records[i].u16_BlkCnt,
				records[i].u16_PartType, records[i].u16_BackupBlkCnt);
	}
}

void dump_general_blk_info(BLK_INFO_t *pBlkInfo)
{
	nand_debug(UNFD_DEBUG_LEVEL_LOW, 1, "u8_BadBlkIdx    : 0x%04x\n", pBlkInfo->u8_BadBlkMark);
	nand_debug(UNFD_DEBUG_LEVEL_LOW, 1, "u8_PartType     : 0x%04x\n", pBlkInfo->u8_PartType);
	nand_debug(UNFD_DEBUG_LEVEL_LOW, 1, "u16_BlkAddr     : 0x%04x\n", pBlkInfo->u16_BlkAddr);
	//nand_debug(UNFD_DEBUG_LEVEL_LOW, 1, "au8_Misc        : 0x%04x\n", *(U16*)pBlkInfo->au8_Misc);
}

void dump_part_info(PARTITION_INFO_t *pPartInfo)
{
	nand_debug(0, 1, "###############################################\n");
	nand_debug(0, 1, "#        PART INFO                            #\n");
	nand_debug(0, 1, "###############################################\n");

	nand_debug(0, 1, "pPartInfo: 0x%08lx\n", (U32)pPartInfo);
	nand_debug(0, 1, "u32_ChkSum      : 0x%04lx\n", pPartInfo->u32_ChkSum);
//	nand_debug(UNFD_DEBUG_LEVEL_LOW, 1, "u32_BLSectorCnt : 0x%04lx\n", pPartInfo->u32_BLSectorCnt);
//	nand_debug(UNFD_DEBUG_LEVEL_LOW, 1, "u32_OSSectorCnt : 0x%04lx\n", pPartInfo->u32_OSSectorCnt);
	nand_debug(0, 1, "u16_PartCnt     : 0x%04x\n", pPartInfo->u16_PartCnt);
	nand_debug(0, 1, "u16_UnitByteCnt : 0x%04x\n", pPartInfo->u16_UnitByteCnt);

	dump_part_records(pPartInfo->records, pPartInfo->u16_PartCnt);
}

void dump_miu_records(MIU_RECORD_t *pRecords, int cnt)
{
	int i;

	if (cnt > (1024 - 8)) {
		nand_debug(UNFD_DEBUG_LEVEL_LOW, 1, "broken\n");
		return;
	}
	for (i = 0; i < (cnt >> 3); i++)
		nand_debug(UNFD_DEBUG_LEVEL_LOW, 1, "miu record[%02d]  : 0x%08lx, 0x%08lx\n",
				i, pRecords[i].u32_RegAddr, pRecords[i].u32_RegValue);
}

void dump_miu_part(MIU_PART_t *pMiuPart)
{
	nand_debug(UNFD_DEBUG_LEVEL_LOW, 1, "###############################################\n");
	nand_debug(UNFD_DEBUG_LEVEL_LOW, 1, "#        MIU PART                             #\n");
	nand_debug(UNFD_DEBUG_LEVEL_LOW, 1, "###############################################\n");

	nand_debug(UNFD_DEBUG_LEVEL_LOW, 1, "pMiuPart: 0x%08lx\n", (U32)pMiuPart);
	nand_debug(UNFD_DEBUG_LEVEL_LOW, 1, "u32_ChkSum      : 0x%04lx\n", pMiuPart->u32_ChkSum);
	nand_debug(UNFD_DEBUG_LEVEL_LOW, 1, "u32_ByteCnt     : 0x%04lx\n", pMiuPart->u32_ByteCnt);

	dump_miu_records(pMiuPart->records, pMiuPart->u32_ByteCnt);
}

void dump_nand_driver(NAND_DRIVER *pNandDrv)
{
	U16 u16_i;
	
	nand_debug(0, 1, "NAND_Info:\n");

    nand_debug(0, 1, "  ID [%u bytes]           : ", pNandDrv->u8_IDByteCnt);
    for(u16_i=0; u16_i<pNandDrv->u8_IDByteCnt; u16_i++)
		nand_debug(0, 0, "%02Xh ", pNandDrv->au8_ID[u16_i]);
    nand_debug(0, 0, "\n");
    
	nand_debug(0, 1, "  BlkCnt                 : 0x%X\n", pNandDrv->u16_BlkCnt);
	nand_debug(0, 1, "  BlkPageCnt             : 0x%X\n", pNandDrv->u16_BlkPageCnt);
	nand_debug(0, 1, "  BlkSectorCnt           : 0x%X\n", pNandDrv->u16_BlkSectorCnt);
	nand_debug(0, 1, "  PageByteCnt            : 0x%X\n", pNandDrv->u16_PageByteCnt);
	nand_debug(0, 1, "  SpareByteCnt           : 0x%X\n\n", pNandDrv->u16_SpareByteCnt);

	nand_debug(0, 1, "  BlkPageCntBits         : 0x%X\n", pNandDrv->u8_BlkPageCntBits);
	nand_debug(0, 1, "  BlkSectorCntBits       : 0x%X\n", pNandDrv->u8_BlkSectorCntBits);
	nand_debug(0, 1, "  PageByteCntBits        : 0x%X\n", pNandDrv->u8_PageByteCntBits);

	nand_debug(0, 1, "  BlkPageCntMask         : 0x%X\n", pNandDrv->u16_BlkPageCntMask);
	nand_debug(0, 1, "  BlkSectorCntMask       : 0x%X\n", pNandDrv->u16_BlkSectorCntMask);
	nand_debug(0, 1, "  PageByteCntMask        : 0x%X\n", pNandDrv->u16_PageByteCntMask);

	nand_debug(0, 1, "  PageSectorCnt          : 0x%X\n", pNandDrv->u16_PageSectorCnt);
	nand_debug(0, 1, "  SectorByteCnt          : 0x%X\n", pNandDrv->u16_SectorByteCnt);
	nand_debug(0, 1, "  SectorSpareByteCnt     : 0x%X\n", pNandDrv->u16_SectorSpareByteCnt);
	nand_debug(0, 1, "  ECCCodeByteCnt         : 0x%X\n\n", pNandDrv->u16_ECCCodeByteCnt);

	nand_debug(0, 1, "  PageSectorCntBits      : 0x%X\n", pNandDrv->u8_PageSectorCntBits);
	nand_debug(0, 1, "  SectorByteCntBits      : 0x%X\n", pNandDrv->u8_SectorByteCntBits);
	nand_debug(0, 1, "  PageSectorCntMask      : 0x%X\n", pNandDrv->u16_PageSectorCntMask);
	nand_debug(0, 1, "  SectorByteCntMask      : 0x%X\n\n", pNandDrv->u16_SectorByteCntMask);

    nand_debug(0, 1, "  CellType               : %s\n", pNandDrv->u8_CellType==NAND_CellType_MLC?"MLC":"SLC");
    nand_debug(0, 1, "  RequireRandomizer      : %s\n", pNandDrv->u8_RequireRandomizer?"Yes":"No");
    nand_debug(0, 1, "  RequireReadRetry       : %s\n", pNandDrv->u8_RequireReadRetry?"Yes":"No");
    nand_debug(0, 1, "  MaxCENum               : 0x%X\n", pNandDrv->u8_MaxCENum);
    
	nand_debug(0, 1, "  OpCode_Erase_AdrCycle  : 0x%X\n", pNandDrv->u8_OpCode_Erase_AdrCycle);
	nand_debug(0, 1, "  OpCode_RW_AdrCycle     : 0x%X\n", pNandDrv->u8_OpCode_RW_AdrCycle);
	nand_debug(0, 1, "  AddrCycleIdx           : 0x%X\n\n", pNandDrv->u8_AddrCycleIdx);

	nand_debug(0, 1, "  u16_tRC;    : 0x%X\n", pNandDrv->u16_tRC);
	nand_debug(0, 1, "  u8_tRP      : 0x%X\n", pNandDrv->u8_tRP);
	nand_debug(0, 1, "  u8_tREH     : 0x%X\n", pNandDrv->u8_tREH);
	nand_debug(0, 1, "  u8_tREA     : 0x%X\n", pNandDrv->u8_tREA);
	nand_debug(0, 1, "  u8_tRR      : 0x%X\n", pNandDrv->u8_tRR);
	nand_debug(0, 1, "  u16_tADL    : 0x%X\n", pNandDrv->u16_tADL);
	nand_debug(0, 1, "  u16_tRHW    : 0x%X\n", pNandDrv->u16_tRHW);
	nand_debug(0, 1, "  u16_tWHR    : 0x%X\n", pNandDrv->u16_tWHR);
	nand_debug(0, 1, "  u16_tCCS    : 0x%X\n", pNandDrv->u16_tCCS);
	nand_debug(0, 1, "  u8_tCS      : 0x%X\n", pNandDrv->u8_tCS);
	nand_debug(0, 1, "  u16_tWC     : 0x%X\n", pNandDrv->u16_tWC);
	nand_debug(0, 1, "  u8_tWP      : 0x%X\n", pNandDrv->u8_tWP);
	nand_debug(0, 1, "  u8_tWH      : 0x%X\n", pNandDrv->u8_tWH);
	nand_debug(0, 1, "  u16_tCWAW   : 0x%X\n", pNandDrv->u16_tCWAW);
	nand_debug(0, 1, "  u8_tCLHZ    : 0x%X\n", pNandDrv->u8_tCLHZ);
	nand_debug(0, 1, "  u16_tWW     : 0x%X\n", pNandDrv->u16_tWW);

	nand_debug(0, 1, "  BBtPageCheckCount : 0x%X\n", pNandDrv->u16_BBtPageCheckCount);
	for(u16_i=0; u16_i<pNandDrv->u16_BBtPageCheckCount; u16_i++)
		nand_debug(0, 1, "  Page:%02Xh  Mark:%02Xh \n", 
		    pNandDrv->u16_BBtPageIdx[pNandDrv->u16_BBtPageCheckCount], 
		    pNandDrv->u16_BBtMarker[pNandDrv->u16_BBtPageCheckCount]);

	nand_debug(0, 1, "  PhySpareByteCnt           : 0x%X\n\n", pNandDrv->u16_PhySpareByteCnt);		
}

void dump_nand_PPM(void)
{
    NAND_DRIVER *pNandDrv = (NAND_DRIVER*)drvNAND_get_DrvContext_address();
    U16 u16_i;
    
    nand_debug(0,1,"\n");

    if(pNandDrv->u8_CellType == NAND_CellType_SLC)
    {
        nand_debug(0,1," SLC has no PPM \n");
        return;
    }
    
    nand_debug(0,1,"    LP      HP\n");
    for(u16_i=0; u16_i<pNandDrv->u16_BlkPageCnt/2; u16_i++)
        nand_debug(0,1,"  %4u    %4u\n", 
            ga_tPairedPageMap[u16_i].u16_LSB, ga_tPairedPageMap[u16_i].u16_MSB);

    nand_debug(0,1,"\n");
}

U8 drvNAND_CountBits(U32 u32_x)
{
	U8 u8_i = 0;

    if(u32_x==0) return u8_i;
    
	while (u32_x) {
		u8_i++;
		u32_x >>= 1;
	}

	return u8_i-1;
}

U32 drvNAND_CheckSum(U8 *pu8_Data, U32 u32_ByteCnt)
{
	U32 u32_Sum = 0;

	while (u32_ByteCnt--)
		u32_Sum += *pu8_Data++;

	return u32_Sum;
}

/* return 0: same, 1: different */
U32 drvNAND_CompareCISTag(U8 *tag)
{
	const char *str = "MSTARSEMIUNFDCIS";
	int i = 0;
	
	for (i = 0; i < 16; i++) {
		if (tag[i] != str[i])
			return 1;
	}

	return 0;
}

/* Search for partition of type @u16_PartType, begin from @pRecord */
PARTITION_RECORD_t *drvNAND_SearchPartition(PARTITION_RECORD_t *pRecord,
											U16 u16_PartType)
{
	NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();
	PARTITION_INFO_t *pPartInfo = pNandDrv->pPartInfo;

	while (pRecord - pPartInfo->records < pPartInfo->u16_PartCnt) {

		if (pRecord->u16_PartType == u16_PartType)
			return pRecord;

		pRecord++;
	}

	return (void*)0;
}

/* return 1: Good block, 0: Bad block */
U32 drvNAND_CheckSpareSpacePattern(U8 *au8_SpareBuf, U32 u32_PageIdx)
{
	U16 u16_PageIdxInBlk;
	NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();

	u16_PageIdxInBlk = u32_PageIdx & pNandDrv->u16_BlkPageCntMask;
	if(au8_SpareBuf[pNandDrv->u16_BBtMarker[u16_PageIdxInBlk]] != 0xff)
		return 0;
	return 1;
}

/* return 1: Good block, 0: Bad block */
U32 drvNAND_IsGoodBlk(U16 u16_PBA)
{
	NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();
	U8 *au8_DataBuf = (U8*)pNandDrv->pu8_PageDataBuf;
	U8 *au8_SpareBuf = (U8*)pNandDrv->pu8_PageSpareBuf;
	U32 u32_PageIdx;
	U32 u32_Err;
	U32 i;
	BLK_INFO_t *pBlkInfo = (BLK_INFO_t*)au8_SpareBuf;

	// check by nni
	if(pNandDrv->u16_BBtPageCheckCount > 0 && pNandDrv->u16_BBtPageCheckCount <= NAND_BBT_PAGE_COUNT){
		for(i=0; i<pNandDrv->u16_BBtPageCheckCount; i++)
		{
			u32_PageIdx = (u16_PBA << pNandDrv->u8_BlkPageCntBits) + pNandDrv->u16_BBtPageIdx[i];
			u32_Err = NC_ReadPages(u32_PageIdx, au8_DataBuf, au8_SpareBuf, 1);
			if(u32_Err != UNFD_ST_SUCCESS){
				nand_debug(UNFD_DEBUG_LEVEL_HIGH, 1, "NC_ReadPage(Row:0x%X), Err:0x%X \n", (unsigned int)u32_PageIdx, (unsigned int)u32_Err);
//				u32_Err = drvNAND_MarkBadBlk(u16_PBA);
//				if(u32_Err != UNFD_ST_SUCCESS)
//					nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Mark bad block(0x%X) failed!!\n", (unsigned int)u16_PBA);
//				return 0;	
			}
			if(!drvNAND_CheckSpareSpacePattern(au8_SpareBuf, u32_PageIdx))
				return 0;
		}
		return 1;
	}

	// old way checking
	for(i=0; i<4; i++)
	{
		u32_PageIdx = u16_PBA << pNandDrv->u8_BlkPageCntBits;
		
	    switch(i)
	    {
			case 0:
			    break;
			case 1: u32_PageIdx++;
			    break;
			case 2: u32_PageIdx += pNandDrv->u16_BlkPageCnt-1;
			    break;
			case 3: u32_PageIdx += pNandDrv->u16_BlkPageCnt-2;
			    break;
	    }
		//u32_Err = NC_ReadSector_RIUMode(u32_PageIdx, 0, au8_DataBuf, au8_SpareBuf);
	    u32_Err = NC_ReadSectors(u32_PageIdx, 0, au8_DataBuf, au8_SpareBuf, 1);
	    if (u32_Err != UNFD_ST_SUCCESS)
            nand_debug(UNFD_DEBUG_LEVEL_HIGH, 1, "NC_ReadSectors(0x%X)=0x%X\n", (unsigned int)u32_PageIdx, (unsigned int)u32_Err);
	
	    if (pBlkInfo->u8_BadBlkMark != 0xFF)
		    return 0; // bad blk
	}

	return 1; // good blk
}


U8 drvNAND_CheckAll0xFF(U8* pu8_Buf, U32 u32_ByteCnt)
{
	register U32 u32_i;
    #if 1 // faster
    register U32 u32_j, *pu32;
    register U8 *pu8;
    
    pu8 = pu8_Buf;
    pu32 = (U32*)((U32)(pu8_Buf + 3) & ~3);
    u32_ByteCnt -= (U32)pu32 - (U32)pu8;
    
    for(; (U32)pu8<(U32)pu32; pu8++)
        if(0xFF != *pu8)
			return 0;

    u32_j = u32_ByteCnt>>2;
    
	for(u32_i=0; u32_i < u32_j; u32_i++)
		if(0xFFFFFFFF != pu32[u32_i])
			return 0;

    if(u32_ByteCnt == u32_i<<2)
        return 0xFF;

    u32_ByteCnt -= u32_i<<2;
    pu8 = (U8*)&pu32[u32_i];
    
    for(u32_i=0; u32_i < u32_ByteCnt; u32_i++)
        if(0xFF != pu8[u32_i])
            return 0;
    #else
    for(u32_i=0; u32_i < u32_ByteCnt; u32_i++)
		if(0xFF != pu8_Buf[u32_i])
			return 0;
    #endif
	return 0xFF;
}


/* return 1: Free block, 0: not Free block */
U32 drvNAND_IsFreeBlk(U16 u16_PBA)
{
	NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();
	U8 *au8_SectorBuf = (U8*)pNandDrv->pu8_PageDataBuf;
	U8 *au8_SpareBuf = (U8*)pNandDrv->pu8_PageSpareBuf;
	U32 u32_PageIdx;
	U32 u32_Err;

	// check first page
	u32_PageIdx = u16_PBA << pNandDrv->u8_BlkPageCntBits;
    #if (defined(NC_SEL_FCIE5) && (NC_SEL_FCIE5)) 
	u32_Err = NC_ReadSectors_Retry(u32_PageIdx, 0, au8_SectorBuf, au8_SpareBuf, 1);
    #else
    u32_Err = NC_ReadSectors(u32_PageIdx, 0, au8_SectorBuf, au8_SpareBuf, 1);
    #endif
	if (u32_Err != UNFD_ST_SUCCESS)
        nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Err Code: %Xh\n", (unsigned int)u32_Err);

	if(UNFD_ST_SUCCESS != u32_Err ||
	   0xFF != drvNAND_CheckAll0xFF(au8_SpareBuf, pNandDrv->u16_SectorSpareByteCnt))
		return 0; // not free blk

    #if 0
    // check last page
	u32_PageIdx = ((u16_PBA+1) << pNandDrv->u8_BlkPageCntBits) -1;
	u32_Err = NC_ReadSectors(u32_PageIdx, 0, au8_SectorBuf, au8_SpareBuf, 1);
	if (u32_Err != UNFD_ST_SUCCESS)
		nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "NC_ReadSectors(0x%lX)=0x%lX\n", u32_PageIdx, u32_Err);

    if(0xFF != drvNAND_CheckAll0xFF(au8_SpareBuf, pNandDrv->u16_SectorSpareByteCnt))
		return 0; // not free blk
    #endif
	return 1; // free blk
}


// return the free page
U16 drvNAND_FindFreePage(U16 u16_PBA) // binary search
{
	NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();
	U8  *pu8_SpareBuf = pNandDrv->pu8_PageSpareBuf;
	U8  *pu8_DataBuf = pNandDrv->pu8_PageDataBuf;
	U32 u32_Row, u32_Ret;
    U16 u16_i, dir;
	
	//----------------------------------
    u32_Row = u16_PBA << pNandDrv->u8_BlkPageCntBits;
    #if (defined(NC_SEL_FCIE5) && (NC_SEL_FCIE5)) 
	u32_Ret = NC_ReadSectors_Retry(u32_Row, 0, pu8_DataBuf, pu8_SpareBuf, 1);
    #else
    u32_Ret = NC_ReadSectors(u32_Row, 0, pu8_DataBuf, pu8_SpareBuf, 1);
    #endif
    if(UNFD_ST_SUCCESS==u32_Ret && 
        0xFF==drvNAND_CheckAll0xFF(pu8_SpareBuf, pNandDrv->u16_SectorSpareByteCnt))
	    return 0;

    dir = 1;
    
    for(u16_i=1; u16_i<=pNandDrv->u8_BlkPageCntBits; u16_i++)
	{
        if(dir)
            u32_Row += pNandDrv->u16_BlkPageCnt >> u16_i;
        else
            u32_Row -= pNandDrv->u16_BlkPageCnt >> u16_i;

        #if (defined(NC_SEL_FCIE5) && (NC_SEL_FCIE5)) 
    	u32_Ret = NC_ReadSectors_Retry(u32_Row, 0, pu8_DataBuf, pu8_SpareBuf, 1);
        #else
        u32_Ret = NC_ReadSectors(u32_Row, 0, pu8_DataBuf, pu8_SpareBuf, 1);
        #endif
        if(UNFD_ST_SUCCESS==u32_Ret && 
            0xFF==drvNAND_CheckAll0xFF(pu8_SpareBuf, pNandDrv->u16_SectorSpareByteCnt))
            dir = 0; 
        else
            dir = 1;
    }

	//----------------------------------
	return (u32_Row & pNandDrv->u16_BlkPageCntMask) + dir; 
}


// return free LSB Idx
U16 drvNAND_FindFreeLSBPageIdx(U16 u16_PBA) // binary search
{
	NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();
	U8  *pu8_SpareBuf = pNandDrv->pu8_PageSpareBuf;
	U8  *pu8_DataBuf = pNandDrv->pu8_PageDataBuf;
	U32 u32_Row, u32_Err;
	U16 u16_PIdx1, u16_PIdx2, u16_PIdxN;
	U16 u16_BlkPageCnt;

	//----------------------------------
	// binary search

	u16_BlkPageCnt = pNandDrv->u16_BlkPageCnt >> pNandDrv->u8_CellType;
	u16_PIdx1 = 0;	
	u16_PIdx2 = u16_BlkPageCnt - 1;
	u32_Row = (u16_PBA << pNandDrv->u8_BlkPageCntBits) + ga_tPairedPageMap[u16_PIdx1].u16_LSB;	
	//u32_Err = NC_ReadPages(u32_Row, pu8_DataBuf, pu8_SpareBuf, 1);
	#if (defined(NC_SEL_FCIE5) && (NC_SEL_FCIE5)) 
	u32_Err = NC_ReadSectors_Retry(u32_Row, 0, pu8_DataBuf, pu8_SpareBuf, 1);
    #else
    u32_Err = NC_ReadSectors(u32_Row, 0, pu8_DataBuf, pu8_SpareBuf, 1);
    #endif
	if(0xFF == drvNAND_CheckAll0xFF(pu8_SpareBuf, pNandDrv->u16_SectorSpareByteCnt) 
			&& u32_Err == UNFD_ST_SUCCESS)
		return 0;
	
	u32_Row = (u16_PBA << pNandDrv->u8_BlkPageCntBits) + ga_tPairedPageMap[u16_PIdx2].u16_LSB;	
	//u32_Err = NC_ReadPages(u32_Row, pu8_DataBuf, pu8_SpareBuf, 1);
	#if (defined(NC_SEL_FCIE5) && (NC_SEL_FCIE5)) 
	u32_Err = NC_ReadSectors_Retry(u32_Row, 0, pu8_DataBuf, pu8_SpareBuf, 1);
    #else
    u32_Err = NC_ReadSectors(u32_Row, 0, pu8_DataBuf, pu8_SpareBuf, 1);
    #endif
	if(0 == drvNAND_CheckAll0xFF(pu8_SpareBuf, pNandDrv->u16_SectorSpareByteCnt))
		return u16_BlkPageCnt;//u16_PIdx2;
		
	while( u16_PIdx1 < u16_PIdx2 -1)
	{
		u16_PIdxN = (u16_PIdx1+u16_PIdx2)/2;
		
		u32_Row = (u16_PBA << pNandDrv->u8_BlkPageCntBits) + ga_tPairedPageMap[u16_PIdxN].u16_LSB;	
		//read page pn
		//u32_Err = NC_ReadPages(u32_Row, pu8_DataBuf, pu8_SpareBuf,  1);
		#if (defined(NC_SEL_FCIE5) && (NC_SEL_FCIE5)) 
	    u32_Err = NC_ReadSectors_Retry(u32_Row, 0, pu8_DataBuf, pu8_SpareBuf, 1);
        #else
        u32_Err = NC_ReadSectors(u32_Row, 0, pu8_DataBuf, pu8_SpareBuf, 1);
        #endif
		if(0xFF == drvNAND_CheckAll0xFF(pu8_SpareBuf, pNandDrv->u16_SectorSpareByteCnt) 
			&& u32_Err == UNFD_ST_SUCCESS)
			u16_PIdx2 = u16_PIdxN;
		else
			u16_PIdx1 = u16_PIdxN;
	}

	//u32_Row = ((u16_PBA << pNandDrv->u8_BlkPageCntBits) +ga_tPairedPageMap[u16_PIdx1 + 1].u16_LSB);

	return u16_PIdx1 + 1; //(u32_Row & pNandDrv->u16_BlkPageCntMask);
}


U32 drvNAND_MoveBlkData(U16 u16_DestBlk, U16 u16_SrcBlk, U16 u16_PageCnt)
{
	NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();
	U8 *au8_PageBuf = (U8*)pNandDrv->pu8_PageDataBuf;
	U8 *au8_SpareBuf = (U8*)pNandDrv->pu8_PageSpareBuf;
	U32 u32_Ret;
	U16 u16_i, u32_SrcRow, u32_DestRow;

	u32_SrcRow = u16_SrcBlk << pNandDrv->u8_BlkPageCntBits;
	u32_DestRow = u16_DestBlk << pNandDrv->u8_BlkPageCntBits;

	for(u16_i=0; u16_i<u16_PageCnt; u16_i++)
	{
		u32_Ret = NC_ReadPages(u32_SrcRow + u16_i, au8_PageBuf, au8_SpareBuf, 1);
		if(UNFD_ST_SUCCESS != u32_Ret)
			return u32_Ret;
		u32_Ret = NC_WritePages(u32_DestRow + u16_i, au8_PageBuf, au8_SpareBuf, 1);
		if(UNFD_ST_SUCCESS != u32_Ret)
		{
			drvNAND_MarkBadBlk(u16_DestBlk);
			return u32_Ret;
		}
	}
	return UNFD_ST_SUCCESS;
}


U32 drvNAND_ReadBlk(U8 *pu8_DestAddr, U8 u8_PartType, U16 u16_PBA,
							U16 u16_LBA, U32 u32_StartSector, U32 u32_SectorCnt)
{
	NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();
	U8 *au8_PageBuf = (U8*)pNandDrv->pu8_PageDataBuf;
	U8 *au8_SpareBuf = (U8*)pNandDrv->pu8_PageSpareBuf;

	U32 u32_Err;
	//BLK_INFO_t *pBlkInfo = (BLK_INFO_t*)au8_SpareBuf;
	U16 u16_PageSectorCnt;
	U8 u8_PageSectorCntBits;
	U16 u16_PageSectorCntMask;
	U32 u32_PageIdx;
	U16 u16_PageCnt;
	U16 u16_SectorCntRead;
	U16 u16_SectorIdxInPage;

	nand_debug(UNFD_DEBUG_LEVEL_HIGH, 1, "BlkIdx:%d, BlkNo:%d, StartSector:%d, SectorCnt:%d\n", (unsigned int)u16_PBA, (unsigned int)u16_LBA, (unsigned int)u32_StartSector, (unsigned int)u32_SectorCnt);

	/* sector size from AP is 512 Bytes, not NAND sector size */
	u16_PageSectorCnt = pNandDrv->u16_PageByteCnt >> 9;
	u8_PageSectorCntBits = drvNAND_CountBits(u16_PageSectorCnt);
	u16_PageSectorCntMask = (1<<u8_PageSectorCntBits)-1;

	/* Read unaligned sectors first */
	if( u32_StartSector & u16_PageSectorCntMask )
	{
		u16_SectorIdxInPage = u32_StartSector & u16_PageSectorCntMask;
		u16_SectorCntRead = u32_SectorCnt > ((U32)u16_PageSectorCnt-(U32)u16_SectorIdxInPage) ? ((U32)u16_PageSectorCnt-(U32)u16_SectorIdxInPage) : (u32_SectorCnt);
		u32_PageIdx = (u16_PBA << pNandDrv->u8_BlkPageCntBits) + (u32_StartSector>>u8_PageSectorCntBits);

		u32_Err = NC_ReadPages(u32_PageIdx, au8_PageBuf, (U8*)au8_SpareBuf, 1);
		if (u32_Err != UNFD_ST_SUCCESS) {

			nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Reading page 0x%04x failed with EC: 0x%04x\n",
					   (unsigned int)u32_PageIdx, (unsigned int)u32_Err);

			return u32_Err;
		}
		u16_SectorIdxInPage = u32_StartSector & u16_PageSectorCntMask;
		u16_SectorCntRead = u32_SectorCnt > ((U32)u16_PageSectorCnt-(U32)u16_SectorIdxInPage) ? ((U32)u16_PageSectorCnt-(U32)u16_SectorIdxInPage) : (u32_SectorCnt);

		memcpy(pu8_DestAddr, (au8_PageBuf+(u16_SectorIdxInPage<<9)), (u16_SectorCntRead<<9));
		pu8_DestAddr += (u16_SectorCntRead<<9);
		u32_StartSector += u16_SectorCntRead;
		u32_SectorCnt -= u16_SectorCntRead;
	}

	/* Read aligned sectors then */
	u32_PageIdx = (u16_PBA << pNandDrv->u8_BlkPageCntBits) + (u32_StartSector>>u8_PageSectorCntBits);
	u16_PageCnt = (u32_SectorCnt>>u8_PageSectorCntBits);

	if(u16_PageCnt>0)
	{
		u32_Err = NC_ReadPages(u32_PageIdx, pu8_DestAddr, (U8*)au8_SpareBuf, u16_PageCnt);
		if (u32_Err != UNFD_ST_SUCCESS) {

			nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Reading page 0x%04x failed with EC: 0x%04x\n",
				(unsigned int)u32_PageIdx, (unsigned int)u32_Err);

			return u32_Err;
		}

		pu8_DestAddr += (pNandDrv->u16_PageByteCnt*u16_PageCnt);
		u32_PageIdx += u16_PageCnt;
		u32_StartSector += (u16_PageSectorCnt*u16_PageCnt);
		u32_SectorCnt -= (u16_PageSectorCnt*u16_PageCnt);
	}

	/* Read remaining unaligned sectors finally */
	if( u32_SectorCnt )
	{
		u32_PageIdx = (u16_PBA << pNandDrv->u8_BlkPageCntBits) + (u32_StartSector>>u8_PageSectorCntBits);
		u32_Err = NC_ReadPages(u32_PageIdx, au8_PageBuf, (U8*)au8_SpareBuf, 1);
		if (u32_Err != UNFD_ST_SUCCESS) {

			nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Reading page 0x%04x failed with EC: 0x%04x\n",
					   (unsigned int)u32_PageIdx, (unsigned int)u32_Err);

			return u32_Err;
		}
		memcpy(pu8_DestAddr, au8_PageBuf, (u32_SectorCnt<<9));
	}

	return UNFD_ST_SUCCESS;
}

U32 drvNAND_WriteBlkNonBackup(U8 *pu8_DestAddr, U8 u8_PartType, U16 u16_PBA,
					 U16 u16_LBA,	U32 u32_StartSector,
					 U32 u32_SectorCnt)
{
	NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();
	U8 *au8_PageBuf = (U8*)pNandDrv->pu8_PageDataBuf;
	U8 *au8_SpareBuf = (U8*)pNandDrv->pu8_PageSpareBuf;
	U32 u32_Err;
	BLK_INFO_t *pBlkInfo = (BLK_INFO_t*)au8_SpareBuf;
	U16 u16_PageSectorCnt;
	U8 u8_PageSectorCntBits;
	//U16 u16_PageSectorCntMask;
	U32 u32_PageIdx;
	U16 u16_PageCnt;

	nand_debug(UNFD_DEBUG_LEVEL_HIGH, 1, "BlkIdx:%d, BlkNo:%d, StartSector:%d, SectorCnt:%d\n", (int)u16_PBA, (int)u16_LBA, (int)u32_StartSector, (int)u32_SectorCnt);

	/* sector size from AP is 512 Bytes, not NAND sector size */
	u16_PageSectorCnt = pNandDrv->u16_PageByteCnt >> 9;
	u8_PageSectorCntBits = drvNAND_CountBits(u16_PageSectorCnt);
	//u16_PageSectorCntMask = (1<<u8_PageSectorCntBits)-1;

	memset(au8_SpareBuf, '\0', sizeof(*pBlkInfo));
	pBlkInfo->u8_BadBlkMark = 0xFF;
	pBlkInfo->u8_PartType = u8_PartType;
	pBlkInfo->u16_BlkAddr = u16_LBA;

	u32_PageIdx = (u16_PBA << pNandDrv->u8_BlkPageCntBits) + (u32_StartSector>>u8_PageSectorCntBits);
	u16_PageCnt = (u32_SectorCnt>>u8_PageSectorCntBits);

	while (u16_PageCnt)
	{
		u32_Err = NC_WritePages(u32_PageIdx, pu8_DestAddr, au8_SpareBuf, 1);
		if (u32_Err != UNFD_ST_SUCCESS) {

			nand_debug(2, 1, "Writting page 0x%04lx failed with EC: 0x%04lx\n",
					   u32_PageIdx, u32_Err);

			return u32_Err;
		}

		pu8_DestAddr += pNandDrv->u16_PageByteCnt;
		u32_PageIdx++;
		u16_PageCnt--;
		u32_SectorCnt -= u16_PageSectorCnt;
	}

	if( u32_SectorCnt )
	{
		memset(au8_PageBuf, 0xFF, pNandDrv->u16_PageByteCnt);
		memcpy(au8_PageBuf, pu8_DestAddr, u32_SectorCnt<<9);

		u32_Err = NC_WritePages(u32_PageIdx, au8_PageBuf, au8_SpareBuf, 1);
		if (u32_Err != UNFD_ST_SUCCESS) {

			nand_debug(2, 1, "Writting page 0x%04lx failed with EC: 0x%04lx\n",
					   u32_PageIdx, u32_Err);

			return u32_Err;
		}
	}

	return UNFD_ST_SUCCESS;
}

U32 drvNAND_WriteBlk(U8 *pu8_DestAddr, U8 u8_PartType, U16 u16_PBA,
					 U16 u16_LBA, U32 u32_StartSector,
					 U32 u32_SectorCnt)
{
	NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();
	U8 *au8_PageBuf = (U8*)pNandDrv->pu8_PageDataBuf;
	U8 *au8_SpareBuf = (U8*)pNandDrv->pu8_PageSpareBuf;
	U32 u32_Err;
	BLK_INFO_t *pBlkInfo = (BLK_INFO_t*)au8_SpareBuf;
	U16 u16_PageSectorCnt;
	U8 u8_PageSectorCntBits;
	U16 u16_PageSectorCntMask;
	U32 u32_SrcPageIdx, u32_BakPageIdx;
	U16 u16_PageCnt;
	U16 u16_SectorCntRead;
	U16 u16_SectorIdxInPage;
	U16 u16_i;
	U16 u16_LastWrittenPage = 0;
	U16 u16_BakBlkIdx;
	U8 u8_GoodBlkCnt;

	/* sector size from AP is 512 Bytes, not NAND sector size */
	u16_PageSectorCnt = pNandDrv->u16_PageByteCnt >> 9;
	u8_PageSectorCntBits = drvNAND_CountBits(u16_PageSectorCnt);
	u16_PageSectorCntMask = (1<<u8_PageSectorCntBits)-1;

	if( (u32_StartSector & u16_PageSectorCntMask)  == 0 )
	{
		u32_SrcPageIdx = (u16_PBA << pNandDrv->u8_BlkPageCntBits) + (u32_StartSector>>u8_PageSectorCntBits);
		u32_Err = NC_ReadPages(u32_SrcPageIdx, au8_PageBuf, au8_SpareBuf, 1);
		if (u32_Err != UNFD_ST_SUCCESS)
		{
			nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Reading page 0x%04lx failed with EC: 0x%04lx\n",
					   u32_SrcPageIdx, u32_Err);

			return u32_Err;
		}

		for(u16_i=0 ; u16_i<pNandDrv->u16_SpareByteCnt ; u16_i++)
		{
			if(au8_SpareBuf[u16_i] != 0xFF)
				goto LABEL_WRITE_BLK;
		}

		for(u16_i=0 ; u16_i<pNandDrv->u16_PageByteCnt ; u16_i++)
		{
			if(au8_PageBuf[u16_i] != 0xFF)
				goto LABEL_WRITE_BLK;
		}

		/* empty page, needn't to backup*/
		u32_Err = drvNAND_WriteBlkNonBackup(pu8_DestAddr, u8_PartType, u16_PBA,
				 					u16_LBA, u32_StartSector, u32_SectorCnt);
		return u32_Err;
	}

LABEL_WRITE_BLK:
    nand_debug(UNFD_DEBUG_LEVEL_HIGH, 1, "BlkIdx:%d, BlkNo:%d, StartSector:%d, SectorCnt:%d\n", (int)u16_PBA, (int)u16_LBA, (int)u32_StartSector, (int)u32_SectorCnt);

	/* Find physical Backup BlkIdx */
	u16_BakBlkIdx = pNandDrv->pPartInfo->records->u16_StartBlk-1;
	u8_GoodBlkCnt = 0;
	while(1)
	{
		if(!drvNAND_IsGoodBlk(u16_BakBlkIdx))
		{
			nand_debug(UNFD_DEBUG_LEVEL_WARNING, 1, "Skip bad blk: 0x%04x\n", u16_BakBlkIdx);
		}
		else
		{
			// reserve last 4 good blks for E2P0/E2P1/NVRAM0/NVRAM1
			if(++u8_GoodBlkCnt == 5)
				break;
		}

		if((--u16_BakBlkIdx)<(pNandDrv->pPartInfo->records->u16_StartBlk-7))
		{
			nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "too many bad block\n");
			return UNFD_ST_ERR_LACK_BLK;
		}
	}
	nand_debug(UNFD_DEBUG_LEVEL_HIGH, 1, "u16_BakBlkIdx:%d\n", u16_BakBlkIdx);

	// erase bak block
	u32_Err = drvNAND_ErasePhyBlk(u16_BakBlkIdx);
	if (u32_Err != UNFD_ST_SUCCESS) {
		nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Erase Blk 0x%04x failed with EC: 0x%08lx\n",
				   u16_BakBlkIdx, u32_Err);
		if(u32_Err == UNFD_ST_ERR_E_FAIL)
			drvNAND_MarkBadBlk(u16_BakBlkIdx);
		return u32_Err;
	}

	for(u16_i=0 ; u16_i<(u32_StartSector>>u8_PageSectorCntBits) ; u16_i++)
	{
		u32_SrcPageIdx = (u16_PBA << pNandDrv->u8_BlkPageCntBits) + u16_i;
		u32_Err = NC_ReadPages(u32_SrcPageIdx, au8_PageBuf, au8_SpareBuf, 1);
		if (u32_Err != UNFD_ST_SUCCESS)
		{
			nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Reading page 0x%04lx failed with EC: 0x%04lx\n",
					   u32_SrcPageIdx, u32_Err);

			return u32_Err;
		}

		u32_BakPageIdx = (u16_BakBlkIdx << pNandDrv->u8_BlkPageCntBits) + u16_i;
		u32_Err = NC_WritePages(u32_BakPageIdx, au8_PageBuf, au8_SpareBuf, 1);
		if (u32_Err != UNFD_ST_SUCCESS)
		{
			nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Writing page 0x%04lx failed with EC: 0x%04lx\n",
					   u32_BakPageIdx, u32_Err);
			if(u32_Err == UNFD_ST_ERR_W_FAIL)
				drvNAND_MarkBadBlk(u16_BakBlkIdx);
			return u32_Err;
		}
		u16_LastWrittenPage = u16_i;
	}

	/* Read unaligned sectors first */
	if( u32_StartSector & u16_PageSectorCntMask )
	{
		u32_SrcPageIdx = (u16_PBA << pNandDrv->u8_BlkPageCntBits) + (u32_StartSector>>u8_PageSectorCntBits);
		u32_Err = NC_ReadPages(u32_SrcPageIdx, au8_PageBuf, au8_SpareBuf, 1);
		if (u32_Err != UNFD_ST_SUCCESS) {

			nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Reading page 0x%04lx failed with EC: 0x%04lx\n",
					   u32_SrcPageIdx, u32_Err);

			return u32_Err;
		}
		u16_SectorIdxInPage = u32_StartSector & u16_PageSectorCntMask;
		u16_SectorCntRead = u32_SectorCnt > ((U32)u16_PageSectorCnt-(U32)u16_SectorIdxInPage) ? ((U32)u16_PageSectorCnt-(U32)u16_SectorIdxInPage) : (u32_SectorCnt);

		memcpy((au8_PageBuf+(u16_SectorIdxInPage<<9)), pu8_DestAddr, (u16_SectorCntRead<<9));
		u32_BakPageIdx = (u16_BakBlkIdx << pNandDrv->u8_BlkPageCntBits) + (u32_StartSector>>u8_PageSectorCntBits);
		u32_Err = NC_WritePages(u32_BakPageIdx, au8_PageBuf, au8_SpareBuf, 1);
		if (u32_Err != UNFD_ST_SUCCESS) {

			nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Writing page 0x%04lx failed with EC: 0x%04lx\n",
					   u32_BakPageIdx, u32_Err);
			if(u32_Err == UNFD_ST_ERR_W_FAIL)
				drvNAND_MarkBadBlk(u16_BakBlkIdx);
			return u32_Err;
		}

		pu8_DestAddr += (u16_SectorCntRead<<9);
		u32_StartSector += u16_SectorCntRead;
		u32_SectorCnt -= u16_SectorCntRead;
		u16_LastWrittenPage = (u32_StartSector>>u8_PageSectorCntBits);
	}

	/* Read aligned sectors then */
	memset(au8_SpareBuf, '\0', sizeof(*pBlkInfo));
	pBlkInfo->u8_BadBlkMark = 0xFF;
	pBlkInfo->u8_PartType = u8_PartType;
	pBlkInfo->u16_BlkAddr = u16_LBA;

	u32_BakPageIdx = (u16_BakBlkIdx << pNandDrv->u8_BlkPageCntBits) + (u32_StartSector>>u8_PageSectorCntBits);
	u16_PageCnt = (u32_SectorCnt>>u8_PageSectorCntBits);
	while (u16_PageCnt)
	{
		u32_Err = NC_WritePages(u32_BakPageIdx, pu8_DestAddr, au8_SpareBuf, 1);
		if (u32_Err != UNFD_ST_SUCCESS) {

			nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Writing page 0x%04lx failed with EC: 0x%04lx\n",
					   u32_BakPageIdx, u32_Err);
			if(u32_Err == UNFD_ST_ERR_W_FAIL)
				drvNAND_MarkBadBlk(u16_BakBlkIdx);
			return u32_Err;
		}

		u16_LastWrittenPage = u32_BakPageIdx -(u16_BakBlkIdx << pNandDrv->u8_BlkPageCntBits);
		pu8_DestAddr += pNandDrv->u16_PageByteCnt;
		u32_BakPageIdx++;
		u16_PageCnt--;
		u32_StartSector += u16_PageSectorCnt;
		u32_SectorCnt -= u16_PageSectorCnt;
	}

	/* Read remaining unaligned sectors finally */
	if( u32_SectorCnt )
	{
		u32_SrcPageIdx = (u16_PBA << pNandDrv->u8_BlkPageCntBits) + (u32_StartSector>>u8_PageSectorCntBits);
		u32_Err = NC_ReadPages(u32_SrcPageIdx, au8_PageBuf, au8_SpareBuf, 1);
		if (u32_Err != UNFD_ST_SUCCESS) {

			nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Reading page 0x%04lx failed with EC: 0x%04lx\n",
					   u32_SrcPageIdx, u32_Err);

			return u32_Err;
		}

		memcpy(au8_PageBuf, pu8_DestAddr, (u32_SectorCnt<<9));
		u32_BakPageIdx = (u16_BakBlkIdx << pNandDrv->u8_BlkPageCntBits) + (u32_StartSector>>u8_PageSectorCntBits);
		u32_Err = NC_WritePages(u32_BakPageIdx, au8_PageBuf, au8_SpareBuf, 1);
		if (u32_Err != UNFD_ST_SUCCESS) {

			nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Writing page 0x%04lx failed with EC: 0x%04lx\n",
					   u32_BakPageIdx, u32_Err);
			if(u32_Err == UNFD_ST_ERR_W_FAIL)
				drvNAND_MarkBadBlk(u16_BakBlkIdx);
			return u32_Err;
		}
		u16_LastWrittenPage = (u32_StartSector>>u8_PageSectorCntBits);
	}

	for(u16_i=(u16_LastWrittenPage+1) ; u16_i<pNandDrv->u16_BlkPageCnt; u16_i++)
	{
		u32_SrcPageIdx = (u16_PBA << pNandDrv->u8_BlkPageCntBits) + u16_i;
		u32_Err = NC_ReadPages(u32_SrcPageIdx, au8_PageBuf, au8_SpareBuf, 1);
		if (u32_Err != UNFD_ST_SUCCESS)
		{
			nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Reading page 0x%04lx failed with EC: 0x%04lx\n",
					   u32_SrcPageIdx, u32_Err);

			return u32_Err;
		}

		u32_BakPageIdx = (u16_BakBlkIdx << pNandDrv->u8_BlkPageCntBits) + u16_i;
		u32_Err = NC_WritePages(u32_BakPageIdx, au8_PageBuf, au8_SpareBuf, 1);
		if (u32_Err != UNFD_ST_SUCCESS)
		{
			nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Writing page 0x%04lx failed with EC: 0x%04lx\n",
					   u32_BakPageIdx, u32_Err);
			if(u32_Err== UNFD_ST_ERR_W_FAIL)
				drvNAND_MarkBadBlk(u16_BakBlkIdx);
			return u32_Err;
		}
	}

	// erase src block
	u32_Err = drvNAND_ErasePhyBlk(u16_PBA);
	if (u32_Err != UNFD_ST_SUCCESS) {
		nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Erase Blk 0x%04x failed with EC: 0x%08x\n",
				(unsigned int)u16_PBA, (unsigned int)u32_Err);
		if(u32_Err == UNFD_ST_ERR_E_FAIL)
			drvNAND_MarkBadBlk(u16_PBA);
		return u32_Err;
	}

	// copy bak to src
	for(u16_i=0 ; u16_i<pNandDrv->u16_BlkPageCnt; u16_i++)
	{
		u32_BakPageIdx = (u16_BakBlkIdx << pNandDrv->u8_BlkPageCntBits) + u16_i;
		u32_Err = NC_ReadPages(u32_BakPageIdx, au8_PageBuf, au8_SpareBuf, 1);
		if (u32_Err != UNFD_ST_SUCCESS)
		{
			nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Reading page 0x%04lx failed with EC: 0x%04lx\n",
					   u32_BakPageIdx, u32_Err);

			return u32_Err;
		}

		u32_SrcPageIdx = (u16_PBA << pNandDrv->u8_BlkPageCntBits) + u16_i;
		u32_Err = NC_WritePages(u32_SrcPageIdx, au8_PageBuf, au8_SpareBuf, 1);
		if (u32_Err != UNFD_ST_SUCCESS)
		{
			nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Reading page 0x%04lx failed with EC: 0x%04lx\n",
					   u32_SrcPageIdx, u32_Err);
			if(u32_Err == UNFD_ST_ERR_W_FAIL)
				drvNAND_MarkBadBlk(u16_PBA);
			return u32_Err;
		}
	}

	// erase bak block
	u32_Err = drvNAND_ErasePhyBlk(u16_BakBlkIdx);
	if (u32_Err != UNFD_ST_SUCCESS) {
		nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Erase Blk 0x%04x failed with EC: 0x%08lx\n",
				   u16_BakBlkIdx, u32_Err);
		if(u32_Err == UNFD_ST_ERR_E_FAIL)
			drvNAND_MarkBadBlk(u16_BakBlkIdx);
		return u32_Err;
	}

	return UNFD_ST_SUCCESS;
}

U32 drvNAND_MarkBadBlk(U16 u16_PBA)
{
	NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();
	U8 *au8_PageBuf = (U8*)pNandDrv->pu8_PageDataBuf;
	U8 *au8_SpareBuf = (U8*)pNandDrv->pu8_PageSpareBuf;
	U32 u32_Err;
	U32 u32_PageIdx;
    #if (defined(NC_SEL_FCIE5) && (NC_SEL_FCIE5))
    U8  u8_LFSR=0;
    #endif

	u32_Err = drvNAND_ErasePhyBlk(u16_PBA);

	//memset(au8_PageBuf, '\0', pNandDrv->u16_PageByteCnt);
	memset(au8_SpareBuf, '\0', 128);//pNandDrv->u16_SpareByteCnt);

    #if (defined(NC_SEL_FCIE5) && (NC_SEL_FCIE5))
    if(IF_LFSR_ENABLED())
    {u8_LFSR = 1;  NC_DisableLFSR();}
    #endif
                    
	/* Clear the 1st page (main + spare) of the block */
	u32_PageIdx = u16_PBA << pNandDrv->u8_BlkPageCntBits;
	u32_Err = NC_WritePages(u32_PageIdx, au8_PageBuf, au8_SpareBuf, 1);
	if (u32_Err != UNFD_ST_SUCCESS)
        nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Mark Blk 0x%04x failed with ErrCode: 0x%08x\n",
            (unsigned int)u16_PBA, (unsigned int)u32_Err);
	/* Clear the last page (main + spare) of the block */
	u32_PageIdx += pNandDrv->u16_BlkPageCnt - 1;
	u32_Err = NC_WritePages(u32_PageIdx, au8_PageBuf, au8_SpareBuf, 1);
	if (u32_Err != UNFD_ST_SUCCESS)
        nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Mark Blk 0x%04x failed with ErrCode: 0x%08x\n",
            (unsigned int)u16_PBA, (unsigned int)u32_Err);

	#if (defined(NC_SEL_FCIE5) && (NC_SEL_FCIE5))
    if(u8_LFSR)
        NC_EnableLFSR();
    #endif

    return u32_Err;
}

U32 drvNAND_ErasePhyBlk(U16 u16_PBA)
{
	NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();

	return NC_EraseBlk (u16_PBA << pNandDrv->u8_BlkPageCntBits);
}


U32 drvNAND_ReadPhyPage(U32 u32_PageIdx, U8 *pu8_Data, U8 *pu8_Spare)
{
	return NC_ReadPages(u32_PageIdx, pu8_Data, pu8_Spare, 1);
}

U32 drvNAND_WritePhyPage(U32 u32_PageIdx, U8 *pu8_Data, U8 *pu8_Spare)
{
	return NC_WritePages(u32_PageIdx, pu8_Data, pu8_Spare, 1);
}

U32 drvNAND_LFSRReadPhyPage(U32 u32_PageIdx, U8 *pu8_Data, U8 *pu8_Spare)
{
	#if defined(FCIE_LFSR) && FCIE_LFSR
	NAND_DRIVER *pNandDrv = (NAND_DRIVER*)drvNAND_get_DrvContext_address();
	if(pNandDrv->u8_RequireRandomizer == 1)
		NC_EnableLFSR();
	#endif
	return NC_ReadPages(u32_PageIdx, pu8_Data, pu8_Spare, 1);
}

U32 drvNAND_LFSRWritePhyPage(U32 u32_PageIdx, U8 *pu8_Data, U8 *pu8_Spare)
{
	#if defined(FCIE_LFSR) && FCIE_LFSR
	NAND_DRIVER *pNandDrv = (NAND_DRIVER*)drvNAND_get_DrvContext_address();
	if(pNandDrv->u8_RequireRandomizer == 1)
		NC_EnableLFSR();
	#endif
	return NC_WritePages(u32_PageIdx, pu8_Data, pu8_Spare, 1);
}

U8 gau8_OneBitCnt[256]={
// 0   1   2   3   4   5   6   7   8   9   A   B   C  D   E   F
   0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4, // 0
   1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, // 1
   1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, // 2
   2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, // 3
   1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, // 4
   2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, // 5
   2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, // 6
   3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, // 7
   1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, // 8
   2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, // 9
   2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, // A
   3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, // B
   2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, // C
   3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, // D
   3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, // E
   4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8  // F
};
U8 gau8_ZeroBitCnt[256]={
// 0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
   8, 7, 7, 6, 7, 6, 6, 5, 7, 6, 6, 5, 6, 5, 5, 4, // 0
   7, 6, 6, 5, 6, 5, 5, 4, 6, 5, 5, 4, 5, 4, 4, 3, // 1
   7, 6, 6, 5, 6, 5, 5, 4, 6, 5, 5, 4, 5, 4, 4, 3, // 2
   6, 5, 5, 4, 5, 4, 4, 3, 5, 4, 4, 3, 4, 3, 3, 2, // 3
   7, 6, 6, 5, 6, 5, 5, 4, 6, 5, 5, 4, 5, 4, 4, 3, // 4
   6, 5, 5, 4, 5, 4, 4, 3, 5, 4, 4, 3, 4, 3, 3, 2, // 5
   6, 5, 5, 4, 5, 4, 4, 3, 5, 4, 4, 3, 4, 3, 3, 2, // 6
   5, 4, 4, 3, 4, 3, 3, 2, 4, 3, 3, 2, 3, 2, 2, 1, // 7
   7, 6, 6, 5, 6, 5, 5, 4, 6, 5, 5, 4, 5, 4, 4, 3, // 8
   6, 5, 5, 4, 5, 4, 4, 3, 5, 4, 4, 3, 4, 3, 3, 2, // 9
   6, 5, 5, 4, 5, 4, 4, 3, 5, 4, 4, 3, 4, 3, 3, 2, // A
   5, 4, 4, 3, 4, 3, 3, 2, 4, 3, 3, 2, 3, 2, 2, 1, // B
   6, 5, 5, 4, 5, 4, 4, 3, 5, 4, 4, 3, 4, 3, 3, 2, // C
   5, 4, 4, 3, 4, 3, 3, 2, 4, 3, 3, 2, 3, 2, 2, 1, // D
   5, 4, 4, 3, 4, 3, 3, 2, 4, 3, 3, 2, 3, 2, 2, 1, // E
   4, 3, 3, 2, 3, 2, 2, 1, 3, 2, 2, 1, 2, 1, 1, 0  // F
};
 /*
  * Check whether the cause of ECC fail is reading a non-all0xff empty page.
  * @main: Main Data of a NAND Page
  * @spare: Spare Data of a NAND Page
  * @return 1: if False Alarm 0: Real ECC Fail
  */
int nand_CheckEmptyPageFalseAlarm(U8 *main, U8 *spare)
{
	NAND_DRIVER *pNandDrv = (NAND_DRIVER*)drvNAND_get_DrvContext_address();
	int  i, j, ECCErrBitCnt;

	//check if false alarm casued by empty page
	for(j = 0; j < pNandDrv->u16_PageSectorCnt; j++)
	{
		ECCErrBitCnt = 0;
		for(i = 0; i < pNandDrv->u16_SectorSpareByteCnt; i++)
		{
			if(j*pNandDrv->u16_SectorSpareByteCnt + i < 512)
				ECCErrBitCnt += gau8_ZeroBitCnt[spare[ j*pNandDrv->u16_SectorSpareByteCnt + i]];
			if(ECCErrBitCnt > pNandDrv->u16_ECCCorretableBit)
				return 0;
		}
		
		for(i = 0; i < pNandDrv->u16_SectorByteCnt; i++)
		{
			ECCErrBitCnt += gau8_ZeroBitCnt[main[ j*pNandDrv->u16_SectorByteCnt + i]];
			if(ECCErrBitCnt > pNandDrv->u16_ECCCorretableBit)
				return 0;
		}
	}
	memset(spare, 0xFF, pNandDrv->u16_SpareByteCnt);
	memset(main, 0xFF, pNandDrv->u16_PageByteCnt);	
	return 1;
}

//shift blocks
extern struct list_head mtdids;
extern struct list_head devices;
//back:the backup block of the start block
//last:the last block of the mtd partition
//curr:current block while shifting blocks
//final:the last data block of the mtd partition
//start:the first block to be shifted
int nand_ShiftBlock_Skipbad(int start, int back, U32 (*nand_markbad)(U32))
{
	int i, ret, first = 1, found = 0;
	int curr = 0 , next = 0, final = 0, last = 0;
	unsigned char *pagebuf, *sparebuf;
	char env[32];
	struct list_head *entry, *n;
	struct mtdids *ids;
	struct mtd_device *mtd_device;
	struct part_info *part;
	NAND_DRIVER *pNandDrv = (NAND_DRIVER*)drvNAND_get_DrvContext_address();
	
	if(mtdparts_init() != 0){
		nand_debug (UNFD_DEBUG_LEVEL_ERROR,1,"mtd init part failed\n");
		return -1;
	}
	list_for_each_safe(entry, n, &mtdids){
		ids = list_entry(entry, struct mtdids, link);
		nand_debug(UNFD_DEBUG_LEVEL,1,"<%d>%s:size=%lld,type=%d\n", ids->num, ids->mtd_id, ids->size, ids->type);
	}
	list_for_each_safe(entry, n, &devices){
		struct list_head *entry2, *n2;
		mtd_device = list_entry(entry, struct mtd_device, link);
		nand_debug(UNFD_DEBUG_LEVEL,1,"find one mtd device\n");
		list_for_each_safe(entry2, n2, &mtd_device->parts){
			part = list_entry(entry2, struct part_info, link);
			nand_debug(UNFD_DEBUG_LEVEL,1,"part %s:%lld,%lld\n", part->name, part->offset, part->size);
		}	
	}
	
	//find the last block of the mtd partition
	list_for_each_safe(entry, n, &devices){
		struct list_head *entry2, *n2;
		mtd_device = list_entry(entry, struct mtd_device, link);
		list_for_each_safe(entry2, n2, &mtd_device->parts){
			part = list_entry(entry2, struct part_info, link);
			if((start << (pNandDrv->u8_BlkPageCntBits + pNandDrv->u8_PageByteCntBits)) >= part->offset &&
				 (start << (pNandDrv->u8_BlkPageCntBits + pNandDrv->u8_PageByteCntBits)) < part->offset + part->size){
				last = ((part->offset + part->size) >> (pNandDrv->u8_BlkPageCntBits + pNandDrv->u8_PageByteCntBits)) - 1;
				found = 1;
				break;
			}	
		}
		if(found)
			break;	
	}
		
	pagebuf = (unsigned char *)malloc(pNandDrv->u16_PageByteCnt);
	sparebuf = (unsigned char *)malloc(pNandDrv->u16_SpareByteCnt);

	//find the last data block
	for(curr = start, final = start; final <= last; final++){
		ret = drvNAND_IsGoodBlk(final);
		if(!ret)
			continue;
		for(i = 0; i < pNandDrv->u16_BlkPageCnt; i++){
			ret = drvNAND_ReadPhyPage((final << pNandDrv->u8_BlkPageCntBits) + i, pagebuf, sparebuf);
			if(ret != UNFD_ST_SUCCESS){
				goto out;
			}
			ret = drvNAND_CheckAll0xFF(sparebuf, pNandDrv->u16_SpareByteCnt);
			if(!ret)
				break;
		}
		if(i == pNandDrv->u16_BlkPageCnt){
			final = curr;
			break;
		}		
		curr = final;
	}

again:
	first = 1;
	for(next = final + 1; next <= last; next++){
		ret = drvNAND_IsGoodBlk(next);
		if(ret)
			break;
	}
	nand_debug(UNFD_DEBUG_LEVEL,1,"start:%d, last:%d, final:%d, next:%d, back:%d\n", start, last, final, next, back);
	if(next > last){
		nand_debug(UNFD_DEBUG_LEVEL,1,"No enough good block in this partition\n");
		goto out;
	}
	while(curr >= start){
		for(curr = next - 1; curr > start; curr--){
			ret = drvNAND_IsGoodBlk(curr);
			if(ret)
				break;
		}
		
		memset(env, 0, 32);
		sprintf(env, "%d,%d,%d,%d,%d,%d", start, last, final, back, curr, next);
		setenv("shift_recover", env);
		saveenv();
		
		nand_debug(UNFD_DEBUG_LEVEL,1,"shift block from %d to %d\n", curr, next);
		if(curr == start){
			nand_debug(UNFD_DEBUG_LEVEL,1,"restore back up block\n");
			break;
		}
		
		ret = drvNAND_ErasePhyBlk(next);
		if(next == 14)
			ret = -1;
		if(ret != UNFD_ST_SUCCESS){
			if(nand_markbad == NULL)
				ret = drvNAND_MarkBadBlk(next);
			else
				ret = nand_markbad(next);
			if(ret != UNFD_ST_SUCCESS){
				nand_debug (UNFD_DEBUG_LEVEL_ERROR,1,"mark block bad failed\n");
				goto out;
			}
			goto again;
		}
		for(i = 0; i < pNandDrv->u16_BlkPageCnt; i++){
			ret = drvNAND_ReadPhyPage((curr << pNandDrv->u8_BlkPageCntBits) + i, pagebuf, sparebuf);
			if(ret != UNFD_ST_SUCCESS){
				nand_debug (UNFD_DEBUG_LEVEL_ERROR,1,"read block %d, page %d failed\n", curr, i);
				goto out;
			}
			ret = drvNAND_WritePhyPage((next << pNandDrv->u8_BlkPageCntBits) + i, pagebuf, sparebuf);
			if(ret != UNFD_ST_SUCCESS){
				drvNAND_ErasePhyBlk(next);
				if(nand_markbad == NULL)
					ret = drvNAND_MarkBadBlk(next);
				else
					ret = nand_markbad(next);
				if(ret != UNFD_ST_SUCCESS){
					nand_debug (UNFD_DEBUG_LEVEL_ERROR,1,"mark block bad failed\n");
					goto out;
				}
				goto again;
			}
		}
		if(first){
			final = next;
			first = 0;
		}
		next = curr;
	}
	
	ret = drvNAND_ErasePhyBlk(next);
	if(ret != UNFD_ST_SUCCESS){
		if(nand_markbad == NULL)
			ret = drvNAND_MarkBadBlk(next);
		else
			ret = nand_markbad(next);
		if(ret != UNFD_ST_SUCCESS){
			nand_debug (UNFD_DEBUG_LEVEL_ERROR,1,"mark block bad failed\n");
			goto out;
		}
		goto again;
	}
	for(i = 0; i < pNandDrv->u16_BlkPageCnt; i++){
		ret = drvNAND_ReadPhyPage((back << pNandDrv->u8_BlkPageCntBits) + i, pagebuf, sparebuf);
		if(ret != UNFD_ST_SUCCESS){
			nand_debug (UNFD_DEBUG_LEVEL_ERROR,1,"read block %d, page %d failed\n", curr, i);
			goto out;
		}
		ret = drvNAND_WritePhyPage((next << pNandDrv->u8_BlkPageCntBits) + i, pagebuf, sparebuf);
		if(ret != UNFD_ST_SUCCESS){
			drvNAND_ErasePhyBlk(next);
			if(nand_markbad == NULL)
				ret = drvNAND_MarkBadBlk(next);
			else
				ret = nand_markbad(next);
			if(ret != UNFD_ST_SUCCESS){
				nand_debug (UNFD_DEBUG_LEVEL_ERROR,1,"mark block bad failed\n");
				goto out;
			}
			goto again;
		}
	}	
	
	//erase start block
	drvNAND_ErasePhyBlk(start);
	if(nand_markbad == NULL)
		ret = drvNAND_MarkBadBlk(start);
	else{
		ret = nand_markbad(start);
	}
	if(ret != UNFD_ST_SUCCESS){
		nand_debug (UNFD_DEBUG_LEVEL_ERROR,1,"mark block bad failed\n");
		goto out;
	}
	
	//erase back up block
	ret = drvNAND_ErasePhyBlk(back);
	if(ret != UNFD_ST_SUCCESS)
	{
		if(nand_markbad == NULL)
			drvNAND_MarkBadBlk(back);
		else
			nand_markbad(back);
	}
	
	setenv("shift_recover", "");
	saveenv();
	free(pagebuf);
	free(sparebuf);	
	return UNFD_ST_SUCCESS;
	
out:
	memset(env, 0, 32);
	sprintf(env, "%d,%d,%d,%d,%d,%d", start, last, final, back, curr, next);
	setenv("shift_error", env);
	setenv("shift_recover", "");
	saveenv();
	free(pagebuf);
	free(sparebuf);
	return -1;
}
int nand_ShiftBlock_Recover(U32 (*nand_markbad)(U32))
{
	const char *p;
	unsigned char *pagebuf, *sparebuf;
	char env[32];
	int ret, i, first = 1;
	int start, last, final, back, curr, next;
	NAND_DRIVER *pNandDrv = (NAND_DRIVER*)drvNAND_get_DrvContext_address();
	
	p = getenv("shift_recover");
	nand_debug(UNFD_DEBUG_LEVEL_LOW,1,"%s\n",p);
	if(!p || !strcmp(p, "")){
		nand_debug(UNFD_DEBUG_LEVEL_LOW,1,"No recovery information, needless to recovery\n");
		return 0;
	}
	start = atoi(p);
	p = strchr(p, ',') + 1;
	last = (U32)atoi(p);
	p = strchr(p, ',') + 1;
	final = (U32)atoi(p);
	p = strchr(p, ',') + 1;
	back = (U32)atoi(p);
	p = strchr(p, ',') + 1;
	curr = (U32)atoi(p);
	p = strchr(p, ',') + 1;
	next = (U32)atoi(p);
	
	nand_debug(UNFD_DEBUG_LEVEL,1,"start:%d, last:%d, final:%d, back:%d, curr:%d, next:%d\n", start, last, final, back, curr, next);
	
	pagebuf = (U8 *)malloc(pNandDrv->u16_PageByteCnt);
	sparebuf = (U8 *)malloc(pNandDrv->u16_SpareByteCnt);
	///
	goto loop;
	
again:
	first = 1;
	for(next = final + 1; next <= last; next++){
		ret = drvNAND_IsGoodBlk(next);
		if(ret)
			break;
	}
	nand_debug(UNFD_DEBUG_LEVEL,1,"start:%d, last:%d, final:%d, next:%d, back:%d\n",start, last, final, next, back);
	if(next > last){
		nand_debug(UNFD_DEBUG_LEVEL,1,"No enough good block in this partition\n");
		goto out;
	}
	
loop:	
	while(curr >= start){
		for(curr = next - 1; curr > start; curr--){
			ret = drvNAND_IsGoodBlk(curr);
			if(ret)
				break;
		}
		
		memset(env, 0, 32);
		sprintf(env, "%d,%d,%d,%d,%d,%d", start, last, final, back, curr, next);
		setenv("shift_recover", env);
		saveenv();
		
		nand_debug(UNFD_DEBUG_LEVEL,1,"shift block from %d to %d\n", curr, next);
		if(curr == start){
			nand_debug(UNFD_DEBUG_LEVEL,1,"restore back up block\n");
			break;
		}
		
		ret = drvNAND_ErasePhyBlk(next);
		if(ret != UNFD_ST_SUCCESS){
			if(nand_markbad == NULL)
				ret = drvNAND_MarkBadBlk(next);
			else
				ret = nand_markbad(next);
			if(ret != UNFD_ST_SUCCESS){
				nand_debug (UNFD_DEBUG_LEVEL_ERROR,1,"mark block bad failed\n");
				goto out;
			}
			goto again;
		}
		for(i = 0; i < pNandDrv->u16_BlkPageCnt; i++){
			ret = drvNAND_ReadPhyPage((curr << pNandDrv->u8_BlkPageCntBits) + i, pagebuf, sparebuf);
			if(ret != UNFD_ST_SUCCESS){
				nand_debug (UNFD_DEBUG_LEVEL_ERROR,1,"read block %d, page %d failed\n", curr, i);
				goto out;
			}
			ret = drvNAND_WritePhyPage((next << pNandDrv->u8_BlkPageCntBits) + i, pagebuf, sparebuf);
			if(ret != UNFD_ST_SUCCESS){
				drvNAND_ErasePhyBlk(next);
				if(nand_markbad == NULL)
					ret = drvNAND_MarkBadBlk(next);
				else
					ret = nand_markbad(next);
				if(ret != UNFD_ST_SUCCESS){
					nand_debug (UNFD_DEBUG_LEVEL_ERROR,1,"mark block bad failed\n");
					goto out;
				}
				goto again;
			}
		}
		if(first){
			final = next;
			first = 0;
		}
		next = curr;
	}
	
	ret = drvNAND_ErasePhyBlk(next);
	if(ret != UNFD_ST_SUCCESS){
		if(nand_markbad == NULL)
			ret = drvNAND_MarkBadBlk(next);
		else
			ret = nand_markbad(next);
		if(ret != UNFD_ST_SUCCESS){
			nand_debug (UNFD_DEBUG_LEVEL_ERROR,1,"mark block bad failed\n");
			goto out;
		}
		goto again;
	}
	for(i = 0; i < pNandDrv->u16_BlkPageCnt; i++){
		ret = drvNAND_ReadPhyPage((back << pNandDrv->u8_BlkPageCntBits) + i, pagebuf, sparebuf);
		if(ret != UNFD_ST_SUCCESS){
			nand_debug (UNFD_DEBUG_LEVEL_ERROR,1,"read block %d, page %d failed\n", curr, i);
			goto out;
		}
		ret = drvNAND_WritePhyPage((next << pNandDrv->u8_BlkPageCntBits) + i, pagebuf, sparebuf);
		if(ret != UNFD_ST_SUCCESS){
			drvNAND_ErasePhyBlk(next);
			if(nand_markbad == NULL)
				ret = drvNAND_MarkBadBlk(next);
			else
				ret = nand_markbad(next);
			if(ret != UNFD_ST_SUCCESS){
				nand_debug (UNFD_DEBUG_LEVEL_ERROR,1,"mark block bad failed\n");
				goto out;
			}
			goto again;
		}
	}	
	
	//erase start block
	drvNAND_ErasePhyBlk(start);
	if(nand_markbad == NULL)
		ret = drvNAND_MarkBadBlk(start);
	else{
		ret = nand_markbad(start);
	}
	if(ret != UNFD_ST_SUCCESS){
		nand_debug (UNFD_DEBUG_LEVEL_ERROR,1,"mark block bad failed\n");
		goto out;
	}
	
	//erase back up block
	ret = drvNAND_ErasePhyBlk(back);
	if(ret != UNFD_ST_SUCCESS)
	{
		if(nand_markbad == NULL)
			drvNAND_MarkBadBlk(back);
		else
			nand_markbad(back);
	}	
		
	setenv("shift_recover", "");
	saveenv();
	free(pagebuf);
	free(sparebuf);	
	return UNFD_ST_SUCCESS;
	
out:
	memset(env, 0, 32);
	sprintf(env, "%d,%d,%d,%d,%d,%d", start, last, final, back, curr, next);
	setenv("shift_error", env);	
	setenv("shift_recover", "");
	saveenv();
	free(pagebuf);
	free(sparebuf);
	return -1;
}

/*
 * Refresh block data to avoid read disturbance happens.
 * @u32_BlkRow: Row of Target block who has correctable ECC bits
 * @u8_torture: Flag of whether to torture the target block if Read-Retry happens
 * @*nand_markbad: Customize Markbad function ex: mtd mark bad function
 */
int nand_ReadDisturbance_BigImg(U32  u32_BlkRow, U8 u8_torture, U32 (*nand_markbad)(U32),  U8 u8_CustSpareMarker, U8 u8_CustMarkerIndex)
{
	NAND_DRIVER *pNandDrv = (NAND_DRIVER*)drvNAND_get_DrvContext_address();
	//MBBS_INFO_t* SPAREINFO;
	U32 u32_GoodBlkIdx, u32_Err, u32_Row;
	U16 u16_i, *pu16_BlkPBA;
	U8 *pu8_PageDataBuf = pNandDrv->pu8_PageDataBuf;
	U8 *pu8_PageSpareBuf = pNandDrv->pu8_PageSpareBuf;

	//SPAREINFO =(MBBS_INFO_t*) pNandDrv->pu8_PageSpareBuf;
	pu16_BlkPBA = (U16 *)(pNandDrv->pu8_PageSpareBuf + pNandDrv->u16_SectorSpareByteCnt);

RETRY:
	u32_GoodBlkIdx = drvNAND_GetBackupBlk();
    if(u32_GoodBlkIdx==0)//No free block to do bakup
        return 0;

	nand_debug(UNFD_DEBUG_LEVEL, 1,"Blk %X is the last good block\n", (unsigned int)u32_GoodBlkIdx);
	
	// write to empty block, so called backup block

	memset(pu8_PageSpareBuf, 0xff, pNandDrv->u16_SpareByteCnt);
	nand_debug(UNFD_DEBUG_LEVEL, 1,"Write data to Blk %X\n", (unsigned int)u32_GoodBlkIdx);
	for(u16_i = 0; u16_i < pNandDrv->u16_BlkPageCnt; u16_i ++)
	{
		//read back each page from target block
		u32_Row = (u32_BlkRow) + u16_i;
		Disable_Err_log = 1;
		u32_Err = drvNAND_LFSRReadPhyPage(u32_Row, pu8_PageDataBuf, pu8_PageSpareBuf);
		Disable_Err_log = 0;
		if(u32_Err != UNFD_ST_SUCCESS)
		{
			nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "NAND fatal error page @ %X,when reading target block\n",(unsigned int)u32_Row);
			nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Please re-program nand\n");
			return -1;
		}
		// write data to backup block with target PBA and marker recorded in spare
		// spare of 1st sector   -> only 0xFF
		// spare of 2nd sector  -> blockrow
		// spare of 3rd sector   -> marker of 3697
		// spare of 4th sector   -> Customer Marker

		pu8_PageSpareBuf[0] = 0xFF;
//		SPAREINFO->u8_BadBlkMarker = 0xFF;
//		SPAREINFO->u16_BackupPBA = u32_BlkRow >> pNandDrv->u8_BlkPageCntBits;
		*pu16_BlkPBA = u32_BlkRow >> pNandDrv->u8_BlkPageCntBits;
//		pu8_PageSpareBuf[3] = 0x36;
//		pu8_PageSpareBuf[4] = 0x97;
		pu8_PageSpareBuf[pNandDrv->u16_SectorSpareByteCnt * 2] = 0x36;
		pu8_PageSpareBuf[pNandDrv->u16_SectorSpareByteCnt * 2 + 1] = 0x97;

		pu8_PageSpareBuf[pNandDrv->u16_SectorSpareByteCnt * 3] = u8_CustSpareMarker;
		pu8_PageSpareBuf[pNandDrv->u16_SectorSpareByteCnt * 3 + 1] = u8_CustMarkerIndex;
		
		u32_Row = (u32_GoodBlkIdx << pNandDrv->u8_BlkPageCntBits) + u16_i;
		
		u32_Err = drvNAND_LFSRWritePhyPage(u32_Row, pu8_PageDataBuf, pu8_PageSpareBuf);
		if(u32_Err != UNFD_ST_SUCCESS)
		{
			//mark bad and goto research a good block
			if(nand_markbad == NULL)
				drvNAND_MarkBadBlk(u32_GoodBlkIdx);
			else
				nand_markbad(u32_GoodBlkIdx);

			goto RETRY;
		}
	}

	//erase target block
	nand_debug(UNFD_DEBUG_LEVEL, 1,"Erase Target block %X\n", (unsigned int)(u32_BlkRow >> pNandDrv->u8_BlkPageCntBits));
	u32_Err = NC_EraseBlk(u32_BlkRow);
	if(u32_Err != UNFD_ST_SUCCESS)
	{
		//complicated need to do
		//mark bad and do shifting block
		u32_Err = nand_ShiftBlock_Skipbad(u32_BlkRow >> pNandDrv->u8_BlkPageCntBits, u32_GoodBlkIdx, nand_markbad);
		if(u32_Err != UNFD_ST_SUCCESS)
			return -1;
		return 0;				
	}
	//if torture needed torture block for recheck read retry

	// write data to target block if torture ok
	// if torture fail -> do shifting block...
	// @FIXME
	if(u8_torture)
		;

	nand_debug(UNFD_DEBUG_LEVEL, 1, "Write data back to target block with page count = %d\n", pNandDrv->u16_BlkPageCnt);
	//read each page from backup block and write back to target block
	for(u16_i = 0; u16_i < pNandDrv->u16_BlkPageCnt; u16_i ++)
	{
		u32_Row = (u32_GoodBlkIdx << pNandDrv->u8_BlkPageCntBits) + u16_i;
		Disable_Err_log = 1;
		u32_Err = drvNAND_LFSRReadPhyPage(u32_Row, pu8_PageDataBuf, pu8_PageSpareBuf);
		Disable_Err_log = 0;

		if(u32_Err != UNFD_ST_SUCCESS)
		{
			nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "NAND fatal error page @ %X,when reading target block\n",(unsigned int)u32_Row);
			nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Please re-program nand\n");
			return -1;
		}

		u32_Row = (u32_BlkRow) + u16_i;
		
		u32_Err = drvNAND_LFSRWritePhyPage(u32_Row, pu8_PageDataBuf, pu8_PageSpareBuf);
		if(u32_Err != UNFD_ST_SUCCESS)
		{
			//mark bad and do shifting block
			//@FIXME
			u32_Err = nand_ShiftBlock_Skipbad(u32_BlkRow >> pNandDrv->u8_BlkPageCntBits, u32_GoodBlkIdx, nand_markbad);
			if(u32_Err != UNFD_ST_SUCCESS)
				return -1;
			return 0;		
		}
	}
	
	//erase backup block
	u32_Err = NC_EraseBlk(u32_GoodBlkIdx << pNandDrv->u8_BlkPageCntBits);
	if(u32_Err != UNFD_ST_SUCCESS)
	{
		//mark bad
		if(nand_markbad == NULL)
			drvNAND_MarkBadBlk(u32_GoodBlkIdx);
		else
			nand_markbad(u32_GoodBlkIdx);
	}

	return 0;
}

/*
 * Restore data from backup block to original target source block.
 * @*nand_markbad: Customize Markbad function ex: mtd mark bad function
 */
U32 nand_ReadDisturbance_BigImgRestore(U32 (*nand_markbad)(U32))
{
	NAND_DRIVER *pNandDrv = (NAND_DRIVER*)drvNAND_get_DrvContext_address();
	U32 u32_GoodBlkIdx, u32_Err =0;
	U16 u16_SrcPBA, u16_i, *pu16_BlkPBA, u16_Start_Blk;
	U8 *pu8_PageDataBuf = (U8*)pNandDrv->pu8_PageDataBuf;
	U8 *pu8_PageSpareBuf = (U8*)pNandDrv->pu8_PageSpareBuf;
	U8	u8_CustSpareMarker, u8_CustMarkerIndex;

	pu16_BlkPBA = (U16 *)(pNandDrv->pu8_PageSpareBuf + pNandDrv->u16_SectorSpareByteCnt);

	u16_Start_Blk = drvNAND_BBT_Rev_StartBlk();
	for(u32_GoodBlkIdx = u16_Start_Blk; u32_GoodBlkIdx < (pNandDrv->u16_BlkCnt-2); u32_GoodBlkIdx ++)
	{
		u32_Err = drvNAND_LFSRReadPhyPage(u32_GoodBlkIdx << pNandDrv->u8_BlkPageCntBits, pu8_PageDataBuf, pu8_PageSpareBuf);
		if(pu8_PageSpareBuf[0] == 0xFF)
			break;
	}
    if(u32_GoodBlkIdx==(pNandDrv->u16_BlkCnt-2))//No free block to do bakup
        return 0;

	nand_debug(UNFD_DEBUG_LEVEL_LOW, 1,"Last good block is find at %lX\n", u32_GoodBlkIdx);

	//check block content
	// read first and last lsb page for checking ecc and empty
	if(u32_Err != UNFD_ST_SUCCESS)
	{
		nand_debug(UNFD_DEBUG_LEVEL_LOW, 1,"Read fail, erase cis block %lX\n", u32_GoodBlkIdx);
		//erase block and skip process
		u32_Err = NC_EraseBlk(u32_GoodBlkIdx << pNandDrv->u8_BlkPageCntBits);
		if(u32_Err != UNFD_ST_SUCCESS)
		{
			//mark bad
			if(nand_markbad == NULL)
				drvNAND_MarkBadBlk(u32_GoodBlkIdx);
			else
				nand_markbad(u32_GoodBlkIdx);
		}
		return 0;
	}
	// spare of 1st sector   -> only 0xFF
	// spare of 2nd sector  -> blockrow
	// spare of 3rd sector   -> marker of 3697
	// spare of 4th sector   -> Customer Marker	
	if(pu8_PageSpareBuf[pNandDrv->u16_SectorSpareByteCnt * 2] == 0xFF && pu8_PageSpareBuf[pNandDrv->u16_SectorSpareByteCnt * 2 + 1] == 0xFF)
	{
		nand_debug(UNFD_DEBUG_LEVEL_LOW, 1, "First page is empty\n");
		//done for the resetore
		return 0;
	}
	if(!(pu8_PageSpareBuf[pNandDrv->u16_SectorSpareByteCnt * 2] == 0x36 && pu8_PageSpareBuf[pNandDrv->u16_SectorSpareByteCnt * 2 + 1] ==0x97))
	{
		nand_debug(UNFD_DEBUG_LEVEL_LOW, 1, "Dummy data find at backup block @ %lX, %X, %X\n", 
			u32_GoodBlkIdx, pu8_PageSpareBuf[pNandDrv->u16_SectorSpareByteCnt * 2], pu8_PageSpareBuf[pNandDrv->u16_SectorSpareByteCnt * 2 + 1]);
		return 0;
	}

	nand_debug(UNFD_DEBUG_LEVEL_LOW, 1, "Read last page of last good cis block\n");

	u32_Err = drvNAND_LFSRReadPhyPage((u32_GoodBlkIdx << pNandDrv->u8_BlkPageCntBits) 
		+ (pNandDrv->u16_BlkPageCnt - 1), pu8_PageDataBuf, pu8_PageSpareBuf);
	if(u32_Err != UNFD_ST_SUCCESS ||
			 (pu8_PageSpareBuf[pNandDrv->u16_SectorSpareByteCnt * 2] == 0xFF && pu8_PageSpareBuf[pNandDrv->u16_SectorSpareByteCnt * 2 + 1] == 0xFF) 
			 || (pu8_PageSpareBuf[pNandDrv->u16_SectorSpareByteCnt * 2] != 0x36 || pu8_PageSpareBuf[pNandDrv->u16_SectorSpareByteCnt * 2 + 1] != 0x97))
	{
		nand_debug(UNFD_DEBUG_LEVEL_LOW, 1, "Read fail or last page is empty, Erase last good cis block\n");
		u32_Err = NC_EraseBlk(u32_GoodBlkIdx << pNandDrv->u8_BlkPageCntBits);
		if(u32_Err != UNFD_ST_SUCCESS)
		{
			//mark bad
			if(nand_markbad == NULL)
				drvNAND_MarkBadBlk(u32_GoodBlkIdx);
			else
				nand_markbad(u32_GoodBlkIdx);
		}
		return 0;
	}

	u16_SrcPBA = *pu16_BlkPBA;
	nand_debug(UNFD_DEBUG_LEVEL_LOW, 1,"Erase Source block %X\n", u16_SrcPBA);
	//erase source block
	u32_Err = NC_EraseBlk(u16_SrcPBA << pNandDrv->u8_BlkPageCntBits);
	if(u32_Err != UNFD_ST_SUCCESS)
	{
		//mark bad and do shifting...
	}
	nand_debug(UNFD_DEBUG_LEVEL, 1,"Write Back data to Source block %X\n", u16_SrcPBA);
	//restore data to source block
	for(u16_i = 0; u16_i < pNandDrv->u16_BlkPageCnt; u16_i ++)
	{
		u32_Err = drvNAND_LFSRReadPhyPage((u32_GoodBlkIdx << pNandDrv->u8_BlkPageCntBits) + u16_i
			, pu8_PageDataBuf, pu8_PageSpareBuf);
		if(u32_Err != UNFD_ST_SUCCESS)
		{
			//fatal error
			nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1,"Read backup block ecc fail, when restoring data. Please re-upgrade data to nand offset 0x%X\n", 
											u16_SrcPBA << (pNandDrv->u8_BlkPageCntBits + pNandDrv->u8_BlkPageCntBits));
			return UNFD_ST_ERR_DATA_CMP_FAIL;
		}
		else
		{
			if(pu8_PageSpareBuf[pNandDrv->u16_SectorSpareByteCnt * 2] == 0x36 && pu8_PageSpareBuf[pNandDrv->u16_SectorSpareByteCnt * 2 + 1] == 0x97)
			{
				u8_CustSpareMarker = pu8_PageSpareBuf[pNandDrv->u16_SectorSpareByteCnt*3];
				u8_CustMarkerIndex = pu8_PageSpareBuf[pNandDrv->u16_SectorSpareByteCnt*3 + 1];
				memset(pu8_PageSpareBuf, 0xFF, pNandDrv->u16_SpareByteCnt);
				if(u8_CustMarkerIndex !=0xFF && u8_CustSpareMarker != 0xFF && u8_CustMarkerIndex < pNandDrv->u16_SpareByteCnt)
					pu8_PageSpareBuf[u8_CustMarkerIndex] = u8_CustSpareMarker;
				if(u8_CustMarkerIndex !=0xFF)		// because kernel do RD of hash or bootloader the u8_CustMarkerIndex = 1 should disable randomizer
				{
					#if defined(FCIE_LFSR) && FCIE_LFSR
					if(pNandDrv->u8_RequireRandomizer == 1)
						NC_DisableLFSR();
					#endif
					u32_Err = NC_WritePages((u16_SrcPBA << pNandDrv->u8_BlkPageCntBits) + u16_i,
							pu8_PageDataBuf, pu8_PageSpareBuf, 1);
				}
				else
					u32_Err = drvNAND_LFSRWritePhyPage((u16_SrcPBA << pNandDrv->u8_BlkPageCntBits) + u16_i,
							pu8_PageDataBuf, pu8_PageSpareBuf);
				if(u32_Err != UNFD_ST_SUCCESS)
				{
					//mark bad and do shifting...
				}
			}
			else
			{
				nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1,"Read backup block ecc fail, when restoring data. Please re-upgrade data to nand offset 0x%X\n", 
											u16_SrcPBA << (pNandDrv->u8_BlkPageCntBits + pNandDrv->u8_BlkPageCntBits));
				return UNFD_ST_ERR_DATA_CMP_FAIL;
			}
		}
	}
	
	//Erase Backup block
	u32_Err = NC_EraseBlk(u32_GoodBlkIdx << pNandDrv->u8_BlkPageCntBits);
	if(u32_Err != UNFD_ST_SUCCESS)
	{
		//mark bad
		if(nand_markbad == NULL)
			drvNAND_MarkBadBlk(u32_GoodBlkIdx);
		else
			nand_markbad(u32_GoodBlkIdx);
	}
	return 0;
}

/*
 *drvNAND_WriteDummyToMSBPage
 * write dummy data to MSB page
 * @u32_PairPageIdx : the last Pair-Page Index that the LSB page is written.
 * @u32_BlkRow : row address of the block
 * @pu8_PageDataBuf : data buffer filled with dummy data
 * @pu8_SpareDataBuf: spare buffer filled with dummy data
 */

U32 drvNAND_WriteDummyToMSBPage(U32 u32_PairPageIdx, U32 u32_BlkRow, U8* pu8_PageDataBuf, U8* pu8_SpareDataBuf, U32 (*nand_markbad)(U32))
{
	NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();
	int j, BlkPageCnt, EndPage;
	U32 u32_Err;
	
	if(pNandDrv->u8_CellType == NAND_CellType_SLC)
		return UNFD_ST_SUCCESS;
	
	BlkPageCnt = pNandDrv->u16_BlkPageCnt >> pNandDrv->u8_CellType;
	if(u32_PairPageIdx >= BlkPageCnt)
		return UNFD_ST_ERR_INVALID_PARAM;
	
	//Write Dummy data to MSB Page of MLC NAND	
	if(u32_PairPageIdx != BlkPageCnt - 1)
        EndPage = ga_tPairedPageMap[u32_PairPageIdx + 1].u16_LSB;
    else
        EndPage = pNandDrv->u16_BlkPageCnt;
    
    for(j = ga_tPairedPageMap[u32_PairPageIdx].u16_LSB + 1; j < EndPage; j ++)
	{
		//nand_debug(UNFD_DEBUG_LEVEL_WARNING, 1, "write page :0x%X\n", u32_BlkRow + j);
			
		u32_Err = NC_WritePages(u32_BlkRow + j, pu8_PageDataBuf, pu8_SpareDataBuf, 1);
		if(u32_Err != UNFD_ST_SUCCESS)
		{
			nand_debug (UNFD_DEBUG_LEVEL_ERROR,1,"NAND write to offset %lx failed %ld\n",
				u32_BlkRow + j, u32_Err);
			nand_debug (UNFD_DEBUG_LEVEL_ERROR,1,"Mark current block as bad block\n");
			//mark bad
			if(nand_markbad == NULL)
				drvNAND_MarkBadBlk(u32_BlkRow >> pNandDrv->u8_BlkPageCntBits);
			else
				nand_markbad(u32_BlkRow >> pNandDrv->u8_BlkPageCntBits);

			u32_BlkRow += pNandDrv->u16_BlkPageCnt;
			return u32_Err;
		}
	}
	return UNFD_ST_SUCCESS;

}


U32 drvNAND_WriteBlkByteOffset(U8 *pu8_DestAddr, U8 u8_PartType, U16 u16_PBA,
					 U16 u16_LBA, U32 u32_StartByte,
					 U32 u32_ByteCnt)
{
	NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();
	U8 *au8_PageBuf = (U8*)pNandDrv->pu8_PageDataBuf;
	U8 *au8_SpareBuf = (U8*)pNandDrv->pu8_PageSpareBuf;
	U32 u32_Err;
	//MBBS_INFO_t* SPAREINFO =(MBBS_INFO_t*) pNandDrv->pu8_PageSpareBuf;
	U32 u32_SrcPageIdx, u32_BakPageIdx;
	U16 u16_PageCnt;
	U16 u16_ByteCntRead;
	U16 u16_ByteIdxInPage;
	U16 u16_i;
	U16 u16_LastWrittenPage = 0;
	U16 u16_BakBlkIdx;
	U8 u8_GoodBlkCnt;
	U16 u16_BlkPageCnt;
	U16 *pu16_BlkPBA = (U16 *)(pNandDrv->pu8_PageSpareBuf + pNandDrv->u16_SectorSpareByteCnt);

	#if defined(FCIE_LFSR) && FCIE_LFSR
	if(pNandDrv->u8_RequireRandomizer)
	{
		NC_DisableLFSR();
	}
	#endif

	u16_BlkPageCnt = pNandDrv->u16_BlkPageCnt;
	if(pNandDrv->u8_CellType == NAND_CellType_MLC)	// MLC
		u16_BlkPageCnt = u16_BlkPageCnt>>1;	

	/* sector size from AP is 512 Bytes, not NAND sector size */
	nand_debug(UNFD_DEBUG_LEVEL_HIGH, 1, "BlkIdx:%d, BlkNo:%d, StartByte:0x%X, ByteCnt:0x%X\n", (int)u16_PBA, (int)u16_LBA, (int)u32_StartByte, (int)u32_ByteCnt);

	/* Find physical Backup BlkIdx in CIS remain block */
	u16_BakBlkIdx = 9;
	u8_GoodBlkCnt = 0;
	while(1)
	{
		if(!drvNAND_IsGoodBlk(u16_BakBlkIdx))
		{
			nand_debug(UNFD_DEBUG_LEVEL_WARNING, 1, "Skip bad blk: 0x%04x\n", u16_BakBlkIdx);
		}
		else
		{
			if(++u8_GoodBlkCnt == 1)
				break;
		}

		if((--u16_BakBlkIdx) < 5)
		{
			nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "too many bad block\n");
			return UNFD_ST_ERR_LACK_BLK;
		}
	}
	nand_debug(UNFD_DEBUG_LEVEL_HIGH, 1, "u16_BakBlkIdx:%d\n", u16_BakBlkIdx);

	//nand_debug(UNFD_DEBUG_LEVEL_WARNING, 1, "erase u16_BakBlkIdx:%d\n", u16_BakBlkIdx);

	// erase bak block
	u32_Err = drvNAND_ErasePhyBlk(u16_BakBlkIdx);
	if (u32_Err != UNFD_ST_SUCCESS) {
		nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Erase Blk 0x%04x failed with EC: 0x%08lx\n",
				   u16_BakBlkIdx, u32_Err);
		drvNAND_MarkBadBlk(u16_BakBlkIdx);
		return u32_Err;
	}

	//copy data from src block to backup block before the startbyte
	for(u16_i=0 ; u16_i<(u32_StartByte>>pNandDrv->u8_PageByteCntBits) ; u16_i++)
	{
		u32_SrcPageIdx = (u16_PBA << pNandDrv->u8_BlkPageCntBits) + ga_tPairedPageMap[u16_i].u16_LSB;
		
		//nand_debug(UNFD_DEBUG_LEVEL_WARNING, 1, "copy data before start byte read  page :0x%X\n", u32_SrcPageIdx);
		u32_Err = NC_ReadPages(u32_SrcPageIdx, au8_PageBuf, au8_SpareBuf, 1);
		if (u32_Err != UNFD_ST_SUCCESS)
		{
			nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Reading page 0x%04lx failed with EC: 0x%04lx\n",
					   u32_SrcPageIdx, u32_Err);

			return u32_Err;
		}
//		SPAREINFO->u8_BadBlkMarker = 0xFF;
//		SPAREINFO->u16_BackupPBA = u16_PBA;
//		au8_SpareBuf[3] = 0x36;
//		au8_SpareBuf[4] = 0x97;
		
		au8_SpareBuf[0] = 0xFF;
		*pu16_BlkPBA = u16_PBA;
		au8_SpareBuf[pNandDrv->u16_SectorSpareByteCnt * 2] = 0x36;
		au8_SpareBuf[pNandDrv->u16_SectorSpareByteCnt * 2 + 1] = 0x97;
		au8_SpareBuf[pNandDrv->u16_SectorSpareByteCnt * 3] = 0xA1;
		au8_SpareBuf[pNandDrv->u16_SectorSpareByteCnt * 3 + 1] = 1;		
		u32_BakPageIdx = (u16_BakBlkIdx << pNandDrv->u8_BlkPageCntBits) + ga_tPairedPageMap[u16_i].u16_LSB;
		//nand_debug(UNFD_DEBUG_LEVEL_WARNING, 1, "write  page :0x%X\n", u32_BakPageIdx);
		
		u32_Err = NC_WritePages(u32_BakPageIdx, au8_PageBuf, au8_SpareBuf, 1);
		if (u32_Err != UNFD_ST_SUCCESS)
		{
			nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Writing page 0x%04lx failed with EC: 0x%04lx\n",
					   u32_BakPageIdx, u32_Err);
			drvNAND_MarkBadBlk(u16_BakBlkIdx);
			return u32_Err;
		}
		memset(au8_PageBuf, 0xFF, pNandDrv->u16_PageByteCnt);
		
		u32_Err = drvNAND_WriteDummyToMSBPage(u16_i, (u16_BakBlkIdx << pNandDrv->u8_BlkPageCntBits), au8_PageBuf, au8_SpareBuf, NULL);
		if(u32_Err != UNFD_ST_SUCCESS)
		{
			nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Writing page 0x%04lx failed with EC: 0x%04lx\n",
					   u32_BakPageIdx, u32_Err);
			return u32_Err;		
		}
		u16_LastWrittenPage = u16_i;
	}

	/* Read unaligned Byte first */
	if( u32_StartByte & pNandDrv->u16_PageByteCntMask)
	{
		u32_SrcPageIdx = (u16_PBA << pNandDrv->u8_BlkPageCntBits) + ga_tPairedPageMap[(u32_StartByte>>pNandDrv->u8_PageByteCntBits)].u16_LSB;

		//nand_debug(UNFD_DEBUG_LEVEL_WARNING, 1, "unaligned byte first read  page :0x%X\n", u32_SrcPageIdx);		
		u32_Err = NC_ReadPages(u32_SrcPageIdx, au8_PageBuf, au8_SpareBuf, 1);
		if (u32_Err != UNFD_ST_SUCCESS) {

			nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Reading page 0x%04lx failed with EC: 0x%04lx\n",
					   u32_SrcPageIdx, u32_Err);

			return u32_Err;
		}
		u16_ByteIdxInPage = u32_StartByte & pNandDrv->u16_PageByteCntMask;
		u16_ByteCntRead = (u32_ByteCnt > ((U32)pNandDrv->u16_PageByteCnt-(U32)u16_ByteIdxInPage)) ? ((U32)pNandDrv->u16_PageByteCnt-(U32)u16_ByteIdxInPage) : (u32_ByteCnt);

		memcpy((au8_PageBuf+u16_ByteIdxInPage), pu8_DestAddr, u16_ByteCntRead);
		u32_BakPageIdx = (u16_BakBlkIdx << pNandDrv->u8_BlkPageCntBits) + ga_tPairedPageMap[(u32_StartByte>>pNandDrv->u8_PageByteCntBits)].u16_LSB;
		
//		SPAREINFO->u8_BadBlkMarker = 0xFF;
//		SPAREINFO->u16_BackupPBA = u16_PBA;
//		au8_SpareBuf[3] = 0x36;
//		au8_SpareBuf[4] = 0x97;

		au8_SpareBuf[0] = 0xFF;
		*pu16_BlkPBA = u16_PBA;
		au8_SpareBuf[pNandDrv->u16_SectorSpareByteCnt * 2] = 0x36;
		au8_SpareBuf[pNandDrv->u16_SectorSpareByteCnt * 2 + 1] = 0x97;
		au8_SpareBuf[pNandDrv->u16_SectorSpareByteCnt * 3] = 0xA1;
		au8_SpareBuf[pNandDrv->u16_SectorSpareByteCnt * 3 + 1] = 1;			
		//nand_debug(UNFD_DEBUG_LEVEL_WARNING, 1, "write  page :0x%X\n", u32_BakPageIdx);
		u32_Err = NC_WritePages(u32_BakPageIdx, au8_PageBuf, au8_SpareBuf, 1);
		if (u32_Err != UNFD_ST_SUCCESS) {

			nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Writing page 0x%04lx failed with EC: 0x%04lx\n",
					   u32_BakPageIdx, u32_Err);
			drvNAND_MarkBadBlk(u16_BakBlkIdx);
			return u32_Err;
		}

		memset(au8_PageBuf, 0xFF, pNandDrv->u16_PageByteCnt);
		u32_Err = drvNAND_WriteDummyToMSBPage((u32_StartByte>>pNandDrv->u8_PageByteCntBits), (u16_BakBlkIdx << pNandDrv->u8_BlkPageCntBits), au8_PageBuf, au8_SpareBuf, NULL);
		if(u32_Err != UNFD_ST_SUCCESS)
		{
			nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Writing page 0x%04lx failed with EC: 0x%04lx\n",
					   u32_BakPageIdx, u32_Err);
			return u32_Err;		
		}

		pu8_DestAddr += u16_ByteCntRead;
		u32_StartByte += u16_ByteCntRead;
		u32_ByteCnt -= u16_ByteCntRead;
		u16_LastWrittenPage = (u32_StartByte>>pNandDrv->u8_PageByteCntBits);
	}

	/* Read aligned sectors then */
//	SPAREINFO->u8_BadBlkMarker = 0xFF;
//	SPAREINFO->u16_BackupPBA = u16_PBA;
//	au8_SpareBuf[3] = 0x36;
//	au8_SpareBuf[4] = 0x97;

	au8_SpareBuf[0] = 0xFF;
	*pu16_BlkPBA = u16_PBA;
	au8_SpareBuf[pNandDrv->u16_SectorSpareByteCnt * 2] = 0x36;
	au8_SpareBuf[pNandDrv->u16_SectorSpareByteCnt * 2 + 1] = 0x97;	
	au8_SpareBuf[pNandDrv->u16_SectorSpareByteCnt * 3] = 0xA1;
	au8_SpareBuf[pNandDrv->u16_SectorSpareByteCnt * 3 + 1] = 1; 

	u32_BakPageIdx = (u16_BakBlkIdx << pNandDrv->u8_BlkPageCntBits) + ga_tPairedPageMap[(u32_StartByte>>pNandDrv->u8_PageByteCntBits)].u16_LSB;
	u16_PageCnt = (u32_ByteCnt>>pNandDrv->u8_PageByteCntBits);
	while (u16_PageCnt)
	{
	
		//nand_debug(UNFD_DEBUG_LEVEL_WARNING, 1, "aligned write page :0x%X\n", u32_BakPageIdx);
		u32_Err = NC_WritePages(u32_BakPageIdx, pu8_DestAddr, au8_SpareBuf, 1);
		if (u32_Err != UNFD_ST_SUCCESS) {

			nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Writing page 0x%04lx failed with EC: 0x%04lx\n",
					   u32_BakPageIdx, u32_Err);
			drvNAND_MarkBadBlk(u16_BakBlkIdx);
			return u32_Err;
		}

		memset(au8_PageBuf, 0xFF, pNandDrv->u16_PageByteCnt);
		u32_Err = drvNAND_WriteDummyToMSBPage((u32_StartByte>>pNandDrv->u8_PageByteCntBits), (u16_BakBlkIdx << pNandDrv->u8_BlkPageCntBits), au8_PageBuf, au8_SpareBuf, NULL);
		if(u32_Err != UNFD_ST_SUCCESS)
		{
			nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Writing page 0x%04lx failed with EC: 0x%04lx\n",
					   u32_BakPageIdx, u32_Err);
			return u32_Err;		
		}

		u16_LastWrittenPage = u32_BakPageIdx -(u16_BakBlkIdx << pNandDrv->u8_BlkPageCntBits);
		pu8_DestAddr += pNandDrv->u16_PageByteCnt;
		u16_PageCnt--;
		u32_StartByte += pNandDrv->u16_PageByteCnt;
		u32_BakPageIdx = (u16_BakBlkIdx << pNandDrv->u8_BlkPageCntBits) + ga_tPairedPageMap[(u32_StartByte>>pNandDrv->u8_PageByteCntBits)].u16_LSB;		
		u32_ByteCnt -= pNandDrv->u16_PageByteCnt;
	}

	/* Read remaining unaligned sectors finally */
	if( u32_ByteCnt)
	{
		u32_SrcPageIdx = (u16_PBA << pNandDrv->u8_BlkPageCntBits) + ga_tPairedPageMap[(u32_StartByte>>pNandDrv->u8_PageByteCntBits)].u16_LSB;
		//nand_debug(UNFD_DEBUG_LEVEL_WARNING, 1, "remaining unaligned read  page :0x%X\n", u32_SrcPageIdx);
		
		u32_Err = NC_ReadPages(u32_SrcPageIdx, au8_PageBuf, au8_SpareBuf, 1);
		if (u32_Err != UNFD_ST_SUCCESS) {

			nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Reading page 0x%04lx failed with EC: 0x%04lx\n",
					   u32_SrcPageIdx, u32_Err);

			return u32_Err;
		}

//		SPAREINFO->u8_BadBlkMarker = 0xFF;
//		SPAREINFO->u16_BackupPBA = u16_PBA;
//		au8_SpareBuf[3] = 0x36;
//		au8_SpareBuf[4] = 0x97;

		au8_SpareBuf[0] = 0xFF;
		*pu16_BlkPBA = u16_PBA;
		au8_SpareBuf[pNandDrv->u16_SectorSpareByteCnt * 2] = 0x36;
		au8_SpareBuf[pNandDrv->u16_SectorSpareByteCnt * 2 + 1] = 0x97;
		au8_SpareBuf[pNandDrv->u16_SectorSpareByteCnt * 3] = 0xA1;
		au8_SpareBuf[pNandDrv->u16_SectorSpareByteCnt * 3 + 1] = 1;	

		memcpy(au8_PageBuf, pu8_DestAddr, u32_ByteCnt);
		u32_BakPageIdx = (u16_BakBlkIdx << pNandDrv->u8_BlkPageCntBits) + ga_tPairedPageMap[(u32_StartByte>>pNandDrv->u8_PageByteCntBits)].u16_LSB;		
		//nand_debug(UNFD_DEBUG_LEVEL_WARNING, 1, "write  page :0x%X\n", u32_BakPageIdx);
		
		u32_Err = NC_WritePages(u32_BakPageIdx, au8_PageBuf, au8_SpareBuf, 1);
		if (u32_Err != UNFD_ST_SUCCESS) {

			nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Writing page 0x%04lx failed with EC: 0x%04lx\n",
					   u32_BakPageIdx, u32_Err);
			drvNAND_MarkBadBlk(u16_BakBlkIdx);
			return u32_Err;
		}
		
		memset(au8_PageBuf, 0xFF, pNandDrv->u16_PageByteCnt);
		u32_Err = drvNAND_WriteDummyToMSBPage((u32_StartByte>>pNandDrv->u8_PageByteCntBits), (u16_BakBlkIdx << pNandDrv->u8_BlkPageCntBits), au8_PageBuf, au8_SpareBuf, NULL);
		if(u32_Err != UNFD_ST_SUCCESS)
		{
			nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Writing page 0x%04lx failed with EC: 0x%04lx\n",
					   u32_BakPageIdx, u32_Err);
			return u32_Err;		
		}
		
		u16_LastWrittenPage = (u32_StartByte>>pNandDrv->u8_PageByteCntBits);
	}

	//copy remain data from PBA to Backup block
	for(u16_i=(u16_LastWrittenPage+1) ; u16_i<u16_BlkPageCnt; u16_i++)
	{
		u32_SrcPageIdx = (u16_PBA << pNandDrv->u8_BlkPageCntBits) + ga_tPairedPageMap[u16_i].u16_LSB;
		//nand_debug(UNFD_DEBUG_LEVEL_WARNING, 1, "remain data read page :0x%X\n", u32_SrcPageIdx);
		
		u32_Err = NC_ReadPages(u32_SrcPageIdx, au8_PageBuf, au8_SpareBuf, 1);
		if (u32_Err != UNFD_ST_SUCCESS)
		{
			nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Reading page 0x%04lx failed with EC: 0x%04lx\n",
					   u32_SrcPageIdx, u32_Err);

			return u32_Err;
		}
//		SPAREINFO->u8_BadBlkMarker = 0xFF;
//		SPAREINFO->u16_BackupPBA = u16_PBA;
//		au8_SpareBuf[3] = 0x36;
//		au8_SpareBuf[4] = 0x97;
		
		au8_SpareBuf[0] = 0xFF;
		*pu16_BlkPBA = u16_PBA;
		au8_SpareBuf[pNandDrv->u16_SectorSpareByteCnt * 2] = 0x36;
		au8_SpareBuf[pNandDrv->u16_SectorSpareByteCnt * 2 + 1] = 0x97;	
		au8_SpareBuf[pNandDrv->u16_SectorSpareByteCnt * 3] = 0xA1;
		au8_SpareBuf[pNandDrv->u16_SectorSpareByteCnt * 3 + 1] = 1;	

		u32_BakPageIdx = (u16_BakBlkIdx << pNandDrv->u8_BlkPageCntBits) + ga_tPairedPageMap[u16_i].u16_LSB;
		
		//nand_debug(UNFD_DEBUG_LEVEL_WARNING, 1, "write  page :0x%X\n", u32_BakPageIdx);
		u32_Err = NC_WritePages(u32_BakPageIdx, au8_PageBuf, au8_SpareBuf, 1);
		if (u32_Err != UNFD_ST_SUCCESS)
		{
			nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Writing page 0x%04lx failed with EC: 0x%04lx\n",
					   u32_BakPageIdx, u32_Err);
			drvNAND_MarkBadBlk(u16_BakBlkIdx);
			return u32_Err;
		}
		
		memset(au8_PageBuf, 0xFF, pNandDrv->u16_PageByteCnt);
		u32_Err = drvNAND_WriteDummyToMSBPage(u16_i, (u16_BakBlkIdx << pNandDrv->u8_BlkPageCntBits), au8_PageBuf, au8_SpareBuf, NULL);
		if(u32_Err != UNFD_ST_SUCCESS)
		{
			nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Writing page 0x%04lx failed with EC: 0x%04lx\n",
					   u32_BakPageIdx, u32_Err);
			return u32_Err;		
		}
		
	}
	
	// erase src block
	
	//nand_debug(UNFD_DEBUG_LEVEL_WARNING, 1, "erase src block :0x%X\n", u16_PBA);
	u32_Err = drvNAND_ErasePhyBlk(u16_PBA);
	if (u32_Err != UNFD_ST_SUCCESS) {
        nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Erase Blk 0x%04x failed with EC: 0x%08x\n",
            (unsigned int)u16_PBA, (unsigned int)u32_Err);
		drvNAND_MarkBadBlk(u16_PBA);
		return u32_Err;
	}

	// copy bak to src
	for(u16_i=0 ; u16_i<u16_BlkPageCnt; u16_i++)
	{
		u32_BakPageIdx = (u16_BakBlkIdx << pNandDrv->u8_BlkPageCntBits) + ga_tPairedPageMap[u16_i].u16_LSB;
		//nand_debug(UNFD_DEBUG_LEVEL_WARNING, 1, "read page :0x%X\n", u32_BakPageIdx);
		
		u32_Err = NC_ReadPages(u32_BakPageIdx, au8_PageBuf, au8_SpareBuf, 1);
		if (u32_Err != UNFD_ST_SUCCESS)
		{
			nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Reading page 0x%04lx failed with EC: 0x%04lx\n",
					   u32_BakPageIdx, u32_Err);

			return u32_Err;
		}

		u32_SrcPageIdx = (u16_PBA << pNandDrv->u8_BlkPageCntBits) + ga_tPairedPageMap[u16_i].u16_LSB;
		//nand_debug(UNFD_DEBUG_LEVEL_WARNING, 1, "write  page :0x%X\n", u32_SrcPageIdx);
		
		memset(au8_SpareBuf, 0xFF, pNandDrv->u16_SpareByteCnt);
		u32_Err = NC_WritePages(u32_SrcPageIdx, au8_PageBuf, au8_SpareBuf, 1);
		if (u32_Err != UNFD_ST_SUCCESS)
		{
			nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Reading page 0x%04lx failed with EC: 0x%04lx\n",
					   u32_SrcPageIdx, u32_Err);
			drvNAND_MarkBadBlk(u16_PBA);
			return u32_Err;
		}

		memset(au8_PageBuf, 0xFF, pNandDrv->u16_PageByteCnt);
		u32_Err = drvNAND_WriteDummyToMSBPage(u16_i, (u16_PBA << pNandDrv->u8_BlkPageCntBits), au8_PageBuf, au8_SpareBuf, NULL);
		if(u32_Err != UNFD_ST_SUCCESS)
		{
			nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Writing page 0x%04lx failed with EC: 0x%04lx\n",
					   u32_BakPageIdx, u32_Err);
			return u32_Err;		
		}		
	}

	// erase bak block
	u32_Err = drvNAND_ErasePhyBlk(u16_BakBlkIdx);
	if (u32_Err != UNFD_ST_SUCCESS) {
		nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Erase Blk 0x%04x failed with EC: 0x%08lx\n",
				   u16_BakBlkIdx, u32_Err);
		drvNAND_MarkBadBlk(u16_BakBlkIdx);
		return u32_Err;
	}

	return UNFD_ST_SUCCESS;
}


U32 drvNAND_ReadBlkByteOffset(U8 *pu8_DestAddr, U8 u8_PartType, U16 u16_PBA,
							U16 u16_LBA, U32 u32_StartByte, U32 u32_ByteCnt)
{
	NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();
	U8 *au8_PageBuf = (U8*)pNandDrv->pu8_PageDataBuf;
	U8 *au8_SpareBuf = (U8*)pNandDrv->pu8_PageSpareBuf;
	U32 u32_Err;
	//BLK_INFO_t *pBlkInfo = (BLK_INFO_t*)au8_SpareBuf;
	U32 u32_PageIdx;
	U16 u16_PageCnt;
	U16 u16_ByteCntRead;
	U16 u16_ByteInPage;
	
	#if defined(FCIE_LFSR) && FCIE_LFSR
	if(pNandDrv->u8_RequireRandomizer)
	{
		NC_DisableLFSR();
	}
	#endif
	
	nand_debug(UNFD_DEBUG_LEVEL_HIGH, 1, "BlkIdx:%d, BlkNo:%d, StartByte:%d, ByteCnt:%d\n", (unsigned int)u16_PBA, (unsigned int)u16_LBA, (unsigned int)u32_StartByte, (unsigned int)u32_ByteCnt);
	
	/* Read unaligned sectors first */
	if( u32_StartByte & pNandDrv->u16_PageByteCntMask)
	{
		u16_ByteInPage = u32_StartByte & pNandDrv->u16_PageByteCntMask;
		u16_ByteCntRead = u32_ByteCnt > ((U32)pNandDrv->u16_PageByteCnt-(U32)u16_ByteInPage) ? ((U32)pNandDrv->u16_PageByteCnt-(U32)u16_ByteInPage) : (u32_ByteCnt);
		u32_PageIdx = (u16_PBA << pNandDrv->u8_BlkPageCntBits) + ga_tPairedPageMap[(u32_StartByte>>pNandDrv->u8_PageByteCntBits)].u16_LSB; ;
		
		u32_Err = NC_ReadPages(u32_PageIdx, au8_PageBuf, au8_SpareBuf, 1);
		if (u32_Err != UNFD_ST_SUCCESS) {

	        nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Reading page 0x%04x failed with EC: 0x%04x\n",
                    (unsigned int)u32_PageIdx, (unsigned int)u32_Err);
			return u32_Err;
		}

		memcpy(pu8_DestAddr, au8_PageBuf + u16_ByteInPage, u16_ByteCntRead);
		pu8_DestAddr += u16_ByteCntRead;
		u32_StartByte += u16_ByteCntRead;
		u32_ByteCnt -= u16_ByteCntRead;
	}

	/* Read aligned sectors then */
	u32_PageIdx = (u16_PBA << pNandDrv->u8_BlkPageCntBits) + ga_tPairedPageMap[(u32_StartByte>>pNandDrv->u8_PageByteCntBits)].u16_LSB; ;
	u16_PageCnt = (u32_ByteCnt>>pNandDrv->u8_PageByteCntBits);

	while (u16_PageCnt)
	{
		u32_Err = NC_ReadPages(u32_PageIdx, pu8_DestAddr, au8_SpareBuf, 1);
		if (u32_Err != UNFD_ST_SUCCESS) {

	                nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Reading page 0x%04x failed with EC: 0x%04x\n",
	                    (unsigned int)u32_PageIdx, (unsigned int)u32_Err);
			return u32_Err;
		}

		pu8_DestAddr += pNandDrv->u16_PageByteCnt;
		u16_PageCnt--;
		u32_StartByte += pNandDrv->u16_PageByteCnt;
		u32_ByteCnt -= pNandDrv->u16_PageByteCnt;
		u32_PageIdx = (u16_PBA << pNandDrv->u8_BlkPageCntBits) + ga_tPairedPageMap[(u32_StartByte>>pNandDrv->u8_PageByteCntBits)].u16_LSB; ;
		
	}

	/* Read remaining unaligned sectors finally */
	if( u32_ByteCnt )
	{
		u32_PageIdx = (u16_PBA << pNandDrv->u8_BlkPageCntBits) +  ga_tPairedPageMap[(u32_StartByte>>pNandDrv->u8_PageByteCntBits)].u16_LSB; ;
		u32_Err = NC_ReadPages(u32_PageIdx, au8_PageBuf, au8_SpareBuf, 1);
		if (u32_Err != UNFD_ST_SUCCESS) {

			nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Reading page 0x%04x failed with EC: 0x%04x\n",
	                    (unsigned int)u32_PageIdx, (unsigned int)u32_Err);
			return u32_Err;
		}
		memcpy(pu8_DestAddr, au8_PageBuf, u32_ByteCnt);
	}
	return UNFD_ST_SUCCESS;
}


U32 drvNAND_ParseAddrToADMA(U8 * pu8_DMADataBuf,  U32 u32_ByteCnt, U8 u8_Direction, ADMA_DESC_t* ADMADesc, U8 u8_IsEnd)
{
	NAND_DRIVER *pNandDrv = (NAND_DRIVER*)drvNAND_get_DrvContext_address();

	if(u32_ByteCnt & pNandDrv->u16_PageByteCntMask)
		return UNFD_ST_ERR_INVALID_PARAM;
	
	ADMADesc->u32_EndFlag=  (u8_IsEnd & 1);
	ADMADesc->u32_MIUSel = (nand_translate_DMA_MIUSel((U32)pu8_DMADataBuf, u32_ByteCnt) & 3);
	ADMADesc->u32_PageCnt = cpu2le32((u32_ByteCnt >> pNandDrv->u8_PageByteCntBits) & 0x3FF);
	ADMADesc->u32_Address = cpu2le32(nand_translate_DMA_address_Ex((U32)pu8_DMADataBuf, u32_ByteCnt, u8_Direction));
	
	//nand_debug(0, 1, "Main Desc:");
	//dump_mem(ADMADesc, sizeof(ADMA_DESC_t_));
	//nand_debug(0, 0, "\n");
	return UNFD_ST_SUCCESS;
}

U32 drvNAND_WriteBlockECCByPass(U16 u16_PBA, U8* pu8_DataBuf, U16 u16_PageCnt, U16 u16_PhySpareByteCnt)
{    
	NAND_DRIVER *pNandDrv = (NAND_DRIVER*)drvNAND_get_DrvContext_address();
    U32 u32_Err;
    U16 u16_Row, u16_PhyPageByteCntWSpare;
    u16_PhyPageByteCntWSpare = pNandDrv->u16_PageByteCnt + u16_PhySpareByteCnt;
    u32_Err = NC_EraseBlk(u16_PBA << pNandDrv->u8_BlkPageCntBits);
    if(u32_Err)
        return u32_Err;
    for(u16_Row = 0; u16_Row < u16_PageCnt; u16_Row ++)
    {
        #if defined(FCIE_LFSR) && FCIE_LFSR
    	if(pNandDrv->u8_RequireRandomizer == 1)
            NC_DisableLFSR();
        #endif        
        pNandDrv->u16_Reg50_EccCtrl|= BIT_NC_BYPASS_ECC;
        NC_Config();

        //check blank page and skip the following data
        if(0xFF == drvNAND_CheckAll0xFF(pu8_DataBuf + u16_PhyPageByteCntWSpare * u16_Row , pNandDrv->u16_PageByteCnt)
            &&
            0xFF == drvNAND_CheckAll0xFF(pu8_DataBuf + u16_PhyPageByteCntWSpare * u16_Row + pNandDrv->u16_PageByteCnt , u16_PhySpareByteCnt))
        {
            continue;
        }

        u32_Err = NC_WritePages((u16_PBA << pNandDrv->u8_BlkPageCntBits) + u16_Row,
                                pu8_DataBuf + u16_PhyPageByteCntWSpare * u16_Row,
                                pu8_DataBuf + u16_PhyPageByteCntWSpare * u16_Row + pNandDrv->u16_PageByteCnt,
                                1);
        if(u32_Err)
            goto EXIT;
    }

EXIT:
    
    #if defined(FCIE_LFSR) && FCIE_LFSR
    if(pNandDrv->u8_RequireRandomizer == 1)
        NC_EnableLFSR();
    #endif
    pNandDrv->u16_Reg50_EccCtrl &= ~BIT_NC_BYPASS_ECC;
    NC_Config();
    return u32_Err;
}

U16 drvNAND_BBT_Rev_StartBlk(void)
{
    U16 u16_StarBlk;
    NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();

    /* set bbt block number to 0.8% of total blocks, or blocks * (2 / 256) */
    u16_StarBlk = pNandDrv->u16_BlkCnt-((pNandDrv->u16_BlkCnt >> 8) * 2);

    return u16_StarBlk;
}

U16 drvNAND_GetBackupBlk(void)
{
    U16 u16_BackupPBA =0, u16_StarBlk, u16_i;
    NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();

    u16_StarBlk = drvNAND_BBT_Rev_StartBlk();

    for(u16_i= u16_StarBlk; u16_i< (pNandDrv->u16_BlkCnt-2); u16_i++)
    {
        if(drvNAND_IsGoodBlk(u16_i))
        {
            if(drvNAND_IsFreeBlk(u16_i))
            {
                u16_BackupPBA = u16_i;
                break;
            }
        }
    }
    if(u16_i == (pNandDrv->u16_BlkCnt-2))
    {
        nand_debug(UNFD_DEBUG_LEVEL_ERROR, 0,"No free block for bakup\n");
        return 0;
    }
    return u16_BackupPBA;
}


//  0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
const U8 u8FSTYPE[256] =
{
    0,19, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 0
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 1
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 2
    0, 0, 0, 0, 0, 0, 0,16, 0, 0, 0, 0, 0, 0, 0, 0, // 3
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 4
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 5
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 6
    0,18, 0, 6, 0, 8,10, 0, 0,12, 0, 0, 0, 0, 0, 0, // 7
    0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, // 8
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 9
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // A
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // B
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // C
    0,13, 0,16, 0,17, 3, 19, 0, 0,15, 0,14, 0, 0, 0, // D
    0, 0, 0, 2, 0, 2, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, // E
   20,13, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // F
};

PAIRED_PAGE_MAP_t ga_tPairedPageMap[PAIRED_PAGE_COUNT] = {{0,0}};

#define VENDOR_FUJITSU            0x04
#define VENDOR_RENESAS            0x07
#define VENDOR_ST                 0x20
#define VENDOR_MICRON             0x2C
#define VENDOR_NATIONAL           0x8F
#define VENDOR_TOSHIBA            0x98
#define VENDOR_HYNIX              0xAD
#define VENDOR_SAMSUNG            0xEC
#define VENDOR_MXIC               0xC2
#define VENDOR_ZENTEL             0x92
#define VENDOR_ZENTEL1            0xC8
#define VENDOR_SPANSION           0x01
#define VENDOR_WINBOND            0xEF
#define VENDOR_PARAGON			  0x4A


int drvNAND_CHECK_FLASH_TYPE(void)
{
  NAND_DRIVER *pNandDrv = (NAND_DRIVER*)drvNAND_get_DrvContext_address();

  U16 u16_i,u16_j=0;
  U8 u8_more_maker=0;

  if(pNandDrv->au8_ID[0] ==0x7F)
  {
      u8_more_maker=1;
  }

  if(u8_more_maker)
  {
      for (u16_i=1; u16_i<NAND_ID_BYTE_CNT;u16_i++)
      {
         if (pNandDrv->au8_ID[u16_i] == 0x7F)
         {
             u16_j = u16_i;
         }
         else
         {
             pNandDrv->au8_ID[u16_i-u16_j-1] = pNandDrv->au8_ID[u16_i];
         }
      }
      pNandDrv->u8_IDByteCnt -= (u16_j+1);
  }

/*
  printf("NAND ID:");
  for(u16_i = 0; u16_i < pNandDrv->u8_IDByteCnt; u16_i++)
      printf("0x%X ", pNandDrv->au8_ID[u16_i]);
  printf("\n");
*/

  if( (pNandDrv->au8_ID[0] != VENDOR_SAMSUNG) &&
    (pNandDrv->au8_ID[0] != VENDOR_TOSHIBA) &&
    (pNandDrv->au8_ID[0] != VENDOR_RENESAS) &&
    (pNandDrv->au8_ID[0] != VENDOR_HYNIX)  &&
    (pNandDrv->au8_ID[0] != VENDOR_FUJITSU) &&
    (pNandDrv->au8_ID[0] != VENDOR_MICRON) &&
    (pNandDrv->au8_ID[0] != VENDOR_NATIONAL)  &&
    (pNandDrv->au8_ID[0] != VENDOR_ST) &&
    (pNandDrv->au8_ID[0] != VENDOR_MXIC) &&
    (pNandDrv->au8_ID[0] != VENDOR_ZENTEL) &&
    (pNandDrv->au8_ID[0] != VENDOR_ZENTEL1) &&
    (pNandDrv->au8_ID[0] != VENDOR_SPANSION) &&
    (pNandDrv->au8_ID[0] != VENDOR_WINBOND) &&
	(pNandDrv->au8_ID[0] != VENDOR_PARAGON))
  {
    pNandDrv->u16_BlkCnt = 0;
    pNandDrv->u16_BlkPageCnt = 0;
    pNandDrv->u16_PageByteCnt = 0;
    pNandDrv->u16_SectorByteCnt = 0;
    pNandDrv->u16_SpareByteCnt = 0;


    printf("Unsupport Vendor %02X\n", pNandDrv->au8_ID[0] );

    return -1; // unsupported flash maker
  }

    pNandDrv->u16_ECCType = ECC_TYPE_4BIT;
    pNandDrv->u8_PlaneCnt = 1;

    /*Default NAND Timing Setting*/
    pNandDrv->u16_tRC = 25;
    pNandDrv->u8_tRP = 12;
    pNandDrv->u8_tREH = 10;
    pNandDrv->u8_tREA = 20;
    pNandDrv->u8_tRR = 20;
    pNandDrv->u16_tADL = 70;
    pNandDrv->u16_tRHW = 100;
    pNandDrv->u16_tWHR = 60;
    pNandDrv->u16_tCCS = 60;
    pNandDrv->u8_tCS = 20;
    pNandDrv->u16_tWC = 25;
    pNandDrv->u8_tWP = 12;
    pNandDrv->u8_tWH = 10;
    pNandDrv->u16_tCWAW = 0;
    pNandDrv->u8_tCLHZ = 0;
    pNandDrv->u16_tWW = 100;
  switch(u8FSTYPE[pNandDrv->au8_ID[1]])
  {
    case 2:
      pNandDrv->u16_BlkCnt = 512;
      pNandDrv->u16_BlkPageCnt = 16;
      pNandDrv->u16_PageByteCnt = 512;
      pNandDrv->u16_SectorByteCnt = 512;
      pNandDrv->u16_SpareByteCnt = 16;
      break;
    case 4:
      pNandDrv->u16_BlkCnt = 1024;
      pNandDrv->u16_BlkPageCnt = 16;
      pNandDrv->u16_PageByteCnt = 512;
      pNandDrv->u16_SectorByteCnt = 512;
      pNandDrv->u16_SpareByteCnt = 16;
      pNandDrv->u8_OpCode_RW_AdrCycle = ADR_C3TFS0;
      pNandDrv->u8_OpCode_Erase_AdrCycle = ADR_C2TRS0;
      break;
    case 6:
      pNandDrv->u16_BlkCnt = 1024;
      pNandDrv->u16_BlkPageCnt = 32;
      pNandDrv->u16_PageByteCnt = 512;
      pNandDrv->u16_SectorByteCnt = 512;
      pNandDrv->u16_SpareByteCnt = 16;
      pNandDrv->u8_OpCode_RW_AdrCycle = ADR_C3TFS0;
      pNandDrv->u8_OpCode_Erase_AdrCycle = ADR_C2TRS0;
      break;
    case 8:
      pNandDrv->u16_BlkCnt = 2048;
      pNandDrv->u16_BlkPageCnt = 32;
      pNandDrv->u16_PageByteCnt = 512;
      pNandDrv->u16_SectorByteCnt = 512;
      pNandDrv->u16_SpareByteCnt = 16;
      pNandDrv->u8_OpCode_RW_AdrCycle = ADR_C3TFS0;
      pNandDrv->u8_OpCode_Erase_AdrCycle = ADR_C2TRS0;
      break;
    case 10:
      pNandDrv->u16_BlkCnt = 4096;
      pNandDrv->u16_BlkPageCnt = 32;
      pNandDrv->u16_PageByteCnt = 512;
      pNandDrv->u16_SectorByteCnt = 512;
      pNandDrv->u16_SpareByteCnt = 16;
      pNandDrv->u8_OpCode_RW_AdrCycle = ADR_C4TFS0;
      pNandDrv->u8_OpCode_Erase_AdrCycle = ADR_C3TRS0;
      break;
    case 12:
      pNandDrv->u16_BlkCnt = 8192;
      pNandDrv->u16_BlkPageCnt = 32;
      pNandDrv->u16_PageByteCnt = 512;
      pNandDrv->u16_SectorByteCnt = 512;
      pNandDrv->u16_SpareByteCnt = 16;
      pNandDrv->u8_OpCode_RW_AdrCycle = ADR_C4TFS0;
      pNandDrv->u8_OpCode_Erase_AdrCycle = ADR_C3TRS0;
      break;
    case 13:
    if(pNandDrv->au8_ID[0]==VENDOR_TOSHIBA)
    {
        pNandDrv->u16_BlkCnt = 1024;
        pNandDrv->u16_BlkPageCnt = 64;
        pNandDrv->u16_PageByteCnt = 2048;
        pNandDrv->u16_SectorByteCnt = 512;
        pNandDrv->u16_SpareByteCnt = 64;
        pNandDrv->u8_OpCode_RW_AdrCycle = ADR_C4TFS0;
        pNandDrv->u8_OpCode_Erase_AdrCycle = ADR_C2TRS0;
        if((pNandDrv->au8_ID[2] == 0x80) && (pNandDrv->au8_ID[3] == 0x15) &&
            (pNandDrv->au8_ID[4] == 0x72) && (pNandDrv->au8_ID[5] == 0x16))
        {
            pNandDrv->u16_SpareByteCnt = 128;
            pNandDrv->u16_ECCType = ECC_TYPE_8BIT;
        }
        if(((pNandDrv->au8_ID[2] & 0x0F) == 0) &&
            ((pNandDrv->au8_ID[3] & 0x33) == 0x11) &&
            ((pNandDrv->au8_ID[4] & 0x0C)== 0))
        {
            pNandDrv->u16_SpareByteCnt = 128;
            pNandDrv->u16_ECCType = ECC_TYPE_8BIT;
        }
        if((pNandDrv->au8_ID[2] == 0x80) && (pNandDrv->au8_ID[3] == 0x15) &&
            (pNandDrv->au8_ID[4] == 0xF2) && (pNandDrv->au8_ID[5] == 0x16))
        {
            pNandDrv->u16_SpareByteCnt = 64;
            pNandDrv->u16_ECCType = ECC_TYPE_8BIT;
        }
    }
    else if(pNandDrv->au8_ID[0]==VENDOR_WINBOND)
    {
        pNandDrv->u16_BlkCnt = 1024;
        pNandDrv->u16_BlkPageCnt = 64;
        pNandDrv->u16_PageByteCnt = 2048;
        pNandDrv->u16_SectorByteCnt = 512;
        pNandDrv->u16_SpareByteCnt = 64;
        pNandDrv->u8_OpCode_RW_AdrCycle = ADR_C4TFS0;
        pNandDrv->u8_OpCode_Erase_AdrCycle = ADR_C2TRS0;
    }
	else if(pNandDrv->au8_ID[0]==VENDOR_PARAGON)
	{
		pNandDrv->u16_BlkCnt = 1024;
		pNandDrv->u16_BlkPageCnt = 64;
		pNandDrv->u16_PageByteCnt = 2048;
		pNandDrv->u16_SectorByteCnt = 512;
		pNandDrv->u16_SpareByteCnt = 64;
		pNandDrv->u8_OpCode_RW_AdrCycle = ADR_C5TFS0;
		pNandDrv->u8_OpCode_Erase_AdrCycle = ADR_C3TRS0;
	}
	else if((pNandDrv->au8_ID[0] == 0x01) && (pNandDrv->au8_ID[1] == 0xF1) &&
			(pNandDrv->au8_ID[2] == 0x80) && (pNandDrv->au8_ID[3] == 0x1D))
	{	//PARAGON G34ML01G2
		pNandDrv->u16_BlkCnt = 1024;
		pNandDrv->u16_BlkPageCnt = 64;
		pNandDrv->u16_PageByteCnt = 2048;
		pNandDrv->u16_SectorByteCnt = 512;
		pNandDrv->u16_SpareByteCnt = 64;
		pNandDrv->u8_OpCode_RW_AdrCycle = ADR_C5TFS0;
		pNandDrv->u8_OpCode_Erase_AdrCycle = ADR_C3TRS0;
	}
    else
    {
        pNandDrv->u16_BlkCnt = 1024;
        pNandDrv->u16_BlkPageCnt = 64;
        pNandDrv->u16_PageByteCnt = 2048;
        pNandDrv->u16_SectorByteCnt = 512;
        pNandDrv->u16_SpareByteCnt = 64;
        pNandDrv->u8_OpCode_RW_AdrCycle = ADR_C4TFS0;
        pNandDrv->u8_OpCode_Erase_AdrCycle = ADR_C2TRS0;
    }
        break;
    case 14:
        if((pNandDrv->au8_ID[0]==VENDOR_SPANSION))      //spansion 4Gbit
        {
            pNandDrv->u16_BlkCnt = 4096;
            pNandDrv->u16_BlkPageCnt = 64;
            pNandDrv->u16_PageByteCnt = 2048;
            pNandDrv->u16_SectorByteCnt = 512;

            if(pNandDrv->au8_ID[2] == 0x90 &&
               pNandDrv->au8_ID[3] == 0x95 &&
               pNandDrv->au8_ID[4] == 0x56)
            {
                pNandDrv->u16_SpareByteCnt = 128;
            }
            else
            {
                pNandDrv->u16_SpareByteCnt = 64;
            }
        }
        else if((pNandDrv->au8_ID[0]==VENDOR_MXIC)&&
                (pNandDrv->au8_ID[2] == 0x90) &&
                (pNandDrv->au8_ID[3] == 0x95) &&
                (pNandDrv->au8_ID[4] == 0x57))
        {
            pNandDrv->u16_BlkCnt = 4096;
            pNandDrv->u16_BlkPageCnt = 64;
            pNandDrv->u16_PageByteCnt = 2048;
            pNandDrv->u16_SectorByteCnt = 512;
            pNandDrv->u16_SpareByteCnt = 112;
            pNandDrv->u16_ECCType = ECC_TYPE_8BIT;
        }
        else if((pNandDrv->au8_ID[0]==VENDOR_MICRON)&&
                (pNandDrv->au8_ID[2] == 0x90) &&
                (pNandDrv->au8_ID[3] == 0xA6) &&
                (pNandDrv->au8_ID[4] == 0x54))
        {
            pNandDrv->u16_BlkCnt = 2048;
            pNandDrv->u16_BlkPageCnt = 64;
            pNandDrv->u16_PageByteCnt = 4096;
            pNandDrv->u16_SectorByteCnt = 512;
            pNandDrv->u16_SpareByteCnt = 224;
            pNandDrv->u16_ECCType = ECC_TYPE_8BIT;
        }
        //TC58NVG2S0FTA00
        else if((pNandDrv->au8_ID[0]==VENDOR_TOSHIBA)&&
                (pNandDrv->au8_ID[2] == 0x90) &&
                (pNandDrv->au8_ID[3] == 0x26) &&
                (pNandDrv->au8_ID[4] == 0x76) &&
                (pNandDrv->au8_ID[5] == 0x15))
        {
            pNandDrv->u16_BlkCnt = 2048;
            pNandDrv->u16_BlkPageCnt = 64;
            pNandDrv->u16_PageByteCnt = 4096;
            pNandDrv->u16_SectorByteCnt = 512;
            pNandDrv->u16_SpareByteCnt = 224;
            pNandDrv->u16_ECCType = ECC_TYPE_8BIT;
        }
        //H27U4G8F2ETR, H27U4G8F2FTR
        else if((pNandDrv->au8_ID[0]==VENDOR_HYNIX)&&
                (pNandDrv->au8_ID[2] == 0x90) &&
                (pNandDrv->au8_ID[3] == 0x95) &&
                (pNandDrv->au8_ID[4] == 0x56))
        {
            pNandDrv->u16_BlkCnt = 4096;
            pNandDrv->u16_BlkPageCnt = 64;
            pNandDrv->u16_PageByteCnt = 2048;
            pNandDrv->u16_SectorByteCnt = 512;
            pNandDrv->u16_SpareByteCnt = 128;
            pNandDrv->u16_tWHR = 200;
        }
        //HY27UG088G5B
        else if((pNandDrv->au8_ID[0]==VENDOR_HYNIX)&&
                (pNandDrv->au8_ID[2] == 0x10) &&
                (pNandDrv->au8_ID[3] == 0x95) &&
                (pNandDrv->au8_ID[4] == 0x54))
        {
            pNandDrv->u16_BlkCnt = 8192;
            pNandDrv->u16_BlkPageCnt = 64;
            pNandDrv->u16_PageByteCnt = 2048;
            pNandDrv->u16_SectorByteCnt = 512;
            pNandDrv->u16_SpareByteCnt = 64;
            pNandDrv->u16_ECCType = ECC_TYPE_4BIT;
            pNandDrv->u8_MaxCENum = 1;
        }
        else
        {
            pNandDrv->u16_PageByteCnt = 1024 << (pNandDrv->au8_ID[3] & 3);
            pNandDrv->u16_BlkPageCnt  = ((64 * 1024)  << ((pNandDrv->au8_ID[3] >> 4) & 3)) /  pNandDrv->u16_PageByteCnt;
            pNandDrv->u16_SectorByteCnt = 512;
            pNandDrv->u16_SpareByteCnt = (8 << (( pNandDrv->au8_ID[3] >> 2)& 0x01)) * ( pNandDrv->u16_PageByteCnt>>9);
            pNandDrv->u16_BlkCnt = 4096 / (pNandDrv->u16_PageByteCnt/2048);
        }
      pNandDrv->u8_OpCode_RW_AdrCycle = ADR_C5TFS0;
      pNandDrv->u8_OpCode_Erase_AdrCycle = ADR_C3TRS0;
      break;
    case 15:
      //spansion 2Gbit
      pNandDrv->u16_BlkCnt = 2048;
      pNandDrv->u16_BlkPageCnt = 64;
      pNandDrv->u16_PageByteCnt = 2048;
      pNandDrv->u16_SectorByteCnt = 512;
      pNandDrv->u16_SpareByteCnt = 64;
      pNandDrv->u8_OpCode_RW_AdrCycle = ADR_C5TFS0;
      pNandDrv->u8_OpCode_Erase_AdrCycle = ADR_C3TRS0;
      if(((pNandDrv->au8_ID[0]==VENDOR_SPANSION)||(pNandDrv->au8_ID[0]==VENDOR_HYNIX))&&
          (pNandDrv->au8_ID[2] == 0x90) &&
          (pNandDrv->au8_ID[3] == 0x95) &&
          (pNandDrv->au8_ID[4] == 0x46))

      {
          pNandDrv->u16_SpareByteCnt = 128;
      }
      //Toshiba 2G SLC 24nm
      else if(pNandDrv->au8_ID[0]==VENDOR_TOSHIBA &&
              pNandDrv->au8_ID[2] == 0x90 &&
              pNandDrv->au8_ID[3] == 0x15 &&
              pNandDrv->au8_ID[4] == 0x76 &&
              pNandDrv->au8_ID[5] == 0x16)
      {
          pNandDrv->u16_SpareByteCnt = 128;
          pNandDrv->u16_ECCType = ECC_TYPE_8BIT;
      }
      //MXIC MX30LFxG28AB 2G SLC 2Gbit
      else if(pNandDrv->au8_ID[0]==VENDOR_MXIC &&
              pNandDrv->au8_ID[2] == 0x90 &&
              pNandDrv->au8_ID[3] == 0x95 &&
              pNandDrv->au8_ID[4] == 0x07)
      {
          pNandDrv->u16_SpareByteCnt = 112;
          pNandDrv->u16_ECCType = ECC_TYPE_12BIT;
      }
      //MICRON MT29F2G08ABAFA 2G SLC 2Gbit
      else if(pNandDrv->au8_ID[0]==VENDOR_MICRON &&
              pNandDrv->au8_ID[2] == 0x90 &&
              pNandDrv->au8_ID[3] == 0x95 &&
              pNandDrv->au8_ID[4] == 0x04)
      {
          pNandDrv->u16_SpareByteCnt = 224;
          pNandDrv->u16_ECCType = ECC_TYPE_8BIT;
      }
	  //WINBOND W29N02GVSIAA SLC 2Gbit
	  else if(pNandDrv->au8_ID[0]==VENDOR_WINBOND &&
              pNandDrv->au8_ID[2] == 0x90 &&
              pNandDrv->au8_ID[3] == 0x95 &&
              pNandDrv->au8_ID[4] == 0x04)
	  {
			pNandDrv->u16_ECCType = ECC_TYPE_8BIT; //WINBOND recommend
	  }
      else
      {
          pNandDrv->u16_SpareByteCnt = 64;
          pNandDrv->u16_ECCType = ECC_TYPE_4BIT;
      }
    break;
    case 16:

      if((pNandDrv->au8_ID[0]==VENDOR_SAMSUNG))
       {
         pNandDrv->u16_BlkCnt = 4096;
         pNandDrv->u16_BlkPageCnt = 64;
         pNandDrv->u16_PageByteCnt = 4096;
         pNandDrv->u16_SectorByteCnt = 512;
         pNandDrv->u16_SpareByteCnt = 128;
       }
      else if((pNandDrv->au8_ID[0]==VENDOR_HYNIX))
       {
         pNandDrv->u16_BlkCnt = 4096;
         pNandDrv->u16_BlkPageCnt = 128;
         pNandDrv->u16_PageByteCnt = 2048;
         pNandDrv->u16_SectorByteCnt = 512;
         pNandDrv->u16_SpareByteCnt = 64;
       }
      else if((pNandDrv->au8_ID[0]==VENDOR_MICRON))
      {
        pNandDrv->u16_BlkCnt = 2048;
        pNandDrv->u16_BlkPageCnt = 128;
        pNandDrv->u16_PageByteCnt = 4096;
        pNandDrv->u16_SectorByteCnt = 512;
        pNandDrv->u16_SpareByteCnt = 128;
      }
      else if((pNandDrv->au8_ID[0]==VENDOR_TOSHIBA))
      {
        //TH58NVG3S0HTA00
        if(pNandDrv->au8_ID[2] == 0x91 &&
           pNandDrv->au8_ID[3] == 0x26 &&
           pNandDrv->au8_ID[4] == 0x76)
        {
            pNandDrv->u16_BlkCnt = 4096;
            pNandDrv->u16_BlkPageCnt = 64;
            pNandDrv->u16_PageByteCnt = 4096;
            pNandDrv->u16_SectorByteCnt = 512;
            pNandDrv->u16_SpareByteCnt = 256;
            pNandDrv->u16_ECCType = ECC_TYPE_8BIT;
        }
        //TH58NVG4S0FTA20
        if(pNandDrv->au8_ID[2] == 0x90 &&
           pNandDrv->au8_ID[3] == 0x26 &&
           pNandDrv->au8_ID[4] == 0x76 &&
           pNandDrv->au8_ID[5] == 0x15)
        {
            pNandDrv->u16_BlkCnt = 8192;
            pNandDrv->u16_BlkPageCnt = 64;
            pNandDrv->u16_PageByteCnt = 4096;
            pNandDrv->u16_SectorByteCnt = 512;
            pNandDrv->u16_SpareByteCnt = 224;
            pNandDrv->u16_PhySpareByteCnt = 232;
            pNandDrv->u16_ECCType = ECC_TYPE_4BIT;
            pNandDrv->u8_MaxCENum = 1;
        }
        else
        {
            pNandDrv->u16_BlkCnt = 4096;
            pNandDrv->u16_BlkPageCnt = 64;
            pNandDrv->u16_PageByteCnt = 4096;
            pNandDrv->u16_SectorByteCnt = 512;
            pNandDrv->u16_SpareByteCnt = 128;
        }
      }
      else if(pNandDrv->au8_ID[0] != VENDOR_ST)
      {
        pNandDrv->u16_BlkCnt = 2048;
        pNandDrv->u16_BlkPageCnt = 64;
        pNandDrv->u16_PageByteCnt = 2048;
        pNandDrv->u16_SectorByteCnt = 512;
        pNandDrv->u16_SpareByteCnt = 64;
      }
      else
      {
        pNandDrv->u16_BlkCnt = 8192;
        pNandDrv->u16_BlkPageCnt = 64;
        pNandDrv->u16_PageByteCnt = 2048;
        pNandDrv->u16_SectorByteCnt = 512;
        pNandDrv->u16_SpareByteCnt = 64;
      }
      pNandDrv->u8_OpCode_RW_AdrCycle = ADR_C5TFS0;
      pNandDrv->u8_OpCode_Erase_AdrCycle = ADR_C3TRS0;
      break;
    case 17:
      if ((pNandDrv->au8_ID[2] & 0xC) == 0)  // for SLC
      {
          if(pNandDrv->au8_ID[0] != VENDOR_SAMSUNG)
          {
              pNandDrv->u16_BlkCnt = 8192;
              pNandDrv->u16_BlkPageCnt = 128;
              pNandDrv->u16_PageByteCnt = 2048;
              pNandDrv->u16_SectorByteCnt = 512;
              pNandDrv->u16_SpareByteCnt = 64;
          }
          else
          {
              pNandDrv->u16_BlkCnt = 16384;
              pNandDrv->u16_BlkPageCnt = 64;
              pNandDrv->u16_PageByteCnt = 2048;
              pNandDrv->u16_SectorByteCnt = 512;
              pNandDrv->u16_SpareByteCnt = 64;
          }
      }
      else  // for MLC
      {
          U8 u8PageSize, u8Value;

          pNandDrv->u16_PageByteCnt = 2048 << (pNandDrv->au8_ID[3] & 0x3);
          u8PageSize = (2 << (pNandDrv->au8_ID[3] & 0x3));
          u8Value = ((pNandDrv->au8_ID[3] >> 4) & 0x3) | ((pNandDrv->au8_ID[3] >> 5) & 0x4);

          if (pNandDrv->au8_ID[0] == VENDOR_SAMSUNG)
          {
              pNandDrv->u16_BlkCnt = 2076;
              pNandDrv->u16_BlkPageCnt = (128 << u8Value) / u8PageSize;
          }
          else if (pNandDrv->au8_ID[0] == VENDOR_HYNIX)
          {
              pNandDrv->u16_BlkCnt = 1024;

              if (u8Value == 0x0)
              {
                  pNandDrv->u16_BlkPageCnt = 128 / u8PageSize;
              }
              else if (u8Value == 0x1)
              {
                  pNandDrv->u16_BlkPageCnt = 256 / u8PageSize;
              }
              else if (u8Value == 0x2)
              {
                  pNandDrv->u16_BlkPageCnt = 512 / u8PageSize;
              }
              else if (u8Value == 0x3) // 768 is not power of 2, should fix according to specific chip
              {
                  pNandDrv->u16_BlkPageCnt = 768 / u8PageSize;
              }
              else if (u8Value == 0x4)
              {
                  pNandDrv->u16_BlkPageCnt = 1024 / u8PageSize;
              }
              else if (u8Value == 0x5)
              {
                  pNandDrv->u16_BlkPageCnt = 2048 / u8PageSize;
              }
           }

           pNandDrv->u16_SectorByteCnt = 1024;
           pNandDrv->u16_SpareByteCnt = 432;
           pNandDrv->u16_ECCType = ECC_TYPE_24BIT1KB;
      }
      pNandDrv->u8_OpCode_RW_AdrCycle = ADR_C5TFS0;
      pNandDrv->u8_OpCode_Erase_AdrCycle = ADR_C3TRS0;
      break;
    case 18:
      pNandDrv->u16_BlkCnt = 16384;
      pNandDrv->u16_BlkPageCnt = 32;
      pNandDrv->u16_PageByteCnt = 512;
      pNandDrv->u16_SectorByteCnt = 512;
      pNandDrv->u16_SpareByteCnt = 16;
      pNandDrv->u8_OpCode_RW_AdrCycle = ADR_C4TFS0;
      pNandDrv->u8_OpCode_Erase_AdrCycle = ADR_C3TRS0;
      break;
    case 19:
      //TOSHIBA TC58NVG5H2HTA00
      if(pNandDrv->au8_ID[0]==VENDOR_TOSHIBA&&
           pNandDrv->au8_ID[2] == 0xA0 &&
           pNandDrv->au8_ID[3] == 0x26 &&
           pNandDrv->au8_ID[4] == 0x76 &&
           pNandDrv->au8_ID[5] == 0x56)
      {
           pNandDrv->u16_BlkCnt = 4096;
           pNandDrv->u16_BlkPageCnt = 128;
           pNandDrv->u16_PageByteCnt = 8192;
           pNandDrv->u16_SectorByteCnt = 1024;
           pNandDrv->u16_SpareByteCnt = 1024;
           pNandDrv->u8_OpCode_RW_AdrCycle = ADR_C5TFS0;
           pNandDrv->u8_OpCode_Erase_AdrCycle = ADR_C3TRS0;
           pNandDrv->u16_ECCType = ECC_TYPE_24BIT1KB;
      }
      else
      {
           pNandDrv->u16_BlkCnt = 4096;
           pNandDrv->u16_BlkPageCnt = 128;
           pNandDrv->u16_PageByteCnt = 8192;
           pNandDrv->u16_SpareByteCnt = 1024;
           pNandDrv->u8_OpCode_RW_AdrCycle = ADR_C5TFS0;
           pNandDrv->u8_OpCode_Erase_AdrCycle = ADR_C3TRS0;
      }
      break;
    case 20:
      pNandDrv->u16_BlkCnt = 1024;
      pNandDrv->u16_BlkPageCnt = 64;
      pNandDrv->u16_PageByteCnt = 2048;
      pNandDrv->u16_SectorByteCnt = 512;
      pNandDrv->u16_SpareByteCnt = 64;
      pNandDrv->u8_OpCode_RW_AdrCycle = ADR_C4TFS0;
      pNandDrv->u8_OpCode_Erase_AdrCycle = ADR_C2TRS0;
      break;
    default:
      pNandDrv->u16_BlkCnt = 0;
      pNandDrv->u16_BlkPageCnt = 0;
      pNandDrv->u16_PageByteCnt = 0;
      pNandDrv->u16_SectorByteCnt = 0;
      pNandDrv->u16_SpareByteCnt = 0;
      return -1;
      break;
  }

  if(pNandDrv->u16_PhySpareByteCnt == 0)
    pNandDrv->u16_PhySpareByteCnt = pNandDrv->u16_SpareByteCnt;

  if(pNandDrv->u8_CellType == NAND_CellType_SLC)
  {
    for(u16_i=0;u16_i<pNandDrv->u16_BlkPageCnt;u16_i++)
    {
      ga_tPairedPageMap[u16_i].u16_LSB = u16_i;
      ga_tPairedPageMap[u16_i].u16_MSB = u16_i;
    }
  }

  return 0;
}

//for MBOOT
void drvNAND_BIST_Test(void)
{
  int i, j;
  U32 u32TotalPages;
  U16 u16PageSize;
  U16 u16SpareSize;

  U8 u8TempBistPattern[8192+512] __attribute((aligned(128)));
  U8 u8TempResult[8192+512] __attribute((aligned(128)));


  U8 *u8BistPattern;
  U8 *u8Result;

  NAND_DRIVER *pNandDrv = (NAND_DRIVER*)drvNAND_get_DrvContext_address();


  u8BistPattern=u8TempBistPattern;
  u8Result=u8TempResult;


  printf("Start NAND BIST Testing...\n\n");

  u32TotalPages = (pNandDrv->u16_BlkCnt-5) * pNandDrv->u16_BlkPageCnt;;

  u16PageSize = pNandDrv->u16_PageByteCnt;

  u16SpareSize = u16PageSize/512;

  // prepare bist pattern for main area and spare area
  for(i=0; i<u16PageSize+512; i++)
          u8BistPattern[i]=0x5A;



  i=-1;
  while(1)
  {
    i++;
    if( i>=u32TotalPages) break;

    //erase
    if( (i % pNandDrv->u16_BlkPageCnt) == 0 )
    {
      printf("\033[31mErasing block\033[m 0x%X\n", i / pNandDrv->u16_BlkPageCnt);

      if(NC_EraseBlk(i) != UNFD_ST_SUCCESS)
      {
        printf("Fail to erase block %X. Skip this block.\n", i);
        i += pNandDrv->u16_BlkPageCnt;
      }
    }


    //write and read test
    printf("\033[35mWriting page\033[m 0x%X\n", i);

    NC_WritePages( i, u8BistPattern, u8BistPattern + u16PageSize, 1);



    printf("\033[34mReading page\033[m 0x%X\n", i);

    NC_ReadPages( i, u8Result, u8Result + u16PageSize, 1);


    // compare
    for(j=0; j<u16PageSize; j++)
    {
      if( u8Result[j] != 0x5A )
      {
        printf("ERROR: Bist testing fails: Row 0x%X, Col 0x%X\n", i, j);
        printf("pattern = 0x5A, result = 0x%02X\n", u8Result[j]);
       }
    }

    for(j=0; j<u16SpareSize; j++)
    {
      if( (j % 16) < 9 ) // skip ECC
      {
        if( u8Result[ u16PageSize+j ] != 0xFF )
        {
          printf("ERROR: Bist testing fails: Row 0x%X, Col 0x%X\n", i, u16PageSize+j);
          printf("pattern = 0x5A, result = 0x%02X\n", u8Result[ u16PageSize+j ] );

        }
      }
    }

  }

  printf("NAND BIST testing is ok!\n");

}


//for MBOOT

void drvNAND_Stable_Test(void)
{ int i,j;

  int READ_TRY_COUNT =1000;


  int blk;
  int count;

  U16 mainlen;
  U16 sparelen;

  U32 eStatus;


  unsigned char TempRMainBuf[8192+512] __attribute((aligned(128)));
  unsigned char TempWMainBuf[8192+512] __attribute((aligned(128)));

  unsigned char *RMainBuf;
  unsigned char *WMainBuf;
  unsigned char u8SpareBuf[512] __attribute((aligned(128)));

  NAND_DRIVER *pNandDrv = (NAND_DRIVER*)drvNAND_get_DrvContext_address();



  RMainBuf=TempRMainBuf;
  WMainBuf=TempWMainBuf;


  mainlen = pNandDrv->u16_PageByteCnt;
  sparelen = (mainlen>>5);



  // Init the main data
  for(i=0; i<mainlen; i++)
  {
    if( (i%2) == 0x1 ) WMainBuf[i] = 0x00;
    else WMainBuf[i] = 0xFF;
  }



  printf("Prepare test data... Please wait...\n");

  // Scan all blocks and write test data
  for(blk=0; blk<pNandDrv->u16_BlkCnt; blk++)
  {
    // Read 1st page to check good block or not

    for(j=0; j<512; j++) u8SpareBuf[j]=0;



    eStatus = NC_Read_RandomIn( blk*pNandDrv->u16_BlkPageCnt, mainlen, u8SpareBuf, sparelen);

    if( (eStatus != UNFD_ST_SUCCESS) || (u8SpareBuf[0] != 0xFF) )
    {
      // Read 2nd page to check good block or not
      for(j=0; j<512; j++) u8SpareBuf[j]=0;
      eStatus = NC_Read_RandomIn( (blk*pNandDrv->u16_BlkPageCnt)+1, mainlen, u8SpareBuf, sparelen);

      // Bad block is found
      if( (eStatus != UNFD_ST_SUCCESS) || (u8SpareBuf[0] != 0xFF) )
      {
        printf("Block \033[31m%X\033[m is bad block!\n", blk);
        continue;
      }
    }

    printf(".");



    // Erase block
    NC_EraseBlk(blk*pNandDrv->u16_BlkPageCnt);

    // Write test data to block
    for(i=0; i<pNandDrv->u16_BlkPageCnt; i++)
    {
      for(j=0; j<512; j++) u8SpareBuf[j]=0xFF;
          NC_WritePages( (blk*pNandDrv->u16_BlkPageCnt)+i, WMainBuf, u8SpareBuf, 1);

    }

  }




  printf("\nStart read test\n");

  for(count=0; count<READ_TRY_COUNT; count++)
  {

    blk=-1;

    while(1)
    {
      blk++;
      if( blk>=pNandDrv->u16_BlkCnt) break;

      // Read 1st page to check good block or not
      for(j=0; j<512; j++) u8SpareBuf[j]=0;

      eStatus = NC_Read_RandomIn( blk*pNandDrv->u16_BlkPageCnt, mainlen, u8SpareBuf, sparelen);

      if( (eStatus != UNFD_ST_SUCCESS) || (u8SpareBuf[0] != 0xFF) )
      {
        // Read 2nd page to check good block or not
        for(j=0; j<512; j++) u8SpareBuf[j]=0;
          eStatus = NC_Read_RandomIn( (blk*pNandDrv->u16_BlkPageCnt)+1, mainlen, u8SpareBuf, sparelen);


        // Bad block is found
        if( (eStatus != UNFD_ST_SUCCESS) || (u8SpareBuf[0] != 0xFF) )
        {
          printf("Block \033[31m%X\033[m is bad block! Skip read test of this block\n", blk);
          blk++;
          continue;
        }

      }

      printf(".");

      // Read data from block
      for(i=0; i<pNandDrv->u16_BlkPageCnt; i++)
      {
        NC_ReadPages( (blk*pNandDrv->u16_BlkPageCnt)+i, RMainBuf, u8SpareBuf, 1);
      }


    }
  }


  printf("\nRead test is ok\n");

}

#if defined(ENABLE_READ_DISTURBANCE_SUPPORT) && ENABLE_READ_DISTURBANCE_SUPPORT
U32 nand_ReadDisturbance(void)
{
    NAND_DRIVER *pNandDrv = (NAND_DRIVER*)drvNAND_get_DrvContext_address();
    U32 u32_Err;
    U16 u16_Reg;
    U16 u16_PBA_Bitflip=0;
    U16 u16_i, u16_bitflip;
    U32 u16_PageCnt;
	U8 *pu8_PageDataBuf = pNandDrv->pu8_PageDataBuf;
	U8 *pu8_PageSpareBuf = pNandDrv->pu8_PageSpareBuf;
    U8 u8_RequireRandomizer = 0;

    #if defined(FCIE_LFSR) && FCIE_LFSR
	if(pNandDrv->u8_RequireRandomizer == 1)
    {
        u8_RequireRandomizer = pNandDrv->u8_RequireRandomizer;
        pNandDrv->u8_RequireRandomizer = 0;
        NC_DisableLFSR();
    }
    #endif

    #if defined(DEBUG_REG_ECC_STATUS)
    REG_READ_UINT16(DEBUG_REG_ECC_STATUS, u16_Reg);
    u16_bitflip = ((u16_Reg>>8)&0x00FF); //bit flip cnt
    u16_PBA_Bitflip = u16_Reg&0x00FF; //bit flip blk
    if(u16_PBA_Bitflip>0 && u16_bitflip>=pNandDrv->u16_BitflipThreshold)
    {
        printf("Read Disturbance for Blk %Xh, %d bitflip\n", u16_PBA_Bitflip, u16_bitflip);
        if(u16_PBA_Bitflip == pNandDrv->u8_BL0PBA)
            u16_i = 0x00;
        else if(u16_PBA_Bitflip == pNandDrv->u8_HashPBA[0][0])
            u16_i = 0x80;
        else if(u16_PBA_Bitflip == pNandDrv->u8_HashPBA[1][0])
            u16_i = 0x81;
        else
        {
            if(pNandDrv->u8_BL0PBA)
                u16_i = 0xFF;
            else
                u16_i = 0x82;
        }

        nand_ReadDisturbance_BigImg((u16_PBA_Bitflip << pNandDrv->u8_BlkPageCntBits), 0, NULL, u16_i, 1);
        REG_WRITE_UINT16(DEBUG_REG_ECC_STATUS, 0x00);
    }
    #endif

    if(pNandDrv->u8_BL0PBA)
    {
        if(pNandDrv->u8_BL0PBA != u16_PBA_Bitflip)
        {
            u16_PageCnt = (BL_SIZE + pNandDrv->u16_PageByteCnt - 1) >> pNandDrv->u8_PageByteCntBits;

            for(u16_i = 0 ; u16_i < u16_PageCnt ; u16_i++)
            {
                u32_Err = NC_ReadPages((pNandDrv->u8_BL0PBA << pNandDrv->u8_BlkPageCntBits) + u16_i,
                                        pu8_PageDataBuf,
                                        pu8_PageSpareBuf,
                                        1);
                if(u32_Err != UNFD_ST_SUCCESS || pu8_PageSpareBuf[0] != 0xFF)
                    break;

                REG_READ_UINT16(NC_ECC_STAT0, u16_Reg);
                u16_Reg = ((u16_Reg & BIT_NC_ECC_MAX_BITS_MASK) >> 1);
                if (u16_Reg >= pNandDrv->u16_BitflipThreshold )
                {
                    printf("Read Disturbance for BL0 Blk %Xh\n", pNandDrv->u8_BL0PBA);
                    nand_ReadDisturbance_BigImg((pNandDrv->u8_BL0PBA << pNandDrv->u8_BlkPageCntBits), 0, NULL, 0x00, 1);
                    break;
                }
            }
        }
    }
    else if(pNandDrv->u8_HashPBA[0][0])
    {
        if(pNandDrv->u8_HashPBA[0][0] != u16_PBA_Bitflip)
        {
            u16_PageCnt = (HASH0_SIZE + pNandDrv->u16_PageByteCnt - 1) >> pNandDrv->u8_PageByteCntBits;

            for(u16_i = 0 ; u16_i < u16_PageCnt ; u16_i++)
            {
                u32_Err = NC_ReadPages((pNandDrv->u8_HashPBA[0][0] << pNandDrv->u8_BlkPageCntBits) + u16_i,
                                        pu8_PageDataBuf,
                                        pu8_PageSpareBuf,
                                        1);
                if(u32_Err != UNFD_ST_SUCCESS || pu8_PageSpareBuf[0] != 0xFF)
                    break;

                REG_READ_UINT16(NC_ECC_STAT0, u16_Reg);
                u16_Reg = ((u16_Reg & BIT_NC_ECC_MAX_BITS_MASK) >> 1);
                if (u16_Reg >= pNandDrv->u16_BitflipThreshold )
                {
                    printf("Read Disturbance for HASH0 Blk %Xh\n", pNandDrv->u8_HashPBA[0][0]);
                    nand_ReadDisturbance_BigImg((pNandDrv->u8_HashPBA[0][0] << pNandDrv->u8_BlkPageCntBits), 0, NULL, 0x80, 1);
                    break;
                }
            }
        }

        if(pNandDrv->u8_HashPBA[1][0] != u16_PBA_Bitflip)
        {
            u16_PageCnt = (HASH1_SIZE + pNandDrv->u16_PageByteCnt - 1) >> pNandDrv->u8_PageByteCntBits;

            for(u16_i = 0 ; u16_i < u16_PageCnt ; u16_i++)
            {
                u32_Err = NC_ReadPages((pNandDrv->u8_HashPBA[1][0] << pNandDrv->u8_BlkPageCntBits) + u16_i,
                                        pu8_PageDataBuf,
                                        pu8_PageSpareBuf,
                                        1);
                if(u32_Err != UNFD_ST_SUCCESS || pu8_PageSpareBuf[0] != 0xFF)
                    break;

                REG_READ_UINT16(NC_ECC_STAT0, u16_Reg);
                u16_Reg = ((u16_Reg & BIT_NC_ECC_MAX_BITS_MASK) >> 1);
                if (u16_Reg >= pNandDrv->u16_BitflipThreshold )
                {
                    printf("Read Disturbance for HASH1 Blk %Xh\n", pNandDrv->u8_HashPBA[1][0]);
                    nand_ReadDisturbance_BigImg((pNandDrv->u8_HashPBA[1][0] << pNandDrv->u8_BlkPageCntBits), 0, NULL, 0x81, 1);
                    break;
                }
            }
        }
    }

    #if defined(FCIE_LFSR) && FCIE_LFSR
    if(u8_RequireRandomizer == 1)
    {
        pNandDrv->u8_RequireRandomizer = 1;
    }
    #endif

    return UNFD_ST_SUCCESS;
}
#endif

#if defined (FCIE_WRITE_PROTECT_SUPPORT) && FCIE_WRITE_PROTECT_SUPPORT
void nand_set_WP_RangeNStart(U32 u32_RowStart, U32 u32_RowEnd)
{
    nand_debug(0, 1, "Start 0x%X, End 0x%X\n", u32_RowStart, u32_RowEnd);
    REG_WRITE_UINT16(REG_FCIE_WP_START_ROW_ADDR_L, u32_RowStart & 0xFFFF);
    REG_WRITE_UINT16(REG_FCIE_WP_START_ROW_ADDR_H, (u32_RowStart >> 16) & 0xFFFF);
    REG_WRITE_UINT16(REG_FCIE_WP_END_ROW_ADDR_L, u32_RowEnd & 0xFFFF);
    REG_WRITE_UINT16(REG_FCIE_WP_END_ROW_ADDR_H, (u32_RowEnd >> 16) & 0xFFFF);
    REG_SET_BITS_UINT16(REG_FCIE_WP_ENABLE, BIT_FCIE_WP_ENABLE);
}
#endif

U32 drvNAND_OTP_Entry(void)
{
	NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();

	U32 u32_Err = UNFD_ST_ERR_NOT_READY;

#if  defined(NAND_OTP_ENABLE) && NAND_OTP_ENABLE
	u32_Err = NC_NAND_OTP_Entry();
	return	u32_Err;
#else
	return	u32_Err;
#endif //NAND_OTP_ENABLE

}


U32 drvNAND_OTP_Exit(void)
{
	NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();

	U32 u32_Err = UNFD_ST_ERR_NOT_READY;

#if  defined(NAND_OTP_ENABLE) && NAND_OTP_ENABLE

	u32_Err = NC_NAND_OTP_Exit();

	return	u32_Err;
#else
	return u32_Err;
#endif //NAND_OTP_ENABLE

}


U32 drvNAND_OTP_ReadPages(U32 u32_PhyRowIdx, U8 *pu8_DataBuf, U32 u32_PageCnt)
{
	NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();

	U32 u32_Err = UNFD_ST_ERR_NOT_READY;

	U8 *au8_SpareBuf = (U8*)pNandDrv->pu8_PageSpareBuf;

	U8 u8_OtpMaxPageCnt = 0;

#if  defined(NAND_OTP_ENABLE) && NAND_OTP_ENABLE
	if (pNandDrv->au8_ID[0] == 0x01) //Spansion
	{
		u8_OtpMaxPageCnt = pNandDrv->u16_BlkPageCnt;
	}
	else if ((0xC2 == pNandDrv->au8_ID[0]) || (0xEF == pNandDrv->au8_ID[0])) //MXIC , WINBOND
	{
		u8_OtpMaxPageCnt = 32;
		u32_PhyRowIdx += 2;
	}
	
	memset(au8_SpareBuf, 0xFF, pNandDrv->u16_SpareByteCnt);

	if ((u32_PhyRowIdx + u32_PageCnt) <= u8_OtpMaxPageCnt)
	{
		u32_Err = NC_ReadPages(u32_PhyRowIdx, pu8_DataBuf, au8_SpareBuf, u32_PageCnt);

		//dump_mem(pu8_DataBuf,2048); 
	}
	else
	{
		nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "ERROR, drvNAND_OTP_ReadPages: invalid parameters:%x \n", u32_PhyRowIdx);
		return UNFD_ST_ERR_INVALID_PARAM;
	}

	return u32_Err;
#else
	return u32_Err;
#endif //NAND_OTP_ENABLE

}


U32 drvNAND_OTP_WritePages(U32 u32_PhyRowIdx, U8 *pu8_DataBuf, U32 u32_PageCnt)
{
	NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();

	U32 u32_Err = UNFD_ST_ERR_NOT_READY;

	U16 u16_DataIdx = 0, u16_OtpMaxPageCnt = 0, u16_PagaIdx = 0, u16_OtpMaxPageIdx;

	U8 *au8_SpareBuf = (U8*)pNandDrv->pu8_PageSpareBuf;

	U8 *au8_PageDataBuf = (U8*)pNandDrv->pu8_PageDataBuf;

	
#if  defined(NAND_OTP_ENABLE) && NAND_OTP_ENABLE
	if (0x01 == pNandDrv->au8_ID[0]) //Spansion
	{
		u16_OtpMaxPageCnt = pNandDrv->u16_BlkPageCnt;
		u16_OtpMaxPageIdx = pNandDrv->u16_BlkPageCnt;
	}
	else if ((0xC2 == pNandDrv->au8_ID[0]) || (0xEF == pNandDrv->au8_ID[0])) //MXIC , WINBOND
	{
		u16_OtpMaxPageCnt = 32;
		u32_PhyRowIdx += 2;
		u16_OtpMaxPageIdx = 30;
	}
#if 1
	//Scan OTP area before programming
	for (u16_PagaIdx = 0;  u16_PagaIdx < u16_OtpMaxPageIdx; u16_PagaIdx ++)
	{		
		memset(au8_PageDataBuf, 0x0, pNandDrv->u16_PageByteCnt);
	
		drvNAND_OTP_ReadPages(u16_PagaIdx, au8_PageDataBuf, 1);

		for (u16_DataIdx = 0; u16_DataIdx < pNandDrv->u16_PageByteCnt; u16_DataIdx++)
		{
			if (0xff != au8_PageDataBuf[u16_DataIdx])
			{
				nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "ERROR, drvNAND_OTP_WritePages: OTP area [%x] programmed: \n", u16_PagaIdx);
				return UNFD_ST_ERR_W_PROTECTED;
			}
		}
	}
#endif			
	memset(au8_SpareBuf, 0xFF, pNandDrv->u16_SpareByteCnt);

	if ((u32_PhyRowIdx + u32_PageCnt) <= u16_OtpMaxPageCnt)
	{
		u32_Err = NC_WritePages(u32_PhyRowIdx, pu8_DataBuf, au8_SpareBuf, u32_PageCnt);
	}
	else
	{
		nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "ERROR, drvNAND_OTP_WritePages: invalid parameters:%x \n", u32_PhyRowIdx);
		return UNFD_ST_ERR_INVALID_PARAM;
	}

	return u32_Err;
#else
	return u32_Err;
#endif //NAND_OTP_ENABLE

}


U32 drvNAND_OTP_Lock(void)
{
	NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();
	U32 i = 0;
	U32 u32_OtpStatus = UNFD_ST_ERR_NOT_READY;
	U8 u8_PageIdx = 0x2;

#if  defined(NAND_OTP_ENABLE) && NAND_OTP_ENABLE

	if (0xEF == pNandDrv->au8_ID[0]) //WINBOND
	{
		for (u8_PageIdx = 0x2; u8_PageIdx<=0x1F; u8_PageIdx++)
		{
			while(1)
			{	
				i++;
				
				NC_NAND_OTP_Lock();

				u32_OtpStatus = NC_NAND_OTP_Status(u8_PageIdx);

				if ((UNFD_ST_ERR_W_PROTECTED == u32_OtpStatus) | (i >100))
				{
					break;
				}
			};
		}
	}
	else
	{
	while(1)
	{	
		i++;

		NC_NAND_OTP_Lock();
		
			u32_OtpStatus = NC_NAND_OTP_Status(0);

		if ((UNFD_ST_ERR_W_PROTECTED == u32_OtpStatus) | (i >100))
		{
			break;
		}
	};
	}

	return	u32_OtpStatus;
#else
	return u32_OtpStatus;
#endif //NAND_OTP_ENABLE

}

U32 drvNAND_OTP_Access_Size(void)
{
	NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();

	return pNandDrv->u16_PageByteCnt;
}


U32 drvNAND_OTP_Total_Size(void)
{
	NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();

	if (0x01 == pNandDrv->au8_ID[0]) //Spansion
	{
		return (pNandDrv->u16_PageByteCnt * pNandDrv->u16_BlkPageCnt);
	}
	else if (0xC2 == pNandDrv->au8_ID[0]) //MXIC
	{
		return (pNandDrv->u16_PageByteCnt * 30);
	}
	else if (0xEF == pNandDrv->au8_ID[0]) //WINBOND
	{
		return (pNandDrv->u16_PageByteCnt * 30);
	}
	else 
		return 0;
		
}

