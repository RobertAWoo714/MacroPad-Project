#include <Arduino.h>
#line 1 "C:\\Users\\capta\\Downloads\\lol\\lol.ino"
// BopbbyArduino macropad binded macros
// Created by Robert Woo

//includes Keypad.h
// This library maps out the rows and column used to make the keyboard
#include "Keypad.h"

//includes HID-Project.h
// Library for all of the keypress functions
#include "HID-Project.h"

#define ENCODER_DO_NOT_USE_INTERRUPTS
#include "Encoder.h"

//MODE variable
//VERY IMPORTANT
int Mode = 0;

//keypad setup
const byte rows = 3;  //four rows
const byte cols = 5;  //three columns
char keys[rows][cols] = {
  { '1', '2', '3', '4', '5' },
  { '6', '7', '8', '9', '0' },
  { 'A', 'B', 'C', 'D', 'E' }
};

byte rowPins[rows] = { 18, 19, 21 };          //connect to the row pinouts of the keypad
byte colPins[cols] = { 9, 8, 7, 6, 5 };  //connect to the column pinouts of the keypad
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, rows, cols);

//===========================================================================================================
#define ENCODER_DO_NOT_USE_INTERRUPTS
#include "Encoder.h"

//Rotary Encoder A knob setup
Encoder KnobA(15, 14);
long oldPosA  = 0;

//Rotary Encoder B knob setup
Encoder KnobB(16, 10);
long oldPosB  = 0;

//LCD Screen set up
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

//===========================================================================================================
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display
int screenscroll = 0;
long oldtime = 0;

//===========================================================================================================
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN        A2 // On Trinket or Gemma, suggest changing this to 1

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 12 // Popular NeoPixel ring size

// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

//Fade global variables
int redfade = 1;
int bluefade = 1;
int greenfade = 1;
int redval = 0;
int blueval = 0;
int greenval = 0;

//Rainbow global variables
//Idk why but i couldn't get the fade values working with an array
int RainbowCol[] = {
    50, 40, 30, 20,
    0, 10, 20, 30,
    0, 0, 0, 0};

int RainbowFade[] = {1, 1, 1, 1,
                     0,-1,-1, -1,
                     0, 0, 0, 0};

//RGB variable values setup
int RBGspeed = 40;
int RGBBright = 50;

//===========================================================================================================
//list of chrome tabs for case 6
String CommBookmark[] = 
{
  "https://canvas.eee.uci.edu//",
  "https://mail.google.com/mail/u/?authuser=rawoo1@uci.edu",
  "https://mail.google.com/mail/u/?authuser=robertwoo714@gmail.com",
  "https://3.basecamp.com/4258652/buckets/13806051/chats/2083133956",
  "https://app.slack.com/client/T05GDL6AYGK/C05G749M3EJ"
};

//list of chrome tabs for case 7
String calcbookmark[] = 
{
  "https://www.desmos.com/calculator",
  "https://www.symbolab.com/solver/algebra-calculator"
};


//===========================================================================================================
// Tbh idk why this works but i think this part is defining the function before it is called in void loop
void EncoderA();
void EncoderB();
void RGB();
// void LCD();

void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(9600);

  // Keyboard command set up
  Keyboard.begin();

  // INITIALIZE NeoPixel strip object (REQUIRED)
  pixels.begin();
  
  // // initialize the lcd 
  // lcd.init();
  // lcd.backlight();
}

