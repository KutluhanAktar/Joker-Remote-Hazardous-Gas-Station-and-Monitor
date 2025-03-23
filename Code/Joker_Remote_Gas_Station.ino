         /////////////////////////////////////////////  
        //   Joker Remote Hazardous Gas Station    //
       //              and Monitor                //
      //             ---------------             //
     //             (Arduino Nano)              //           
    //             by Kutluhan Aktar           // 
   //                                         //
  /////////////////////////////////////////////

//
// Using an IR remote control, display the presence of hazardous gases w/ MQ-2, MQ-3, MQ-4, MQ-6, MQ-9 and get notified when they reach dangerous levels.
//
// For more information:
// https://www.theamplituhedron.com/projects/Joker_Remote_Hazardous_Gas_Station_and_Monitor
//
//
// Connections
// Arduino Nano :  
//                                IR Receiver Module
// D2  --------------------------- S         
//                                ST7789 240x240 IPS
// GND --------------------------- GND
// 3.3V -------------------------- VCC
// D13 --------------------------- SCL
// D11 --------------------------- SDA
// D9  --------------------------- RES
// D10 --------------------------- DC
//                                MQ-2 Air Quality Sensor
// A0  --------------------------- S
//                                MQ-3 Air Quality Sensor
// A1  --------------------------- S
//                                MQ-4 Air Quality Sensor
// A2  --------------------------- S
//                                MQ-6 Air Quality Sensor
// A3  --------------------------- S
//                                MQ-9 Air Quality Sensor
// A4  --------------------------- S
//                                5mm Common Anode RGB LED
// D3  --------------------------- R
// D5  --------------------------- G
// D6  --------------------------- B
//                                Buzzer
// D7  --------------------------- +

/*
 ST7789 240x240 IPS (without CS pin) connections (only 6 wires required):

 #01 GND -> GND
 #02 VCC -> VCC (3.3V only!)
 #03 SCL -> D13/SCK
 #04 SDA -> D11/MOSI
 #05 RES -> D8 or any digital
 #06 DC  -> D7 or any digital
 #07 BLK -> NC
*/

// Include the required libraries.
#include <Adafruit_GFX.h>
#include <Arduino_ST7789_Fast.h>
#include <IRremote.h>

// Define the IR receiver module pin and settings.
#define RECV_PIN 2
IRrecv irrecv(RECV_PIN); 
decode_results results;

// IR codes obtained from the IR remote control by executing the IRrecvDemo.ino file.
#define up 0xFF18E7
#define down 0xFF4AB5
#define right 0xFF5AA5
#define left 0xFF10EF
#define ok 0xFF38C7

// Define the ST7789 240x240 IPS display settings.
#define TFT_DC    10
#define TFT_RST   9
#define SCR_WD   240
#define SCR_HT   240

// Include the converted images:
#include "gas.c"
#include "lethal.c"
#include "toxic.c"

// Initiate the ST7789 240x240 IPS display.
Arduino_ST7789 tft = Arduino_ST7789(TFT_DC, TFT_RST); 

// Define the gas sensors' pins.
#define mq_2 A0
#define mq_3 A1
#define mq_4 A2
#define mq_6 A3
#define mq_9 A4

// Define RGB pins.
#define redPin 3
#define greenPin 5
#define bluePin 6

// Define the buzzer pin.
#define buzzer 7

// Define interface options:
volatile boolean mq2, mq3, mq4, mq6, mq9, activated;

// Define data holders:
int mq_2_val, mq_3_val, mq_4_val, mq_6_val, mq_9_val, selected = 0;
unsigned long IR_codeValue = 0;
 
void setup() {
  Serial.begin(9600);

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);  
  adjustColor(0,0,0);
  
  // Initiate the IR receiver module.
  irrecv.enableIRIn();

  // Open and clear the ST7789 240x240 IPS display.
  tft.init(SCR_WD, SCR_HT);
  tft.fillScreen(RGBto565(235, 46, 0));

}

