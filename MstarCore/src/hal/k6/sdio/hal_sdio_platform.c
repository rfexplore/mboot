////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2011 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// ("MStar Confidential Information") by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

// this file is use only for kastor project 2016.4.18

//#include <config.h>
//#include "drvFCIE_config.h"



//#include <stdio.h>
//#include <mmc.h>
//#include "drvFCIE5.h"

struct mmc_host;
#include "hal_sdio.h"
#ifdef CONFIG_MSTAR_CLKM
#include "linux/clkm.h"
#endif
extern unsigned char sd_ddr_mode;
extern void HalSdio_DumpDebugBus(void);
extern unsigned char gu8SdioSilenceTuning;

/**************************************
* Config definition
**************************************/
#define DBG_CR_CLK(MSG)             //MSG
#define DBG_CR_PAD(MSG)             //MSG

/**************************************
* Local function definition
**************************************/


/**************************************
* Global function definition
**************************************/
void HalSdio_ResetIP(void)
{
	U16 u16Reg, u16Cnt;

	FCIE_RIU_W16(FCIE_SD_CTRL, 0); // clear job start for safety

	//printk(LIGHT_CYAN"sdio reset\n"NONE);

	FCIE_RIU_W16(FCIE_MIE_FUNC_CTL, BIT_SDIO_EN);

	FCIE_RIU_16_OF(FCIE_RST, BIT_FCIE_SOFT_RST_n); /* active low */

	// SDIO reset - wait
	u16Cnt=0;

	do
	{
		u16Reg = FCIE_RIU_R16(FCIE_RST);

		HalSdioDelayUs(1);

		if(0x1000 == u16Cnt++)
		{
			printk("SD Err: SDIO Reset fail!! FCIE_RST = %04Xh\n", u16Reg);
			while(1);
		}

	} while (BIT_RST_STS_MASK  != (u16Reg  & BIT_RST_STS_MASK));

	FCIE_RIU_16_ON(FCIE_RST, BIT_FCIE_SOFT_RST_n);

	u16Cnt=0;

	do
	{
		u16Reg = FCIE_RIU_R16(FCIE_RST);
		//printk("FCIE_RST = %04Xh\n", u16Reg);

		if(0x1000 == u16Cnt++)
		{
			printk("SD Err: SDIO Reset fail2:h \n");
			return ;
		}

		HalSdioDelayUs(1);


	} while (0  != (u16Reg  & BIT_RST_STS_MASK));

	//printk("ok\n");

}

//#define CONFIG_SDIO_MODE 3

void HalSdio_Platform_InitChiptop(void)
{
#if (ALTERNATIVE_IP == FCIE_IP)

	#ifdef CONFIG_SD
	unsigned long ulSD_CONFIG_NUM = 1;
	#endif

	FCIE_RIU_16_OF(CHIPTOP_50h, REG_ALL_PAD_IN);
	FCIE_RIU_16_OF(CHIPTOP_08h, REG_NAND_MODE); // clear nand_mode
	FCIE_RIU_16_OF(CHIPTOP_08h, REG_EMMC_CONFIG); // clear emmc_config

	if(ulSD_CONFIG_NUM==1)
	{
		FCIE_RIU_16_ON(CHIPTOP_08h, REG_SD_CONFIG);
	}
	else
	{
		printk(LIGHT_RED"SdErr: wrong parameter for sd_config %ld!\n"NONE, ulSD_CONFIG_NUM);
	}

	// MIU select by FCIE
	if(!(FCIE_RIU_R16(MIU2_7A)&MIU_SELECT_BY_FCIE))
	{
		FCIE_RIU_16_ON(MIU2_7A, MIU_SELECT_BY_FCIE);
		printk("SdErr: MIU select setting wrong for FCIE\n");
	}

#elif (ALTERNATIVE_IP == SDIO_IP)

	#ifdef CONFIG_SD
	unsigned long ulSDIO_CONFIG_NUM = 1;
	#endif

	FCIE_RIU_16_OF(CHIPTOP_50h, REG_ALL_PAD_IN);

	FCIE_RIU_16_OF(CHIPTOP_08h, REG_SDIO_CONFIG_MSK);

	if(ulSDIO_CONFIG_NUM==1) // PCM PAD
	{
		FCIE_RIU_16_ON(CHIPTOP_08h, REG_SDIO_MODE_1);
	}
	else
	{
		printk(LIGHT_RED"SdErr: wrong parameter for sdio_config %ld!\n"NONE, ulSDIO_CONFIG_NUM);
	}

	// MIU select by SDIO
	FCIE_RIU_16_ON(MIU2_7A, MIU_SELECT_BY_SDIO);

#endif
}


