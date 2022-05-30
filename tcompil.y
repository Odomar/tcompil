%{
/* tpc-2018.y */
/* Syntaxe du TPC pour le projet de compilation de 2018 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INIT_SIZE 16
#define MAX_WORD_SIZE 64
#define PARAM_MAX 64
#define VARSIZE 8

// Types
#define VID -1
#define CAR 0
#define CARTAB 1
#define ENT 2
#define ENTTAB 3

int start_flag = 0;
int if_label = 0;
int jump_label=0;
int while_label = 0;
int popidx = 0;
int argcount[MAX_WORD_SIZE];
char fctname[MAX_WORD_SIZE][MAX_WORD_SIZE];
int i_arg = 0;
int return_flag = 0;
int constval;
#define z(instruction) printf("%s\n", instruction)
#define z1(instruction, operande) printf("%s %d\n", instruction, operande)
#define zlabel(ch1, n, ch2) printf("%s%d%s\n", ch1, n, ch2)

int yylex();
void yyerror(char *);
extern int lineno;

typedef struct {
	char name[MAX_WORD_SIZE]; //nom de la variable
	int type; // type
	int addr; // addresse
	int globale; // 0 ou 1, globale ou locale
} STVentry;

typedef struct {
	STVentry * table; //table 
	int max; // taille max
	int size; // taille actuelle
	int stack_pointer; // pointeur de pile (commence à 0)
} SymbolTableVar;

typedef struct {
	char name[MAX_WORD_SIZE]; //nom de la variable
	int type; // type
	int val; // addresse
	int globale; // 0 ou 1, globale ou locale
} STCentry;

typedef struct {
	STCentry * table; //table 
	int max; // taille max
	int size; // taille actuelle
} SymbolTableCst;

typedef struct {
	char name[MAX_WORD_SIZE]; // nom de la fct
	int type; // type de retour de la fonction
	int param_num; // nombre réel de parametres
	//int param_max; // nombre max de parametres
	int param_types[PARAM_MAX]; // types des parametres
	char param_names[MAX_WORD_SIZE][PARAM_MAX]; // noms des parametres
} STFentry;

typedef struct {
	STFentry * table; // table des fonctions
	int max; // taille max
	int size; // taille actuelle
} SymbolTableFct;

typedef struct {
	SymbolTableVar * vars; // Pile (tableau) de tables des variables
	SymbolTableCst * csts; // Pile de tables des constantes
	int size; // La taille réelle de la pile de tables des symboles 
	int max; // La taille max de la pile des tables des symboles
} SymbolTableStk; // La pile des differentes tables des symboles

SymbolTableStk ST;
SymbolTableFct STF;

/* Alloue la table des symboles globale
 */
void STallocate() {
	int i;
	if (NULL != (ST.vars = (SymbolTableVar *)malloc(INIT_SIZE * sizeof(SymbolTableVar)))) {
		for (i = 0; i < INIT_SIZE; i++) {
			if (NULL != (ST.vars[i].table = (STVentry *)malloc(INIT_SIZE * sizeof(STVentry)))) {
				ST.vars[i].max = INIT_SIZE;
				ST.vars[i].size = 0;
				ST.vars[i].stack_pointer = VARSIZE;
			}
		}
	}
	if (NULL != (ST.csts = (SymbolTableCst *)malloc(INIT_SIZE * sizeof(SymbolTableCst)))) {
		for (i = 0; i < INIT_SIZE; i++) {
			if (NULL != (ST.csts[i].table = (STCentry *)malloc(INIT_SIZE * sizeof(STCentry)))) {
				ST.csts[i].max = INIT_SIZE;
				ST.csts[i].size = 0;
			}
		}
	}
	ST.max = INIT_SIZE;
	ST.size = 1;
	// La première case du tableau est pour les globales
}

/* Alloue la table des symboles globale
 */
void STFallocate() {
	int i;
	if (NULL != (STF.table = (STFentry *)malloc(INIT_SIZE * sizeof(STFentry)))) {
		STF.max = INIT_SIZE;
		STF.size = 1;
		strcpy(STF.table[0].name, "Variables globales");
		// La première case du tableau est pour les globales
		for(i = 0; i < INIT_SIZE; i++) {
			STF.table[i].param_num = 0;
		}
	}
	else {
		fprintf(stderr, "Allocation error\n");
		exit(EXIT_FAILURE);
	}
}

/* Réalloue la pile de table des symboles globale
 */
void STrealloc_stack() {
	if (ST.size >= ST.max) {
		ST.max *= 2; // realloc
		if (NULL == (ST.vars = realloc(ST.vars, ST.max * sizeof(SymbolTableVar)))){
			fprintf(stderr, "Allocation error\n");
			exit(EXIT_FAILURE);
		}
		if (NULL == (ST.csts = realloc(ST.csts, ST.max * sizeof(SymbolTableCst)))){
			fprintf(stderr, "Allocation error\n");
			exit(EXIT_FAILURE);
		}
	}
}

/* Realloue le haut de la pile de la table des symboles.
 */
void STrealloc_table(){
	if (ST.vars[ST.size].size >= ST.vars[ST.size].max) {
		ST.vars[ST.size].max *= 2;
		if (NULL == (ST.vars[ST.size].table = realloc(ST.vars[ST.size].table, ST.vars[ST.size].max * sizeof(STVentry)))) {
			fprintf(stderr, "Allocation error\n");
			exit(EXIT_FAILURE);
		}
	}
	if (ST.csts[ST.size].size >= ST.csts[ST.size].max) {
		ST.csts[ST.size].max *= 2;
		if (NULL == (ST.csts[ST.size].table = realloc(ST.csts[ST.size].table, ST.csts[ST.size].max * sizeof(STCentry)))) {
			fprintf(stderr, "Allocation error\n");
			exit(EXIT_FAILURE);
		}
	}
}

void STpush() {
	STrealloc_stack();
	ST.vars[ST.size].table = malloc(INIT_SIZE * sizeof(STVentry));
	if (ST.vars[ST.size].table == NULL){
		fprintf(stderr, "Allocation error\n");
		exit(EXIT_FAILURE);
	}
	ST.vars[ST.size].max = INIT_SIZE;
	ST.vars[ST.size].size = 0;
	ST.vars[ST.size].stack_pointer = VARSIZE;

	ST.csts[ST.size].table = malloc(INIT_SIZE * sizeof(STCentry));
	if (ST.csts[ST.size].table == NULL){
		fprintf(stderr, "Allocation error\n");
		exit(EXIT_FAILURE);
	}
	ST.csts[ST.size].max = INIT_SIZE;
	ST.csts[ST.size].size = 0;
	ST.size++;
}

void write_pop() {
	printf("\tpop rbx\n");
}

void write_endfct() {
	int i;
	printf("\n_end_%s:\n", STF.table[STF.size - 1].name);
	i = ST.vars[1].stack_pointer - 8;
    while(i >= 8) {
        write_pop();
        i -= VARSIZE;
    }
	printf("\tpop rbp\n\
\tret\n");
}

void STpop() {
	char err_msg[128];
	if (ST.size < 1) {
		fprintf(stderr, "Popping empty symbol table stack\n");
		exit(EXIT_FAILURE);
	}
	write_endfct();
	if (STF.table[STF.size - 1].type != VID && !return_flag) {
		sprintf(err_msg, "no return in non void function : %s", STF.table[STF.size - 1].name);
		yyerror(err_msg);
	}
	return_flag = 0;
	ST.size--;
}	

void STFrealloc() {
	int i;
	if (STF.size >= STF.max) {
		if (NULL == (STF.table = realloc(STF.table, 2 * STF.max * sizeof(STFentry)))) {
			fprintf(stderr, "Allocation error\n");
			exit(EXIT_FAILURE);
		}
		for(i = STF.max; i < 2 * STF.max; i++) {
			STF.table[i].param_num = 0;
		}
		STF.max *= 2;
	}
}

void write_push(int val) {
	printf("\tpush QWORD %d\n", val);
}

/* Renvoie l'indice du parametre dans le tableau de parametres de la dernière fonction.
 */
int get_param(char name[]) {
	int i;
	char err_msg[128];

	for(i = 0; i < STF.table[STF.size - 1].param_num; i++) {
		if (!strcmp(STF.table[STF.size - 1].param_names[i], name))
			return i;
	}

	sprintf(err_msg, "%s undeclared (get_param)", name);
	yyerror(err_msg);
	return -1;
}

/* Vérifie si la variable est définie ou non.
 * 1 -> déja là 0 sinon.
 * -1 si parametre de la fonction.
 */
int is_declared_var(const char name[]){
	int i;
	// On cherche dans les variables de la fonction
	for (i = 0; i < ST.vars[ST.size - 1].size; i++){
		if (!strcmp(ST.vars[ST.size - 1].table[i].name, name))
			return 1;
	}
	// Puis dans les parametres de la fonctions
	for(i = 0; i < STF.table[STF.size - 1].param_num; i++) {
		if (!strcmp(STF.table[STF.size - 1].param_names[i], name))
			return -1;
	}
	// Dans les constantes locales
	for (i = 0; i < ST.csts[ST.size - 1].size; i++){
		if (!strcmp(ST.csts[ST.size - 1].table[i].name, name))
			return 1;
	}
	// Dans les constantes globales
	for (i = 0; i < ST.csts[0].size; i++){
		if (!strcmp(ST.csts[0].table[i].name, name))
			return 1;
	}
	// Enfin dans les globales
	for (i = 0; i < ST.vars[0].size; i++){
		if (!strcmp(ST.vars[0].table[i].name, name))
			return 1;
	}
	return 0;
}

int is_constant(char * name) {
	int i;
	// constante locale
	for (i = 0; i < ST.csts[ST.size -1].size; i++){
		if (!strcmp(ST.csts[ST.size -1].table[i].name, name))
			return 1;
	}
	// constante globale
	for (i = 0; i < ST.csts[0].size; i++){
		if (!strcmp(ST.csts[0].table[i].name, name))
			return 1;
	}
	return 0;
}

STVentry get_entry(char name[]) {
	int i;
	char err_msg[128];
	int err = is_declared_var(name);
	STVentry null_entry;
	STVentry param_entry;
	if (err == 0) {
		// variable non declaree
		sprintf(err_msg, "%s undeclared (get_entry)", name);
		yyerror(err_msg);
		return null_entry;
	}
	if (err == 1){
		// variable locale
		for (i = 0; i < ST.vars[ST.size - 1].size; i++){
			if (!strcmp(ST.vars[ST.size - 1].table[i].name, name))
				return ST.vars[ST.size - 1].table[i];
		}
		// variable globale
		for (i = 0; i < ST.vars[0].size; i++){
			if (!strcmp(ST.vars[0].table[i].name, name))
				return ST.vars[0].table[i];
		}
	}
	// parametre de la fct : on construit un STVentry
	i = get_param(name);
	if (i == -1) {
		sprintf(err_msg, "%s undeclared (get_entry)", name);
		yyerror(err_msg);
		return null_entry;
	}
	strcpy(param_entry.name, name);
	param_entry.type = STF.table[STF.size - 1].param_types[i];
	param_entry.addr = -(STF.table[STF.size - 1].param_num - i + 1) * VARSIZE;
	param_entry.globale = 0;
	return param_entry;
}

int get_cst_value(char name[]) {
	int i;
	char err_msg[128];

	for (i = 0; i < ST.csts[ST.size -1].size; i++){
		if (!strcmp(ST.csts[ST.size -1].table[i].name, name))
			return ST.csts[ST.size -1].table[i].val;
	}
	for (i = 0; i < ST.csts[0].size; i++){
		if (!strcmp(ST.csts[0].table[i].name, name))
			return ST.csts[0].table[i].val;
	}
	sprintf(err_msg, "undeclared constant %s", name);
	yyerror(err_msg);
	return 0;
}

void write_push_var_tab(char * var) {
	/*int i = get_entry(var);
	int addr = ST.table[i].addr;
	int addr_next = ST.table[i + 1].addr;
	if(addr + VARSIZE * dec >= addr_next)
		fprintf(stderr, "Index Out Of Bound\n");
        exit(EXIT_FAILURE); */
    STVentry s = get_entry(var);
    if (s.globale) {
	    printf("\tpush %d\n", -s.addr);
	    printf("\tpush %d\n", VARSIZE);
		printf("\tcall tab_glo\n");
    }
    else {
	    printf("\tpush %d\n", s.addr);
	    printf("\tpush %d\n", VARSIZE);
		printf("\tcall tab_var\n");
	}
    
}

void pre_calcul(int t1, int t2) {
	if(t1 % 2 == 0 && t2 % 2 == 0)
		return;
	else if(t1 % 2 != 0 && t2 % 2 == 0) {		
		printf("\tpop rax\n\
\tpop rbx\n\
\tpop rcx\n\
\tpush rbx\n\
\tpush rax\n");
	}
	else if(t2 % 2 != 0 && t1 % 2 == 0) {
		printf("\tpop rax\n\
\tpop rcx\n\
\tpop rbx\n\
\tpush rbx\n\
\tpush rax\n");
	}
	else {
		printf("\tpop rax\n\
\tpop rcx\n\
\tpop rbx\n\
\tpop rcx\n\
\tpush rbx\n\
\tpush rax\n");
	}	
}

void write_push_var(char * varname) {
	STVentry var = get_entry(varname);
	if (var.globale) 
		printf("\tpush QWORD [glob+%d]\n", var.addr);
	else if (var.addr >= 0)
		printf("\tpush QWORD [rbp-%d]\n", var.addr);
	else 
		printf("\tpush QWORD [rbp+%d]\n", -var.addr);
}

void write_push_cst(char * cstname) {
	printf("\tpush QWORD %d\n", get_cst_value(cstname));
}

void write_assign(STVentry var) {
	if (var.globale) 
		printf("\tpop QWORD [glob+%d]\n", var.addr);
	else if (var.addr >= 0)
		printf("\tpop QWORD [rbp-%d]\n", var.addr);
	else 
		printf("\tpop QWORD [rbp+%d]\n", -var.addr);
}

void write_add() {
	printf("\tpop rbx\n\
\tpop rax\n\
\tadd rax, rbx\n\
\tpush rax\n");
}

void write_sub() {
	printf("\tpop rbx\n\
\tpop rax\n\
\tsub rax, rbx\n\
\tpush rax\n");
}

void write_mul() {
	printf("\tpop rbx\n\
\tpop rax\n\
\timul eax, ebx\n\
\tpush rax\n");
}

void write_div() {
	printf("\tpop rbx\n\
\tpop rax\n\
\tmov rdx, 0\n\
\tidiv ebx\n\
\tpush rax\n");
}

void write_mod() {
	printf("\tpop rbx\n\
\tpop rax\n\
\tmov rdx, 0\n\
\tidiv ebx\n\
\tpush rdx\n");
}

void write_opposite() {
	printf("\tmov rax, 0\n\
\tpop rbx\n\
\tsub rax, rbx\n\
\tpush rax\n");
}


/* Vérifie si la fonction est définie ou non.
 * 1 -> déja là 0 sinon.
 */
int is_declared_fct(const char name[]){
	int i;

	for (i = 0; i < STF.size; i++){
		if (!strcmp(STF.table[i].name, name))
			return 1;
	}
	return 0;
}

/* Vérifie que la variable a bien été declarée auparavant.
 * Affiche un message d'erreur sinon.
 */
void lookup_var(const char name[]) {
	char err_msg[128];
	if (!is_declared_var(name)) {
		sprintf(err_msg, "%s undeclared (look_up_var)", name);
		yyerror(err_msg);
	}
}

/* Vérifie que la variable a bien été declarée auparavant.
 * Affiche un message d'erreur sinon.
 */
void lookup_fct(const char name[]) {
	char err_msg[128];
	if (!is_declared_fct(name)) {
		sprintf(err_msg, "%s undeclared (look_up_fct)", name);
		yyerror(err_msg);
	}
}

/* Renvoie le type. 
 */
int get_type(const char name[]) {
	char err_msg[128];
	int i;
	for (i = 0; i < STF.size; i++) {
		if (!strcmp(STF.table[i].name, name))
			return STF.table[i].type;
	}
	for(i = 0; i < ST.vars[ST.size - 1].size; i++) {
		if (!strcmp(ST.vars[ST.size - 1].table[i].name, name))
			return ST.vars[ST.size - 1].table[i].type;
	}
	for (i = 0; i < STF.table[STF.size - 1].param_num; i++) {
		if (!strcmp(STF.table[STF.size - 1].param_names[i], name))
			return STF.table[STF.size - 1].param_types[i];
	}
	for(i = 0; i < ST.vars[0].size; i++) {
		if (!strcmp(ST.vars[0].table[i].name, name))
			return ST.vars[0].table[i].type;
	}
	for(i = 0; i < ST.csts[ST.size - 1].size; i++) {
		if (!strcmp(ST.csts[ST.size - 1].table[i].name, name))
			return ST.csts[ST.size - 1].table[i].type;
	}
	for(i = 0; i < ST.csts[0].size; i++) {
		if (!strcmp(ST.csts[0].table[i].name, name))
			return ST.csts[0].table[i].type;
	}
	sprintf(err_msg, "%s undeclared (get_type)", name);
	yyerror(err_msg);
	return -1;
}

/* Retourne le type de l'entrée en chaine de caractère affichable.
 */
char * entry_type_to_string(STVentry entry) {
	switch (entry.type) {
		case ENT: return "entier";
		case CAR: return "caractere";
		case CARTAB: return "tab_caractere";
        case ENTTAB: return "tab_entier";
		default: return NULL; // This should never happens.
	}
	return NULL; // This should never happens.
}

/* Ajoute la variable dans la table si elle n'est pas dedans.
 */
void addVar(const char name[], const char type[], int size){
	char err_msg[128];
	STVentry var;
	if(!is_declared_var(name)) {
		STrealloc_table();
		strcpy(var.name, name);
		if (!strcmp(type, "caractere")) {
			if(size > 1)
				var.type = CARTAB;
        	else
				var.type = CAR;
		}
	    else if (!strcmp(type, "entier")) {
			if(size > 1)
				var.type = ENTTAB;
        	else
				var.type = ENT;
		}
		else {
			sprintf("unkown type : %s", type);
			yyerror(err_msg);
		}
		var.addr = ST.vars[ST.size - 1].stack_pointer;
		ST.vars[ST.size - 1].stack_pointer += VARSIZE * size;
		if (ST.size != 1) {
			var.globale = 0;
			int i;
			for(i = 0; i < size; i++)
				printf("\tpush QWORD 0\n");
		}
		else  {
			var.globale = 1;
		}
		ST.vars[ST.size - 1].table[ST.vars[ST.size - 1].size] = var;
		ST.vars[ST.size - 1].size += 1;
	}
}

/* Ajoute une fonction dans la table des fonctions.
 */
void addFct(const char type[], const char name[]) {
	STFentry fct;	
	char err_msg[128];

	if(!is_declared_fct(name)) {
		STpush();
		STFrealloc();
		STrealloc_stack();
		strcpy(fct.name, name);
		if (!strcmp(type, "void"))
			fct.type = VID;
		else if (!strcmp(type, "caractere"))
			fct.type = CAR;
		else if (!strcmp(type, "entier"))
			fct.type = ENT;
		else {
			sprintf("unkown type : %s", type);
			yyerror(err_msg);
		}
		STF.table[STF.size] = fct;
		STF.size++;
	}
}

void addParam(const char type[], const char name[]) {
	if (!strcmp(type, "void")) {
		STF.table[STF.size - 1].param_num = 0;
		return;
	}
	if (STF.table[STF.size - 1].param_num >= PARAM_MAX){
		yyerror("too much params"); // TODO : realloc
	}
	if(!strcmp(type, "entier"))
		STF.table[STF.size - 1].param_types[STF.table[STF.size - 1].param_num] = ENT;
	if(!strcmp(type, "caractere"))
		STF.table[STF.size - 1].param_types[STF.table[STF.size - 1].param_num] = CAR;
	strcpy(STF.table[STF.size - 1].param_names[STF.table[STF.size - 1].param_num], name);
	STF.table[STF.size - 1].param_num ++;
}

void addCst(const char name[], int type) {
	STCentry cst;
	if(!is_declared_var(name)) {
		STrealloc_table();
		strcpy(cst.name, name);
		cst.type = type;
		cst.val = constval;
		cst.globale = (ST.size == 1); // cst est globale si ST.size est à 1 (partie "globale" de la table)
		ST.csts[ST.size - 1].table[ST.csts[ST.size - 1].size] = cst;
		ST.csts[ST.size - 1].size += 1;
	}
}

char * string_of_type(int t) {
	switch (t) {
		case VID: return "void";
		case ENT: return "entier";
		case CAR: return "caractere";
		case ENTTAB : return "tab_entier";
        case CARTAB : return "tab_caractere";
		default: return NULL; // This should never happens.
	}
}

int check_type(int t1, int t2) {
	char err_msg[128];
	if((t1 <= 1 && t2 <= 1) || (t1 > 1 && t2 > 1)) {
		return t1;
	}
	sprintf(err_msg, "different type : %s %s", string_of_type(t1), string_of_type(t2));
	yyerror(err_msg);
	return -1;		
}

int check_argcount(char * name) {
	int i;
	char err_msg[128];
	for (i = 0; i < STF.size; i++) {
		if (!strcmp(STF.table[i].name, name)) {
			if(STF.table[i].param_num != argcount[i_arg]) {
				sprintf(err_msg, "%s error in argument count : %d needed, %d passed", name, STF.table[i].param_num, argcount[i_arg]);
				yyerror(err_msg);
				return -1;	
			}
			else{
				i_arg -= 1;
				return 0;
			}
		}
	}
	sprintf(err_msg, "%s undeclared (check_argcount)", name);
	yyerror(err_msg);
	return -1;	
}

int check_argtype(int type) {
	int i;
	char err_msg[128];
	for (i = 0; i < STF.size; i++) {
		if (!strcmp(STF.table[i].name, fctname[i_arg])) {
			if(STF.table[i].param_types[(argcount[i_arg]) - 1] != type) {
				sprintf(err_msg, "type error, %s expected, got %s", string_of_type(STF.table[i].param_types[argcount[i_arg]]), string_of_type(type));
				yyerror(err_msg);
				return -1;	
			}
			else{
				return 0;
			}
		}
	}
	sprintf(err_msg, "%s undeclared (check_argtype)", fctname[i_arg]);
	yyerror(err_msg);
	return -1;
}

/* Affiche la table
 */
void display_tables(){
	int i;
	fprintf(stderr, "Taille de la table : %d/%d\nVariables globales :\n", ST.size, ST.max);
	for (i = 0; i < ST.vars[i].size; i++) {
		fprintf(stderr, "\tVariable %2d : [%03d] %s \"%s\"\n", i, ST.vars[i].table[i].addr, string_of_type(ST.vars[i].table[i].type), ST.vars[i].table[i].name);
	}
	fprintf(stderr, "Fonctions declarées :\n");
	for (i = 1; i < STF.size; i++) {
		fprintf(stderr, "\t%s \"%s\"\n", string_of_type(STF.table[i].type), STF.table[i].name);
	}
}

/* Libère la table des symboles
 */
void STfree() {
	free(ST.vars);
}

/* Libère la table des symboles de fonctions
 */
void STFfree() {
	free(STF.table);
}

void write_start() {
	printf("section .bss\n\
glob resq %d\n\n\
section .data\n\n\
section .text\n\n\
extern printf\n\n\
extern print_registers\n\n\
extern scanf\n\n\
global _start\n\n\
\tformat_int: db \"%%ld\", 10, 0\n\
\tformat_char: db \"%%c\", 10, 0\n\
\tformat_scanf_c: db \"%%c\", 0\n\
\tformat_scanf_d: db \"%%d\", 0\n\n\
tab_var:\n\
\tpop rsi\n\
\tpop rdx\n\
\tmov rbx, rdx\n\
\timul rbx, rax\n\
\tmov rdx, rbp\n\
\tpop rax\n\
\tmov rcx, rax\n\
\tadd rbx, rcx\n\
\tsub rdx, rbx\n\
\tpush rdx\n\
\tpush QWORD [rdx]\n\
\tpush rsi\n\
\tret\n\n\
tab_glo:\n\
\tpop rsi\n\
\tpop rdx\n\
\tmov rbx, rdx\n\
\timul rbx, rax\n\
\tmov rdx, glob\n\
\tpop rax\n\
\tmov rcx, rax\n\
\tadd rbx, rcx\n\
\tadd rdx, rbx\n\
\tpush rdx\n\
\tpush QWORD [rdx]\n\
\tpush rsi\n\
\tret\n\n\
_start:\n\
\tcall main\n\
\tjmp _end\n", ST.vars[0].size * 8);
}

void print(int type) {
	if(type == ENT) {
		printf("\tpop rax\n\tmov rsi, rax\n\
\tmov rdi, format_int\n\
\tmov rax, 0\n\
\tcall printf\n");
	}
	else if (type == CAR){
		printf("\tpop rax\n\
\tmov rsi, rax\n\
\tmov rdi, format_char\n\
\tmov rax, 0\n\
\tcall printf\n");
	}
	else if (type == ENTTAB){
		printf("\tpop rbx\n\
\tmov rax, rbx\n\
\tmov rsi, rax\n\tmov rdi, format_int\n\
\tmov rax, 0\n\
\tcall printf\n\
\tpop rcx\n");
	}
	else if (type == CARTAB) {
		printf("\tpop rbx\n\
\tmov rax, rbx\n\
\tmov rsi, rax\n\
\tmov rdi, format_char\n\
\tmov rax, 0\n\
\tcall printf\n\
\tpop rcx\n");
	}
}

void read(int addr, int type) {
	if(type == ENT) {
		printf("\tmov rdi, format_scanf_d\n\
\tmov rsi, rsp\n\
\tmov rax, 0\n\
\tmov rbx, [rsp]\n\
\tcall scanf\n\
\tmov rax, [rsp]\n\
\tmov QWORD [rbp - %d], rax\n\
\tmov [rsp], rbx\n", addr);
	}
	else if (type == CAR){
		printf("\tmov rdi, format_scanf_c\n\
\tmov rsi, rsp\n\tmov rax, 0\n\
\tmov rbx, [rsp]\n\
\tcall scanf\n\
\tmov rax, [rsp]\n\
\tmov QWORD [rbp - %d], rax\n\
\tmov [rsp], rbx\n", addr);
	}
}

void write_end() {
	printf("\n_end:\n\
\tmov rdi, rax\n\
\tmov rax, 60\n\
\tsyscall\n");
}

void write_fct(char * name) {
	printf("\n%s:\n\
\tpush rbp\n\
\tmov rbp, rsp\n", name);
}

STFentry get_fct(char * name) {
	int i;
	char err_msg[128];
	STFentry null_entry;
	for (i = 0; i < STF.size; i++) {
		if (!strcmp(STF.table[i].name, name))
			return STF.table[i];
	}

	sprintf("undeclared fonction %s", name);
	yyerror(err_msg);
	return null_entry;
}

void write_call(char * name) {
	printf("\tmov rax, 0\n\
\tcall %s\n", name);
	int i;
	STFentry fct = get_fct(name);
	for(i = 0; i < fct.param_num; i++) {
		write_pop();
	}
	// printf("\tpush rax\n\n");
}

void write_jmpend() {
	printf("\tjmp _end_%s\n", STF.table[STF.size - 1].name);
}

void write_test() {
	printf("\tpush 1\n"); 
	zlabel("\tjmp jump_label" , jump_label,""); 
	zlabel("\njump_label" , jump_label -1 ," :"); 
	printf("\tpush 0\n"); 
	zlabel("\njump_label" , jump_label ++ ," :");
}

%}

