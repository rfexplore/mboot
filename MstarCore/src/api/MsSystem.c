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

#include <command.h>
#include <common.h>
#include <drvWDT.h>
#include <MsMath.h>
#include <MsSystem.h>
#include <MsUtility.h>
#include <MsDebug.h>
#include <exports.h>
#include <drvGPIO.h>
#include <MsOS.h>
#include <MsRawIO.h>
#include <ShareType.h>
#include <drvUART.h>
#include <MsUboot.h>
#include <drvMBX.h>
#include <msAPI_Power.h>
#include <drvSYS.h>
#include <mhal_sys.h>
extern int snprintf(char *str, size_t size, const char *fmt, ...);
extern unsigned int GetRealOffset(unsigned int);

#define BRICK_TMP_PATCH 0

int do_wdt_enable (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
#define WDT_TIMEOUT_DEFAULT 60
#define WDT_TIMEOUT_MIN 5
#define WDT_TIMEOUT_ENV_STR "WDT_TUMEOUT"
#define WDT_ENABLE_ENV_STR "WDT_ENABLE"
    unsigned int u32Timer = WDT_TIMEOUT_DEFAULT;
    char *pEnv = NULL;

    if (argc < 1)
    {
        printf ("Usage:\n%s\n", cmdtp->usage);
        return 1;
    }
#if (1 != CONFIG_RESCUE_ENV)
    pEnv = getenv(WDT_ENABLE_ENV_STR);
    if(pEnv != NULL && !(strcmp(pEnv, "1")))
#endif
    {
        if (argc == 1)
        {
            pEnv = getenv(WDT_TIMEOUT_ENV_STR);
            if(pEnv != NULL)
            {
                u32Timer = (unsigned int)simple_strtol(pEnv, NULL, 10);
            }
            if(u32Timer < WDT_TIMEOUT_MIN) // NOTE: should not be too short to prevent infinite reboot
            {
                UBOOT_INFO("%s: Incorrect WDT TIMEOUT, should be not less than '%u', at %d\n", __func__, WDT_TIMEOUT_MIN, __LINE__);
                u32Timer = WDT_TIMEOUT_MIN;
            }
        }
        else
        {
            u32Timer = simple_strtoul(argv[1], NULL, 10);
        }
        MDrv_WDT_Init(E_WDT_DBGLV_ALL);
        MDrv_WDT_SetTimer(E_WDT_DBGLV_ALL, u32Timer);
    }
#if (1 != CONFIG_RESCUE_ENV)
    else
    {
        printf("WDT is not Enable !!!setenv WDT_ENABLE 1 to enable it.\n");
    }
#endif
    return 0;
}

int do_disable_uart ( cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    volatile u32 *ptr;

    printf("Disable UART\n");

    ptr = (u32 *)(MS_RIU_MAP + (0x0E12<<1));
    *ptr &= ~0x1800;
    //mdrv_uart_connect(E_UART_PORT0,E_UART_OFF);
    return 0;
}

static U32 gu32ChunkHeader[CH_ITEM_LAST+1];
static BOOLEAN ChunkHeaderReady = FALSE;
int MsApiChunkHeader_Init(void)
{
    U32 u32ChunkHeaderOffset = 0;
    int ret = -1;

    UBOOT_TRACE("IN\n");
    raw_io_config_push();
    ret = mboot_raw_io_Config();
    if(ret != 0)
    {
        UBOOT_ERROR("raw_io_config setting fail!\n");
        raw_io_config_pop();
        return ret;
    }

    if(get_raw_status()==E_RAW_DATA_IN_SPI)
    {
        if(IsHouseKeepingBootingMode()==FALSE){
             u32ChunkHeaderOffset = 0x30000;
        }
        else
        {
            #if(CONFIG_MSTAR_RT_PM_IN_SPI==1)
                u32ChunkHeaderOffset = 0x30000;
            #else
#if defined(CONFIG_PM_SIZE_KB_FORCED) && (0!=CONFIG_PM_SIZE_KB_FORCED)
                u32ChunkHeaderOffset = 0x10000+(CONFIG_PM_SIZE_KB_FORCED*0x400);
#else
#if defined (CONFIG_MSTAR_NS_UBOOT) && (CONFIG_TV_CHIP == 0)
                u32ChunkHeaderOffset = 0x40000;
#else
                u32ChunkHeaderOffset = 0x20000;
#endif //CONFIG_MSTAR_NS_UBOOT && CONFIG_TV_CHIP == 0
#endif
            #endif
            #if(CONFIG_DISABLE_PM_PARTITION)
                u32ChunkHeaderOffset -= 0x10000;
                UBOOT_DEBUG("The chunk header offset is 0x%x\n",u32ChunkHeaderOffset);
            #endif
        }
    }
    else
    {
         u32ChunkHeaderOffset = 0;
    }
    #if defined (CONFIG_DOUBLE_MBOOT) && !defined (CONFIG_DUAL_SYSTEM)
    if (mbootbak_raw_io_Config() == 1)
    {
        ret = raw_read((U32)0x45000000,0,0x300000);
        ret = raw_read((U32)gu32ChunkHeader,u32ChunkHeaderOffset,(CH_ITEM_LAST+1)*4);
        raw_io_config(E_RAW_DATA_IN_MMC, "MBOOT", UNNECESSARY_VOLUME);
        raw_write((U32)0x45000000,0,0x300000);
    }
    else
    #endif
    {
        ret = raw_read((U32)gu32ChunkHeader,u32ChunkHeaderOffset,(CH_ITEM_LAST+1)*4);
    }
    raw_io_config_pop();

    if(ret != 0)
    {
         UBOOT_ERROR("raw_read gu8ChunkHeader fail\n");
         return -1;
    }

    ChunkHeaderReady = TRUE;
    UBOOT_TRACE("OK\n");
    return 0;
}

