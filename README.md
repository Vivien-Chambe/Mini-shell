
# Pablo Shell
Auteurs : Chambe Vivien et Rozen Adjamé

## Introduction

Le but de ce projet est de créer un shell en C.
Pour cela nous allons utiliser des appels systèmes pour les fonctions de base du shell.

# Parsing
Pour le parsing ce sont les fonctions `fgets` et ``split_tokens`` qui sont utilisées afin de récuperer un tableau de chaînes de caractères qui correspondent aux commandes et arguments.

A partir de là nous pouvons les traiter comme nous le souhaitons.

# Gestion des redirections

Pour la gestion des redirections nous avons utilisé les appels systèmes `dup2` et `open` pour rediriger les entrées et sorties. Si le fichier n'existe pas, il est créé avec le flag `O_CREAT` ainsi que les droits `S_IRUSR | S_IWUSR` qui correspondent à `rw-------`. On utilise également `O_TRUNC` pour vider le fichier s'il existe déjà.

## Cas des redirections multiples
 Si on a plusieurs redirections simples dans la même commande, on va appeler `trouve_redirection` en boucle jusqu'à ne plus en trouver pour avoir le dernier fichier de la chaîne puis utiliser `dup2` pour rediriger l'entrée ou la sortie vers ce fichier.

## Cas de la concaténation `>>`
Si on a une redirection de concaténation, on va utiliser la fonction `open` avec le flag `O_APPEND` pour ajouter à la fin du fichier.

## Cas des redirections d'entrées et de sorties 
Si on a une redirection d'entrée et une redirection de sortie dans la même commande, on va traiter la redirection d'entrée en premier puis la redirection de sortie en utilisant la fonction `dup2` pour rediriger l'entrée ou la sortie vers le fichier correspondant.
# Pipes
Pour les pipes nous pouvons en faire plusieurs à la suite qui vont transmettre la sortie de la commande précédente à l'entrée de la commande suivante et afficher le résultat de la dernière commande dans le terminal.

Bien entendu, les pipes et les redirections peuvent être utilisés en même temps.
par exemple : `ls | grep o | grep u > titi` va afficher dans le fichier `toto` les fichiers du répertoire courant qui contiennent la lettre `o` et la lettre `u`.

```
> ls | grep o | grep u > titi
```

# Implémentation d'un historique

Nous avons voulu implémenter la notion d'historique avec les fléches comme avec Bash seulement cela aurait impliqué de vérifier chaque touche entrée par l'utilisateur ce qui n'allait pas avec le code fourni nous avons donc implémenté des fonctions pour ajouter les commandes tapées dans un fichier (`cd`, `get` et `hist` sont volontairement non ajoutées à l'historique) que nous pourrons consulter en tapant `hist` dans le terminal pour afficher les 10 dernières commandes ou `hist x` pour les x dernières.

La fonction `add_history` ajoute la commande en haut du fichier `.history` et la fonction `get_history` permet de récupérer les x dernières commandes.

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
Il a également fallu implémenter la fonction `cd` car celle ci est une fonction "built-in" d'un shell nous avons donc utilisé l'appel système `chdir` pour changer le répértoire courant. Cette fonction prends le chemin du répertoire en argument et change celui courant.
```c
if(nb_tokens==2 && strcmp(tokens[0],"cd")==0){
			chdir(tokens[1]);continue;	
}
```

# Liste des bugs connus
- Les redirections utilisant à la fois >> et > dans la même commande ne fonctionnent pas
- Le chemin pour accéder à l'historique est fixe et ne fonctionne que dans le dossier où se trouve l'exécutable (il faudrait utiliser `getenv` et `find` pour récupérer le chemin de l'utilisateur mais le processus de recherche serait bien trop long)
- L'utilisation de & sur certaines commandes peut entrainer des problemes d'affichage et d'execution comme avec : top & ou encore nano &

# Conclusion

Ce projet nous a permis de nous familiariser avec les appels systèmes et de mieux comprendre le fonctionnement d'un shell. Malheureusement nous n'avons pas correctement pu implémenter les processus en arrière-plan ainsi la redirection d'entrée `<<`.