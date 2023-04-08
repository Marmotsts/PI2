//Transmitter program
#include <SPI.h>
#include "Mirf.h"
#include "nRF24L01.h"
#include "MirfHardwareSpiDriver.h"
#include <LedControl.h>
#include "binary.h"

//define LED matrix pins
int DIN=12;
int CLK=11;
int CS=10;
LedControl lc=LedControl(DIN,CLK,CS,1);

//define commands 
#define CMD_FORWARD 10
#define CMD_BACKWARDS 20
#define CMD_SLIDE_R 30
#define CMD_SLIDE_L 40
#define CMD_OPEN 50
#define CMD_CLOSE 60
#define CMD_UP 70
#define CMD_DOWN 80
#define CMD_IDLE 90

//define drawings
/* 1 means led on, 0 means off
use this to generate code from a drawing
https://www.riyas.org/2013/12/online-led-matrix-font-generator-with.html
*/

byte arrow_down[8]= {
  B00111100,
  B00111100,
  B00111100,
  B00111100,
  B11111111,
  B01111110,
  B00111100,
  B00011000
};

byte arrow_up[8] = {
  B00011000,
  B00111100,
  B01111110,
  B11111111,
  B00111100,
  B00111100,
  B00111100,
  B00111100
};

byte square[8] = {
  B00000000,
  B00000000,
  B00111100,
  B00111100,
  B00111100,
  B00111100,
  B00000000,
  B00000000
};

byte arrow_right[8] ={
  B00001000,
  B00001100,
  B11111110,
  B11111111,
  B11111111,
  B11111110,
  B00001100,
  B00001000
};

byte arrow_left[8] ={
  B00010000,
  B00110000,
  B01111111,
  B11111111,
  B11111111,
  B01111111,
  B00110000,
  B00010000
};

/*byte up_left [8] ={
  B00111111,
  B00000111,
  B00001111,
  B00011101,
  B00111001,
  B01110001,
  B11100000,
  B11000000
};*/

byte smile [8] ={
  B00111100,
  B01000010,
  B10100101,
  B10000001,
  B10100101,
  B10011001,
  B01000010,
  B00111100
};

//nrf module ini.
Nrf24l Mirf = Nrf24l(10, 9); //don't touch, works but i have no idea why      CE, CSN pins
int message; // can only be a double digit integer so 0-99

void draw(byte *sprite){ //allume les LED pour afficher une image
    for(int r = 0; r < 8; r++){
        lc.setRow(0, r, sprite[r]);
    }
}

void setup(){
  // lire input joystick
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  // initialiser matrice DEL
  lc.shutdown(0,false);               //The MAX72XX is in power-saving mode on startup
  lc.setIntensity(0,1);               // (address, intesity) intensity goes from dimest 0-15 brightest
  draw(smile);                        //:)
  delay(500);                         // give you a moment to soak in it's glory
  lc.clearDisplay(0); 
  //Serial.begin(9600);               //uncomment for debug
  Mirf.spi = &MirfHardwareSpi;
  Mirf.init();
  Mirf.setRADDR((byte *)"ABCDE");     //Set your own address (sender address) using 5 characters
  Mirf.payload = sizeof(message);
  Mirf.channel = 90;                  //Set the channel used
  Mirf.config();
}

void loop(){
  //read analog input from joystick
  int horizontal_axis = analogRead(A1);
  int vertical_axis = analogRead(A0);
  /*check les positions du joystick
  valeurs mesurées avec joystick:
  Idle value for x = ~492; y axis = ~519
  600 for upper limit and 400 for lower limit works fine
  */
  int max = 800;
  int min = 200;
  if (vertical_axis > max && horizontal_axis > min  && horizontal_axis < max) {   //FORWARD
    digitalWrite(LED_BUILTIN, HIGH);
    draw(arrow_up);
    message = CMD_FORWARD;
  }  

  if (vertical_axis < min && horizontal_axis > min  && horizontal_axis < max) {   //arrow_down
    digitalWrite(LED_BUILTIN, HIGH);
    draw(arrow_down);
    message = CMD_BACKWARDS;
  }

  if (horizontal_axis > max && vertical_axis > min  && vertical_axis < max) {   //right 
    digitalWrite(LED_BUILTIN, HIGH);
    draw(arrow_right);
    message = CMD_SLIDE_R;
  }

  if (horizontal_axis < min && vertical_axis > min  && vertical_axis < max) {   //left
    digitalWrite(LED_BUILTIN, HIGH);
    draw(arrow_left);
    message = CMD_SLIDE_L;
  }

  /*if(vert > max && horz > max){                  //not using this 
    draw(up_left);
  }*/

  if(vertical_axis > min  && vertical_axis < max && horizontal_axis > min  && horizontal_axis < max){   //IDLE
    draw(square);
    message = CMD_IDLE;
  }


  Mirf.setTADDR((byte *)"FGHIJ");           //Set the receiver address                    
  Mirf.send((byte *)&message);              //Send instructions, send random number message
  //Serial.print("Wait for sending.....");  //uncomment for debug
  while (Mirf.isSending()) delay(1);        //Until you send successfully, exit the loop
  //Serial.print("Send success:");          //uncomment for debug
  //Serial.println(message);                //uncomment for debug
  delay(1000);
}