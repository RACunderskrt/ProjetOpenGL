## Pour compiler :
1 - build le projet avec cmake ou cmake-gui <br/>
2 - se rendre dans exosTP/projet <br/>
3 - lancer une de ces commandes. <br/>
		- make && ./projet dragon2_small.obj <br/>
		- make && ./projet bunny.obj <br/>
		- make && ./projet carre.obj <br/>

## Pour utiliser l'application :
L'application permet de visualiser différents shaders sur un objet 3D. <br/>
Celui qui est chargé de base est le shader "simple" sans illumination. <br/>
Vous pouvez retrouver à chaque disponible un shader NPR et un shader Phong. <br/>
Il est à noter qu'il peut y avoir un shader suplémentaire si le modèle chargé possède des coordonnées de texture, ici "carre.obj". Ce dernier crée une texture générée procéduralement en damier sur votre objet. <br/>
Il a des paramètres que l'on peut modifier afin de mieux comprendre le comportement de chaque shader, par exemple, la couleur du modèle ou de la lumière ou encore la position de cette dernière. <br/>
Dans le shader NPR, c'est la couleur de la bordure et l'activation ou la désactivation de cette dernière. <br/>
Dans le shader Phong, c'est la valeur de spéculaire et ambient que l'on peut modifier. <br/>
Le clique gauche de la souris permet de faire pivoter le modèle, le clique molette permet de zoomer et le clique droit permet de déplacer le modèle. <br/>
Vous pouvez presser Echap pour quitter le programme.