int MsApiChunkHeader_GetValue(EN_CHUNK_HEADER_ITEM enItem,unsigned int *puValue)
{
    UBOOT_TRACE("IN\n");

    //UBOOT_DUMP(gu8ChunkHeader,CH_ITEM_LAST);
    if(puValue==NULL)
    {
        UBOOT_ERROR("puValue is a null pointer\n");
        return -1;
    }

    if(ChunkHeaderReady != TRUE)
    {
        if(MsApiChunkHeader_Init() !=0)
        {
            UBOOT_ERROR("MsApiChunkHeader_Init fail\n");
            return -1;
        }
    }


    if((enItem>=CH_ITEM_FIRST) && (enItem<=CH_ITEM_LAST))
    {
        *puValue = gu32ChunkHeader[enItem];
        UBOOT_DEBUG("ChunkHeaderOffset(0x%x): 0x%x\n",enItem,*puValue);
    }
    else
    {
        UBOOT_ERROR("ERROR: Out of chunk header bound : %d \n",enItem);
        return -1;
    }

    UBOOT_TRACE("OK\n");
    return 0;
}

#ifdef CONFIG_FAST_UART_RTSCTS
void SwitchUARTToPIUFASTUART(void)
{
    MS_U8 u8Index = 1;//default to UART1
    MS_U16 u16RegTemp = *(volatile U32*)(MS_RIU_MAP+(0x101EA6<<1));
#ifdef UART_INDEX_TO_PIU_FAST_UART
    u8Index = UART_INDEX_TO_PIU_FAST_UART;
#endif
    u16RegTemp &= ~(0x000F << (u8Index * 4));
    u16RegTemp |= 0x0007 << (u8Index * 4);
    *(volatile U32*)(MS_RIU_MAP+(0x101EA6<<1)) = u16RegTemp;

    //set piu_fast_uart clock to 123MHz
    u16RegTemp = *(volatile U32*)(MS_RIU_MAP+(0x100B2a<<1));
    u16RegTemp &= 0xFF00;
    u16RegTemp |= 0x000C;
    *(volatile U32*)(MS_RIU_MAP+(0x100B2a<<1)) = u16RegTemp;
}
#endif

extern U32 __heap;
extern U32 __heap_end;
void console_init(void)
{

    Mhal_console_init();
#ifdef CONFIG_FAST_UART_RTSCTS
    // Switch UARTx to PIU FAST UART
    SwitchUARTToPIUFASTUART();
#endif

}

void console_disable(void)
{
    *(volatile U32*)(MS_RIU_MAP+(0x101EA6<<1)) |= 0x000F;
    //*(volatile U32*)(MS_RIU_MAP+(0x0E12<<1)) &= ~0x800;    //don't enable uart (decided by mboot env)

    // Switch UART0 to PIU UART 0
	//mdrv_uart_connect(E_UART_PORT0,E_UART_OFF);

#ifdef CONFIG_FAST_UART_RTSCTS
    // Switch UARTx to PIU FAST UART
    SwitchUARTToPIUFASTUART();
#endif

}

MS_BOOL IsHouseKeepingBootingMode(void)
{
    UBOOT_TRACE("IN\n");
    #if defined (CONFIG_BOOTING_FROM_OTP_WITH_PM51)|| defined (CONFIG_BOOTING_FROM_EXT_SPI_WITH_PM51)
    UBOOT_DEBUG("Host keeping is not the booting CPU\n");
    UBOOT_TRACE("OK\n");
    return FALSE;
    #else
    UBOOT_DEBUG("Host keeping is the booting CPU\n");
    UBOOT_TRACE("OK\n");
    return TRUE;
    #endif
}

MS_BOOL IsBootingFromMaskRom(void)
{
    UBOOT_TRACE("IN\n");
    #if defined (CONFIG_MSTAR_ROM_BOOT_WITH_NAND_FLASH)|| defined (CONFIG_MSTAR_ROM_BOOT_WITH_EMMC_FLASH)
    UBOOT_DEBUG("Boot from mask rom\n");
    UBOOT_TRACE("OK\n");
    return TRUE;
    #else
    UBOOT_DEBUG("Doesn't boot from mask rom\n");
    UBOOT_TRACE("OK\n");
    return FALSE;
    #endif
}

MS_BOOL MsApi_MBX_Init(void)
{
    MBX_CPU_ID eHKCPU;
    MS_U32 u32TimeoutMillSecs = 1500;

    eHKCPU = E_MBX_CPU_MIPS;
    if( E_MBX_SUCCESS != MDrv_MBX_Init(eHKCPU, E_MBX_ROLE_HK, u32TimeoutMillSecs))
    {
        UBOOT_ERROR("Error> MBX init failed !!\n");
        jump_to_console();
    }
    else
    {
        MDrv_MBX_Enable(TRUE);
        return TRUE;
    }
    return FALSE;
}

void MsSystemReset(void)
{
   if((IsHouseKeepingBootingMode()==FALSE)&&(ENABLE_MODULE_SECURITY_BOOT==1))
   {
    msAPI_PM_MBX_Init();
    msAPI_PM_Reset();
   }
   else
   {
    MDrv_Sys_WholeChipReset();
   }
   hang();
}

unsigned long MsSystemGetBootTime()
{
  return MDrv_TIMER_GetMs(E_TIMER_1);
}

unsigned long MsSystemGetUSTime()
{
  return MDrv_TIMER_GetUs(E_TIMER_1);
}

