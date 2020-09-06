/*---------------------------------------------------------------------------
  --      main.c                                                    	   --
  --      Christine Chen                                                   --
  --      Ref. DE2-115 Demonstrations by Terasic Technologies Inc.         --
  --      Fall 2014                                                        --
  --                                                                       --
  --      For use with ECE 298 Experiment 7                                --
  --      UIUC ECE Department                                              --
  ---------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <io.h>
#include <fcntl.h>
#include <math.h>


#include "system.h"
#include "alt_types.h"
#include <unistd.h>  // usleep 
#include "sys/alt_irq.h"
#include "io_handler.h"

#include "cy7c67200.h"
#include "usb.h"
#include "lcp_cmd.h"
#include "lcp_data.h"
#include "game_logic.h"

// Structure of the zombies
struct Zombie {
    int index; // TODO: Index used for sorting
    int x_pos; // Current x position of the zombie
    int y_pos; // Current y position of the zombie
    int blood; // Current HP of the zombie
    int enable; // Whether the zombie is alive
};

// Structure of the player
struct Player {
    int x_pos; // Current x position of the zombie
    int y_pos; // Current y position of the zombie
    int blood; // Current HP of the zombie
};

/**
 * Helper functions
 */

/**
 * This function determines the position of the zombies
 * Directions about the zombies
 * 7 8 9
 * 4 5 6
 * 1 2 3
 */

char determine_pos(int delta_x, int delta_y) {
    if (delta_x == 0) {
        if (delta_y == 0) {
            return '5';
        } else if (delta_y > 0) {
            return '2';
        } else {
            return '8';
        }
    } else if (delta_y == 0) {
        if (delta_x > 0) {
            return '6';
        } else {
            return '4';
        }
    } else {
        if (delta_x < 0) {
            if (delta_y > 0) {
                return '1';
            } else {
                return '7';
            }
        } else {
            if (delta_y > 0) {
                return '3';
            } else {
                return '9';
            }
        }
    }
}
/**
 * This function checks for collisions
 * Return 0 for success and return 1 for failure.
 */

int move_zombie (char(* display)[WINDOW_HEIGHT], struct Zombie* cur_zombie, struct Player* player_1) {
    int new_x, new_y;
    int delta_x, delta_y;
    float normalizer;
    // First check whether the zombie exists, if not, create a new zombie
    if (cur_zombie == NULL) {
        return 1; // failure
    } else {
        if (cur_zombie -> enable == 0) {
            // Check whether there is zombie at the born place
            if (display[0][WINDOW_WIDTH / 2] != ' '){
                return 1;
            } else {
                cur_zombie -> x_pos = WINDOW_WIDTH / 2;
                cur_zombie -> y_pos = 0;
                cur_zombie -> enable = 1;
                return 0;
            }
        }
    }

    // Check for existing zombies
    // All delta distances are the relevant distance with respect to the player

    delta_x = player_1 -> x_pos - cur_zombie -> x_pos;
    delta_y = player_1 -> y_pos - cur_zombie -> y_pos;
    if (ZOMBIE_MOVE >= delta_x * delta_x + delta_y * delta_y) { // do not need to normalize the distance
        new_x = cur_zombie -> x_pos + delta_x;
        new_y = cur_zombie -> y_pos + delta_y;
        player_1 -> blood -= ZOMBIE_ATTACT; // zombie bites the player
    } else { // need to the distance
        normalizer = ZOMBIE_MOVE / pow(delta_x * delta_x + delta_y * delta_y, 0.5);
        new_x = cur_zombie -> x_pos + round(delta_x * normalizer);
        new_y = cur_zombie -> y_pos + round(delta_y * normalizer);
    }

    // Check for collisions

    // if (display[new_y * WINDOW_WIDTH + new_x] == ' ') {
    //     display[(cur_zombie -> y_pos) * WINDOW_WIDTH + cur_zombie -> x_pos] = ' ';
    //     display[new_y * WINDOW_WIDTH + new_x] = determine_pos(delta_x, delta_y);
    if (display[new_x][new_y] == ' ') {
        display[cur_zombie -> x_pos][cur_zombie -> y_pos] = ' ';
        display[new_x][new_y] = determine_pos(delta_x, delta_y);
        cur_zombie -> x_pos = new_x;
        cur_zombie -> y_pos = new_y;
    } else {
        return 1;
    }
    return 0;
}

