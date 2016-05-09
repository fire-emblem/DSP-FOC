#include "DSP281x_Device.h" 
#include "DSP281x_GlobalPrototypes.h" 



// 2�߽ӿڵ�CH454����
#define		CH454_I2C_ADDR		0x40			// CH454�ĵ�ַ
#define		CH454_I2C_MASK		0x3E			// CH454��2�߽ӿڸ��ֽ���������

extern void DelayUS(Uint16 N_US);

Uint16 ch454_key=0;
Uint16 Key_Flag=0;
Uint16 Show_time=0;
Uint16 Show_time2=0;

Uint16 mBCD[20] = { 0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f, 
	  		   	   0x77, 0x7c, 0x58, 0x5e, 0x79, 0x71,0x3e ,0x00,0x49,0};//3e u

//---------------------------------------------------------------------------
// InitGpio: 
//---------------------------------------------------------------------------
// This function initializes the Gpio to a known (default) state.
//
// For more details on configuring GPIO's as peripheral functions,
// refer to the individual peripheral examples and/or GPIO setup example.

void Init_DIs_Gpio(void)
{
    EALLOW;
    GpioMuxRegs.GPBMUX.bit.PWM11_GPIOB4=0;//sdl
     GpioMuxRegs.GPBDIR.bit.GPIOB4=0;
     GpioMuxRegs.GPBMUX.bit.PWM12_GPIOB5=0;//scl
     GpioMuxRegs.GPBDIR.bit.GPIOB5=1;
     EDIS;

}

void iic_delay(void)//250ns
{


     asm(" NOP");
     asm(" NOP");
     asm(" NOP");
     asm(" NOP");
     asm(" NOP");
     asm(" NOP");
     asm(" NOP");
     asm(" NOP");
     asm(" NOP");
     asm(" NOP");
     asm(" NOP");
     asm(" NOP");
     asm(" NOP");
     asm(" NOP");
     asm(" NOP");
     asm(" NOP");
     asm(" NOP");
     asm(" NOP");
     asm(" NOP");
     asm(" NOP");
     asm(" NOP");
     asm(" NOP");
     asm(" NOP");
     asm(" NOP");
     asm(" NOP");
     asm(" NOP");
     asm(" NOP");
     asm(" NOP");
	 asm(" NOP");
     asm(" NOP");
	 asm(" NOP");
     asm(" NOP");
     asm(" NOP");
     asm(" NOP");
     asm(" NOP");
     asm(" NOP");
     asm(" NOP");
     asm(" NOP");
     asm(" NOP");
	 asm(" NOP");
      asm(" NOP");
       asm(" NOP");
       asm(" NOP");
      asm(" NOP");
       asm(" NOP");
       asm(" NOP");
      asm(" NOP");
	  asm(" NOP");
      asm(" NOP");
       asm(" NOP");
       asm(" NOP");
      asm(" NOP");
       asm(" NOP");
       asm(" NOP");
      asm(" NOP");
	  asm(" NOP");
      asm(" NOP");
       asm(" NOP");
       asm(" NOP");
      asm(" NOP");
       asm(" NOP");
       asm(" NOP");
      asm(" NOP");
       asm(" NOP");
      asm(" NOP");
       asm(" NOP");
       asm(" NOP");
      asm(" NOP");
       asm(" NOP");
       asm(" NOP");
       asm(" NOP");
      asm(" NOP");
       asm(" NOP");
       asm(" NOP");
      asm(" NOP");
       asm(" NOP");
         asm(" NOP");
      asm(" NOP");
       asm(" NOP");
       asm(" NOP");
      asm(" NOP");
       asm(" NOP");
       asm(" NOP");
       asm(" NOP");
      asm(" NOP");
       asm(" NOP");
       asm(" NOP");
      asm(" NOP");
       asm(" NOP");
       asm(" NOP");
      asm(" NOP");
	  asm(" NOP");
      asm(" NOP");
	  asm(" NOP");
      asm(" NOP");
      asm(" NOP");
       asm(" NOP");
       asm(" NOP");
      asm(" NOP");
       asm(" NOP");
       asm(" NOP");
      asm(" NOP");
	  asm(" NOP");
      asm(" NOP");
       asm(" NOP");
       asm(" NOP");
      asm(" NOP");
       asm(" NOP");
       asm(" NOP");
      asm(" NOP");
	  asm(" NOP");
      asm(" NOP");
       asm(" NOP");
       asm(" NOP");
      asm(" NOP");
       asm(" NOP");
       asm(" NOP");
      asm(" NOP");
	  asm(" NOP");
      asm(" NOP");
       asm(" NOP");
       asm(" NOP");
      asm(" NOP");
       asm(" NOP");
       asm(" NOP");
      asm(" NOP");
	 // DELAY_US(1);
      
    

}

