//=====[Libraries]=============================================================

#include "mbed.h"
#include "arm_book_lib.h"

//=====[Defines]===============================================================
#define BUZZER_ON 0
#define BUZZER_OFF 1

//=====[Declaration and initialization of public global objects]===============

DigitalIn driverSeatSensor(D7);
DigitalIn driverSeatbeltSensor(D6);
DigitalIn passengerSeatSensor(D5);
DigitalIn passengerSeatbeltSensor(D4);

DigitalIn ignition(BUTTON1);

DigitalOut greenLED(LED1);
DigitalOut blueLED(LED2);
DigitalOut buzzer(D8);

UnbufferedSerial uartUsb(USBTX, USBRX, 115200);

//=====[Declaration and initialization of public global variables]=============


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

        if(ignition){
            uartUsb.write ("Trying to start the engine\r\n", 28);
            if(greenLED){

                uartUsb.write("Engine started.\r\n", 17);
                greenLED = OFF;
                blueLED = ON;

            } else {
                problemCheck();
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

        uartUsb.write("Welcome to enhanced alarm system model 218-W25\r\n", 48);

        if(driverSeatbeltSensor && passengerSeatSensor && passengerSeatbeltSensor){
            greenLED = ON;
        } 
    }
}

void problemCheck(){
    buzzer = BUZZER_ON;

    string outputString = " ";

    if(!driverSeatSensor){
        outputString += "Driver seat not occupied.\r\n";
    } 
    if(!driverSeatbeltSensor){
        outputString += "Driver seatbelt not fastened.\r\n";
    }
    if(!passengerSeatSensor){
        outputString += "Passenger seat not occupied.\r\n";
    }
    if(!passengerSeatbeltSensor){
        outputString += "Passenger seatbelt not fastened.\r\n";
    }
    uartUsb.write("Ignition Inhibited\r\n", 20);
    uartUsb.write(outputString, outputString.length());
}
