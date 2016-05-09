// TI File $Revision: /main/2 $
// Checkin $Date: April 29, 2005   11:10:14 $
//###########################################################################
//
// FILE:	DSP281x_Gpio.c
//
// TITLE:	DSP281x General Purpose I/O Initialization & Support Functions.
//
//###########################################################################
// $TI Release: DSP281x C/C++ Header Files V1.20 $
// $Release Date: July 27, 2009 $
//###########################################################################

#include "DSP281x_Device.h"     // DSP281x Headerfile Include File
#include "DSP281x_Examples.h"   // DSP281x Examples Include File
Uint16 IPM_Fault=0;
void InitSci_Gpio(void) 
{
    EALLOW;
    
    GpioMuxRegs.GPGMUX.bit.SCIRXDB_GPIOG5=1;
    GpioMuxRegs.GPGMUX.bit.SCITXDB_GPIOG4=1;
    GpioMuxRegs.GPFMUX.bit.SCIRXDA_GPIOF5=1;
    GpioMuxRegs.GPFMUX.bit.SCITXDA_GPIOF4=1;

	EDIS;

}
void InitPWM_GPIO(void)
{    EALLOW; 
    GpioMuxRegs.GPAMUX.bit.PWM1_GPIOA0=1;//PWM1 
     GpioMuxRegs.GPAMUX.bit.PWM2_GPIOA1=1;//PWM2
      GpioMuxRegs.GPAMUX.bit.PWM3_GPIOA2=1;//PWM3
       GpioMuxRegs.GPAMUX.bit.PWM4_GPIOA3=1;//PWM4
        GpioMuxRegs.GPAMUX.bit.PWM5_GPIOA4=1;//PWM5
         GpioMuxRegs.GPAMUX.bit.PWM6_GPIOA5=1;//PWM6
 

         EDIS;

}
void InitSPI_GPIO(void)
{
    EALLOW;
   GpioMuxRegs.GPFMUX.bit.SPISIMOA_GPIOF0=1;// 
    GpioMuxRegs.GPFMUX.bit.SPISOMIA_GPIOF1=1;// 
     GpioMuxRegs.GPFMUX.bit.SPICLKA_GPIOF2=1;//   
      EDIS;
} 