//===========================================================================================================
void loop() 
{
  // put your main code here, to run repeatedly:
   char key = keypad.getKey();
            EncoderA();
            EncoderB();
            RGB();
            // LCD();

  if (Mode == 0) 
  {
    switch (key) 
    {
      case '1': //Changes mode from 0 to 1

        Mode = 1;
        Serial.print("Now in mode 1");
        Serial.print("\n");
        break;
      
      case '2': //Highlights and copies next word 
      
        Keyboard.press(KEY_LEFT_CTRL);
        Keyboard.press(KEY_LEFT_SHIFT);
        Keyboard.press(KEY_RIGHT_ARROW);
        Keyboard.releaseAll();

        delay(100);

        Keyboard.press(KEY_LEFT_CTRL);
        Keyboard.press('c');
        Keyboard.releaseAll();
        break;

      case '3': //Highlights and copies next line PLEASE FIX 

        Keyboard.press(KEY_RIGHT_SHIFT);
        Keyboard.write(KEY_DOWN_ARROW);
        Keyboard.releaseAll();
        break;

      case '4': //Highlights and copies next paragraph

        Keyboard.press(KEY_LEFT_CTRL);
        Keyboard.press(KEY_LEFT_SHIFT);
        Keyboard.press(KEY_DOWN_ARROW);
        Keyboard.releaseAll();

        delay(100);

        Keyboard.press(KEY_LEFT_CTRL);
        Keyboard.press('c');
        Keyboard.releaseAll();
        break;

      case '5': //Paste

        Keyboard.press(KEY_LEFT_CTRL);
        Keyboard.press('v');
        Keyboard.releaseAll();
        break;
        
      case '6': //Opens communication chrome tabs

        Serial.print("six");

        //opens Window run
        Keyboard.press(KEY_LEFT_GUI);
        Keyboard.press('r');
        Keyboard.releaseAll();

        delay(100);

        //opens chrome
        Keyboard.println("chrome.exe");

        delay(500);

        for (int tabnumber = 0; tabnumber < 5; tabnumber++) 
        {
          Keyboard.println(CommBookmark[tabnumber]);
          delay(100);
          Keyboard.press(KEY_LEFT_CTRL);
          Keyboard.press('t');
          Keyboard.releaseAll();
          delay(100);
        }
        break;

      case '7': //Opens calculator chrome tabs

        //opens Window run
        Keyboard.press(KEY_LEFT_GUI);
        Keyboard.press('r');
        Keyboard.releaseAll();

        delay(100);

        //opens chrome
        Keyboard.println("chrome.exe");

        delay(500);

        for (int tabnumber = 0; tabnumber < 2; tabnumber++) {
          Keyboard.println(calcbookmark[tabnumber]);
          delay(100);
          Keyboard.press(KEY_LEFT_CTRL);
          Keyboard.press('t');
          Keyboard.releaseAll();
          delay(100);
        }

        //Go to tab 1
        Keyboard.press(KEY_LEFT_CTRL);
        Keyboard.press('1');
        Keyboard.releaseAll();
        break;

      case '8': //Opens Notepad++

        //opens Window run
        Keyboard.press(KEY_LEFT_GUI);
        Keyboard.press('r');
        Keyboard.releaseAll();

        delay(100);

        //opens chrome
        Keyboard.println("notepad++.exe");
        delay(500);

        //Go to tab 1
        Keyboard.press(KEY_LEFT_CTRL);
        Keyboard.press('n');
        Keyboard.releaseAll();
        break;

      case '9': //Opens snipping tool

        //opens Window run
        Keyboard.press(KEY_LEFT_GUI);
        Keyboard.press('r');
        Keyboard.releaseAll();

        delay(100);

        //opens chrome
        Keyboard.println("snippingtool");
        delay(700);

        //Go to tab 1
        Keyboard.press(KEY_LEFT_CTRL);
        Keyboard.press('n');
        Keyboard.releaseAll();
        break;

      case '0': //Volume mutes

        Consumer.write(MEDIA_VOLUME_MUTE);
        break;

      case 'A': //Opens Discord and Youtube music

        //opens Window run
        Keyboard.press(KEY_LEFT_GUI);
        Keyboard.press('r');
        Keyboard.releaseAll();

        delay(100);

        //opens Discord
        Keyboard.println("C:\\Users\\capta\\AppData\\Local\\Discord\\Update.exe --processStart Discord.exe");
        delay(100);


        //opens Window run
        Keyboard.press(KEY_LEFT_GUI);
        Keyboard.press('r');
        Keyboard.releaseAll();

        delay(100);

        //opens Youtube music
        Keyboard.println("C:\\Users\\capta\\AppData\\Local\\Programs\\youtube-music-desktop-app\\YouTube Music Desktop App.exe");
        delay(100);
        break;

      case 'B': //Goes back on media song played

        Consumer.write(MEDIA_PREV);
        break;

      case 'C': //Pauses and Plays

        Consumer.write(MEDIA_PLAY_PAUSE);
        break;

      case 'D': //Forward media song played

        Consumer.write(MEDIA_NEXT);
        break;

      case 'E': //Locks out computer
        Keyboard.press(KEY_LEFT_CTRL);
        Keyboard.press(KEY_LEFT_ALT);
        Keyboard.press(KEY_DELETE);
        Keyboard.releaseAll();

        delay(500);

        Keyboard.press(KEY_ENTER);
        Keyboard.releaseAll();

        break;
    }
    // this releases the buttons
    //more of a saftety thing than anything else
    Keyboard.releaseAll(); 

  }

  else if (Mode == 1) //just the function keys
  {
    switch (key) {

      case '1': //Changes mode from 0 to 1

        Mode = 0;
        Serial.print("Now in mode 1");
        Serial.print("\n");
        break;
      
      case '2': //F1
      
        Keyboard.write(KEY_F1);
        break;

      case '3': //F2

        Keyboard.write(KEY_F2);
        break;

      case '4': //F3

        Keyboard.write(KEY_F3);
        break;

      case '5': //F4

        Keyboard.write(KEY_F4);
        break;
        
      case '6': //F5

        Keyboard.write(KEY_F5);
        break;

      case '7': //F6

        Keyboard.write(KEY_F6);
        break;

      case '8': //F7

        Keyboard.write(KEY_F7);
        break;

      case '9': //F8

        Keyboard.write(KEY_F8);
        break;

      case '0': //Vol mute

        Consumer.write(MEDIA_VOLUME_MUTE);
        break;

      case 'A': //F9

        Keyboard.write(KEY_F9);
        break;

      case 'B': //F10

        Keyboard.write(KEY_F10);
        break;

      case 'C': //F11

        Keyboard.write(KEY_F11);
        break;

      case 'D': //F12

        Keyboard.write(KEY_F12);
        break;

      case 'E': //Locks out computer

        Keyboard.press(KEY_LEFT_CTRL);
        Keyboard.press(KEY_LEFT_ALT);
        Keyboard.press(KEY_DELETE);
        Keyboard.releaseAll();

        delay(500);

        Keyboard.press(KEY_ENTER);
        Keyboard.releaseAll();

        break;
    }
    Keyboard.releaseAll();  // this releases the buttons
  }
}

