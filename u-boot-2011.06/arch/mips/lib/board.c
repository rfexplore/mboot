/*
 * (C) Copyright 2003
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>
#include <command.h>
#include <malloc.h>
#include <stdio_dev.h>
#include <timestamp.h>
#include <version.h>
#include <net.h>
#include <environment.h>
#include <nand.h>
#include <onenand_uboot.h>
#include <spi.h>
// MSTAR start
#include <MsInit.h>
#include <ShareType.h>
// MSTAR end
#ifdef CONFIG_BITBANGMII
#include <miiphy.h>
#endif

DECLARE_GLOBAL_DATA_PTR;

#undef DEBUG


extern int incaip_set_cpuclk(void);

extern ulong uboot_end_data;
extern ulong uboot_end;

extern int snprintf(char *str, size_t size, const char *fmt, ...);

ulong monitor_flash_len;

const char version_string[] =
	U_BOOT_VERSION" (" U_BOOT_DATE " - " U_BOOT_TIME ")";

static char *failed = "*** failed ***\n";

/*
 * mips_io_port_base is the begin of the address space to which x86 style
 * I/O ports are mapped.
 */
//unsigned long mips_io_port_base = -1;

int __board_early_init_f(void)
{
	/*
	 * Nothing to do in this dummy implementation
	 */
	return 0;
}
int board_early_init_f(void) __attribute__((weak, alias("__board_early_init_f")));


static int init_func_ram (void)
{
#ifdef	CONFIG_BOARD_TYPES
	int board_type = gd->board_type;
#else
	//int board_type = 0;	/* use dummy arg */
#endif
	puts ("DRAM:  ");
	if ((gd->ram_size = CONFIG_DRAM_SIZE_MB_RELOCATE * 0x100000 ) > 0) {
		print_size (gd->ram_size, "\n");
		return (0);
	}
	puts (failed);
	return (1);
}

// this code is more prior, so we can't use array to extern. it will be cleared
#if defined (CONFIG_POINTS_HANDLER_ENABLE)
#define CMD_BUF                 128
char aPrePoints[CMD_BUF] = "MB_start_ub";
unsigned int start_ub_time = 9;   // value: only in data section
#endif
static int display_banner(void)
{
    /* checkpoint for autotest boottime, plz dont remove it */
    extern unsigned long G_MS_BOOTTIME_SBOOT; // global variable for storing the boot time used in sboot (ms)
    unsigned int PiuTick = *(volatile unsigned int *)(0xBF006090);
    PiuTick |= (*(volatile unsigned int *)(0xBF006094)) << 16;
    unsigned int PiuTime = PiuTick / 12000;
    G_MS_BOOTTIME_SBOOT = (unsigned long)PiuTime;
    printf("[AT][MB][start ub][%u]", PiuTime);
#if defined (CONFIG_POINTS_HANDLER_ENABLE)
    start_ub_time = PiuTime;
#endif

    printf ("\n\n%s  %s\n\n", version_string,MBOOT_CL);
    return (0);
}

#ifndef CONFIG_SYS_NO_FLASH
static void display_flash_config(ulong size)
{
	puts ("Flash: ");
	print_size (size, "\n");
}
#endif

static int init_baudrate (void)
{
	char tmp[64];	/* long enough for environment variables */
	int i = getenv_f("baudrate", tmp, sizeof (tmp));

	gd->baudrate = (i > 0)
			? (int) simple_strtoul (tmp, NULL, 10)
			: CONFIG_BAUDRATE;

	return (0);
}


/*
 * Breath some life into the board...
 *
 * The first part of initialization is running from Flash memory;
 * its main purpose is to initialize the RAM so that we
 * can relocate the monitor code to RAM.
 */

/*
 * All attempts to come up with a "common" initialization sequence
 * that works for all boards and architectures failed: some of the
 * requirements are just _too_ different. To get rid of the resulting
 * mess of board dependend #ifdef'ed code we now make the whole
 * initialization sequence configurable to the user.
 *
 * The requirements for any new initalization function is simple: it
 * receives a pointer to the "global data" structure as it's only
 * argument, and returns an integer return code, where 0 means
 * "continue" and != 0 means "fatal error, hang the system".
 */
typedef int (init_fnc_t) (void);
extern int timer_init(void);

/* defined in uboot.lds */
extern char other_vector[];
extern char __heap[];
extern char __heap_end[];
extern char __boot_params[];
extern char __boot_params_end[];
extern char __stack_begin[];


init_fnc_t *init_sequence[] = {
	board_early_init_f,
	timer_init,
	env_init,		/* initialize environment */
#ifdef CONFIG_INCA_IP
	incaip_set_cpuclk,	/* set cpu clock according to environment variable */
#endif
	init_baudrate,		/* initialze baudrate settings */
	serial_init,		/* serial communications setup */
	console_init_f,
	display_banner,		/* say that we are here */
	//checkboard,
	init_func_ram,
	NULL,
};


