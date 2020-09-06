//io_handler.c
#include "io_handler.h"
#include <stdio.h>

void io_init(void)
{
	*otg_hpi_reset = 1;
	*otg_hpi_cs = 1;
	*otg_hpi_r = 1;
	*otg_hpi_w = 1;
	*otg_hpi_address = 0;
	*otg_hpi_data = 0;
	// Reset OTG chip
	*otg_hpi_cs = 0;
	*otg_hpi_reset = 0;
	*otg_hpi_reset = 1;
	*otg_hpi_cs = 1;
}
/**
 *
 * This function is a helper function to write into the OTG chip
 *
 * @param    Address is the address of the register,
 * 			 Data is the data needed to be written into the address
 *
 * @return   None
 *
 * @note     None
 *
 ******************************************************************************/
void io_write(alt_u8 Address, alt_u16 Data)
{
	*otg_hpi_address = Address;
	*otg_hpi_cs = 0; // Set chip select
	*otg_hpi_w = 0;
	*otg_hpi_data = Data;
	*otg_hpi_w = 1;
	*otg_hpi_cs = 1;
}
/**
 *
 * This function is a helper function to write into the OTG chip
 *
 * @param    Address is the address of the register.
 *
 * @return   The data read from the specified address
 *
 * @note     None
 *
 ******************************************************************************/
alt_u16 io_read(alt_u8 Address)
{
	alt_u16 temp;
	*otg_hpi_address = Address;
	*otg_hpi_cs = 0; // Set chip select
	*otg_hpi_r = 0;
	temp = *otg_hpi_data;
	*otg_hpi_r = 1; // Set the read signal back to 1 after reading
	//printf("%x\n",temp);
	*otg_hpi_cs = 1;
	return temp;
}
