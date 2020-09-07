/*---------------------------------------------------------------------------
  --      keyboard.c                                                       --
  --      Christine Chen                                                   --
  --      Ref. DE2-115 Demonstrations by Terasic Technologies Inc.         --
  --      Fall 2014                                                        --
  --                                                                       --
  --      For use with ECE 298 Experiment 7                                --
  --      UIUC ECE Department                                              --
  --      Adapt by liuzikai at Fall 2020                                   --
  ---------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <io.h>
#include <fcntl.h>
#include <unistd.h>

#include "system.h"
#include "alt_types.h"
#include "sys/alt_irq.h"

#include "cy7c67200.h"
#include "lcp_cmd.h"
#include "lcp_data.h"
#include "io_handler.h"
#include "usb.h"

alt_u16 intStat;
alt_u16 usb_ctl_val;
alt_u16 ctl_reg = 0;
alt_u16 no_device = 0;
alt_u16 fs_device = 0;
alt_u8 data_size;
alt_u8 hot_plug_count;
alt_u16 code;
alt_u8 fetch_toggle = 0;

void keyboard_init() {
    io_init();

    /* while(1)
    {
        io_write(HPI_MAILBOX,COMM_EXEC_INT);
        printf("[ERROR]:routine mailbox data is %x\n",io_read(HPI_MAILBOX));
        // usb_write(0xc008,0x000f);
        // usb_read(0xc008);
        usleep(10*10000);
    } */

    printf("USB keyboard setup...\n\n");

    // ---------------------------------------- SIE1 initial ---------------------------------------------------//
    USB_HOT_PLUG:
    usb_soft_reset();

    // STEP 1a:
    usb_write(HPI_SIE1_MSG_ADR, 0);
    usb_write(HOST1_STAT_REG, 0xFFFF);

    /* Set HUSB_pEOT time */
    usb_write(HUSB_pEOT, 600);  // adjust the according to your USB device speed

    usb_ctl_val = SOFEOP1_TO_CPU_EN | RESUME1_TO_HPI_EN;  // | SOFEOP1_TO_HPI_EN;
    usb_write(HPI_IRQ_ROUTING_REG, usb_ctl_val);

    intStat = A_CHG_IRQ_EN | SOF_EOP_IRQ_EN;
    usb_write(HOST1_IRQ_EN_REG, intStat);
    // STEP 1a end

    // STEP 1b begin
    usb_write(COMM_R0, 0x0000);  // reset time
    usb_write(COMM_R1, 0x0000);  // port number
    usb_write(COMM_R2, 0x0000);  // r1
    usb_write(COMM_R3, 0x0000);  // r1
    usb_write(COMM_R4, 0x0000);  // r1
    usb_write(COMM_R5, 0x0000);  // r1
    usb_write(COMM_R6, 0x0000);  // r1
    usb_write(COMM_R7, 0x0000);  // r1
    usb_write(COMM_R8, 0x0000);  // r1
    usb_write(COMM_R9, 0x0000);  // r1
    usb_write(COMM_R10, 0x0000);  // r1
    usb_write(COMM_R11, 0x0000);  // r1
    usb_write(COMM_R12, 0x0000);  // r1
    usb_write(COMM_R13, 0x0000);  // r1
    usb_write(COMM_INT_NUM, HUSB_SIE1_INIT_INT);  // HUSB_SIE1_INIT_INT
    io_write(HPI_MAILBOX, COMM_EXEC_INT);

    while (!(io_read(HPI_STATUS) & 0xFFFF)) {} // read sie1 msg register
    while (io_read(HPI_MAILBOX) != COMM_ACK) {
        printf("[ERROR]:routine mailbox data is %x\n", io_read(HPI_MAILBOX));
        goto USB_HOT_PLUG;
    }
    // STEP 1b end

    printf("STEP 1 Complete");
    // STEP 2 begin
    usb_write(COMM_INT_NUM, HUSB_RESET_INT);  // husb reset
    usb_write(COMM_R0, 0x003c);  // reset time
    usb_write(COMM_R1, 0x0000);  // port number
    usb_write(COMM_R2, 0x0000);  // r1
    usb_write(COMM_R3, 0x0000);  // r1
    usb_write(COMM_R4, 0x0000);  // r1
    usb_write(COMM_R5, 0x0000);  // r1
    usb_write(COMM_R6, 0x0000);  // r1
    usb_write(COMM_R7, 0x0000);  // r1
    usb_write(COMM_R8, 0x0000);  // r1
    usb_write(COMM_R9, 0x0000);  // r1
    usb_write(COMM_R10, 0x0000);  // r1
    usb_write(COMM_R11, 0x0000);  // r1
    usb_write(COMM_R12, 0x0000);  // r1
    usb_write(COMM_R13, 0x0000);  // r1

    io_write(HPI_MAILBOX, COMM_EXEC_INT);

    while (io_read(HPI_MAILBOX) != COMM_ACK) {
        printf("[ERROR]:routine mailbox data is %x\n", io_read(HPI_MAILBOX));
        goto USB_HOT_PLUG;
    }
    // STEP 2 end

    ctl_reg = USB1_CTL_REG;
    no_device = (A_DP_STAT | A_DM_STAT);
    fs_device = A_DP_STAT;
    usb_ctl_val = usb_read(ctl_reg);

    if (!(usb_ctl_val & no_device)) {
        for (hot_plug_count = 0; hot_plug_count < 5; hot_plug_count++) {
            usleep(5 * 1000);
            usb_ctl_val = usb_read(ctl_reg);
            if (usb_ctl_val & no_device) break;
        }
        if (!(usb_ctl_val & no_device)) {
            printf("\n[INFO]: no device is present in SIE1!\n");
            printf("[INFO]: please insert a USB keyboard in SIE1!\n");
            while (!(usb_ctl_val & no_device)) {
                usb_ctl_val = usb_read(ctl_reg);
                if (usb_ctl_val & no_device)
                    goto USB_HOT_PLUG;

                usleep(2000);
            }
        }
    } else {
        /* check for low speed or full speed by reading D+ and D- lines */
        if (usb_ctl_val & fs_device) {
            printf("[INFO]: full speed device\n");
        } else {
            printf("[INFO]: low speed device\n");
        }
    }

    // STEP 3 begin
    // ----------------------------------------------- set address ---------------------------------------------------
    usb_set_address();

    while (!(io_read(HPI_STATUS) & HPI_STATUS_SIE1msg_FLAG))  // read sie1 msg register
    {
        usb_set_address();
        usleep(10 * 1000);
    }

    usb_wait_td_list_done();

    io_write(HPI_ADDR, 0x0506);  // i
    printf("[ENUM PROCESS]:step 3 TD Status Byte is %x\n", io_read(HPI_DATA));

    io_write(HPI_ADDR, 0x0508);  // n
    usb_ctl_val = io_read(HPI_DATA);
    printf("[ENUM PROCESS]:step 3 TD Control Byte is %x\n", usb_ctl_val);
    while (usb_ctl_val != 0x03) // retries occurred
    {
        usb_ctl_val = usb_get_retry_cnt();

        goto USB_HOT_PLUG;
    }

    printf("------------[ENUM PROCESS]:set address done!---------------\n");

    // STEP 4 begin
    // ------------------------------- get device descriptor-1 ----------------------------------- //
    // TASK: Call the appropriate function for this step.
    usb_get_device_desc1();    // Get Device Descriptor -1

    // usleep(10*1000);
    while (!(io_read(HPI_STATUS) & HPI_STATUS_SIE1msg_FLAG))  // read sie1 msg register
    {
        // TASK: Call the appropriate function again if it wasn't processed successfully.
        usb_get_device_desc1();
        usleep(10 * 1000);
    }

    usb_wait_td_list_done();

    io_write(HPI_ADDR, 0x0506);
    printf("[ENUM PROCESS]:step 4 TD Status Byte is %x\n", io_read(HPI_DATA));

    io_write(HPI_ADDR, 0x0508);
    usb_ctl_val = io_read(HPI_DATA);
    printf("[ENUM PROCESS]:step 4 TD Control Byte is %x\n", usb_ctl_val);
    while (usb_ctl_val != 0x03) {
        usb_ctl_val = usb_get_retry_cnt();
    }

    printf("---------------[ENUM PROCESS]:get device descriptor-1 done!-----------------\n");


    //--------------------------------get device descriptor-2---------------------------------------------//
    // get device descriptor
    // TASK: Call the appropriate function for this step.
    usb_get_device_desc2();    // Get Device Descriptor -2

    // if no message
    while (!(io_read(HPI_STATUS) & HPI_STATUS_SIE1msg_FLAG))  // read sie1 msg register
    {
        // resend the get device descriptor
        // get device descriptor
        // TASK: Call the appropriate function again if it wasn't processed successfully.
        usb_get_device_desc2();
        usleep(10 * 1000);
    }

    usb_wait_td_list_done();

    io_write(HPI_ADDR, 0x0506);
    printf("[ENUM PROCESS]:step 4 TD Status Byte is %x\n", io_read(HPI_DATA));

    io_write(HPI_ADDR, 0x0508);
    usb_ctl_val = io_read(HPI_DATA);
    printf("[ENUM PROCESS]:step 4 TD Control Byte is %x\n", usb_ctl_val);
    while (usb_ctl_val != 0x03) {
        usb_ctl_val = usb_get_retry_cnt();
    }

    printf("------------[ENUM PROCESS]:get device descriptor-2 done!--------------\n");


    // STEP 5 begin
    // ----------------------------------- get configuration descriptor -1 ----------------------------------//
    // TASK: Call the appropriate function for this step.
    usb_get_config_desc1();    // Get Configuration Descriptor -1

    // if no message
    while (!(io_read(HPI_STATUS) & HPI_STATUS_SIE1msg_FLAG))  // read sie1 msg register
    {
        // resend the get device descriptor
        // get device descriptor

        // TASK: Call the appropriate function again if it wasn't processed successfully.
        usb_get_config_desc1();
        usleep(10 * 1000);
    }

    usb_wait_td_list_done();

    io_write(HPI_ADDR, 0x0506);
    printf("[ENUM PROCESS]:step 5 TD Status Byte is %x\n", io_read(HPI_DATA));

    io_write(HPI_ADDR, 0x0508);
    usb_ctl_val = io_read(HPI_DATA);
    printf("[ENUM PROCESS]:step 5 TD Control Byte is %x\n", usb_ctl_val);
    while (usb_ctl_val != 0x03) {
        usb_ctl_val = usb_get_retry_cnt();
    }
    printf("------------[ENUM PROCESS]:get configuration descriptor-1 pass------------\n");

    // STEP 6 begin
    //-----------------------------------get configuration descriptor-2------------------------------------//
    // get device descriptor
    // TASK: Call the appropriate function for this step.
    usb_get_config_desc2();    // Get Configuration Descriptor -2

    usleep(100 * 1000);
    // if no message
    while (!(io_read(HPI_STATUS) & HPI_STATUS_SIE1msg_FLAG))  // read sie1 msg register
    {
        // TASK: Call the appropriate function again if it wasn't processed successfully.
        usb_get_config_desc2();
        usleep(10 * 1000);
    }

    usb_wait_td_list_done();

    io_write(HPI_ADDR, 0x0506);
    printf("[ENUM PROCESS]:step 6 TD Status Byte is %x\n", io_read(HPI_DATA));

    io_write(HPI_ADDR, 0x0508);
    usb_ctl_val = io_read(HPI_DATA);
    printf("[ENUM PROCESS]:step 6 TD Control Byte is %x\n", usb_ctl_val);
    while (usb_ctl_val != 0x03) {
        usb_ctl_val = usb_get_retry_cnt();
    }


    printf("-----------[ENUM PROCESS]:get configuration descriptor-2 done!------------\n");


    // --------------------------------- get device info --------------------------------------------- //

    // TASK: Write the address to read from the memory for byte 7 of the interface descriptor to HPI_ADDR.
    io_write(HPI_ADDR, 0x056c);
    code = io_read(HPI_DATA);
    code = code & 0x003;
    printf("\ncode = %x\n", code);

    if (code == 0x01) {
        printf("\n[INFO]:check TD rec data7 \n[INFO]:Keyboard Detected!!!\n\n");
    } else {
        printf("\n[INFO]:Keyboard Not Detected!!! \n\n");
    }

    // TASK: Write the address to read from the memory for the endpoint descriptor to HPI_ADDR.

    io_write(HPI_ADDR, 0x0576);
    io_write(HPI_DATA, 0x073F);
    io_write(HPI_DATA, 0x8105);
    io_write(HPI_DATA, 0x0003);
    io_write(HPI_DATA, 0x0008);
    io_write(HPI_DATA, 0xAC0A);
    usb_write(HUSB_SIE1_pCurrentTDPtr, 0x0576);  // HUSB_SIE1_pCurrentTDPtr

    // data_size = (io_read(HPI_DATA)>>8)&0x0ff;
    // data_size = 0x08;  //(io_read(HPI_DATA))&0x0ff;
    // usb_print_mem();
    io_write(HPI_ADDR, 0x057c);
    data_size = (io_read(HPI_DATA)) & 0x0ff;
    printf("[ENUM PROCESS]:data packet size is %d\n", data_size);
    // STEP 7 begin
    // ------------------------------------ set configuration ----------------------------------------- //
    // TASK: Call the appropriate function for this step.
    usb_set_config();        // Set Configuration

    while (!(io_read(HPI_STATUS) & HPI_STATUS_SIE1msg_FLAG))  // read sie1 msg register
    {
        // TASK: Call the appropriate function again if it wasn't processed successfully.
        usb_set_config();        // Set Configuration
        usleep(10 * 1000);
    }

    usb_wait_td_list_done();

    io_write(HPI_ADDR, 0x0506);
    printf("[ENUM PROCESS]:step 7 TD Status Byte is %x\n", io_read(HPI_DATA));

    io_write(HPI_ADDR, 0x0508);
    usb_ctl_val = io_read(HPI_DATA);
    printf("[ENUM PROCESS]:step 7 TD Control Byte is %x\n", usb_ctl_val);
    while (usb_ctl_val != 0x03) {
        usb_ctl_val = usb_get_retry_cnt();
    }

    printf("------------[ENUM PROCESS]:set configuration done!-------------------\n");

    //---------------------------------------------- class request out ------------------------------------------ //
    // TASK: Call the appropriate function for this step.
    usb_class_request();

    while (!(io_read(HPI_STATUS) & HPI_STATUS_SIE1msg_FLAG))  // read sie1 msg register
    {
        // TASK: Call the appropriate function again if it wasn't processed successfully.
        usb_class_request();
        usleep(10 * 1000);
    }

    usb_wait_td_list_done();

    io_write(HPI_ADDR, 0x0506);
    printf("[ENUM PROCESS]:step 8 TD Status Byte is %x\n", io_read(HPI_DATA));

    io_write(HPI_ADDR, 0x0508);
    usb_ctl_val = io_read(HPI_DATA);
    printf("[ENUM PROCESS]:step 8 TD Control Byte is %x\n", usb_ctl_val);
    while (usb_ctl_val != 0x03) {
        usb_ctl_val = usb_get_retry_cnt();
    }


    printf("------------[ENUM PROCESS]:class request out done!-------------------\n");

    // STEP 8 begin
    // -------------------------------- get descriptor(class 0x21 = HID) request out ------------------------------ //
    // TASK: Call the appropriate function for this step.
    usb_get_hid_desc();

    while (!(io_read(HPI_STATUS) & HPI_STATUS_SIE1msg_FLAG))  // read sie1 msg register
    {
        // TASK: Call the appropriate function again if it wasn't processed successfully.
        usb_get_hid_desc();
        usleep(10 * 1000);
    }

    usb_wait_td_list_done();

    io_write(HPI_ADDR, 0x0506);
    printf("[ENUM PROCESS]:step 8 TD Status Byte is %x\n", io_read(HPI_DATA));

    io_write(HPI_ADDR, 0x0508);
    usb_ctl_val = io_read(HPI_DATA);
    printf("[ENUM PROCESS]:step 8 TD Control Byte is %x\n", usb_ctl_val);
    while (usb_ctl_val != 0x03) {
        usb_ctl_val = usb_get_retry_cnt();
    }

    printf("------------[ENUM PROCESS]:get descriptor (class 0x21) done!-------------------\n");

    // STEP 9 begin
    // ------------------------------- get descriptor (class 0x22 = report) ---------------------------------------- //
    // TASK: Call the appropriate function for this step.
    usb_get_report_desc();
    // if no message
    while (!(io_read(HPI_STATUS) & HPI_STATUS_SIE1msg_FLAG))  // read sie1 msg register
    {
        // TASK: Call the appropriate function again if it wasn't processed successfully.
        usb_get_report_desc();
        usleep(10 * 1000);
    }

    usb_wait_td_list_done();

    io_write(HPI_ADDR, 0x0506);
    printf("[ENUM PROCESS]: step 9 TD Status Byte is %x\n", io_read(HPI_DATA));

    io_write(HPI_ADDR, 0x0508);
    usb_ctl_val = io_read(HPI_DATA);
    printf("[ENUM PROCESS]: step 9 TD Control Byte is %x\n", usb_ctl_val);
    while (usb_ctl_val != 0x03) {
        usb_ctl_val = usb_get_retry_cnt();
    }

    printf("---------------[ENUM PROCESS]:get descriptor (class 0x22) done!----------------\n");

    usleep(10000);
}