void board_init_f(ulong bootflag)
{
	gd_t gd_data, *id;
	bd_t bd;
    init_fnc_t **init_fnc_ptr;
	ulong addr, addr_sp, len = (ulong)&uboot_end - CONFIG_SYS_MONITOR_BASE;
	ulong *s;

	/* Pointer is writable since we allocated a register for it.
	 */

	gd = &gd_data;

	/* compiler optimization barrier needed for GCC >= 3.4 */
	__asm__ __volatile__("": : :"memory");

	memset ((void *)gd, 0, sizeof (gd_t));

	for (init_fnc_ptr = init_sequence; *init_fnc_ptr; ++init_fnc_ptr) {
		if ((*init_fnc_ptr)() != 0) {
			hang ();
		}
	}

	/*
	 * Now that we have DRAM mapped and working, we can
	 * relocate the code and continue running from DRAM.
	 */
	addr = CONFIG_SYS_SDRAM_BASE + gd->ram_size;

	/* We can reserve some RAM "on top" here.
	 */

	/* round down to next 4 kB limit.
	 */
	addr &= ~(4096 - 1);
//	printf ("Top of RAM usable for U-Boot at: %08lx\n", addr);

	/* Reserve memory for U-Boot code, data & bss
	 * round down to next 16 kB limit
	 */
	addr -= len;
	addr &= ~(16 * 1024 - 1);
	addr = (ulong)other_vector;

        printf("\n\rHello U-Boot\n");
	debug ("Reserving %ldk for U-Boot at: %08lx\n", len >> 10, addr);

	 /* Reserve memory for malloc() arena.
	 */
	addr_sp = addr - TOTAL_MALLOC_LEN;
	debug ("Reserving %dk for malloc() at: %08lx\n",
			TOTAL_MALLOC_LEN >> 10, addr_sp);

	/*
	 * (permanently) allocate a Board Info struct
	 * and a permanent copy of the "global" data
	 */
	//addr_sp -= sizeof(bd_t);
	//bd = (bd_t *)addr_sp;
	gd->bd = &bd;
	debug ("Reserving %zu Bytes for Board Info at: %08lx\n",
			sizeof(bd_t), &bd);

	addr_sp -= sizeof(gd_t);
	id = (gd_t *)addr_sp;
	debug  ("Reserving %zu Bytes for Global Data at: %08lx\n",
			sizeof (gd_t), gd);

	/* Reserve memory for boot params.
	 */
	addr_sp -= CONFIG_SYS_BOOTPARAMS_LEN;
	bd.bi_boot_params = (ulong)__boot_params;
	debug ("Reserving %dk for boot params() at: %08lx\n",
			((ulong)__boot_params_end - (ulong)__boot_params) >> 10, (ulong)__boot_params);

	/*
	 * Finally, we set up a new (bigger) stack.
	 *
	 * Leave some safety gap for SP, force alignment on 16 byte boundary
	 * Clear initial stack frame
	 */
	addr_sp -= 16;
	addr_sp &= ~0xF;
	s = (ulong *)addr_sp;
	*s-- = 0;
	*s-- = 0;
	addr_sp = (ulong)s;
	printf ("Stack Pointer at: %08lx\n", (ulong)__stack_begin);

	/*
	 * Save local variables to board info struct
	 */
	bd.bi_memstart	= CONFIG_SYS_SDRAM_BASE;	/* start of  DRAM memory */
	bd.bi_memsize	= gd->ram_size;		/* size  of  DRAM memory in bytes */
	bd.bi_baudrate	= gd->baudrate;		/* Console Baudrate */

    /* marked because of removing relocation*/
	//memcpy (id, (void *)gd, sizeof (gd_t));

	addr = (ulong)other_vector;
	//relocate_code (addr_sp, gd, addr);
         board_init_r((gd_t *)gd,addr);

	/* NOTREACHED - relocate_code() does not return */
}
/************************************************************************
 *
 * This is the next part if the initialization sequence: we are now
 * running from RAM and have a "normal" C environment, i. e. global
 * data can be written, BSS has been cleared, the stack size in not
 * that critical any more, etc.
 *
 ************************************************************************
 */
unsigned int u32UbootStart = 0;
unsigned int u32UbootEnd = 0;

