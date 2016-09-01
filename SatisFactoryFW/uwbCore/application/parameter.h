/*! ----------------------------------------------------------------------------
 *  @file    parameter.h
 *  @brief   Device input parameters
 *
 * @attention
 *
 * Copyright 2015 (c) DecaWave Ltd, Dublin, Ireland.
 *
 * All rights reserved.
 *
 * @author DecaWave
 */

#ifndef _PARAMETER_H_
#define _PARAMETER_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "deca_types.h"
#include "deca_device_api.h"
#include "compiler.h"
#define MATEO_IMPL

//#define NUM_ANCHORTEST
//#define NUM_ANCHORTEST2

#ifdef MATEO_IMPL


bool *TA_SW3;
int NUM_DATA_ARRAY;
int Naddress;
int MaskAddr;


//Device Parameters
#define NUM_DISP 						(50) // 	Number of Tag to Use NUM_DISP - 1
#define DEVICE_ID 						(3) // Device ID in Decimal From 0 to
#define FREQUENCY 						(1)  // Number of localization per TAG (one every superframe, one every 2 superframes etc.)
#define DEVICE_TYPE						TRUE  // FALSE = TAG , TRUE = ANCHOR
#define DATA_RATE						FALSE  // FALSE = 110kbps , TRUE = 6,81Mbps
#define OPERATION_CHANNEL				TRUE   // FALSE = Channel 2 (3.993 GHz) , TRUE = Channel 5(6.489 GHz)

#define MAX_TAG_LIST_SIZE				(NUM_DISP) //

#if (DEVICE_ID > NUM_DISP)
	#error "Device ID wrong! (greater than NUM_DISP)"
#endif

//LOWER DATA RATE
#define TOTAL_NUMBER_OF_SLOTS			(MAX_TAG_LIST_SIZE+2)
#define SLOT_SIZE 						(23)
#define SUPERFRAME_SIZE					(SLOT_SIZE*TOTAL_NUMBER_OF_SLOTS)
#define SCHEDULED_FINAL_DELAY			(17500)  //

// HIGHER DATA RATE
#define TOTAL_NUMBER_OF_SLOTS			(MAX_TAG_LIST_SIZE+2)
#define SLOT_SIZE_HDR 					(2.3)
#define SUPERFRAME_SIZE_HDR				(SLOT_SIZE*TOTAL_NUMBER_OF_SLOTS)
#define SCHEDULED_FINAL_DELAY_HDR		(2100)  //



#define SWITCH_ON 			TRUE
#define SWITCH_OFF 			FALSE
#define SET_ON				FALSE  // FALSE = ON , True =OFF (This switch should be set to ON)
#define RESERVED_SW			FALSE  // This switch should be set to FALSE

void Init_Param(void);


#else
	#define MAX_TAG_LIST_SIZE				(8)
#endif


#ifdef NUM_ANCHORTEST

	#define MAX_ANCHOR_LIST_SIZE			(3) //this is limited to 3 in this application
	#define NUM_EXPECTED_RESPONSES			(MAX_ANCHOR_LIST_SIZE-1) //e.g. MAX_ANCHOR_LIST_SIZE - 1
	#define NUM_EXPECTED_RESPONSES_ANC		(1) //anchors A0, A1 and A2 are involved in anchor to anchor ranging
	#define NUM_EXPECTED_RESPONSES_ANC0		(2) //anchor A0 expects response from A1 and A2

#else
	#define MAX_ANCHOR_LIST_SIZE			(4) //this is limited to 4 in this application
	#define NUM_EXPECTED_RESPONSES			(3) //e.g. MAX_ANCHOR_LIST_SIZE - 1
	#define NUM_EXPECTED_RESPONSES_ANC		(1) //anchors A0, A1 and A2 are involved in anchor to anchor ranging
	#define NUM_EXPECTED_RESPONSES_ANC0		(2) //anchor A0 expects response from A1 and A2
#endif


//this it the delay used for configuring the receiver on delay (wait for response delay)
//NOTE: this RX_RESPONSE_TURNAROUND is dependent on the microprocessor and code optimisations
#define RX_RESPONSEX_TURNAROUND (50) //takes about 100 us for response to come back
#define RX_RESPONSE1_TURNAROUND (200) //takes about 200 us for the 1st response to come back (from A0)
#define RX_RESPONSE1_TURNAROUND_6M81 (300) //takes about 100 us for response to come back
#define RX_RESPONSE1_TURNAROUND_110K (300) //takes about 100 us for response to come back



#ifdef __cplusplus
}
#endif

#endif