void CH454_I2c_Start( void )  // ������ʼ
{
	
	SDL_1;   /*������ʼ�����������ź�*/
    EALLOW;   /* ����SDAΪ������� */
     GpioMuxRegs.GPBDIR.bit.GPIOB4=1;
    EDIS;
	iic_delay();
	SCL_1;
	iic_delay();

	SDL_0;   /*������ʼ�ź�*/
	iic_delay();    

	SCL_0;   /*ǯסI2C���ߣ�׼�����ͻ�������� */
    iic_delay();
}

void CH454_I2c_Stop( void )  // ��������
{
		SDL_0; 
    EALLOW;   /* ����SDAΪ������� */
     GpioMuxRegs.GPBDIR.bit.GPIOB4=1;
    EDIS;

	iic_delay();

	SCL_1;
	iic_delay();

	SDL_1;  /*����I2C���߽����ź�*/
	iic_delay();

    EALLOW; 
     GpioMuxRegs.GPBDIR.bit.GPIOB4=0;
    EDIS;   /* ����SDAΪ���뷽�� */
    iic_delay();

  	
}

void CH454_I2c_WrByte( Uint16 dat )	//дһ���ֽ�����
{
	Uint16 i;
	
    EALLOW;   
 GpioMuxRegs.GPBDIR.bit.GPIOB4=1; 
 EDIS; 
    
	iic_delay();

	for( i = 0; i != 8; i++ )  // ���8λ����
	{
		if( dat & 0x0080 ) 
		{
		    SDL_1;
		}
		else 
		{
		    SDL_0;
		}
		iic_delay();

		SCL_1;
		dat <<= 1;
		iic_delay();

		SCL_0;
		iic_delay();
	}
	 EALLOW;   
 GpioMuxRegs.GPBDIR.bit.GPIOB4=0;   
 EDIS; 
    SDL_1;
	iic_delay();

	SCL_1;  // ����Ӧ��
	iic_delay();

	SCL_0;
	iic_delay();
}

Uint16 CH454_I2c_RdByte( void )		//��һ���ֽ�����
{
	Uint16 dat,i;
    SDL_1;
	EALLOW;   
 GpioMuxRegs.GPBDIR.bit.GPIOB4=0;   
 EDIS; 
    iic_delay();

	dat = 0;
	for( i = 0; i != 8; i++ )  // ����8λ����
	{
		SCL_1;
		iic_delay();

		dat <<= 1;
		if( GpioDataRegs.GPBDAT.bit.GPIOB4==1) dat++;  // ����1λ

		SCL_0;
		iic_delay();
	}

	SDL_1;
    
	iic_delay();

	SCL_1;  // ������ЧӦ��

	iic_delay();

	SCL_0;
	iic_delay();

	return dat;
}

void ch454_write( Uint16 cmd )	//д����
{
	CH454_I2c_Start();               /*��������*/

   	CH454_I2c_WrByte((cmd>>7)&CH454_I2C_MASK|CH454_I2C_ADDR); 

   	CH454_I2c_WrByte(cmd);     /*��������*/
  	CH454_I2c_Stop();                 /*��������*/ 
}

Uint16 CH454_Read( Uint16 read_cmd )		//��ȡ����
{
	Uint16  keycode;
   	CH454_I2c_Start();                /*��������*/

	CH454_I2c_WrByte((read_cmd>>7)&CH454_I2C_MASK|CH454_I2C_ADDR|0x01);
	
   	keycode = CH454_I2c_RdByte();      /*��ȡ����*/
	CH454_I2c_Stop();                /*��������*/ 

	return keycode;
}


void Read_key(void)
{
static Uint16 i=0,j=0,k=0;

i++;
if(i==800)
{
i=0;
    if(Key_Flag==0)
    {
    k = CH454_Read(0x0700); 
    if((k>>6)==1)//�м�����
    {
       j=1;

    }
    else
    {
        if(j==1)
        {
             Key_Flag=1;//
             ch454_key=k;
             j=0;


        }


    }
    }
	}


}