%union {
	char caractere;
	int num;
	char ident[64];
	char type[16];
	char comp[3];
	char addsub;
	char divstar;
}

%token <caractere> CARACTERE
%token <num> NUM
%token <ident> IDENT
%token <comp> ORDER EQ
%token <addsub> ADDSUB
%token <divstar> DIVSTAR
%token <type> TYPE
%token OR AND CONST IF WHILE RETURN VOID READE READC PRINT

%left ','
%precedence ')'
%precedence ELSE

%type <num> EB TB FB F T E M Exp Litteral NombreSigne
%type <ident> LValue Declarateur DeclFonct EnTeteFonct
%%
Prog:  DeclConsts DeclVars DeclFoncts 
	;
DeclConsts:
	   DeclConsts CONST ListConst ';' 
	|  ;
ListConst:
	   ListConst ',' IDENT '=' Litteral {fprintf(stderr, "AddCst1 : %s\n", $3); addCst($3, $5);}
	|  IDENT '=' Litteral {fprintf(stderr, "AddCst2 : %s = %d\n", $1, $3); addCst($1, $3);}
	;
Litteral:
	   NombreSigne {$$ = $1;}
	|  CARACTERE {$$ = CAR; constval = $1;}
	;
NombreSigne:
	   NUM {$$ = ENT; constval = $1;}
	|  ADDSUB NUM {$$ = ENT; if ($1 == '+') constval = $2; else constval = -($2);}
	;
