/**
 * \file limit_switch_slave.ino
 * \brief This ino file contains EasyCAT slave configuration for limit switch inputs.
 * Veysi ADIN 22 Apr 2021 (veysi.adin@kist.re.kr)
 * @note Inputs and outputs named based on arduino inputs.So if a variable named input_x_y, it's input for Arduino.Outputs are commands coming from EtherCAT master.
 */
///*********************************************************************************************
#define CUSTOM                      // Custom mode
#include "LimitSwitchSlave.h"       // This file has been created by the Easy Configurator 
                                    // and must be located in the Arduino project folder
                                    //
                                    // There are two others files created by the Easy Configurator:
                                    // LimitSwitchSlave.bin that must be loaded into the EEPROM.
                                    // LimitSwitchSlave.xml that can, when appropriate, be used by the EtherCAT master. 
                                    
///*********************************************************************************************


#include "EasyCAT.h"                // EasyCAT library to interface the LAN9252
#include <SPI.h>                    // SPI library
///-----------------------------------------------------------------------------------
EasyCAT EASYCAT;                    // EasyCAT istantiation

                                    // The constructor allow us to choose the pin used for the EasyCAT SPI chip select 
                                    // Without any parameter pin 9 will be used 
                   
                                    // We can choose between:
                                    // 8, 9, 10, A5, 6, 7                                    

                                    // On the EasyCAT board the SPI chip select is selected through a bank of jumpers                                                  // For this application we choose a cycle time of 150 mS

///---- global variables ---------------------------------------------------------------------------

uint32_t g_millis;
uint32_t g_sample_time;

///---- Inputs ---------------------------------------------------------------------------
uint8_t  right_limit_switch; 
uint8_t  left_limit_switch;
uint8_t  input_digital_03;
uint8_t  input_digital_04;
uint8_t  input_digital_05;
uint16_t input_analog_01;
uint16_t input_analog_02;
uint16_t input_analog_03;

///---- Outputs ---------------------------------------------------------------------------
uint8_t output_digital_01;
uint8_t output_digital_02;
uint8_t output_digital_03;
uint8_t output_digital_04;
uint8_t output_digital_05;
uint16_t output_analog_01;
uint16_t output_analog_02;
uint16_t output_analog_03;
///---- Com State ---------------------------------------------------------------------------
uint8_t g_ecat_state; 
//----- Input Pin Map --------------------------------------------------------------------------------------
#define R_LIMIT_SWITCH_PIN 1
#define L_LIMIT_SWITCH_PIN 2
#define I_DIGITAL_03_PIN  3
#define I_DIGITAL_04_PIN  4
#define I_DIGITAL_05_PIN  5
#define I_ANALOG_01_PIN  A1
#define I_ANALOG_02_PIN  A2
#define I_ANALOG_03_PIN  A3
//----- Output Pin Map --------------------------------------------------------------------------------------

#define O_DIGITAL_01_PIN  6
#define O_DIGITAL_02_PIN  7
#define O_DIGITAL_03_PIN  8
#define O_DIGITAL_04_PIN  10
#define O_DIGITAL_05_PIN  11
#define O_ANALOG_01_PIN   A0
#define O_ANALOG_02_PIN   A4
#define O_ANALOG_03_PIN   A5

#define BUILT_IN_LED     13

//-------------------------------------------------------------------------------------------------

#define ANALOG_INPUT_FILTER 0.90

#define SAMPLE_TIME_MS 1
#define MODE_OPERATIONAL 0x08
 
//---- setup ---------------------------------------------------------------------------------------
void setup()
{
  Serial.begin(9600);                                             // serial line initialization
                                                                  //(used only for debug)  
  Serial.print ("\nEasyCAT - Custom slave\n");                    // print the banner
  delay(3000);  
                                                                  //---- initialize the EasyCAT board -----
                                                                  
  if (EASYCAT.Init() == true)                                     // initialization
  {                                                               // succesfully completed
    Serial.print ("initialized");                                 //
  }                                                               //

  else                                                            // initialization failed   
  {                                                               // the EasyCAT board was not recognized
    Serial.print ("initialization failed");                       //     
                                                                  // The most common reason is that the SPI 
                                                                  // chip select choosen on the board doesn't 
                                                                  // match the one choosen by the firmware                                                             //  
  }  
  right_limit_switch = 0x00; 
  left_limit_switch  = 0x00;
  input_digital_03   = 0x00;
  input_digital_04   = 0x00;
  input_digital_05   = 0x00;
  input_analog_01    = 0x00;
  input_analog_02    = 0x00;
  input_analog_03    = 0x00;
  output_digital_01  = 0x00;
  output_digital_02  = 0x00;
  output_digital_03  = 0x00;
  output_digital_04  = 0x00;
  output_digital_05  = 0x00;
  output_analog_01   = 0x00;
  output_analog_02   = 0x00;
  output_analog_03   = 0x00;
  input_digital_03   = 0x00;
  g_sample_time = millis();                                       //
}
void loop()                                                       // In the main loop we must call ciclically the 
{                                                                 // EasyCAT task and our application  
                                                                  //
                                                                  // This allows the bidirectional exachange of the data
                                                                  // between the EtherCAT master and our application
                                                                  //
                                                                  // The EasyCAT cycle and the Master cycle are asynchronous
                                         
  g_ecat_state = EASYCAT.MainTask();                                 // execute the EasyCAT task
  
  Application();                                                  // execute the user applications
}


