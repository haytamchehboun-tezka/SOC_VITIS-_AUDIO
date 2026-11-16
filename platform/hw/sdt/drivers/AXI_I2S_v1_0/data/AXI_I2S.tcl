

proc generate {drv_handle} {
	xdefine_include_file $drv_handle "xparameters.h" "AXI_I2S" "NUM_INSTANCES" "DEVICE_ID"  "C_S02_AXI_BASEADDR" "C_S02_AXI_HIGHADDR"
}