#if !defined FPGA_BOARD || FPGA_BOARD==0

#if (ALTERNATIVE_IP == SDIO_IP)

const U8 SdioClockIdxNum = 7;

const U32 SDIO_CLOCK[] =
{
    48000, //  0
    43200, //  1
    40000, //  2
    36000, //  3
    32000, //  4
    20000, //  5
    12000, //  6
      300, //  7
};

// find clock close to target but not over

U32 HalSdio_SetClock(U32 u32Clock)
{
    U8 u8ClockSlector;
	/*static U32 u32_OldClock=0xFFFFFFFF;

	//printk("HalSdio_SetClock(%ld)\n", u32Clock);

	if(u32_OldClock == u32Clock)
		return 0;
	else
		u32_OldClock = u32Clock;*/

#ifdef CONFIG_MSTAR_CLKM
	S32 sdio_clk, sdio_gate;

	sdio_clk = get_handle("g_clk_sdio_p");
	sdio_gate = get_handle("g_clk_sdio");
	set_clk_source(sdio_gate, "g_sdio_clk_xtal"); // clock gated
#else
    FCIE_RIU_16_ON(REG_CLK_SDIO, BIT00); // turn on clock gating
#endif

	if(u32Clock>1000)
	{
		DBG_CR_CLK(printk("Set SDIO clock as %d.%d MHz, ", u32Clock/1000, (u32Clock%1000)/100 ) );
	}
	else
	{
		DBG_CR_CLK(printk("Set SDIO clock as %d KHz, ", u32Clock));
	}

	for(u8ClockSlector=0; u8ClockSlector<=SdioClockIdxNum; u8ClockSlector++)
	{
		if( SDIO_CLOCK[u8ClockSlector] <= u32Clock )
		{
			break;
		}
	}

	if(u8ClockSlector>SdioClockIdxNum)
	{
		DBG_CR_CLK(printk("Error!!! Can not find proper clock!\r\n"));
		while(1);
		return 0x01;
	}

	if(u32Clock>1000)
	{
		DBG_CR_CLK(printk("select SDIO clock as %d.%d MHz\r\n", SDIO_CLOCK[u8ClockSlector]/1000, (SDIO_CLOCK[u8ClockSlector]%1000)/100));
	}
	else
	{
		DBG_CR_CLK(printk("select SDIO clock as %d KHz\r\n", SDIO_CLOCK[u8ClockSlector]));
	}

	#ifndef CONFIG_MSTAR_CLKM
		FCIE_RIU_16_ON(REG_CLK_SDIO, BIT06);
		FCIE_RIU_16_OF(REG_CLK_SDIO, BIT05+BIT04+BIT03+BIT02); // mask all clock select
	#endif

	//printk("switch to clock: %d\n", u8ClockSlector);

	switch(u8ClockSlector)
	{
		case 0: // 48M
			#ifdef CONFIG_MSTAR_CLKM
				set_clk_source(sdio_clk, "g_sdio_clk_48");
			#else
				FCIE_RIU_16_ON(REG_CLK_SDIO, 0xF<<BIT_CLKGEN_SDIO_SHIFT);
			#endif
			break;

		case 1: // 43.2M
			#ifdef CONFIG_MSTAR_CLKM
				set_clk_source(sdio_clk, "g_sdio_clk_43");
			#else
				FCIE_RIU_16_ON(REG_CLK_SDIO, 0x5<<BIT_CLKGEN_SDIO_SHIFT);
			#endif
			break;

		case 2: // 40M
			#ifdef CONFIG_MSTAR_CLKM
				set_clk_source(sdio_clk, "g_sdio_clk_40");
			#else
				FCIE_RIU_16_ON(REG_CLK_SDIO, 0x4<<BIT_CLKGEN_SDIO_SHIFT);
			#endif
			break;

		case 3: // 36M
			#ifdef CONFIG_MSTAR_CLKM
				set_clk_source(sdio_clk, "g_sdio_clk_36");
			#else
				FCIE_RIU_16_ON(REG_CLK_SDIO, 0x3<<BIT_CLKGEN_SDIO_SHIFT);
			#endif
			break;

		case 4: // 32M
			#ifdef CONFIG_MSTAR_CLKM
				set_clk_source(sdio_clk, "g_sdio_clk_32");
			#else
				FCIE_RIU_16_ON(REG_CLK_SDIO, 0x2<<BIT_CLKGEN_SDIO_SHIFT);
			#endif
			break;

		case 5: // 20M
			#ifdef CONFIG_MSTAR_CLKM
				set_clk_source(sdio_clk, "g_sdio_clk_20");
			#else
				FCIE_RIU_16_ON(REG_CLK_SDIO, 0x1<<BIT_CLKGEN_SDIO_SHIFT);
			#endif
			break;

		case 6: // 12M
			#ifdef CONFIG_MSTAR_CLKM
				set_clk_source(sdio_clk, "g_sdio_clk_xtal");
			#else
				FCIE_RIU_16_ON(REG_CLK_SDIO, 0xE<<BIT_CLKGEN_SDIO_SHIFT);
			#endif
			break;

		case 7: // 300K
			#ifdef CONFIG_MSTAR_CLKM
				set_clk_source(sdio_clk, "g_sdio_clk_300k");
			#else
				FCIE_RIU_16_ON(REG_CLK_SDIO, 0xD<<BIT_CLKGEN_SDIO_SHIFT);
			#endif
			break;

		default:
			printk("SDIO Err: wrong clock selector!\n");
			while(1);
			break;

	}

	DBG_CR_CLK(printk("REG_CLK_SDIO = 0x%04X\r\n", FCIE_RIU_R16(REG_CLK_SDIO)));

	#ifdef CONFIG_MSTAR_CLKM
		set_clk_source(sdio_gate, "g_sdio_clk_p"); // clock enable
	#else
		FCIE_RIU_16_OF(REG_CLK_SDIO, BIT00); // turn off clock gating
	#endif

	return SDIO_CLOCK[u8ClockSlector];

}

