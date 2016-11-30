/*! ----------------------------------------------------------------------------
 *  @file    main.c
 *  @brief   main loop for the DecaRanging application
 *
 * @attention
 *
 * Copyright 2015 (c) DecaWave Ltd, Dublin, Ireland.
 *
 * All rights reserved.
 *
 * @author DecaWave
 */
/* Includes */
#include "compiler.h"
//#include "port.h"

#include "instance.h"
#include "parameter.h" // NEW HEADER FILE
#include "deca_types.h"
#include <uwbMain.h>
#include "../platform/deca_port.h"
#include "../../User/inc/compiler.h"

#include "instance.h"
#include "parameter.h"
#include "deca_types.h"
#include "deca_regs.h"
#include "deca_spi.h"
#include "deca_device_api.h"

#include "uart.h"


#define SWS1_SHF_MODE 0x02	//short frame mode (6.81M)
#define SWS1_CH5_MODE 0x04	//channel 5 mode
#define SWS1_ANC_MODE 0x08  //anchor mode


#define SWS1_A1A_MODE 0x10  //anchor/tag address A1
#define SWS1_A2A_MODE 0x20  //anchor/tag address A2
#define SWS1_A3A_MODE 0x40  //anchor/tag address A3


#define SWS1_USB2SPI_MODE 0x78  //USB to SPI mode
#define SWS1_TXSPECT_MODE 0x38  //Continuous TX spectrum mode
//"1234567812345678"
#define SOFTWARE_VER_STRING    "Ver.  2.10  TREK" //16 bytes!


/* Defines used to substitute buttons and pins for porting to SatisFactory*/
#ifdef MATEO_IMPL

//all the definitions are in the instance.h file
// all the parameters are in the #include "parameter.h" file

/* *
 * Position
 * * 0->SetOn
 * * 1->Mode
 * * 2->Channel
 * * 3->Unit
 * * 4,5,6,7... Data...
 * * n->Reserved
 * */

extern bool *TA_SW3;
extern int NUM_DATA_ARRAY;
extern int Naddress;
extern int MaskAddr;
extern int time_final = TIME_FINAL;
extern int slot_size=SLOT_SIZE;
extern int super_frame_size=SUPERFRAME_SIZE;
extern int num_disp=NUM_DISP;
#ifdef LOCALIZATION_2D
extern int num_anchor = NUM_ANCHOR-1;
#else
extern int num_anchor = NUM_ANCHOR;
#endif
extern int sch_final_delay=SCHEDULED_FINAL_DELAY;



#else

#define SWITCH_ON 			TRUE
#define SWITCH_OFF 			FALSE

#define BUTTON_0			FALSE //DARA RATE
#define TA_SW1_3			TRUE //CHANNEL
#define TA_SW1_4			TRUE	/* FALSE: Tag - TRUE: Anchor */
#define TA_SW1_5			FALSE  //MSB
#define TA_SW1_6			FALSE
#define TA_SW1_7			FALSE  //LSB
#define TA_SW1_8			FALSE  //ALWAYS IN FALSE

#define FASTRANGING 		SWITCH_OFF
#endif


#ifdef MATEO_IMPL
uint32 s1switch = 0;
int Init_instance_anchaddr = 0;
#else
 uint8 s1switch = 0;
#endif

int instance_anchaddr = 0;
int dr_mode = 0;
int chan, tagaddr, ancaddr;
int instance_mode = ANCHOR;

#define LCD_BUFF_LEN (80)
uint8 dataseq[LCD_BUFF_LEN];
uint8 dataseq1[LCD_BUFF_LEN];
#ifdef MATEO_IMPL
uint8 dataseqMATEO[LCD_BUFF_LEN];
uint8 dataseqMATEO2[LCD_BUFF_LEN];
uint8 dataseqMATEO3[LCD_BUFF_LEN];
uint8 dataseqMATEO4[LCD_BUFF_LEN];
#endif
uint32_t pauseTWRReports  = 0;
uint32_t printTWRReports  = 0;


uint8_t uwbInitComplete = 0;

osThreadId uwbMainTaskHandle;
osThreadId uwbProcessInterruptTaskHandle;

SemaphoreHandle_t uwbInterruptSemHandle;

int uwb_setup(void);

void UwbMainTask(void const * argument);
void UwbProcessInterruptTask(void const * argument);


typedef struct
{
	uint8 channel ;
	uint8 prf ;
	uint8 datarate ;
	uint8 preambleCode ;
	uint8 preambleLength ;
	uint8 pacSize ;
	uint8 nsSFD ;
	uint16 sfdTO ;
} chConfig_t ;


