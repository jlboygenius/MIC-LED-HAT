
/*
 Name:		MICdHat.ino
 Created:	7/26/2015 10:10:34 AM
 Author:	jeff
*/



//#include "LPD8806.h"
#include <SPI.h>
#include "FastLED.h"


#define NUM_LEDS 13
#define DATA_PIN 14
#define CENTER_LED 12
#define BUTTON_PIN A4

int analogPin =A6; // read from multiplexer using analog input 6
int strobePin = 2; // strobe is attached to digital pin 2
int resetPin = 12; // reset is attached to digital pin 12
int spectrumValue[7]; // to hold a2d values
int BUTTON_MODE = 0;

const int BRIGHTNESS = 128;
int SPINSPEED = 75; // number of times through the loop before moving turning it
const int CHASESPACE = 6; // number of spaces between dots in the chase around the loop
uint16_t LeftRainbowWheel = 0;
uint8_t lightPosR[NUM_LEDS];
uint8_t lightPosG[NUM_LEDS];
uint8_t lightPosB[NUM_LEDS];


CRGB LightPosLeft[NUM_LEDS];

int LeftWaitLoops = 0;



int rotaryR = 0;
int rotaryB = 0;
int rotaryG = 0;

int LChaseSpace = 5;
int RChaseSpace = 5;
int SpinSpace = 5;
int barLevel = 0;
int barUpDownStatus = 1;



CRGB leds[13];
void setup() {
	//pinMode(A0, OUTPUT);
	FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
	Serial.begin(9600);

	pinMode(BUTTON_PIN, INPUT_PULLUP);

}
void loop() {
	//leds[CENTER_LED] = CRGB::White; FastLED.show(); delay(30);
	//leds[CENTER_LED] = CRGB::Black; FastLED.show(); delay(30);
	//resetMic();

	 
	if (button_Pressed()){} // do something funky



	//leds[NUM_LEDS - 1] = CRGB::Black;
	FastLED.show();
	//readMic();

	FastLED.show();

	
	if (BUTTON_MODE == 0 ){
		// off,random color on step
		computeSpinSpeed();

		// chasing Random, random color on step

		if (LeftWaitLoops>0) LeftWaitLoops--;
		SpinSpace = SpinSpace - 3;
		
		if (SpinSpace <= 0){
			 	// we're not waiting on the step anymore, 
				// add a pixel to the chase, 
			 
					BarGraph(barLevel);
					if (barLevel >= NUM_LEDS / 2){ barUpDownStatus = -1; }
					if (barLevel == 0){ barUpDownStatus = 1; }
					barLevel += barUpDownStatus;
					 
			 
			SpinSpace = SPINSPEED;
		}
		else{
			SpinSpace--;

		}

	}
	else if (BUTTON_MODE ==1){
		// chasing white, random color on step
		
		computeSpinSpeed();
		 
		if (LeftWaitLoops>0) LeftWaitLoops--;
		


		if (SpinSpace <= 0){
			if (LeftWaitLoops == 0){
				// we're not waiting on the step anymore, 
				// add a pixel to the chase, 
				if (LChaseSpace == 0){
					AddToChaseL(CRGB::White);
					LChaseSpace = CHASESPACE;
				}
				else{
					AddToChaseL(CRGB::Black);
					LChaseSpace--;
				}
			}
			 
			SpinSpace = SPINSPEED;
		}
		else{
			SpinSpace--;

		}



	}
	else if (BUTTON_MODE==2){
		
		computeSpinSpeed();

		// chasing Random, random color on step
		 
		if (LeftWaitLoops>0) LeftWaitLoops--;
		


		if (SpinSpace <= 0){
			if (LeftWaitLoops == 0){
				// we're not waiting on the step anymore, 
				// add a pixel to the chase, 
				if (LChaseSpace == 0){
					AddToChaseL(RandomGoodColor());
					LChaseSpace = CHASESPACE;
				}
				else{
					AddToChaseL(CRGB::Black);
					LChaseSpace--;
				}

			}
			 
			SpinSpace = SPINSPEED;
		}
		else{
			SpinSpace--;

		}

	}
	else if (BUTTON_MODE==3){
		//fading rainbow, random color on step
		
		computeSpinSpeed();
		//SPINSPEED = SPINSPEED / 5;
		 
		if (LeftWaitLoops>0) LeftWaitLoops--;
		


		if (SpinSpace <= 0){
			if (LeftWaitLoops == 0){
				// we're not waiting on the step anymore, 
				// add a pixel to the chase, 
				AddToChaseL(Wheel(LeftRainbowWheel));
				LeftRainbowWheel++;
				if (LeftRainbowWheel == 384) LeftRainbowWheel = 0;
			}
			 
			SpinSpace = SPINSPEED;
		}
		else{
			SpinSpace--;
		}



	}
	else if (BUTTON_MODE==4){
		//chasing rainbow, random color on step
		
		computeSpinSpeed();
		//SPINSPEED = SPINSPEED / 5;
		 
		if (LeftWaitLoops>0) LeftWaitLoops--;
	 


		if (SpinSpace <= 0){
			if (LeftWaitLoops == 0){
				// we're not waiting on the step anymore, 
				// add a pixel to the chase, 
				AddToChaseL(Wheel(((LeftRainbowWheel * 384 / NUM_LEDS) + LeftRainbowWheel) % 384));
				LeftRainbowWheel++;
				if (LeftRainbowWheel == 384) LeftRainbowWheel = 0;
			}
			 
			SpinSpace = SPINSPEED;
		}
		else{
			SpinSpace--;
		}

	}
	else if (BUTTON_MODE==5){
		// headlights! white front, red back
		//		setRotaryColor(255, 200, 200);
		setstripsoff();
		headlights(leds); 
	}
	else if (BUTTON_MODE==6){
		// light up based on angle, white
		// read raw accel measurements from device
		 


	}
	else if (BUTTON_MODE==7){
		// light up based on angle, random color

		// read raw accel measurements from device
		 

	}
	else if (BUTTON_MODE==8){

		colorWipe3(leds, CRGB::Black, BRIGHTNESS);

	}


















}
void readMic(){
	for (int i = 0; i < 7; i++)
	{
		digitalWrite(strobePin, LOW);
		delayMicroseconds(30); // to allow the output to settle
		spectrumValue[i] = analogRead(analogPin);

		/*  THIS SECTION IS FOR TESTING - OUTPUT TO SERIAL*/
						if (spectrumValue[i] < 10)
						{
							Serial.print(" ");
							Serial.print(spectrumValue[i]);
						}
						else if (spectrumValue[i] < 100)
						{
							Serial.print(" ");
							Serial.print(spectrumValue[i]);
						}
						else
						{
							Serial.print(" ");
							Serial.print(spectrumValue[i]);
						}



						Serial.print(" ");

		digitalWrite(strobePin, HIGH);
	}
	Serial.println();
}