#if CONFIG_RESCUE_ENV
#include <MsVfs.h>
#include <drvSERFLASH.h>
#include <MsEnvironment.h>
#include <environment.h>
#include <config.h>
static int _MsApi_BrickTerminator_Recover_Reserve (void);
static int _MsApi_BrickTerminator_Recover_NonReserve (void);
static int _MsApi_BrickTerminator_CheckRecover (unsigned char u8Count);
static int _MsApi_BrickTerminator_BackupEnvStart (void);
static int _MsApi_BrickTerminator_BackupEnv (void);
static int _MsApi_BrickTerminator_RestoreEnv (void);
static int _MsApi_BrickTerminator_ForceRecover (unsigned char u8Status);
static int _MsApi_BrickTerminator_Add2BootArgs (void);
extern int env_reload(void);
extern int restoreenv_from_rescue(void);
extern char *getenv_rescue(char *);
extern unsigned int cfg_env_offset;

//From Kenny
#define BRICK_TERMINATOR_SPI_STATUS_OFFSET_STRING	"BRICK_TERMINATOR_SPI_STATUS_OFFSET"
#define BRICK_TERMINATOR_SPI_WP_GPIO_NUM_STRING	"SPI_WP"
#define STATUS_SIZE 8
#define SIZE_OF_BLOCK sizeof(Brick_Block)

typedef struct
{
    MS_U32 index;
    MS_U8 value[32];

} Brick_Block;

#define BRICKTERMINATOR_STATUS_LENGTH 8
#define SPI_ADDR CONFIG_SPI_STATUS_ADDR
MS_BOOL MsApi_BrickTerminator_GetStatus(unsigned char *status)
{
    *status = MDrv_SYS_ReadBrickTerminatorStatus();
    return TRUE;
}

MS_BOOL MsApi_BrickTerminator_SetStatus(unsigned char status)
{
    MDrv_SYS_WriteBrickTerminatorStatus((unsigned int)status);
    return TRUE;
}

#define BOOT_RECOVER_CNT_DEFAULT 0xF
#define BOOT_RECOVER_MAX_RETRY_COUNT 3
#define BOOT_RECOVER_THRESHOLD (BOOT_RECOVER_CNT_DEFAULT-BOOT_RECOVER_MAX_RETRY_COUNT) //0xC
#define BOOT_RECOVER_PHASE2_MAX_RETRY_COUNT 1
#define BOOT_RECOVER_PHASE2_THRESHOLD (BOOT_RECOVER_CNT_DEFAULT-BOOT_RECOVER_MAX_RETRY_COUNT-BOOT_RECOVER_PHASE2_MAX_RETRY_COUNT) //0xB
#define BOOT_STATUS(x) (x&0xF)
#define BOOT_COUNT(x) (x>>4)
#define BOOT_MERGE(cnt,status) ((cnt<<4)|(BOOT_STATUS(status)))

char *BOOT_RECOVER_ENV_LIST[] =
{
    "bootargs",
    "bootcmd",
    "info_exchange",
    "mtdparts",
    "verify",
    "MIU0_GROUP_PRIORITY",
    "MIU0_GROUP_SELMIU",
    "MIU1_GROUP_PRIORITY",
    "MIU1_GROUP_SELMIU",
    "UBI_SPEED_UP",
    "keybank_done",
    "E_MMAP_ID_VE_ADR",
    "OSD_BufferAddr",
    "str_crc",
    "bootlogo_buffer",
    "bootlogo_gopidx",
    NULL
};
extern char *CUS_BOOT_RECOVER_ENV_LIST[];

typedef enum _E_BRICK_TERMINATOR_STATUS
{
    E_BRICK_TERMINATOR_STATUS_START_RECOVER=0x0, // for type 4 file backup
    E_BRICK_TERMINATOR_STATUS_START_MBOOT=0x1,
    E_BRICK_TERMINATOR_STATUS_START_KERNEL=0x2,
    E_BRICK_TERMINATOR_STATUS_START_RESTORE=0xE, // for type 4 file restore
    E_BRICK_TERMINATOR_STATUS_CLEAR=0xF,
} E_BRICK_TERMINATOR_STATUS;

typedef enum _E_BRICK_TERMINATOR_CHECKSTATUS
{
    E_BRICK_TERMINATOR_CHECKSTATUS_UNDEFINED=-2,
    E_BRICK_TERMINATOR_CHECKSTATUS_ERROR=-1,
    E_BRICK_TERMINATOR_CHECKSTATUS_DONE=0x0,
    E_BRICK_TERMINATOR_CHECKSTATUS_NOUPDATE=0x1,
    E_BRICK_TERMINATOR_CHECKSTATUS_GOPHASE2=0x2,
} E_BRICK_TERMINATOR_CHECKSTATUS;

