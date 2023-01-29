#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h> 

#include "annexes.h"
#include "shell-utils.h"

#define INPUT_BUFFER_SIZE 2048
#define MAX_NB_TOKENS 512

int main() {
	if(fork()==0){execl("/bin/clear","clear",NULL);} // Pour lancer uniquement notre shell
	else{wait(NULL);}
	init(); // Initialisation shell
	/* Une variable pour sotcker les caractères tapés au clavier */
	char line[INPUT_BUFFER_SIZE+1];

	/* Une variable qui pointera vers les données lues par fgets
	 Sauf problème (erreur, plus de données, ...), elle pointera en
	 fait vers la variable précédente (line) */
	char* data;

	/* Un tableau où les mots de l'utilisateur seront stockés
	 indépendamment les uns des autres
	 Note: un mot est une suite de lettres séparées par une ou plusieurs
	 espaces des autres mots.  */
	char* tokens[MAX_NB_TOKENS+1];
	/* variables entières pour compter le nombre de token */
	int nb_tokens;

	while(1){
		color_vert(1);
		printf("PabloShell:");
		color_reset();

		// TODO ajouter gestion des fleches (curseur, historique, ...)
		// Mais pour ça il faudrais lire les touches tapées au clavier une par une
		// et non pas tout le buffer d'un coup comme c'est fait ici

		/* Récupération des données tapées au clavier */
		data=fgets(line, INPUT_BUFFER_SIZE, stdin);

		if (data==NULL) {
			/* Erreur ou fin de fichier : on quitte tout de suite */
			if (errno) {
				/* Une erreur: on affiche le message correspondant
				  et on quitte en indiquant une erreur */
				perror("fgets");
				exit(1);
			} else {
				/* Sinon ça doit être une fin de fichier.
				 On l'indique et on quitte normalement */
				fprintf(stderr, "EOF: exiting\n");
				exit(0);
			}
		}
		/* On vérifie que l'utilisateur n'a pas donné une ligne trop longue */
		if (strlen(data) == INPUT_BUFFER_SIZE-1) {
			fprintf(stderr, "Input line too long: exiting\n");
			exit(1);
		}

		/* On découpe la ligne en tokens (mots)
		 Voir documentation dans shell-utils.h (avec des exemples) */
		nb_tokens=split_tokens(tokens, data, MAX_NB_TOKENS);

		if(nb_tokens==1 && strcmp(tokens[0],"hist")==0 ){
			if(fork()==0){print_history(10);} 
			else{wait(NULL);}
		}
		if(nb_tokens==2 && strcmp(tokens[0],"hist")==0 ){
			if(fork()==0){print_history(atoi(tokens[1]));} 
			else{wait(NULL);}
		}
		/* S'il y a trop de tokens, on abandonne */
		if (nb_tokens==MAX_NB_TOKENS) {
			fprintf(stderr, "Too many tokens: exiting\n");
			exit(1);
		}
		/* S'il n'y a pas de token, c'est que l'utilisateur n'a pas donné de
		 commande. Il n'y a rien à faire. On arrête tout. */
		if (nb_tokens<=0) {
			fprintf(stderr, "No tokens found: exiting\n");
			continue; // On continue la boucle while(1) pour pouvoir relancer une commande et pas quitter le shell
		}
		/* On ajoute la commande à l'historique */
		// Etant donné que l'on a fait des vérifications sur le nombre de tokens, on peut ajouter la commande à l'historique
		add_history(tokens,nb_tokens);

		if(strcmp(tokens[0],"exit")==0){exit(0);}
		if(fork()==0){
			int fd;
			char *file_out;
			char *file_in;

			file_in = trouve_redirection(tokens,"<");
			if(file_in != NULL){
				char *file_in2 = trouve_redirection(tokens,"<");
				while(file_in2 != NULL){
					file_in = file_in2;
					file_in2 = trouve_redirection(tokens,"<");
				}
				fd = open(file_in, O_RDONLY, 0644); //O_TRUNC pour que le fichier soit vidé s'il existe déjà
				if(fd == -1){
					custom_err("open");
					custom_err(file_in);
					exit(1);
				}
				dup2(fd, 0);close(fd);
				execvp(tokens[0], tokens);
			}


			file_out = trouve_redirection(tokens,">");
			if(file_out != NULL){
				char *file_out2 = trouve_redirection(tokens,">");
				while(file_out2 != NULL){
					file_out = file_out2;
					file_out2 = trouve_redirection(tokens,">");
				}
				fd = open(file_out, O_WRONLY | O_CREAT | O_TRUNC, 0644); //O_TRUNC pour que le fichier soit vidé s'il existe déjà
				if(fd == -1){
					custom_err("open");
					custom_err(file_out);
					exit(1);
				}
				dup2(fd, 1);close(fd);
				execvp(tokens[0], tokens);
			}
			// On peut pas gérer le cas où le nom de fichier est vide car trouver_redirection renvoie NULL si pas de fichier trouvé


			file_out = trouve_redirection(tokens,">>");
			if(file_out != NULL){
				fd = open(file_out, O_WRONLY |O_APPEND| O_CREAT, 0644); // Je laisse le O_CREAT pour que le fichier soit créé s'il n'existe pas
				if(fd == -1){
					custom_err("open");
					custom_err(file_out);
					exit(1);
				}
				dup2(fd, 1);close(fd);
				execvp(tokens[0], tokens);
			}

			// On veut gérer le cas où il y a plusieurs redirections
			// On va donc chercher la première redirection et on va la traiter
			// Puis on va chercher la seconde redirection et on va la traiter
			// Et ainsi de suite jusqu'à ce qu'il n'y ait plus de redirections
			// On va donc utiliser une boucle while
			
			
			



			/* On exécute la commande donné par l'utilisateur.
			 Son nom est dans le premier token (le premier mot tapé)
			 ses arguments (éventuels) seront les tokens suivants */
			execvp(tokens[0], tokens);
		}
		else{wait(NULL);}
		
	}

	/* On ne devrait jamais arriver là */

	perror("execvp");
	exit(1);
}
