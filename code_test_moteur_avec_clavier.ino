/*
 # Edited by:  Matt
 # Date:       2015.09.06
 # Version:    1.1
 # Product:    Cherokey 4WD Mobile Platform
 # SKU:        ROB0102/ROB0117

 # Description:
 # Drive 2 motors with this Cherokey 4WD Mobile Platform
 # Connect D4,D5,D6,D7,GND to UNO digital 4,5,6,7,GND
*/

// Déclaration des broches utilisées pour contrôler les moteurs
int E1 = 5;     // Contrôle de la vitesse du moteur 1 (via PWM sur D5)
int E2 = 6;     // Contrôle de la vitesse du moteur 2 (via PWM sur D6)
int M1 = 4;     // Contrôle de la direction du moteur 1 (avant/arrière)
int M2 = 7;     // Contrôle de la direction du moteur 2

// Fonction pour arrêter complètement les moteurs
void stop(void)
{
  digitalWrite(E1, 0);       // Coupe la vitesse du moteur 1
  digitalWrite(M1, LOW);     // Direction neutre
  digitalWrite(E2, 0);       // Coupe la vitesse du moteur 2
  digitalWrite(M2, LOW);     // Direction neutre
}

// Fonction pour faire avancer le robot
void advance(char a, char b)
{
  analogWrite(E1, a);        // Applique une vitesse au moteur 1
  digitalWrite(M1, HIGH);    // Moteur 1 en marche avant
  analogWrite(E2, b);        // Applique une vitesse au moteur 2
  digitalWrite(M2, HIGH);    // Moteur 2 en marche avant
}

// Fonction pour reculer le robot
void back_off(char a, char b)
{
  analogWrite(E1, a);        // Vitesse pour moteur 1
  digitalWrite(M1, LOW);     // Moteur 1 en marche arrière
  analogWrite(E2, b);        // Vitesse pour moteur 2
  digitalWrite(M2, LOW);     // Moteur 2 en marche arrière
}

// Fonction pour tourner à gauche
void turn_L(char a, char b)
{
  analogWrite(E1, a);        // Vitesse moteur 1
  digitalWrite(M1, LOW);     // Moteur 1 en marche arrière
  analogWrite(E2, b);        // Vitesse moteur 2
  digitalWrite(M2, HIGH);    // Moteur 2 en marche avant
}

// Fonction pour tourner à droite
void turn_R(char a, char b)
{
  analogWrite(E1, a);        // Vitesse moteur 1
  digitalWrite(M1, HIGH);    // Moteur 1 en marche avant
  analogWrite(E2, b);        // Vitesse moteur 2
  digitalWrite(M2, LOW);     // Moteur 2 en marche arrière
}

// Fonction d'initialisation : configuration des broches et affichage d'instructions
void setup(void) {
  int i;
  for (i = 4; i <= 7; i++)    // Définit les broches 4 à 7 comme sorties
    pinMode(i, OUTPUT);

  Serial.begin(9600);         // Initialise la communication série à 9600 bauds

  // Affiche les commandes disponibles dans le moniteur série
  Serial.println("hello. w = forward, d = advance , a = back, s = droite, x = stop, z = hello world");

  digitalWrite(E1, LOW);      // Coupe la vitesse des moteurs au démarrage
  digitalWrite(E2, LOW);
}

// Boucle principale : attend les commandes de l'utilisateur
void loop(void) {
  if (Serial.available()) {       // Vérifie si une commande est reçue via USB
    char val = Serial.read();     // Lit le caractère envoyé

    if (val != -1)                // Vérifie que le caractère est valide
    {
      switch (val)               // Exécute l'action correspondante à la commande
      {
        case 'w':                // Avancer
          Serial.println("tourne gauche");
          advance(255, 255);     // Vitesse maximale pour avancer
          delay(1000);           // Avance pendant 1 seconde
          stop();                // Puis arrêt
          break;

        case 's':                // Reculer
          Serial.println("tourne droite");
          back_off(255, 255);    // Recul à pleine vitesse
          delay(1000);
          stop();
          break;

        case 'a':                // Tourner à gauche
          Serial.println("recule");
          turn_L(255, 255);      // Rotation vers la gauche
          delay(1000);
          stop();
          break;

        case 'd':                // Tourner à droite
          Serial.println("avance");
          turn_R(255, 255);      // Rotation vers la droite
          delay(1000);
          stop();
          break;

        case 'z':                // Message test
          Serial.println("hello world!");
          break;

        case 'x':                // Arrêt immédiat
          Serial.println("stop");
          stop();
          break;
      }
    }
    else stop();  // Si aucun caractère valide n'est reçu, arrêt de sécurité
  }
}
