
# Pablo Shell

## Introduction

Le but de ce projet est de créer un shell en C.
Pour cela nous allons utiliser des appels systèmes pour les fonctions de base du shell.

# Parsing
Pour le parsing ce sont les fonctions `fgets` et ``split_tokens`` qui sont utilisées afin de récuperer un tableau de chaînes de caractères qui correspondent aux commandes et arguments.

A partir de là nous pouvons les traiter comme nous le souhaitons.

# Gestion des redirections
## Cas des redirections multiples
## Cas des redirections d'entrées et de sorties 

# Pipes

# Implémentation d'un historique

Nous avons voulu implémenter la notion d'historique avec les fléches comme avec Bash seulement cela aurait impliqué de vérifier chaque touche entrée par l'utilisateur ce qui n'allait pas avec le code fourni nous avons donc implémenté des fonctions pour ajouter les commandes tapées dans un fichier (`cd`, `get` et `hist` sont volontairement non ajoutées à l'historique) que nous pourrons consulter en tapant `hist` dans le terminal pour afficher les 10 dernières commandes ou `hist x` pour les x dernières.

Nous pouvons alors exécuter ces commandes sans les retaper en faisant ``get`` suivi du numéro de ligne.

```c
> hist

Historique des 10 dernières commandes:
10: ls -l | wc | wc 
9: ls -l | wc 
8: ls > toto 
7: ls | grep o | grep u > o 
6: ls | grep o | grep u 
5: grep o
4: ls | grep o | grep u
3: ls 
2: ls | wc
1: ls 
> get 2
Execution de la commande : ls | wc

     13      13     120
```
# Implémentation de cd
Il à également fallu implémenter la fonction `cd` car celle ci est une fonction "built-in" d'un shell nous avons donc utilisé l'appel système `chdir` pour changer le répértoire courant.
```c
if(nb_tokens==2 && strcmp(tokens[0],"cd")==0){
			chdir(tokens[1]);continue;	
}
```

# Liste des bugs connus
- Les redirections utilisant à la fois >> et > dans la même commande ne fonctionnent pas
- Le chemin pour accéder à l'historique est fixe et ne fonctionne que sur notre machine (il faudrait utiliser `getenv` pour récupérer le chemin de l'utilisateur)