#if CONFIG_RESCUE_ENV
int do_BrickTerminator_Main (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    const char *BRICK_TERMINATOR_SPI_STATUS_OFFSET = BRICK_TERMINATOR_SPI_STATUS_OFFSET_STRING;
#if(PIN_FLASH_WP0)
    const char *BRICK_TERMINATOR_SPI_WP = BRICK_TERMINATOR_SPI_WP_GPIO_NUM_STRING;
#endif
    unsigned char bClearStatusAndSkipCheck = 0;;
    unsigned char u8Status = 0, u8Count=0;
    char *pSwitch = getenv((char *)BRICK_TERMINATOR_SWITCH_STR);
//    uint u32Tmp=0;

    UBOOT_DEBUG("%s: at %d\n", __func__, __LINE__);
#if (ENABLE_MODULE_SPI_NAND_FLASH == 1)
    MDrv_SYS_Init();
#endif
    //NOTE: check if function switch is off
    {
        unsigned int u32Switch = 1;
        if(NULL!=pSwitch)
        {
            u32Switch = (unsigned int)simple_strtoul(pSwitch, NULL, 10);
        }
        if(0 == u32Switch)
        {
            UBOOT_DEBUG("%s: BYPASS, '%s': %u, at %d\n", __func__, BRICK_TERMINATOR_SWITCH_STR, u32Switch, __LINE__);
            if(0 != del_bootargs_cfg((char*)BRICK_TERMINATOR_SPI_STATUS_OFFSET,0))
            {
                UBOOT_ERROR("%s: Error! del_bootargs_cfg failed, at %d\n", __func__, __LINE__);
            }
#if(PIN_FLASH_WP0)
            if(0 != del_bootargs_cfg((char*)BRICK_TERMINATOR_SPI_WP,0))
            {
                UBOOT_ERROR("%s: Error! del_bootargs_cfg failed, at %d\n", __func__, __LINE__);
            }
#endif
            return 0;
        }
    }

    if (argc > 2)
    {
        printf ("Usage:\n%s\n", cmdtp->usage);
        return 1;
    }
    else if(argc==2)
    {
        if(0==strcmp(argv[1], "recover_reserve"))
        {
            return _MsApi_BrickTerminator_Recover_Reserve();
        }
        else if(0==strcmp(argv[1], "recover_nonreserve"))
        {
            return _MsApi_BrickTerminator_Recover_NonReserve();
        }
        else if(0==strcmp(argv[1], "backup_env_start"))
        {
            return _MsApi_BrickTerminator_BackupEnvStart();
        }
        else if(0==strcmp(argv[1], "backup_env"))
        {
            return _MsApi_BrickTerminator_BackupEnvStart();
        }
        else if(0==strcmp(argv[1], "backup_env_end"))
        {
            return _MsApi_BrickTerminator_BackupEnv();
        }
        else if(0==strcmp(argv[1], "restore_env"))
        {
            return _MsApi_BrickTerminator_RestoreEnv();
        }
        else if(0==strcmp(argv[1], "force_recover"))
        {
            u8Status = BOOT_MERGE(BOOT_RECOVER_THRESHOLD, E_BRICK_TERMINATOR_STATUS_CLEAR); //0xCF
            UBOOT_DEBUG("%s: status: 0x%02x, at %d\n", __func__, u8Status, __LINE__); //DBG
            if(TRUE != MsApi_BrickTerminator_SetStatus(u8Status))
            {
                UBOOT_ERROR("%s: Error! MsApi_BrickTerminator_SetStatus failed! at %d\n", __func__, __LINE__);
                return 1;
            }
            UBOOT_DEBUG("%s: status: 0x%02x, at %d\n", __func__, u8Status, __LINE__); //DBG
            if(TRUE != MsApi_BrickTerminator_GetStatus(&u8Status))
            {
                UBOOT_ERROR("%s: Error! MsApi_BrickTerminator_GetStatus failed! at %d\n", __func__, __LINE__);
                return 1;
            }
            UBOOT_DEBUG("%s: status: 0x%02x, at %d\n", __func__, u8Status, __LINE__); //DBG
#if BRICK_TMP_PATCH
            _MsApi_BrickTerminator_ForceRecover(u8Status);

            {
                if(0 != _MsApi_BrickTerminator_Recover_NonReserve())
                {
                    UBOOT_ERROR("%s: Error! _MsApi_BrickTerminator_Recover_NonReserve failed! at %d\n", __func__, __LINE__);
                    return 1;
                }
                u8Status = BOOT_MERGE(BOOT_RECOVER_CNT_DEFAULT, E_BRICK_TERMINATOR_STATUS_CLEAR);
                if(TRUE != MsApi_BrickTerminator_SetStatus(u8Status))
                {
                    UBOOT_ERROR("%s: Error! MsApi_BrickTerminator_SetStatus failed! at %d\n", __func__, __LINE__);
                    return 1;
                }
            }
            goto BRICK_PATCH_EXIT;
#else
            return _MsApi_BrickTerminator_ForceRecover(u8Status);
#endif
        }
        else if(0==strcmp(argv[1], "add2bootargs"))
        {
            return _MsApi_BrickTerminator_Add2BootArgs();
        }
    }

    if(0 != _MsApi_BrickTerminator_Add2BootArgs())
    {
        UBOOT_ERROR("%s: Error! _MsApi_BrickTerminator_Add2BootArgs failed, at %d\n", __func__, __LINE__);
        //return 1; //NOTE: no need to return, for the case the main env is damaged
    }

    // NOTE: MMIO should be initialized first
    //    MDrv_MMIO_Init();
    //    MDrv_MMIO_GetBASE((MS_U32 *)&MS_RIU_MAP, (MS_U32 *)&u32Tmp, MS_MODULE_PM);

    if(TRUE != MsApi_BrickTerminator_GetStatus(&u8Status))
    {
        UBOOT_ERROR("%s: Error! MsApi_BrickTerminator_GetStatus failed! at %d\n", __func__, __LINE__);
        return 1;
    }
    UBOOT_DEBUG("%s: status: 0x%02x, at %d\n", __func__, u8Status, __LINE__); //DBG

    u8Count = BOOT_COUNT(u8Status);

    //FLOW: 1. check if enter start restore status (0xE)
    if((unsigned char)E_BRICK_TERMINATOR_STATUS_START_RESTORE == BOOT_STATUS(u8Status))
    {
        if(0 != _MsApi_BrickTerminator_Recover_NonReserve())
        {
            UBOOT_ERROR("%s: Error! _MsApi_BrickTerminator_Recover_NonReserve failed! at %d\n", __func__, __LINE__);
            return 1;
        }
    }
    else
    {
        {
#if ((CONFIG_RESCUE_ENV_IR_TRIGGER) && (1 != CONFIG_RESCUE_ENV_AUTO_TRIGGER))
            //NOTE: if IR triggered recover done (PHASE2 done), end all
            if(((unsigned char)E_BRICK_TERMINATOR_STATUS_CLEAR == BOOT_STATUS(u8Status)) && ((unsigned char)BOOT_RECOVER_CNT_DEFAULT == BOOT_COUNT(u8Status)))
            {
                unsigned int u32Value = 0;
                char *pEnv = getenv(BRICK_TERMINATOR_IR_TRIGGERED_STRING);
                if(NULL != pEnv)
                {
                    u32Value = (unsigned int)simple_strtoul(pEnv, NULL, 10);
                    if(1 == u32Value)
                    {
#if BRICK_TMP_PATCH
BRICK_PATCH_EXIT:
                ;
#endif
                        if(0 != del_bootargs_cfg((char*)BRICK_TERMINATOR_SPI_STATUS_OFFSET,0))
                        {
                            UBOOT_ERROR("%s: Error! del_bootargs_cfg failed, at %d\n", __func__, __LINE__);
                        }
#if(PIN_FLASH_WP0)
                        if(0 != del_bootargs_cfg((char*)BRICK_TERMINATOR_SPI_WP,0))
                        {
                            UBOOT_ERROR("%s: Error! del_bootargs_cfg failed, at %d\n", __func__, __LINE__);
                        }
#endif
                        //NOTE: clean env after job done
                        setenv(BRICK_TERMINATOR_IR_TRIGGERED_STRING, NULL);
                        saveenv();
                        UBOOT_DEBUG("%s: status: 0x%02x, at %d\n", __func__, u8Status, __LINE__); //DBG
#if BRICK_TMP_PATCH
                        run_command("reset", 0);
#endif
                        return 0;
                    }
                }
            }
#endif //CONFIG_RESCUE_ENV_IR_TRIGGER

            //NOTE: check if needed to leave
            //RULE: if non-WDT, then should leave
            MDrv_WDT_Init(E_WDT_DBGLV_ALL);
            if(E_WDT_OK != MDrv_WDT_IsReset())
            {
                UBOOT_DEBUG("%s: NON WDT at %d\n", __func__, __LINE__);
                bClearStatusAndSkipCheck = 1;
            }
            else
            {
                UBOOT_DEBUG("%s: WDT on at %d\n", __func__, __LINE__);
                if(E_WDT_OK != MDrv_WDT_ClearRstFlag())
                {
                    UBOOT_ERROR("%s: Error! MDrv_WDT_ClearRstFlag() failed at %d\n", __func__, __LINE__);
                    return 1;
                }
            }

            //NOTE: if no need to run BrickTerminator
            if(1 == bClearStatusAndSkipCheck)
            {
                u8Status = BOOT_MERGE(BOOT_RECOVER_CNT_DEFAULT, E_BRICK_TERMINATOR_STATUS_START_MBOOT);
                if(TRUE != MsApi_BrickTerminator_SetStatus(u8Status))
                {
                    UBOOT_ERROR("%s: Error! MsApi_BrickTerminator_SetStatus failed! at %d\n", __func__, __LINE__);
                    return 1;
                }
                UBOOT_DEBUG("%s: status: 0x%02x, at %d\n", __func__, u8Status, __LINE__); //DBG
                return 0;
            }
        }

        //FLOW: 2. check if enter clear status (0xF)
        if((unsigned char)E_BRICK_TERMINATOR_STATUS_CLEAR == BOOT_STATUS(u8Status))
        {
            if(0 != _MsApi_BrickTerminator_ForceRecover(u8Status))
            {
                UBOOT_ERROR("%s: Error! _MsApi_BrickTerminator_ForceRecover failed! at %d\n", __func__, __LINE__);
                return 1;
            }
            UBOOT_DEBUG("%s: status: 0x%02x, at %d\n", __func__, u8Status, __LINE__); //DBG
            if(TRUE != MsApi_BrickTerminator_GetStatus(&u8Status))
            {
                UBOOT_ERROR("%s: Error! MsApi_BrickTerminator_GetStatus failed! at %d\n", __func__, __LINE__);
                return 1;
            }
        }
        //FLOW: 3. other status
        else
        {
            //NOTE: 1. CNT += 1
            u8Count -= 1;
            u8Status = BOOT_MERGE(u8Count, u8Status);
            if(TRUE != MsApi_BrickTerminator_SetStatus(u8Status))
            {
                UBOOT_ERROR("%s: MsApi_BrickTerminator_SetStatus errs! at %d\n", __func__, __LINE__);
                return 1;
            }
            //NOTE: 2. continue normal boot
            UBOOT_DEBUG("%s: status: 0x%02x, at %d\n", __func__, u8Status, __LINE__); //DBG
            if(TRUE != MsApi_BrickTerminator_GetStatus(&u8Status))
            {
                UBOOT_ERROR("%s: MsApi_BrickTerminator_GetStatus errs! at %d\n", __func__, __LINE__);
                return 1;
            }
            UBOOT_DEBUG("%s: u8Count: 0x%02x, at %d\n", __func__, u8Count, __LINE__); //DBG
            UBOOT_DEBUG("%s: status: 0x%02x, at %d\n", __func__, u8Status, __LINE__); //DBG

            if(0 != _MsApi_BrickTerminator_ForceRecover(u8Status))
            {
                UBOOT_ERROR("%s: Error! _MsApi_BrickTerminator_ForceRecover failed! at %d\n", __func__, __LINE__);
                return 1;
            }
            UBOOT_DEBUG("%s: status: 0x%02x, at %d\n", __func__, u8Status, __LINE__); //DBG
            if(TRUE != MsApi_BrickTerminator_GetStatus(&u8Status))
            {
                UBOOT_ERROR("%s: Error! MsApi_BrickTerminator_GetStatus failed! at %d\n", __func__, __LINE__);
                return 1;
            }
        }
    }

    UBOOT_DEBUG("%s: status: 0x%02x, at %d\n", __func__, u8Status, __LINE__); //DBG

    return 0;
}
#endif