//Configuration for DecaRangeRTLS TREK Modes (4 default use cases selected by the switch S1 [2,3] on EVB1000, indexed 0 to 3 )
chConfig_t chConfig[4] ={
		//mode 0 - S1: 2 off, 3 off
		{
				2,              // channel
				DWT_PRF_16M,    // prf
				DWT_BR_110K,    // datarate
				4,              // preambleCode
				DWT_PLEN_1024,  // preambleLength
				DWT_PAC32,      // pacSize
				1,       // non-standard SFD
				(1025 + 64 - 32) //SFD timeout
		},
		//mode 1 - S1: 2 on, 3 off   //PRUEBAS TRABAJANDO EN ESTE MODO
		{
				2,              // channel
				DWT_PRF_16M,    // prf
				DWT_BR_6M8,    // datarate
				4,             // preambleCode
				DWT_PLEN_128,   // preambleLength
				DWT_PAC8,       // pacSize
				0,       // non-standard SFD
				(129 + 8 - 8) //SFD timeout
		},
		//mode 2 - S1: 2 off, 3 on
		{
				5,              // channel
				DWT_PRF_16M,    // prf
				DWT_BR_110K,    // datarate
				3,              // preambleCode
				DWT_PLEN_1024,  // preambleLength
				DWT_PAC32,      // pacSize
				1,       // non-standard SFD
				(1025 + 64 - 32) //SFD timeout
		},
		//mode 3 - S1: 2 on, 3 on
		{
				5,              // channel
				DWT_PRF_16M,    // prf
				DWT_BR_6M8,    // datarate
				3,             // preambleCode
				DWT_PLEN_128,   // preambleLength
				DWT_PAC8,       // pacSize
				0,       // non-standard SFD
				(129 + 8 - 8) //SFD timeout
		}
};

//Slot and Superframe Configuration for DecaRangeRTLS TREK Modes (4 default use cases selected by the switch S1 [2,3] on EVB1000, indexed 0 to 3 )
sfConfig_t sfConfig[4] ={
		//mode 0 - S1: 2 off, 3 off
#ifdef MATEO_IMPL

		{
				(SLOT_SIZE), //ms -
				(TOTAL_NUMBER_OF_SLOTS),   //thus 130 slots - thus 3.64s superframe means 0.27 Hz location rate (130 slots are needed as AtoA ranging takes 30+ ms)
				(SUPERFRAME_SIZE), //superframe period
				(SUPERFRAME_SIZE), //poll sleep delay
				(SCHEDULED_FINAL_DELAY)
		},
#else
		{
				(28), //ms -
				(10),   //thus 10 slots - thus 280ms superframe means 3.57 Hz location rate (10 slots are needed as AtoA ranging takes 30+ ms)
				(10*28), //superframe period
				(10*28), //poll sleep delay
				(20000)
		},
#endif

		//mode 1 - S1: 2 on, 3 off   // OJO TRABAJANDO EN ESTE MODO
#ifdef MATEO_IMPL
		{
				(SLOT_SIZE_HDR),   // slot period ms
				(TOTAL_NUMBER_OF_SLOTS),   // number of slots (only 100 are used) - thus 390 ms superframe means 2.56 Hz location rate
				(SUPERFRAME_SIZE_HDR), // superframe period (390 ms - gives 2.56 Hz)
				(SUPERFRAME_SIZE_HDR), // poll sleep delay (tag sleep time, usually = superframe period)
				(SCHEDULED_FINAL_DELAY_HDR)
		},


#else
		//mode 2 - S1: 2 on, 3 off   // OJO TRABAJANDO EN ESTE MODO
		{
				(10),   // slot period ms
				(10),   // number of slots (only 10 are used) - thus 100 ms superframe means 10 Hz location rate
				(10*10), // superframe period (100 ms - gives 10 Hz)
				(10*10), // poll sleep delay (tag sleep time, usually = superframe period)
				(2500)
		},
#endif
		//mode 2 - S1: 2 off, 3 on
#ifdef MATEO_IMPL

		{
				(SLOT_SIZE), //ms -
				(TOTAL_NUMBER_OF_SLOTS),   //thus 130 slots - thus 3.64s superframe means 0.27 Hz location rate (130 slots are needed as AtoA ranging takes 30+ ms)
				(SUPERFRAME_SIZE), //superframe period
				(SUPERFRAME_SIZE), //poll sleep delay
				(SCHEDULED_FINAL_DELAY)
		},
#else
		{
				(28), //ms -
				(10),   //thus 10 slots - thus 280ms superframe means 3.57 Hz location rate (10 slots are needed as AtoA ranging takes 30+ ms)
				(10*28), //superframe period
				(10*28), //poll sleep delay
				(20000)
		},
#endif

		//mode 3 - S1: 2 on, 3 on
#ifdef MATEO_IMPL
		{
				(SLOT_SIZE_HDR),   // slot period ms
				(TOTAL_NUMBER_OF_SLOTS),   // number of slots (only 100 are used) - thus 390 ms superframe means 2.56 Hz location rate
				(SUPERFRAME_SIZE_HDR), // superframe period (390 ms - gives 2.56 Hz)
				(SUPERFRAME_SIZE_HDR), // poll sleep delay (tag sleep time, usually = superframe period)
				(SCHEDULED_FINAL_DELAY_HDR)
		}

#else
		//mode 2 - S1: 2 on, 3 off   // OJO TRABAJANDO EN ESTE MODO
		{
				(10),   // slot period ms
				(10),   // number of slots (only 10 are used) - thus 100 ms superframe means 10 Hz location rate
				(10*10), // superframe period (100 ms - gives 10 Hz)
				(10*10), // poll sleep delay (tag sleep time, usually = superframe period)
				(2500)
		}
#endif
};
// ======================================================
//
//  Configure instance tag/anchor/etc... addresses
//
void addressconfigure(uint16 mode)
{
	uint16 instAddress = 0;
#ifdef MATEO_IMPL

	instance_anchaddr = Init_instance_anchaddr;

#else
	instance_anchaddr = (((s1switch & SWS1_A1A_MODE) << 2) + (s1switch & SWS1_A2A_MODE) + ((s1switch & SWS1_A3A_MODE) >> 2)) >> 4;
#endif
	if(mode == ANCHOR)
	{

#ifdef MATEO_IMPL

		if(instance_anchaddr > NUM_ANCHOR)		{
			instAddress = GATEWAY_ANCHOR_ADDR | MaskAddr ; //listener
		}
#else
		if(instance_anchaddr > 4)		{
			instAddress = GATEWAY_ANCHOR_ADDR | 0x4 ; //listener
		}
#endif
		else
		{
			instAddress = GATEWAY_ANCHOR_ADDR | instance_anchaddr;
		}
	}
	else
	{
		instAddress = instance_anchaddr;
	}

	instancesetaddresses(instAddress);
}

