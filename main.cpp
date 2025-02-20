//=====[Libraries]=============================================================

#include "mbed.h"
#include "arm_book_lib.h"

//=====[Defines]===============================================================
#define BUZZER_ON 0
#define BUZZER_OFF 1

//=====[Declaration and initialization of public global objects]===============

DigitalIn driverSeatSensor(D7);
DigitalIn passengerSeatSensor(D6);
DigitalIn driverSeatbeltSensor(D5);
DigitalIn passengerSeatbeltSensor(D4);

DigitalIn ignition(BUTTON1);

DigitalOut greenLED(LED1);
DigitalOut blueLED(LED2);
DigitalOut buzzer(D8);

UnbufferedSerial uartUsb(USBTX, USBRX, 115200);

//=====[Declaration and initialization of public global variables]=============

bool driverIn = false;
bool engineOn = false;
bool problem = false;

//=====[Declarations (prototypes) of public functions]=========================

void inputs();
void outputs();
void ignitionReady();
void problemCheck();

//=====[Main function, the program entry point after power on or reset]========

int main()
{
    inputs();
    outputs();

    while (true){
        ignitionReady();

        if(ignition && !problem && !engineOn){
            if(greenLED){
                uartUsb.write("Engine started.\r\n", 17);
                greenLED = OFF;
                blueLED = ON;
                engineOn = true;

            } else {
                problemCheck();
                problem = true;
            }
        }
    }
}

//=====[Implementations of public functions]===================================

void inputs(){
    driverSeatSensor.mode(PullDown);
    driverSeatbeltSensor.mode(PullDown);
    passengerSeatSensor.mode(PullDown);
    passengerSeatbeltSensor.mode(PullDown);
}

void outputs(){

    greenLED = OFF;
    blueLED = OFF;
    buzzer = BUZZER_OFF;
}

void ignitionReady(){

    if (driverSeatSensor){

        if (!driverIn && !engineOn){
            uartUsb.write("Welcome to enhanced alarm system model 218-W25\r\n", 48);
            driverIn = true;
            }
    
        if(driverSeatbeltSensor && passengerSeatSensor && passengerSeatbeltSensor){
            greenLED = ON;
        } 
        else{
            greenLED = OFF;
        }
    } else {
        driverIn = false;
        greenLED = OFF;
    }
}

void problemCheck(){
    buzzer = BUZZER_ON;
    uartUsb.write("Ignition Inhibited\r\n", 20);
    

    if(!driverSeatSensor){
        uartUsb.write("Driver seat not occupied.\r\n", 27);
    } 
    if(!driverSeatbeltSensor){
        uartUsb.write("Driver seatbelt not fastened.\r\n", 31);
    }
    if(!passengerSeatSensor){
        uartUsb.write("Passenger seat not occupied.\r\n", 30);
    }
    if(!passengerSeatbeltSensor){
        uartUsb.write("Passenger seatbelt not fastened.\r\n", 34);
    }
}
