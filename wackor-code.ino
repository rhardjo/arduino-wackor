/*

Technology in Creative Context
Created on 7th June 2015
Code by Rano Hardjosemito
Studentnumber: 2061502
Project Group: 42CM08i

I provided the comments in english.

*/


/* ---------- LIBRARIES ---------- */
#include <Servo.h>


/* ---------- CONSTANTS ---------- */
// Maxsonar
const int sonarPin = 5;
const int sonarInterval = 200; //Sonar delay speed

// Light Dependent Resistor
const int ldrPin = A0;
const int ldrInterval = 5;

// Servo
const int servoPin = 6;
const int servoMinDegrees = 20; // Limits the servo movement
const int servoMaxDegrees = 42;

// RGB Led
const int redPin = 11;
const int greenPin = 10;
const int bluePin = 9;

// Piezo
const int speakerPin = 4;


/* ---------- VARIABLES ---------- */
// Servo
Servo servoBell;
int servoPosition = 30; // The initial angle of the servo - starting at 30.
int servoScared = 10; // Millisecs between servo moves, controls speed
int servoCurious = 35;
int servoCalm = 100;
int servoInterval = servoCalm; // Initial millisecs between servo moves
int servoDegrees = 2; // Servo each step

// Light Dependent Resistor
int lightValue;
int hue;

// RGB
int redIntensity;
int greenIntensity;
int blueIntensity;

// Piezo
int pitch;

// Sonar
long pulse, inches, cm;

// Delay timers
unsigned long currentMillis = 0; // Stores millis value in each loop iteration
unsigned long previousServoMillis = 0; // The time when the servo was last moved
unsigned long previousSonarMillis = 0; // Last time sonar checked
unsigned long previousLdrMillis = 0;


/* ---------- SETUP ---------- */
void setup() {
  
  Serial.begin(9600);
  
  pinMode(sonarPin, INPUT);
  pinMode(ldrPin, INPUT);
  
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  
  servoBell.attach(servoPin);
  servoBell.write(servoPosition); // Sets the initial position
  
}


/* ---------- LOOP ---------- */
void loop() {
  
  currentMillis = millis(); // Capture the latest value of millis()
  
  //Call the functions
  distanceBehaviour();
  ringBell();
  checkLight();
  piezocato();
  
}


/* ---------- SONAR ---------- */
void distanceBehaviour() {

  if (currentMillis - previousSonarMillis >= sonarInterval) { // Typical millis delay, checks whether enough time(interval) has passed for a new move
    previousSonarMillis += sonarInterval; 
   
    pulse = pulseIn(sonarPin, HIGH); // Raw data into pulse variable
    inches = pulse/147; // Calculate raw data to inches
    cm = inches * 2.54; // Calculate inches to centimeters  
    
    //  Serial.print(cm); //Prints for debugging
    //  Serial.println("cm");
  }

}


/* ---------- PIEZO ---------- */
void piezocato() {
  pitch = map(cm, 12, 100, 1976, 31); // Map the sonar distance to the tone frequencies
  
  if (cm < 100) { // When the object is within 100 cm
    tone(speakerPin, pitch); // Create a pseudo-theremin pitch
  } else {
    noTone(speakerPin); // Shut up
  }
}


/* ---------- SERVO ---------- */
void ringBell() {
  
  if (currentMillis - previousServoMillis >= servoInterval) {
    previousServoMillis += servoInterval; // Time for another move   
    servoPosition = servoPosition + servoDegrees;

    if ((servoPosition >= servoMaxDegrees) || (servoPosition <= servoMinDegrees))  {
        // If the servo is at either extreme, move it the other way
      servoDegrees = - servoDegrees; // Reverse direction
        // Update the position to ensure it is within range
      servoPosition = servoPosition + servoDegrees; 
    }
    
    if (cm > 12 && cm < 30) { // When the object is between the minimal distance and 30cm
      servoInterval = servoScared; // Set servo speed
    } else if (cm > 30 && cm < 100) { // Between 30cm and 100cm
      servoInterval = servoCurious;
    } else { // When the distance is more than 100cm
      servoInterval = servoCalm;  
    }
    
        // Make the servo move to the next position
    servoBell.write(servoPosition);
        // And record the time when the move happened
  }

}


/* ---------- LDR ---------- */
void checkLight() {

  if (currentMillis - previousLdrMillis >= ldrInterval) {
    lightValue = analogRead(ldrPin); // Insert read into variable
    hue = map(lightValue, 0, 800, 0, 767); // Remap 0-1024 to 0-767 (float) 
    tasteTheRainbow(hue); // Call the color function
  }
  
}


/* ---------- RGB LED ---------- */
void tasteTheRainbow(int color_1) {
  
  if (color_1 <= 255) { // Zone 1
    redIntensity = 255 - color_1; // Red goes from on to off
    greenIntensity = color_1; // Green goes from off to on
    blueIntensity = 0; // Blue is always off
  } else if (color_1 <= 511) { // Zone 1
    redIntensity = 0; // Red is always off
    greenIntensity = 255 - (color_1 - 256); // Green on to off
    blueIntensity = (color_1 - 256); // Blue off to on
  } else { // color_1 >= 512       // Zone 3
    redIntensity = (color_1 - 512); // Red off to on
    greenIntensity = 0; // Green is always off
    blueIntensity = 255 - (color_1 - 512);  // Blue on to off
  }

  // When the hue values have been set, set LEDs to those values
  analogWrite(redPin, redIntensity);
  analogWrite(bluePin, blueIntensity);
  analogWrite(greenPin, greenIntensity);
  
}