uint32 inittestapplication(uint16 s1switch);


//returns the use case / operational mode
int decarangingmode(uint16 s1switch)
{
	int mode = 0;

	if(s1switch & SWS1_SHF_MODE)
	{
		mode = 1;
	}

	if(s1switch & SWS1_CH5_MODE)
	{
		mode = mode + 2;
	}

	return mode;
}

uint32 inittestapplication(uint16 s1switch)
{


	uint32 devID ;
	instanceConfig_t instConfig;
	int result;
	uint64 uid_new = 0x100768105E37A70B;

	SPI_ConfigFastRate(SPI_BAUDRATEPRESCALER_16);  //max SPI before PLLs configured is ~4M

	//this is called here to wake up the device (i.e. if it was in sleep mode before the restart)
	devID = instancereaddeviceid() ;
	if(DWT_DEVICE_ID != devID) //if the read of device ID fails, the DW1000 could be asleep
	{
		port_SPIx_clear_chip_select();  //CS low
		Sleep(1);   //200 us to wake up then waits 5ms for DW1000 XTAL to stabilise
		port_SPIx_set_chip_select();  //CS high
		Sleep(7);
		devID = instancereaddeviceid() ;
		// SPI not working or Unsupported Device ID
		if(DWT_DEVICE_ID != devID)
			return(-1) ;
		//clear the sleep bit - so that after the hard reset below the DW does not go into sleep
		dwt_softreset();
	}

	//reset the DW1000 by driving the RSTn line low
	reset_DW1000();

	dwt_seteui((uint8 *) &uid_new);

	result = instance_init() ;
	if (0 > result) return(-1) ; // Some failure has occurred

	SPI_ConfigFastRate(SPI_BAUDRATEPRESCALER_4); //increase SPI to max
	devID = instancereaddeviceid() ;

	if (DWT_DEVICE_ID != devID)   // Means it is NOT DW1000 device
	{
		// SPI not working or Unsupported Device ID
		return(-1) ;
	}

	if((s1switch & SWS1_ANC_MODE) == 0)
	{
		instance_mode = TAG;
	}
	else
	{
		instance_mode = ANCHOR;
	}

	addressconfigure(instance_mode) ;                            // set up initial payload configuration

#ifdef MATEO_IMPL
	if((instance_mode == ANCHOR) && (instance_anchaddr > NUM_ANCHOR))
	{
		instance_mode = LISTENER;
	}
#else
	if((instance_mode == ANCHOR) && (instance_anchaddr > 0x4))
	{
		instance_mode = LISTENER;
	}
#endif

	instancesetrole(instance_mode) ;     // Set this instance role

	// get mode selection (index) this has 4 values see chConfig struct initialiser for details.
	dr_mode = decarangingmode(s1switch);

	chan = instConfig.channelNumber = chConfig[dr_mode].channel ;
	instConfig.preambleCode = chConfig[dr_mode].preambleCode ;
	instConfig.pulseRepFreq = chConfig[dr_mode].prf ;
	instConfig.pacSize = chConfig[dr_mode].pacSize ;
	instConfig.nsSFD = chConfig[dr_mode].nsSFD ;
	instConfig.sfdTO = chConfig[dr_mode].sfdTO ;
	instConfig.dataRate = chConfig[dr_mode].datarate ;
	instConfig.preambleLen = chConfig[dr_mode].preambleLength ;

	instance_config(&instConfig, &sfConfig[dr_mode]) ;                  // Set operating channel etc

	return devID;
}
/**
 **===========================================================================
 **
 **  Abstract: main program
 **
 **===========================================================================
 */
