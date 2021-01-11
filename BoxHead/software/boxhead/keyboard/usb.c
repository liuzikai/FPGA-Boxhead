/*---------------------------------------------------------------------------
  --      usb.c                                                    	   --
  --      Christine Chen                                                   --
  --      Ref. DE2-115 Demonstrations by Terasic Technologies Inc.         --
  --      Fall 2014                                                        --
  --                                                                       --
  --      For use with ECE 298 Experiment 7                                --
  --      UIUC ECE Department                                              --
  ---------------------------------------------------------------------------*/

#include "usb.h"

#include "system.h"
#include "alt_types.h"
#include <unistd.h>  // usleep
#include <stdio.h>
//#include <io.h>

#include "io_handler.h"
#include "cy7c67200.h"
#include "lcp_cmd.h"
#include "lcp_data.h"

//#define CY7C67200_0_BASE CY7C67200_IF_0_BASE

//-------------USB operation sub function-----------
/*****************************************************************************/
/**
 *
 * This function writes data to the internal registers of the Cypress
 * CY7C67200 USB controller.
 *
 * @param    Address is the address of the register.
 * @param    Data is the data to be written to the register.
 *
 * @return   None
 *
 * @note     None
 *
 ******************************************************************************/
void usb_write(alt_u16 Address, alt_u16 Data) {
    io_write(HPI_ADDR, Address);
    io_write(HPI_DATA, Data);
}

/*****************************************************************************/
/**
 *
 * This function reads data from the internal registers of the Cypress
 * CY7C67200 USB controller.
 *
 * @param    Address is the address of the register.
 *
 * @return   The data read from the specified address
 *
 * @note     None
 *
 ******************************************************************************/
alt_u16 usb_read(alt_u16 Address) {
    // Write the address into HPI address port register
    io_write(HPI_ADDR, Address);
    return io_read(HPI_DATA);
}


/*****************************************************************************/
/**
 *
 * This function does a software reset of the Cypress CY7C67200 USB controller.
 *
 * @param    UsbBaseAddress is the starting location of the USB internal memory
 *           to which this bin file data is written.
 *
 * @return   None
 *
 * @note     None
 *
 ******************************************************************************/
void usb_soft_reset() {
    //XIo_Out16(USB_MAILBOX, COMM_RESET);
    io_write(HPI_MAILBOX, COMM_RESET); //COMM_JUMP2CODE
    usleep(100000);
    printf("[USB INIT]:reset finished!\n");

    usleep(500000);
    printf("[USB INIT]:Clear up the interrupt\r\n");
    io_read(HPI_MAILBOX);
    io_read(HPI_STATUS);

    // Had to add the write due to a bug in BIOS where they overwrite
    // the mailbox after initialization with garbage.  The read clears
    // any pending interrupts.
    usb_read(HPI_SIE1_MSG_ADR);
    usb_write(HPI_SIE1_MSG_ADR, 0);
    usb_read(HPI_SIE2_MSG_ADR);
    usb_write(HPI_SIE2_MSG_ADR, 0);


    usb_write(HOST1_STAT_REG, 0xFFFF);
    usb_write(HOST2_STAT_REG, 0xFFFF);
}


void usb_set_address() {
    //the starting address
    io_write(HPI_ADDR, 0x0500); //the start address
    // TD #1: 6 writes
    io_write(HPI_DATA, 0x050C);
    io_write(HPI_DATA, 0x0008); //4 port number
    // TASK: Complete with 4 more IOWR functions
    io_write(HPI_DATA, 0x00D0);
    io_write(HPI_DATA, 0x0001);
    io_write(HPI_DATA, 0x0013);
    io_write(HPI_DATA, 0x0514);
    // TD #2: 4 writes
    // TASK: Complete with 4 IOWR functions
    io_write(HPI_DATA, 0x0500);
    io_write(HPI_DATA, 0x0002);
    io_write(HPI_DATA, 0x0000);
    io_write(HPI_DATA, 0x0000);
    // TD #3: 6 writes
    // TASK: Complete with 6 IOWR functions
    io_write(HPI_DATA, 0x0000); //
    io_write(HPI_DATA, 0x0000);//
    io_write(HPI_DATA, 0x0090);//
    io_write(HPI_DATA, 0x0041);
    io_write(HPI_DATA, 0x0013);
    io_write(HPI_DATA, 0x0000);

    usb_write(HUSB_SIE1_pCurrentTDPtr, 0x0500); //HUSB_SIE1_pCurrentTDPtr
}


