# 🤖 Projet Robot Mobile – Cherokey Intelligent
Bienvenue sur le dépôt GitHub de notre projet de robot mobile intelligent basé sur le châssis Cherokey. Ce robot a été conçu pour fonctionner en trois modes distincts :

🚗 Sortie autonome de labyrinthe

📡 Contrôle à distance via Bluetooth

🎥 Suivi de ligne assisté par caméra

🎯 Objectifs du projet
Ce projet vise à développer un robot capable de :

naviguer de façon autonome dans un environnement complexe grâce à un capteur à ultrasons monté sur un servomoteur,

être contrôlé à distance via une application mobile (GoBLE) grâce au Bluetooth intégré,

effectuer un suivi de ligne visuel à l’aide d’une caméra intelligente (ex. : HuskyLens).

🔧 Matériel utilisé
🧠 Carte Romeo BLE (ATmega compatible Arduino + Bluetooth intégré)

⚙️ Châssis Cherokey 4WD

🔋 Batterie externe Li-ion (6V–12V)

📏 Capteur à ultrasons + Servomoteur SG90

📷 Caméra HuskyLens (ou autre caméra de suivi de ligne)

📱 Application GoBLE pour Android/iOS

🛠️ Fonctionnalités principales
1. Sortie de labyrinthe
Détection des obstacles via capteur à ultrasons rotatif

Évitement intelligent d’obstacles

Prise de décision en temps réel par traitement des distances

2. Mode Bluetooth (manuel)
Contrôle depuis smartphone avec joystick analogique

Gestion dynamique de la vitesse et direction

Communication via Bluetooth intégré à la carte Romeo

3. Suivi de ligne par caméra
Suivi visuel automatique d’une ligne tracée au sol

Ajustement de la direction et vitesse selon la position détectée par la caméra

4. suivie de couleur.