static int _MsApi_BrickTerminator_CheckRecover (unsigned char u8Count)
{
    unsigned char u8Status = 0;
    if(BOOT_RECOVER_THRESHOLD >= u8Count)
    {
        if(BOOT_RECOVER_PHASE2_THRESHOLD >= u8Count) //NOTE: AP failed to recover, go phase2 straight
        {
            return E_BRICK_TERMINATOR_CHECKSTATUS_GOPHASE2;
        }
        UBOOT_DEBUG("%s: START RECOVERY, u8Count: 0x%02x, at %d\n", __func__, u8Count, __LINE__); //DBG
        //NOTE: 0. DO env RECOVERY
        if(0 != _MsApi_BrickTerminator_RestoreEnv())
        {
            UBOOT_ERROR("%s: Error! _MsApi_BrickTerminator_RestoreEnv failed! at %d\n", __func__, __LINE__);
            //return 1; //TODO: consider of need to return
        }
        if(0 != _MsApi_BrickTerminator_Add2BootArgs())
        {
            UBOOT_ERROR("%s: Error! _MsApi_BrickTerminator_Add2BootArgs failed, at %d\n", __func__, __LINE__);
            return 1;
        }
        //NOTE: 1. DO reserve part RECOVERY
#if 1
        {
            if(0 != _MsApi_BrickTerminator_Recover_Reserve())
            {
                UBOOT_ERROR("%s: Error! _MsApi_BrickTerminator_Recover_Reserve failed! at %d\n", __func__, __LINE__);
                return E_BRICK_TERMINATOR_CHECKSTATUS_ERROR;
            }
        }
#endif
        //NOTE: 2. FLAGS = 0x0
        u8Status = (unsigned char)E_BRICK_TERMINATOR_STATUS_START_RECOVER;

        u8Status = BOOT_MERGE(u8Count, u8Status);
        if(TRUE != MsApi_BrickTerminator_SetStatus(u8Status))
        {
            UBOOT_ERROR("%s: Error! MsApi_BrickTerminator_SetStatus failed! at %d\n", __func__, __LINE__);
            return E_BRICK_TERMINATOR_CHECKSTATUS_ERROR;
        }
        UBOOT_DEBUG("%s: status: 0x%02x, at %d\n", __func__, u8Status, __LINE__); //DBG
        if(TRUE != MsApi_BrickTerminator_GetStatus(&u8Status))
        {
            UBOOT_ERROR("%s: Error! MsApi_BrickTerminator_GetStatus failed! at %d\n", __func__, __LINE__);
            return E_BRICK_TERMINATOR_CHECKSTATUS_ERROR;
        }
        //NOTE: 4. continue normal boot
        return E_BRICK_TERMINATOR_CHECKSTATUS_DONE; // NOTE: done
    }
    return E_BRICK_TERMINATOR_CHECKSTATUS_NOUPDATE; // NOTE: no need to update
}