void usb_get_device_desc1() {
    //the starting address
    io_write(HPI_ADDR, 0x0500); //the start address
    io_write(HPI_DATA, 0x050C);
    io_write(HPI_DATA, 0x0008); //4 port number
    io_write(HPI_DATA, 0x02D0); //device address
    io_write(HPI_DATA, 0x0001);
    io_write(HPI_DATA, 0x0013);
    io_write(HPI_DATA, 0x0514);

    //td content 4 bytes
    io_write(HPI_DATA, 0x0680);//c
    io_write(HPI_DATA, 0x0100); //device 0x01
    io_write(HPI_DATA, 0x0000);
    io_write(HPI_DATA, 0x0008);

    //data phase IN
    io_write(HPI_DATA, 0x052C); //
    io_write(HPI_DATA, 0x0008);//
    io_write(HPI_DATA, 0x0290);//
    io_write(HPI_DATA, 0x0041);
    io_write(HPI_DATA, 0x0013);
    io_write(HPI_DATA, 0x0520);

    //    //status phase
    io_write(HPI_DATA, 0x0000); //don't care
    io_write(HPI_DATA, 0x0000);//port number
    io_write(HPI_DATA, 0x0210);//device address
    io_write(HPI_DATA, 0x0041);
    io_write(HPI_DATA, 0x0013);
    io_write(HPI_DATA, 0x0000);

    usb_write(HUSB_SIE1_pCurrentTDPtr, 0x0500); //HUSB_SIE1_pCurrentTDPtr
}

void usb_get_device_desc2() {
    //the starting address
    io_write(HPI_ADDR, 0x0500); //the start address
    io_write(HPI_DATA, 0x050C);
    io_write(HPI_DATA, 0x0008); //4 port number
    io_write(HPI_DATA, 0x02D0); //device address
    io_write(HPI_DATA, 0x0001);
    io_write(HPI_DATA, 0x0013);
    io_write(HPI_DATA, 0x0514);

    //td content 4 bytes
    io_write(HPI_DATA, 0x0680);//c
    io_write(HPI_DATA, 0x0100);//e //device 0x01
    io_write(HPI_DATA, 0x0000);//0
    io_write(HPI_DATA, 0x0012);//2

    //data phase IN-1
    io_write(HPI_DATA, 0x0544); //514
    io_write(HPI_DATA, 0x0008);//6
    io_write(HPI_DATA, 0x0290);//8
    io_write(HPI_DATA, 0x0041);//a
    io_write(HPI_DATA, 0x0013);//c
    io_write(HPI_DATA, 0x0520);//e

    //data phase IN-2
    io_write(HPI_DATA, 0x054c); //520
    io_write(HPI_DATA, 0x0008);//2
    io_write(HPI_DATA, 0x0290);//4
    io_write(HPI_DATA, 0x0001);//6
    io_write(HPI_DATA, 0x0013);//8
    io_write(HPI_DATA, 0x052c);//a

    //data phase IN-3
    io_write(HPI_DATA, 0x0554); //c
    io_write(HPI_DATA, 0x0002);//e
    io_write(HPI_DATA, 0x0290);//530
    io_write(HPI_DATA, 0x0041);//2
    io_write(HPI_DATA, 0x0013);//4
    io_write(HPI_DATA, 0x0538);//6

    //status phase
    io_write(HPI_DATA, 0x0000); //538
    io_write(HPI_DATA, 0x0000);//a
    io_write(HPI_DATA, 0x0210);//c
    io_write(HPI_DATA, 0x0041);//e
    io_write(HPI_DATA, 0x0013);//540
    io_write(HPI_DATA, 0x0000);//2

    usb_write(HUSB_SIE1_pCurrentTDPtr, 0x0500); //HUSB_SIE1_pCurrentTDPtr
}