//---- user application ------------------------------------------------------------------------------

void Application ()                                        

{       

///---- Inputs ---------------------------------------------------------------------------
uint8_t r_limit_switch;
uint8_t l_limit_switch;
uint8_t  i_digital_03;
uint8_t  i_digital_04;
uint8_t  i_digital_05;
float i_analog_01;
float i_analog_02;
float i_analog_03;

///---- Outputs ---------------------------------------------------------------------------
uint8_t o_digital_01;
uint8_t o_digital_02;
uint8_t o_digital_03;
uint8_t o_digital_04;
uint8_t o_digital_05;
float o_analog_01;
float o_analog_02;
float o_analog_03;
  uint8_t i;


  g_millis = millis();                                                // in this custom slave we choose
  if (g_millis - g_sample_time >= SAMPLE_TIME_MS)                      // a cycle time of 1 mS 
  {                                                                   // 
    g_sample_time = g_millis;                                          //  

    if (g_ecat_state == MODE_OPERATIONAL)                          // if the slave is in operational
    {                                                             // manage the I/O
                           
      o_digital_01 = EASYCAT.BufferOut.Cust.output_digital_01;                 // read the status of the display segments from the master
      o_digital_02 = EASYCAT.BufferOut.Cust.output_digital_02;                 // read the status of the display segments from the master
      o_digital_03 = EASYCAT.BufferOut.Cust.output_digital_03;                 // read the status of the display segments from the master
      o_digital_04 = EASYCAT.BufferOut.Cust.output_digital_04;                 // read the status of the display segments from the master
      o_digital_05 = EASYCAT.BufferOut.Cust.output_digital_05;                 // read the status of the display segments from the master
      o_analog_01 = EASYCAT.BufferOut.Cust.output_analog_01;                   // read the status of the display segments from the master
      o_analog_02 = EASYCAT.BufferOut.Cust.output_analog_02;                   // read the status of the display segments from the master
      o_analog_03 = EASYCAT.BufferOut.Cust.output_analog_03;                   // read the status of the display segments from the master                                                    //
                                           
      // read the analog inputs and filter the values
      i_analog_01 = ANALOG_INPUT_FILTER*i_analog_01 + (1.0-ANALOG_INPUT_FILTER)*analogRead(I_ANALOG_01_PIN);
      i_analog_02 = ANALOG_INPUT_FILTER*i_analog_02 + (1.0-ANALOG_INPUT_FILTER)*analogRead(I_ANALOG_02_PIN);
      i_analog_02 = ANALOG_INPUT_FILTER*i_analog_03 + (1.0-ANALOG_INPUT_FILTER)*analogRead(I_ANALOG_03_PIN);
      // read digital inputs
      r_limit_switch = digitalRead(R_LIMIT_SWITCH_PIN);
      l_limit_switch = digitalRead(L_LIMIT_SWITCH_PIN);
      i_digital_03   = digitalRead(I_DIGITAL_03_PIN);
      i_digital_04   = digitalRead(I_DIGITAL_04_PIN);
      i_digital_05   = digitalRead(I_DIGITAL_05_PIN);
      
      // send the value to the master
      EASYCAT.BufferIn.Cust.right_limit_switch = r_limit_switch ;
      EASYCAT.BufferIn.Cust.left_limit_switch = l_limit_switch ; 
      EASYCAT.BufferIn.Cust.input_digital_03 = i_digital_03 ; 
      EASYCAT.BufferIn.Cust.input_digital_04 = i_digital_04 ; 
      EASYCAT.BufferIn.Cust.input_digital_05 = i_digital_05 ; 
      EASYCAT.BufferIn.Cust.input_analog_01 = (uint16_t)i_analog_01;
      EASYCAT.BufferIn.Cust.input_analog_02 = (uint16_t)i_analog_02;
      EASYCAT.BufferIn.Cust.input_analog_03 = (uint16_t)i_analog_03;
    }          

    else                                                          // if the slave is not in Operational
    {                                                             // signal it on the display

      digitalWrite(BUILT_IN_LED,HIGH) ; 
      delay(500);                                                //
      digitalWrite(BUILT_IN_LED,LOW) ; 
      delay(100); 
      digitalWrite(BUILT_IN_LED,HIGH) ;                          //
      delay(500);                                                 //
    }                                                             //
  }   
}
