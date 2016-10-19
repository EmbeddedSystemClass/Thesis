/*! ----------------------------------------------------------------------------
 *  @file    parameter.h
 *  @brief   Device input parameters
 *
 * Created by: Mateo Giraldo
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

#ifdef MATEO_IMPL

	//#define LOCALIZATION_2D   // UnComment to work with 2D localization
	//#define SHORT_FRAME_2D  // UnComment to use shorter frames in 2D localization

	bool *TA_SW3;
	int NUM_DATA_ARRAY;
	int Naddress;
	int MaskAddr;
	int time_final;

	//--------------------------------------------------- DEVICE PARAMETERS ---------------------------------------------------------//

	#define NUM_DISP 						(40) // 	Number of Tag to Use
	#define DEVICE_ID 						(1) // Device ID in Decimal From 0 to (NUM_DISP - 1)
	#define FREQUENCY 						(1)  // Number of localization per TAG (one every superframe, one every 2 superframes etc.)
	#define DEVICE_TYPE						FALSE  // FALSE = TAG , TRUE = ANCHOR
	#define DATA_RATE						FALSE  // FALSE = 110kbps , TRUE = 6,81Mbps
	#define OPERATION_CHANNEL				TRUE   // FALSE = Channel 2 (3.993 GHz) , TRUE = Channel 5(6.489 GHz)
	#define NUM_ANCHOR						(9)   // Number of expected responses

	#define MAX_TAG_LIST_SIZE				(NUM_DISP) //
    #define TOTAL_NUMBER_OF_SLOTS			(MAX_TAG_LIST_SIZE+2)

	#if (DEVICE_ID > NUM_DISP)
		#error "Device ID wrong! (greater than NUM_DISP)"
	#endif

	//--------------------------------------------------- OPTIMIZED TIMES ---------------------------------------------------------//

	//This times are the message lengths in us
	//LOWER DATA RATE
	#define TIME_RESP_1 			(2620)
	#define TIME_RESP_OTHERS 		(3100)
	#define MARGIN_FINAL_DELAY 		(80)
#if (NUM_ANCHOR>4)
	//#define TIME_FINAL 			((int)(5500+(((((NUM_ANCHOR-4)*5)*8*8205.13)+172308)/1000)))
	#define TIME_FINAL 				((int)(5500+((NUM_ANCHOR-4)*600)))
	//#define TIME_FINAL 			((int)(300+(((((NUM_ANCHOR-4)*5)*8*128.21)+21539)/1000)))
	#define TIME_FINAL_HDR 			((int)(300 +((NUM_ANCHOR-4)*50)))
#else
	#define TIME_FINAL 				(5500)
	#define TIME_FINAL_HDR 			(300)
#endif

	// HIGHER DATA RATE
	#define TIME_RESP_1_HDR 		(320)
	#define TIME_RESP_OTHERS_HDR 	(340)
	#define MARGIN_FINAL_DELAY_HDR 	(65)


	#ifdef LOCALIZATION_2D

	//------------------------------------------------WORKING FOR 2D LOCALIZATION (ONLY 3 ANCHORS)------------------------------------------------------------//


		#define MAX_ANCHOR_LIST_SIZE			(3) //this is limited to 3 in this application
		#define NUM_EXPECTED_RESPONSES			(MAX_ANCHOR_LIST_SIZE-1) //e.g. MAX_ANCHOR_LIST_SIZE - 1
		#define NUM_EXPECTED_RESPONSES_ANC		(1) //anchors A0, A1 and A2 are involved in anchor to anchor ranging
		#define NUM_EXPECTED_RESPONSES_ANC0		(2) //anchor A0 expects response from A1 and A2

		#ifdef SHORT_FRAME_2D
  //-------------------------------------------------SHORTER FINAL MESSAGE SIZE------------------------------------------------------------//
			#define TAG_FINAL_MSG_LEN                   28	//FunctionCode(1), Range Num (1), Poll_TxTime(5), // Resp0_RxTime(5), Resp1_RxTime(5), Resp2_RxTime(5), Final_TxTime(5), Valid Response Mask (1)
			#define FTXT                                22	// Final TX time
			#define VRESP                               27	// Mask of valid response times (e.g. if bit 1 = A0's response time is valid)


	//------------------------------------------------SHORTER FRAMES------------------------------------------------------------//
			//LOWER DATA RATE
			#define SLOT_SIZE 						((TIME_RESP_1+(TIME_RESP_OTHERS*2)+(TIME_FINAL*2)+MARGIN_FINAL_DELAY)/1000) // slot period in ms
			#define SUPERFRAME_SIZE					(SLOT_SIZE*TOTAL_NUMBER_OF_SLOTS) // Super frame period in ms
			#define SCHEDULED_FINAL_DELAY			(TIME_RESP_1+(TIME_RESP_OTHERS*2)+TIME_FINAL+MARGIN_FINAL_DELAY)  // scheduled final delay in us
																													//No need to wait for the response of the anchor 4

			// HIGHER DATA RATE
			#define SLOT_SIZE_HDR 					((TIME_RESP_1_HDR+(TIME_RESP_OTHERS_HDR*2)+(TIME_FINAL_HDR*2)+MARGIN_FINAL_DELAY_HDR)/1000) // slot period in ms
			#define SUPERFRAME_SIZE_HDR				(SLOT_SIZE*TOTAL_NUMBER_OF_SLOTS) // Super frame period in ms
			#define SCHEDULED_FINAL_DELAY_HDR		(TIME_RESP_1_HDR+(TIME_RESP_OTHERS_HDR*2)+TIME_FINAL_HDR+MARGIN_FINAL_DELAY_HDR)  // scheduled final delay in us


		#else
	//------------------------------------------------LARGER FINAL MSG LENGTH AND FRAMES------------------------------------------------------------//

			#define TAG_FINAL_MSG_LEN                   (33)        // FunctionCode(1), Range Num (1), Poll_TxTime(5),// Resp0_RxTime(5), Resp1_RxTime(5), Resp2_RxTime(5), Resp3_RxTime(5), Final_TxTime(5), Valid Response Mask (1)
			#define FTXT                                (27)		// Final TX time
			#define VRESP                               (32)		// Mask of valid response times (e.g. if bit 1 = A0's response time is valid)

		//LOWER DATA RATE
			#define SLOT_SIZE 						((TIME_RESP_1+(TIME_RESP_OTHERS*3)+(TIME_FINAL*2)+MARGIN_FINAL_DELAY)/1000) // slot period in ms
			#define SUPERFRAME_SIZE					(SLOT_SIZE*TOTAL_NUMBER_OF_SLOTS) // Super frame period in ms
			#define SCHEDULED_FINAL_DELAY			(TIME_RESP_1+(TIME_RESP_OTHERS*3)+TIME_FINAL+MARGIN_FINAL_DELAY)  //  scheduled final delay in us

			// HIGHER DATA RATE
			#define SLOT_SIZE_HDR 					((TIME_RESP_1_HDR+(TIME_RESP_OTHERS_HDR*3)+(TIME_FINAL_HDR*2)+MARGIN_FINAL_DELAY_HDR)/1000) // slot period in ms
			#define SUPERFRAME_SIZE_HDR				(SLOT_SIZE*TOTAL_NUMBER_OF_SLOTS) // Super frame period in ms
			#define SCHEDULED_FINAL_DELAY_HDR		(TIME_RESP_1_HDR+(TIME_RESP_OTHERS_HDR*3)+TIME_FINAL_HDR+MARGIN_FINAL_DELAY_HDR)  // scheduled final delay in us

		#endif


	#else
	//------------------------------------------------WORKING FOR 3D LOCALIZATION (4 ANCHORS)------------------------------------------------------------//

		#define MAX_ANCHOR_LIST_SIZE			(NUM_ANCHOR) //this is limited to 4 in this application
		#define NUM_EXPECTED_RESPONSES			(MAX_ANCHOR_LIST_SIZE-1) //e.g. MAX_ANCHOR_LIST_SIZE - 1
		#define NUM_EXPECTED_RESPONSES_ANC		(1) //anchors A0, A1 and A2 are involved in anchor to anchor ranging
		#define NUM_EXPECTED_RESPONSES_ANC0		(2) //anchor A0 expects response from A1 and A2

	//------------------------------------------------LARGER FINAL MSG LENGTH AND FRAMES------------------------------------------------------------//

		#define TAG_FINAL_MSG_LEN                   (13+((NUM_ANCHOR)*5))        // FunctionCode(1), Range Num (1), Poll_TxTime(5),	// Resp0_RxTime(5), Resp1_RxTime(5), Resp2_RxTime(5), Resp3_RxTime(5), Final_TxTime(5), Valid Response Mask (1)
		#define FTXT                                (TAG_FINAL_MSG_LEN-6)		// Final TX time
		#define VRESP                               (TAG_FINAL_MSG_LEN-1)		// Mask of valid response times (e.g. if bit 1 = A0's response time is valid)

		//LOWER DATA RATE
		#define SLOT_SIZE 						((TIME_RESP_1+(TIME_RESP_OTHERS*(NUM_ANCHOR-1))+(TIME_FINAL*2)+MARGIN_FINAL_DELAY)/1000) // slot period in ms
		#define SUPERFRAME_SIZE					(SLOT_SIZE*TOTAL_NUMBER_OF_SLOTS) // Super frame period in ms
		#define SCHEDULED_FINAL_DELAY			(TIME_RESP_1+(TIME_RESP_OTHERS*(NUM_ANCHOR-1))+TIME_FINAL+MARGIN_FINAL_DELAY)  // scheduled final delay in us

		// HIGHER DATA RATE
		#define SLOT_SIZE_HDR 					((TIME_RESP_1_HDR+(TIME_RESP_OTHERS_HDR*(NUM_ANCHOR-1))+(TIME_FINAL_HDR*2)+MARGIN_FINAL_DELAY_HDR)/1000) // slot period in ms
		#define SUPERFRAME_SIZE_HDR				(SLOT_SIZE*TOTAL_NUMBER_OF_SLOTS) // Super frame period in ms
		#define SCHEDULED_FINAL_DELAY_HDR		(TIME_RESP_1_HDR+(TIME_RESP_OTHERS_HDR*(NUM_ANCHOR-1))+TIME_FINAL_HDR+MARGIN_FINAL_DELAY_HDR)  // scheduled final delay in us


	#endif


	#define SWITCH_ON 			TRUE
	#define SWITCH_OFF 			FALSE
	#define SET_ON				FALSE  // FALSE = ON , True =OFF (This switch should be set to ON)
	#define RESERVED_SW			FALSE  // This switch should be set to FALSE

	void Init_Param(void);


#else
	#define MAX_TAG_LIST_SIZE				(8)
#endif


//this it the delay used for configuring the receiver on delay (wait for response delay)
//NOTE: this RX_RESPONSE_TURNAROUND is dependent on the microprocessor and code optimizations
#define RX_RESPONSEX_TURNAROUND (50) //takes about 100 us for response to come back
#define RX_RESPONSE1_TURNAROUND (200) //takes about 200 us for the 1st response to come back (from A0)
#define RX_RESPONSE1_TURNAROUND_6M81 (300) //takes about 100 us for response to come back
#define RX_RESPONSE1_TURNAROUND_110K (300) //takes about 100 us for response to come back

//#define RX_RESPONSE1_TURNAROUND_6M81 (NUM_EXPECTED_RESPONSES*100) //takes about 100 us for response to come back
//#define RX_RESPONSE1_TURNAROUND_110K (NUM_EXPECTED_RESPONSES*100) //takes about 100 us for response to come back



#ifdef __cplusplus
}
#endif

#endif


