
////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2007 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (��MStar Confidential Information��) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////
#include <common.h>
#include "usb.h"
#include "drvhostlib4.h"
#include "drvusbmain4.h"
#include <malloc.h>
#include "usb_defs.h"
#include "ehci_usb.h"

#if defined(ENABLE_FOURTH_EHC)
#define Scsi_Max_Transfer_Len	16*1024 //65536;;yuwen

 extern UINT8 OTGH_GETDESCRIPTOR_DEVICE_8[]  ;
 extern UINT8 OTGH_GETDESCRIPTOR_DEVICE[]    ;
 extern UINT8 OTGH_GETDESCRIPTOR_CONFIG[]  ;

 extern UINT8 OTGH_SETADDRESS[]  ;

 extern UINT8 OTGH_SETCONFIGURATION[];
 extern UINT8 OTGH_SET_FEATURE_OTG[] ;
 extern UINT8 OTGH_SETDESCRIPTOR_DEVICE[] ; //Set Device Descriptor

 UINT8 QtdBufP3[Host20_qTD_SIZE*Host20_qTD_MAX+0x20*4] __attribute__ ((aligned (128)));

 qHD_Structure  Host20_qHD_List_Control0_P3 __attribute__ ((aligned (128)));
 qHD_Structure  Host20_qHD_List_Control1_P3 __attribute__ ((aligned (128)))  ;
 qHD_Structure  Host20_qHD_List_Bulk0_P3 __attribute__ ((aligned (128)));
 qHD_Structure  Host20_qHD_List_Bulk1_P3 __attribute__ ((aligned (128))) ;

 qHD_Structure *pHost20_qHD_List_Control0_P3,*pHost20_qHD_List_Control1_P3;
 qHD_Structure *pHost20_qHD_List_Bulk0_P3,*pHost20_qHD_List_Bulk1_P3;



UINT8      Host20_qTD_Manage_P3[Host20_qTD_MAX];        //1=>Free 2=>used
//Host20_Attach_Device_Structure   sAttachDevice  ;

//Host20_Attach_Device_Structure   *psDevice_AP;
UINT8 *pUsbCtrlBufP3;
UINT8 UsbCtrlBufP3[0x100] __attribute__ ((aligned (128)));
UINT8  pUsbDataP3[128]  __attribute__ ((aligned (128))) ;
//volatile UINT32 gwLastqTDSendOK;
UINT8 *Host20_STRUCTURE_qTD_BASE_ADDRESS_P3,*Host20_STRUCTURE_qHD_BASE_ADDRESS_P3;
UINT8 *qTD_Base_Buf_P3;
UINT8  bSpeedP3,bSendStatusErrorP3;
int  gTotalBytesP3;

int gInQHDArrayNumP3;
int gOutQHDArrayNumP3;
int FirstBulkInP3,FirstBulkOutP3;

//extern UINT8 XBYTE_READ(UINT32 Addr, UINT8 offset);
//extern void XBYTE_SET(UINT32 Addr, UINT8 offset,UINT8 val);
extern void Chip_Flush_Memory(void);
extern void Chip_Read_Memory(void);
//====================================================================
// * Function Name: OTGH_PT_Bulk_Init
// * Description:
// * Input: none
// * OutPut: none
//====================================================================
void USB_Bulk_Init4(struct usb_device *dev)
{
    struct usb_endpoint_descriptor *ep,*ep2;

#if (ENABLE_MSTAR_TITANIA_BD_MST090F_C01A)    // should refine it later
       ep=&dev->config.if_desc[0].ep_desc[1];
       ep2=&dev->config.if_desc[0].ep_desc[2];
#else
       ep=&dev->config.if_desc[0].ep_desc[0];
       ep2=&dev->config.if_desc[0].ep_desc[1];
#endif

	printf("bulk max packet size: ep %x ep2 %x\n", ep->wMaxPacketSize, ep2->wMaxPacketSize);

   //<5>.Hook the qHD
       if ((ep->bmAttributes)==OTGH_ED_BULK)
          {//<5.1>.stop Asynchronous Schedule
             flib_Host20_Asynchronous_Setting4(HOST20_Disable);

               	//<5.2>.Hook the qHD for ED0~ED3
           //       SetPointer(&(pHost20_qHD_List_Control1_P2->bNextQHDPointer3),VirtoPhyAddr((UINT16)pHost20_qHD_List_Bulk0_P2));
           //       SetPointer(&(pHost20_qHD_List_Bulk0_P2->bNextQHDPointer3),VirtoPhyAddr((UINT16)pHost20_qHD_List_Bulk1_P2));
            //      SetPointer(&(pHost20_qHD_List_Bulk1_P2->bNextQHDPointer3),VirtoPhyAddr((UINT16)&Host20_qHD_List_Control0));
                  pHost20_qHD_List_Control1_P3->bNextQHDPointer=(VA2PA((UINT32)pHost20_qHD_List_Bulk0_P3)>>5);
                  pHost20_qHD_List_Bulk0_P3->bNextQHDPointer=(VA2PA((UINT32)pHost20_qHD_List_Bulk1_P3)>>5);
                  pHost20_qHD_List_Bulk1_P3->bNextQHDPointer=(VA2PA((UINT32)pHost20_qHD_List_Control0_P3)>>5);




           //<5.2>.Enable Asynchronous Schedule
           flib_Host20_Asynchronous_Setting4(HOST20_Enable);

           }
        FirstBulkInP3=1;
        FirstBulkOutP3=1;




       //Setting Max Packet Size and print message
         pHost20_qHD_List_Bulk0_P3->bMaxPacketSize=ep->wMaxPacketSize;
         pHost20_qHD_List_Bulk1_P3->bMaxPacketSize=ep2->wMaxPacketSize;
         pHost20_qHD_List_Bulk0_P3->bDeviceAddress=dev->devnum;
         pHost20_qHD_List_Bulk1_P3->bDeviceAddress=dev->devnum;
         pHost20_qHD_List_Bulk0_P3->bEdNumber=(ep->bEndpointAddress) & 0x0f; //set endpoint address
         pHost20_qHD_List_Bulk1_P3->bEdNumber=(ep2->bEndpointAddress) & 0x0f; //set endpoint address


         //For Bulk-Out = 2K bytes
  #if 0

#endif
        // wTemp=sOTGH_PT_BLK->pbDataPage_In[0];
         //for (i=0;i<(sOTGH_PT_BLK->wDataRange);i++)
             // *(wTemp+i)=0x88;

	if (((ep->bEndpointAddress)&0x80)>0)
       {
         gInQHDArrayNumP3=0;//Array 0           //for in 0, for out,1
         gOutQHDArrayNumP3=1;//Array 0
       }
    else
      {
         gInQHDArrayNumP3=1;//Array 0           //for in 0, for out,1
         gOutQHDArrayNumP3=0;//Array 0
       }


}
//************************************************************************************************************
//************************************************************************************************************
//                          *** Group-1:Main Function ***
//*************************************************************************************************************
//************************************************************************************************************
unsigned char mwHost20Port4(int bOffset)
{
    if (bOffset & 1)
        return(*((UINT8 volatile   *) ( Host20_BASE_ADDRESS+bOffset*2-1)));
    else                                                                                                                //even
       return(*((UINT8 volatile   *) ( Host20_BASE_ADDRESS+bOffset*2)));
}
void mwHost20Port_wr4(int bOffset,int value)
{
    if (bOffset & 1)
        (*((UINT8 volatile   *) ( Host20_BASE_ADDRESS+bOffset*2-1)))=value;
    else                                                                                                                //even
        (*((UINT8 volatile   *) ( Host20_BASE_ADDRESS+bOffset*2)))=value;
}

