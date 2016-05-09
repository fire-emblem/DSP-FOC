//*****************************************************************************************************
//Flash��RAM����汾�л�˵��(����Ĭ��Ϊram�汾)
//
//һ.�л�ΪFlash��д�汾����
//1.���������е�:MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);
//               InitFlash();
//  ��������ȡ��ע��
//2.�������е�2812_RAM.cmd�ӹ�����ɾ�������CMD�ļ����µ�F2812_Flash.cmd�ļ���ȫ����һ�μ�����д��
//
//��.�л�ΪRAM���߷���汾����
//1.���������е�:MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);
//               InitFlash();
//  ��������ע�͵�
//2.�������е�F2812_Flash.cmd�ӹ�����ɾ�������CMD�ļ����µ�2812_RAM.cmd�ļ���ȫ����һ�μ��ɡ�
//
//*****************************************************************************************************
 
//*****************************************************************************************************
//ͷ�ļ�
//*****************************************************************************************************
#include "DSP281x_Device.h" 
#include "DSP281x_GlobalPrototypes.h" 
#include <math.h>

//*****************************************************************************************************
//�궨��
//***************************************************************************************************** 
#define CPU_CLOCK_SPEED      6.6667L   // for a 150MHz CPU clock speed
#define ADC_usDELAY 5000L
#define System_Operation_usDELAY 50000L
//#define DELAY_US(A)  DSP28x_usDelay(((((long double) A * 1000.0L) / (long double)CPU_CLOCK_SPEED) - 9.0L) / 5.0L) 

//*****************************************************************************************************
//�Ӻ�������
//***************************************************************************************************** 
extern void DSP28x_usDelay(unsigned long Count);  
interrupt void MainISR(void);
interrupt void SCIBRX_ISR(void);     // SCI-B
interrupt void ISRTimer2(void);
interrupt void Cap3_ISR(void);
void Init_SiShu(void);

//*****************************************************************************************************
//ȫ�ֱ����������ʼ��
//***************************************************************************************************** 
float32 i=0;
float32 j=0;
float32 k=0;
Uint16 l=0;
Uint16 IsrTicker = 0;
Uint16 BackTicker = 0; //���ڴ�������
Uint16 T1Period=0;     // T1��ʱ������(Q0)
Uint16 T3Period = 0;   
float32 Modulation=0.25;    // ���Ʊ�
int16 MPeriod=0;
int32 Tmp=0;
Uint16 lcd_dis_flag=0;
//:::::::::::::::::::::::::::λ�û���������:::::::::::::::::::::::::::
long PlaceError=0,Place_now=0, Now_P=0,//Ȧ��
              OutPreSat_Place=0;//λ�ñ���ֵ����
Uint16 PlaceSetBit=0;  //λ���趨��־λ

int32 	PosCount = 0;

float32 MfuncF1=0;
float32 MfuncF2=0;
float32 MfuncF3=0;  
//===============ת�ӳ�ʼλ�ö�λ=============================  
Uint16 LocationFlag=1;
Uint16 LocationEnd=0; 
Uint16 Position=1;
Uint16 PositionPhase60=1;
Uint16 PositionPhase120=2;
Uint16 PositionPhase180=3; 
Uint16 PositionPhase240=4;
Uint16 PositionPhase300=5;
Uint16 PositionPhase360=6;  

//===============DACģ��===================================== 
_iq DACTemp0=0;
_iq DACTemp1=0;
_iq DACTemp2=0; 

_iq MfuncC1=0;
_iq MfuncC2=0;
_iq MfuncC3=0; 
 
//===============ת���ٶȼ���===================================== 
Uint16 SpeedLoopPrescaler = 10;     // �ٶȻ�����
Uint16 SpeedLoopCount = 1;          // �ٶȻ�����  
_iq NewRawTheta=0;
_iq OldRawTheta=0; 
_iq SpeedRpm=0;                     //�ٶȣ���λ��ת/ÿ����

_iq RawThetaTmp=0;
float32 SpeedRef=0;
_iq Speed=0;                        //�ٶȣ�����ֵ

//===============ת�ӽǶȼ���===================================
Uint16 DirectionQep=0;               //ת����ת����
_iq RawTheta=0;
_iq OldRawThetaPos = 0;

_iq TotalPulse=0; 

_iq MechTheta = 0;                   //��е�Ƕȣ���λ����
_iq ElecTheta = 0;                   //�����Ƕȣ���λ����
_iq	AnglePU=0;                       //�Ƕȱ��ۻ�
_iq	Cosine=0;
_iq	Sine=0;

//===============���������������============================ 
_iq ia=0;
_iq ib=0;
_iq ic=0;
_iq ialfa=0;
_iq ibeta=0; 
_iq id=0;
_iq iq=0; 

//===============PI��������������============================ 
_iq ID_Given=0;
_iq ID_Ref=0;
_iq ID_Fdb=0;
_iq ID_Error=0; 

