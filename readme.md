# Présentation de l'algorithme
L'algorithme qui a été choisit, est le second présenté dans l'article : [New Algorithms for Subset Sum Problem, by Zhengjun Cao and Lihua Liu](https://arxiv.org/pdf/1807.02611.pdf).
Il s'agit d'un algorithme probabilistique, c'est à dire, qu'il utilise de l'aléatoire.
## Principe
Le principe du code est explicité aux pages 5 et 6 de l'article. Il se compose en 4 étapes :
- Mélanger le set de départ et en récupérer une partie (un subset).
- Exécuter un algorithme glouton sur ce subset : C'est à dire que si le subset est [w1, w2, w3] et que l'on a pour "target" T, nous calculons : [T, T-w1, T-w2, T-w1-w2, T-w3, T-w1-w3, T-w2-w3, T-w1-w2-w3]
- Si le tableau obtenu contient un zéro, nous avons trouvé une solution au problème.
- Sinon, on recommence à l'étape 1.

## Limite
### Nombre d'itérations
Cet algorithme permet d'exécuter l'algorithme glouton (étape 2) sur des subset du set principale, et donc dans limiter la complexiter. Si l'on considère n la taille du set et k la taille du subset (avec k < n), la cmoplexiter devient 2<sup>k</sup> au lieu de 2<sup>n</sup>.

Cependant, il faut donner un nombre d'itération maximale pour éviter des boucles infinies en cas d'absence de solution. Ainsi, si jamais nous atteignons ce nombre maximale d'itération, le programme nous dit qu'il n'y a pas de solution, mais nous ne pouvons pas en être sûr.

### Bien choisir la taille du subset
Comme nous l'avons vue précédemment, le choix de la taille du subset est importante. Si on le choisit trop petit par rapport à la solution, on ne trouvera jamais une solution au problème, malgré son existence.

Si l'on souhaite choisir une taille de subset trop grande pour être sûr de trouver la solution, on perd tout l'intéret de l'algorithme. En effet, k tend vers n, et donc la complexité se rapproche de la complexité de l'algorithme glouton.

Le dernier point de blocage concerne l'utilisation mémoire de l'étape 3. En effet cette dernière nécessite de créer un tableau de taille 2<sup>k</sup>. Ainsi, si la solution contient 30 éléments, il nous faut un subset de taille 30 minimum, et donc créer un tableau de taille 2<sup>30</sup>=1073741824. Cela occupe donc beaucoup d'espace mémoire et prend également beaucoup de temps à être construit.


# Études de performances
## Première implémentation séquentielle

## Seconde implémentation séquentielle

## Implémentation parallèle