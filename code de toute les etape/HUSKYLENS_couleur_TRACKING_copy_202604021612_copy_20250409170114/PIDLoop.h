#include "HUSKYLENS.h"
#include "DFMobile.h"

#define BASE_SPEED 150   // Vitesse de base du robot
#define TURN_SPEED 100   // Ajustement de la vitesse pour tourner

DFMobile Robot(7, 6, 4, 5);  // Initialisation des moteurs
HUSKYLENS huskylens;

const int SCREEN_CENTER = 160; // Centre de l'écran Huskylens
const int TOLERANCE = 20; // Zone neutre où le robot ne corrige pas sa direction

void setup() {
    Serial.begin(115200);
    Robot.Direction(HIGH, LOW);  // Configuration de la direction initiale

    Wire.begin();
    while (!huskylens.begin(Wire)) {
        Serial.println(F("Échec de connexion à HUSKYLENS !"));
        delay(100);
    }

    huskylens.writeAlgorithm(ALGORITHM_COLOR_RECOGNITION); // Activation du suivi de couleur
}

void loop() {
    if (!huskylens.request()) {
        Serial.println(F("Erreur : échec de requête Huskylens."));
        Robot.Speed(0, 0); // Arrêter le robot
        return;
    }

    if (!huskylens.isLearned()) {
        Serial.println(F("Couleur non apprise !"));
        Robot.Speed(0, 0);
        return;
    }

    if (!huskylens.available()) {
        Serial.println(F("Couleur perdue ! Le robot s'arrête."));
        Robot.Speed(0, 0);
        return;
    }

    // Lire la position de la couleur détectée
    HUSKYLENSResult result = huskylens.read();
    int xPos = result.xCenter;
    int error = xPos - SCREEN_CENTER; // Erreur de position

    Serial.print("Couleur détectée à x = ");
    Serial.println(xPos);

    int leftSpeed = BASE_SPEED;
    int rightSpeed = BASE_SPEED;

    // Ajustement de la direction
    if (error > TOLERANCE) { 
        // Couleur à droite -> tourner à droite
        leftSpeed += TURN_SPEED;
        rightSpeed -= TURN_SPEED;
    } 
    else if (error < -TOLERANCE) { 
        // Couleur à gauche -> tourner à gauche
        leftSpeed -= TURN_SPEED;
        rightSpeed += TURN_SPEED;
    }

    // Limiter la vitesse pour éviter les dépassements
    leftSpeed = constrain(leftSpeed, 0, 255);
    rightSpeed = constrain(rightSpeed, 0, 255);

    // Appliquer la vitesse aux moteurs
    Robot.Speed(leftSpeed, rightSpeed);

    Serial.print("Vitesse Gauche : ");
    Serial.print(leftSpeed);
    Serial.print(" | Vitesse Droite : ");
    Serial.println(rightSpeed);
}