DeclVars:
	   DeclVars TYPE Declarateurs ';' {strcpy($<type>0, $2);}
	|  ;
Declarateurs:
	   Declarateurs ',' Declarateur 
	|  Declarateur 
	;
Declarateur:
	   IDENT {addVar($1, $<type>0, 1);}
	|  IDENT '[' NUM ']'  {addVar($1, $<type>0, $3);}
	;
DeclFoncts:
	   DeclFoncts DeclFonct
	|  DeclFonct
	;
DeclFonct:
	   EnTeteFonct Corps 
	;
EnTeteFonct:
	   TYPE IDENT '(' {
		   	if (!start_flag) {
		   		start_flag = 1; write_start();
		   	} 
		   	addFct($1, $2);
	    } 
	   Parametres ')' {
	   		write_fct($2); 
		}
	|  VOID IDENT '(' {
		   	if (!start_flag) {
		   		start_flag = 1; write_start();
		   	} 
		   	addFct("void", $2);
	    }
	   Parametres ')' {
	   		write_fct($2); 
		}
	;
Parametres:
	   VOID {addParam("void", "");}
	|  ListTypVar
	;
ListTypVar:
	   ListTypVar ',' TYPE IDENT {addParam($3, $4);}
	|  TYPE IDENT {addParam($1, $2);}
	;
