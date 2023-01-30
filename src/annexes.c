#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h> 

#include "shell-utils.h"

void color_noir (int bold) {
    printf("\033[%d;30m", !!bold);
}

void color_rouge (int bold) {
    printf("\033[%d;31m", !!bold);
}

void color_vert (int bold) {
    printf("\033[%d;32m", !!bold);
}

void color_jaune (int bold) {
    printf("\033[%d;33m", !!bold);
}

void color_bleu (int bold) {
    printf("\033[%d;34m", !!bold);
}

void color_violet (int bold) {
    printf("\033[%d;35m", !!bold);
}

void color_cyan (int bold) {
    printf("\033[%d;36m", !!bold);
}

void color_blanc (int bold) {
    printf("\033[%d;37m", !!bold);
}

void color_reset () {
    printf("\033[0m");
}

void init(){
    color_vert(1);
    printf("             / '-'  /\n");
    printf("            ;       ;\n");
    printf("         /'-|       |-' |\n");
    printf("        |   |_______P   |\n");
    printf("        |   '-------'   /\n");
    printf("         '.___.....___.'\n");
    printf("             | ;  : ;|\n");
    printf("            _|;__;__.|_\n");
    printf("           |     Y     |\n");
    printf("           |___.'^'.___|\n");
    printf("             |_  ;  _|      .--.\n");
    printf(" .--.        {^^^^^^^}     /;   |\n");
    printf("/   ;|       {       }    | ;   |\n");
    printf("|  ; |       {       }    |     |\n");
    printf("|;   |       ;-.__.'|    |:  ; |\n");
    printf("| ;  |      |;  ;   |_____/ ;  |\n");
    printf("|   '.'-----'      ' -_   .'   /\n");
    printf("|  '.   - _  ' ;  ;  _  -    .'\n");
    printf(" '.   -     - ;  ;   .------`\n");
    printf("   `--------.      ; |\n");
    printf("            |;  ,    |\n");
    printf("            |;  ,    |\n");
    color_reset();
    printf("\nBienvenue dans le shell\n\n");
}

void print_tokens(char * tokens[],int nb_tokens){
    printf("Tokens: ");
			for(int i=0; i<nb_tokens; i++){
				printf("%s ", tokens[i]);
			}
			printf("\n");
}

void custom_err(char* msg){
    color_rouge(1);
    if (strcmp(msg, "open") == 0){
        printf("Erreur: impossible d'ouvrir le fichier\n");
    }
    else if (strcmp(msg, "read")== 0){
        printf("Erreur: impossible de lire le fichier\n");
    }
    else if (strcmp(msg, "write")== 0){
        printf("Erreur: impossible d'écrire dans le fichier\n");
    }
    else if (strcmp(msg, "not_found")== 0){
        printf("Erreur: fichier non trouvé\n");
    }
    else if (strcmp(msg, "history")== 0)
    {
       printf("Erreur: Historique vide\n");
    }
    
    else{
        printf("Erreur: %s introuvable\n", msg);
    }
}


void redirect_to(char* tokens[], const char* symb){
    int fd;
    char *file_out;
    file_out = trouve_redirection(tokens,symb);
			if(file_out != NULL){
				char *file_out2 = trouve_redirection(tokens,symb);
				while(file_out2 != NULL){
					file_out = file_out2;
					file_out2 = trouve_redirection(tokens,symb);
				}
                if (strcmp(symb,">")==0){
                    fd = open(file_out, O_WRONLY | O_CREAT | O_TRUNC, 0644); //O_TRUNC pour que le fichier soit vidé s'il existe déjà
                }
                else if (strcmp(symb,">>")==0){
                    fd = open(file_out, O_WRONLY | O_CREAT | O_APPEND, 0644); //O_APPEND pour que le fichier soit vidé s'il existe déjà
                }
				if(fd == -1){
					custom_err("open");
					custom_err(file_out);
					exit(1);
				}
				dup2(fd, 1);close(fd);
				execvp(tokens[0], tokens);
			}
}
// Faudrait gérer le fait d'avoir plusieurs redirections mais de symboles différents