#define BACKUP_DONE_STR "BACKUP_DONE"
static int _MsApi_BrickTerminator_BackupEnvStart ()
{
    setenv_rescue(BACKUP_DONE_STR, NULL);
    saveenv_rescue();
    return 0;
}

static int _MsApi_BrickTerminator_BackupEnv ()
{
    char *pEnv = getenv_rescue(BACKUP_DONE_STR);
    if((NULL != pEnv) && (1 == (unsigned int)simple_strtoul(pEnv, NULL, 10)))
    {
        UBOOT_DEBUG("%s: BACKUP_DONE, at %d\n", __func__, __LINE__);
        return 0;
    }
#if (defined(CONFIG_BRICK_BACKUP_WHOLE_ENV_ENABLE) && (1==CONFIG_BRICK_BACKUP_WHOLE_ENV_ENABLE))
    char *ptr=NULL;
    char cmd[CMD_BUF]={};
    void* pEnvBuf = NULL;
    unsigned int u32fileSize=0;
    pEnvBuf = malloc(CONFIG_ENV_SIZE);
    if(pEnvBuf==NULL)
    {
        UBOOT_ERROR("malloc() failed\n");
        return 1;
    }
    UBOOT_DEBUG("%s: addr of pEnvBuf: 0x%x, at %d\n", __func__, (unsigned int)pEnvBuf, __LINE__);
    memset(cmd,0,sizeof(cmd));
    snprintf(cmd, sizeof(cmd)-1, "env export -c %x", pEnvBuf);;
    UBOOT_DEBUG("%s: cmd: '%s', at %d\n", __func__, cmd, __LINE__);
    run_command(cmd, 0);

    ptr = getenv("filesize");
    if(NULL != ptr)
    {
        u32fileSize = (unsigned int)simple_strtoul(ptr, NULL, 16);
    }
    memset(cmd,0,sizeof(cmd));
    snprintf(cmd, sizeof(cmd)-1, "env import_rescue -c %x %x", pEnvBuf, u32fileSize);
    UBOOT_DEBUG("%s: cmd: '%s', at %d\n", __func__, cmd, __LINE__);
    run_command(cmd, 0);

    free(pEnvBuf);

    if(0!=saveenv_rescue())
    {
        UBOOT_ERROR("%s: ERROR: saveenv_rescue failed, at %d\n", __func__, __LINE__);
        return 1;
    }
#else
#if 1 // NOTE: backup env to rescue
    int idx=0;
    MS_BOOL bUpdate = FALSE;
    char *pBuf=NULL;
    pEnv = NULL;

    UBOOT_DEBUG("%s: at %d\n", __func__, __LINE__); // DBG
    //NOTE: 1. backup the required env from current
    for(idx=0; NULL!=BOOT_RECOVER_ENV_LIST[idx]; idx++)
    {
        pEnv = getenv(BOOT_RECOVER_ENV_LIST[idx]);
        if(NULL != pEnv)
        {
            pBuf = malloc(strlen(pEnv)+1);
            if(NULL == pBuf)
            {
                UBOOT_ERROR("%s: Error! malloc(%d) failed, at %d\n", __func__, strlen(pEnv)+1, __LINE__); //DBG
                continue;
            }
            memset(pBuf, 0, strlen(pEnv)+1);
            snprintf(pBuf, strlen(pEnv), "%s", pEnv);
            setenv_rescue(BOOT_RECOVER_ENV_LIST[idx], pBuf);
            if(bUpdate != TRUE)
            {
                bUpdate = TRUE;
            }
            UBOOT_DEBUG("%s: update env: '%s', at %d\n", __func__, BOOT_RECOVER_ENV_LIST[idx], __LINE__); //DBG
            free(pBuf);
        }
    }
    //NOTE: 1.1 backup the customer required env from current
    for(idx=0; NULL!=CUS_BOOT_RECOVER_ENV_LIST[idx]; idx++)
    {
        pEnv = getenv(CUS_BOOT_RECOVER_ENV_LIST[idx]);
        if(NULL != pEnv)
        {
            pBuf = malloc(strlen(pEnv)+1);
            if(NULL == pBuf)
            {
                UBOOT_ERROR("%s: Error! malloc(%d) failed, at %d\n", __func__, strlen(pEnv)+1, __LINE__); //DBG
                continue;
            }
            memset(pBuf, 0, strlen(pEnv)+1);
            snprintf(pBuf, strlen(pEnv), "%s", pEnv);
            setenv_rescue(CUS_BOOT_RECOVER_ENV_LIST[idx], pBuf);
            if(bUpdate != TRUE)
            {
                bUpdate = TRUE;
            }
            UBOOT_DEBUG("%s: update env: '%s', at %d\n", __func__, CUS_BOOT_RECOVER_ENV_LIST[idx], __LINE__); //DBG
            free(pBuf);
        }
    }
    if(TRUE == bUpdate)
    {
        if(0!=saveenv_rescue())
        {
            UBOOT_ERROR("%s: ERROR: saveenv_rescue failed, at %d\n", __func__, __LINE__);
            return 1;
        }
    }
#endif
#endif
    setenv_rescue(BACKUP_DONE_STR, "1");
    saveenv_rescue();
    UBOOT_DEBUG("%s: at %d\n", __func__, __LINE__); // DBG
    return 0;
}