void board_init_r (gd_t *id, ulong dest_addr)
{
#ifndef CONFIG_SYS_NO_FLASH
	ulong size;
#endif
	extern void malloc_bin_reloc (void);
#ifndef CONFIG_ENV_IS_NOWHERE
	extern char * env_name_spec;
#endif
	ulong malloc_start;
	char *s;
	bd_t *bd;
	/* The Malloc area is immediately below the monitor copy in DRAM */
	malloc_start = dest_addr - TOTAL_MALLOC_LEN;
	printf("mem initial, start 0x%x, len 0x%x\n", (unsigned int)malloc_start, (unsigned int)TOTAL_MALLOC_LEN);
	mem_malloc_init (malloc_start, TOTAL_MALLOC_LEN);    
	malloc_bin_reloc();
    /*MSTAR start*/
    MstarSysInit();

    u32UbootStart = CONFIG_SYS_SDRAM_BASE+id->ram_size-16*0x100000;
    u32UbootEnd = CONFIG_SYS_SDRAM_BASE+id->ram_size;
    printf("uboot held at [%08x~%08x]\n",
            u32UbootStart,
            u32UbootEnd);
    /* MSTAR end*/

	gd = id;
	gd->flags |= GD_FLG_RELOC;	/* tell others: relocation done */

	printf("Now running in RAM - U-Boot at: %08lx\n", dest_addr);

	gd->reloc_off = dest_addr - CONFIG_SYS_MONITOR_BASE;

	monitor_flash_len = (ulong)&uboot_end_data - dest_addr;

#if defined(CONFIG_NEEDS_MANUAL_RELOC)
	/*
	 * We have to relocate the command table manually
	 */
	fixup_cmdtable(&__u_boot_cmd_start,
		(ulong)(&__u_boot_cmd_end - &__u_boot_cmd_start));
#endif /* defined(CONFIG_NEEDS_MANUAL_RELOC) */

	/* there are some other pointer constants we must deal with */
#ifndef CONFIG_ENV_IS_NOWHERE
	env_name_spec += gd->reloc_off;
#endif

	bd = gd->bd;

#ifndef CONFIG_SYS_NO_FLASH
	/* configure available FLASH banks */
	size = flash_init();
	display_flash_config (size);
	bd->bi_flashsize = size;
#endif

#if (ENABLE_MODULE_SPI == 1)
	bd->bi_flashstart = CONFIG_SYS_FLASH_BASE;
#if CONFIG_SYS_MONITOR_BASE == CONFIG_SYS_FLASH_BASE
	bd->bi_flashoffset = monitor_flash_len;	/* reserved area for U-Boot */
#else
	bd->bi_flashoffset = 0;
#endif
#endif

#ifdef CONFIG_CMD_NAND
	puts ("NAND:  ");
	nand_init ();		/* go init the NAND */
#endif

#if defined(CONFIG_CMD_ONENAND)
	onenand_init();
#endif

#if defined(CONFIG_GENERIC_MMC) && defined(CONFIG_ENV_IS_IN_MMC)
        puts("MMC:  ");
        mmc_initialize(bd);
#endif

	/* relocate environment function pointers etc. */
	env_relocate();

#if defined(CONFIG_GENERIC_MMC) && defined(CONFIG_ENV_IS_IN_SPI_FLASH)
        puts("MMC:  ");
        mmc_initialize(bd);
#endif

	/* IP Address */
	bd->bi_ip_addr = getenv_IPaddr("ipaddr");

#if defined(CONFIG_PCI)
	/*
	 * Do pci configuration
	 */
	pci_init();
#endif

/** leave this here (after malloc(), environment and PCI are working) **/
	/* Initialize stdio devices */
	stdio_init ();

	jumptable_init ();

#if (CONFIG_Second_UART_Mode_1 || CONFIG_Second_UART_Mode_2  || CONFIG_Second_UART_Mode_3 ||\
	CONFIG_Third_UART_Mode_1 || CONFIG_Third_UART_Mode_2  || CONFIG_Third_UART_Mode_3 ||\
	CONFIG_Fourth_UART_Mode_1 || CONFIG_Fourth_UART_Mode_2  || CONFIG_Fourth_UART_Mode_3)
//Do nothing
#else
	/* Initialize the console (after the relocation and devices init) */
	console_init_r ();
#endif
/** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** **/


	/* Initialize from environment */
	if ((s = getenv ("loadaddr")) != NULL) {
		load_addr = simple_strtoul (s, NULL, 16);
	}
#if defined(CONFIG_CMD_NET)
	if ((s = getenv ("bootfile")) != NULL) {
		copy_filename (BootFile, s, sizeof (BootFile));
	}
#endif

#ifdef CONFIG_CMD_SPI
	//puts ("SPI:   ");
	//spi_init ();		/* go init the SPI */
	//puts ("ready\n");
#endif

#if defined(CONFIG_MISC_INIT_R)
	/* miscellaneous platform dependent initialisations */
	//misc_init_r ();
#endif

#ifdef CONFIG_BITBANGMII
	bb_miiphy_init();
#endif
#if defined(CONFIG_CMD_NET)
#if defined(CONFIG_NET_MULTI)
	puts ("Net:   ");
#endif
	eth_initialize(gd->bd);
#endif

    // It will be opened after fix spi error Jerry.CP
    if(-1==MstarDrvInit())
    {
        printf("Error: MstarInit() \n");
    }
    timer_init();
#if CONFIG_MINIUBOOT
    run_command("spi ptinit" , 0);
    run_command("bootargs_set" , 0);
    run_command("panel_pre_init" , 0);
    run_command("bootlogo" , 0);
#endif

#if (ENABLE_MODULE_NAND_FLASH == 1)
	extern U32 nand_MTDMarkBad(U32 u32_GoodBlkIdx);
	extern int nand_ShiftBlock_Recover(U32 (*nand_markbad)(U32));
	nand_ShiftBlock_Recover(nand_MTDMarkBad);
#endif

	/* main_loop() can return to retry autoboot, if so just run it again. */
	for (;;) {
		main_loop ();
	}

	/* NOTREACHED - no way out of command loop except booting */
}

void hang (void)
{
	puts ("### ERROR ### Please RESET the board ###\n");
	for (;;);
}
