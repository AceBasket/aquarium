# Partie serveur
## Lancer le serveur

Pour lancer la partie serveur:
- Si on se trouve à la racine:
> $ make controller
- Sinon si on est situé dans le dossier controller:
> $ make


## Commandes du contrôleur

- Pour initialiser l'aquarium, on saisi la commande:
> load aquariums_files/aquarium\<numéro>

Attention, il n'est plus possible de changer d'aquarium au cours de l'exécution !

Il y a trois types d'aquariums enregistrés:

1. aquarium0: 4 vues en forme de carré => l'aquarium fait 1000x1000
2. aquarium1: 2 vues côtes à côtes => l'aquarium fait 1000x500
3. aquarium2: 3 vues : 2 côte à côte et la troisième au milieu et par dessus les deux autres => l'aquarium fait 1000x500

- Pour ajouter une vue, on utilise la commande:
> add view \<nom de la vue> \<abscisse coin en haut à gauche>x\<ordonnée coin en haut à gauche>+\<hauteur>+\<longeur>

- Pour supprimer une vue, on utilise:
> del view \<nom de la vue>

- Pour enregistrer l'aquarium, on utilise:
> save \<chemin vers fichier de sauvegarde>

- Ensuite, pour afficher la configuration de l'aquarium:
> show aquarium

- Enfin, pour fermer le programme, il suffit de faire CTRL+D dans le terminal du contrôleur.

# Partie client

## Lancer un client / une vue

Pour démarre un client, il faut utiliser:
- Si on se trouve à la racine:
> $ make view
- Sinon si on est situé dans le dossier view:
> $ make


## Fonctionnalités du client

- Le client peut ajouter des poissons à l'aide de la commande:
> addFish \<nom du poisson> at \<abscisse de la position>x\<ordonnée de la position>, \<longueur>x\<largeur>, \<mode de déplacement>

saisie sur le terminal du client

Si le nom du poisson contient le mot "Rouge", "Bleu", "Jaune" alors l'image du poisson sera un poisson de la couleur correspondante. Par défaut, il s'agit d'un poisson arc-en-ciel

- Pour lancer l'animation du poisson, ilfaut saisir:
> startFish \<nom du poisson>

- Pour supprimer un poisson, il faut saisir:
> delFish \<nom du poisson>

Le poisson peut prendre un peu de temps avant de disparaitre des autres fenêtres.

- Enfin, pour fermer un client, il suffit, comme pour le serveur, de faire CTRL+D dans le terminal du client correspondant.