#elif (ALTERNATIVE_IP == FCIE_IP)

const U8 FcieClockIdxNum = 8;

const U32 FCIE_CLOCK[] =
{
    48000, //  0
    40000, //  1
    36000, //  2
    27000, //  3
    21500, //  4
    18000, //  5
    15500, //  6
    12000, //  7
       75, //  8
};

// find clock close to target but not over

U32 HalSdio_SetClock(U32 u32Clock)
{
    U8 u8ClockSlector;

    FCIE_RIU_16_ON(REG_CLK_FCIE, BIT_FCIE_CLK_GATING); // turn on clock gating

	if(u32Clock>1000)
	{
		DBG_CR_CLK(printk("Set FCIE clock as %d.%d MHz, ", u32Clock/1000, (u32Clock%1000)/100 ) );
	}
	else
	{
		DBG_CR_CLK(printk("Set FCIE clock as %d KHz, ", u32Clock));
	}

	for(u8ClockSlector=0; u8ClockSlector<=FcieClockIdxNum; u8ClockSlector++)
	{
		if( FCIE_CLOCK[u8ClockSlector] <= u32Clock )
		{
			break;
		}
	}

	if(u8ClockSlector>FcieClockIdxNum)
	{
		DBG_CR_CLK(printk("Error!!! Can not find proper clock!\r\n"));
		while(1);
		return 0x01;
	}

	if(u32Clock>1000)
	{
		DBG_CR_CLK(printk("select FCUE clock as %d.%d MHz\r\n", FCIE_CLOCK[u8ClockSlector]/1000, (FCIE_CLOCK[u8ClockSlector]%1000)/100));
	}
	else
	{
		DBG_CR_CLK(printk("select FCIE clock as %d KHz\r\n", FCIE_CLOCK[u8ClockSlector]));
	}

	FCIE_RIU_16_ON(REG_CLK_FCIE, BIT_FCIE_CLK_SRC_SEL);
	FCIE_RIU_16_OF(REG_CLK_FCIE, BIT_CLKGEN_FCIE_MASK); // mask all clock select

	//printk("switch to clock: %d\n", u8ClockSlector);

	switch(u8ClockSlector)
	{
		case 0: // 48M
			FCIE_RIU_16_ON(REG_CLK_FCIE, 0x8<<BIT_CLKGEN_FCIE_SHIFT);
			break;

		case 1: // 40M
			FCIE_RIU_16_ON(REG_CLK_FCIE, 0x7<<BIT_CLKGEN_FCIE_SHIFT);
			break;

		case 2: // 36M
			FCIE_RIU_16_ON(REG_CLK_FCIE, 0x6<<BIT_CLKGEN_FCIE_SHIFT);
			break;

		case 3: // 27M
			FCIE_RIU_16_ON(REG_CLK_FCIE, 0x5<<BIT_CLKGEN_FCIE_SHIFT);
			break;

		case 4: // 21.5M
			FCIE_RIU_16_ON(REG_CLK_FCIE, 0x4<<BIT_CLKGEN_FCIE_SHIFT);
			break;

		case 5: // 18M
			FCIE_RIU_16_ON(REG_CLK_FCIE, 0x3<<BIT_CLKGEN_FCIE_SHIFT);
			break;

		case 6: // 15.5M
			FCIE_RIU_16_ON(REG_CLK_FCIE, 0x2<<BIT_CLKGEN_FCIE_SHIFT);
			break;

		case 7: // 12M
			FCIE_RIU_16_ON(REG_CLK_FCIE, 0x1<<BIT_CLKGEN_FCIE_SHIFT);
			break;

		case 8: // 75K
			FCIE_RIU_16_ON(REG_CLK_FCIE, 0xC<<BIT_CLKGEN_FCIE_SHIFT);
			break;

		default:
			printk("FCIE Err: wrong clock selector!\n");
			while(1);
			break;

	}

	FCIE_RIU_16_OF(REG_CLK_FCIE, BIT_FCIE_CLK_GATING); // turn off clock gating

	DBG_CR_CLK(printk("REG_CLK_FCIE = 0x%04X\r\n", FCIE_RIU_R16(REG_CLK_FCIE)));

	return FCIE_CLOCK[u8ClockSlector];

}

