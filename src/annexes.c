#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
