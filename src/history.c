#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "annexes.h"


char* main(int argc, char *argv[])
{
    // affiche l'historique des commandes tapées dans le terminal
    if (argc == 1){
        print_history();
        return NULL;
    }
    // renvoie la commande tapée à l'index passé en paramètre
    else if (argc == 2){
        int index = atoi(argv[1]);
        char *cmd = get_history(index);
        if (cmd != NULL){
            return cmd;
        }
    
    }
    return NULL;
}