#endif

//static U32 gu32SdClock = 0;

#define BUS_SPEED_SDR12		1
#define BUS_SPEED_SDR25		2
#define BUS_SPEED_DDR50		3
#define BUS_SPEED_SDR50		4
#define BUS_SPEED_SDR104	5

//static U32 gu8BusSpeed = BUS_SPEED_SDR12;

#else

U8 HalSdio_SetSdioClock(U32 u32Clock)
{
	if (u32Clock < 400)
	{
		printk("SDIO FPGA clock 187.5KHz\n");
		FCIE_RIU_16_ON(R_SDIO_PLL_0x1D, BIT00);
	}
	else
	{
		printk("SDIO FPGA clock 1.5MHz\n");
		FCIE_RIU_16_OF(R_SDIO_PLL_0x1D, BIT00);
	}
	FCIE_RIU_16_ON(FCIE_SD_MODE, BIT_CLK_EN); // enable clk

	return 0;
}

#endif

static U8 u8CurrentPadType = SDIO_MODE_UNKNOWN;

U8 HalSdio_GetPadType(void)
{
	return u8CurrentPadType;
}

void HalSdio_SwitchPad(unsigned char u32Mode)
{
	//printk("switch pad %d, current pad type = %d\n", u32Mode, u8CurrentPadType);

	// chiptop
	HalSdio_Platform_InitChiptop();

	// sdio
	FCIE_RIU_16_OF(FCIE_DDR_MODE, BIT_FALL_LATCH|BIT_PAD_IN_SEL_SD|BIT_32BIT_MACRO_EN|BIT_DDR_EN|BIT_8BIT_MACRO_EN);


	switch(u32Mode)
	{
		///////////////////////////////////////////////////////////////////////////////////////////
		case SDIO_MODE_GPIO_PAD_BPS:
			DBG_CR_PAD(printk(LIGHT_CYAN"SDIO_MODE_GPIO_PAD_BPS\n"NONE));
			u8CurrentPadType = SDIO_MODE_GPIO_PAD_BPS;
			break;

		///////////////////////////////////////////////////////////////////////////////////////////
		case SDIO_MODE_GPIO_PAD_SDR:
			DBG_CR_PAD(printk(LIGHT_CYAN"SDIO_MODE_GPIO_PAD_SDR\n"NONE));
			u8CurrentPadType = SDIO_MODE_GPIO_PAD_SDR;
			FCIE_RIU_16_ON(FCIE_DDR_MODE, BIT_FALL_LATCH|BIT_PAD_IN_SEL_SD);
			// New patch for feedback clock reflex
			FCIE_RIU_16_ON(FCIE_DDR_MODE, BIT_FEEDBACK_CLK2);
			break;

		default:
			u8CurrentPadType = SDIO_MODE_UNKNOWN;
			DBG_CR_PAD(printk(LIGHT_CYAN"SdErr: wrong parameter for switch pad func\n"NONE));
			while(1);
			//return 0x01;
			break;
	}


	//return 0x00;

//ErrorHandle:

	//printk("SDIO Err: set bus width before pad switch\n");
	//return 0x02;

}