void mwHost20Portw4(int bOffset,int value)             //bOffset should be 32 bits alignment
{
        *((UINT32 volatile   *)(Host20_BASE_ADDRESS+bOffset*2))=value & 0xffff;
        *((UINT32 volatile   *)(Host20_BASE_ADDRESS+bOffset*2+4))=(value>>16) & 0xffff;

}
int  mwHost20Bit_Rd4(int bByte,int wBitNum)
{
   return (mwHost20Port4(bByte)&wBitNum);
}
void  mwHost20Bit_Set4(int bByte,int wBitNum)
{
    UINT8 temp;
    temp=mwHost20Port4(bByte);
    temp|=wBitNum;
    mwHost20Port_wr4(bByte,temp);

}
void  mwHost20Bit_Clr4(int bByte,int wBitNum)
{
    UINT8 temp;
    temp=mwHost20Port4(bByte);
    temp&=~wBitNum;

    mwHost20Port_wr4(bByte,temp);
}

//====================================================================
// * Function Name: flib_OTGH_Init
// * Description: Init the Host HW and prepare the ED/TD
//   <1>.Init All the Data Structure
//       <1.1>.Build control list
//       <1.2>.Build Bulk list
//       <1.3>.Build Interrupt list
//       <1.4>.Build ISO list (Reserved)
//   <2>.Reset the chip
//   <3>.Set HW register
//       <3.1>.Enable FIFO-Enable(0x100->Bit5) & FPGA-Mode-Half-Speed (0x100->Bit1)
//       <3.2>.Enable interrupts
//       <3.3>.Periodic List Base Address register
//       <3.4>.USBCMD (Interrupt/Threshod/Frame List/Run-Stop)
//
// * Input: wTimeWaitForConnect_ms:The time of waitting for connecting
// * OutPut: 0:Device do not connect
//           1:Host init ok
//           2:Bus Rest Fail
//====================================================================
#define   wTimeWaitForConnect_ms 9500

UINT8 flib_OTGH_Init4(UINT8 wForDevice_B, UINT32 wDelay)
{
   UINT8 wValue=0;
   UINT16 wTimer_ms=0;


   USB_DELAY(wDelay);
   if (mwHost20_PORTSC_ConnectStatus_Rd() == 0)
   {
    printf("No USB is connecting\n");
    return (0);
   }

   //<1>.Waiting for the Device connect
#if 1
   if (wForDevice_B==0) {
      mbHost20_USBCMD_HCReset_Set();            //host controller reset
      while(mbHost20_USBCMD_HCReset_Rd()>0);
   }
#endif

   wValue=0;
   wTimer_ms=0;
   do {
      wValue=mwHost20_PORTSC_ConnectStatus_Rd();
          //  printf("wValue:%02bx\n",wValue);

      if (wValue==0) {
         USB_DELAY(1);//10, wait 1 ms
         wTimer_ms++;
      }
      //if (mwHost20_PORTSC_ConnectStatus_Rd()==0) return 0;
      if (wTimer_ms>wTimeWaitForConnect_ms)          // Case1:Waiting for 10 sec=10000
      {                                           // Case2:Waiting for 100 ms =100
         printf("??? Waiting for Peripheral Connecting Fail...\n");
         return (0);
      }
   }while(wValue==0);



   mwHost20_Misc_EOF1Time_Set(Host20_EOF1Time);
   //printf("34:%02bx\n",XBYTE[0x2434]);
   //<2>.Init All the Data Structure & write Base Address register
   flib_Host20_InitStructure4();

   //Write Base Address to Register
  // Host20_STRUCTURE_BASE_ADDRESS
   //printf("Async base addr: 0x%x \n", (UINT32)Host20_STRUCTURE_qHD_BASE_ADDRESS_P3);

   mwHost20_CurrentAsynchronousAddr_Set(VA2PA((UINT32)Host20_STRUCTURE_qHD_BASE_ADDRESS_P3));
   //printf("Reg 0x28: 0x%x 0x%x\n", *((UINT32 volatile   *)(Host20_BASE_ADDRESS+0x28*2)), *((UINT32 volatile   *)(Host20_BASE_ADDRESS+0x28*2+4)));

   if (wForDevice_B==0) {
	  if (flib_Host20_PortBusReset4()>0)
         return(2);
   }
 if (bSpeedP3==0)                 //full speed
  {

    //XBYTE[UTMIBaseAddr+0x09]&=0x7F;
       UTMI_WRREG_ODD8(0x9,UTMI_RDREG_ODD8(0x9)&(0x7f));

    mwHost20_Misc_EOF1Time_Set(2);

  }
  else if (bSpeedP3==2)                //high speed
  {
    //XBYTE[UTMIBaseAddr+0x09]|=0x80;			   //HS rx robust enable
     UTMI_WRREG_ODD8(0x9,UTMI_RDREG_ODD8(0x9)|(0x80));

    mwHost20_Misc_EOF1Time_Set(3);

  }
   flib_Host20_QHD_Control_Init4();
 // printf("return 1");

   return (1);
}


//====================================================================
// * Function Name: flib_Host20_Close
// * Description:
//   <1>.Suspend Host
//   <2>.Disable the interrupt
//   <3>.Clear Interrupt Status
//   <4>.Issue HW Reset
//   <5>.Free the Allocated Memory
// * Input:
// * OutPut:
//====================================================================
UINT8 flib_Host20_Close4(UINT8 bForDeviceB)
{
   UINT32 wTemp;

   if (mwHost20_USBINTR_Rd()>0) {
      //<1>.Suspend Host
      if (bForDeviceB==0)
        {
         flib_Host20_Suspend4();
        }
      else
		 flib_Host20_StopRun_Setting4(HOST20_Disable);

      //<2>.Disable the interrupt
      mwHost20_USBINTR_Set(0);

      //<3>.Clear Interrupt Status
      wTemp=mwHost20_USBSTS_Rd();
      wTemp=wTemp&0x0000003F;
      mwHost20_USBSTS_Set(wTemp);
   }

   return (1);
}

//====================================================================
// * Function Name: flib_Host20_StopRun_Setting
// * Description:
// * Input:
// * OutPut:
//====================================================================
void flib_Host20_StopRun_Setting4(UINT8 bOption)
{
   if (bOption==HOST20_Enable) {
      if (mbHost20_USBCMD_RunStop_Rd()>0)
         return;

      mbHost20_USBCMD_RunStop_Set();

      while(mbHost20_USBCMD_RunStop_Rd()==0);
   }
   else if (bOption==HOST20_Disable) {
      if (mbHost20_USBCMD_RunStop_Rd()==0)
         return;

      mbHost20_USBCMD_RunStop_Clr();

      while(mbHost20_USBCMD_RunStop_Rd()>0);
   }
   else {
	  printf("??? Input Error 'flib_Host20_StopRun_Setting'...");
      while(1);
   }
}

//====================================================================
// * Function Name: flib_Host20_Asynchronous_Setting
// * Description:
// * Input:
// * OutPut:
//====================================================================
void flib_Host20_Asynchronous_Setting4(UINT8 bOption)
{
   if (bOption==HOST20_Enable) {
      if (mwHost20_USBSTS_AsynchronousStatus_Rd()>0)
         return;

      mbHost20_USBCMD_AsynchronousEnable_Set();

      while(mwHost20_USBSTS_AsynchronousStatus_Rd()==0);
   }
   else if (bOption==HOST20_Disable) {
      if (mwHost20_USBSTS_AsynchronousStatus_Rd()==0)
         return;

      mbHost20_USBCMD_AsynchronousEnable_Clr();

      while(mwHost20_USBSTS_AsynchronousStatus_Rd()>0);
   }
   else {
	  printf("??? Input Error 'flib_Host20_Asynchronous_Setting'...\n");
      // while(1);
   }
}

//void _noop_()
//{
   //  int i=0;