static int _MsApi_BrickTerminator_RestoreEnv ()
{
    UBOOT_DEBUG("%s: at %d\n", __func__, __LINE__); // DBG
    if(0 != restoreenv_from_rescue()) // IF FINISH RECOVERY
    {
        UBOOT_ERROR("%s: Error! restoreenv_from_rescue failed! at %d\n", __func__, __LINE__);
        return 1;
    }
    //NOTE: reload env
    if(0!=env_reload())
    {
        UBOOT_ERROR("%s: Error! env_reload failed! at %d\n", __func__, __LINE__);
        return 1;
    }
#if !(CONFIG_LOGO_STORE_IN_MBOOT) && !(ENABLE_LOGO_STORE_IN_MBOOT_HYBRID)
    run_command("dbtable_init", 0); //TODO: refine it
#endif
    run_command("updatemiureg", 0); //TODO: refine it
    return 0;
}

static int _MsApi_BrickTerminator_Recover_Reserve ()
{
    MS_BOOL bNeedReburn = TRUE;
    U32 u32PathSize = 0;
    const char* BOOT_RECOVER_BRICK_RESERVE_VOLUME = "brickreserve";
    const char* BOOT_RECOVER_BRICK_BACKUP_VOLUME = "brickbackup";
    const char* BOOT_RECOVER_BRICK_RESERVE_FILE = "/.ready";
    const char* BOOT_RECOVER_BRICK_BACKUP_FILE = "/MstarUpgrade_reserve.bin";
    if(0 == vfs_mount((char*)BOOT_RECOVER_BRICK_RESERVE_VOLUME))
    {
        u32PathSize = vfs_getsize((char*)BOOT_RECOVER_BRICK_RESERVE_FILE);
        if(0 < u32PathSize)
        {
            bNeedReburn = FALSE;
            UBOOT_DEBUG("%s: u32PathSize: %u, at %d\n", __func__, u32PathSize, __LINE__); // DBG
        }
        else // TODO: remove after debug
        {
            UBOOT_ERROR("%s: ERROR: vfs_getsize failed, at %d\n", __func__, __LINE__);
        }
    }
    if(TRUE == bNeedReburn)
    {
        char cmd[CMD_BUF] = {};
        vfs_umount();
        if(0 == vfs_mount((char*)BOOT_RECOVER_BRICK_BACKUP_VOLUME))
        {
            sprintf(cmd, "bstar %s", BOOT_RECOVER_BRICK_BACKUP_FILE);
            UBOOT_ERROR("%s: ERROR: VFS failed, at %d\n", __func__, __LINE__);
            UBOOT_DEBUG("%s: DO BRICK RESERVE RECOVER, at %d\n", __func__, __LINE__);
            //NOTE: do partition recover
            run_command(cmd, 0);
            vfs_umount();
        }
    }
    else
    {
        UBOOT_DEBUG("%s: '%s' is ok, no need to re-burn, at %d\n", __func__, BOOT_RECOVER_BRICK_RESERVE_VOLUME, __LINE__);
        vfs_umount();
    }
    return 0;
}

