#include "HUSKYLENS.h"
#include "SoftwareSerial.h"
#include "PIDLoop.h"
#include "DFMobile.h"

#define ZUMO_FAST 200

DFMobile Robot(7, 6, 4, 5);  // Initialisation des moteurs
PIDLoop headingLoop(2000, 0, 0, false);
HUSKYLENS huskylens;

int ID1 = 1;
void printResult(HUSKYLENSResult result);

void setup() {
    Serial.begin(115200);
    Robot.Direction(HIGH, LOW);  // Sens de déplacement par défaut

    Wire.begin();
    while (!huskylens.begin(Wire)) {
        Serial.println(F("Échec de connexion à HUSKYLENS !"));
        delay(100);
    }
    huskylens.writeAlgorithm(ALGORITHM_LINE_TRACKING); // Activation du suivi de ligne
}

void loop() {
    int32_t error;
    int left = 0, right = 0;

    // Demander les données à Huskylens
    if (!huskylens.request()) {
        Serial.println(F("Échec de requête Huskylens ! Vérifiez la connexion."));
        Robot.Speed(0, 0);  // Arrêt du robot
        return;
    }

    int nombreDeLignes = huskylens.count(); // Nombre d'éléments détectés
    Serial.print("Lignes détectées : ");
    Serial.println(nombreDeLignes);

    // Si aucune ligne détectée, le robot s'arrête
    if (nombreDeLignes == 0) {
        Serial.println(F("Aucune ligne détectée. Arrêt."));
        Robot.Speed(0, 0);
        return;
    }
    // Si plusieurs lignes détectées, le robot s'arrête
    else if (nombreDeLignes > 1) {
        Serial.println(F("Plusieurs lignes détectées. Arrêt."));
        Robot.Speed(0, 0);
        return;
    }

    // Lire les données de la ligne détectée
    HUSKYLENSResult result = huskylens.read();
    printResult(result);

    // Calcul de l'erreur de position (écart par rapport au centre)
    error = (int32_t)result.xTarget - 160;

    // Exécution de l'algorithme PID pour ajuster la direction
    headingLoop.update(error);

    // Déterminer les vitesses des roues en fonction de l'erreur
    left = headingLoop.m_command + ZUMO_FAST;
    right = -headingLoop.m_command + ZUMO_FAST;

    // Appliquer les vitesses calculées au robot
    Robot.Speed(left, right);

    Serial.print("Gauche : ");
    Serial.print(left);
    Serial.print(" | Droite : ");
    Serial.println(right);
}

// Fonction d'affichage des résultats
void printResult(HUSKYLENSResult result) {
    if (result.command == COMMAND_RETURN_BLOCK) {
        Serial.print("Bloc détecté - xCenter=");
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
    else if (result.command == COMMAND_RETURN_ARROW) {
        Serial.print("Flèche détectée - xTarget=");
        Serial.print(result.xTarget);
        Serial.print(", yTarget=");
        Serial.print(result.yTarget);
        Serial.print(", ID=");
        Serial.println(result.ID);
    }
    else {
        Serial.println("Objet inconnu !");
    }
}