Corps: '{' DeclConsts DeclVars SuiteInstr '}' {STpop();}
	;
SuiteInstr:
	   SuiteInstr Instr 
	|  
	;

DEBUTIF: {zlabel("\nstart_if",$<num>$=if_label," :");}
;   

DEBUTELSE: {zlabel("\tjmp exit_if" ,if_label + 1,""); zlabel("\nexit_if",if_label ++," :");}
;
	
ENDIF: {printf("\tpop rax\n");printf("\tcmp rax, 1\n"); zlabel("\tjne exit_if" , if_label + 1 ,""); zlabel("\tjmp exit_if" , if_label ,"");zlabel("\nexit_if",if_label ++," :");}
;	

DEBUTWHILE : {zlabel("\nstart_while",while_label++," :");}
;

Instr:
	   Exp ';' {if ($1 >= 0) {write_pop();}}
	|  ';' 
	|  RETURN Exp ';' {printf("\tpop rax\n"); write_jmpend(); check_type(STF.table[STF.size - 1].type, $2); return_flag = 1;}
	|  RETURN ';' {write_jmpend(); check_type(STF.table[STF.size - 1].type, VID); fprintf(stderr, "%d / %d\n", STF.table[STF.size - 1].type, VID); return_flag = 1;}
	|  READE '(' IDENT ')' ';' {lookup_var($3); read(get_entry($3).addr,get_type($3));}
	|  READC '(' IDENT ')' ';' {lookup_var($3); read(get_entry($3).addr,get_type($3));}
	|  PRINT '(' Exp ')' ';' {print($3);}
	|  DEBUTIF IF '(' Exp ')' ENDIF Instr {if($4 % 2 != 0) {printf("\tpop rax");} zlabel("\nexit_if" ,if_label," :");}
    |  DEBUTIF IF '(' Exp ')' ENDIF Instr ELSE DEBUTELSE Instr {zlabel("\nexit_if", if_label ++," :");if($4 % 2 != 0) {printf("\n\tpop rax\n");}}
    |  DEBUTWHILE WHILE '(' Exp ')' {printf("\tpop rax\n");printf("\tcmp rax, 1\n"); zlabel("\tjne exit_while" , while_label - 1 ,"");}
       Instr {zlabel("\tjmp start_while",while_label-- -1 ,""); zlabel("\nexit_while" ,while_label ," :");}
    |  '{' SuiteInstr '}' 
	;