//}
//====================================================================
// * Function Name: flib_Host20_PortBusReset
// * Description:
//   <1>.Waiting for HCHalted=0
//   <2>.Write PortEnable=0(Reserved for Faraday-IP)
//   <3>.Write PortReset=0
//   <4>.Wait time
//   <5>.Write PortReset=0
//   <6>.Waiting for IRS->PortChangeDetect
// * Input:
// * OutPut:
//====================================================================
UINT8 flib_Host20_PortBusReset4(void)
{
//   UINT8 ttt;
  UINT32 wTmp;
//   flib_Host20_ForceSpeed(0);

   //<1>.Disable RunStop

// printf("disable run\n");
   if (mbHost20_USBCMD_RunStop_Rd()>0)
      flib_Host20_StopRun_Setting4(HOST20_Disable);
//   printf("0x10 ->%02bx \n",XBYTE[0x2410]);

   //<2>.Write PortReset=0

   UTMI_WRREG8(0x2C, 0x10);
   UTMI_WRREG_ODD8(0x2D, 0);
   UTMI_WRREG8(0x2E, 0x00);
   UTMI_WRREG_ODD8(0x2F, 0);
   UTMI_WRREG8(0x2A, 0x80);    //Chirp K detection level: 0x80 => 400mv, 0x20 => 575mv

   mwHost20_PORTSC_PortReset_Set();

   //<3>.Wait time=>55ms
 //  flib_Host20_TimerEnable(55);
   USB_DELAY(50);
#if (ENABLE_NAPOLI)
	   UTMI_WRREG8(0x2A, 0x60);  // adjust UTMI disconnect level to 625 mv for 28 nm process
#else
	   UTMI_WRREG8(0x2A, 0x00);    
#endif   
   USB_DELAY(20);
   // flib_Debug_LED_Off_All();; //GPIO-High

   //<4>.Write PortReset=0
  // sAttachDevice.bPortReset=1;
 //temporarily marked  ,yuwen
   mwHost20_PORTSC_PortReset_Clr();

  UTMI_WRREG8(0x2C, 0x10);
  UTMI_WRREG_ODD8(0x2D, 0x02);
  UTMI_WRREG_ODD8(0x2F, 0x81);

  // printf("Host Speed:%02bx\n",mwOTG20_Control_HOST_SPD_TYP_Rd());

   //<5>.Waiting for IRS->PortChangeDetect
  //printf("wait reset\n");
  #if 1
   wTmp=0;

   while (1)
   {
      if (mwHost20_PORTSC_PortReset_Rd()==0)  break;

      //MsOS_DelayTask(4);
      //if (mwHost20_PORTSC_ConnectStatus_Rd()==0) return 1;
      wTmp++;
      if (wTmp>20000)
	{
         printf("??? Error waiting for Bus Reset Fail...==> Reset HW Control\n");
         mbHost20_USBCMD_HCReset_Set();
         while(mbHost20_USBCMD_HCReset_Rd()==1);
         return (1);
       }
   }
   #endif
   //<6>.Enable RunStop Bit
#if 0
   if (mwHost20_PORTSC_ForceSuspend_Rd())
    {
        printf("port suspend\n");
        mwHost20_PORTSC_ForceResume_Set();          //force resume
        USBDELAY(14);
        mwHost20_PORTSC_ForceResume_Clr();
    }
#endif

 // XBYTE[UTMIBaseAddr+0x06]|=0x03;        //reset UTMI
 // XBYTE[UTMIBaseAddr+0x06]&=0xfc;
    UTMI_WRREG8(6,UTMI_RDREG8(6)|0x03);
    UTMI_WRREG8(6,UTMI_RDREG8(6)&0xfc);

          flib_Host20_StopRun_Setting4(HOST20_Enable);

      USB_DELAY(5);		//wait some slow device to be ready
   //<7>.Detect Speed
    bSpeedP3= mwOTG20_Control_HOST_SPD_TYP_Rd();
    printf("Host type:%x \n",  bSpeedP3);
   //<8>.Delay 20 ms
   return (0);
}

//====================================================================
// * Function Name: flib_Host20_Suspend
// * Description:
//   <1>.Make sure PortEnable=1
//   <2>.Write PORTSC->Suspend=1
//   <3>.Waiting for the ISR->PORTSC->Suspend=1
// * Input:
// * OutPut: 0:OK
//           1:Fail
//====================================================================
UINT8 flib_Host20_Suspend4(void)
{
   if (mbHost20_USBCMD_RunStop_Rd()==0)
      return(1);

   //<1>.Make sure PortEnable=1
   if (mwHost20_PORTSC_EnableDisable_Rd()==0)
      return(1);

   //<2>.Write PORTSC->Suspend=1
   flib_Host20_StopRun_Setting4(HOST20_Disable);//For Faraday HW request

   //<3>.Write PORTSC->Suspend=1
   mwHost20_PORTSC_ForceSuspend_Set();

   //<4>.Waiting for the ISR->PORTSC->Suspend=1
#if 0
   flib_Host20_TimerEnable_UnLock(1);//1sec
   bExitFlag=0;

   do {
      if (mwHost20_PORTSC_ForceSuspend_Rd()>0)
         bExitFlag=1;

      if (gwOTG_Timer_Counter>5) {
		 bExitFlag=1;
         printf(">>> Fail => Time Out for Waiting ForceSuspend...\n");
      }
   }
   while(bExitFlag==0);
#else
  while(mwHost20_PORTSC_ForceSuspend_Rd()==0);
#endif

   return (0);
}





//====================================================================
 //For Control-Single qTD// * Function Name: flib_Host20_Issue_Control