void redirect_from(char* tokens[]){
    int fd;
    char *file_in;
    file_in = trouve_redirection(tokens,"<");
            if(file_in != NULL){
                char *file_in2 = trouve_redirection(tokens,"<");
                while(file_in2 != NULL){
                    file_in = file_in2;
                    file_in2 = trouve_redirection(tokens,"<");
                }
                fd = open(file_in, O_RDONLY);
                if(fd == -1){
                    custom_err("open");
                    custom_err(file_in);
                    exit(1);
                }
                dup2(fd, 0);close(fd);
                redirect_to(tokens,">");
                redirect_to(tokens,">>");
                execvp(tokens[0], tokens);
            }
}

void detect_pipes(char* tokens[]){
    int fd[2];
    char ** sous_tokens2;
    int nb_pipes = 0;
    
    if((sous_tokens2 = trouve_pipe(tokens)) != NULL){
        nb_pipes = 1;
        pipe(fd);
        if (fork() == 0){
            close(fd[0]);
            dup2(fd[1],1);
            close(fd[1]);
            execvp(tokens[0], tokens);
        }
        else{
            wait(NULL);
            if (fork() == 0){
                close(fd[1]);
                dup2(fd[0],0);
                close(fd[0]);
                execvp(sous_tokens2[0], sous_tokens2);
            }
            else{
                wait(NULL);
            }
        }
       
    }

}


// Cette fonction compte le nombre de lignes dans le fichier .history
int len_history(){
    FILE *f = fopen(".history","r");
    if (f == NULL){
        custom_err("open");
        return -1;
    }

    int c;
    int nb_lines = 0;
    while ((c = fgetc(f)) != EOF){
        if (c == '\n'){
            nb_lines++;
        }
    }
    fclose(f);
    return nb_lines;
}

// fonction that puts history file in tmp file the writes the new command in the history file and then copies the tmp file in the history file
void add_history(char *tokens[], int nb_tokens){
    FILE *f = fopen(".history","r");
    if (f == NULL){
        custom_err("open");
        return;
    }

    FILE *tmp = fopen(".tmp","w");
    if (tmp == NULL){
        custom_err("open");
        return;
    }

    // copy history file in tmp file
    int c;
    while ((c = fgetc(f)) != EOF){
        fputc(c, tmp);
    }
    fclose(f);
    fclose(tmp);

    f = fopen(".history","w");
    if (f == NULL){
        custom_err("open");
        return;
    }

    tmp = fopen(".tmp","r");
    if (tmp == NULL){
        custom_err("open");
        return;
    }
    // write new command in history file
    for (int i = 0; i < nb_tokens; i++){
        fprintf(f, "%s ", tokens[i]);
    }
    fprintf(f, "\n");
    // copy tmp file in history file
    while ((c = fgetc(tmp)) != EOF){
        fputc(c, f);
    }
    fclose(f);
    fclose(tmp);
}

char *get_history(int index){
    FILE *f = fopen(".history", "r");
    if (f == NULL){
        custom_err("open");
        return NULL;
    }

    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int i = 0;
    while ((read = getline(&line, &len, f)) != -1) {
        if (i == index){
            fclose(f);
            return line;
        }
        i++;
    }
    fclose(f);
    if (line)
        free(line);
    return NULL;
}

// fonction permettant de lire les 10 dernières commandes tapées et de les afficher dans le terminal
void print_history(int nb){
    int len = len_history();
    if (len <= 0){
        custom_err("history");
    }
    if (nb > len){
        nb = len;
    }
    printf("\nHistorique des %d dernières commandes:\n",nb);
    // on affiche les commandes à l'envers
     for (int i = nb-1; i >= 0; i--){
         printf("%d: %s", i+1, get_history(i));
     }
}