void process_dwRSTn_irq(void)
{
	instance_notify_DW1000_inIDLE(1);
}

void process_deca_irq(void)
{


	do{

		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_1,SET);
		HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_6);
		instance_process_irq(0);

	} while(port_CheckEXT_IRQ() == GPIO_PIN_SET); //while IRS line active (ARM can only do edge sensitive interrupts)

	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_1,RESET);
}

void uwbPrintRole(uint16 s1switch)
{
	int role = instancegetrole();

#ifdef MATEO_IMPL
	sprintf((char*)&dataseq[0], "DecaRangeRTLS %s%d Time Final=%d", (s1switch & SWS1_SHF_MODE) ? "S" : "L", chan, time_final);
	uartWriteLineNoOS((char *) dataseq); //send some data

	sprintf((char*)&dataseqMATEO[0], "Slot Size = %dms  Scheduled Final =%dms" , slot_size,sch_final_delay);
	uartWriteLineNoOS((char *) dataseqMATEO); //send some data

	sprintf((char*)&dataseqMATEO2[0], "Super Frame Size = %dms ", super_frame_size);
	uartWriteLineNoOS((char *) dataseqMATEO2); //send some data

	sprintf((char*)&dataseqMATEO3[0], " Number of possible tags = %d", num_disp);
	uartWriteLineNoOS((char *) dataseqMATEO3); //send some data

	sprintf((char*)&dataseqMATEO4[0], " Number of Anchors used = %d", num_anchor);
	uartWriteLineNoOS((char *) dataseqMATEO4); //send some data

#else
	sprintf((char*)&dataseq[0], "DecaRangeRTLS %s%d ", (s1switch & SWS1_SHF_MODE) ? "S" : "L", chan);
	uartWriteLineNoOS((char *) dataseq); //send some data
#endif
	tagaddr = instance_anchaddr;
	ancaddr = instance_anchaddr;

	if(role == TAG)
	{
		sprintf((char*)&dataseq1[0], "Tag:%d    ", tagaddr);
		uartWriteLineNoOS((char *) dataseq1); //send some data

	}
	else if(role == ANCHOR)
	{
		sprintf((char*)&dataseq1[0], "Anchor:%d ", ancaddr);
		uartWriteLineNoOS((char *) dataseq1); //send some data
	}
	else
	{
#ifdef MATEO_IMPL
		ancaddr = NUM_ANCHOR;
		sprintf((char*)&dataseq1[0], "Listener ");
		uartWriteLineNoOS((char *) dataseq1); //send some data
	}
#else
	ancaddr = 4;
	sprintf((char*)&dataseq1[0], "Listener ");
	uartWriteLineNoOS((char *) dataseq1); //send some data
}
#endif

}