U8 HalSdio_Platform_ClearEvent(U16 nReq)
{
	U16  u16Tmp;

	u16Tmp = 0x0080;
	while((FCIE_RIU_R16(FCIE_MIE_EVENT)& nReq) != 0x00)
	{
		FCIE_RIU_W16(FCIE_MIE_EVENT, nReq); // write 1 clear register
		if (u16Tmp==0)
		{
			printk("Error!!! Can not clear MIE event.\r\n");
			return(1);
		}
		else
		{
			u16Tmp--;
		}
	}

	return 0;
}

#if defined(ENABLE_SDIO_INTERRUPT_MODE)&&ENABLE_SDIO_INTERRUPT_MODE

///////////////////////////////////////////////////////////////////////////////////////////////////

static DECLARE_WAIT_QUEUE_HEAD(sdio_mie_wait);

static volatile U16 sdio_event;

typedef enum
{
	IRQ_TYPE_NONE	= 0,
	IRQ_TYPE_EVENT	= 1,
	IRQ_TYPE_D1INT	= 2,

} E_IRQ_TYPE;


void HalSdio_ClearWaitQueue(void)
{
	sdio_event = 0;
}

E_IRQ_TYPE HalSdio_SaveMieEvent(struct mmc_host *host)
{

	//U16 u16Reg = FCIE_RIU_R16(FCIE_MIE_EVENT);

#if defined SDIO_D1_INTR_MODE && SDIO_D1_INTR_MODE

	if(u16Reg & BIT_SDIO_INT)
	{
		mmc_signal_sdio_irq(host);
		FCIE_RIU_W16(FCIE_MIE_EVENT, BIT_SDIO_INT); // W1C event
		return IRQ_TYPE_D1INT;
	}
	else

#endif

	{
		//sdio_event |= u16Reg; // summary all mie event

		sdio_event = FCIE_RIU_R16(FCIE_MIE_EVENT) & FCIE_RIU_R16(FCIE_MIE_INT_EN);

		if(sdio_event & BIT_DMA_END)
		{
			FCIE_RIU_16_OF(FCIE_MIE_INT_EN, BIT_DMA_END);
		}

		if(sdio_event & BIT_SD_CMD_END)
		{
			FCIE_RIU_16_OF(FCIE_MIE_INT_EN, BIT_SD_CMD_END);
		}

		if(sdio_event & BIT_BUSY_END_INT)
		{
			FCIE_RIU_16_OF(FCIE_MIE_INT_EN, BIT_BUSY_END_INT);
		}

		if(sdio_event & BIT_ERR_STS)
		{
			FCIE_RIU_16_OF(FCIE_MIE_INT_EN, BIT_ERR_STS);
		}

		return IRQ_TYPE_EVENT;
	}

	return IRQ_TYPE_NONE;

}


irqreturn_t HalSdio_KernelIrq(int irq, void *devid)
{
	irqreturn_t irq_t = IRQ_NONE;
	struct mmc_host *host = devid;
	E_IRQ_TYPE eIrqType;

	//printk("SDIO IRQ EV_%04Xh, IE_%04Xh\n", FCIE_RIU_R16(FCIE_MIE_EVENT), FCIE_RIU_R16(FCIE_MIE_INT_EN));

	eIrqType = HalSdio_SaveMieEvent(host);

	if(eIrqType ==IRQ_TYPE_EVENT)
	{
		wake_up(&sdio_mie_wait);
		irq_t = IRQ_HANDLED;
	}
	else if(eIrqType ==IRQ_TYPE_D1INT) // no need to wake up wait queue head
	{
		irq_t = IRQ_HANDLED;
	}

	return irq_t;
}


