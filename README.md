# TP7 : Gestion de scène
### Vincent BAZIA

 ## Octree
 Création d'une classe Octree qui va se charger de gérer l'octree de notre terrain\
Le fonctionnement d'un Octree est simple ([octree.h](https://github.com/xviniette/tp7/blob/bazia/octree.h) / [octree.cpp](https://github.com/xviniette/tp7/blob/bazia/octree.cpp)).\
On va tout d'abord instancier un Octree qui va englober la scène. Une fois fait, il va lancer la "division" qui consiste à :
- Si des objets sont contenus dans cet octree alors
    -  il stocke les objets contenus
    - Si on est pas "trop profond"
        - On crée 8 fils égaux (dimensions) qui vont répéter récursivement depuis le début
 - Sinon on s'arrête.
 
Pour tester si l'objet est contenu, c'est un simple teste AABB sur 3 axes.
 
De plus une méthode display va nous permettre d'afficher cet octree et tous ses fils.

On obtient ce résultat.
![alt tag](https://raw.githubusercontent.com/xviniette/tp7/bazia/demo.png)

## Un peu de physique... avec une sphère

Pour ça, j'ai créé deux classes :
- Sphère ([sphere.h](https://github.com/xviniette/tp7/blob/bazia/sphere.h)  [sphere.cpp](https://github.com/xviniette/tp7/blob/bazia/sphere.cpp))
- AABB ([aabb.h](https://github.com/xviniette/tp7/blob/bazia/aabb.h) [aabb.cpp](https://github.com/xviniette/tp7/blob/bazia/aabb.cpp))

Une sphère est défini par son point d'origine (x,y,z) et un rayon.\
Une box AABB est défini par point d'origine (x,y,z) ainsi qu'une largeur (w), hauteur (h) et une profondeur (d).

Collision entre deux sphères :
Si distance entre les deux points d'origines est inférieur à la somme de leur rayons, alors il y a collisions.

Collision entre une sphère et une box AABB. Elle se fait en 3 étapes :
- Si le point d'origine du cercle est à l'intérieur du AABB => true (Compris dans la box)
- Si Le point d'origine est à une distance inférieur à son rayon pour chaques face de la box AABB => true (Collision par une face)
- Si le point d'origine de la sphère est à une distance inférieur à son rayon de n'importe quel des 8 points de la box => true (Collision par coin)
- Sinon => false

![alt tag](https://raw.githubusercontent.com/xviniette/tp7/bazia/ok.png)\
Résultat de collisions entre Sphère/Sphère et Sphère/Box.

Pour AABB, j'ai aussi une méthode qui gère la présence d'un point dans la box, ainsi qu'une méthode qui permet de savoir si il y a collision avec une autre box AABB (utilisé dans la contenance d'un objet de l'octree).

Je n'ai pas réussi à implémenter les collision K-DOP.

Pour ce qui est des autres exercices, ayant déjà eu beaucoup de mal à finir les derniers tps, je n'ai pas réussi à rajouter ces fonctionnalités.

Bonne journée !