void InitGpio(void)
{

// Set GPIO A port pins,AL(Bits 7:0)(input)-AH(Bits 15:8) (output) 8bits
// Input Qualifier =0, none
     EALLOW;
     GpioMuxRegs.GPAMUX.all=0x0000;     
     GpioMuxRegs.GPADIR.all=0xFF00;    	// upper byte as output/low byte as input
     GpioMuxRegs.GPAQUAL.all=0x0000;	// Input qualifier disabled

// Set GPIO B port pins, configured as EVB signals
// Input Qualifier =0, none
// Set bits to 1 to configure peripherals signals on the pins
     GpioMuxRegs.GPBMUX.all=0xFFFF;   
     GpioMuxRegs.GPBQUAL.all=0x0000;	// Input qualifier disabled
     EDIS;

     EALLOW; 

      

     GpioMuxRegs.GPFMUX.bit.MCLKXA_GPIOF8=0;//OUTPUT1 SVREADY
     GpioMuxRegs.GPFDIR.bit.GPIOF8=1;//设置方向为输出

     GpioMuxRegs.GPFMUX.bit.MFSRA_GPIOF11=0;//OUTPUT2 电机过热
     GpioMuxRegs.GPFDIR.bit.GPIOF11=1;//设置方向为输出

     GpioMuxRegs.GPFMUX.bit.SPISTEA_GPIOF3=0;//OUTPUT3
     GpioMuxRegs.GPFDIR.bit.GPIOF3=1;//设置方向为输出

   


     GpioMuxRegs.GPFMUX.bit.MDXA_GPIOF12=0;//INPUT1 start
     GpioMuxRegs.GPFDIR.bit.GPIOF12=0;//设置方向为输入

     GpioMuxRegs.GPFMUX.bit.MCLKRA_GPIOF9=0;//INPUT2
     GpioMuxRegs.GPFDIR.bit.GPIOF9=0;//设置方向为输入


     GpioMuxRegs.GPFMUX.bit.MFSXA_GPIOF10=0;//INPUT3 
     GpioMuxRegs.GPFDIR.bit.GPIOF10=0;//设置方向为输入
 

     

     GpioMuxRegs.GPDMUX.bit.T4CTRIP_SOCB_GPIOD6=0;//pwm_en
     GpioMuxRegs.GPDDIR.bit.GPIOD6=1;//设置方向为输出
     GpioDataRegs.GPDDAT.bit.GPIOD6=1;//输出高电平
    
     
      
     

     GpioMuxRegs.GPAMUX.bit.TDIRA_GPIOA11=0;//DC_ON 软启动继电器控制
     
     GpioDataRegs.GPADAT.bit.GPIOA11=1;//输出高电平

     GpioMuxRegs.GPADIR.bit.GPIOA11=1;//设置方向为输出

     

   

     

     
     GpioMuxRegs.GPBMUX.bit.PWM10_GPIOB3=0;//LED-YELLOW
     GpioMuxRegs.GPBDIR.bit.GPIOB3=1;//设置方向为输出

     
     GpioMuxRegs.GPBMUX.bit.PWM9_GPIOB2=0;//LED-RED
     GpioMuxRegs.GPBDIR.bit.GPIOB2=1;//设置方向为输出

     GpioMuxRegs.GPAMUX.bit.C1TRIP_GPIOA13=0;//CS_FRAM
     GpioMuxRegs.GPADIR.bit.GPIOA13=1;//设置方向为输出
     GpioDataRegs.GPADAT.bit.GPIOA13=1;

     GpioMuxRegs.GPEMUX.bit.XINT2_ADCSOC_GPIOE1=0;//AD_CS
     GpioMuxRegs.GPEDIR.bit.GPIOE1=1;//设置方向为输出
      GpioDataRegs.GPEDAT.bit.GPIOE1=1;//ad cs
     

     GpioMuxRegs.GPEMUX.bit.XNMI_XINT13_GPIOE2=0;//convst ad
     GpioMuxRegs.GPEDIR.bit.GPIOE2=1;//设置方向为输出
     GpioDataRegs.GPEDAT.bit.GPIOE2=1;//convst ad
     

     GpioMuxRegs.GPEMUX.bit.XINT1_XBIO_GPIOE0=0;//ad_rd
     GpioMuxRegs.GPEDIR.bit.GPIOE0=1;//设置方向为输出
     GpioDataRegs.GPEDAT.bit.GPIOE0=1;//ad rd
    

     GpioMuxRegs.GPAMUX.bit.C3TRIP_GPIOA15=0;//busy ad
     GpioMuxRegs.GPADIR.bit.GPIOA15=0;//设置方向为输入

     GpioMuxRegs.GPAMUX.bit.T1PWM_GPIOA6=0;//rest ad
     GpioMuxRegs.GPADIR.bit.GPIOA6=1;//设置方向为输出
     GpioDataRegs.GPADAT.bit.GPIOA6=0;//rest ad

    GpioMuxRegs.GPAMUX.bit.C2TRIP_GPIOA14=0;//SYNC 
    GpioMuxRegs.GPADIR.bit.GPIOA14=1;//方向设置为输出 
    GpioDataRegs.GPADAT.bit.GPIOA14=1; 

    GpioMuxRegs.GPBMUX.bit.CAP5Q2_GPIOB9=0;
    GpioMuxRegs.GPBDIR.bit.GPIOB9=0;

    GpioMuxRegs.GPBMUX.bit.CAP6QI2_GPIOB10=0;
    GpioMuxRegs.GPBDIR.bit.GPIOB10=1;
    GpioMuxRegs.GPBMUX.bit.TDIRB_GPIOB11=0;
    GpioMuxRegs.GPBDIR.bit.GPIOB11=0;

    GpioMuxRegs.GPDMUX.bit.T1CTRIP_PDPA_GPIOD0=0;//alram 设置为中断 一但进入此中断 立即关闭ipm输出
    GpioMuxRegs.GPDDIR.bit.GPIOD0=0;

    
    GpioMuxRegs.GPAMUX.bit.CAP1Q1_GPIOA8=1;//qep1
    GpioMuxRegs.GPAMUX.bit.CAP2Q2_GPIOA9=1;//qep2
    GpioMuxRegs.GPAMUX.bit.CAP3QI1_GPIOA10=1;//z信号

    GpioMuxRegs.GPBMUX.bit.PWM8_GPIOB1=0;
    GpioMuxRegs.GPBDIR.bit.GPIOB1=0;

    GpioMuxRegs.GPBMUX.bit.C6TRIP_GPIOB15=0;//W
     GpioMuxRegs.GPBDIR.bit.GPIOB15=0;//设置方向为输入
     
     GpioMuxRegs.GPBMUX.bit.C5TRIP_GPIOB14=0;//V
     GpioMuxRegs.GPBDIR.bit.GPIOB14=0;//设置方向为输入

     GpioMuxRegs.GPBMUX.bit.C4TRIP_GPIOB13=0;//U
     GpioMuxRegs.GPBDIR.bit.GPIOB13=0;//设置方向为输入

     

     InitSci_Gpio();
     InitPWM_GPIO();
     InitSPI_GPIO();
    
      EDIS;



     

}


void CPU_RUN(void)
{


   static Uint32 i=0;
   i++;
   if(i==5000)
   {
     EALLOW;
     GpioDataRegs.GPBDAT.bit.GPIOB10=1;
     
     EDIS;

   }
   else if(i==10000)
   {
    EALLOW;
    GpioDataRegs.GPBDAT.bit.GPIOB10=0;
  
    EDIS;
    i=0;
   }

}

void eva_open(void)
{
     EALLOW;
   EvaRegs.ACTRA.all = 0x0999;
   
   EDIS;


}

void eva_close(void)
{
     EALLOW;
   EvaRegs.ACTRA.all = 0x0bbb;       /*  1.3.5强制高，2.4.6高有效 */
    EvaRegs.CMPR1 = 1875;
   EvaRegs.CMPR2 = 1875;
   EvaRegs.CMPR3 = 1875;
   EDIS;
   Run_PMSM=0;
   LocationFlag=1;
   Speed_Ui=0;
   ID_Ui=0;
   IQ_Ui=0;
   Position=1;
   j=0;
   speed_dis=0;
   IQ_Given=0;
   OldRawTheta=0;
   SpeedRef=0;
   speed_give=0;
   Modulation=0.25;    // 调制比
   O_Current=0;
   Speed_SatError=0;
   IQ_SatError=0;
   ID_SatError=0;
   PosCount=0;
   OldRawThetaPos=0;
   HallAngle=0;
   Hall_Fault=0;
}


void  IPM_BaoHu(void)
{
    static Uint32 i=0;
    Uint16 j=0;

    j=GpioDataRegs.GPDDAT.bit.GPIOD0;//读取故障位
    if(j==0)
    {
        i++;
        if(i==1)
        {
            Pwm_EN_1;
            eva_close();
            IPM_Fault=1;
            	

        }

    }
    else
    {i=0;
	
    
    }

    



}

//===========================================================================
// No more.
//===========================================================================