_iq ID_Up=0;
_iq ID_Up1=0;
_iq ID_Ui=0;
_iq ID_OutPreSat=0;
_iq ID_SatError=0;
_iq ID_OutMax=_IQ(1);
_iq ID_OutMin=_IQ(-1); 
_iq ID_Out=0;

_iq IQ_Given=0;
_iq IQ_Ref=0;
_iq IQ_Fdb=0;
_iq IQ_Error=0; 


_iq IQ_Up=0;
_iq IQ_Up1=0;
_iq IQ_Ui=0;
_iq IQ_OutPreSat=0;
_iq IQ_SatError=0;
_iq IQ_OutMax=_IQ(1);
_iq IQ_OutMin=_IQ(-1); 
_iq IQ_Out=0; 

_iq Speed_Given=_IQ(0.2); //�ٶȸ���    ����ֵ 0.2==>600RPM�����ת��1.0==>3000RPM
_iq Speed_Ref=0;
_iq Speed_Fdb=0;
_iq Speed_Error=0; 

_iq Speed_Up=0;
_iq Speed_Up1=0;
_iq Speed_Ui=0;
_iq Speed_OutPreSat=0;
_iq Speed_SatError=0;
_iq Speed_OutMax=_IQ(0.99999);
_iq Speed_OutMin=_IQ(-0.99999);
_iq Speed_Out=0;   

//===============SVPWM����==================================== 
Uint16 Sector = 0; 
_iq	Ualfa=0;  		
_iq	Ubeta=0;		
_iq	Ud=0;		
_iq	Uq=0;			
_iq	B0=0;			
_iq	B1=0;
_iq	B2=0;
_iq	X=0;
_iq	Y=0;
_iq	Z=0;
_iq	t1=0;
_iq	t2=0;
_iq	Ta=0;
_iq	Tb=0;
_iq	Tc=0;
_iq	MfuncD1=0;
_iq	MfuncD2=0;
_iq	MfuncD3=0; 
//===================================================================
Uint16 Run_PMSM=0;
float32 TEMP2=0;
Uint16 speed_give=0;
_iq MechScaler=_IQ(0.0);           
_iq SpeedScaler=_IQ(0.00);
Uint16 HallAngle=0;

Uint32 RS232_CNT=0;
Uint16 Hall_Fault=0;
Uint16 Angle_uint16=0;

Uint16 BuChang=0;
int16 TotalCnt=0;
_iq RawCnt1=0;
_iq RawCnt2=0;
Uint16 ShangDian_Err=0;

//========================�ٶȻ�PI����=================================
_iq Speed_Kp=_IQ(5);
_iq Speed_Ki=_IQ(0.001);
//=====================================================================

//========================Q�������PI����==============================
_iq IQ_Kp=_IQ(0.3);
_iq IQ_Ki=_IQ(0.001);
//=====================================================================

//========================D�������PI����==============================
_iq ID_Kp=_IQ(0.3);
_iq ID_Ki=_IQ(0.001);
//=====================================================================

long PlaceSet=1000000;//λ�û�ָ��������
Uint16 PlaceEnable=0;//λ�û�ʹ��  1 ʹ�� ;  0 ��ֹ

//=====================��������========================================
float32 E_Ding_DianLiu=1.5;         //���õ���Ķ���� ��λA 
_iq BaseRpm=_IQ(3000);              //���õ���ת��
Uint16 BaseSpeed=3000;              //���õ���ת����BaseRpm���
_iq PolePairs=_IQ(4);               //���õ��������
_iq LineEncoder=_IQ(2500);          //���õ��������ʽ������������
_iq Offset_Angle=_IQ(0.48);         //���õ��������Z������A���ƫ�ƽǶȣ���ôֵ
Uint16 ZhengFan=1;                  //����PMSM�������ת  1 ��ת ;  0 ��ת
Uint16 KEY_Type=1;                  //���ü�����ʾ������  1 LCD  ;  0 �����  


//*****************************************************************************************************
//������
//*****************************************************************************************************
void main(void)
{
//======================================================================================================
//ϵͳ��ʼ��
//======================================================================================================
	InitSysCtrl();
    InitGpio();
	DINT;
	
//======================================================================================================
//PIE�жϿ�������ʼ��
//====================================================================================================== 
	InitPieCtrl();
    IER = 0x0000;
	IFR = 0x0000;

//======================================================================================================
//PIE�жϿ�������ʼ��
//====================================================================================================== 
	InitPieVectTable();

	//====����ע�ͺ�����
    //====flash��д��ȡ��ע�ͺ�����===
    MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);
    InitFlash();
    
    InitSci();
    //InitCpuTimers();
	//ConfigCpuTimer(&CpuTimer2, 150, 20000);	
 	//StartCpuTimer2();