E_IO_STS HalSdio_WaitMieEvent(U16 u16ReqVal, U32 u32WaitMs)
{
	unsigned long timeout;

	timeout = msecs_to_jiffies(u32WaitMs+10);

	if(wait_event_timeout(sdio_mie_wait, (sdio_event==u16ReqVal) || ( sdio_event&BIT_ERR_STS), timeout))
	{
		if(sdio_event&BIT_ERR_STS)
		{
			return IO_ERROR_DETECT;
		}
		return IO_SUCCESS;
	}
	else
	{
		printk("wait sdio mie evnet to req %04Xh, event = %04Xh\n", u16ReqVal, sdio_event);
		return IO_TIME_OUT;
	}

	return IO_SUCCESS;
}

#else

E_IO_STS HalSdio_WaitMieEvent(U16 u16ReqVal, U32 u32WaitMs)
{
	U32 u32Count = 0;
	//u32WaitMs *= 100;
	U16 u16Event;

	while(1)
	{
		u16Event = FCIE_RIU_R16(FCIE_MIE_EVENT);

		if(u16Event&BIT_ERR_STS)
		{
			/*if(!gu8SdioSilenceTuning)
			{
				printk("\33[1;31mSDIO ErrDet, SD_STS = %04Xh\33[m\n", FCIE_RIU_R16(FCIE_SD_STATUS));
			}*/
			HalSdio_Platform_ClearEvent(BIT_ERR_STS);
			return IO_ERROR_DETECT;
		}
		else if((u16Event&u16ReqVal)==u16ReqVal)
		{
			//printk("Got event %04X\n", u16Event);
			break;
		}

		HalSdioDelayUs(1000);

		u32Count++;

		if(u32Count>u32WaitMs)
		{
			printk("u32Counter=%d\r\n", u32Count);
			printk("\r\n");
			printk("------------------------------------------\r\n");
			printk("ERROR!!! MIE EVENT TIME OUT!!!\n");
			printk("request event = %04Xh, event = %04Xh\n", u16ReqVal, u16Event);
			printk("------------------------------------------\r\n");

			if(!gu8SdioSilenceTuning)
			{
				HalSdio_DumpRegister();
				HalSdio_DumpDebugBus();
				//while(1);
			}

			return(IO_TIME_OUT);
		}
	}

	///////////////////////////////////////////////////////////////////////////////////////////
	// Clear mie event
	// Only can clear request value,
	// because read operation might enable command and data transfer at the same time

	if (HalSdio_Platform_ClearEvent(u16ReqVal))
	{
		return (IO_TIME_OUT);
	}

	return(IO_SUCCESS);
} // wait MIE Event End

#endif

///////////////////////////////////////////////////////////////////////////////////////////////////

void HalSdioDelayUs(U32 u32Us)
{
	U32 u32_i, u32Ms;

	if(u32Us<1000)
	{
		udelay(u32Us);
	}
	else
	{
		u32Ms = u32Us/1000;
		for(u32_i=0; u32_i<u32Ms; u32_i++)
			udelay(1000);
	}
}

void HalSdioDelayMs(U32 u32Ms)
{
	U32 u32_i;

	for(u32_i=0; u32_i<u32Ms; u32_i++)
		udelay(1000);
}

void HalSdio_DelaySecond(U32 u32Sec)
{
	U32 u32_i;

	for(u32_i=0; u32_i<u32Sec; u32_i++)
		HalSdioDelayMs(1000);
}

#if 1 // uboot need

void HalSdio_TimerEnable(void)
{
	// reset HW timer
	FCIE_RIU_W16(TIMER1_MAX_LOW, 0xFFFF);
	FCIE_RIU_W16(TIMER1_MAX_HIGH, 0xFFFF);
	FCIE_RIU_W16(TIMER1_ENABLE, 0);

	// start HW timer
	FCIE_RIU_16_ON(TIMER1_ENABLE, 0x0001);

	// 0xFFFFFFFF = 4,294,967,295 tick
	// divide 12 --> 357,913,941 us --> 357 sec --> 6 min
}


U32 HalSdio_TimerGetTick(void)
{
	U32 u32HWTimer = 0;
	U16 u16TimerLow = 0;
	U16 u16TimerHigh = 0;

	// Get HW timer
	u16TimerLow = FCIE_RIU_R16(TIMER1_CAP_LOW);
	u16TimerHigh = FCIE_RIU_R16(TIMER1_CAP_HIGH);

	u32HWTimer = (u16TimerHigh<<16) | u16TimerLow;

	return u32HWTimer;
}

// max: 357,913,941 = 0x15555555
U32 HalSdio_TimerGetMicroSec(void)
{
	return (HalSdio_TimerGetTick()/12);
}