void loop() {
  read_IR_controls();
  collect_Data();
  interface();
  change_menu_options();
  
  if(mq2){
    do{
      tft.setTextColor(RGBto565(0, 38, 153), WHITE);
      tft.setTextSize(4);
      tft.setCursor(75, 25);
      tft.println("MQ-2");
      if(IR_codeValue == ok){
        activated = true;
        tft.fillScreen(WHITE);
        while(activated){
          read_IR_controls();
          // Print data:
          collect_Data();
          tft.setCursor(75, 25);
          tft.setTextColor(BLACK, WHITE);
          tft.setTextSize(4);
          tft.print(F("MQ-2"));
          tft.drawImageF(30, 80, 40, 40, gas);
          tft.drawImageF(100, 80, 40, 40, lethal);
          tft.drawImageF(170, 80, 40, 40, toxic);
          tft.setCursor(60, 160);
          tft.setTextSize(7);
          tft.print(mq_2_val);
          // Check gas status:
          if(mq_2_val < 600){ adjustColor(0, 255, 0); }
          else if(mq_2_val >= 600 && mq_2_val < 700){ adjustColor(255, 255, 0); }
          else if(mq_2_val >= 700){ adjustColor(255, 0, 0); tone(buzzer, 500, 1000); }
          // Exit and Close:
          if(IR_codeValue == left){ activated = false; tft.fillScreen(RGBto565(235, 46, 0)); adjustColor(0, 0, 0); noTone(buzzer); }
        }
      }
    }while(!mq2);
  }

  if(mq3){
    do{
      tft.setTextColor(RGBto565(0, 38, 153), WHITE);
      tft.setTextSize(4);
      tft.setCursor(75, 65);
      tft.println("MQ-3");
      if(IR_codeValue == ok){
        activated = true;
        tft.fillScreen(WHITE);
        while(activated){
          read_IR_controls();
          // Print data:
          collect_Data();
          tft.setCursor(75, 25);
          tft.setTextColor(BLACK, WHITE);
          tft.setTextSize(4);
          tft.print(F("MQ-3"));
          tft.drawImageF(30, 80, 40, 40, gas);
          tft.drawImageF(100, 80, 40, 40, lethal);
          tft.drawImageF(170, 80, 40, 40, toxic);
          tft.setCursor(60, 160);
          tft.setTextSize(7);
          tft.print(mq_3_val);
          // Check gas status:
          if(mq_3_val < 650){ adjustColor(0, 255, 0); }
          else if(mq_3_val >= 650 && mq_3_val < 800){ adjustColor(255, 255, 0); }
          else if(mq_3_val >= 800){ adjustColor(255, 0, 0); tone(buzzer, 500, 1000); }
          // Exit and Close:
          if(IR_codeValue == left){ activated = false; tft.fillScreen(RGBto565(235, 46, 0)); adjustColor(0, 0, 0); noTone(buzzer); }
        }
      }
    }while(!mq3);
  }

  if(mq4){
    do{
      tft.setTextColor(RGBto565(0, 38, 153), WHITE);
      tft.setTextSize(4);
      tft.setCursor(75, 105);
      tft.println("MQ-4");
      if(IR_codeValue == ok){
        activated = true;
        tft.fillScreen(WHITE);
        while(activated){
          read_IR_controls();
          // Print data:
          collect_Data();
          tft.setCursor(75, 25);
          tft.setTextColor(BLACK, WHITE);
          tft.setTextSize(4);
          tft.print(F("MQ-4"));
          tft.drawImageF(30, 80, 40, 40, gas);
          tft.drawImageF(100, 80, 40, 40, lethal);
          tft.drawImageF(170, 80, 40, 40, toxic);
          tft.setCursor(60, 160);
          tft.setTextSize(7);
          tft.print(mq_4_val);
          // Check gas status:
          if(mq_4_val < 450){ adjustColor(0, 255, 0); }
          else if(mq_4_val >= 450 && mq_4_val < 600){ adjustColor(255, 255, 0); }
          else if(mq_4_val >= 600){ adjustColor(255, 0, 0); tone(buzzer, 500, 1000); }
          // Exit and Close:
          if(IR_codeValue == left){ activated = false; tft.fillScreen(RGBto565(235, 46, 0)); adjustColor(0, 0, 0); noTone(buzzer); }
        }
      }
    }while(!mq4);
  }

  if(mq6){
    do{
      tft.setTextColor(RGBto565(0, 38, 153), WHITE);
      tft.setTextSize(4);
      tft.setCursor(75, 145);
      tft.println("MQ-6");
      if(IR_codeValue == ok){
        activated = true;
        tft.fillScreen(WHITE);
        while(activated){
          read_IR_controls();
          // Print data:
          collect_Data();
          tft.setCursor(75, 25);
          tft.setTextColor(BLACK, WHITE);
          tft.setTextSize(4);
          tft.print(F("MQ-6"));
          tft.drawImageF(30, 80, 40, 40, gas);
          tft.drawImageF(100, 80, 40, 40, lethal);
          tft.drawImageF(170, 80, 40, 40, toxic);
          tft.setCursor(60, 160);
          tft.setTextSize(7);
          tft.print(mq_6_val);
          // Check gas status:
          if(mq_6_val < 500){ adjustColor(0, 255, 0); }
          else if(mq_6_val >= 500 && mq_6_val < 650){ adjustColor(255, 255, 0); }
          else if(mq_6_val >= 650){ adjustColor(255, 0, 0); tone(buzzer, 500, 1000); }
          // Exit and Close:
          if(IR_codeValue == left){ activated = false; tft.fillScreen(RGBto565(235, 46, 0)); adjustColor(0, 0, 0); noTone(buzzer); }
        }
      }
    }while(!mq6);
  }

  if(mq9){
    do{
      tft.setTextColor(RGBto565(0, 38, 153), WHITE);
      tft.setTextSize(4);
      tft.setCursor(75, 185);
      tft.println("MQ-9");
      if(IR_codeValue == ok){
        activated = true;
        tft.fillScreen(WHITE);
        while(activated){
          read_IR_controls();
          // Print data:
          collect_Data();
          tft.setCursor(75, 25);
          tft.setTextColor(BLACK, WHITE);
          tft.setTextSize(4);
          tft.print(F("MQ-9"));
          tft.drawImageF(30, 80, 40, 40, gas);
          tft.drawImageF(100, 80, 40, 40, lethal);
          tft.drawImageF(170, 80, 40, 40, toxic);
          tft.setCursor(60, 160);
          tft.setTextSize(7);
          tft.print(mq_9_val);
          // Check gas status:
          if(mq_9_val < 700){ adjustColor(0, 255, 0); }
          else if(mq_9_val >= 700 && mq_9_val < 850){ adjustColor(255, 255, 0); }
          else if(mq_9_val >= 850){ adjustColor(255, 0, 0); tone(buzzer, 500, 1000); }
          // Exit and Close:
          if(IR_codeValue == left){ activated = false; tft.fillScreen(RGBto565(235, 46, 0)); adjustColor(0, 0, 0); noTone(buzzer); }
        }
      }
    }while(!mq9);
  }

}