//======================================================================================================
//EVA��ʼ��
//====================================================================================================== 
    EALLOW;

	//EVAͨ�ö�ʱ��ȫ�ֿ��ƼĴ�����ʼ��
    EvaRegs.GPTCONA.all = 0x0;		 /* gp timer control register */

	//��ʱ��1�����ڼĴ���
    EvaRegs.T1PR =3750;  //����Ƶ��10KHz��((1/10KHz)/13.333334ns)/2=7500=3750
	T1Period=EvaRegs.T1PR;    
    EvaRegs.T1CNT = 0x0000;   	//��ʱ��1�ļ����Ĵ��� 
    EvaRegs.T1CON.all = 0x0840; //��ʱ��1�Ŀ��ƼĴ���
    //0x0840 = 0000 1000 0100 0000 
    //15,14  00  �����������ֹͣ 
	//13     0   ����
    //12,11  01  ������/������ģʽ
    //10~8  000  ����ʱ��Ԥ��������x/1
    //7      0   ʹ���Լ�ʹ��λ(TENABEL)
    //6      1   ��ʱ��ʹ�ܿ���λ
    //5,4   00   ʹ���ڲ�����ʱ��HSPLCK
	//3,2   00   ��ʱ���ȽϼĴ�����װ������--������ֵ����0
    //1     0    ��ֹ��ʱ���Ƚϲ��� 
    //0     0    ��ʱ��2�����ڼĴ���ѡ��λ��ʹ���Լ������ڼĴ���
    
    //����Ԫ�Ĵ���_������ƼĴ���
    EvaRegs.CAPCONA.all=0x8004;
	//0x8004 = 1000 0000 0000 0100 
	//����Ԫ3�����ؼ��

	TotalPulse=_IQmpy(_IQ(4),LineEncoder);
   
    
    EvaRegs.T2PR = (Uint16)(TotalPulse>>15);//��ʱ��2�����ڼĴ���
	EvaRegs.T2CNT = 0; 	 					//��ʱ��2�ļ����Ĵ���
	EvaRegs.T2CON.all = 0x1874;				//��ʱ��2�Ŀ��ƼĴ���
	//0x1874=0001 1000 0111 0100 
	//15,14   00   �����������ֹͣ 
 	//12,11   11   ������/������
	//10~8   000   ����ʱ��Ԥ��������x/1
	//7       0    ʹ���Լ�ʹ��λ(TENABEL)   
	//6       1    ʹ�ܶ�ʱ������
 	//5,4    11    ʱ����QEP��·����
 	//3,2    01    ��ʱ���ȽϼĴ�����װ������--������ֵ����0��������ڼĴ�����ֵ
    //1      0     ��ֹ��ʱ���Ƚϲ��� 
    //0      0     ��ʱ��2�����ڼĴ���ѡ��λ��ʹ���Լ������ڼĴ���

    EvaRegs.CMPR1 = 1000; 
    EvaRegs.CMPR2 = 1000; 
    EvaRegs.CMPR3 = 1000;            //ȫ�Ƚ�����ֵ  
	EvaRegs.GPTCONA.bit.T1TOADC = 1; // ��EVA�������жϱ�־����,����ADC
    
    //EvaRegs.DBTCONA.all = 0x0ff0;  /* ����ʱ��3.2us */ 
    //EvaRegs.DBTCONA.all = 0x0cf4;  /* ����ʱ��5.1us */ 
    EvaRegs.DBTCONA.all = 0x09f4;  /* ����ʱ��3.8us */ 
    //0x09f4 = 0000 1001 1111 0100
	//11~8 1001  ������ʱ������ 9
	//4~2  101   ������ʱ��Ԥ�������λ��xΪ��������ʱ��Ƶ��,����ĿΪ75MHz�� x/32


    EvaRegs.EVAIMRA.all = 0x0200;   /* ����ʱ�������ж� */
    EvaRegs.COMCONA.all = 0x8200; 
    //�ȽϿ��ƼĴ���COMCONA 
    //0 A600=1010 0110 0000 0000 
    //15      1   ʹ�ܱȽϵ�Ԫ����
    //14,13  01   ��T1CNT=0��T1PR��CMPRX��������Ϊ���������ƥ�䣩
    //12      0   ��ֹ�ռ�ʸ��PWMģʽ
    //11,10  01   ��T1CNT=0��T1RP���ȽϷ�ʽ���ƼĴ�������������Ϊ���������ƥ�䣩
    //9       1   ʹ��ȫ�Ƚ����

    EvaRegs.CAPCONA.all = 0x9004;  //���������
    EvaRegs.EVAIFRC.bit.CAP3INT=1; //���cap3�жϱ�־
    EvaRegs.EVAIMRC.bit.CAP3INT=1; //ʹ��cap3�ж�
//======================================================================================================
//EVB��ʼ��,����ģ��DAC���
//====================================================================================================== 
    EvbRegs.T3PR = 3750;  ////����Ƶ��10KHz 
	T3Period=EvbRegs.T3PR;         
    EvbRegs.T3CNT = 0x0000;
    EvbRegs.T3CON.all = 0x0840;   
    //0x0840=0000 1000 0100 0000 
    //������/������ģʽ
    //����ʱ��Ԥ����x/1
    //��ֹ��ʱ���Ƚϲ��� 
    //ʹ���ڲ�����ʱ��HSPLCK
    //ʹ���Լ�ʹ��λ
    //�����������ֹͣ 


    EvbRegs.CMPR4 = 1000; 
    EvbRegs.CMPR5 = 1000; 
    EvbRegs.CMPR6 = 1000;         //ȫ�Ƚ�����ֵ 

    EvbRegs.ACTRB.all = 0x0999;   //PWM1,3,5����Ч��PWM2,4,6����Ч
    EvbRegs.DBTCONB.all = 0x0AF8; 
    //0x0AF8=0000 1010 1111 1000
    //�������� x/32=150MHz/32=4.6875MHz  ��������Ϊ10������
    //����ʱ��=32*6.67*10=2134.4ns=2us
    EvbRegs.COMCONB.all =0xA600;  
    //0xA600=1010 0110 0000 0000 
    //ʹ�ܱȽ�������
    //�ȽϼĴ���CMPRX��������Ϊ���������ƥ��
    //��ֹӲ���ռ�����PWMģʽ
    //ȫ�Ƚ������ʹ��
    //��ʽ���ƼĴ���ACTRA��������Ϊ���������ƥ��  

//======================================================================================================
//���ж�
//====================================================================================================== 
	EvaRegs.EVAIMRA.bit.T1UFINT = 1;
    EvaRegs.EVAIFRA.bit.T1UFINT = 1;//T1�����ж�
    EDIS;

 	EALLOW;
    PieVectTable.T1UFINT = &MainISR;
    PieVectTable.RXBINT = &SCIBRX_ISR;   //���ô���B�����жϵ��ж�����
    PieVectTable.CAPINT3=&Cap3_ISR;
    //PieVectTable.TINT2 =  &ISRTimer2;
    EDIS;

    PieCtrlRegs.PIEIER2.bit.INTx6=1;//t1upint PIE�жϱ�־�Ĵ���PIEIFRx�����жϼ���ʱ����Ӧλ��1���жϱ���Ӧ����Զ����㣬����Ҫ�û�������
    PieCtrlRegs.PIEIER9.bit.INTx3=1;//SCIRXB
    PieCtrlRegs.PIEIER3.bit.INTx7=1;//CAP3
   	IER |= M_INT2;	// //CPU�ж�ʹ�ܼĴ�����д��1����Ӧλ��ʹ���ж�INT2�� 
   	IER |= M_INT9;    //SCI //�������ж�
   	IER |= M_INT3;
    //IER |= M_INT14;   //cputimer2
    Init_SiShu();
    eva_close();
    
    Ad_CaiJi(); 
    Ad_CaiJi();  

    if(AD_BUF[4]<100)
    {
 	Pwm_EN_0;//����PWMʹ��
    }

    else
    {
    Pwm_EN_1;//��ֹPWMʹ��
	ShangDian_Err=1;
    }
    
    Init_ch454();
    Init_lcd();
    DELAY_US(1000000);

    EINT;  
    ERTM;
     
//======================================================================================================
//��ѭ��
//====================================================================================================== 
	for(;;)
	{ 
   		
        CPU_RUN();
        DC_Link();
        ShowDisp();
        Dis_Show();
        deal_key();
        LCD_DIS();

	}

}

