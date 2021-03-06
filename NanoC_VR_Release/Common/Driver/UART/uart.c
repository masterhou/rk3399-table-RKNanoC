/*
********************************************************************************
*                   Copyright (c) 2008, Rock-Chips
*                         All rights reserved.
*
* File Name：   Uart.c
* 
* Description:  C program template
*
* History:      <author>          <time>        <version>       
*               
*    desc:    ORG.
********************************************************************************
*/

#define  IN_DRIVER_UART

#include "DriverInclude.h"

#define _IN_UART_

#ifdef DRIVERLIB_IRAM
/*
--------------------------------------------------------------------------------
  Function name : void UARTRest(void)
  Author        : 
  Description   : 
                  
  Input         : 
                  
  Return        : 

  History:     <author>         <time>         <version>       
              
  desc:       
--------------------------------------------------------------------------------
*/
_ATTR_DRIVERLIB_CODE_
void UARTRest(void)
{
    UartReg->UART_SRR = UART_RESET | RCVR_FIFO_REST | XMIT_FIFO_RESET;
    UartReg->UART_IER = 0;
    UartReg->UART_DMASA = 1;
}

/*
--------------------------------------------------------------------------------
  Function name : int32 UARTSetIOP(uint8 useIrDA)
  Author        : 
  Description   : set uart role as serial port or IRDA.
                  
  Input         : 
                  
  Return        : 0:OK -1:fail

  History:     <author>         <time>         <version>       
              
  desc:       
--------------------------------------------------------------------------------
*/
_ATTR_DRIVERLIB_CODE_
int32 UARTSetIOP(uint8 useIrDA)
{
    //pUART_REG   phwHead   = (pUART_REG)UART1_BASE_ADDR;
    
    if ((useIrDA == IRDA_SIR_DISABLED) || (useIrDA == IRDA_SIR_ENSABLED))
    {
        UartReg->UART_MCR = useIrDA;
        return (0);
    }
    
    return (-1);
}

/*
--------------------------------------------------------------------------------
  Function name : UARTSetBaudRate(pUART_REG phead, uint32 baudRate)
  Author        : 
  Description   : 
                  baudRate:serial initial speed,get by table seek,
  Input         : 
                  
  Return        : 0:OK -1:fail

  History:     <author>         <time>         <version>       
              
  desc:       
--------------------------------------------------------------------------------
*/
_ATTR_DRIVERLIB_CODE_
int32 UARTSetBaudRate(uint32 clk, uint32 baudRate)
{
    uint32  DivLatch;
    
    //  DLL and THR(RBR) are mapped the same offset, set LCR[7] to access to
    //  DLL register, otherwise the THR(RBR)
    
    DivLatch = clk / MODE_X_DIV / baudRate;

    UartReg->UART_LCR |= LCR_DLA_EN;
    
    UartReg->UART_DLL  = (DivLatch >> 0) & 0xff;
    UartReg->UART_DLH  = (DivLatch >> 8) & 0xff;
    
    UartReg->UART_LCR &= (~LCR_DLA_EN); 

    return (0);
}

/*
--------------------------------------------------------------------------------
  Function name : int32 UARTSetLcrReg(uint8 byteSize, uint8 parity,uint8 stopBits )
  Author        : 
  Description   : 
                  
  Input         : 
                  
  Return        : 0:OK -1:fail

  History:     <author>         <time>         <version>       
              
  desc:       
--------------------------------------------------------------------------------
*/
_ATTR_DRIVERLIB_CODE_
int32 UARTSetLcrReg(uint8 byteSize, uint8 parity,uint8 stopBits )
{
    uint32 lcr;
    int32  bRet = 0;

    lcr = (UartReg->UART_LCR) & (~(uint32)(UART_DATABIT_MASK));

    //Data length select
    switch ( byteSize )    
    {
        case UART_DATA_5B:
            lcr |= LCR_WLS_5;
            break;
        case UART_DATA_6B:
            lcr |= LCR_WLS_6;
            break;
        case UART_DATA_7B:
            lcr |= LCR_WLS_7;
            break;
        case UART_DATA_8B:
            lcr |= LCR_WLS_8;
            break;
        default:
            bRet = -1;
            break;
    }

    //Parity set
    switch ( parity ) 
    {
        case UART_ODD_PARITY:
        case UART_EVEN_PARITY:
            lcr |= PARITY_ENABLED;
            lcr |= ((parity) << 4);
            break;
        case UART_PARITY_DISABLE:
            lcr &= ~PARITY_ENABLED;
            break;
        default:
            bRet = -1;
            break;
    }

    //StopBits set
    switch ( stopBits ) 
    {
        case 0:
            lcr |= ONE_STOP_BIT;
            break;
        case 1:
            lcr |= ONE_HALF_OR_TWO_BIT;
            break;
        default:
            bRet = -1;
            break;
    }

    if (bRet == 0)
    {
        UartReg->UART_LCR = lcr;
    }
    
    return(bRet);
}

