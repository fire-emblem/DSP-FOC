//
//      TMDX ALPHA RELEASE
//      Intended for product evaluation purposes
//
//###########################################################################
//
// FILE:	DSP28_Adc.c
//
// TITLE:	DSP28 ADC Initialization & Support Functions.
//
//###########################################################################
//
//  Ver | dd mmm yyyy | Who  | Description of changes
// =====|=============|======|===============================================
//  0.55| 06 May 2002 | L.H. | EzDSP Alpha Release
//  0.56| 20 May 2002 | L.H. | No change
//  0.57| 27 May 2002 | L.H. | No change
//###########################################################################

#include "DSP281x_Device.h"

int32 AD_BUF[8] = {0,0,0,0,0,0,0,0}; 
int16 *AdcResult = (int16 *)0x2001;	//read AD convst result
Uint16 I_A=0;
Uint16 I_B=0;

float32 IA_MAX=0;
float32 IB_MAX=0;

float32 U_dc=0;
Uint16 U_dc_dis=0;
Uint16 GuoliuZhi=0;

int16 HALL_U=0;//U相霍尔零点
int16 HALL_W=0;//W相霍尔零点
Uint16 O_Current=0;
Uint16 DC_ON_flag=0;
Uint16 DC_ON_OPEN=0;
Uint32 DC_ON_CNT=0;

void DelayUS(Uint16 N_US) //1US延时 
{
    Uint16 i=0;  
	for(i=0;i<N_US;i++)
	{
	  asm("	NOP");
	}
}

void Ad_CaiJi(void)
{	
    Uint16 i=0,j=0; 
    int32 temp1,temp2;


    EALLOW; 
    GpioDataRegs.GPEDAT.bit.GPIOE2=0;//convst ad 
    asm("	NOP");
	asm("	NOP");
	asm("	NOP");
    asm("	NOP");
    asm("	NOP");
    GpioDataRegs.GPEDAT.bit.GPIOE2=1;//convst ad
    EDIS;
    DelayUS(1);

    while(GpioDataRegs.GPADAT.bit.GPIOA15==1)
    {
		j++;
		if(j==200)
		{break;}
     };
     EALLOW; 
     GpioDataRegs.GPEDAT.bit.GPIOE1=0;//ad cs
     EDIS;

     for(i=0;i<8;i++)
     {
        EALLOW;
        GpioDataRegs.GPEDAT.bit.GPIOE0=0;//ad rd
        EDIS;
       
        AD_BUF[i] = (*AdcResult);
	
        //AD_BUF[i]=(AD_BUF[i]*610)/1000;//这里是真实值 ad7607 单位 mv
              
        AD_BUF[i]=(AD_BUF[i]*305)/2000;
        //这里是真实值 ad7607 单位 mv
		//LSB=305/2 uV 

        EALLOW;
        GpioDataRegs.GPEDAT.bit.GPIOE0=1;//ad rd
        EDIS;
     }

     if(Run_PMSM==0)
     {
        HALL_U=AD_BUF[0];
        HALL_W=AD_BUF[1];
     }

     temp1=AD_BUF[0]-HALL_U;//mv

     ia=_IQmpy(_IQ(temp1), _IQ(0.04));
     //安 
     //(I/40+2.5)*305/2000*40/1000=
     ia=_IQdiv(ia,_IQ(E_Ding_DianLiu));//标么
     
     temp2=AD_BUF[1]-HALL_W;//mv
     ic=_IQmpy(_IQ(temp2), _IQ(0.04));//安
     ic=_IQdiv(ic,_IQ(E_Ding_DianLiu));//标么
    
     ib=-ia-ic;    // 

     EALLOW; 
     GpioDataRegs.GPEDAT.bit.GPIOE1=1;//ad cs
     EDIS;

}
 


void DC_Link(void)//计算母线电压
{
    Uint16 i;
    U_dc=0.2951*AD_BUF[4];
    i=U_dc;
    U_dc_dis=U_dc;
    if(i>16)//检测到有给动力电
    {
        DC_ON_flag=1;
        if(DC_ON_OPEN==1)
        {
       		 DC_ON_0;
        }
    }
    else
    {
        DC_ON_1;
	    DC_ON_flag=0;
        DC_ON_OPEN=0;
        DC_ON_CNT=0;
    }
}

void JiSuan_Dl(void)//计算电流
{
    float32 IA,IB;
    static Uint16 i=0;
    
    IA=(AD_BUF[0]-HALL_U)*0.4;//放大10倍
    if(IA<0.0)
    {IA=-IA;}
   
    IB=(AD_BUF[1]-HALL_W)*0.4;//放大10倍
    if(IB<0.0)
    {IB=-IB;}

    if(IA>IA_MAX)
    {
        IA_MAX=IA;
    }
    if(IB>IB_MAX)
    {
        IB_MAX=IB;
    }
    i++;
    if(i==400)
    {
    	I_A=IA/1.414;//有效值
    	I_B=IB/1.414;//有效值
    	i=0;
   	    IB_MAX=0;
    	IA_MAX=0;

    	if((I_A>GuoliuZhi)||(I_B>GuoliuZhi))
   		{
      		Pwm_EN_1;
       		eva_close();//过流保护，停机
        	O_Current=1;
        	Run_PMSM=0;
        }
        else if((I_A>100)||(I_B>100))
   		{
       		Pwm_EN_1;
       		eva_close();//过流保护， 停机
      		O_Current=1;
       		 Run_PMSM=0;
    	 }
    }
   }


//===========================================================================
// No more.
//===========================================================================
