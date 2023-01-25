#include <stdio.h>

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

// On veut faire un historique des commandes tapées et pouvoir y accéder avec les flèches du haut et du bas