void configure_continuous_txspectrum_mode(uint16 s1switch)
{
	sprintf((char*)&dataseq[0], "Continuous TX %s%d", (s1switch & SWS1_SHF_MODE) ? "S" : "L", chan);
	uartWriteLineNoOS((char *) dataseq); //send some data
	memcpy(dataseq, (const uint8 *) "Spectrum Test   ", 16);
	uartWriteLineNoOS((char *) dataseq); //send some data

	//configure DW1000 into Continuous TX mode
	instance_starttxtest(0x1000);
	//measure the power
	//Spectrum Analyser set:
	//FREQ to be channel default e.g. 3.9936 GHz for channel 2
	//SPAN to 1GHz
	//SWEEP TIME 1s
	//RBW and VBW 1MHz
	//measure channel power

	//user has to reset the board to exit mode
	while(1)
	{
		Sleep(2);
	}

}


/*
 *
 */
void uwb_interrupt_cb(void)
{
	if(uwbInitComplete)
	{
		xSemaphoreGiveFromISR(uwbInterruptSemHandle, NULL);
	}
}


/*
 * @fn      main()
 * @brief   main entry point
 **/
extern instance_data_t instance_data[NUM_INST] ;

#pragma GCC optimize ("O3")

int uwb_setup(void)
{
	uartWriteLineNoOS("DECAWAVE");
	uartWriteLineNoOS(SOFTWARE_VER_STRING); // Also set at line #26 (Should make this from single value !!!)



#ifdef MATEO_IMPL

	Init_Param();

	for(int i=0 ; i < NUM_DATA_ARRAY ; i++){
		s1switch |= TA_SW3[i] << i;
	}

	for(int i=(NUM_DATA_ARRAY-2) ; i>3 ; i--){
		Init_instance_anchaddr |= TA_SW3[i] << (NUM_DATA_ARRAY-2-i);
	}

	MaskAddr = pow(2,Naddress)-1 ;


//	free(TA_SW3);

#else
	s1switch = BUTTON_0 << 1 // is_switch_on(TA_SW1_2) << 2
			| TA_SW1_3 << 2
			| TA_SW1_4 << 3
			| TA_SW1_5 << 4
			| TA_SW1_6 << 5
			| TA_SW1_7 << 6
			| TA_SW1_8 << 7;
#endif
	port_DisableEXT_IRQ(); //disable ScenSor IRQ until we configure the device

	uartWriteLineNoOS("DECAWAVE TREK");

	if(inittestapplication(s1switch) == (uint32)-1)
	{
		uartWriteLineNoOS("ERROR - INIT FAIL");
		return 0; //error
	}

	// Is continuous spectrum test mode selected?
	if((s1switch & SWS1_TXSPECT_MODE) == SWS1_TXSPECT_MODE)
	{
		//this function does not return!
		configure_continuous_txspectrum_mode(s1switch);
	}

	uwbPrintRole(s1switch);

	port_EnableEXT_IRQ(); //enable ScenSor IRQ before starting

	osThreadDef(uwbMainTask, UwbMainTask, osPriorityNormal, 0, 1024);
	uwbMainTaskHandle = osThreadCreate(osThread(uwbMainTask), 0);

	osThreadDef(uwbProcessInterruptTask, UwbProcessInterruptTask, osPriorityAboveNormal, 0, 1024);
	uwbProcessInterruptTaskHandle = osThreadCreate(osThread(uwbProcessInterruptTask), 0);

	uwbInterruptSemHandle = xSemaphoreCreateBinary();

	uwbInitComplete = 1;

	return 0;

}