//*****************************************************************************************************
//�жϷ������
//***************************************************************************************************** 
interrupt void MainISR(void)
{
    _iq t_01,t_02;
    
     IPM_BaoHu();

    RS232_CNT=RS232_CNT+5;
    Show_time++;
    Show_time2++;
    if(Show_time2==1000)//1��
    {
        Show_time2=0;
        lcd_dis_flag=1;
    }
    Read_key(); 
    Ad_CaiJi();
    JiSuan_Dl(); 
    
	if(Run_PMSM==1&&IPM_Fault==0)
	{

   		if(LocationFlag!=LocationEnd)
		{ 
    		Modulation=0.95;
        	if(GpioDataRegs.GPBDAT.bit.GPIOB15)//W
        	{
            	HallAngle+=1;
        	}
        	if(GpioDataRegs.GPBDAT.bit.GPIOB14)//V
        	{ 
           		HallAngle+=2;
        	}
        	if(GpioDataRegs.GPBDAT.bit.GPIOB13)//U
        	{
          		HallAngle+=4;
        	}

			switch(HallAngle)
			{
			case 5:
				Position=PositionPhase60;
                LocationFlag=LocationEnd;//��λ����
				EvaRegs.T2CNT =(EvaRegs.T2PR-BuChang*0-BuChang/2);   
				break;
			case 1:
				Position=PositionPhase360;
                LocationFlag=LocationEnd;//��λ����
				EvaRegs.T2CNT =(EvaRegs.T2PR-BuChang*5-BuChang/2);   
				break;
			case 3:
				Position=PositionPhase300;
                LocationFlag=LocationEnd;//��λ����
				EvaRegs.T2CNT =(EvaRegs.T2PR-BuChang*4-BuChang/2);   
				break;
			case 2:
				Position=PositionPhase240;
                LocationFlag=LocationEnd;//��λ����
				EvaRegs.T2CNT =(EvaRegs.T2PR-BuChang*3-BuChang/2);   
				break;
			case 6:
				Position=PositionPhase180;
                LocationFlag=LocationEnd;//��λ����
				EvaRegs.T2CNT =(EvaRegs.T2PR-BuChang*2-BuChang/2);   
				break;
			case 4:
				Position=PositionPhase120;
                LocationFlag=LocationEnd;//��λ����
				EvaRegs.T2CNT =(EvaRegs.T2PR-BuChang*1-BuChang/2);     
				break;
            default:
                Run_PMSM=0;//�����źŴ�������ֹͣ
                eva_close();
                Hall_Fault=1;
                break;
			}

		}   
//======================================================================================================
//��ʼλ�èλ��������ʼ�ջ�����
//====================================================================================================== 
	else if(LocationFlag==LocationEnd)
	{  
		l++;   
        //��ת�����ж� 
		DirectionQep = 0x4000&EvaRegs.GPTCONA.all;
		DirectionQep = DirectionQep>>14;
        RawTheta = _IQ(EvaRegs.T2CNT);
		RawTheta =TotalPulse - RawTheta;
        
		// �����е�Ƕ�
		if(DirectionQep ==1) //T2STAT=1,��������������˳ʱ�룻
		{			
            if((RawTheta> RawCnt1) && (OldRawThetaPos<_IQ(1000)))
			{
				PosCount += TotalCnt;
			}
			Place_now= _IQtoF(TotalPulse - RawTheta)+PosCount;
			OldRawThetaPos = RawTheta; 
		}
		else if(DirectionQep ==0)//T2STAT=0���ݼ�������������ʱ��
		{  
            if((OldRawThetaPos> RawCnt2) && (RawTheta<_IQ(900)))
			{
				PosCount -= TotalCnt;
			}
			Place_now = _IQtoF(TotalPulse - RawTheta)+PosCount;
			OldRawThetaPos = RawTheta;
		} 

		MechTheta = _IQmpy(MechScaler,RawTheta);
        if(MechTheta>_IQ(360))
        {MechTheta=MechTheta-_IQ(360);}
        if(MechTheta<_IQ(-360))
        {MechTheta=MechTheta+_IQ(360);}
		ElecTheta = _IQmpy(PolePairs,MechTheta);    
	
		AnglePU=_IQdiv(ElecTheta,_IQ(360))+Offset_Angle;
	   	Sine = _IQsinPU(AnglePU);
	   	Cosine = _IQcosPU(AnglePU);   
//======================================================================================================
//�����������ia,ib,ic===>ialfa,ibeta===>id,iq��ʸ���任
//======================================================================================================
		
		ialfa=ia;
		ibeta=_IQmpy(ia,_IQ(0.57735026918963))+_IQmpy(ib,_IQ(1.15470053837926));  

		id = -_IQmpy(ialfa,Cosine) -_IQmpy(ibeta,Sine);
		iq = -_IQmpy(ibeta,Cosine)+_IQmpy(ialfa,Sine) ; 
 
	    if (SpeedLoopCount>=SpeedLoopPrescaler)
	    {   
			// ��ת�����ж� 
			DirectionQep = 0x4000&EvaRegs.GPTCONA.all;
			DirectionQep = DirectionQep>>14;

 			NewRawTheta =_IQ(EvaRegs.T2CNT);
			// �����е�Ƕ�
			if(DirectionQep ==1) //T2STAT=1,����������
			{
               	RawThetaTmp =  OldRawTheta-NewRawTheta ; 
				if(RawThetaTmp > _IQ(0))
				{
				 	RawThetaTmp = RawThetaTmp - TotalPulse;  
				} 
			}
			else if(DirectionQep ==0) //T2STAT=0���ݼ�����
			{                
			 	RawThetaTmp =OldRawTheta- NewRawTheta; 
				if(RawThetaTmp < _IQ(0))
				{
					 RawThetaTmp = RawThetaTmp + TotalPulse;
				}
			 }
	 
			Speed = _IQmpy(RawThetaTmp,SpeedScaler);
			SpeedRpm = _IQmpy(BaseRpm,Speed);   				
			OldRawTheta = NewRawTheta;
            if(Speed<0)
			{
				speed_dis=_IQtoF(_IQmpy(Speed, _IQ(-100)));
			}
			else
			{
            	speed_dis=_IQtoF(_IQmpy(Speed, _IQ(100)));
            }
		    SpeedLoopCount=1; 
			RawThetaTmp=0; 


//=================λ�û�����===================================
	if(PlaceEnable ==1)
   	    {
        PlaceError = PlaceSet + Place_now;
  
		OutPreSat_Place = PlaceError;
		if((PlaceError<=10000)&&(PlaceError>=-10000))
        { 
           OutPreSat_Place = PlaceError/3; 
		}  
		
        if (OutPreSat_Place> 2000)
        {
          SpeedRef =  0.5;
        }
        else if (OutPreSat_Place< -2000)
        {
          SpeedRef =  -0.5;
        }
        else
        {
          SpeedRef = OutPreSat_Place/(float32)BaseSpeed;
        }
	   
   	}
//=================�ٶȻ�PI===================================

            Speed_Ref=_IQ(SpeedRef);
			Speed_Fdb=Speed;

			Speed_Error=Speed_Ref - Speed_Fdb;//���

			Speed_Up=_IQmpy(Speed_Kp,Speed_Error);//kp
			

			Speed_OutPreSat=Speed_Up+Speed_Ui;

			if(Speed_OutPreSat>Speed_OutMax)
				{
                    Speed_Out=Speed_OutMax;

            	}
			else if(Speed_OutPreSat<Speed_OutMin)
	 			{
                    Speed_Out=Speed_OutMin;
				}
			else 
				{
                    Speed_Out=Speed_OutPreSat;  
				}
	
			Speed_SatError=Speed_Out-Speed_OutPreSat;  
            Speed_Ui=Speed_Ui + _IQmpy(Speed_Ki,Speed_Up) + _IQmpy(Speed_Ki,Speed_SatError);

			IQ_Given=Speed_Out; 
		} 

	else 
    {
        SpeedLoopCount++; 
	}

//======================================================================================================
//IQ����PID���ڿ���
//======================================================================================================  
		IQ_Ref=IQ_Given;
		IQ_Fdb=iq;

		IQ_Error=IQ_Ref-IQ_Fdb;//���

		IQ_Up=_IQmpy(IQ_Kp,IQ_Error);//kp
		

		IQ_OutPreSat=IQ_Up+IQ_Ui;

		if(IQ_OutPreSat>IQ_OutMax)
		{
            IQ_Out=IQ_OutMax;
		}
		else if(IQ_OutPreSat<IQ_OutMin)
		{
            IQ_Out=IQ_OutMin;
		}
		else 
		{
            IQ_Out=IQ_OutPreSat;  
        }

		IQ_SatError=IQ_Out-IQ_OutPreSat;  
        IQ_Ui=IQ_Ui + _IQmpy(IQ_Ki,IQ_Up) + _IQmpy(IQ_Ki,IQ_SatError);

		Uq=IQ_Out;

//======================================================================================================
//ID����PID���ڿ���
//======================================================================================================  
		ID_Ref=0;
		ID_Fdb=id;

		ID_Error=ID_Ref-ID_Fdb;//���

		ID_Up=_IQmpy(ID_Kp,ID_Error);    
		  

		ID_OutPreSat=ID_Up+ID_Ui;   

		if(ID_OutPreSat>ID_OutMax)   
		{
        	ID_Out=ID_OutMax;
		}
		else if(ID_OutPreSat<ID_OutMin)
		{
        	ID_Out=ID_OutMin;
        }
		else 
		{
        	ID_Out=ID_OutPreSat; 
		}

		ID_SatError=ID_Out-ID_OutPreSat;  
        ID_Ui=ID_Ui+_IQmpy(ID_Ki,ID_Up)+_IQmpy(ID_Ki,ID_SatError);  

		Ud=ID_Out;

//======================================================================================================
//IPark�任UD,UQ-->UALFA,UBETA��ø����ź�
//====================================================================================================== 
		Ualfa = _IQmpy(Ud,Cosine) - _IQmpy(Uq,Sine);
		Ubeta = _IQmpy(Uq,Cosine) + _IQmpy(Ud,Sine); 
    
		B0=Ubeta;
		B1=_IQmpy(_IQ(0.8660254),Ualfa)- _IQmpy(_IQ(0.5),Ubeta);// 0.8660254 = sqrt(3)/2 
		B2=_IQmpy(_IQ(-0.8660254),Ualfa)- _IQmpy(_IQ(0.5),Ubeta); // 0.8660254 = sqrt(3)/2

		Sector=0;
		if(B0>_IQ(0)) Sector =1;
		if(B1>_IQ(0)) Sector =Sector +2;
		if(B2>_IQ(0)) Sector =Sector +4; 

		X=Ubeta;//va
		Y=_IQmpy(_IQ(0.8660254),Ualfa)+ _IQmpy(_IQ(0.5),Ubeta);// 0.8660254 = sqrt(3)/2 vb
		Z=_IQmpy(_IQ(-0.8660254),Ualfa)+ _IQmpy(_IQ(0.5),Ubeta); // 0.8660254 = sqrt(3)/2 vc 

	    if(Sector==1)
	 	{
			t_01=Z;
			t_02=Y;
     	    if((t_01+t_02)>_IQ(1))
      	    {
     		   t1=_IQmpy(_IQdiv(t_01, (t_01+t_02)),_IQ(1));
     		   t2=_IQmpy(_IQdiv(t_02, (t_01+t_02)),_IQ(1));
      	    }
     	    else
       	    { 
               t1=t_01;
       		   t2=t_02;
     	    }
			Tb=_IQmpy(_IQ(0.5),(_IQ(1)-t1-t2));
			Ta=Tb+t1;
			Tc=Ta+t2;
		}
		else if(Sector==2)
		{
			t_01=Y;
			t_02=-X;
            if((t_01+t_02)>_IQ(1))
   		    {
     		   t1=_IQmpy(_IQdiv(t_01, (t_01+t_02)),_IQ(1));
     		   t2=_IQmpy(_IQdiv(t_02, (t_01+t_02)),_IQ(1));
      	    }
       		else
      	    { 
      	       t1=t_01;
     		   t2=t_02;
     	    }
			Ta=_IQmpy(_IQ(0.5),(_IQ(1)-t1-t2));
			Tc=Ta+t1;
			Tb=Tc+t2;
 		} 
	    else if(Sector==3)
 	    {
			t_01=-Z;
			t_02=X;
            if((t_01+t_02)>_IQ(1))
     	    {
				t1=_IQmpy(_IQdiv(t_01, (t_01+t_02)),_IQ(1));
				t2=_IQmpy(_IQdiv(t_02, (t_01+t_02)),_IQ(1));
      	    }
            else
			{ 
				t1=t_01;
       			t2=t_02;
      	    }
			Ta=_IQmpy(_IQ(0.5),(_IQ(1)-t1-t2));
			Tb=Ta+t1;
			Tc=Tb+t2;	
	    } 
	    else if(Sector==4)
	    {
			t_01=-X;
			t_02=Z;
            if((t_01+t_02)>_IQ(1))
      		 {
        	 	t1=_IQmpy(_IQdiv(t_01, (t_01+t_02)),_IQ(1));
     		    t2=_IQmpy(_IQdiv(t_02, (t_01+t_02)),_IQ(1));
      		 }
            else
            {
            	t1=t_01;
            	t2=t_02;
            }
			Tc=_IQmpy(_IQ(0.5),(_IQ(1)-t1-t2));
			Tb=Tc+t1;
			Ta=Tb+t2;
	    } 
	    else if(Sector==5)
	    {
			t_01=X;
			t_02=-Y;
            if((t_01+t_02)>_IQ(1))
      	    {
      	    	t1=_IQmpy(_IQdiv(t_01, (t_01+t_02)),_IQ(1));
      	   	    t2=_IQmpy(_IQdiv(t_02, (t_01+t_02)),_IQ(1));
	        }
      	    else
      	    {
      	   	    t1=t_01;
      	   	    t2=t_02;
            }
			Tb=_IQmpy(_IQ(0.5),(_IQ(1)-t1-t2));
			Tc=Tb+t1;
	 		Ta=Tc+t2;
		}
		else if(Sector==6)
		{
			t_01=-Y;
			t_02=-Z;
            if((t_01+t_02)>_IQ(1))
     	    {
     	    	t1=_IQmpy(_IQdiv(t_01, (t_01+t_02)),_IQ(1));
       			t2=_IQmpy(_IQdiv(t_02, (t_01+t_02)),_IQ(1));
      	    }
       		else
      		{ 
       			t1=t_01;
       			t2=t_02;
       		}
			Tc=_IQmpy(_IQ(0.5),(_IQ(1)-t1-t2));
			Ta=Tc+t1;
			Tb=Ta+t2;
		} 
		MfuncD1=_IQmpy(_IQ(2),(_IQ(0.5)-Ta));
		MfuncD2=_IQmpy(_IQ(2),(_IQ(0.5)-Tb));
		MfuncD3=_IQmpy(_IQ(2),(_IQ(0.5)-Tc));  
		}
//======================================================================================================
//EVAȫ�Ƚ���������ֵ�������������
//====================================================================================================== 
	MPeriod = (int16)(T1Period * Modulation);              // Q0 = (Q0 * Q0)

	Tmp = (int32)MPeriod * (int32)MfuncD1;                    // Q15 = Q0*Q15������ȫ�Ƚ���CMPR1��ֵ
	EvaRegs.CMPR1 = (int16)(Tmp>>16) + (int16)(T1Period>>1); // Q0 = (Q15->Q0)/2 + (Q0/2)

	Tmp = (int32)MPeriod * (int32)MfuncD2;                    // Q15 = Q0*Q15������ȫ�Ƚ���CMPR2��ֵ
	EvaRegs.CMPR2 = (int16)(Tmp>>16) + (int16)(T1Period>>1); // Q0 = (Q15->Q0)/2 + (Q0/2)

	Tmp = (int32)MPeriod * (int32)MfuncD3;                    // Q15 = Q0*Q15������ȫ�Ƚ���CMPR3��ֵ
	EvaRegs.CMPR3 = (int16)(Tmp>>16) + (int16)(T1Period>>1); // Q0 = (Q15->Q0)/2 + (Q0/2) 
}

	if(DC_ON_flag==1)
	{
   		 DC_ON_CNT++;
   		 if(DC_ON_CNT==5000)
    	 {
      		 DC_ON_OPEN=1;
		     DC_ON_CNT=0;
   		 }
   	 }

//======================================================================================================
//�жϷ���
//====================================================================================================== 
PieCtrlRegs.PIEACK.all|=0x0002;// Issue PIE ack
EvaRegs.EVAIFRA.bit.T1UFINT=1;

}

