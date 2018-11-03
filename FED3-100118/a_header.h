/********************************************************
  Include these libraries
********************************************************/
#include <Wire.h>
#include <SPI.h>
#include <Stepper.h>
#include <RTCZero.h>
#include "RTClib.h"
#include <SdFat.h>
SdFat SD;             //Quick way to make SdFat work with standard SD.h sketches
#include <Adafruit_GFX.h>
#include <Adafruit_SharpMem.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Adafruit_NeoPixel.h>

/********************************************************
  Initialize variables for FED3 - you can alter these variables, but be careful with the others!
********************************************************/
//Timeout duration in seconds
const byte timeout = 0; //timeout between trials in seconds

//variables for Compulsivity test
const byte trialsToAversStim = 3;
const byte stimsBeforeSwitch = 5; // How many aversive stims in Active mode?

//variables for Prob Reversal Task
byte rewardProb=80; //probability of reward on active poke in prob reversal task 
byte consecCorrect = 0;

/********************************************************
  You should never need to change the below variables
********************************************************/
int FED;
int FR;
int previousFR = FR;
int previousFED = FED;
int retInterval = 0;
int pelletTime = 0;
int numMotorTurns = 0;
int numJamClears = 0;
int ratio = 1;  // for progressive ratio trials
static int LeftCount = 0;
static int RightCount = 0;
static int PelletCount = 0;
byte FEDmode;
byte previousFEDmode = FEDmode;
bool PelletAvailable = false;
bool PelletJam = false;
bool CountReady = false;
bool LeftReady = false;
bool RightReady = false;
bool TimeoutReady = true;
bool OutReady = false;
bool Ratio_Met = false;
bool SetFED = false;
int EndTime = 0;
bool compulsive = false;
byte trialCount = 0;
byte aversiveStim = 0;  // binary variable for logging when it's an aversive stimulation trial
bool activePoke = 1;  // 0 means right, 1 means left
float measuredvbat = 1.00;
bool rewardedTrial = true;

/********************************************************
  Function Prototypes
********************************************************/
void StartUpCommands();
bool CheckForPellet();
void sleep();
void wake();
void CheckPokes();
void Feed();
void Timeout();
void ClearJam();
void VibrateJam();
void ReadBatteryLevel();
void UpdateDisplay();
void colorWipe(uint32_t c, uint8_t wait);
void RcolorWipe(uint32_t c, uint8_t wait);
void Blink(byte PIN, byte DELAY_MS, byte loops);
void CheckReset();
void SelectMode();
void logdata();
void CreateFile();
void CreateDataFile ();
void WriteToSD();
void writeHeader();
void writeConfigFile();
void writeFEDmode();
void error(uint8_t errno);
void getFilename(char *filename);
bool CheckLeft ();
bool CheckRight ();
void ConditionedStimulus();
void RConditionedStim();
void ReleaseMotor ();
void SerialOut();
void SetDeviceNumber();
void DisplaySleep();
void DisplaySDError() ;
void DisplayJamClear();
void DisplaySDLogging();
void DisplayDispense();
void DisplayRetrievalInt();
void warningStimulus();
void aversiveStimulus();
void leftStimulus();
void rightStimulus();

/********************************************************
  Feather pins being used
********************************************************/
#define NEOPIXEL A1
#define MOTOR_ENABLE 13
#define GREEN_LED 8
#define PELLET_WELL 1
#define LEFT_POKE 6
#define RIGHT_POKE 5
#define BUZZER 0  //Set to 0 to activate beeper, or 7 to turn it off and stop annoying people on the train
#define VBATPIN A7
#define cardSelect 4
#define BNC_OUT A0

/********************************************************
  Initialize NEOPIXEL strip
********************************************************/
Adafruit_NeoPixel strip = Adafruit_NeoPixel(8, NEOPIXEL, NEO_GRBW + NEO_KHZ800);

/********************************************************
  Setup Sharp Memory Display
********************************************************/
#define SHARP_SCK  12
#define SHARP_MOSI 11
#define SHARP_SS   10
#define BLACK 0
#define WHITE 1
Adafruit_SharpMem display(SHARP_SCK, SHARP_MOSI, SHARP_SS, 144, 168);

/********************************************************
  Setup RTC objects
********************************************************/
RTCZero rtc2;
RTC_PCF8523 rtc;

/********************************************************
  Setup SD Card
********************************************************/
void dateTime(uint16_t* date, uint16_t* time) {
  DateTime now = rtc.now();
  // return date using FAT_DATE macro to format fields
  *date = FAT_DATE(now.year(), now.month(), now.day());

  // return time using FAT_TIME macro to format fields
  *time = FAT_TIME(now.hour(), now.minute(), now.second());
}

File logfile;         // Create file object
File ratiofile;       // Create another file object
File configfile;      // Create another file object
char filename[15];    // Array for file name data logged to named in setup

/********************************************************
  Setup stepper object
********************************************************/
// Create the motor shield object with the default I2C address
#define STEPS 200
Stepper stepper(STEPS, A2, A3, A4, A5);