/**
 * This function checks for collision for player move
 * as well as cause damage to the zombies
 * Return 0 for success and return 1 for failure.
 * Directions are defined the same as above.
 * 8 - up; 4 - left; 6 - right; 2 - down
 */

int move_player(char(* display)[WINDOW_HEIGHT], struct Zombie* zombies, struct Player* player_1, int direction){
    int delta_x = 0;
    int delta_y = 0;
    int new_x, new_y;
    switch (direction) {
        case 8: // up
            delta_y = -PLAYER_MOVE;
            break;
        case 2: // down
            delta_y = PLAYER_MOVE;
            break;
        case 4: // left
            delta_x = -PLAYER_MOVE;
            break;
        case 6: // right
            delta_x = PLAYER_MOVE;
            break;
        default:
            break;
    }

    // Check for boundaries
    if (player_1 -> x_pos + delta_x < 0) {
        new_x = 0;
    } else if (player_1 -> x_pos + delta_x >= WINDOW_WIDTH) {
        new_x = WINDOW_WIDTH - 1;
    } else {
        new_x = player_1 -> x_pos + delta_x;
    }
    if (player_1 -> y_pos + delta_y < 0) {
        new_y = 0;
    } else if (player_1 -> y_pos + delta_y >= WINDOW_HEIGHT) {
        new_y = WINDOW_HEIGHT - 1;
    } else {
        new_y = player_1 -> y_pos + delta_y;
    }

    // Check for collision
    // if (display[new_y * WINDOW_WIDTH + new_x] == ' ') {
    //     display[(player_1 -> y_pos) * WINDOW_WIDTH + player_1 -> x_pos] = ' ';
    //     display[new_y * WINDOW_WIDTH + new_x] = '*';
    if (display[new_x][new_y] == ' ') {
        display[player_1 -> x_pos][player_1 -> y_pos] = ' ';
        display[new_x][new_y] = '*';
        player_1 -> x_pos = new_x;
        player_1 -> y_pos = new_y;
    } else {
        return 1;
    }
    return 0;
}

