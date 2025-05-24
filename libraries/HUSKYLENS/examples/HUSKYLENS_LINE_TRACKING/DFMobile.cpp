#include "HUSKYLENS.h"
#include "SoftwareSerial.h"
#include "PIDLoop.h"
#include "DFMobile.h"

#define ZUMO_FAST 150  // Vitesse de base du robot

DFMobile Robot(7, 6, 4, 5);  // Initialisation des moteurs
PIDLoop headingLoop(1000, 0, 0, false);  // PID ajusté pour suivi d'objet
HUSKYLENS huskylens;

int OBJECT_ID = 1; // ID de l'objet à suivre (doit être appris sur HuskyLens)

void setup() {
    Serial.begin(115200);
    Robot.Direction(HIGH, LOW);  // Sens de déplacement du robot

    Wire.begin();
    while (!huskylens.begin(Wire)) {
        Serial.println(F("Échec de connexion à HuskyLens !"));
        delay(100);
    }

    huskylens.writeAlgorithm(ALGORITHM_OBJECT_TRACKING); // Mode suivi d'objet
}

void loop() {
    int32_t error;
    int left = 0, right = 0;

    if (!huskylens.request()) {
        Serial.println(F("Échec de requête HuskyLens ! Vérifiez la connexion."));
        Robot.Speed(0, 0);  // Stopper le robot
        return;
    }

    int nombreObjets = huskylens.count();
    Serial.print("Objets détectés : ");
    Serial.println(nombreObjets);

    // Si aucun objet détecté, arrêter le robot
    if (nombreObjets == 0) {
        Serial.println(F("Aucun objet détecté. Arrêt."));
        Robot.Speed(0, 0);
        return;
    }

    // Rechercher l'objet spécifique par son ID
    HUSKYLENSResult result;
    bool found = false;
    
    for (int i = 0; i < nombreObjets; i++) {
        result = huskylens.read();
        if (result.ID == OBJECT_ID) {
            found = true;
            break;
        }
    }

    if (!found) {
        Serial.println(F("Objet cible non détecté. Arrêt."));
        Robot.Speed(0, 0);
        return;
    }

    printResult(result);

    // Calcul de l'erreur de position (différence entre la position de l'objet et le centre de la caméra)
    error = (int32_t)result.xCenter - 160;

    // Ajustement PID
    headingLoop.update(error);

    // Ajuster les roues pour suivre l'objet
    left = headingLoop.m_command + ZUMO_FAST;
    right = -headingLoop.m_command + ZUMO_FAST;

    // Empêcher des valeurs hors limites
    left = constrain(left, -255, 255);
    right = constrain(right, -255, 255);

    Robot.Speed(left, right);

    Serial.print("Gauche : ");
    Serial.print(left);
    Serial.print(" | Droite : ");
    Serial.println(right);
}

// Fonction d'affichage des données
void printResult(HUSKYLENSResult result) {
    Serial.print("Objet détecté - xCenter=");
    Serial.print(result.xCenter);
    Serial.print(", yCenter=");
    Serial.print(result.yCenter);
    Serial.print(", width=");
    Serial.print(result.width);
    Serial.print(", height=");
    Serial.print(result.height);
    Serial.print(", ID=");
    Serial.println(result.ID);
}
