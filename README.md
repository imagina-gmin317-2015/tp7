- Les controles :
 - controles de base pour tout ce qui est zoom/dezoom/rotation
 - rotation de la camera à la souris, il faut cliquer sur une fenêtre pour capturer le curseur
 - echap permet de libérer le curseur
 - tab quitte toutes les fenêtres
 - F5 pour le chargement
 - F6 pour la sauvegarde

Il n'y a plus qu'une seule fenêtre pour ce TP au lieu des quatre, une fenêtre supplémentaire permettant de changer les saisons est néamoins présente. Attention, elle peut se trouver trop à droite ou derrière la fenêtre principale si l'écran est trop petit.

![alt tag](./result/result1.png)
On peut voir que le nombre de triangles est très élevé

J'ai donc simplifé le maillage en utilisant un quadtree, le quadtree sert comme algo de décimation. Je ne garde que les points, sans garder les carrés, puis je les triangule (avec une triangulation de delaunay) pour les relier.

![alt tag](./result seuil_1.png)
![alt tag](./result_1.png)
Arrêt du quadtree lorsque la variance dans un quad est inferieure ou égale à 1

![alt tag](./result_seuil_2.png)
![alt tag](./result_2.png)
Arrêt du quadtree lorsque la variance dans un quad est inferieure ou égale à 2

![alt tag](./result_seuil_5.png)
![alt tag](./result_5.png)
Arrêt du quadtree lorsque la variance dans un quad est inferieure ou égale à 5

![alt tag](./result_seuil_10.png)
![alt tag](./result_10.png)
Arrêt du quadtree lorsque la variance dans un quad est inferieure ou égale à 10

![alt tag](./result_seuil_20.png)
![alt tag](./result_20.png)
Arrêt du quadtree lorsque la variance dans un quad est inferieure ou égale à 20

On peut voir que plus le seuil de variance est élevé, mois il y aura de triangles, mais moins il y aura de détals.

J'ai également mis en place un octree adaptatif pour la collision des oiseaux (2eme saison (un clic sur le bouton de changement de saisons)). Il est mis à jour lors du déplacement des oiseaux. Le cube s'affiche en rouge lorsqu'il y a une collision
