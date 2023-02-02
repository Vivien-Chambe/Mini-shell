void init();
void color_noir (int bold);
void color_rouge (int bold);
void color_vert (int bold);
void color_jaune (int bold);
void color_bleu (int bold);
void color_violet (int bold);
void color_cyan (int bold);
void color_blanc (int bold);
void color_reset ();
void print_tokens (char *tokens[],int nb_tokens);
void custom_err (char* msg,char* tokens[]);

void redirect_to (char *tokens[], const char* symb);
void redirect_from (char *tokens[]);
void detect_pipes (char *tokens[]);

void handler(int sig);
int trouve_et(char** tokens);

void add_history (char *tokens[], int nb_tokens);
void print_history (int nb,char *tokens[]);
int len_history (char *tokens[]);
char *get_history (int index,char *tokens[]);

char* locate_history ();