/*
--------------------------------------------------------------------------------
  Function name : UARTSetFifoEnabledNumb()
  Author        : 
  Description   : 
                  
  Input         : 
                  
  Return        : 0:OK -1:fail

  History:     <author>         <time>         <version>       
              
  desc:       
--------------------------------------------------------------------------------
*/
_ATTR_DRIVERLIB_CODE_
void UARTSetFifoEnabledNumb(uint32 param)
{
    UartReg->UART_SFE  = (param & 0x01);
    UartReg->UART_SRT  = (param >> 1) & 0x03;
    UartReg->UART_STET = (param >> 3) & 0x03;
}

/*
--------------------------------------------------------------------------------
  Function name : void UARTSetIntEnabled(uint32 uartIntNumb)
  Author        : 
  Description   : 设置UART口的中断使能打开。
                  
  Input         : uartIntNumb：UART的中断号
                  
  Return        : 0:OK -1:fail

  History:     <author>         <time>         <version>       
              
  desc:       
--------------------------------------------------------------------------------
*/
_ATTR_DRIVERLIB_CODE_
void UARTSetIntEnabled(uint32 uartIntNumb)
{
    UartReg->UART_IER |= uartIntNumb;
}

/*
--------------------------------------------------------------------------------
  Function name : void UARTSetIntDisabled(uint32 uartIntNumb)
  Author        : 
  Description   : 关中断使能
                  
  Input         : uartIntNumb：UART的中断号
                  
  Return        : 0:OK -1:fail

  History:     <author>         <time>         <version>       
              
  desc:       
--------------------------------------------------------------------------------
*/
_ATTR_DRIVERLIB_CODE_
void UARTSetIntDisabled(uint32 uartIntNumb)
{
    UartReg->UART_IER &= (~uartIntNumb);
}

/*
--------------------------------------------------------------------------------
  Function name : UARTWriteByte(uint8 ch)
  Author        : 
  Description   : 串口写一个字节
                  
  Input         : byte:输入的字节值
                  uartTimeOut:等待延时时间
                  
  Return        : 0:OK -1:fail

  History:     <author>         <time>         <version>       
              
  desc:       
--------------------------------------------------------------------------------
*/
_ATTR_DRIVERLIB_CODE_
int32 UARTWriteByte(uint8 byte, uint32 uartTimeOut)
{
    while ((UartReg->UART_USR & UART_TRANSMIT_FIFO_NOT_FULL) != UART_TRANSMIT_FIFO_NOT_FULL)
    {
        if (uartTimeOut == 0)
        {
            return (-1);
        }
        uartTimeOut--;
    }
    
    UartReg->UART_THR = byte;
    
    return (0);
}

/*
--------------------------------------------------------------------------------
  Function name : int32 UARTReadByte(uint8 *pdata, uint32 uartTimeOut)
  Author        : 
  Description   : 串口读取一个字节
                  
  Input         : 
                  uartTimeOut:等待延时时间
                  
  Return        : 0:OK -1:fail

  History:     <author>         <time>         <version>       
              
  desc:       
--------------------------------------------------------------------------------
*/
_ATTR_DRIVERLIB_CODE_
int32 UARTReadByte(uint8 *pdata, uint32 uartTimeOut)
{
    while ((UartReg->UART_USR & UART_RECEIVE_FIFO_NOT_EMPTY) != UART_RECEIVE_FIFO_NOT_EMPTY)
    {
        if (uartTimeOut == 0)
        {
            return (-1);
        }
        uartTimeOut--;
    }
    
    *pdata = (uint8 )UartReg->UART_RBR;
    
    return (0);
}
#endif