void HalSdio_TimerTest(void)
{
	unsigned int sec = 0;

	HalSdio_TimerEnable();
	printk("count to 3 then start test: ");
	while(1)
	{
		if (HalSdio_TimerGetMicroSec() >= (1000000+sec*1000000))
		{
			printk("%d ", ++sec);
		}
		if(sec==3)
		{
			printk("Go!\n");
			break;
		}
	}
}

static unsigned int tick_start;
static unsigned int tick_stop;

void HalSdio_TimerStart(void)
{
	HalSdio_TimerEnable();
	tick_start = HalSdio_TimerGetTick();
}

U32 HalSdio_TimerStop(void)
{
	tick_stop = HalSdio_TimerGetTick();
	return ((tick_stop - tick_start) / 12);
}

#endif

static int CDZ_option = 0;
static int CdzGpioBank = 0;
static int CdzGpioOffset = 0;
static int CdzGpioBits = 0;

static int pwr_option = 0;
static int pwrGpioBank = 0;
static int pwrGpioOffset = 0;
static int pwrGpioBits = 0;

static int wp_option = 0;
static int wpGpioBank = 0;
static int wpGpioOffset = 0;
static int wpGpioBits = 0;

#define	SBOOT_SIGNATURE		0x3800

#define CD_ALWAYS_REMOVE	0
#define CD_INSERT_L			1
#define CD_INSERT_H			2
#define CD_ALWAYS_INSERT	3

#define PC_ALWAYS_ON		0
#define PC_POWERON_L		1
#define PC_POWERON_H		2

#define WP_PROTECT_L		1
#define WP_PROTECT_H		2

#define BITNUM(X)  (X&0x00000001? 0:X&0x00000002? 1:X&0x00000004? 2:X&0x00000008? 3: \
					X&0x00000010? 4:X&0x00000020? 5:X&0x00000040? 6:X&0x00000080? 7: \
					X&0x00000100? 8:X&0x00000200? 9:X&0x00000400?10:X&0x00000800?11: \
					X&0x00001000?12:X&0x00002000?13:X&0x00004000?14:X&0x00008000?15: \
					X&0x00010000?16:X&0x00020000?17:X&0x00040000?18:X&0x00080000?19: \
					X&0x00100000?20:X&0x00200000?21:X&0x00400000?22:X&0x00800000?23: \
					X&0x01000000?24:X&0x02000000?25:X&0x04000000?26:X&0x08000000?27: \
					X&0x10000000?28:X&0x20000000?29:X&0x40000000?30:X&0x80000000?31:3697)

void HalSdio_GetSBootGPIOConfig(void)
{
	if ((FCIE_RIU_R16(FCIE_ZDEC_CTL0) & 0xff00)==SBOOT_SIGNATURE)
	{
		FCIE_RIU_W16(FCIE_SD_CTRL, 0x0080); // enable register access

		///////////////////////////////////////////////////////////////////////////////////////////
		// card detection
		CDZ_option=(FCIE_RIU_R16(FCIE_TEST_MODE) >> 8) & 0x3;
		if( (CDZ_option==CD_INSERT_L) || (CDZ_option==CD_INSERT_H) )
		{
			CdzGpioBank = FCIE_RIU_R16(CDZ_GPIO_BANK);
			CdzGpioOffset = FCIE_RIU_R16(CDZ_GPIO_OFFSET);
			CdzGpioBits=CdzGpioOffset &0xff;
			CdzGpioOffset=CdzGpioOffset >> 8;
		}
		///////////////////////////////////////////////////////////////////////////////////////////
		// power control
		pwr_option=(FCIE_RIU_R16(FCIE_TEST_MODE) >> 10) & 0x3;
		if( (pwr_option==PC_POWERON_L) || (pwr_option==PC_POWERON_H) )
		{
			pwrGpioBank=FCIE_RIU_R16(PWR_GPIO_BANK);
			pwrGpioOffset=FCIE_RIU_R16(PWR_GPIO_OFFSET);
			pwrGpioBits=pwrGpioOffset &0xff;
			pwrGpioOffset=pwrGpioOffset >> 8;
		}
		else
		{
			pwr_option = PC_ALWAYS_ON; // always power on
		}
		///////////////////////////////////////////////////////////////////////////////////////////
		// write protection
		wp_option = (FCIE_RIU_R16(FCIE_TEST_MODE) >> 12) & 0x3;
		if( (wp_option==WP_PROTECT_L) || (wp_option==WP_PROTECT_H) )
		{
			wpGpioBank=FCIE_RIU_R16(WP_GPIO_BANK);
			wpGpioOffset=FCIE_RIU_R16(FCIE_BOOT_CONFIG) >> 8;
			wpGpioBits=FCIE_RIU_R16(WP_GPIO_BITS) & 0x00FF;
		}
		else
		{
			wp_option = 0; // ex: t-flash socket
		}
		///////////////////////////////////////////////////////////////////////////////////////////

		FCIE_RIU_W16(FCIE_SD_CTRL, 0x0000); // disable register access

		printk("SDIO-CD(%d) 0x%04Xh[%02Xh]#%d\n", CDZ_option, CdzGpioBank, CdzGpioOffset, BITNUM(CdzGpioBits));
		printk("SDIO-PC(%d) 0x%04Xh[%02Xh]#%d\n", pwr_option, pwrGpioBank, pwrGpioOffset, BITNUM(pwrGpioBits));
		printk("SDIO-WP(%d) 0x%04Xh[%02Xh]#%d\n", wp_option, wpGpioBank, wpGpioOffset, BITNUM(wpGpioBits));

	} // check signature
	else
	{
		printk(LIGHT_CYAN"SDIO: no GPIO signature found\n"NONE);
	}

}

