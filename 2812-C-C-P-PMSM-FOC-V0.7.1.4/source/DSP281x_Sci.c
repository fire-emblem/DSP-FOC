// TI File $Revision: /main/2 $
// Checkin $Date: April 29, 2005   11:08:40 $
//###########################################################################
//
// FILE:	DSP281x_Sci.c
//
// TITLE:	DSP281x SCI Initialization & Support Functions.
//
//###########################################################################
// $TI Release: DSP281x C/C++ Header Files V1.20 $
// $Release Date: July 27, 2009 $
//###########################################################################

#include "DSP281x_Device.h"     // DSP281x Headerfile Include File
#include "DSP281x_Examples.h"   // DSP281x Examples Include File
unsigned int * UART_MODE = (unsigned int *) 0x4010;

Uint16 speed_dis=0;
void InitSci(void)
{
	// Initialize SCI-B:
	*UART_MODE = 0x44;
	
 
	/* loopback   8 bit data */
	ScibRegs.SCICCR.all = 0x07;
	
	ScibRegs.SCICTL1.all = 0x03;
	ScibRegs.SCICTL2.all = 0x03;
	//Baud = LSPCLK/((BBR+1)*8)
	//(6000000/(9600*8))-1=77.125
	//389.625 -->0x0185
	ScibRegs.SCIHBAUD = 0x01;
	ScibRegs.SCILBAUD = 0xE7;
	
	ScibRegs.SCICTL1.all = 0x23;
	
	PieCtrlRegs.PIEIER9.bit.INTx3 = 1;
	PieCtrlRegs.PIEIER9.bit.INTx4 = 0;
	
}

void ShowDisp(void)       //������ʾ
{
	static unsigned int i=0;
	while(ScibRegs.SCICTL2.bit.TXRDY!=1);
    if(RS232_CNT>400)
    {
	switch(i)
	{
	case 0://��ʾ����
		ScibRegs.SCITXBUF =i;  
        while(ScibRegs.SCICTL2.bit.TXRDY!=1);
        if(IPM_Fault==1)
        {
		ScibRegs.SCITXBUF =12;//����
        }
        else if(O_Current==1)
        {
            ScibRegs.SCITXBUF =13;//����


        }
        else
        {
            ScibRegs.SCITXBUF =22;//����
        }
        while(ScibRegs.SCICTL2.bit.TXRDY!=1);
        i++;
		 RS232_CNT=0;
		break;
	case 1://��ʾA�����
	    ScibRegs.SCITXBUF =i;  
        while(ScibRegs.SCICTL2.bit.TXRDY!=1);
		ScibRegs.SCITXBUF=I_A;
        while(ScibRegs.SCICTL2.bit.TXRDY!=1);
		i++;
		 RS232_CNT=0;
		break;
	case 2://��ʾ�����ٶ�
	    ScibRegs.SCITXBUF =i;  
        while(ScibRegs.SCICTL2.bit.TXRDY!=1);
		ScibRegs.SCITXBUF=speed_dis;
        while(ScibRegs.SCICTL2.bit.TXRDY!=1);
        i++;
		 RS232_CNT=0;
		break;
	case 3://��ʾb�����
	    ScibRegs.SCITXBUF =i;  
        while(ScibRegs.SCICTL2.bit.TXRDY!=1);
	    ScibRegs.SCITXBUF=I_B;
        while(ScibRegs.SCICTL2.bit.TXRDY!=1);
		i++;
		 RS232_CNT=0;
		break;
    case 4://��ʾĸ�ߵ�ѹ
        ScibRegs.SCITXBUF =i;  
        while(ScibRegs.SCICTL2.bit.TXRDY!=1);
	    ScibRegs.SCITXBUF=U_dc_dis>>8;//��
	    while(ScibRegs.SCICTL2.bit.TXRDY!=1);
        ScibRegs.SCITXBUF=U_dc_dis;
        
        while(ScibRegs.SCICTL2.bit.TXRDY!=1);
		i++;
		 RS232_CNT=0;
		break;
    case 5://��ʾ�����ٶȰٷֱ�
        ScibRegs.SCITXBUF =i;  
        while(ScibRegs.SCICTL2.bit.TXRDY!=1);
        ScibRegs.SCITXBUF =speed_give;
        while(ScibRegs.SCICTL2.bit.TXRDY!=1);
        i++;
		 RS232_CNT=0;
        break;
         case 6://���� 
        ScibRegs.SCITXBUF =i;  
        while(ScibRegs.SCICTL2.bit.TXRDY!=1);
	    ScibRegs.SCITXBUF=BaseSpeed>>8;//��
	    while(ScibRegs.SCICTL2.bit.TXRDY!=1);
        ScibRegs.SCITXBUF=BaseSpeed;
        
        while(ScibRegs.SCICTL2.bit.TXRDY!=1);
        i=0;
		 RS232_CNT=0;
        break;
	default:
		i=0;
        RS232_CNT=0;
		break;
	}
   
    }
	
	
}
	
//===========================================================================
// No more.
//===========================================================================
