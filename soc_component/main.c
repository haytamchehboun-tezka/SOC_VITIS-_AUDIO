
#include "xparameters.h"
#include "xil_printf.h"
#include "xil_io.h"
#include "xstatus.h"

// AXI GPIO addresses
#define GPIO_INPUT_BASE   XPAR_AXI_GPIO_0_BASEADDR
#define GPIO_OUTPUT_BASE  XPAR_AXI_GPIO_1_BASEADDR

// GPIO register offsets
#define GPIO_DATA_OFFSET   0x00
#define GPIO_TRI_OFFSET    0x04
#define GPIO2_DATA_OFFSET  0x08
#define GPIO2_TRI_OFFSET   0x0C

#define MASK_4BITS 0x0F

int main(void)
{
    u32 buttons;
    u32 switches;
    u32 leds;

    xil_printf("\r\n");
    xil_printf("============================\r\n");
    xil_printf(" BUTTON / SWITCH LED TEST\r\n");
    xil_printf("============================\r\n");

    // Configure buttons as inputs (GPIO channel 1)
    Xil_Out32(GPIO_INPUT_BASE + GPIO_TRI_OFFSET, MASK_4BITS);

    // Configure switches as inputs (GPIO channel 2)
    Xil_Out32(GPIO_INPUT_BASE + GPIO2_TRI_OFFSET, MASK_4BITS);

    // Configure LEDs as outputs
    Xil_Out32(GPIO_OUTPUT_BASE + GPIO_TRI_OFFSET, 0x0);

    // Initially turn off all LEDs
    Xil_Out32(GPIO_OUTPUT_BASE + GPIO_DATA_OFFSET, 0x0);

    xil_printf("GPIO initialized successfully\r\n");
    xil_printf("Move switches or press buttons!\r\n");

    while (1)
    {
        // Read buttons
        buttons = Xil_In32(
            GPIO_INPUT_BASE + GPIO_DATA_OFFSET
        ) & MASK_4BITS;

        // Read switches
        switches = Xil_In32(
            GPIO_INPUT_BASE + GPIO2_DATA_OFFSET
        ) & MASK_4BITS;

        // Combine buttons and switches
        // A LED lights if its switch OR its button is active
        leds = buttons | switches;

        // Update LEDs
        Xil_Out32(
            GPIO_OUTPUT_BASE + GPIO_DATA_OFFSET,
            leds
        );
    }

    return XST_SUCCESS;
}
