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
	signal(SIGCHLD,handler);
	/* char * path = ".history";
	if(strcmp(path,".history")==0){};
	 Pour trouver le chemin de l'historique mais très long à lancer 
	if(fork()==0){path = locate_history();} */
	

	
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

		
		/* S'il y a trop de tokens, on abandonne */
		if (nb_tokens==MAX_NB_TOKENS) {
			fprintf(stderr, "Too many tokens: exiting\n");
			exit(1);
		}
		/* S'il n'y a pas de token, c'est que l'utilisateur n'a pas donné de
		 commande. Il n'y a rien à faire. On arrête tout. */
		if (nb_tokens<=0) {
			fprintf(stderr, "No tokens found\n");
			continue; // On continue la boucle while(1) pour pouvoir relancer une commande et pas quitter le shell
		}

		if(nb_tokens==2 && strcmp(tokens[0],"cd")==0){
			chdir(tokens[1]);continue;	
		}

		if(nb_tokens==2 && strcmp(tokens[0],"get")==0){
			printf("Execution de la commande : %s\n",get_history(atoi(tokens[1])-1,tokens));
			nb_tokens=split_tokens(tokens,get_history(atoi(tokens[1])-1,tokens),MAX_NB_TOKENS);
		}

		// Affichage de l'historique
		else if(nb_tokens==1 && strcmp(tokens[0],"hist")==0 ){
			print_history(10,tokens);continue;
		}
		else if(nb_tokens==2 && strcmp(tokens[0],"hist")==0 ){
			print_history(atoi(tokens[1]),tokens);continue;
		}

		else{
			/* On ajoute la commande à l'historique */
			// Etant donné que l'on a fait des vérifications sur le nombre de tokens, on peut ajouter la commande à l'historique
			add_history(tokens,nb_tokens);
		}
		
		if(strcmp(tokens[0],"exit")==0){exit(0);}
		int pid;
		int trouve = trouve_et(tokens);

		

		if((pid=fork())==0){
			//print_tokens(tokens,nb_tokens);
			/* On détecte les pipes et on les traite */
			detect_pipes(tokens);
		}
		
		else{
			if(trouve){waitpid(-1,NULL,WNOHANG);}
			else{waitpid(pid,NULL,0);}
		}
		
	}

	/* On ne devrait jamais arriver là */

	perror("execvp");
	exit(1);
}