interrupt void SCIBRX_ISR(void)     // SCI-B
{
	static unsigned int RxBuf;
	
    RxBuf=ScibRegs.SCIRXBUF.all;
	switch(RxBuf)
	{
	case 0:  //��ת
	    ZhengFan=1;
        SpeedRef=speed_give*1.0/100;
		eva_close();
		break;
	case 1:	 //����
	    Run_PMSM=1;
		eva_open();
	    break;
	case 2:  //�����ٶ� �ٷֱ� 10��10���ļ�
	    if(ZhengFan==1)
	    {
       		 if(speed_give<91)
             {
           		speed_give=10+speed_give;
                SpeedRef=speed_give*1.0/100;
       	     }        
        }
        else
        {
       		 if(speed_give<91)
       		 {
           		 speed_give=10+speed_give;
        		 SpeedRef=speed_give*1.0/-100;
             }
         }		
		break;
	case 3://����Ƶ�� һ��һ���ļ�
	    if(ZhengFan==1)
	    {
      		 if(speed_give<100)
       		 {
           		 speed_give=1+speed_give;
        		 SpeedRef=speed_give*1.0/100;
             }
         }
         else
         {
        	 if(speed_give<100)
       		 {
             	 speed_give=1+speed_give;
        		 SpeedRef=speed_give*1.0/-100;
             }
       	 }
		break;
	case 4:	//��ת
		ZhengFan=0;
        SpeedRef=speed_give*1.0/-100;
        eva_close();
		break;
	case 5: //ֹͣ
		eva_close();
        Run_PMSM=0;
		break;
	case 6: //��С�ٶȰٷֱ� 10��10���ļ�
        if(ZhengFan==1)
	    {
       		 if(speed_give>9)
        	 {
            	 speed_give=speed_give-10;
         		 SpeedRef=speed_give*1.0/100;
             }
        }
        else
        {
       		 if(speed_give>9)
        	 {
           		 speed_give=speed_give-10;
        	 	 SpeedRef=speed_give*1.0/-100;
              }
        }		
		break;
	case 7://��С�ٶȰٷֱ� һ��һ���ļ�
		if(ZhengFan==1)
	    {
        	 if(speed_give>0)
        	 {
             		speed_give=speed_give-1;
           	 		SpeedRef=speed_give*1.0/100;
             }
        }
        else
        {
       		 if(speed_give>0)
        	 {
            		speed_give=speed_give-1;
       				SpeedRef=speed_give*1.0/-100;
             }
        }
		break;
	}
    PieCtrlRegs.PIEACK.bit.ACK9 = 1;
}


