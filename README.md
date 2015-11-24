# TP7

La class quadtree est constitué de quatre float representant les coordonnées x et y des points supérieurs gauche et inferieur bas de la section du quadtree. De plus, un quatree est composé de quatre autres quadtree (initialisé à null) représentant les quatree fils du quatree ainsi qu'un vecteur contenant les entités appartenant au quadtree.

Le constructeur du quadtree prend en paramètres les coordonnées, l'ensemble des entités appartenant au père (si il y en a un) et la profondeur (qui est égale à 0 pour le premier nœud et égale à la profondeur du père + 1 pour les autres nœuds). Dans ce constructeur, on vérifie que la profondeur du quadtree n'est pas égale à la profondeur (auquel cas, on ajoute aux entités du quadtree les entités du père appartenant au quadtree) sinon, on cherche les entités appartenant au quadtree et l'on divise la quadtree en quatre quadtree (que l'on stock dans les fils).

Pour savoir si une entité appartient au quadtree, on calcule si la hitbox (de type AABB) n'entre pas en colision avec le quadtree (grâce à la fonction instersect() de l'objet QRect qui prend paramètres de points (represantant les points supérieurs gauche et inferieur droit d'un rectangle dont on veut savoir s'il entre en collision avec le QRect appelant la fonction).

Dans la fonction intialize() de TriangleWindow, on créé le premier noeud du quadtree en l'initialisant avec les coordonnées du terrain (coin supérieur gauche et inférieur droit) ainsi que les entités du terrain (et une profondeur de 0). Le quadtree est construit sur l'axe des x et des y (en gardant un z stable).
