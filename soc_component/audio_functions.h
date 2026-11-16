#ifndef AUDIO_FUNCTIONS_H
#define AUDIO_FUNCTIONS_H

/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */

#include "xaxidma.h"
#include "xiicps.h"
#include "xil_types.h"

/* IIC address of the SSM2603 device and the desired IIC clock speed */
#define IIC_SLAVE_ADDR		0b0011010
#define IIC_SCLK_RATE		100000

/* ------------------------------------------------------------ */
/*					Procedure Declarations						*/
/* ------------------------------------------------------------ */

int Init_IIC(XIicPs *IicInstance, UINTPTR BaseAddr);
int CodecRegSet(XIicPs *IicInstance, u8 RegAddr, u16 Data);
int init_codecReg(XIicPs *IicInstance);
int Init_AXI_DMA(XAxiDma *DmaInstance, UINTPTR BaseAddr);

#endif // AUDIO_FUNCTIONS_H