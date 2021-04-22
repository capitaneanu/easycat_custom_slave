#ifndef CUSTOM_PDO_NAME_H
#define CUSTOM_PDO_NAME_H

//-------------------------------------------------------------------//
//                                                                   //
//     This file has been created by the Easy Configurator tool      //
//                                                                   //
//     Easy Configurator project LimitSwitchSlave.prj
//                                                                   //
//-------------------------------------------------------------------//


#define CUST_BYTE_NUM_OUT	11
#define CUST_BYTE_NUM_IN	11
#define TOT_BYTE_NUM_ROUND_OUT	12
#define TOT_BYTE_NUM_ROUND_IN	12


typedef union												//---- output buffer ----
{
	uint8_t  Byte [TOT_BYTE_NUM_ROUND_OUT];
	struct
	{
		int16_t     output_analog_01;
		int16_t     output_analog_02;
		int16_t     output_analog_03;
		uint8_t     output_digital_04;
		uint8_t     output_digital_05;
		uint8_t     output_digital_01;
		uint8_t     output_digital_02;
		uint8_t     output_digital_03;
	}Cust;
} PROCBUFFER_OUT;


typedef union												//---- input buffer ----
{
	uint8_t  Byte [TOT_BYTE_NUM_ROUND_IN];
	struct
	{
		uint16_t    input_analog_01;
		uint16_t    input_analog_02;
		uint16_t    input_analog_03;
		uint8_t     input_digital_04;
		uint8_t     input_digital_05;
		uint8_t     left_limit_switch;
		uint8_t     right_limit_switch;
		uint8_t     input_digital_03;
	}Cust;
} PROCBUFFER_IN;

#endif