/*
--------------------------------------------------------------------------------
  Function name : int32 UartDmaWrite(uint8 *pdata, uint32 size, uint32 uartTimeOut, pFunc Callback)
  Author        : 
  Description   : 通过DMA向串口写数据
                  
  Input         : pdata:输出数据指针
                  size: 输出数据大小(byte)
                  uartTimeOut:等待延时时间
                  Callback:DMA中断回调函数
                  
  Return        : 0:OK -1:fail

  History:     <author>         <time>         <version>       
              
  desc:       
--------------------------------------------------------------------------------
*/
_ATTR_DRIVER_CODE_
int32 UartDmaWrite(uint8 *pdata, uint32 size, uint32 uartTimeOut, pFunc Callback)
{
    int32 channel;    
    DMA_CFGX DmaCfg = {DMA_CTLL_M2UART,DMA_CFGL_M2UART,DMA_CFGH_M2UART,0};
    
    while ((UartReg->UART_USR & UART_TRANSMIT_FIFO_NOT_FULL) != UART_TRANSMIT_FIFO_NOT_FULL)
    {
        if (uartTimeOut == 0)
        {
            return (-1);
        }
        uartTimeOut--;
    }
    
    channel = DmaGetChannel();
    if (channel != DMA_FALSE)
    {
        DmaStart((uint32)(channel), (UINT32)(pdata),(uint32)(&(UartReg->UART_THR)),size, &DmaCfg, Callback);
        return channel;
    }
    
    return (-1);
}

/*
--------------------------------------------------------------------------------
  Function name : int32 UartDmaRead(uint8 *pdata, uint32 size, uint32 uartTimeOut, pFunc Callback)
  Author        : 
  Description   : read the uart data by dma.
                  
  Input         : pdata:input data pointer.
                  size: input data size.(byte)
                  uartTimeOut: waiting delay time.
                  Callback:DMA interrupt callback.
                  
  Return        : 0:OK -1:fail

  History:     <author>         <time>         <version>       
              
  desc:       
--------------------------------------------------------------------------------
*/
_ATTR_DRIVER_CODE_
int32 UartDmaRead(uint8 *pdata, uint32 size, uint32 uartTimeOut, pFunc Callback)
{
    eDMA_CHN channel;    
    DMA_CFGX DmaCfg = {DMA_CTLL_UART2M,DMA_CFGL_UART2M,DMA_CFGH_UART2M,0};
    
    while ((UartReg->UART_USR & UART_TRANSMIT_FIFO_NOT_FULL) != UART_TRANSMIT_FIFO_NOT_FULL)
    {
        if (uartTimeOut == 0)
        {
            return (-1);
        }
        uartTimeOut--;
    }
    
    channel = DmaGetChannel();
    if (channel != DMA_FALSE)
    {
        DmaStart((uint32)(channel), (uint32)(&(UartReg->UART_RBR)),(UINT32)(pdata),size, &DmaCfg, Callback);
        return channel;
    }
    
    return (-1);
}

#ifndef DRIVERLIB_IRAM
/*
--------------------------------------------------------------------------------
  Function name : int32 UARTInit(eUART_BaudRate baudRate,eUART_dataLen dataBit,eUART_stopBit stopBit,eUART_parity_en parity)
  Author        : 
  Description   : 
                  
  Input         : 
                  
  Return        : 0:OK -1:fail

  History:     <author>         <time>         <version>       
              
  desc:       
--------------------------------------------------------------------------------
*/
_ATTR_DRIVERLIB_CODE_
int32 UARTInit(uint32 clk,eUART_BaudRate baudRate,eUART_dataLen dataBit,eUART_stopBit stopBit,eUART_parity_en parity)
{
    UARTRest();
    DelayMs(10);
    
    SetUartFreq(clk/(1000*1000));
    DelayMs(10);

    UARTSetIOP(IRDA_SIR_DISABLED);

    UARTSetFifoEnabledNumb(SHADOW_FIFI_ENABLED | RCVR_TRIGGER_HALF_FIFO | TX_TRIGGER_TWO_IN_FIFO);

    UARTSetLcrReg(dataBit,parity,stopBit);
    
    UARTSetBaudRate(clk, baudRate);
    
    return (0);
}
#endif

#define DEFAULT_INPUT_BUFFER_SIZE                                128
#define DEFAULT_OUTPUT_BUFFER_SIZE                               64
#define XOFF_LIMIT                                               32
#define XON_LIMIT                                                128

