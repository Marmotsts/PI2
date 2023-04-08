//Include Libraries
//Receiver program
#include <SPI.h>
#include "Mirf.h"
#include "nRF24L01.h"
#include "MirfHardwareSpiDriver.h"
#include "AFMotor.h"
#include <Servo.h> 

Nrf24l Mirf = Nrf24l(9, 8);  //CE, CSN pins

int message;

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

//define motors
AF_DCMotor FRONT_RIGHT(1);
AF_DCMotor BACK_RIGHT(2);
AF_DCMotor BACK_LEFT(3);
AF_DCMotor FRONT_LEFT(4);
Servo POULIE;
Servo FRACTAL;
int speed = 255;
void setup()
{
  //Serial.begin(9600);
  Mirf.spi = &MirfHardwareSpi;
  Mirf.init();

  Mirf.setRADDR((byte *)"FGHIJ"); //Set your own address (receiver address) using 5 characters
  Mirf.payload = sizeof(message);
  Mirf.channel = 90;             //Set the used channel
  Mirf.config();
  //Serial.println("Listening...");  //Start listening to received data

  POULIE.attach(9);     //ce sont les pins par défaut du shield, mais d'autres configurations sont possibles  
  FRACTAL.attach(10);

  //Turn on motors
  FRONT_RIGHT.setSpeed(255);
  FRONT_RIGHT.run(RELEASE);
  FRONT_LEFT.setSpeed(255);
  FRONT_LEFT.run(RELEASE);
  BACK_RIGHT.setSpeed(255);
  BACK_RIGHT.run(RELEASE);
  BACK_LEFT.setSpeed(255);
  BACK_LEFT.run(RELEASE);
}

void forward(){
  FRONT_RIGHT.setSpeed(speed);
  FRONT_RIGHT.run(FORWARD);
  FRONT_LEFT.setSpeed(speed);
  FRONT_LEFT.run(FORWARD);
  BACK_RIGHT.setSpeed(speed);
  BACK_RIGHT.run(FORWARD);
  BACK_LEFT.setSpeed(speed);
  BACK_LEFT.run(FORWARD);
  delay(200);
}

void backwards(){
  FRONT_RIGHT.setSpeed(speed);
  FRONT_RIGHT.run(BACKWARD);
  FRONT_LEFT.setSpeed(speed);
  FRONT_LEFT.run(BACKWARD);
  BACK_RIGHT.setSpeed(speed);
  BACK_RIGHT.run(BACKWARD);
  BACK_LEFT.setSpeed(speed);
  BACK_LEFT.run(BACKWARD);
  delay(200);

}

void RIGHT(){
  FRONT_RIGHT.setSpeed(speed);
  FRONT_RIGHT.run(BACKWARD);
  FRONT_LEFT.setSpeed(speed);
  FRONT_LEFT.run(FORWARD);
  BACK_RIGHT.setSpeed(speed);
  BACK_RIGHT.run(FORWARD);
  BACK_LEFT.setSpeed(speed);
  BACK_LEFT.run(BACKWARD);
  delay(200);
}

void LEFT(){
  FRONT_RIGHT.setSpeed(speed);
  FRONT_RIGHT.run(FORWARD);
  FRONT_LEFT.setSpeed(speed);
  FRONT_LEFT.run(BACKWARD);
  BACK_RIGHT.setSpeed(speed);
  BACK_RIGHT.run(BACKWARD);
  BACK_LEFT.setSpeed(speed);
  BACK_LEFT.run(FORWARD);
  delay(200);
}

void STOP(){
  FRONT_RIGHT.setSpeed(0);
  FRONT_RIGHT.run(RELEASE);
  FRONT_LEFT.setSpeed(0);
  FRONT_LEFT.run(RELEASE);
  BACK_RIGHT.setSpeed(0);
  BACK_RIGHT.run(RELEASE);
  BACK_LEFT.setSpeed(0);
  BACK_LEFT.run(RELEASE);
  delay(200);
}

void loop() {
  if (Mirf.dataReady()) { //When the program is received, the received data is output from the serial port
    Mirf.getData((byte *) &message);
    //Serial.print("Got data: "); //Uncomment for debug
    //Serial.println(value); //Uncomment for debug
  }
  if (message == CMD_FORWARD){
    forward();
  }
  if (message == CMD_BACKWARDS)
  {
    backwards();
  }
  if (message == CMD_SLIDE_L)
  {
    RIGHT();
  }
  if (message == CMD_SLIDE_L)
  {
    LEFT();
  }
}