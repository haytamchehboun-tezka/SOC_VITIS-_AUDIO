/************************************************************************/
/*																		*/
/*	audio_functions.c	--	ZYBO Audio demonstration 						    */
/*																		*/
/************************************************************************/
/*                                                                      */
/*  Module Description: 												*/
/*																		*/
/*		This file contains code for running the	SSM2603 Audio codec	    */
/*		the ZYBO.														*/
/************************************************************************/

/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */

#include "audio_functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "xparameters.h"
#include "xil_types.h"
#include "xil_io.h"
#include "xil_printf.h"
#include "sleep.h"

/* Redefine the XPAR constants */
#define IIC_ADDR		XPAR_XIICPS_0_BASEADDR
#define I2S_ADDRESS 	XPAR_AXI_I2S_ADI_0_BASEADDR
#define TIMER_ADDR 	    XPAR_SCUTIMER_BASEADDR

/* ------------------------------------------------------------ */
/*				Procedure Definitions							*/
/* ------------------------------------------------------------ */

/*	Init_Codec(XIicPs IicInstance, UINTPTR TimerAddr, UINTPTR iicAddr)
**
**	Parameters:
**      *IicInstance - Pointer to the initialized XIicPs struct
**		iicAddr 	- Base address for the PS IIC controller connected to the SSM2603
**
**	Return Value: int
**		XST_SUCCESS if successful
**
**	Errors:
**
**	Description:
**		Initializes the I2C communication protocol. Must be called once and only once.
**
*/
int Init_IIC(XIicPs *IicInstance, UINTPTR BaseAddr) {
	int Status;
	XIicPs_Config *Config;

	Config = XIicPs_LookupConfig(BaseAddr);
	if (NULL == Config) {
        xil_printf("Error: I2C Configuration not found (BaseAddr: 0x%X)\r\n", BaseAddr);
		return XST_FAILURE;
	}

	Status = XIicPs_CfgInitialize(IicInstance, Config, Config->BaseAddress);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Perform a self-test to ensure that the hardware was built correctly.
	 */
	Status = XIicPs_SelfTest(IicInstance);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Set the IIC serial clock rate.
	 */
	Status = XIicPs_SetSClk(IicInstance, IIC_SCLK_RATE);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

    xil_printf("I2C initialized successfully.\r\n");
    return XST_SUCCESS;
}


/* Init_Codec(XIicPs IicInstance, UINTPTR TimerAddr, UINTPTR iicAddr)
**
**	Parameters:
**		IicInstance - Pointer to the initialized XIicPs struct
**		regAddr - Register in the SSM2603 to write to
**		regData - Data to write to the register (lower 9 bits are used)
**
**	Return Value: int
**		XST_SUCCESS if successful
**
**	Errors:
**
**	Description:
**		Writes a value to a register in the SSM2603 device over IIC.
**
*/

int CodecRegSet(XIicPs *IicInstance, u8 regAddr, u16 regData)
{
	int Status;
	u8 SendBuffer[2];

	SendBuffer[0] = regAddr << 1;
	SendBuffer[0] = SendBuffer[0] | ((regData >> 8) & 0b1);

	SendBuffer[1] = regData & 0xFF;

	Status = XIicPs_MasterSendPolled(IicInstance, SendBuffer, 2, IIC_SLAVE_ADDR);
	if (Status != XST_SUCCESS) {
		xil_printf("IIC send failed\n\r");
		return XST_FAILURE;
	}
	/*
	 * Wait until bus is idle to start another transfer.
	 */
	while (XIicPs_BusIsBusy(IicInstance)) {
		/* NOP */
	}
	return XST_SUCCESS;
}

/*	init_codecReg(XIicPs *IicInstance, UINTPTR timerAddr, u16 regData)
**
**	Parameters:
**		IicInstance - Pointer to the initialized XIicPs struct
**		timerAddr - SCU Timer Base Address
**
**	Return Value: int
**		XST_SUCCESS if successful
**
**	Errors:
**
**	Description:
**		Writes the boot sequence of the Audio Codec. Must be called once and only once.
**
*/
int init_codecReg(XIicPs *IicInstance){
	int Status;

	/*
	 * Write to the SSM2603 audio codec registers to configure the device. Refer to the
	 * SSM2603 Audio Codec data sheet for information on what these writes do.
	 */
	Status = CodecRegSet(IicInstance, 15, 0b000000000); //Perform Reset
	usleep(75000);
	Status |= CodecRegSet(IicInstance, 6, 0b000110000); // Power up with DAC disabled to avoid parasitic noise
	Status |= CodecRegSet(IicInstance, 0, 0b000010111); // Registers setting
	Status |= CodecRegSet(IicInstance, 1, 0b000010111); // cont'd
	Status |= CodecRegSet(IicInstance, 2, 0b001111001); // cont'd
	Status |= CodecRegSet(IicInstance, 3, 0b001111001); // cont'd
	Status |= CodecRegSet(IicInstance, 4, 0b000001010); // Bypass activation
	Status |= CodecRegSet(IicInstance, 5, 0b000000000); // cont'd
	Status |= CodecRegSet(IicInstance, 7, 0b000001010); // Word length is 24
	Status |= CodecRegSet(IicInstance, 8, 0b000000000); // No CLKDIV2
	usleep(75000);
	Status |= CodecRegSet(IicInstance, 9, 0b000000001); // Digital core activated
	Status |= CodecRegSet(IicInstance, 6, 0b000100000); // DAC activation

	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	return XST_SUCCESS;
}

/*	Init_AXI_DMA(XAxiDma *DmaInstance, UINTPTR BaseAddr)
**
**	Parameters:
**		DmaInstance - Pointer to the initialized XDMA struct
**		BaseAddr - DMA Base Address
**
**	Return Value: int
**		XST_SUCCESS if successful
**
**	Errors:
**
**	Description:
**		Initializes the AXI DMA structure
**
*/

int Init_AXI_DMA(XAxiDma *DmaInstance, UINTPTR BaseAddr) {
    XAxiDma_Config *Config;
    int Status;

    Config = XAxiDma_LookupConfig(BaseAddr);
    if (!Config) {
        xil_printf("Error: AXI DMA Configuration not found (BaseAddr: 0x%X)\r\n", BaseAddr);
        return XST_FAILURE;
    }

    Status = XAxiDma_CfgInitialize(DmaInstance, Config);
    if (Status != XST_SUCCESS) {
        xil_printf("Error: AXI DMA Initialization failed\r\n");
        return XST_FAILURE;
    }

    // Disable interrupts to start in simple polling mode
    XAxiDma_IntrDisable(DmaInstance, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DMA_TO_DEVICE);
    XAxiDma_IntrDisable(DmaInstance, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DEVICE_TO_DMA);

    xil_printf("AXI DMA initialized successfully.\r\n");
    return XST_SUCCESS;
}