void UwbMainTask(void const * argument) {

	UNUSED(argument);

	//int i = 0;
	int rx = 0;
	int toggle = 0;
	//uint8 dataseq[40] = "";


	// main loop
	while(1)
	{
		//int n = 0;

		int monitor_local = instance_data[0].monitor ;
		int txdiff = (portGetTickCnt() - instance_data[0].timeofTx);

		osThreadYield();
		//Sleep(1);




		instance_run();
		instance_mode = instancegetrole();

		//if delayed TX scheduled but did not happen after expected time then it has failed... (has to be < slot period)
		//if anchor just go into RX and wait for next message from tags/anchors
		//if tag handle as a timeout
		if((monitor_local == 1) && ( txdiff > instance_data[0].slotPeriod))
		{
			int an = 0;
			uint32 tdly ;
			uint32 reg1, reg2;

			reg1 = dwt_read32bitoffsetreg(0x0f, 0x1);
			reg2 = dwt_read32bitoffsetreg(0x019, 0x1);
			tdly = dwt_read32bitoffsetreg(0x0a, 0x1);
			an = sprintf((char*)&dataseq1[0], "T%08x %08x time %08x %08x", (unsigned int) reg2, (unsigned int) reg1,
					(unsigned int) dwt_read32bitoffsetreg(0x06, 0x1), (unsigned int) tdly);
			uartWriteLineNoOS((char *) dataseq1);


			//sprintf((char*)&dataseq1[0], "Anchor:%d ", ancaddr);

			instance_data[0].wait4ack = 0;

			if(instance_mode == TAG)
			{
				inst_processrxtimeout(&instance_data[0]);
			}
			else //if(instance_mode == ANCHOR)
			{
				dwt_forcetrxoff();	//this will clear all events
				//dwt_rxreset();
				//enable the RX
				instance_data[0].testAppState = TA_RXE_WAIT ;
			}
			instance_data[0].monitor = 0;
		}

		rx = instancenewrange();

		//if there is a new ranging report received or a new range has been calculated, then prepare data
		//to output over USB - Virtual COM port, and update the LCD
		if(rx != TOF_REPORT_NUL)
		{
			int r= 0, aaddr, taddr; //l = 0,
			int rangeTime, valid;
			//int correction ;
			uint16 txa, rxa;

			//send the new range information to LCD and/or USB
			aaddr = instancenewrangeancadd() & 0xf;
			taddr = instancenewrangetagadd() & 0xf;
			rangeTime = instancenewrangetim() & 0xffffffff;

			//if((dr_mode & 0x1) == 0) //only print for 110k
			//if(printTWRReports + 2000 <= portGetTickCnt())
			if(true)
			{

				//anchors will print a range to each tag in sequence with 1 second pause
				//they will show the last rage to that tag
				if(instance_mode == ANCHOR)
				{
					int b = 0;
					double rangetotag = getTagDist(toggle) ;

					while(((int) (rangetotag*1000)) == 0) //if 0 then go to next tag
					{
						if(b > MAX_TAG_LIST_SIZE)
							break;

						toggle++;
						if(toggle >= MAX_TAG_LIST_SIZE)
							toggle = 0;

						rangetotag = getTagDist(toggle) ;
						b++;
					}

					sprintf((char*)&dataseq[0], "A%d T%d: %3.2f m", ancaddr, toggle, rangetotag);
					uartWriteLineNoOS((char *) dataseq); //send some data

					toggle++;

					if(toggle >= MAX_TAG_LIST_SIZE)
						toggle = 0;
				}
				else if(instance_mode == TAG)
				{
					sprintf((char*)&dataseq[0], "T%d A%d: %3.2f m", tagaddr, toggle, instancegetidist(toggle));
					//toggle = 1;
					uartWriteLineNoOS((char *) dataseq); //send some data

					toggle++;

					if(toggle >= MAX_ANCHOR_LIST_SIZE)
						toggle = 0;
				}


				//update the print time
				printTWRReports = portGetTickCnt();
			}
		} //if new range present

		//HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_RESET);

	}
}


void UwbInitTask(void const * argument) {

	UNUSED(argument);

	uwb_setup();

	osThreadTerminate(NULL);

}

void UwbProcessInterruptTask(void const * argument) {

	UNUSED(argument);

	xSemaphoreTake(uwbInterruptSemHandle,0); // don't block just take

	while(1) {

		if(xSemaphoreTake(uwbInterruptSemHandle, osWaitForever) == pdTRUE) {
			process_deca_irq();
		}

		//osDelay(1);
	}
}

#ifdef MATEO_IMPL

// Configure the Virtual Switch on the Device dependent on the input parameters on parameter.h
void Init_Param(void){


	Naddress = (int) ceil(log2((double)NUM_DISP));
	NUM_DATA_ARRAY = 4 + Naddress + 1;

	int BinaryArray[Naddress];
	long dividend = DEVICE_ID;
	for(int j=Naddress-1; j>=0; j--){
		BinaryArray[j] = dividend % 2;
		dividend /= 2;
	}

	bool *SWInit = pvPortMalloc(NUM_DATA_ARRAY*sizeof(bool));
	SWInit[0] = SET_ON;
	SWInit[1] = DATA_RATE;
	SWInit[2] = OPERATION_CHANNEL;
	SWInit[3] = DEVICE_TYPE;

	for(int i=4; i<NUM_DATA_ARRAY-1; i++){
		SWInit[i] = (bool)BinaryArray[i-4];
	}

	SWInit[NUM_DATA_ARRAY-1] = RESERVED_SW;

	TA_SW3 = &SWInit[0];


	vPortFree(SWInit);
}

#endif