Exp :  LValue '=' Exp {$$ = get_type($1); 
						check_type(get_type($1), $3);
						lookup_var($1); 
						if ($$ % 2 == 0 ){
							if (is_constant($1))
								yyerror("assigning constant value");
							else 
								write_assign(get_entry($1));} 
						else {
							printf("\tpop rax\n\
\tpop rbx\n\
\tpop rcx\n\
\tpush rax\n\
\tpop QWORD [rcx]\n\
\tpush rax\n");}}
    |  EB 
    ;
EB  :  EB OR TB  {$$ = check_type($1, $3); 
					printf("\tpop rax\n\
\tpop rbx\n\
\tor rax, rbx\n\
\tpush rax\n");} 
    |  TB
    ;
TB  :  TB AND FB {$$ = check_type($1, $3); 
					printf("\tpop rax\n\
\tpop rbx\n\
\tand rax, rbx\n\
\tpush rax\n");}	
    |  FB 
    ;
FB  :  FB EQ M {{$$ = check_type($1, $3); 
		printf("\tpop rcx\n"); 
		printf("\tpop rax\n");}
		if (!strcmp($2, "==")) {
            printf("\tcmp rax, rcx\n");
            zlabel("\tjne jump_label" , jump_label ++ ,""); 
			write_test();}
		else if (!strcmp($2, "!=")) {
            printf("\tcmp rax, rcx\n");
            zlabel("\tje jump_label" , jump_label ++ ,""); 
			write_test();}
    }
    |  M 
    ;