S32 HalSdio_GetCardDetect(void)
{
	if(CDZ_option==CD_ALWAYS_REMOVE) // always remove, not run
	{
		return 0;
	}
	else if(CDZ_option==CD_INSERT_L) // insert low case
	{
		if( FCIE_RIU_R8( RIU_BANK_2_BASE(CdzGpioBank) + ((CdzGpioOffset&0xFE)<<1) + (CdzGpioOffset&0x01) ) & CdzGpioBits )
			return 0;
		else
			return 1;
	}
	else if(CDZ_option==CD_INSERT_H) // insert high case
	{
		if( FCIE_RIU_R8( RIU_BANK_2_BASE(CdzGpioBank) + ((CdzGpioOffset&0xFE)<<1) + (CdzGpioOffset&0x01) ) & CdzGpioBits )
			return 1;
		else
			return 0;
	}
	else // always insert case, ex: wifi module
	{
		return 1;
	}
}

// 0: not write protect
// 1: write protect
S32 HalSdio_GetWriteProtect(void)
{
	if(wp_option==WP_PROTECT_L) // protect low case
	{
		if( FCIE_RIU_R8( RIU_BANK_2_BASE(wpGpioBank) + ((wpGpioOffset&0xFE)<<1) + (wpGpioOffset&0x01) ) & wpGpioBits )
		{
			return 0;
		}
		else
		{
			printk("write protected\n");
			return 1;
		}
	}
	else if(wp_option==WP_PROTECT_H) // protect high case
	{
		if( FCIE_RIU_R8( RIU_BANK_2_BASE(wpGpioBank) + ((wpGpioOffset&0xFE)<<1) + (wpGpioOffset&0x01) ) & wpGpioBits )
		{
			printk("write protected\n");
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}
}

void HalSdio_SetCardPower(U8 u8OnOff)
{
	if (pwr_option==PC_POWERON_L) // output low to turn on power case
	{
		if (u8OnOff)
			FCIE_RIU_8_OF( RIU_BANK_2_BASE(pwrGpioBank) + ((pwrGpioOffset&0xFE)<<1) + (pwrGpioOffset&0x01), pwrGpioBits);
		else
			FCIE_RIU_8_ON( RIU_BANK_2_BASE(pwrGpioBank) + ((pwrGpioOffset&0xFE)<<1) + (pwrGpioOffset&0x01), pwrGpioBits);
	}
	else if (pwr_option==PC_POWERON_H) // output high to turn on power case
	{
		if (u8OnOff)
			FCIE_RIU_8_ON( RIU_BANK_2_BASE(pwrGpioBank) + ((pwrGpioOffset&0xFE)<<1) + (pwrGpioOffset&0x01), pwrGpioBits);
		else
			FCIE_RIU_8_OF( RIU_BANK_2_BASE(pwrGpioBank) + ((pwrGpioOffset&0xFE)<<1) + (pwrGpioOffset&0x01), pwrGpioBits);
	}
	else
	{
		// always power on case
	}
}

void HalSdio_SetDrivingStrength(U16 u16DrivingLevel)
{
	printk("please porting me...\n");
}