static int _MsApi_BrickTerminator_Recover_NonReserve ()
{
    char cmd[CMD_BUF] = {};
    const char* BOOT_RECOVER_BRICK_BACKUP_VOLUME = "brickbackup";
    const char* BOOT_RECOVER_BRICK_BACKUP_FILE = "/MstarUpgrade_backup.bin";

    vfs_umount();
    if(0 == vfs_mount((char*)BOOT_RECOVER_BRICK_BACKUP_VOLUME))
    {
        sprintf(cmd, "bstar %s", BOOT_RECOVER_BRICK_BACKUP_FILE);
        UBOOT_ERROR("%s: ERROR: VFS failed, at %d\n", __func__, __LINE__);
        UBOOT_DEBUG("%s: DO BRICK RESERVE RECOVER, at %d\n", __func__, __LINE__);
        //NOTE: do partition recover
        run_command(cmd, 0);
        vfs_umount();
    }
    return 0;
}

static int _MsApi_BrickTerminator_ForceRecover (unsigned char status)
{
    E_BRICK_TERMINATOR_CHECKSTATUS u8RetStatus=E_BRICK_TERMINATOR_CHECKSTATUS_UNDEFINED;
    unsigned char u8Status = 0, u8Count=0;

    UBOOT_DEBUG("%s: at %d\n", __func__, __LINE__); // DBG
    u8Count = BOOT_COUNT(status);
    u8Status = BOOT_STATUS(status);

    u8RetStatus = _MsApi_BrickTerminator_CheckRecover (u8Count);
    UBOOT_DEBUG("%s: u8RetStatus: 0x%02x, at %d\n", __func__, u8RetStatus, __LINE__); //DBG
    if(E_BRICK_TERMINATOR_CHECKSTATUS_ERROR == u8RetStatus)
    {
        UBOOT_ERROR("%s: Error! _MsApi_BrickTerminator_CheckRecover failed! at %d\n", __func__, __LINE__);
        return 1;
    }
    else if(E_BRICK_TERMINATOR_CHECKSTATUS_NOUPDATE == u8RetStatus) // no need to update
    {
        UBOOT_DEBUG("%s: status: 0x%02x, at %d\n", __func__, u8Status, __LINE__); //DBG
        //NOTE: 2. FLAGS = 0x1
        u8Status = (unsigned char)E_BRICK_TERMINATOR_STATUS_START_MBOOT;
        u8Status = BOOT_MERGE(u8Count, u8Status);
        if(TRUE != MsApi_BrickTerminator_SetStatus(u8Status))
        {
            UBOOT_ERROR("%s: Error! MsApi_BrickTerminator_SetStatus failed! at %d\n", __func__, __LINE__);
            return 1;
        }
    }
    else if(E_BRICK_TERMINATOR_CHECKSTATUS_GOPHASE2 == u8RetStatus)
    {
        if(0 != _MsApi_BrickTerminator_Recover_NonReserve())
        {
            UBOOT_ERROR("%s: Error! _MsApi_BrickTerminator_Recover_NonReserve failed! at %d\n", __func__, __LINE__);
            return 1;
        }
        u8Status = BOOT_MERGE(BOOT_RECOVER_CNT_DEFAULT, E_BRICK_TERMINATOR_STATUS_CLEAR);
        if(TRUE != MsApi_BrickTerminator_SetStatus(u8Status))
        {
            UBOOT_ERROR("%s: Error! MsApi_BrickTerminator_SetStatus failed! at %d\n", __func__, __LINE__);
            return 1;
        }
    }
    return 0;
}

static int _MsApi_BrickTerminator_Add2BootArgs (void)
{
    const char *BRICK_TERMINATOR_SPI_STATUS_OFFSET = BRICK_TERMINATOR_SPI_STATUS_OFFSET_STRING;
    unsigned int u32RescueStatusOffset =0;
    char buf[CMD_BUF] = "\0";
    int ret=0;
    // set BRICK_TERMINATOR_SPI_STATUS_OFFSET to bootargs
    MsApiChunkHeader_GetValue(CH_RESCURE_STATUS_ROM_OFFSET,&u32RescueStatusOffset);
    u32RescueStatusOffset = GetRealOffset(u32RescueStatusOffset);
    sprintf(buf, "%s=%08X", BRICK_TERMINATOR_SPI_STATUS_OFFSET, u32RescueStatusOffset);
    if(0 != set_bootargs_cfg((char*)BRICK_TERMINATOR_SPI_STATUS_OFFSET, buf, 0))
    {
        UBOOT_ERROR("Error! set_bootargs_cfg failed\n");
        ret = -1;
    }

#if (PIN_FLASH_WP0&&ENABLE_MODULE_ENV_IN_SERIAL)
    {   // set Write Protect GPIO to bootargs
        const char *BRICK_TERMINATOR_SPI_WP = BRICK_TERMINATOR_SPI_WP_GPIO_NUM_STRING;
        memset(buf,0,sizeof(buf));
        sprintf(buf, "%s=%X", BRICK_TERMINATOR_SPI_WP, PIN_FLASH_WP0);
        if(0 != set_bootargs_cfg((char*)BRICK_TERMINATOR_SPI_WP, buf, 0))
        {
            UBOOT_ERROR("%s: Error! set_bootargs_cfg failed, at %d\n", __func__, __LINE__);
            ret = -1;
        }
    }
#endif
    return ret;
}

MS_BOOL MSys_IsStatusBricking(void)
{
    unsigned char status = MDrv_SYS_ReadBrickTerminatorStatus();
    UBOOT_DEBUG("%s: status: 0x%02x, at %d\n", __func__, status, __LINE__);
    if(((BOOT_STATUS(status) == (unsigned char)E_BRICK_TERMINATOR_STATUS_START_RECOVER) || (BOOT_STATUS(status) == (unsigned char)E_BRICK_TERMINATOR_STATUS_START_RESTORE))
        && ((unsigned char)BOOT_RECOVER_PHASE2_THRESHOLD <= BOOT_COUNT(status)) && (BOOT_COUNT(status) <= (unsigned char)BOOT_RECOVER_THRESHOLD))
    {
        return true;
    }
    return false;
}
#endif