interrupt void Cap3_ISR(void)
{
	EvaRegs.T2CNT = 0;  
 
	vaRegs.EVAIFRC.bit.CAP3INT=1;

}

void Init_SiShu(void)
{
	float32 temp;
    int16 tempcnt;

	temp=(90.0*32768)/LineEncoder;
 	MechScaler=_IQ(temp);

 	temp=(15000*32768)/LineEncoder;
 	temp=temp/BaseSpeed;
 	SpeedScaler=_IQ(temp);

 
 	temp=(((LineEncoder/32768)*4)/360.0)*60;
	temp=(temp*32768)/PolePairs;
	BuChang=temp;

	temp=(Offset_Angle*360)/32768.0;
 	temp=(((LineEncoder/32768)*4)/360.0)*temp;
 	temp=(temp*32768)/PolePairs;

	Angle_uint16=temp;

	TotalCnt=_IQtoF(_IQmpy(LineEncoder, PolePairs));
 
 	tempcnt=TotalCnt-1000;
 	RawCnt1=_IQ(tempcnt);

 	tempcnt=TotalCnt-100;
 	RawCnt2=_IQ(tempcnt);
 	E_Ding_DianLiu=1.414*E_Ding_DianLiu;
 	GuoliuZhi=15*E_Ding_DianLiu;

}


/*interrupt void ISRTimer2(void)
{
 
	
	
}*/
/*====================================================================================================
The end!
=====================================================================================================*/