M   :  M ORDER E {$$ =check_type($1, $3); printf("\tpop rcx\n"); printf("\tpop rax\n"); 
					if (!strcmp($2, ">")) {
						printf("\tcmp rax, rcx\n");
						zlabel("\tjle jump_label" , jump_label ++ ,""); 
						write_test();}
					else if (!strcmp($2, "<=")) {
						printf("\tcmp rax, rcx\n");
						zlabel("\tjg jump_label" , jump_label ++ ,""); 
						write_test();}
					else if (!strcmp($2, "<")) {
						printf("\tcmp rax, rcx\n");
						zlabel("\tjge jump_label" , jump_label ++ ,""); 
						write_test();}
					else if (!strcmp($2, ">=")) {
						printf("\tcmp rax, rcx\n");
						zlabel("\tjl jump_label" , jump_label ++ ,""); 
						write_test();}
				} 
    |  E 
    ;
E   :  E ADDSUB T {$$ = check_type($1, $3); 
						switch ($2) {
							case '+': pre_calcul($1, $3); write_add(); break;
							case '-': pre_calcul($1, $3); write_sub(); break;
							default: yyerror("unknown operand");
						}
					}
    |  T 
    ;    
T   : T DIVSTAR F {$$ = check_type($1, $3);  
						switch ($2) { 
							case '*': pre_calcul($1, $3); write_mul(); break; 
							case '/': pre_calcul($1, $3); write_div(); break; 
							default: yyerror("unknow operand"); 
						} 
					}
    | F 
    ;
