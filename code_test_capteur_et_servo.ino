#include <Servo.h>       // Inclusion de la bibliothèque pour contrôler les servomoteurs
#include <Metro.h>       // Inclusion de la bibliothèque Metro pour gérer des temporisations simples

// Création d’un objet Metro pour mesurer la distance toutes les 50 millisecondes
Metro measureDistance = Metro(50);

// Création d’un autre objet Metro pour contrôler le balayage du servomoteur toutes les 20 ms
Metro sweepServo = Metro(20);

// Variable pour stocker la distance mesurée par le capteur à ultrasons
unsigned long actualDistance = 0;

// Création de l'objet servo pour le contrôle du capteur à ultrasons
Servo myservo;

// Position actuelle du servo (en degrés)
int pos = 60;

// Drapeau pour déterminer le sens du balayage (aller ou retour)
int sweepFlag = 1;

// Broches utilisées par le capteur à ultrasons
int URPWM = 3;  // Broche de lecture PWM : la durée de l'impulsion représente la distance (1cm = 50µs)
int URTRIG = 10; // Broche de déclenchement du capteur (commande de mesure)

// Tableau contenant la commande envoyée au capteur pour activer le mode PWM
uint8_t EnPwmCmd[4] = {0x44, 0x02, 0xbb, 0x01};

void setup() {
  myservo.attach(9);         // Le servomoteur est connecté à la broche 9
  Serial.begin(9600);        // Démarrage de la communication série à 9600 bauds
  SensorSetup();             // Initialisation du capteur à ultrasons
}

void loop() {
  // Vérifie si 50 ms se sont écoulées : si oui, on mesure la distance
  if (measureDistance.check() == 1) {
      actualDistance = MeasureDistance();  // Mesure de la distance
      Serial.print("Distance : ");         // Affiche un message avant la valeur
      Serial.print(actualDistance);        // Affiche la distance mesurée
      Serial.println(" cm");               // Affiche l’unité et passe à la ligne suivante
  }

  // Vérifie si 20 ms se sont écoulées : si oui, on bouge le servo pour le balayage
  if (sweepServo.check() == 1) {
      servoSweep();   // Appel de la fonction de balayage du servo
  }
}

// Fonction qui configure les broches du capteur à ultrasons
void SensorSetup() {
  pinMode(URTRIG, OUTPUT);        // URTRIG en sortie (commande de mesure)
  digitalWrite(URTRIG, HIGH);     // Mise à l'état haut par défaut
  pinMode(URPWM, INPUT);          // URPWM en entrée (pour lire la durée de l'impulsion)

  // Envoi de la commande série pour activer le mode PWM du capteur
  for (int i = 0; i < 4; i++) {
      Serial.write(EnPwmCmd[i]);
  }
}

// Fonction qui mesure la distance en microsecondes puis la convertit en cm
int MeasureDistance() {
  // Déclenche une mesure : une impulsion basse, puis haute
  digitalWrite(URTRIG, LOW);
  digitalWrite(URTRIG, HIGH);

  // Lecture de la durée de l'impulsion (LOW) sur URPWM
  unsigned long distance = pulseIn(URPWM, LOW);

  // Si la durée est trop longue, on considère que la mesure est invalide
  if (distance == 50000) {
    Serial.print("Invalid");
  } else {
    distance = distance / 50;   // Conversion : chaque 50 µs correspond à 1 cm
  }

  return distance;  // Renvoie la distance calculée en cm
}

// Fonction pour balayer le servo entre 60° et 120°
void servoSweep() {
  if (sweepFlag) {
    // Balayage dans le sens croissant (60° à 120°)
    if (pos >= 60 && pos <= 120) {
        pos = pos + 1;           // Incrémentation de 1°
        myservo.write(pos);      // Envoie la nouvelle position au servo
    }
    if (pos > 119) sweepFlag = false;  // Inversion du sens une fois 120° atteint
  } else {
    // Balayage dans le sens décroissant (120° à 60°)
    if (pos >= 60 && pos <= 120) {
        pos = pos - 1;           // Décrémentation de 1°
        myservo.write(pos);      // Mise à jour de la position du servo
    }
    if (pos < 61) sweepFlag = true;   // Inversion du sens une fois 60° atteint
  }
}