void resetMic(){
	digitalWrite(resetPin, HIGH);
	delayMicroseconds(5);
	digitalWrite(resetPin, LOW);

}

CRGB randomColor(){
	 
	return CreateColor(random(0, 255), random(0, 255), random(0, 255));
}

 


void setstripsoff(){
	for (int i = 0; i < NUM_LEDS; i++) {

		leds[i] = CRGB::Black;  // set our current dot to black before we continue
		
	}


}

void setStripSegment(bool foot, CRGB newColor, int start, int segend){

	for (int i = start; i <= segend; i++){
		
		leds[i] = newColor;
		 

	}

}
void BarGraph(int level){

	//Serial.println(level);

	for (int k = 0; k < NUM_LEDS-1; k++){
		leds[k] = CRGB::Black;
	}
	leds[NUM_LEDS - 1] = CRGB::White;
	for (int i = 0; i < level; i++){
		leds[i] = CRGB::White;
		leds[NUM_LEDS-i-2] = CRGB::White;
	} 

}

void AddToChaseL(CRGB newColor){
	for (int y = NUM_LEDS - 1; y>0; y--){
		LightPosLeft[y] = LightPosLeft[y - 1];
	}
	LightPosLeft[0] = newColor;
	for (int i = 0; i<NUM_LEDS; i++){
		leds[i] = LightPosLeft[i];
	}

}
 

void headlights(CRGB *strp){
	strp[0] = CRGB::White;
	strp[1] = CRGB::White;
	strp[11] = CRGB::White;
	strp[12] = CRGB::White;
	strp[10] = CRGB::White;


	
	strp[5] = CRGB::Green;
	strp[6] = CRGB::Green;
	return;
	
}

void AddToChase(CRGB *strp, int newR, int newG, int newB){

	CRGB holderR = newR;
	CRGB holderG = newG;
	CRGB holderB = newB;
	for (int y = NUM_LEDS - 1; y>0; y--){
		lightPosR[y] = lightPosR[y - 1];
		lightPosG[y] = lightPosG[y - 1];
		lightPosB[y] = lightPosB[y - 1];

	}
	lightPosR[0] = holderR;
	lightPosG[0] = holderG;
	lightPosB[0] = holderB;

	for (int i = 0; i<NUM_LEDS; i++){
		strp[i].setRGB(lightPosR[i], lightPosR[i], lightPosR[i]);
	}


}






