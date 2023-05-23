# Projet de réseaux

- Lancement du serveur: `make controller` si situé à la racine ou alors `make` si situé dans le dossier controller
  - Saisir `load aquariums_files/aquarium<numéro>`. Il y a trois types d'aquariums enregistrés:
    - 0: 4 vues en forme de carré. L'aquarium fait 1000x1000
    - 1: 2 vues côte à côtes. L'aquarium fait 1000x500
    - 2: 3 vues : 2 côte à côte et la troisième au milieu et par dessus les deux autres. L'aquarium fait 1000x500
  - Ajout d'une vue avec `add view <nom de la vue> <abscisse coin en haut à gauche>x<ordonnée coin en haut à gauche>+<hauteur>+<longeur>`
  - Suppresion d'une vue avec `del view <nom de la vue>`
  - Enregistrement de l'aquarium avec `save <chemin vers fichier de sauvegarde>`
  - Afficher la configuration de l'aquarium avec `show aquarium`
- Lancement d'une vue: `make view` si situé à la racine ou alors `make` si situé dans le dossier view
  - Ajout de poisson: sur le terminal d'un des clients, saisir `addFish <nom du poisson> at <abscisse de la position>x<ordonnée de la position>, <longueur>x<largeur>, <mode de déplacement>`
    - Si le nom du poisson contient le mot "Rouge", "Bleu", "Jaune" alors l'image du poisson sera un poisson de la couleur correspondante. Par défaut, il s'agit d'un poisson arc-en-ciel
  - Lancement de l'animation: saisir `startFish <nom du poisson>`
  - Suppression d'un poisson: saisir `delFish <nom du poisson>`. Le poisson peut prendre un peu de temps avant de disparaitre des autres fenêtres.
- Terminer un des deux programmes (client ou serveur): saisir ctrl+D