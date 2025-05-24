#include "HUSKYLENS.h"
#include "Wire.h"
#include "DFMobile.h"

DFMobile Robot(7, 6, 4, 5);
HUSKYLENS huskylens;

#define MAX_SPEED 230      // Vitesse maximale
#define MIN_SPEED 170        // Vitesse minimale quand l'objet est proche
#define CENTER_X 160        // Centre de l'image Huskylens (x)
#define MARGE 20            // Zone de tolérance autour du centre
#define IMAGE_WIDTH 320     // Largeur image Huskylens
#define IMAGE_HEIGHT 240    // Hauteur image Huskylens
#define OBJECT_MAX_HEIGHT 150  // Hauteur de l'objet quand il est très proche
#define OBJECT_MIN_HEIGHT 30   // Hauteur quand l'objet est loin

void setup() {
    Serial.begin(115200);
    Wire.begin();
    Robot.Direction(HIGH, LOW);

    while (!huskylens.begin(Wire)) {
        Serial.println(F("Échec de connexion à HUSKYLENS !"));
        delay(100);
    }

    huskylens.writeAlgorithm(ALGORITHM_COLOR_RECOGNITION);
}

void loop() {
    if (!huskylens.request()) {
        Serial.println("Erreur de requête Huskylens !");
        Robot.Speed(0, 0);
        return;
    }

    int nbObjets = huskylens.count();

    if (nbObjets == 0) {
        Serial.println("Aucune couleur détectée.");
        Robot.Speed(0, 0);
    }
    else if (nbObjets > 1) {
        Serial.println("Plusieurs couleurs détectées !");
        Robot.Speed(0, 0);
    }
    else {
        HUSKYLENSResult result = huskylens.read();
        int x = result.xCenter;
        int hauteur = result.height;

        // 🧠 Calcul de la vitesse selon la hauteur de l'objet détecté
        int speed;
        if (hauteur >= OBJECT_MAX_HEIGHT) {
            speed = MIN_SPEED;
        }
        else if (hauteur <= OBJECT_MIN_HEIGHT) {
            speed = MAX_SPEED;
        }
        else {
            float ratio = (float)(hauteur - OBJECT_MIN_HEIGHT) / (OBJECT_MAX_HEIGHT - OBJECT_MIN_HEIGHT);
            speed = MAX_SPEED - (int)(ratio * (MAX_SPEED - MIN_SPEED));
        }

        // 📡 Affichage des infos utiles
        Serial.print("xCenter = ");
        Serial.print(x);
        Serial.print(" | Hauteur = ");
        Serial.print(hauteur);
        Serial.print(" | Vitesse calculée = ");
        Serial.println(speed);

        // 🎯 Calcul du décalage par rapport au centre
        int ecart = x - CENTER_X;

        // 📈 Ajustement de la rotation : plus l'objet est éloigné du centre, plus on corrige fort
        int correction = map(abs(ecart), 0, CENTER_X, 0, speed);  // max correction = speed
        correction = min(correction * 1.5, MAX_SPEED); 
        int leftSpeed, rightSpeed;

        if (abs(ecart) <= MARGE) {
            // Centré → avancer tout droit
            leftSpeed = speed;
            rightSpeed = speed;
            Serial.println("Avance tout droit");
        }
        else if (ecart < 0) {
            // Objet à gauche → tourner à gauche
            leftSpeed = speed - correction;
            rightSpeed = speed + correction;
            Serial.println("Tourne à gauche");
        }
        else {
            // Objet à droite → tourner à droite
            leftSpeed = speed + correction;
            rightSpeed = speed - correction;
            Serial.println("Tourne à droite");
        }

        // Appliquer les vitesses aux moteurs
        Robot.Speed(leftSpeed, rightSpeed);
    }

    delay(100);
}