void usb_get_config_desc1() {
    //the starting address
    io_write(HPI_ADDR, 0x0500); //the start address
    io_write(HPI_DATA, 0x050C);
    io_write(HPI_DATA, 0x0008); //4 port number
    io_write(HPI_DATA, 0x02D0); //device address
    io_write(HPI_DATA, 0x0001);
    io_write(HPI_DATA, 0x0013);
    io_write(HPI_DATA, 0x0514);

    //td content 4 bytes
    io_write(HPI_DATA, 0x0680);//c
    io_write(HPI_DATA, 0x0200);//e //config 0x02
    io_write(HPI_DATA, 0x0000);//0
    io_write(HPI_DATA, 0x0009);//2

    //data phase IN-1
    io_write(HPI_DATA, 0x0544); //514
    io_write(HPI_DATA, 0x0008);//6
    io_write(HPI_DATA, 0x0290);//8
    io_write(HPI_DATA, 0x0041);//a
    io_write(HPI_DATA, 0x0013);//c
    io_write(HPI_DATA, 0x0520);//e

    //data phase IN-2
    io_write(HPI_DATA, 0x054c); //520
    io_write(HPI_DATA, 0x0001);//2
    io_write(HPI_DATA, 0x0290);//4
    io_write(HPI_DATA, 0x0001);//6 //data0
    io_write(HPI_DATA, 0x0013);//8
    io_write(HPI_DATA, 0x052c);//a

    //status phase
    io_write(HPI_DATA, 0x0000); //52c
    io_write(HPI_DATA, 0x0000);//e
    io_write(HPI_DATA, 0x0210);//530
    io_write(HPI_DATA, 0x0041);//2
    io_write(HPI_DATA, 0x0013);//4
    io_write(HPI_DATA, 0x0000);//6

    usb_write(HUSB_SIE1_pCurrentTDPtr, 0x0500); //HUSB_SIE1_pCurrentTDPtr
}


void usb_get_config_desc2() {
    //the starting address
    io_write(HPI_ADDR, 0x0500); //the start address
    io_write(HPI_DATA, 0x050C);
    io_write(HPI_DATA, 0x0008); //4 port number
    io_write(HPI_DATA, 0x02D0); //device address
    io_write(HPI_DATA, 0x0001);
    io_write(HPI_DATA, 0x0013);
    io_write(HPI_DATA, 0x0514);

    //td content 4 bytes
    io_write(HPI_DATA, 0x0680);//c
    io_write(HPI_DATA, 0x0200);//e //config 0x02
    io_write(HPI_DATA, 0x0000);//0
    io_write(HPI_DATA, 0x00FF);//2

    //data phase IN-1
    io_write(HPI_DATA, 0x055c); //514
    io_write(HPI_DATA, 0x0008);//6
    io_write(HPI_DATA, 0x0290);//8
    io_write(HPI_DATA, 0x0041);//a
    io_write(HPI_DATA, 0x0013);//c
    io_write(HPI_DATA, 0x0520);//e

    //data phase IN-2
    io_write(HPI_DATA, 0x0564); //520
    io_write(HPI_DATA, 0x0008);//2
    io_write(HPI_DATA, 0x0290);//4
    io_write(HPI_DATA, 0x0001);//6 //data0
    io_write(HPI_DATA, 0x0013);//8
    io_write(HPI_DATA, 0x052c);//a

    //data phase IN-3
    io_write(HPI_DATA, 0x056c); //52c
    io_write(HPI_DATA, 0x0008);//e
    io_write(HPI_DATA, 0x0290);//530
    io_write(HPI_DATA, 0x0041);//2
    io_write(HPI_DATA, 0x0013);//4
    io_write(HPI_DATA, 0x0538);//6

    //data phase IN-4
    io_write(HPI_DATA, 0x0574); //538
    io_write(HPI_DATA, 0x0008);//a
    io_write(HPI_DATA, 0x0290);//c
    io_write(HPI_DATA, 0x0001);//e //data0
    io_write(HPI_DATA, 0x0013);//540
    io_write(HPI_DATA, 0x0544);//2

    //data phase IN-5
    io_write(HPI_DATA, 0x057c); //544
    io_write(HPI_DATA, 0x0002);//6
    io_write(HPI_DATA, 0x0290);//8
    io_write(HPI_DATA, 0x0041);//a //data1
    io_write(HPI_DATA, 0x0013);//c
    io_write(HPI_DATA, 0x0550);//e

    //status phase
    io_write(HPI_DATA, 0x0000); //550
    io_write(HPI_DATA, 0x0000);//2
    io_write(HPI_DATA, 0x0210);//4
    io_write(HPI_DATA, 0x0041);//6
    io_write(HPI_DATA, 0x0013);//8
    io_write(HPI_DATA, 0x0000);//a

    usb_write(HUSB_SIE1_pCurrentTDPtr, 0x0500); //HUSB_SIE1_pCurrentTDPtr
}

