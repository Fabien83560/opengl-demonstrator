

# Démonstrateur pédagogique

## Compilation

Pour compiler le projet, suivez ces étapes :

1. Créez un répertoire `build` et se placer dedans:
```bash
mkdir build && cd build
```

2. Générez les fichiers de build avec CMake :
```bash
cmake ..
```

3. Compilez le projet :
```bash
make
```

L'exécutable sera créé à l'emplacement suivant et peut être exécuté avec : `./exosTP/projet/projet.exe`

## Fonctionnalités

L'interface graphique, située sur la droite de la fenêtre, offre plusieurs options de contrôle :

### Informations générales
- Affichage de la position de la caméra en temps réel (x, y, z)
- Contrôle de la couleur de l'arrière-plan
- Option pour afficher/masquer la grille

### Paramètres d'éclairage
L'onglet "Light" permet de configurer :
- Choix du mode d'éclairage :
  - None (aucun éclairage)
  - Phong
  - Blinn-Phong
  - Gaussian
- Position de la lumière (x, y, z)
- Couleur de la lumière
- Force ambiante (0.0 - 1.0)
- Force spéculaire (0.0 - 1.0)
- Brillance (1.0 - 256.0)
- Option pour afficher/masquer l'indicateur de lumière

### Paramètres de l'objet
L'onglet "Object" permet de contrôler :
- Sélection du modèle 3D à partir d'une liste de fichiers disponibles
- Option d'affichage en wireframe
- Couleur de l'objet
- Affichage des normales avec options supplémentaires :
  - Longueur des normales (0.01 - 1.0)
  - Couleur des normales

## Notes techniques
- L'interface est ancrée à droite de la fenêtre et ne peut pas être déplacée ou redimensionnée
- Le chargement des objets 3D se fait dynamiquement depuis le répertoire `exosTP/projet/resources/objects`
- Les changements de paramètres sont appliqués en temps réel