F   :  ADDSUB F {check_type($2, ENT); if ($1 == '-') {write_opposite();}}
    |  '!' F {check_type($2, ENT);}
    |  '(' Exp ')' {$$ = $2;}
    |  LValue {$$ = get_type($1);}
    |  NUM {$$ = ENT; write_push($1); }
    |  CARACTERE {$$ = CAR; write_push($1); }
    |  IDENT {if (i_arg >= MAX_WORD_SIZE) {perror("Too much nested functions calls"); /* TODO realloc */} 
		i_arg += 1; argcount[i_arg] = 0; strcpy(fctname[i_arg], $1);} '(' Arguments  ')' {$$ = get_type($1); check_argcount($1); write_call($1); printf("\tpush rax\n");}
    ;
LValue:
       IDENT {lookup_var($1); if (is_constant($1)) write_push_cst($1); else write_push_var($1);}
    |  IDENT  '[' Exp  ']' {lookup_var($1); printf("\tpop rax\n"); if (is_constant($1)) write_push_cst($1); else write_push_var_tab($1);}
    ;
Arguments:
       ListExp
    | ;
ListExp:
       ListExp ',' Exp {argcount[i_arg] += 1; check_argtype($3);}
    |  Exp {argcount[i_arg] += 1; check_argtype($1);}
    ;
%%

int main(void){
	STallocate();
	STFallocate();
	yyparse();
	//display_tables();
	STfree();
	STFfree();
	write_end();
	return 0;
}

void yyerror(char *s){
	fprintf(stderr, "yyerror : %s near line %d\n", s, lineno);
	exit(1);
}