void Init_ch454(void)
{
	Init_DIs_Gpio();
    ch454_write(0x443);//����ʾ����
	ch454_write(CH454_NO_BCD);//ֱ�����뷽ʽ   
  
     //������ʾ����
  	 ch454_write( CH454_DIG0_LO | mBCD[4]);
  	 ch454_write( CH454_DIG1_LO | mBCD[3]);
  	 ch454_write( CH454_DIG2_LO | mBCD[2]);
 	 ch454_write( CH454_DIG3_LO | mBCD[1]);
}

void Dis_Show(void)
{
    Uint16 i=0;
    Uint16 j=0;
    
    if(Show_time>10000&&Show_time<12000)
    {
    if(Run_PMSM==0)//��ʾ��ѹ
    {
        
       ch454_write( CH454_DIG3_LO | mBCD[16]);//u
       i= U_dc_dis/100;
       ch454_write( CH454_DIG2_LO | mBCD[i]);
        i= (U_dc_dis/10)%10;
       ch454_write( CH454_DIG1_LO | mBCD[i]);
       i= U_dc_dis%10;
       ch454_write( CH454_DIG0_LO | mBCD[i]);

    }
    else
    {
        if(O_Current==0)//��ʾת��
        {
            
            j=BaseSpeed/100;
            j=speed_dis*j;
            i= j/1000;
         ch454_write( CH454_DIG3_LO | mBCD[i]);
       i= (j/100)%10;
       ch454_write( CH454_DIG2_LO | mBCD[i]);
        i= (j/10)%10;
       ch454_write( CH454_DIG1_LO | mBCD[i]);
       i= (j)%10;
       ch454_write( CH454_DIG0_LO | mBCD[i]);

        }
        else//��ʾ����ָʾ
        {
            ch454_write( CH454_DIG0_LO | mBCD[10]);
   ch454_write( CH454_DIG1_LO | mBCD[0]);
   ch454_write( CH454_DIG2_LO | mBCD[17]);//r
   ch454_write( CH454_DIG3_LO | mBCD[14]);

        }


    }
    }
    else if(Show_time>22000)
    {
        Show_time=0;
         ch454_write( CH454_DIG3_LO | mBCD[18]);//
       i= speed_give/100;
       ch454_write( CH454_DIG2_LO | mBCD[i]);
        i= (speed_give/10)%10;
       ch454_write( CH454_DIG1_LO | mBCD[i]);
       i= speed_give%10;
       ch454_write( CH454_DIG0_LO | mBCD[i]);

        



    }
    





}

void deal_key_led(void)
{
    if(Key_Flag==1)
    {
        Key_Flag=0;
      
	switch(ch454_key)
	{ 
	case 3:	 //++
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
       Key_Flag=0;
         ch454_key=0xef;
		
		break;
        case 2://-- 
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
       Key_Flag=0;
         ch454_key=0xef;
		
		break;
	case 1:	 //����
	     Run_PMSM=1;
		 eva_open();
         Key_Flag=0;
         ch454_key=0xef;
		break;

 
	case 0: //ֹͣ
		eva_close();
        Run_PMSM=0;
        Place_now=0;
        Key_Flag=0;
        ch454_key=0xef;
		break;

        default:
            Key_Flag=0;
         ch454_key=0xef;
         break;
	}


    }



}

void deal_key_lcd(void)
{
    if(Key_Flag==1)
    {
        Key_Flag=0;
      
	switch(ch454_key)
	{ 
	case 0:	 //++
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
       Key_Flag=0;
         ch454_key=0xef;
		
		break;
        case 1://-- 
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
       Key_Flag=0;
         ch454_key=0xef;
		
		break;
	case 4:	 //����
	     Run_PMSM=1;
		 eva_open();
         Key_Flag=0;
         ch454_key=0xef;
		break;
	
	case 3://��ת
	ZhengFan=0;
        SpeedRef=speed_give*1.0/-100;
        eva_close();
        Key_Flag=0;
         ch454_key=0xef;
		
		break;
 
	case 5: //ֹͣ
		eva_close();
        Run_PMSM=0;
        Place_now=0;
        Key_Flag=0;
        ch454_key=0xef;
		break;

	case 2://��ת
	 ZhengFan=1;
        SpeedRef=speed_give*1.0/100;
        eva_close();
        Key_Flag=0;
         ch454_key=0xef; 
		
		break;
        default:
            Key_Flag=0;
         ch454_key=0xef;
         break;
	}


    }



}

void deal_key(void)
{
    if(KEY_Type==0)//����ܼ���
    {deal_key_led();}
    else
    {deal_key_lcd();} 

}

	
//===========================================================================
// End of file.
//===========================================================================