// * Description:
//   <1>.Analysis the Controller Command => 3 type
//   <2>.Case-1:"Setup/In/Out' Format..."
//       (get status/get descriptor/get configuration/get interface)
//   <3>.Case-2:'Setup/In' Format...      => Faraday Driver will not need
//       (clear feature/set feature/set address/set Configuration/set interface  )
//   <4>.Case-3:'Setup/Out/In'
//       (set descriptor)
// * Input:
// * OutPut: 0: OK
//           X:>0 => Fail
//====================================================================
UINT8 flib_Host20_Issue_Control4(UINT8 bEdNum,UINT8* pbCmd,UINT16 hwDataSize,UINT8* pbData)
{
   qTD_Structure  *spTempqTD;
//   UINT32       bpDataPage;
   UINT8        bReturnValue;
 // UINT8 i;
 qHD_Structure  *qh_ptr;
//  UINT8 i;

if (bEdNum==0)
     qh_ptr=pHost20_qHD_List_Control0_P3;
else
{
    pHost20_qHD_List_Control1_P3->bDeviceAddress = bEdNum;
    qh_ptr=pHost20_qHD_List_Control1_P3;
}


   //<0>.Allocate qTD & Data Buffer
   spTempqTD=flib_Host20_GetStructure4(Host20_MEM_TYPE_qTD);//0=>qTD
   if(spTempqTD==NULL){
        return HOST20_FATAL;
    }
  // bpDataPage=sAttachDevice.bDataBuffer;           //2//2k buffer

   //<2.1>.Setup packet
   //<A>.Fill qTD
   spTempqTD->bPID=HOST20_qTD_PID_SETUP;                   //Bit8~9
   spTempqTD->bTotalBytes=8;           //Bit16~30

   spTempqTD->bDataToggle=0;            //Bit31
  //  FillBufferArray(spTempqTD, bpDataPage);
   memcpy(pUsbCtrlBufP3,pbCmd,8);


   spTempqTD->ArrayBufferPointer_Word[0]=VA2PA((UINT32)pUsbCtrlBufP3);

  // printf("bpDataPage:%x\n",bpDataPage);
//   memcpy((UINT8  *)bpDataPage,pbCmd,8);

#if 1
   //<B>.Send qTD
  //  Dump_QTD(spTempqTD);

  //  Dump_Data(0x2400,0x50);
 //  bReturnValue=flib_Host20_Send_qTD(spTempqTD ,psHost20_qHD_List_Control[bEdNum],5);
     bReturnValue=flib_Host20_Send_qTD4(spTempqTD ,qh_ptr,Host20_Timeout);

   if (bReturnValue>0)
   {
        printf("flib_Host20_Send_qTD4 fail !\n");
               goto exit_issue_control;
   }

 //  printf("c3");

   //<1>.Analysis the Controller Command
   switch (*(pbCmd+1)) { // by Standard Request codes
      // <2>.Case-1:"Setup/In/Out' Format..."
   	  case 0:		// get status
	  case 6:		// get descriptor
	  case 8:		// get configuration
	  case 10:	// get interface
	  case 0xfe:        //get Max Lun
         //<2.2>.In packet
    SetupRead:
         //<A>.Fill qTD
         spTempqTD=flib_Host20_GetStructure4(Host20_MEM_TYPE_qTD);//0=>qTD
         if(spTempqTD==NULL){
            return HOST20_FATAL;
         }
         spTempqTD->bPID=HOST20_qTD_PID_IN;                   //Bit8~9
         spTempqTD->bTotalBytes=hwDataSize;           //Bit16~30

         spTempqTD->bDataToggle=1;            //Bit31

       //  spTempqTD->ArrayBufferPointer_Word[0]=VirtoPhyAddr(bpDataPage);
         //FillBufferArray(spTempqTD, bpDataPage);
         spTempqTD->ArrayBufferPointer_Word[0]=VA2PA((UINT32)pUsbCtrlBufP3);


         //<B>.Send qTD
         //bReturnValue=flib_Host20_Send_qTD(spTempqTD ,psHost20_qHD_List_Control[bEdNum],5);
              bReturnValue=flib_Host20_Send_qTD4(spTempqTD ,qh_ptr,Host20_Timeout);

         if (bReturnValue>0)
                   goto exit_issue_control;


         //<C>.Waiting for result
         memcpy((UINT8  *)pbData,pUsbCtrlBufP3,hwDataSize);

     //    Dump_Data((UINT16)pbData,hwDataSize);

         //<2.3>.Out packet

         //<A>.Fill qTD
         spTempqTD=flib_Host20_GetStructure4(Host20_MEM_TYPE_qTD);//0=>qTD
         if(spTempqTD==NULL){
            return HOST20_FATAL;
         }
         spTempqTD->bPID=HOST20_qTD_PID_OUT;                   //Bit8~9
         spTempqTD->bTotalBytes=0;           //Bit16~30

         spTempqTD->bDataToggle=1;            //Bit31

         //<B>.Send qTD
       //  bReturnValue=flib_Host20_Send_qTD(spTempqTD ,psHost20_qHD_List_Control[bEdNum],5);
            bReturnValue=flib_Host20_Send_qTD4(spTempqTD ,qh_ptr,Host20_Timeout);

         if (bReturnValue>0)
                   goto exit_issue_control;


         break;

      //<3>.Case-2:'Setup/In' Format...      => Faraday Driver will not need
	  case 1:		// clear feature
	  case 3:		// set feature
   	  case 5:		// set address
   	  case 0xf0:   //read
   	  case 0xf1:    //write
   	      if (*(pbCmd)==0xc0)  goto SetupRead;
		  else if (*(pbCmd)==0x40)  goto SetupWrite;
   	  case 9:		// set Configuration
      case 11:	// set interface
         case 0xff:             //device reset

         //<3.2>.In packet

         //<A>.Fill qTD
         spTempqTD=flib_Host20_GetStructure4(Host20_MEM_TYPE_qTD);//0=>qTD
         if(spTempqTD==NULL){
            return HOST20_FATAL;
         }
         spTempqTD->bPID=HOST20_qTD_PID_IN;                   //Bit8~9
         spTempqTD->bTotalBytes=hwDataSize;           //Bit16~30
         spTempqTD->bDataToggle=1;            //Bit31
         spTempqTD->ArrayBufferPointer_Word[0]=VA2PA((UINT32)pUsbCtrlBufP3);
         //FillBufferArray(spTempqTD, bpDataPage);

         //<B>.Send qTD
        // bReturnValue=flib_Host20_Send_qTD(spTempqTD ,psHost20_qHD_List_Control[bEdNum],5);
             bReturnValue=flib_Host20_Send_qTD4(spTempqTD ,qh_ptr,Host20_Timeout);

         if (bReturnValue>0)
           goto exit_issue_control;
         //<C>.Copy Result
        // memcpy(pbData,pUsbCtrlBuf,hwDataSize);
         break;

      //<4>.Case-3:'Setup/Out/In'
      case 7:		// set descriptor
         //<4.2>.Out packet
         //<A>.Fill qTD
       SetupWrite:
         spTempqTD=flib_Host20_GetStructure4(Host20_MEM_TYPE_qTD);//0=>qTD
         if(spTempqTD==NULL){
            return HOST20_FATAL;
          }
         spTempqTD->bPID=HOST20_qTD_PID_OUT;                   //Bit8~9
         spTempqTD->bTotalBytes=hwDataSize;           //Bit16~30
         spTempqTD->bDataToggle=1;            //Bit31
         spTempqTD->ArrayBufferPointer_Word[0]=(UINT32)pUsbCtrlBufP3;
        // FillBufferArray(spTempqTD, bpDataPage);

         memcpy(pUsbCtrlBufP3,pbData,hwDataSize);

         //<B>.Send qTD
      //   bReturnValue=flib_Host20_Send_qTD(spTempqTD ,psHost20_qHD_List_Control[bEdNum],5);
           bReturnValue=flib_Host20_Send_qTD4(spTempqTD ,qh_ptr,Host20_Timeout);

         if (bReturnValue>0)
                     goto exit_issue_control;


         //<4.3>.In packet
         //<A>.Fill qTD
         spTempqTD=flib_Host20_GetStructure4(Host20_MEM_TYPE_qTD);//0=>qTD
         if(spTempqTD==NULL){
            return HOST20_FATAL;
         }
         spTempqTD->bPID=HOST20_qTD_PID_IN;                   //Bit8~9
         spTempqTD->bTotalBytes=0;           //Bit16~30
         spTempqTD->bDataToggle=1;            //Bit31

         //<B>.Send qTD
       //  bReturnValue=flib_Host20_Send_qTD(spTempqTD ,psHost20_qHD_List_Control[bEdNum],5);
            bReturnValue=flib_Host20_Send_qTD4(spTempqTD ,qh_ptr,Host20_Timeout);

         if (bReturnValue>0)
                       goto exit_issue_control;

      break;

      default:
   	      if ( *(pbCmd) & 0x80 )  goto SetupRead;
		  else if ( !(*(pbCmd) & 0x80) )  goto SetupWrite;
 	     break;
   }
   #endif
   return (0);
exit_issue_control:

        return (bReturnValue);

}
UINT8 Send_Receive_Bulk_Data4(void *buffer,int len,int dir_out)
{

 // UINT8 *pbDataPage[5];
  UINT32 wTotalLengthRemain;
  UINT32 buf;
  UINT8 result=0;
  UINT16  TransferLen;
//printf("data phase\n");

  //<1>.To fill the data buffer

      wTotalLengthRemain=len;

      buf=(UINT32)buffer;


  //<2>.Issue Transfer

  while (wTotalLengthRemain)
    {
        if(wTotalLengthRemain > Scsi_Max_Transfer_Len)
                    TransferLen=Scsi_Max_Transfer_Len;
        else
                    TransferLen=wTotalLengthRemain;

           if (dir_out)
           {

	        result=flib_Host20_Issue_Bulk4 (gOutQHDArrayNumP3,TransferLen
	                         ,buf,OTGH_Dir_Out);

               if (result!=HOST20_OK) return result;

           }
            else
            {

	       result=flib_Host20_Issue_Bulk4 (gInQHDArrayNumP3 ,TransferLen
	                         ,buf,OTGH_Dir_IN);

               if (result!=HOST20_OK) return result;
             }
           buf+=TransferLen;
           wTotalLengthRemain-=TransferLen;

    }
 return(result);

}