void chase(CRGB *strp, int height, int brightness){


	for (int i = 0; i<height; i++){
		//       strp.setPixelColor(i, strp.Color(lightPosR[i],lightPosR[i],lightPosR[i]));
		strp[i].setRGB(brightness, brightness, brightness);
	}

	//  Serial.print(lightPos[height-1]);
	//  Serial.print(":");
	//  Serial.print(lightPos[0] ); 
	//  Serial.println();
	int holderR = lightPosR[height - 1];
	int holderG = lightPosG[height - 1];
	int holderB = lightPosB[height - 1];
	for (int y = height - 1; y>0; y--){
		lightPosR[y] = lightPosR[y - 1];
		lightPosG[y] = lightPosG[y - 1];
		lightPosB[y] = lightPosB[y - 1];

	}
	lightPosR[0] = holderR;
	lightPosG[0] = holderG;
	lightPosB[0] = holderB;
	//     for(int z=0;z<height;z++){
	//    
	//      Serial.print(lightPosR[z]);
	//      Serial.print(lightPosG[z]);
	//      Serial.print(",");
	//     }


}



void setDotSpacing(uint8_t lightsR[], uint8_t lightsG[], uint8_t lightsB[], int spacing){
	int spaces = NUM_LEDS / spacing;
	Serial.print(spaces);
	int y = spaces;
	for (int x = 0; x<NUM_LEDS; x++){
		if (y == spaces){
			lightsR[x] = BRIGHTNESS;
			lightsG[x] = BRIGHTNESS;
			lightsB[x] = BRIGHTNESS;

			y = 0;
		}
		else{
			y++;
		}
	}


}

void setStepColor(CRGB *strp, CRGB c){
	for (uint8_t i = 0; i < NUM_LEDS; i++) {

		strp[i] = c;

	}


}
CRGB CreateColor(uint8_t r, uint8_t g, uint8_t b){
	CRGB c;
	c.red = r;
	c.green = g;
	c.blue = b;

	return c;
}
// totally random colors are pretty bad, lets limit it
CRGB RandomGoodColor(){
	int c = random(1, 7);
	uint32_t r, g, b;

	switch (c){
	case 0: return CRGB::White;

	case 1:  return CRGB::Red;
	case 2: return CRGB::Green;
	case 3: return CRGB::Blue;

	case 4: return CRGB::Purple;
	case 5: return CRGB::Turquoise;
	case 6: return CRGB::MediumPurple;
	case 7: return CRGB::Yellow;

	case 8: return CRGB::Plum;
	case 9:  break;
	case 10:  break;


	}
	return CRGB::White;


}



void colorWipe3(CRGB *strp, CRGB c, uint8_t brightness) {
	//note: rgb order is really RBG
	int i;


	int dolight[38];

	//first set of lights are green
	for (i = 0; i < NUM_LEDS; i++) {
		strp[i] = c;


	}


}


// Slightly different, this one makes the rainbow wheel equally distributed 
// along the chain
void rainbowCycle(CRGB *stripT, uint8_t wait) {
	uint16_t i, j;
	// can we use HUE to do this in a simplier way?
	/*
	uint8_t k;
	for (k = 0; k < 255; k++){
	x.setHue(k);
	}
	*/

	for (j = 0; j < 384 * 5; j++) {     // 5 cycles of all 384 colors in the wheel
		for (i = 0; i < NUM_LEDS; i++) {
			// tricky math! we use each pixel as a fraction of the full 384-color wheel
			// (thats the i / strip.numPixels() part)
			// Then add in j which makes the colors go around per pixel
			// the % 384 is to make the wheel cycle around
			stripT[i] = Wheel(((i * 384 / NUM_LEDS) + j) % 384);

		}
		FastLED.show();

		delay(wait);
	}
}

void computeSpinSpeed(){
	SPINSPEED = SPINSPEED;
	 
	
}

 

 


/* Helper functions */

//Input a value 0 to 384 to get a color value.
//The colours are a transition r - g -b - back to r

CRGB Wheel(uint16_t WheelPos)
{
	byte r, g, b;
	switch (WheelPos / 128)
	{
	case 0:
		r = 127 - WheelPos % 128;   //Red down
		g = WheelPos % 128;      // Green up
		b = 0;                  //blue off
		break;
	case 1:
		g = 127 - WheelPos % 128;  //green down
		b = WheelPos % 128;      //blue up
		r = 0;                  //red off
		break;
	case 2:
		b = 127 - WheelPos % 128;  //blue down 
		r = WheelPos % 128;      //red up
		g = 0;                  //green off
		break;
	}
	CRGB c;
	c.red = r;
	c.blue = b;
	c.green = g;

	return c;
}

bool button_Pressed(){
	if (!digitalRead(BUTTON_PIN)){
		BUTTON_MODE += 1;
		if (BUTTON_MODE > 10){
			BUTTON_MODE = 0;
		}
		while (!digitalRead(BUTTON_PIN)){}
		Serial.println(BUTTON_MODE);
	}

}