void EncoderA()//function for lower rotary encoder
//yea, i know its backwards >:(
{
  long newPosA = KnobA.read()/4;
  if (newPosA != oldPosA && newPosA > oldPosA) 
  {
    oldPosA = newPosA;

    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.write('z');
    Keyboard.releaseAll();

  }

  else if (newPosA != oldPosA && newPosA < oldPosA) 
  {
    oldPosA = newPosA;

    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.write('y');
    Keyboard.releaseAll();

  }
}

void EncoderB() //function for lower rotary encoder
//yea, i know its backwards >:(
{
  long newPosB = KnobB.read()/4;
  if (newPosB != oldPosB && newPosB > oldPosB) 
  {

    oldPosB = newPosB;
    Consumer.write(MEDIA_VOLUME_DOWN);

  }

  else if (newPosB != oldPosB && newPosB < oldPosB) 
  {

    oldPosB = newPosB;
    Consumer.write(MEDIA_VOLUME_UP);

  }
}

void Rainbow(int col)
{
  if (RainbowCol[col] >= RGBBright)
  {
    RainbowFade[col] = -1;
    RainbowFade[col + 4] = 1;
    RainbowFade[col + 8] = 0;
  }
  else if (RainbowCol[col + 4] >= RGBBright)
  {
    RainbowFade[col] = 0;
    RainbowFade[col + 4] = -1;
    RainbowFade[col + 8] = 1;

  }
  else if (RainbowCol[col + 8] >= RGBBright)
  {
    RainbowFade[col] = 1;
    RainbowFade[col + 4] = 0;
    RainbowFade[col + 8] = -1;
  }

  RainbowCol[col] = RainbowCol[col] + RainbowFade[col];
  RainbowCol[col + 4] = RainbowCol[col + 4] + RainbowFade[col + 4];
  RainbowCol[col + 8] = RainbowCol[col + 8] + RainbowFade[col + 8];

  pixels.setPixelColor(col    , pixels.Color(RainbowCol[col], RainbowCol[col + 4], RainbowCol[col + 8]));
  pixels.setPixelColor(col + 4, pixels.Color(RainbowCol[col], RainbowCol[col + 4], RainbowCol[col + 8]));
  pixels.setPixelColor(col + 8, pixels.Color(RainbowCol[col], RainbowCol[col + 4], RainbowCol[col + 8]));

  pixels.show();
}