void usb_set_config() {
    //the starting address
    io_write(HPI_ADDR, 0x0500); //the start address
    io_write(HPI_DATA, 0x050C);
    io_write(HPI_DATA, 0x0008); //4 port number
    io_write(HPI_DATA, 0x02D0); //port address
    io_write(HPI_DATA, 0x0001);
    io_write(HPI_DATA, 0x0013);
    io_write(HPI_DATA, 0x0514);

    //td content 4 bytes
    io_write(HPI_DATA, 0x0900);
    io_write(HPI_DATA, 0x0001);//device address
    io_write(HPI_DATA, 0x0000);
    io_write(HPI_DATA, 0x0000);
    //in packet
    io_write(HPI_DATA, 0x0000); //don't care
    io_write(HPI_DATA, 0x0000);//port number
    io_write(HPI_DATA, 0x0290);//device address
    io_write(HPI_DATA, 0x0041); //data 1
    io_write(HPI_DATA, 0x0013);
    io_write(HPI_DATA, 0x0000);

    usb_write(HUSB_SIE1_pCurrentTDPtr, 0x0500); //HUSB_SIE1_pCurrentTDPtr

}

void usb_class_request() {
    //the starting address
    io_write(HPI_ADDR, 0x0500); //the start address
    io_write(HPI_DATA, 0x050C);
    io_write(HPI_DATA, 0x0008); //4 port number
    io_write(HPI_DATA, 0x02D0); //port address
    io_write(HPI_DATA, 0x0001);
    io_write(HPI_DATA, 0x0013);
    io_write(HPI_DATA, 0x0514);

    //td content 4 bytes
    io_write(HPI_DATA, 0x0A21);
    io_write(HPI_DATA, 0x0000);//device address
    io_write(HPI_DATA, 0x0000);
    io_write(HPI_DATA, 0x0000);
    //in packet
    io_write(HPI_DATA, 0x0000); //don't care
    io_write(HPI_DATA, 0x0000);//port number /data length
    io_write(HPI_DATA, 0x0290);//device address
    io_write(HPI_DATA, 0x0041); //data 1
    io_write(HPI_DATA, 0x0013);
    io_write(HPI_DATA, 0x0000);

    usb_write(HUSB_SIE1_pCurrentTDPtr, 0x0500); //HUSB_SIE1_pCurrentTDPtr
}


void usb_get_hid_desc() {
    //the starting address
    io_write(HPI_ADDR, 0x0500); //the start address
    io_write(HPI_DATA, 0x050C);
    io_write(HPI_DATA, 0x0008); //4 port number
    io_write(HPI_DATA, 0x02D0); //port address
    io_write(HPI_DATA, 0x0001);
    io_write(HPI_DATA, 0x0013);
    io_write(HPI_DATA, 0x0514);

    //td content 4 bytes
    io_write(HPI_DATA, 0x0681);//c
    io_write(HPI_DATA, 0x2100);//e //HID 0x21
    io_write(HPI_DATA, 0x0000);//0
    io_write(HPI_DATA, 0x007B);//2

    //data phase IN-1
    io_write(HPI_DATA, 0x0544); //514
    io_write(HPI_DATA, 0x0008);//6
    io_write(HPI_DATA, 0x0290);//8
    io_write(HPI_DATA, 0x0041);//a
    io_write(HPI_DATA, 0x0013);//c
    io_write(HPI_DATA, 0x0520);//e

    //status phase
    io_write(HPI_DATA, 0x0000); //52c
    io_write(HPI_DATA, 0x0000);//e
    io_write(HPI_DATA, 0x0210);//530
    io_write(HPI_DATA, 0x0041);//2
    io_write(HPI_DATA, 0x0013);//4
    io_write(HPI_DATA, 0x0000);//6

    usb_write(HUSB_SIE1_pCurrentTDPtr, 0x0500); //HUSB_SIE1_pCurrentTDPtr

}