int keyboard_fetch(alt_u16 *keycode_ptr) {

    fetch_toggle = 1 - fetch_toggle;

    io_write(HPI_ADDR, 0x0500);  // the start address
    // data phase IN-1
    io_write(HPI_DATA, 0x051c);  // 500

    io_write(HPI_DATA, 0x000f & data_size);  // 2 data length

    io_write(HPI_DATA, 0x0291);  // 4 // endpoint 1
    if (fetch_toggle) {
        io_write(HPI_DATA, 0x0001);  // 6 // data 1
    } else {
        io_write(HPI_DATA, 0x0041);  // 6 // data 1
    }
    io_write(HPI_DATA, 0x0013);  // 8
    io_write(HPI_DATA, 0x0000);  // a
    usb_write(HUSB_SIE1_pCurrentTDPtr, 0x0500);  // HUSB_SIE1_pCurrentTDPtr

    while (!(io_read(HPI_STATUS) & HPI_STATUS_SIE1msg_FLAG))  // read sie1 msg register
    {
        io_write(HPI_ADDR, 0x0500);  // the start address
        // data phase IN-1
        io_write(HPI_DATA, 0x051c);  // 500

        io_write(HPI_DATA, 0x000f & data_size);  // 2 data length

        io_write(HPI_DATA, 0x0291);  // 4 // endpoint 1

        if (fetch_toggle) {
            io_write(HPI_DATA, 0x0001);  // 6 // data 1
        } else {
            io_write(HPI_DATA, 0x0041);  // 6 // data 1
        }

        io_write(HPI_DATA, 0x0013);  // 8
        io_write(HPI_DATA, 0x0000);  //
        usb_write(HUSB_SIE1_pCurrentTDPtr, 0x0500);  // HUSB_SIE1_pCurrentTDPtr

        // NOTE: [liuzikai] disable this sleep so that this procesure won't take too long
        // usleep(10 * 1000);
    }

    usb_ctl_val = usb_wait_td_list_done();

    // The first two keycodes are stored in 0x051E. Other keycodes are in subsequent addresses.
    *keycode_ptr = usb_read(0x051e);
    *(keycode_ptr + 1) = usb_read(0x051f);
    *(keycode_ptr + 2) = usb_read(0x0520);
    *(keycode_ptr + 3) = usb_read(0x0521);

    // printf("\nThe first two keycode values are %04x\n", *keycode);

    // We only need the first keycode, which is at the lower byte of keycode.
    // Send the keycode to hardware via PIO.
    *keycode_base = *keycode_ptr & 0xffu;

    // NOTE: [liuzikai] disable hot plug function to reduce the time consumption

    /* usleep(200);  // usleep(5000);
    usb_ctl_val = usb_read(ctl_reg);

    if (!(usb_ctl_val & no_device)) {
        // USB hot plug routine
        for (hot_plug_count = 0; hot_plug_count < 7; hot_plug_count++) {
            usleep(5 * 1000);
            usb_ctl_val = usb_read(ctl_reg);
            if (usb_ctl_val & no_device) break;
        }
        if (!(usb_ctl_val & no_device)) {
            printf("\n[INFO]: the keyboard has been removed!!! \n");
            printf("[INFO]: please insert again!!! \n");
            return 1;
        }
    } */

    return 0;
}

int keyboard_hot_plugged() {
    usb_ctl_val = usb_read(ctl_reg);
    usleep(5 * 1000);
    usb_ctl_val = usb_read(ctl_reg);
    usleep(5 * 1000);
    usb_ctl_val = usb_read(ctl_reg);
    usleep(5 * 1000);

    if (usb_ctl_val & no_device) return 1;

    return 0;
}