//----------------------------------------------------------------------------------------//
//
//                                Main function
//
//----------------------------------------------------------------------------------------//
int main(void)
{
	IO_init();

	/*while(1)
	{
		IO_write(HPI_MAILBOX,COMM_EXEC_INT);
		printf("[ERROR]:routine mailbox data is %x\n",IO_read(HPI_MAILBOX));
		//UsbWrite(0xc008,0x000f);
		//UsbRead(0xc008);
		usleep(10*10000);
	}*/

	alt_u16 intStat;
	alt_u16 usb_ctl_val;
	static alt_u16 ctl_reg = 0;
	static alt_u16 no_device = 0;
	alt_u16 fs_device = 0;
	int keycode = 0;
	alt_u8 toggle = 0;
	alt_u8 data_size;
	alt_u8 hot_plug_count;
	alt_u16 code;    
	
	
	
	/**
     *  Unit test display tool
     * ' ' means empty
     * 
     * Directions about the zombies
     * 7 8 9
     * 4 5 6 
     * 1 2 3
     * 
     * '*' means the position of the player
     */

    char display[WINDOW_WIDTH][WINDOW_HEIGHT];

    struct Zombie zombies[MAX_ZOMBIES_ON_SCREEN]; 
    struct Player player_1;
    int time_counter = 0;
    int zombie_appear_counter = 0;
    int cur_zombie_counter = 0;
    char key_buf[8];
    int wait_counter = 0;

    // Initialization
    // Player is born at the center of the screen
    int i, j;
    for (i = 0; i < WINDOW_WIDTH; i++){
        for (j = 0; j < WINDOW_HEIGHT; j++){
            display[i][j] = ' ';
        }
    }
    player_1.x_pos = WINDOW_WIDTH / 2;
    player_1.y_pos = WINDOW_HEIGHT / 2;
    player_1.blood = 100;
    display[player_1.x_pos][player_1.y_pos] = '*';

    // Zombies are born at the the top center of the screenS
    zombies[0].x_pos = WINDOW_WIDTH / 2;
    zombies[0].y_pos = 0;
    zombies[0].blood = 100;
    zombies[0].enable = 1;
    display[zombies[0].x_pos][zombies[0].y_pos] = '5';
    cur_zombie_counter ++;
	printf("Game logic initialization done.\n\n");

	//---------------------------------------Modification----------------------------------------------------//

	printf("USB keyboard setup...\n\n");

	//----------------------------------------SIE1 initial---------------------------------------------------//
	USB_HOT_PLUG:
	UsbSoftReset();

	// STEP 1a:
	UsbWrite (HPI_SIE1_MSG_ADR, 0);
	UsbWrite (HOST1_STAT_REG, 0xFFFF);

	/* Set HUSB_pEOT time */
	UsbWrite(HUSB_pEOT, 600); // adjust the according to your USB device speed

	usb_ctl_val = SOFEOP1_TO_CPU_EN | RESUME1_TO_HPI_EN;// | SOFEOP1_TO_HPI_EN;
	UsbWrite(HPI_IRQ_ROUTING_REG, usb_ctl_val);

	intStat = A_CHG_IRQ_EN | SOF_EOP_IRQ_EN ;
	UsbWrite(HOST1_IRQ_EN_REG, intStat);
	// STEP 1a end

	// STEP 1b begin
	UsbWrite(COMM_R0,0x0000);//reset time
	UsbWrite(COMM_R1,0x0000);  //port number
	UsbWrite(COMM_R2,0x0000);  //r1
	UsbWrite(COMM_R3,0x0000);  //r1
	UsbWrite(COMM_R4,0x0000);  //r1
	UsbWrite(COMM_R5,0x0000);  //r1
	UsbWrite(COMM_R6,0x0000);  //r1
	UsbWrite(COMM_R7,0x0000);  //r1
	UsbWrite(COMM_R8,0x0000);  //r1
	UsbWrite(COMM_R9,0x0000);  //r1
	UsbWrite(COMM_R10,0x0000);  //r1
	UsbWrite(COMM_R11,0x0000);  //r1
	UsbWrite(COMM_R12,0x0000);  //r1
	UsbWrite(COMM_R13,0x0000);  //r1
	UsbWrite(COMM_INT_NUM,HUSB_SIE1_INIT_INT); //HUSB_SIE1_INIT_INT
	IO_write(HPI_MAILBOX,COMM_EXEC_INT);

	while (!(IO_read(HPI_STATUS) & 0xFFFF) )  //read sie1 msg register
	{
	}
	while (IO_read(HPI_MAILBOX) != COMM_ACK)
	{
		printf("[ERROR]:routine mailbox data is %x\n",IO_read(HPI_MAILBOX));
		goto USB_HOT_PLUG;
	}
	// STEP 1b end

	printf("STEP 1 Complete");
	// STEP 2 begin
	UsbWrite(COMM_INT_NUM,HUSB_RESET_INT); //husb reset
	UsbWrite(COMM_R0,0x003c);//reset time
	UsbWrite(COMM_R1,0x0000);  //port number
	UsbWrite(COMM_R2,0x0000);  //r1
	UsbWrite(COMM_R3,0x0000);  //r1
	UsbWrite(COMM_R4,0x0000);  //r1
	UsbWrite(COMM_R5,0x0000);  //r1
	UsbWrite(COMM_R6,0x0000);  //r1
	UsbWrite(COMM_R7,0x0000);  //r1
	UsbWrite(COMM_R8,0x0000);  //r1
	UsbWrite(COMM_R9,0x0000);  //r1
	UsbWrite(COMM_R10,0x0000);  //r1
	UsbWrite(COMM_R11,0x0000);  //r1
	UsbWrite(COMM_R12,0x0000);  //r1
	UsbWrite(COMM_R13,0x0000);  //r1

	IO_write(HPI_MAILBOX,COMM_EXEC_INT);

	while (IO_read(HPI_MAILBOX) != COMM_ACK)
	{
		printf("[ERROR]:routine mailbox data is %x\n",IO_read(HPI_MAILBOX));
		goto USB_HOT_PLUG;
	}
	// STEP 2 end

	ctl_reg = USB1_CTL_REG;
	no_device = (A_DP_STAT | A_DM_STAT);
	fs_device = A_DP_STAT;
	usb_ctl_val = UsbRead(ctl_reg);

	if (!(usb_ctl_val & no_device))
	{
		for(hot_plug_count = 0 ; hot_plug_count < 5 ; hot_plug_count++)
		{
			usleep(5*1000);
			usb_ctl_val = UsbRead(ctl_reg);
			if(usb_ctl_val & no_device) break;
		}
		if(!(usb_ctl_val & no_device))
		{
			printf("\n[INFO]: no device is present in SIE1!\n");
			printf("[INFO]: please insert a USB keyboard in SIE1!\n");
			while (!(usb_ctl_val & no_device))
			{
				usb_ctl_val = UsbRead(ctl_reg);
				if(usb_ctl_val & no_device)
					goto USB_HOT_PLUG;

				usleep(2000);
			}
		}
	}
	else
	{
		/* check for low speed or full speed by reading D+ and D- lines */
		if (usb_ctl_val & fs_device)
		{
			printf("[INFO]: full speed device\n");
		}
		else
		{
			printf("[INFO]: low speed device\n");
		}
	}



	// STEP 3 begin
	//------------------------------------------------------set address -----------------------------------------------------------------
	UsbSetAddress();

	while (!(IO_read(HPI_STATUS) & HPI_STATUS_SIE1msg_FLAG) )  //read sie1 msg register
	{
		UsbSetAddress();
		usleep(10*1000);
	}

	UsbWaitTDListDone();

	IO_write(HPI_ADDR,0x0506); // i
	printf("[ENUM PROCESS]:step 3 TD Status Byte is %x\n",IO_read(HPI_DATA));

	IO_write(HPI_ADDR,0x0508); // n
	usb_ctl_val = IO_read(HPI_DATA);
	printf("[ENUM PROCESS]:step 3 TD Control Byte is %x\n",usb_ctl_val);
	while (usb_ctl_val != 0x03) // retries occurred
	{
		usb_ctl_val = UsbGetRetryCnt();

		goto USB_HOT_PLUG;
	}

	printf("------------[ENUM PROCESS]:set address done!---------------\n");

	// STEP 4 begin
	//-------------------------------get device descriptor-1 -----------------------------------//
	// TASK: Call the appropriate function for this step.
	UsbGetDeviceDesc1(); 	// Get Device Descriptor -1

	//usleep(10*1000);
	while (!(IO_read(HPI_STATUS) & HPI_STATUS_SIE1msg_FLAG) )  //read sie1 msg register
	{
		// TASK: Call the appropriate function again if it wasn't processed successfully.
		UsbGetDeviceDesc1();
		usleep(10*1000);
	}

	UsbWaitTDListDone();

	IO_write(HPI_ADDR,0x0506);
	printf("[ENUM PROCESS]:step 4 TD Status Byte is %x\n",IO_read(HPI_DATA));

	IO_write(HPI_ADDR,0x0508);
	usb_ctl_val = IO_read(HPI_DATA);
	printf("[ENUM PROCESS]:step 4 TD Control Byte is %x\n",usb_ctl_val);
	while (usb_ctl_val != 0x03)
	{
		usb_ctl_val = UsbGetRetryCnt();
	}

	printf("---------------[ENUM PROCESS]:get device descriptor-1 done!-----------------\n");


	//--------------------------------get device descriptor-2---------------------------------------------//
	//get device descriptor
	// TASK: Call the appropriate function for this step.
	UsbGetDeviceDesc2(); 	// Get Device Descriptor -2

	//if no message
	while (!(IO_read(HPI_STATUS) & HPI_STATUS_SIE1msg_FLAG) )  //read sie1 msg register
	{
		//resend the get device descriptor
		//get device descriptor
		// TASK: Call the appropriate function again if it wasn't processed successfully.
		UsbGetDeviceDesc2();
		usleep(10*1000);
	}

	UsbWaitTDListDone();

	IO_write(HPI_ADDR,0x0506);
	printf("[ENUM PROCESS]:step 4 TD Status Byte is %x\n",IO_read(HPI_DATA));

	IO_write(HPI_ADDR,0x0508);
	usb_ctl_val = IO_read(HPI_DATA);
	printf("[ENUM PROCESS]:step 4 TD Control Byte is %x\n",usb_ctl_val);
	while (usb_ctl_val != 0x03)
	{
		usb_ctl_val = UsbGetRetryCnt();
	}

	printf("------------[ENUM PROCESS]:get device descriptor-2 done!--------------\n");


	// STEP 5 begin
	//-----------------------------------get configuration descriptor -1 ----------------------------------//
	// TASK: Call the appropriate function for this step.
	UsbGetConfigDesc1(); 	// Get Configuration Descriptor -1

	//if no message
	while (!(IO_read(HPI_STATUS) & HPI_STATUS_SIE1msg_FLAG) )  //read sie1 msg register
	{
		//resend the get device descriptor
		//get device descriptor

		// TASK: Call the appropriate function again if it wasn't processed successfully.
		UsbGetConfigDesc1();
		usleep(10*1000);
	}

	UsbWaitTDListDone();

	IO_write(HPI_ADDR,0x0506);
	printf("[ENUM PROCESS]:step 5 TD Status Byte is %x\n",IO_read(HPI_DATA));

	IO_write(HPI_ADDR,0x0508);
	usb_ctl_val = IO_read(HPI_DATA);
	printf("[ENUM PROCESS]:step 5 TD Control Byte is %x\n",usb_ctl_val);
	while (usb_ctl_val != 0x03)
	{
		usb_ctl_val = UsbGetRetryCnt();
	}
	printf("------------[ENUM PROCESS]:get configuration descriptor-1 pass------------\n");

	// STEP 6 begin
	//-----------------------------------get configuration descriptor-2------------------------------------//
	//get device descriptor
	// TASK: Call the appropriate function for this step.
	UsbGetConfigDesc2(); 	// Get Configuration Descriptor -2

	usleep(100*1000);
	//if no message
	while (!(IO_read(HPI_STATUS) & HPI_STATUS_SIE1msg_FLAG) )  //read sie1 msg register
	{
		// TASK: Call the appropriate function again if it wasn't processed successfully.
		UsbGetConfigDesc2();
		usleep(10*1000);
	}

	UsbWaitTDListDone();

	IO_write(HPI_ADDR,0x0506);
	printf("[ENUM PROCESS]:step 6 TD Status Byte is %x\n",IO_read(HPI_DATA));

	IO_write(HPI_ADDR,0x0508);
	usb_ctl_val = IO_read(HPI_DATA);
	printf("[ENUM PROCESS]:step 6 TD Control Byte is %x\n",usb_ctl_val);
	while (usb_ctl_val != 0x03)
	{
		usb_ctl_val = UsbGetRetryCnt();
	}


	printf("-----------[ENUM PROCESS]:get configuration descriptor-2 done!------------\n");


	// ---------------------------------get device info---------------------------------------------//

	// TASK: Write the address to read from the memory for byte 7 of the interface descriptor to HPI_ADDR.
	IO_write(HPI_ADDR,0x056c);
	code = IO_read(HPI_DATA);
	code = code & 0x003;
	printf("\ncode = %x\n", code);

	if (code == 0x01)
	{
		printf("\n[INFO]:check TD rec data7 \n[INFO]:Keyboard Detected!!!\n\n");
	}
	else
	{
		printf("\n[INFO]:Keyboard Not Detected!!! \n\n");
	}

	// TASK: Write the address to read from the memory for the endpoint descriptor to HPI_ADDR.

	IO_write(HPI_ADDR,0x0576);
	IO_write(HPI_DATA,0x073F);
	IO_write(HPI_DATA,0x8105);
	IO_write(HPI_DATA,0x0003);
	IO_write(HPI_DATA,0x0008);
	IO_write(HPI_DATA,0xAC0A);
	UsbWrite(HUSB_SIE1_pCurrentTDPtr,0x0576); //HUSB_SIE1_pCurrentTDPtr

	//data_size = (IO_read(HPI_DATA)>>8)&0x0ff;
	//data_size = 0x08;//(IO_read(HPI_DATA))&0x0ff;
	//UsbPrintMem();
	IO_write(HPI_ADDR,0x057c);
	data_size = (IO_read(HPI_DATA))&0x0ff;
	printf("[ENUM PROCESS]:data packet size is %d\n",data_size);
	// STEP 7 begin
	//------------------------------------set configuration -----------------------------------------//
	// TASK: Call the appropriate function for this step.
	UsbSetConfig();		// Set Configuration

	while (!(IO_read(HPI_STATUS) & HPI_STATUS_SIE1msg_FLAG) )  //read sie1 msg register
	{
		// TASK: Call the appropriate function again if it wasn't processed successfully.
		UsbSetConfig();		// Set Configuration
		usleep(10*1000);
	}

	UsbWaitTDListDone();

	IO_write(HPI_ADDR,0x0506);
	printf("[ENUM PROCESS]:step 7 TD Status Byte is %x\n",IO_read(HPI_DATA));

	IO_write(HPI_ADDR,0x0508);
	usb_ctl_val = IO_read(HPI_DATA);
	printf("[ENUM PROCESS]:step 7 TD Control Byte is %x\n",usb_ctl_val);
	while (usb_ctl_val != 0x03)
	{
		usb_ctl_val = UsbGetRetryCnt();
	}

	printf("------------[ENUM PROCESS]:set configuration done!-------------------\n");

	//----------------------------------------------class request out ------------------------------------------//
	// TASK: Call the appropriate function for this step.
	UsbClassRequest();

	while (!(IO_read(HPI_STATUS) & HPI_STATUS_SIE1msg_FLAG) )  //read sie1 msg register
	{
		// TASK: Call the appropriate function again if it wasn't processed successfully.
		UsbClassRequest();
		usleep(10*1000);
	}

	UsbWaitTDListDone();

	IO_write(HPI_ADDR,0x0506);
	printf("[ENUM PROCESS]:step 8 TD Status Byte is %x\n",IO_read(HPI_DATA));

	IO_write(HPI_ADDR,0x0508);
	usb_ctl_val = IO_read(HPI_DATA);
	printf("[ENUM PROCESS]:step 8 TD Control Byte is %x\n",usb_ctl_val);
	while (usb_ctl_val != 0x03)
	{
		usb_ctl_val = UsbGetRetryCnt();
	}


	printf("------------[ENUM PROCESS]:class request out done!-------------------\n");

	// STEP 8 begin
	//----------------------------------get descriptor(class 0x21 = HID) request out --------------------------------//
	// TASK: Call the appropriate function for this step.
	UsbGetHidDesc();

	while (!(IO_read(HPI_STATUS) & HPI_STATUS_SIE1msg_FLAG) )  //read sie1 msg register
	{
		// TASK: Call the appropriate function again if it wasn't processed successfully.
		UsbGetHidDesc();
		usleep(10*1000);
	}

	UsbWaitTDListDone();

	IO_write(HPI_ADDR,0x0506);
	printf("[ENUM PROCESS]:step 8 TD Status Byte is %x\n",IO_read(HPI_DATA));

	IO_write(HPI_ADDR,0x0508);
	usb_ctl_val = IO_read(HPI_DATA);
	printf("[ENUM PROCESS]:step 8 TD Control Byte is %x\n",usb_ctl_val);
	while (usb_ctl_val != 0x03)
	{
		usb_ctl_val = UsbGetRetryCnt();
	}

	printf("------------[ENUM PROCESS]:get descriptor (class 0x21) done!-------------------\n");

	// STEP 9 begin
	//-------------------------------get descriptor (class 0x22 = report)-------------------------------------------//
	// TASK: Call the appropriate function for this step.
	UsbGetReportDesc();
	//if no message
	while (!(IO_read(HPI_STATUS) & HPI_STATUS_SIE1msg_FLAG) )  //read sie1 msg register
	{
		// TASK: Call the appropriate function again if it wasn't processed successfully.
		UsbGetReportDesc();
		usleep(10*1000);
	}

	UsbWaitTDListDone();

	IO_write(HPI_ADDR,0x0506);
	printf("[ENUM PROCESS]: step 9 TD Status Byte is %x\n",IO_read(HPI_DATA));

	IO_write(HPI_ADDR,0x0508);
	usb_ctl_val = IO_read(HPI_DATA);
	printf("[ENUM PROCESS]: step 9 TD Control Byte is %x\n",usb_ctl_val);
	while (usb_ctl_val != 0x03)
	{
		usb_ctl_val = UsbGetRetryCnt();
	}

	printf("---------------[ENUM PROCESS]:get descriptor (class 0x22) done!----------------\n");



	//-----------------------------------get keycode value------------------------------------------------//
	usleep(10000);
	while(1)
	{
		toggle++;
		wait_counter++;
		IO_write(HPI_ADDR,0x0500); //the start address
		//data phase IN-1
		IO_write(HPI_DATA,0x051c); //500

		IO_write(HPI_DATA,0x000f & data_size);//2 data length

		IO_write(HPI_DATA,0x0291);//4 //endpoint 1
		if(toggle%2)
		{
			IO_write(HPI_DATA,0x0001);//6 //data 1
		}
		else
		{
			IO_write(HPI_DATA,0x0041);//6 //data 1
		}
		IO_write(HPI_DATA,0x0013);//8
		IO_write(HPI_DATA,0x0000);//a
		UsbWrite(HUSB_SIE1_pCurrentTDPtr,0x0500); //HUSB_SIE1_pCurrentTDPtr
		
		while (!(IO_read(HPI_STATUS) & HPI_STATUS_SIE1msg_FLAG) )  //read sie1 msg register
		{
			IO_write(HPI_ADDR,0x0500); //the start address
			//data phase IN-1
			IO_write(HPI_DATA,0x051c); //500

			IO_write(HPI_DATA,0x000f & data_size);//2 data length

			IO_write(HPI_DATA,0x0291);//4 //endpoint 1
			if(toggle%2)
			{
				IO_write(HPI_DATA,0x0001);//6 //data 1
			}
			else
			{
				IO_write(HPI_DATA,0x0041);//6 //data 1
			}
			IO_write(HPI_DATA,0x0013);//8
			IO_write(HPI_DATA,0x0000);//
			UsbWrite(HUSB_SIE1_pCurrentTDPtr,0x0500); //HUSB_SIE1_pCurrentTDPtr
			usleep(10*1000);
		}//end while

		usb_ctl_val = UsbWaitTDListDone();

		// The first two keycodes are stored in 0x051E. Other keycodes are in 
		// subsequent addresses.
		keycode = UsbRead(0x051e);
		// printf("\nfirst two keycode values are %04x\n",keycode);
		// We only need the first keycode, which is at the lower byte of keycode.
		// Send the keycode to hardware via PIO.
		*keycode_base = keycode & 0xff;

		//---------------------------------------Modification----------------------------------------------------// 
		if (wait_counter % 2 == 0)	{// Unit test print
			for (j = 0; j < WINDOW_HEIGHT; j++) {
				for (i = 0; i < WINDOW_WIDTH; i++) {
					printf("%c", display[i][j]);
				}
				printf("\n");
			}
			printf("Player blood: %d ", player_1.blood);
			printf("Current time: %d\n", time_counter);

			// Update time
			time_counter++;

			// Move all the zombies
			if (time_counter % 8 == 0) {
				for (i = 0; i < MAX_ZOMBIES_ON_SCREEN; i++) {
					if (zombies[i].enable == 1) {
						move_zombie(display, &(zombies[i]), &player_1);
					}
				}
			}
			
			// Add new zombies
			if (time_counter % 8 == 0) {
				if ((zombie_appear_counter < MAX_TOTAL_ZOMBIES) && (cur_zombie_counter < MAX_ZOMBIES_ON_SCREEN)) {
					for (i = 0; i < MAX_ZOMBIES_ON_SCREEN; i++){
						if (zombies[i].enable == 0){
							zombies[i].index = cur_zombie_counter;
							zombies[i].blood = 100;
							if (0 == move_zombie(display, &(zombies[i]), &player_1)) {
								cur_zombie_counter ++;
								zombie_appear_counter ++;
								break;
							} 
						}
					}
				}
			}

			

			// Check for user input and update player position
			// Update blood of the zombies

			// scanf("%s", &key_buf);
			switch (keycode & 0xff) {
				case 26: // up
					move_player(display, zombies, &player_1, 8);
					for (i = 0;  i < MAX_ZOMBIES_ON_SCREEN; i++) {
						if ((zombies[i].enable == 1) && (zombies[i].x_pos == player_1.x_pos) && (zombies[i].y_pos < player_1.y_pos)){
							zombies[i].blood -= PLAYER_ATTACT;
						}
					}
				break;
				case 4: // left
					move_player(display, zombies, &player_1, 4);
					for (i = 0;  i < MAX_ZOMBIES_ON_SCREEN; i++) {
						if ((zombies[i].enable == 1) && (zombies[i].y_pos == player_1.y_pos) && (zombies[i].x_pos < player_1.x_pos)){
							zombies[i].blood -= PLAYER_ATTACT;
						}
					}
				break;
				case 22: // down
					move_player(display, zombies, &player_1, 2);
					for (i = 0;  i < MAX_ZOMBIES_ON_SCREEN; i++) {
						if ((zombies[i].enable == 1) && (zombies[i].x_pos == player_1.x_pos) && (zombies[i].y_pos > player_1.y_pos)){
							zombies[i].blood -= PLAYER_ATTACT;
						}
					}
				break;
				case 7: // right
					move_player(display, zombies, &player_1, 6);
					for (i = 0;  i < MAX_ZOMBIES_ON_SCREEN; i++) {
						if ((zombies[i].enable == 1) && (zombies[i].y_pos == player_1.y_pos) && (zombies[i].x_pos > player_1.x_pos)){
							zombies[i].blood -= PLAYER_ATTACT;
						}
					}
				break;
			}
			

			// Check for death of zombies and the player
			if (player_1.blood <= 0) {
				printf("You are dead, please restart the game!");
				return 0;
			} 
			if ((cur_zombie_counter == 0) && (zombie_appear_counter == MAX_TOTAL_ZOMBIES)) {
				printf("Congratulations, you have killed all the zombies!");
				return 0;
			}
			for (i = 0; i < MAX_ZOMBIES_ON_SCREEN; i++) {
				if ((zombies[i].enable == 1) && (zombies[i].blood <= 0)) {
					display[zombies[i].x_pos][zombies[i].y_pos] = ' '; // bug: forgot to erase the dead zombie
					zombies[i].enable = 0;
					cur_zombie_counter --;
				}
			}
		}

		usleep(200);//usleep(5000);
		usb_ctl_val = UsbRead(ctl_reg);

		if(!(usb_ctl_val & no_device))
		{
			//USB hot plug routine
			for(hot_plug_count = 0 ; hot_plug_count < 7 ; hot_plug_count++)
			{
				usleep(5*1000);
				usb_ctl_val = UsbRead(ctl_reg);
				if(usb_ctl_val & no_device) break;
			}
			if(!(usb_ctl_val & no_device))
			{
				printf("\n[INFO]: the keyboard has been removed!!! \n");
				printf("[INFO]: please insert again!!! \n");
			}
		}

		while (!(usb_ctl_val & no_device))
		{

			usb_ctl_val = UsbRead(ctl_reg);
			usleep(5*1000);
			usb_ctl_val = UsbRead(ctl_reg);
			usleep(5*1000);
			usb_ctl_val = UsbRead(ctl_reg);
			usleep(5*1000);

			if(usb_ctl_val & no_device)
				goto USB_HOT_PLUG;

			usleep(200);
		}

	}//end while

	return 0;
}