void usb_get_report_desc() {
    //the starting address
    io_write(HPI_ADDR, 0x0500); //the start address
    io_write(HPI_DATA, 0x050C);
    io_write(HPI_DATA, 0x0008); //4 port number
    io_write(HPI_DATA, 0x02D0); //device address
    io_write(HPI_DATA, 0x0001);
    io_write(HPI_DATA, 0x0013);
    io_write(HPI_DATA, 0x0514);

    //td content 4 bytes
    io_write(HPI_DATA, 0x0681);//c
    io_write(HPI_DATA, 0x2200);//e //report 0x22
    io_write(HPI_DATA, 0x0000);//0
    io_write(HPI_DATA, 0x007B);//2

    //data phase IN-1
    io_write(HPI_DATA, 0x0580); //514
    io_write(HPI_DATA, 0x0008);//6
    io_write(HPI_DATA, 0x0290);//8
    io_write(HPI_DATA, 0x0041);//a
    io_write(HPI_DATA, 0x0013);//c
    io_write(HPI_DATA, 0x0520);//e

    //data phase IN-2
    io_write(HPI_DATA, 0x0588); //520
    io_write(HPI_DATA, 0x0008);//2
    io_write(HPI_DATA, 0x0290);//4
    io_write(HPI_DATA, 0x0001);//6 //data0
    io_write(HPI_DATA, 0x0013);//8
    io_write(HPI_DATA, 0x052c);//a

    //data phase IN-3
    io_write(HPI_DATA, 0x0590); //52c
    io_write(HPI_DATA, 0x0008);//e
    io_write(HPI_DATA, 0x0290);//530
    io_write(HPI_DATA, 0x0041);//2
    io_write(HPI_DATA, 0x0013);//4
    io_write(HPI_DATA, 0x0538);//6

    //data phase IN-4
    io_write(HPI_DATA, 0x0598); //538
    io_write(HPI_DATA, 0x0008);//a
    io_write(HPI_DATA, 0x0290);//c
    io_write(HPI_DATA, 0x0001);//e //data0
    io_write(HPI_DATA, 0x0013);//540
    io_write(HPI_DATA, 0x0544);//2

    //data phase IN-5
    io_write(HPI_DATA, 0x05a0); //544
    io_write(HPI_DATA, 0x0008);//6
    io_write(HPI_DATA, 0x0290);//8
    io_write(HPI_DATA, 0x0041);//a //data1
    io_write(HPI_DATA, 0x0013);//c
    io_write(HPI_DATA, 0x0550);//e

    //data phase IN-6
    io_write(HPI_DATA, 0x05a8); //550
    io_write(HPI_DATA, 0x0008);//2
    io_write(HPI_DATA, 0x0290);//4
    io_write(HPI_DATA, 0x0001);//6 //data0
    io_write(HPI_DATA, 0x0013);//8
    io_write(HPI_DATA, 0x055c);//a

    //data phase IN-7
    io_write(HPI_DATA, 0x05b0); //c
    io_write(HPI_DATA, 0x0008);//e
    io_write(HPI_DATA, 0x0290);//560
    io_write(HPI_DATA, 0x0041);//2 //data1
    io_write(HPI_DATA, 0x0013);//4
    io_write(HPI_DATA, 0x0568);//6

    //data phase IN-8
    io_write(HPI_DATA, 0x05b8); //8
    io_write(HPI_DATA, 0x0003);//a
    io_write(HPI_DATA, 0x0290);//c
    io_write(HPI_DATA, 0x0001);//e //data0
    io_write(HPI_DATA, 0x0013);//570
    io_write(HPI_DATA, 0x0574);//2

    //status phase
    io_write(HPI_DATA, 0x0000); //574
    io_write(HPI_DATA, 0x0000);//6
    io_write(HPI_DATA, 0x0210);//8
    io_write(HPI_DATA, 0x0041);//a
    io_write(HPI_DATA, 0x0013);//c
    io_write(HPI_DATA, 0x0000);//e

    usb_write(HUSB_SIE1_pCurrentTDPtr, 0x0500); //HUSB_SIE1_pCurrentTDPtr

}


alt_u16 usb_wait_td_list_done() {
    alt_u16 usb_ctl_val;

    usb_ctl_val = usb_read(HPI_SIE1_MSG_ADR); // STEP 3 j
    usb_write(HPI_SIE1_MSG_ADR, 0);
    while (usb_ctl_val != HUSB_TDListDone)  // k, read sie1 msg register
    {
        if (usb_ctl_val == 0x0000) {
        } else {
            printf("[SIE1 MSG]:SIE1 msg reg is %x\n", usb_ctl_val);
        }
        usb_ctl_val = usb_read(HPI_SIE1_MSG_ADR);
        usb_write(HPI_SIE1_MSG_ADR, 0);
    }

    return usb_ctl_val;
}


alt_u16 usb_get_retry_cnt() {
    alt_u16 usb_ctl_val;

    io_read(HPI_STATUS);
    if (usb_read(HPI_SIE1_MSG_ADR) == HUSB_TDListDone) {
        usb_write(HPI_SIE1_MSG_ADR, 0);

        while (!(io_read(HPI_STATUS) & HPI_STATUS_SIE1msg_FLAG))  //read sie1 msg register
        {
        }
    }
    //usleep(1000);
    io_write(HPI_ADDR, 0x0508);
    usb_ctl_val = io_read(HPI_DATA);

    return usb_ctl_val;
}


void usb_print_mem() {
    int i, code;
    io_write(HPI_ADDR, 0x0500); //the start address
    for (i = 0; i <= 200; i += 2) {
        code = io_read(HPI_DATA);
        printf("\naddr %x = %04x\n", 0x0500 + i, code);
    }
}