//====================================================================
// * Function Name: flib_Host20_Issue_Bulk
// * Description: Input data must be 4K-Alignment
//               <1>.MaxSize=20 K
//               <2>.Support Only 1-TD
// * Input:
// * OutPut:
//====================================================================
//#ifdef MS_NOSAPI
//UINT8 usb_temp_buf[Scsi_Max_Transfer_Len] __attribute__ ((aligned (32)));
//#endif
UINT8  flib_Host20_Issue_Bulk4 (UINT8 bArrayListNum,UINT16 hwSize,UINT32 pwBuffer,UINT8 bDirection)
{
 qTD_Structure  *spTempqTD;
 UINT8 bTemp,i;
 UINT16 count;
 UINT32 addr;
 UINT32 mybuf,workbuf/*,tempbuf*/;
int    TranSize;
  qHD_Structure   *spTempqH;

     mybuf=0;
     //tempbuf=0;
     if ( pwBuffer !=(UINT32) KSEG02KSEG1(pwBuffer) )
  	{
  	#if 0
  		 if (pwBuffer & 0x7)			//flush should be 8 bytes aligned
  		 {
  		//	printf(" buf ");
  		   //  #ifndef MS_NOSAPI
  			 tempbuf=(UINT32)malloc(hwSize);  //temporarily buffer for USB control
  			 mybuf=(UINT32) KSEG02KSEG1(tempbuf);         //uncached buffer
  		     //#else
                   //   mybuf=(UINT32) KSEG02KSEG1(usb_temp_buf);
                 //  #endif
       		  if ( tempbuf == 0 )  printf("usb memory allocate failed!\n");

  		}
		 else
            #endif
		{
		//    printf(" fuh ");
  		  //  dcache_flush_range((void*)pwBuffer,hwSize);		//flush buffer to uncached buffer
  		  //marked , dcache not ready yet
   		   flush_cache((ulong)pwBuffer,hwSize);		//flush buffer to uncached buffer
                #if (_USB_FLUSH_BUFFER == 1)
                Chip_Flush_Memory();
                #endif
  		    pwBuffer=(UINT32)KSEG02KSEG1(pwBuffer);
		}
  	}
     #if 0
	 else
	 {
		  if (pwBuffer & 0x7)
		 	 {
		 	 //   printf(" buf ");

  			 tempbuf=(UINT32)malloc(hwSize );  //temporarily buffer for USB control
  		        mybuf=(UINT32) KSEG02KSEG1(tempbuf);         //uncached buffer
       		  if ( tempbuf == 0 )  printf("usb memory allocate failed!\n");

		  	}
         }
   #endif

        spTempqTD =flib_Host20_GetStructure4(Host20_MEM_TYPE_qTD); //The qTD will be release in the function "Send"
        if(spTempqTD==NULL){
            return HOST20_FATAL;
        }
        spTempqTD->bTotalBytes=hwSize ;

      //    FillBufferArray2(spTempqTD,pwBufferArray,hwSize);         //use global buffer , because it is 4k alignment
#if 0
       spTempqTD->ArrayBufferPointer_Word[0]=pwBufferArray;
	if (((pwBufferArray&0xfff)+hwSize) > 0xfff)     //goto page 2
          spTempqTD->ArrayBufferPointer_Word[1]=pwBufferArray+0x1000; 		//+4K
	if (hwSize > (0x1000+0x1000-(pwBufferArray&0xfff))
#endif
//not allow buffer over 16K for my usage
   //if (mybuf==0)            //use original buf
     workbuf=pwBuffer;
   /*else
        workbuf=mybuf;*/

    spTempqTD->ArrayBufferPointer_Word[0] = VA2PA(workbuf);

     count = 0x1000 - (workbuf & 0x0fff);  /* rest of that page */
     if ( hwSize < count)    /* ... iff needed */
	    count = hwSize;
     else
     {
         workbuf +=  0x1000;
    	  workbuf &= ~0x0fff;

    /* per-qtd limit: from 16K to 20K (best alignment) */
        for (i = 1; (count < hwSize) && (i < 5); i++)
	   {
      		addr = workbuf;
      		spTempqTD->ArrayBufferPointer_Word[i] = VA2PA(addr);
//      		lastnums++;
      		workbuf += 0x1000;
      		if ((count + 0x1000) < hwSize)
        	   count += 0x1000;
      		else
        	    count = hwSize;
    	   }

  }
    if (bArrayListNum==0)
        {
        spTempqH=pHost20_qHD_List_Bulk0_P3;
       // printf("use bulk 0\n");
        }
  else  // if (bArrayListNum==1)
    {
        //printf("use bulk 1\n");
        spTempqH=pHost20_qHD_List_Bulk1_P3;
    }

  //<2>.Analysis the Direction
          if (bDirection==OTGH_Dir_IN)
            {
                 spTempqTD->bPID=HOST20_qTD_PID_IN;
                 if (FirstBulkInP3)
  		    {
			spTempqTD->bDataToggle=0;
			spTempqH->bDataToggleControl=1;
  		    }
            }
          else
          {
                  spTempqTD->bPID=HOST20_qTD_PID_OUT;
                  if (FirstBulkOutP3)
  		    {
			spTempqTD->bDataToggle=0;
			spTempqH->bDataToggleControl=1;

  		    }
		 //   if (mybuf)  memcpy((void*)mybuf,(void*)pwBuffer,hwSize);	//copy buffer
          }
  //<3>.Send TD
//  if (bArrayListNum==0)
   //     bTemp=flib_Host20_Send_qTD(spTempqTD ,pHost20_qHD_List_Bulk0_P2,Host20_Timeout);
 // else if (bArrayListNum==1)
        bTemp=flib_Host20_Send_qTD4(spTempqTD ,spTempqH,Host20_Timeout);

        if ((FirstBulkInP3)&&(bDirection==OTGH_Dir_IN))
        {
 	        spTempqH->bDataToggleControl=0;
	        FirstBulkInP3=0;
        }
        if ((FirstBulkOutP3)&&(bDirection==OTGH_Dir_Out))
        {
 	        spTempqH->bDataToggleControl=0;
	        FirstBulkOutP3=0;
        }

       if (bDirection==OTGH_Dir_IN)
       {
       #if 1
                 TranSize=hwSize-gTotalBytesP3;
                 if ((TranSize % 8)==7)
                 {
                        //printf("patch wallace bug\n");
                        *((UINT8*)workbuf+TranSize-1)=*((UINT8*)workbuf+TranSize+3); //fix hardware bug
                 }
                 else if ((TranSize % 8)==3)
                 {
                        //printf("patch wallace bug\n");
                        *((UINT8*)workbuf+TranSize-1)=*((UINT8*)workbuf+TranSize+7); //fix hardware bug
                 }
      #endif
		  //  if (mybuf)
                 // {
                         //   printf("copy buffer\n");
				//memcpy((void*)pwBuffer,(void*)mybuf,TranSize);	//copy buffer

                    //}

       }

      // if (tempbuf)
	   //free((void*)tempbuf);

	 //  printf("Z");
        return (bTemp);

}









//************************************************************************************************************
//************************************************************************************************************
//                          *** Group-4:Structure Function ***
//*************************************************************************************************************
//************************************************************************************************************
//====================================================================
// * Function Name: flib_Host20_InitStructure
// * Description:
//              1.Init qHD for Control
//                qHD_C-->qHD_C-->qHD_C
//              2.Init qHD for Bulk
//                |-------------------------|
//                qHD_C-->qHD_C-->qHD_B-->qHD_B
//
//              3.Init qHD for Interrupt
//              4.Init iTD for ISO (Reserved for feature)
// * Input:Type =0 =>iTD
//              =1 =>qTD
//              =2
// * OutPut: 0:Fail
//           1:ok
//====================================================================
void flib_Host20_InitStructure4(void)
{
   UINT16   i;
   UINT8 *pData;

   //<1>.Clear memory

      // XBYTE[Host20_STRUCTURE_qHD_BASE_ADDRESS+i]=0x00;

    //for ( i=0 ; i < Host20_qTD_SIZE*Host20_qTD_MAX ; i++)
     //  XBYTE[Host20_STRUCTURE_qTD_BASE_ADDRESS+i]=0x00;

    pUsbCtrlBufP3= (UINT8 *)KSEG02KSEG1(UsbCtrlBufP3);

   // ASSERT(pUsbCtrlBuf != NULL)

    qTD_Base_Buf_P3= (UINT8 *)KSEG02KSEG1(QtdBufP3);
    //ASSERT(qTD_Base_Buf != NULL)
    Host20_STRUCTURE_qTD_BASE_ADDRESS_P3=qTD_Base_Buf_P3;

    pData=qTD_Base_Buf_P3;
    for (i=0 ; i < (Host20_qTD_SIZE*Host20_qTD_MAX+0x20) ; i++)
        pData[i]=0;

    if ((UINT32)Host20_STRUCTURE_qTD_BASE_ADDRESS_P3& 0x10)
		Host20_STRUCTURE_qTD_BASE_ADDRESS_P3+=0x10;			//make it aligned with 32
    pHost20_qHD_List_Control0_P3=(qHD_Structure*)KSEG02KSEG1(&Host20_qHD_List_Control0_P3);
    pHost20_qHD_List_Control1_P3=(qHD_Structure*)KSEG02KSEG1(&Host20_qHD_List_Control1_P3);
    pHost20_qHD_List_Bulk0_P3=(qHD_Structure*)KSEG02KSEG1(&Host20_qHD_List_Bulk0_P3);
    pHost20_qHD_List_Bulk1_P3=(qHD_Structure*)KSEG02KSEG1(&Host20_qHD_List_Bulk1_P3);

    pData=(UINT8*)pHost20_qHD_List_Control0_P3;
    for ( i=0 ; i < sizeof(qHD_Structure); i++)
        pData[i]=0;
    pData=(UINT8*)pHost20_qHD_List_Control1_P3;
    for ( i=0 ; i < sizeof(qHD_Structure); i++)
        pData[i]=0;
    pData=(UINT8*)pHost20_qHD_List_Bulk0_P3;
    for ( i=0 ; i < sizeof(qHD_Structure); i++)
        pData[i]=0;
    pData=(UINT8*)pHost20_qHD_List_Bulk1_P3;
    for ( i=0 ; i < sizeof(qHD_Structure); i++)
        pData[i]=0;

    Host20_STRUCTURE_qHD_BASE_ADDRESS_P3=(UINT8*)pHost20_qHD_List_Control0_P3;

    //printf("qhd:%x\n",(UINT32)pHost20_qHD_List_Control0_P3);
    //printf("qhd1:%x\n",(UINT32)pHost20_qHD_List_Control1_P3);
    //printf("qtd:%x\n",(UINT32)qTD_Base_Buf_P3);
   //<2>.For qTD & iTD & 4K-Buffer Manage init
   for (i=0;i<Host20_qTD_MAX;i++)
	  Host20_qTD_Manage_P3[i]=Host20_MEM_FREE;


//   sAttachDevice.bDataBuffer=flib_Host20_GetStructure(Host20_MEM_TYPE_4K_BUFFER);//For Control


  //
 // printf("List_control 0:%x\n",(UINT16) pHost20_qHD_List_Control0_P2);
#if 1
//psHost20_qHD_List_Control[0]->bType=HOST20_HD_Type_QH;
   flib_Host20_Allocate_QHD4(pHost20_qHD_List_Control0_P3,HOST20_HD_Type_QH,0,1,0,64);//Address=0,Head=1,EndPt=0,Size

   flib_Host20_Allocate_QHD4(pHost20_qHD_List_Control1_P3,HOST20_HD_Type_QH,1,0,0,64);//Address=1,Head=0,EndPt=0,Size

#endif



#if 1
   flib_Host20_Allocate_QHD4(pHost20_qHD_List_Bulk0_P3,HOST20_HD_Type_QH,1,0,1,64);//Address=1,Head=0,EndPt=1,Size
   flib_Host20_Allocate_QHD4(pHost20_qHD_List_Bulk1_P3,HOST20_HD_Type_QH,1,0,2,64);//Address=1,Head=0,EndPt=2,Size
#endif


   //<3.3>.Link the qHD (contol)
   #if 1

 pHost20_qHD_List_Control0_P3->bNextQHDPointer=(VA2PA((UINT32)pHost20_qHD_List_Control1_P3)>>5);
 pHost20_qHD_List_Control1_P3->bNextQHDPointer=(VA2PA((UINT32)pHost20_qHD_List_Control0_P3)>>5);

  // SetPointer(&(pHost20_qHD_List_Control0_P2->bNextQHDPointer3),VirtoPhyAddr((UINT16)pHost20_qHD_List_Control1_P2));
    //SetPointer(&(pHost20_qHD_List_Control1_P2->bNextQHDPointer3),VirtoPhyAddr((UINT16)pHost20_qHD_List_Control0_P2));
   // SetPointer(&(Host20_qHD_List_Work.bNextQHDPointer3),VirtoPhyAddr((UINT16)&Host20_qHD_List_Work));

   #endif

}

//====================================================================
// * Function Name: flib_Host20_GetStructure
// * Description:
//
// * Input:Type =0 =>qTD
//              =1 =>iTD
//              =2 =>4K Buffer
// * OutPut: 0:Fail
//           ~0:Addrress
//====================================================================
qTD_Structure *flib_Host20_GetStructure4(UINT8 Type)
{
 UINT32 i;
 UINT8 bFound;
// UINT16 spTempqTD;
 qTD_Structure   *spTempqTD;
// iTD_Structure  *spTempiTD;
//  siTD_Structure  *spTempsiTD;
 bFound=0;

 switch(Type)
       {
       	case Host20_MEM_TYPE_qTD:

       	     //For qTD

               for (i=0;i<Host20_qTD_MAX;i++)
                   if (Host20_qTD_Manage_P3[i]==Host20_MEM_FREE)
                        {bFound=1;
                         Host20_qTD_Manage_P3[i]=Host20_MEM_USED;
                         break;
                        }


               if (bFound==1)
                  {
                     //printf("USB base:%lx  \n",USB_BUFFER_START_ADR);

                   spTempqTD=(qTD_Structure    *)((UINT32)Host20_STRUCTURE_qTD_BASE_ADDRESS_P3+i*Host20_qTD_SIZE);
                   memset((unsigned char   *)spTempqTD ,0, Host20_qTD_SIZE);
                   spTempqTD->bTerminate=1;         //Bit0
                   spTempqTD->bStatus_Active=0;             //Bit7
                   spTempqTD->bInterruptOnComplete=1;   //Bit15

                   spTempqTD->bAlternateTerminate=1;
                   spTempqTD->bErrorCounter=3;
                 	return (spTempqTD);
                   }
               else printf("QTD underrun!\n");



       	break;

       	default:
       	       return 0;
       	break;

       	}

 //Not Found...


 return (0);

}
//====================================================================
// * Function Name: flib_Host20_ReleaseStructure
// * Description:
//
// * Input:Type =0 =>qTD
//              =1 =>iTD
//              =2
// * OutPut: 0:Fail
//           ~0:Addrress
//====================================================================
void flib_Host20_ReleaseStructure4(UINT8 Type,UINT32 pwAddress)
{
 UINT8 i;
 UINT16 wReleaseNum;
 UINT8  *pData;

//printf("release QTD:%x\n",pwAddress);
 pData=(UINT8*)pwAddress;

  switch(Type)
        {
        case Host20_MEM_TYPE_qTD:

             if (pwAddress<(UINT32)Host20_STRUCTURE_qTD_BASE_ADDRESS_P3)
                 {
                 printf("??? Memory release area fail...\n");
                 return;
                 }

             if ((pwAddress-(UINT32)Host20_STRUCTURE_qTD_BASE_ADDRESS_P3)==0)
                wReleaseNum=0;
             else
                wReleaseNum=(pwAddress-(UINT32)Host20_STRUCTURE_qTD_BASE_ADDRESS_P3)/Host20_qTD_SIZE;

			if (wReleaseNum>=Host20_qTD_MAX)//ERROR FIX Prevent Tool 070522
                 {
                 printf("??? Memory release area fail...\n");
                 return;
                 }


             Host20_qTD_Manage_P3[wReleaseNum]=Host20_MEM_FREE;

             for (i=0;i<Host20_qTD_SIZE ;i++) //qTD size=32 bytes
                  *(pData+i)=0;


        break;



        }


}
//====================================================================
// * Function Name: flib_Host20_QHD_Control_Init
// * Description:
//
// * Input:Type =0 =>qTD
//              =1 =>iTD
//              =2
// * OutPut: 0:Fail
//           ~0:Addrress
//====================================================================
void flib_Host20_QHD_Control_Init4(void)
{


  //<1>.Init Control-0/1
   pHost20_qHD_List_Control0_P3->bEdSpeed= bSpeedP3;
 // printf("bEdSpeed:%bx\n",sAttachDevice.bSpeed);
     pHost20_qHD_List_Control0_P3->bInactiveOnNextTransaction=0;
     pHost20_qHD_List_Control0_P3->bDataToggleControl=1;

     pHost20_qHD_List_Control1_P3->bEdSpeed= bSpeedP3;
     pHost20_qHD_List_Control1_P3->bInactiveOnNextTransaction=0;
     pHost20_qHD_List_Control1_P3->bDataToggleControl=1;

 //<2>.Init Bulk-0/1
    pHost20_qHD_List_Bulk0_P3->bEdSpeed= bSpeedP3;
     pHost20_qHD_List_Bulk0_P3->bInactiveOnNextTransaction=0;
     pHost20_qHD_List_Bulk0_P3->bDataToggleControl=0;

     pHost20_qHD_List_Bulk1_P3->bEdSpeed= bSpeedP3;
     pHost20_qHD_List_Bulk1_P3->bInactiveOnNextTransaction=0;
     pHost20_qHD_List_Bulk1_P3->bDataToggleControl=0;


//printf("enable aynch \n");
   //<12>.Enable Asynchronous

         mbHost20_USBCMD_AsynchronousEnable_Set();    //Temp;;Bruce
  //      printf("0x10:%02x",XBYTE[0x2410]);


}
void SetControl1MaxPacket4(UINT8 max)
{
       pHost20_qHD_List_Control1_P3->bMaxPacketSize=max;

}
//====================================================================
// * Function Name: flib_Host20_Allocate_QHD
// * Description:
//
// * Input:Type =0 =>qTD
//              =1 =>iTD
//              =2
// * OutPut: 0:Fail
//           ~0:Addrress
//====================================================================
void flib_Host20_Allocate_QHD4(qHD_Structure  *psQHTemp,UINT8 bNextType,UINT8 bAddress,UINT8 bHead,UINT8 bEndPt, UINT32 wMaxPacketSize)
{
   qTD_Structure  *spTempqTD;
  // UINT32 sp;

   psQHTemp->bTerminate=0;             //Bit0
   psQHTemp->bType=bNextType;          //Bit2~1

   psQHTemp->bDeviceAddress=bAddress;             //Bit0~6
   psQHTemp->bEdNumber=bEndPt;                  //Bit11~8
   psQHTemp->bHeadOfReclamationListFlag=bHead; //Bit15
   psQHTemp->bMaxPacketSize=wMaxPacketSize;            //Bit16~26
   psQHTemp->bNakCounter=Host20_QHD_Nat_Counter;

   psQHTemp->bOverlay_NextTerminate=1;
   psQHTemp->bOverlay_AlternateNextTerminate=1;

   //<2>.allocate dumy qTD
#if 1
   //<2.1>.Allocate qTD
   spTempqTD=flib_Host20_GetStructure4(0);//0=>qTD
     psQHTemp->bOverlay_NextqTD=(VA2PA((UINT32)spTempqTD)>>5);

   #endif
   //<2.3>.Active the qTD
   psQHTemp->bOverlay_NextTerminate=0;

}
unsigned long  gUsbStatusP3;

//====================================================================
// * Function Name: flib_Host20_CheckingForResult_QHD
// * Description:
// * Input:Type
// * OutPut:
//====================================================================
UINT8 flib_Host20_CheckingForResult_QHD4(qHD_Structure *spTempqHD)
{
    UINT8 bQHStatus;
    UINT16 wIntStatus;

    wIntStatus=mwHost20_USBSTS_Rd();
    //  printf("USB int:%x\n",wIntStatus);
   gUsbStatusP3=0;
    //<2>.Checking for the Error type interrupt => Halt the system
    if (wIntStatus&HOST20_USBINTR_SystemError)
    {
         //printf("???System Error... Halt the system...\n ");
      bSendStatusErrorP3=1;

    }
    if (wIntStatus&HOST20_USBINTR_USBError) {
        //printf("usb intr err\n");
        mwHost20_USBSTS_USBError_Set();
      bSendStatusErrorP3=1;

    }

     if (bSendStatusErrorP3==0)
        return HOST20_OK;
    //<1>.Analysis the qHD Status
    bSendStatusErrorP3=0;
    //  printf("??? USB Error Interrupt Event...\n");
    bQHStatus=spTempqHD->bOverlay_Status;
    // printf("bQHStatus:%x\n",bQHStatus);
	//while (1) {}				//stop to check ehci status
    if (bQHStatus&HOST20_qTD_STATUS_Halted)
    {
        spTempqHD->bOverlay_Status=0;      //clear halt status
        spTempqHD->bOverlay_TotalBytes=0;
        spTempqHD->bOverlay_Direction=0;
        if (bQHStatus==HOST20_qTD_STATUS_Halted)            //no other error status
        {
            //printf("STALL\n");
                    gUsbStatusP3|=USB_ST_STALLED;
            return HOST20_DEVICE_STALL;
        }
        else
            printf("qHD Status => Halted (Stall/Babble/Error Counter=0)...0x%x\n", bQHStatus);
    }
    if (bQHStatus&HOST20_qTD_STATUS_BufferError)
    {
        gUsbStatusP3|=USB_ST_BUF_ERR;
        //printf("??? qHD Status => HOST20_qTD_STATUS_BufferError...\n");
    }
    if (bQHStatus&HOST20_qTD_STATUS_Babble)
    {
        printf("!!! qHD Status => HOST20_qTD_STATUS_Babble !!! \n");
        //sAttachDevice.bSendStatusError=0;			//don't fix babble error for Bert
        spTempqHD->bOverlay_Status=0;
        gUsbStatusP3|=USB_ST_BABBLE_DET;

        return HOST20_OK;
    }
    if (bQHStatus&HOST20_qTD_STATUS_TransactionError)
    {
        //printf("??? qHD Status => HOST20_qTD_STATUS_TransactionError...\n");
                gUsbStatusP3|=USB_ST_CRC_ERR;
    }
    if (bQHStatus&HOST20_qTD_STATUS_MissMicroFrame)
    {
        //printf("??? qHD Status => HOST20_qTD_STATUS_MissMicroFrame...\n");
             gUsbStatusP3|=USB_ST_BIT_ERR;
    }

    //<2>.Clear the status
    spTempqHD->bOverlay_Status=0;

    return HOST20_FAIL;
}


#if 1
//====================================================================
// * Function Name: flib_Host20_Send_qTD
// * Description:
//   Case-1:1qTD
//   Case-2:2qTD
//   Case-3:3qTD above
// * Input:Type
// * OutPut: 0 => OK
//           1 => TimeOut
//====================================================================
UINT8 flib_Host20_Send_qTD4(qTD_Structure  *spHeadqTD ,qHD_Structure  *spTempqHD,int wTimeOutSec)
{
   UINT8  bReturnValue;
   qTD_Structure  *spNewDumyqTD;
   qTD_Structure  *spOldDumyqTD;
   qTD_Structure  *spReleaseqTD;
   qTD_Structure  *spReleaseqTDNext;
   qTD_Structure  *spLastqTD;
  // UINT32 wDummyTemp;
//   UINT32  *pwData;
   unsigned long wTimes;
 //  UINT32 StartTime;
//   MS_BOOL   wait;
//   UINT32                 UINT32Events = 0;

  if (wTimeOutSec==0)
	 	printf("error, timeout sec is zero\n");
   //spOldDumyqTD=GetPointer(&(spTempqHD->bOverlay_NextqTD3));
   spOldDumyqTD=(qTD_Structure*)PA2VA(((UINT32)(spTempqHD->bOverlay_NextqTD))<<5);

  // spTempqHD->bOverlay_Status|=HOST20_qTD_STATUS_Halted;
  #if 0
  while (mbHost20_USBCMD_AsynchronousEnable_Rd())
  	{
  	mbHost20_USBCMD_AsynchronousEnable_Clr();//pause asynchronous scheduler
  	}
  #endif
//printf("U");
 //  spHeadqTD->bTerminate=1;			//set to terminate
   memcpy(spOldDumyqTD,spHeadqTD,Host20_qTD_SIZE);

   //<2>.Prepare new dumy qTD
   spNewDumyqTD=spHeadqTD;
   memset((unsigned char  *)spNewDumyqTD ,0, Host20_qTD_SIZE);
   spNewDumyqTD->bTerminate=1;
  // spNewDumyqTD->bAlternateTerminate=1;
   //spNewDumyqTD->bStatus_Halted=1;
   //<3>.Find spLastqTD & link spLastqTD to NewDumyqTD & Set NewDumyqTD->T=1
   spLastqTD=spOldDumyqTD;
   while(spLastqTD->bTerminate==0) {
	  spLastqTD=(qTD_Structure*)PA2VA((((UINT32)(spLastqTD->bNextQTDPointer))<<5));
   };
   spLastqTD->bNextQTDPointer= VA2PA((UINT32)spNewDumyqTD)>>5;
   spLastqTD->bTerminate=0;

   //Link Alternate qTD pointer
  // SetPointer(&(spLastqTD->bAlternateQTDPointer3),VirtoPhyAddr((UINT32)spNewDumyqTD));
   spLastqTD->bAlternateQTDPointer= VA2PA((UINT32)spNewDumyqTD)>>5;

   spLastqTD->bAlternateTerminate=0;

   //sAttachDevice.psSendLastqTD=spLastqTD;
   bSendStatusErrorP3=0;


    //while (mwHost20_USBSTS_Rd() & 0x3f)
 	{
   		mwHost20_USBSTS_Set(0x3f);      //clear interrupt
 	}
   //bExitLoop=0;
   //spOldDumyqTD->bStatus_Halted=0;
   //spOldDumyqTD->bStatus_Halted=0;
   spOldDumyqTD->bStatus_Active=1;

#if (_USB_FLUSH_BUFFER == 1)
    Chip_Flush_Memory();
#endif
 // while (1){}
#if 0
   while (mbHost20_USBCMD_AsynchronousEnable_Rd()==0)
   	{
         mbHost20_USBCMD_AsynchronousEnable_Set();//re start asynchronous scheduler
   	}
 #endif
   //   spTempqHD->bOverlay_Status&=~HOST20_qTD_STATUS_Halted;
  //wait until asynchronous scheduler is idle

   //mbHost20_USBCMD_RunStop_Set();

  // spTempqHD->bOverlay_Status&=~0x40;			//clr HALT bit, start this queue head
//XBYTE[0x2410]|=1;//start run
  // wDummyTemp=0;
   wTimes=0;
#if  0
   StartTime=MsOS_GetSystemTime();
   //<5>.Waiting for result
   //printf("wait flag");
   wait= MsOS_WaitEvent(_s32UsbEventId, USBWAITEVENTS, &UINT32Events, E_OR_CLEAR, wTimeOutSec*1000);
  // printf("ok");
//    if (UINT32Events & HOST20_USBINTR_CompletionOfTransaction)
    bReturnValue=HOST20_OK;
    if (wait==FALSE) bReturnValue=HOST20_FATAL;     //timeout
    if (UINT32Events & HOST20_USBINTR_PortChangeDetect)
        {
            printf("port change int\n");
           bReturnValue=HOST20_FATAL;
        }
 #else              //non interrupt
    wTimes=0;
   while (1)
    {
           flush_cache((ulong)spOldDumyqTD,0x10);       //wried, seems like cache has bug,
                                                                                 //CPU always keep its own copy,
                                                                                 //even we use non-cache memory
        if ( mwHost20_USBSTS_Rd() & HOST20_USBINTR_CompletionOfTransaction )
        {
        #if (_USB_FLUSH_BUFFER == 1)
            Chip_Read_Memory();
        #endif
            if (spOldDumyqTD->bStatus_Active==0)
            {
            	bReturnValue=HOST20_OK;
                break;
            }
        }

    //     if (bExitLoop) break;
          if (mwHost20_USBSTS_Rd()&(HOST20_USBINTR_SystemError+HOST20_USBINTR_USBError))
            {
                  bReturnValue=HOST20_OK;
                  break;            //USB interrupt happened
            }
        // if (gwOTG_Timer_Counter>wTimeOutSec)
      wTimes++;
      //
      if (mwHost20_PORTSC_ConnectStatus_Rd()==0)
      {
          printf("device disconn\n");
          gUsbStatusP3 |= USB_ST_DISCONNECT;
          bReturnValue = HOST20_FATAL;
          break;
      }

      wait_ms(1);


     // if ((MsOS_GetSystemTime()-StartTime) > wTimeOutSec *1000)
       if (wTimes > wTimeOutSec*1000)
        {
         //   bExitLoop=2;
            bReturnValue=HOST20_FATAL;
            gUsbStatusP3 |= USB_ST_TIMEOUT;
            //mbHost20_USBCMD_HCReset_Set();            //host controller reset
               //  while(mbHost20_USBCMD_HCReset_Rd()>0);

            printf(">>> Fail => Time Out for Send qTD...\n");
            break;
         }

    }
#endif
    gTotalBytesP3=spOldDumyqTD->bTotalBytes;
   //<6>.Checking the Result
   if (bReturnValue!=HOST20_FATAL)
      bReturnValue=flib_Host20_CheckingForResult_QHD4(spTempqHD);


   //<5>.Release the all the qTD (Not include spNewDumyqTD)
   #if 1
   spReleaseqTD=spOldDumyqTD;
   do {
      spReleaseqTDNext=(qTD_Structure*)PA2VA(((UINT32)(spReleaseqTD->bNextQTDPointer))<<5);

      flib_Host20_ReleaseStructure4(Host20_MEM_TYPE_qTD,(UINT32)spReleaseqTD);
      spReleaseqTD=spReleaseqTDNext;
   } while(((UINT32)spReleaseqTD)!=((UINT32)spNewDumyqTD));
#endif

    //<6>.Double Check the QH overlay status. Adjust it if need.
    if( spTempqHD->bOverlay_NextqTD != (VA2PA(( UINT32)spNewDumyqTD)>>5)
        || spTempqHD->bOverlay_Status & BIT7 )
    {
        // Bug of WD 3.0 disk. Disconnection happens while data face of SETUP transaction.
        // If chain qTDs after disconnection/timeout, QH overlay will not be advanced by HW.
        // It muss up qTD chain layout. QH doesn't stop at old dumy and stare at new dumy.
        // SW advance QH overlay manually no matter HW advancing or not.
        // Run bit is cleared by HQ when disconnection, so it is safe to modify the QH.

        printf("[Warning] Adjust bad qTD chain..\r\n");

        spTempqHD->bOverlay_Status = spTempqHD->bOverlay_Status & ~BIT7;
        spTempqHD->bOverlay_CurrentqTD = VA2PA(( UINT32)spOldDumyqTD) >> 5;
        spTempqHD->bOverlay_NextqTD = VA2PA(( UINT32)spNewDumyqTD) >> 5;
        spTempqHD->bOverlay_AlternateqTD = VA2PA(( UINT32)spNewDumyqTD) >> 5;
    }
//printf("F");
   return (bReturnValue);
}
#endif
#endif //#if defined(ENABLE_FOURTH_EHC)

