
#include <mhal_miu.h>
#include <MsTypes.h>
#include <drvMIU.h>



static void _MIU_Write2Bytes(U32 u32RegOffset, U16 u16Val)
{
    ((volatile U16*)(MIUREG_BASE))[u32RegOffset] = u16Val;
}

static const U16 client_gp[9] = {REG_MIUSEL_GP0, REG_MIUSEL_GP1, REG_MIUSEL_GP2, REG_MIUSEL_GP3, REG_MIUSEL_GP4, REG_MIUSEL_GP5, REG_MIUSEL_GP6, REG_MIUSEL_GP7, 0};

U8 HAL_MIU_SelectMIU_UBoot(U8 bMIU1, U16 *au16SelMiu, U8 u8GpNum)
{
    U8 idx;
    U32 u32RegAddr = 0;
    U32 u32RegArbbAddr = 0;

    if(bMIU1==0)
    {
        u32RegAddr =MIU0_REG_BASE;
        u32RegArbbAddr = MIU0_ARBB_REG_BASE;
    }
    else if(bMIU1==1)
    {
        u32RegAddr =MIU1_REG_BASE;
        u32RegArbbAddr = MIU1_ARBB_REG_BASE;
    }
    else if(bMIU1==2)
    {
        u32RegAddr =MIU2_REG_BASE;
        u32RegArbbAddr = MIU2_ARBB_REG_BASE;
    }

	for (idx = 0; idx < u8GpNum; idx++)
	{
        switch (idx)
        {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
            _MIU_Write2Bytes( ( u32RegAddr+client_gp[idx]), au16SelMiu[idx]);
            break;
        case 6:
        case 7:
            _MIU_Write2Bytes(u32RegArbbAddr+client_gp[idx], au16SelMiu[idx]);
            break;
        default:
            break;
        }
    }

    return TRUE;
}



static MS_U8 u8MIUProtectkernel_ID[16] = {
    MIU_CLIENT_MIPS_RW, //MIPS W
    MIU_CLIENT_NAND_RW, 
    MIU_CLIENT_USB_UHC0_RW, //USB0
    MIU_CLIENT_USB_UHC1_RW, //USB1
    MIU_CLIENT_USB_UHC2_RW, //USB2
    MIU_CLIENT_G3D_RW, //G3D
    MIU_CLIENT_USB3_RW, //USB3.0
    MIU_CLIENT_SDIO_RW, //SD IO
    MIU_CLIENT_SATA_RW, //SATA
    MIU_CLIENT_USB_UHC3_RW, //USB3
    MIU_CLIENT_USB30_1_RW, //USB3
    MIU_CLIENT_USB30_2_RW, //USB3
};


MS_U8* HAL_MIU_GetKernelProtectClients(void)
{
    return u8MIUProtectkernel_ID;
}


MS_U64 HAL_MIU_PA2BA(MS_U64 u64PhyAddr)
{
    MS_U64 u64BusAddr = 0x0;
    MS_U64 U64_MIU1_BUS = CONFIG_SYS_MIU1_BUS;

#if defined(CONFIG_SUPPORT_64BIT_PA)
U64_MIU1_BUS =0x200000000;
#endif

    // ba = pa + offset
    if( (u64PhyAddr >= 0) && (u64PhyAddr < CONFIG_SYS_MIU_INTERVAL) )     // MIU0
        u64BusAddr = u64PhyAddr  + CONFIG_SYS_MIU0_BUS;
    else if( (u64PhyAddr >= CONFIG_SYS_MIU_INTERVAL) && (u64PhyAddr < CONFIG_SYS_MIU2_INTERVAL) )        // MIU1
        u64BusAddr = u64PhyAddr - CONFIG_SYS_MIU_INTERVAL + U64_MIU1_BUS;
    else
        u64BusAddr = u64PhyAddr - CONFIG_SYS_MIU2_INTERVAL + CONFIG_SYS_MIU2_BUS;    // MIU2

    return u64BusAddr;
}

MS_U64 HAL_MIU_BA2PA(MS_U64 u64BusAddr)
{

    MS_U64 U64_MIU1_BUS = CONFIG_SYS_MIU1_BUS;

#if defined(CONFIG_SUPPORT_64BIT_PA)
U64_MIU1_BUS =0x200000000;
#endif

    if((u64BusAddr >= CONFIG_SYS_MIU0_BUS )&&(u64BusAddr<U64_MIU1_BUS ) )
    {
        return(u64BusAddr-CONFIG_SYS_MIU0_BUS);
    }
    else if ((u64BusAddr >= U64_MIU1_BUS )&&(u64BusAddr<CONFIG_SYS_MIU2_BUS ) )
    {
        return(u64BusAddr-U64_MIU1_BUS+CONFIG_SYS_MIU_INTERVAL);
    }
    else  if ((u64BusAddr >= CONFIG_SYS_MIU2_BUS )&&(u64BusAddr<=0x37FFFFFFF ) )
    {
        return(u64BusAddr-CONFIG_SYS_MIU2_BUS+CONFIG_SYS_MIU2_INTERVAL);
    }
    else
    {
        return 0;
    }
}