typedef struct _tagUartContext_t
{
//   unsigned char          ID;
//   unsigned long          UartBase;
//   unsigned char          RxBuffer[DEFAULT_INPUT_BUFFER_SIZE];
//
//   int                    RxBufferSize;
//   volatile int           RxBytesFree;
//   int                    RxInIndex;
//   int                    RxOutIndex;
//   int                    XOffLimit;
//   int                    XOnLimit;
     unsigned char          TxBuffer[1024];
     unsigned char          *TxBufferP;

     int                    TxBufferSize;
//   volatile int           TxBytesFree;
//   int                    TxInIndex;
     int                      TxOutIndex;
//   unsigned char          Flags;
//   HCILL_Action_Request_t HCILL_Action;
//   Byte_t                 HCILL_Byte;
     unsigned char          isInTx;
     unsigned char          unused[3];
   void (* recv)(uint8 data);
   void (* tx_complete)(void);
   
} UartContext_t;
_ATTR_DRIVER_BSS_ static UartContext_t  UartContext;

_ATTR_DRIVER_CODE_
int BT_UARTInit(uint32 clk,eUART_BaudRate baudRate,eUART_dataLen dataBit,eUART_stopBit stopBit,eUART_parity_en parity)
{
    UARTInit(clk,baudRate,dataBit,stopBit, parity );
    UARTSetIntEnabled(UART_IE_RX );//| UART_IE_THRE);
    UartReg->UART_IIR = 0;
  //  UartReg->UART_MCR = (1 << 5) |(1 << 1) ;
       
    memset(&UartContext, 0 , sizeof(UartContext_t));
}

_ATTR_DRIVER_CODE_
void BT_Recv_Register(void (*func)(uint data))
{
    UartContext.recv = func;
}

_ATTR_DRIVER_CODE_
void BT_Tx_Complete_Register(void (*func)(void))
{
    UartContext.tx_complete = func;
}

_ATTR_DRIVER_CODE_
uint8 get_send_state(void)
{
    return UartContext.isInTx;
}


_ATTR_DRIVER_CODE_
void BT_UARTSend(char * data, int len)
{
    memcpy(UartContext.TxBuffer, data, len);
   // while(UartContext.isInTx);

    UartContext.isInTx = 1;
    //UartContext.TxBufferP = data;
    UartContext.TxBufferP = UartContext.TxBuffer;
    UartContext.TxBufferSize = len;
    UartContext.TxOutIndex = 0;
    UARTSetIntEnabled(UART_IE_TX);

    //UartContext.TxOutIndex++;
    //UartReg->UART_THR = UartContext.TxBuffer[0];
    //UartReg->UART_IIR = UartReg->UART_IIR | UART_IF_THR_EMPTY;

}

_ATTR_DRIVER_CODE_
void BT_UARTInterrupt(void)
{   
    uint32 intstatus;
    uint8  recvdata;

    intstatus = UartReg->UART_IIR & UART_IF_MASK;
    UartReg->UART_IIR = 0x01;
    
    switch(intstatus)
    {
        case UART_IF_REC_DATA:
            if(UartContext.recv != NULL)
            {
                //while ((UartReg->UART_USR & UART_RECEIVE_FIFO_NOT_EMPTY) == UART_RECEIVE_FIFO_NOT_EMPTY)
                {
                    recvdata = (uint8)UartReg->UART_RBR;
                    UartContext.recv(recvdata);
                }
            }
            break;
            
        case UART_IF_THR_EMPTY:
            //while ((UartReg->UART_USR & UART_TRANSMIT_FIFO_NOT_FULL) == UART_TRANSMIT_FIFO_NOT_FULL)
            {
                if(UartContext.TxBufferSize != UartContext.TxOutIndex)
                {
                    UartReg->UART_THR = UartContext.TxBufferP[UartContext.TxOutIndex];
                    UartContext.TxOutIndex++;
                }
                else
                {
                    UARTSetIntDisabled(UART_IE_TX);
                    UartContext.isInTx = 0;
                    if(UartContext.tx_complete != NULL)
                    {
                        UartContext.tx_complete();
                    }
                    break;
                }
            }
            break;

        default:
            break;
    }
        
}

/*      
********************************************************************************
*       
*                         End of Uart.c
*       
********************************************************************************
*/      
      
