/*********************************************************************
 * DFMobile GoBLE_APP - Gestion progressive des vitesses
 *********************************************************************/

#include "DFMobile.h"
DFMobile Robot(7,6,4,5);
#include "GoBLE.h"

// Variables pour stocker les valeurs du joystick
int joystickX, joystickY;
int buttonState[7];

// Vitesse des roues
int LeftWheelSpeed = 0;
int RightWheelSpeed = 0;
const int vitesseMax = 255;  // Vitesse maximale du robot

// LED de test
int ledPin = 13;

void setup() {
  Robot.Direction(LOW, LOW); // Initialisation direction moteurs
  pinMode(ledPin, OUTPUT);
  Goble.begin();
  Serial.begin(115200);
}

void loop() {
  if (Goble.available()) {
    // Lecture du joystick
    joystickX = Goble.readJoystickX();  
    joystickY = Goble.readJoystickY();

    // Lecture des boutons
    buttonState[SWITCH_UP]     = Goble.readSwitchUp();
    buttonState[SWITCH_LEFT]   = Goble.readSwitchLeft();
    buttonState[SWITCH_RIGHT]  = Goble.readSwitchRight();
    buttonState[SWITCH_DOWN]   = Goble.readSwitchDown();
    buttonState[SWITCH_SELECT] = Goble.readSwitchSelect();
    buttonState[SWITCH_START]  = Goble.readSwitchStart();

    // Transformation des valeurs du joystick en plage (-255 à 255)
    int SpeedX = map(joystickX, 1, 255, -vitesseMax, vitesseMax);
    int SpeedY = map(joystickY, 1, 255, -vitesseMax, vitesseMax);

    Serial.print("SpeedX: "); Serial.print(SpeedX);
    Serial.print("  SpeedY: "); Serial.println(SpeedY);

    // Gestion progressive de la vitesse pour toutes les directions
    if (abs(SpeedX) > 20) { // Mouvement avant/arrière progressif
      LeftWheelSpeed = map(abs(SpeedX), 20, 255, 0, vitesseMax) * (SpeedX > 0 ? 1 : -1);
      RightWheelSpeed = LeftWheelSpeed;
    } 
    else if (abs(SpeedY) > 20) { // Rotation progressive
      LeftWheelSpeed = map(abs(SpeedY), 20, 255, 0, vitesseMax) * (SpeedY > 0 ? 1 : -1);
      RightWheelSpeed = -LeftWheelSpeed;
    }
    else { // Si le joystick est au centre, le robot s'arrête
      LeftWheelSpeed = 0;
      RightWheelSpeed = 0;
    }

    // Application des vitesses au robot
    Robot.Speed(LeftWheelSpeed, RightWheelSpeed);

    // Gestion de la LED avec un bouton
    if (buttonState[1] == PRESSED) {
        digitalWrite(ledPin, HIGH);  
    }
    if (buttonState[1] == RELEASED) {
        digitalWrite(ledPin, LOW);
    }
  }
}