void read_IR_controls(){
  // Read received IR codes from the IR remote control.
  if(irrecv.decode(&results)){
    IR_codeValue = results.value;
    irrecv.resume();
    delay(500);
  }else{
    IR_codeValue = 0;
  }
}

void interface(){
  // Print interface with menu options.
  tft.setTextColor(RGBto565(0, 38, 153), RGBto565(235, 46, 0));
  tft.setTextSize(4);
  tft.setCursor(75, 25);
  tft.println("MQ-2");
  tft.setCursor(75, 65);
  tft.println("MQ-3");
  tft.setCursor(75, 105);
  tft.println("MQ-4");
  tft.setCursor(75, 145);
  tft.println("MQ-6");
  tft.setCursor(75, 185);
  tft.println("MQ-9");
}

void collect_Data(){
  // Get variables generated by the gas sensors.
  mq_2_val = analogRead(mq_2);
  mq_3_val = analogRead(mq_3);
  mq_4_val = analogRead(mq_4);
  mq_6_val = analogRead(mq_6);
  mq_9_val = analogRead(mq_9);
}

void change_menu_options(){
  // Increase or decrease the option number using the IR remote control buttons.
  if(IR_codeValue == up) selected--; 
  if(IR_codeValue == down) selected++; 
  if(selected < 0) selected = 5;
  if(selected > 5) selected = 1;
  delay(100);

  // Depending on the selected option number, change the boolean status.
  switch(selected){
    case 1:
      mq2 = true;
      mq3 = false;
      mq4 = false;
      mq6 = false;
      mq9 = false;
    break;
    case 2:     
      mq2 = false;
      mq3 = true;
      mq4 = false;
      mq6 = false;
      mq9 = false;
    break;
    case 3:
      mq2 = false;
      mq3 = false;
      mq4 = true;
      mq6 = false;
      mq9 = false;
    break;
    case 4:
      mq2 = false;
      mq3 = false;
      mq4 = false;
      mq6 = true;
      mq9 = false;
    break;
    case 5:
      mq2 = false;
      mq3 = false;
      mq4 = false;
      mq6 = false;
      mq9 = true;
    break;
  }
}

void adjustColor(int r, int g, int b){
  analogWrite(redPin, (255-r));
  analogWrite(greenPin, (255-g));
  analogWrite(bluePin, (255-b));
}
     
