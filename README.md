## Pour compiler :
1 - build le projet avec cmake ou cmake-gui
2 - se rendre dans exosTP/projet
3 - lancer une de ces commandes.
		-make && ./projet dragon2_small.obj
		-make && ./projet bunny.obj
		-make && ./projet carre.obj

## Pour utiliser l'application :
L'application permet de visualiser différents shaders sur un objet 3D.
Celui qui est chargé de base est le shader "simple" sans illumination.
Vous pouvez retrouver à chaque disponible un shader NPR et un shader Phong.
Il est à noter qu'il peut y avoir un shader suplémentaire si le modèle chargé possède des coordonnées de texture, ici "carre.obj". Ce dernier crée une texture générée procéduralement en damier sur votre objet.
Il a des paramètres que l'on peut modifier afin de mieux comprendre le comportement de chaque shader, par exemple, la couleur du modèle ou de la lumière ou encore la position de cette dernière.
Dans le shader NPR, c'est la couleur de la bordure et l'activation ou la désactivation de cette dernière.
Dans le shader Phong, c'est la valeur de spéculaire et ambient que l'on peut modifier.
Le clique gauche de la souris permet de faire pivoter le modèle, le clique molette permet de zoomer et le clique droit permet de déplacer le modèle.
Vous pouvez presser Echap pour quitter le programme.