void FadeWhite()
{
  redval = redval + redfade;
  blueval = blueval + bluefade;
  greenval = greenval + greenfade;

  for (int i = 0; i < NUMPIXELS; i++) 
    pixels.setPixelColor(i, pixels.Color(redval, blueval, greenval));
    pixels.show();

  if (greenval <= 0 || greenval >= RGBBright)
  {
    redfade = -redfade;
    bluefade = -bluefade;
    greenfade = -greenfade;
  }
}

void RGB()
{
  unsigned long newtime = millis();
  if ((newtime - oldtime) >= RBGspeed)
  {
    oldtime = newtime;
    if (Mode == 0)
    {
        FadeWhite();
    }

    if (Mode == 1)
    {
      oldtime = newtime;
      Rainbow(0); // Col 1
      Rainbow(1); // Col 2
      Rainbow(2); // Col 3
      Rainbow(3); // Col 4
    }
  }
}

// ==========================================================================================================
// void LCD() //if you want to add an LCD screen off of amazon using I2C protocol
// {
//   unsigned long newtime = millis();
//   if ((newtime - oldtime) >= 4000)
//   {
//     if (Mode == 0)
//     {
//       oldtime = newtime;
//       if (screenscroll == 0)
//       {
//         lcd.clear();
        
//         lcd.setCursor(0,0);
//         lcd.print("1. Copies next word");

//         lcd.setCursor(0,1);
//         lcd.print("2. Copies next line");

//         lcd.setCursor(0,2);
//         lcd.print("3. Copies next para");

//         lcd.setCursor(0,3);
//         lcd.print("4. Pastes");

//         screenscroll = 1;
//       }

//       else if (screenscroll == 1)
//       {
//         lcd.clear();
        
//         lcd.setCursor(0,0);
//         lcd.print("5. Work tabs");

//         lcd.setCursor(0,1);
//         lcd.print("6. Calc tabs");

//         lcd.setCursor(0,2);
//         lcd.print("7. Notepad++");

//         lcd.setCursor(0,3);
//         lcd.print("8. Snipping tool");

//         screenscroll = 2;
//       }

//       else if (screenscroll == 2)
//       {
//         lcd.clear();
        
//         lcd.setCursor(0,0);
//         lcd.print("9.  Discord/Music");

//         lcd.setCursor(0,1);
//         lcd.print("10. Media prev");

//         lcd.setCursor(0,2);
//         lcd.print("11. Pause/Play");

//         lcd.setCursor(0,3);
//         lcd.print("12. Media forw");

//         screenscroll = 0;
//       }
//     }
//     if (Mode == 1)
//     {
//       lcd.clear();
//     }
//   }
// }


// case '6':
//   Keyboard.println("Alpha key6"); break;
// case '7':
//   Keyboard.println("Alpha key7"); break;
// case '8':
//   Keyboard.println("Alpha key8"); break;
// case '9':
//   Keyboard.println("Alpha key9"); break;
// case '0':
//   Keyboard.println("Alpha key10"); break;
// case 'A':
//   Keyboard.println("Alpha key11"); break;
// case 'B':
//   Keyboard.println("Alpha key12"); break;
// case 'C':
//   Keyboard.println("Alpha key13"); break;
// case 'D':
//   Keyboard.println("Alpha key14"); break;
// case 'E':
//   Keyboard.println("Alpha key15"); break;


// Ideas:
// custom signoff for emails that are copied
// mode 2 can do brightness knob
// change between speakers and headphones
