/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 1 "tcompil.y" /* yacc.c:339  */

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


#line 959 "tcompil.tab.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "tcompil.tab.h".  */
#ifndef YY_YY_TCOMPIL_TAB_H_INCLUDED
# define YY_YY_TCOMPIL_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    CARACTERE = 258,
    NUM = 259,
    IDENT = 260,
    ORDER = 261,
    EQ = 262,
    ADDSUB = 263,
    DIVSTAR = 264,
    TYPE = 265,
    OR = 266,
    AND = 267,
    CONST = 268,
    IF = 269,
    WHILE = 270,
    RETURN = 271,
    VOID = 272,
    READE = 273,
    READC = 274,
    PRINT = 275,
    ELSE = 276
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 894 "tcompil.y" /* yacc.c:355  */

	char caractere;
	int num;
	char ident[64];
	char type[16];
	char comp[3];
	char addsub;
	char divstar;

#line 1031 "tcompil.tab.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_TCOMPIL_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 1048 "tcompil.tab.c" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   171

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  32
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  36
/* YYNRULES -- Number of rules.  */
#define YYNRULES  74
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  148

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   276

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    31,     2,     2,     2,     2,     2,     2,
      28,    22,     2,     2,    21,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    24,
       2,    25,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    26,     2,    27,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    29,     2,    30,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    23
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   920,   920,   923,   924,   926,   927,   930,   931,   934,
     935,   938,   939,   941,   942,   945,   946,   949,   950,   953,
     956,   956,   965,   965,   976,   977,   980,   981,   983,   986,
     987,   990,   993,   996,   999,  1003,  1004,  1005,  1006,  1007,
    1008,  1009,  1010,  1011,  1012,  1012,  1014,  1016,  1031,  1033,
    1038,  1040,  1045,  1047,  1059,  1061,  1079,  1081,  1088,  1090,
    1097,  1099,  1100,  1101,  1102,  1103,  1104,  1105,  1105,  1109,
    1110,  1113,  1114,  1116,  1117
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "CARACTERE", "NUM", "IDENT", "ORDER",
  "EQ", "ADDSUB", "DIVSTAR", "TYPE", "OR", "AND", "CONST", "IF", "WHILE",
  "RETURN", "VOID", "READE", "READC", "PRINT", "','", "')'", "ELSE", "';'",
  "'='", "'['", "']'", "'('", "'{'", "'}'", "'!'", "$accept", "Prog",
  "DeclConsts", "ListConst", "Litteral", "NombreSigne", "DeclVars",
  "Declarateurs", "Declarateur", "DeclFoncts", "DeclFonct", "EnTeteFonct",
  "$@1", "$@2", "Parametres", "ListTypVar", "Corps", "SuiteInstr",
  "DEBUTIF", "DEBUTELSE", "ENDIF", "DEBUTWHILE", "Instr", "$@3", "Exp",
  "EB", "TB", "FB", "M", "E", "T", "F", "$@4", "LValue", "Arguments",
  "ListExp", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,    44,    41,   276,    59,    61,    91,    93,    40,   123,
     125,    33
};
# endif

#define YYPACT_NINF -63

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-63)))

#define YYTABLE_NINF -68

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -63,    29,    20,   -63,    30,    -3,    31,   -18,    52,    59,
      -2,   -63,    38,    17,    61,   -63,   -24,    10,   -63,    41,
      75,   -63,   -63,   -63,   -63,   -63,    78,   -63,   -63,    62,
      86,   -63,    93,   -63,   -63,    72,    20,   -63,    17,    76,
      45,    81,   -63,    45,    98,   -63,   -63,   104,   -63,    89,
      92,    90,    93,     8,   -63,   -63,   105,   -63,   -63,   -63,
      -9,    91,    37,    88,    95,    96,   -63,    91,   -63,   -63,
      91,   100,   102,   -63,    94,   109,   113,   114,   120,   119,
     121,   -63,   103,   124,    91,   106,   -63,   -63,   -63,   107,
     127,   128,    91,   115,    55,   -63,   108,   110,   -63,    91,
      91,    91,    91,    91,    91,    91,   -63,   112,    91,   -63,
     118,   122,   123,   -63,   -63,    91,    91,   113,   114,   120,
     119,   121,   -63,   -63,   -63,   -63,   125,   129,   111,   117,
     130,   126,   131,   -63,    91,   -63,   -63,   -63,   -63,   -63,
     -63,    73,    73,   132,   -63,   -63,    73,   -63
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       4,     0,    12,     1,     0,     0,     0,     0,     0,     0,
       2,    18,     0,     0,     0,     3,    15,     0,    14,     0,
       0,    17,     4,    19,     8,     9,     0,     6,     7,     0,
       0,    20,     0,    11,    22,     0,    12,    10,     0,     0,
       0,    15,    13,     0,    30,     5,    16,     0,    24,     0,
      25,     0,     0,    31,    27,    21,     0,    23,    66,    65,
      69,     0,     0,     0,     0,     0,    36,     0,    30,    28,
       0,     0,     0,    29,     0,    48,    50,    52,    54,    56,
      58,    60,    64,     0,     0,     0,    61,    64,    38,     0,
       0,     0,     0,     0,    31,    62,     0,     0,    35,     0,
       0,     0,     0,     0,     0,     0,    26,     0,    72,    37,
       0,     0,     0,    63,    46,     0,     0,    49,    51,    53,
      55,    57,    59,    47,    70,    74,     0,    71,     0,     0,
       0,     0,     0,    68,     0,    39,    40,    41,    33,    44,
      73,    31,    31,    42,    45,    32,    31,    43
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -63,   -63,   134,   -63,   133,   -63,   116,   -63,   135,   -63,
     136,   -63,   -63,   -63,    99,   -63,   -63,    83,   -63,   -63,
     -63,   -63,   -36,   -63,   -62,   -63,    44,    49,    56,    57,
      58,   -60,   -63,   -52,   -63,   -63
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,     7,    27,    28,     5,    17,    18,    10,
      11,    12,    40,    43,    49,    50,    23,    53,    71,   146,
     141,    72,    73,   142,    74,    75,    76,    77,    78,    79,
      80,    81,    85,    82,   126,   127
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      89,    86,    30,    14,    31,    93,    15,     8,    20,    87,
      95,    58,    59,    60,     9,     9,    61,    84,    87,   -67,
      24,    25,   107,   -34,    62,    26,    63,    64,    65,     3,
     112,    32,    66,     4,    33,     6,    67,    68,    69,    70,
      58,    59,    60,   123,   122,    61,   125,    87,    87,    87,
      87,    87,    87,   131,   132,    47,    13,    16,    58,    59,
      60,    88,    48,    61,    19,    67,    29,    22,    70,    34,
     -34,    62,   140,    63,    64,    65,    58,    59,    60,    66,
      35,    61,    37,    67,    68,   114,    70,    38,   -34,    62,
      39,    63,    64,    65,    58,    59,    60,    66,    41,    61,
      31,    67,    68,    46,    70,   143,   144,    30,    52,    54,
     147,    55,    57,    56,    96,    83,    90,    97,    98,    67,
      99,   101,    70,    91,    92,   100,   102,   103,   105,   106,
     104,   109,   110,   111,   108,   135,   115,   113,   116,   124,
     128,   136,    51,   117,   129,   130,    21,   133,   138,   118,
     134,    94,    44,   139,   137,   145,    36,   119,     0,   120,
       0,   121,     0,     0,     0,     0,     0,    42,     0,     0,
       0,    45
};

static const yytype_int16 yycheck[] =
{
      62,    61,    26,    21,    28,    67,    24,    10,    10,    61,
      70,     3,     4,     5,    17,    17,     8,    26,    70,    28,
       3,     4,    84,    15,    16,     8,    18,    19,    20,     0,
      92,    21,    24,    13,    24,     5,    28,    29,    30,    31,
       3,     4,     5,   105,   104,     8,   108,    99,   100,   101,
     102,   103,   104,   115,   116,    10,    25,     5,     3,     4,
       5,    24,    17,     8,     5,    28,     5,    29,    31,    28,
      15,    16,   134,    18,    19,    20,     3,     4,     5,    24,
       5,     8,     4,    28,    29,    30,    31,    25,    15,    16,
       4,    18,    19,    20,     3,     4,     5,    24,     5,     8,
      28,    28,    29,    27,    31,   141,   142,    26,    10,     5,
     146,    22,    22,    21,    14,    10,    28,    15,    24,    28,
      11,     7,    31,    28,    28,    12,     6,     8,    25,     5,
       9,    24,     5,     5,    28,    24,    28,    22,    28,    27,
      22,    24,    43,    99,    22,    22,    10,    22,    22,   100,
      21,    68,    36,    22,    24,    23,    22,   101,    -1,   102,
      -1,   103,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
      -1,    38
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    33,    34,     0,    13,    38,     5,    35,    10,    17,
      41,    42,    43,    25,    21,    24,     5,    39,    40,     5,
      10,    42,    29,    48,     3,     4,     8,    36,    37,     5,
      26,    28,    21,    24,    28,     5,    34,     4,    25,     4,
      44,     5,    40,    45,    38,    36,    27,    10,    17,    46,
      47,    46,    10,    49,     5,    22,    21,    22,     3,     4,
       5,     8,    16,    18,    19,    20,    24,    28,    29,    30,
      31,    50,    53,    54,    56,    57,    58,    59,    60,    61,
      62,    63,    65,    10,    26,    64,    63,    65,    24,    56,
      28,    28,    28,    56,    49,    63,    14,    15,    24,    11,
      12,     7,     6,     8,     9,    25,     5,    56,    28,    24,
       5,     5,    56,    22,    30,    28,    28,    58,    59,    60,
      61,    62,    63,    56,    27,    56,    66,    67,    22,    22,
      22,    56,    56,    22,    21,    24,    24,    24,    22,    22,
      56,    52,    55,    54,    54,    23,    51,    54
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    32,    33,    34,    34,    35,    35,    36,    36,    37,
      37,    38,    38,    39,    39,    40,    40,    41,    41,    42,
      44,    43,    45,    43,    46,    46,    47,    47,    48,    49,
      49,    50,    51,    52,    53,    54,    54,    54,    54,    54,
      54,    54,    54,    54,    55,    54,    54,    56,    56,    57,
      57,    58,    58,    59,    59,    60,    60,    61,    61,    62,
      62,    63,    63,    63,    63,    63,    63,    64,    63,    65,
      65,    66,    66,    67,    67
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     3,     4,     0,     5,     3,     1,     1,     1,
       2,     4,     0,     3,     1,     1,     4,     2,     1,     2,
       0,     6,     0,     6,     1,     1,     4,     2,     5,     2,
       0,     0,     0,     0,     0,     2,     1,     3,     2,     5,
       5,     5,     7,    10,     0,     7,     3,     3,     1,     3,
       1,     3,     1,     3,     1,     3,     1,     3,     1,     3,
       1,     2,     2,     3,     1,     1,     1,     0,     5,     1,
       4,     1,     0,     3,     1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 5:
#line 926 "tcompil.y" /* yacc.c:1646  */
    {fprintf(stderr, "AddCst1 : %s\n", (yyvsp[-2].ident)); addCst((yyvsp[-2].ident), (yyvsp[0].num));}
#line 2229 "tcompil.tab.c" /* yacc.c:1646  */
    break;

  case 6:
#line 927 "tcompil.y" /* yacc.c:1646  */
    {fprintf(stderr, "AddCst2 : %s = %d\n", (yyvsp[-2].ident), (yyvsp[0].num)); addCst((yyvsp[-2].ident), (yyvsp[0].num));}
#line 2235 "tcompil.tab.c" /* yacc.c:1646  */
    break;

  case 7:
#line 930 "tcompil.y" /* yacc.c:1646  */
    {(yyval.num) = (yyvsp[0].num);}
#line 2241 "tcompil.tab.c" /* yacc.c:1646  */
    break;

  case 8:
#line 931 "tcompil.y" /* yacc.c:1646  */
    {(yyval.num) = CAR; constval = (yyvsp[0].caractere);}
#line 2247 "tcompil.tab.c" /* yacc.c:1646  */
    break;

  case 9:
#line 934 "tcompil.y" /* yacc.c:1646  */
    {(yyval.num) = ENT; constval = (yyvsp[0].num);}
#line 2253 "tcompil.tab.c" /* yacc.c:1646  */
    break;

  case 10:
#line 935 "tcompil.y" /* yacc.c:1646  */
    {(yyval.num) = ENT; if ((yyvsp[-1].addsub) == '+') constval = (yyvsp[0].num); else constval = -((yyvsp[0].num));}
#line 2259 "tcompil.tab.c" /* yacc.c:1646  */
    break;

  case 11:
#line 938 "tcompil.y" /* yacc.c:1646  */
    {strcpy((yyvsp[-4].type), (yyvsp[-2].type));}
#line 2265 "tcompil.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 945 "tcompil.y" /* yacc.c:1646  */
    {addVar((yyvsp[0].ident), (yyvsp[-1].type), 1);}
#line 2271 "tcompil.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 946 "tcompil.y" /* yacc.c:1646  */
    {addVar((yyvsp[-3].ident), (yyvsp[-4].type), (yyvsp[-1].num));}
#line 2277 "tcompil.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 956 "tcompil.y" /* yacc.c:1646  */
    {
		   	if (!start_flag) {
		   		start_flag = 1; write_start();
		   	} 
		   	addFct((yyvsp[-2].type), (yyvsp[-1].ident));
	    }
#line 2288 "tcompil.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 962 "tcompil.y" /* yacc.c:1646  */
    {
	   		write_fct((yyvsp[-4].ident)); 
		}
#line 2296 "tcompil.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 965 "tcompil.y" /* yacc.c:1646  */
    {
		   	if (!start_flag) {
		   		start_flag = 1; write_start();
		   	} 
		   	addFct("void", (yyvsp[-1].ident));
	    }
#line 2307 "tcompil.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 971 "tcompil.y" /* yacc.c:1646  */
    {
	   		write_fct((yyvsp[-4].ident)); 
		}
#line 2315 "tcompil.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 976 "tcompil.y" /* yacc.c:1646  */
    {addParam("void", "");}
#line 2321 "tcompil.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 980 "tcompil.y" /* yacc.c:1646  */
    {addParam((yyvsp[-1].type), (yyvsp[0].ident));}
#line 2327 "tcompil.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 981 "tcompil.y" /* yacc.c:1646  */
    {addParam((yyvsp[-1].type), (yyvsp[0].ident));}
#line 2333 "tcompil.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 983 "tcompil.y" /* yacc.c:1646  */
    {STpop();}
#line 2339 "tcompil.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 990 "tcompil.y" /* yacc.c:1646  */
    {zlabel("\nstart_if",(yyval.num)=if_label," :");}
#line 2345 "tcompil.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 993 "tcompil.y" /* yacc.c:1646  */
    {zlabel("\tjmp exit_if" ,if_label + 1,""); zlabel("\nexit_if",if_label ++," :");}
#line 2351 "tcompil.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 996 "tcompil.y" /* yacc.c:1646  */
    {printf("\tpop rax\n");printf("\tcmp rax, 1\n"); zlabel("\tjne exit_if" , if_label + 1 ,""); zlabel("\tjmp exit_if" , if_label ,"");zlabel("\nexit_if",if_label ++," :");}
#line 2357 "tcompil.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 999 "tcompil.y" /* yacc.c:1646  */
    {zlabel("\nstart_while",while_label++," :");}
#line 2363 "tcompil.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 1003 "tcompil.y" /* yacc.c:1646  */
    {if ((yyvsp[-1].num) >= 0) {write_pop();}}
#line 2369 "tcompil.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 1005 "tcompil.y" /* yacc.c:1646  */
    {printf("\tpop rax\n"); write_jmpend(); check_type(STF.table[STF.size - 1].type, (yyvsp[-1].num)); return_flag = 1;}
#line 2375 "tcompil.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 1006 "tcompil.y" /* yacc.c:1646  */
    {write_jmpend(); check_type(STF.table[STF.size - 1].type, VID); fprintf(stderr, "%d / %d\n", STF.table[STF.size - 1].type, VID); return_flag = 1;}
#line 2381 "tcompil.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 1007 "tcompil.y" /* yacc.c:1646  */
    {lookup_var((yyvsp[-2].ident)); read(get_entry((yyvsp[-2].ident)).addr,get_type((yyvsp[-2].ident)));}
#line 2387 "tcompil.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 1008 "tcompil.y" /* yacc.c:1646  */
    {lookup_var((yyvsp[-2].ident)); read(get_entry((yyvsp[-2].ident)).addr,get_type((yyvsp[-2].ident)));}
#line 2393 "tcompil.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 1009 "tcompil.y" /* yacc.c:1646  */
    {print((yyvsp[-2].num));}
#line 2399 "tcompil.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 1010 "tcompil.y" /* yacc.c:1646  */
    {if((yyvsp[-3].num) % 2 != 0) {printf("\tpop rax");} zlabel("\nexit_if" ,if_label," :");}
#line 2405 "tcompil.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 1011 "tcompil.y" /* yacc.c:1646  */
    {zlabel("\nexit_if", if_label ++," :");if((yyvsp[-6].num) % 2 != 0) {printf("\n\tpop rax\n");}}
#line 2411 "tcompil.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 1012 "tcompil.y" /* yacc.c:1646  */
    {printf("\tpop rax\n");printf("\tcmp rax, 1\n"); zlabel("\tjne exit_while" , while_label - 1 ,"");}
#line 2417 "tcompil.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 1013 "tcompil.y" /* yacc.c:1646  */
    {zlabel("\tjmp start_while",while_label-- -1 ,""); zlabel("\nexit_while" ,while_label ," :");}
#line 2423 "tcompil.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 1016 "tcompil.y" /* yacc.c:1646  */
    {(yyval.num) = get_type((yyvsp[-2].ident)); 
						check_type(get_type((yyvsp[-2].ident)), (yyvsp[0].num));
						lookup_var((yyvsp[-2].ident)); 
						if ((yyval.num) % 2 == 0 ){
							if (is_constant((yyvsp[-2].ident)))
								yyerror("assigning constant value");
							else 
								write_assign(get_entry((yyvsp[-2].ident)));} 
						else {
							printf("\tpop rax\n\
\tpop rbx\n\
\tpop rcx\n\
\tpush rax\n\
\tpop QWORD [rcx]\n\
\tpush rax\n");}}
#line 2443 "tcompil.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 1033 "tcompil.y" /* yacc.c:1646  */
    {(yyval.num) = check_type((yyvsp[-2].num), (yyvsp[0].num)); 
					printf("\tpop rax\n\
\tpop rbx\n\
\tor rax, rbx\n\
\tpush rax\n");}
#line 2453 "tcompil.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 1040 "tcompil.y" /* yacc.c:1646  */
    {(yyval.num) = check_type((yyvsp[-2].num), (yyvsp[0].num)); 
					printf("\tpop rax\n\
\tpop rbx\n\
\tand rax, rbx\n\
\tpush rax\n");}
#line 2463 "tcompil.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 1047 "tcompil.y" /* yacc.c:1646  */
    {{(yyval.num) = check_type((yyvsp[-2].num), (yyvsp[0].num)); 
		printf("\tpop rcx\n"); 
		printf("\tpop rax\n");}
		if (!strcmp((yyvsp[-1].comp), "==")) {
            printf("\tcmp rax, rcx\n");
            zlabel("\tjne jump_label" , jump_label ++ ,""); 
			write_test();}
		else if (!strcmp((yyvsp[-1].comp), "!=")) {
            printf("\tcmp rax, rcx\n");
            zlabel("\tje jump_label" , jump_label ++ ,""); 
			write_test();}
    }
#line 2480 "tcompil.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 1061 "tcompil.y" /* yacc.c:1646  */
    {(yyval.num) =check_type((yyvsp[-2].num), (yyvsp[0].num)); printf("\tpop rcx\n"); printf("\tpop rax\n"); 
					if (!strcmp((yyvsp[-1].comp), ">")) {
						printf("\tcmp rax, rcx\n");
						zlabel("\tjle jump_label" , jump_label ++ ,""); 
						write_test();}
					else if (!strcmp((yyvsp[-1].comp), "<=")) {
						printf("\tcmp rax, rcx\n");
						zlabel("\tjg jump_label" , jump_label ++ ,""); 
						write_test();}
					else if (!strcmp((yyvsp[-1].comp), "<")) {
						printf("\tcmp rax, rcx\n");
						zlabel("\tjge jump_label" , jump_label ++ ,""); 
						write_test();}
					else if (!strcmp((yyvsp[-1].comp), ">=")) {
						printf("\tcmp rax, rcx\n");
						zlabel("\tjl jump_label" , jump_label ++ ,""); 
						write_test();}
				}
#line 2503 "tcompil.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 1081 "tcompil.y" /* yacc.c:1646  */
    {(yyval.num) = check_type((yyvsp[-2].num), (yyvsp[0].num)); 
						switch ((yyvsp[-1].addsub)) {
							case '+': pre_calcul((yyvsp[-2].num), (yyvsp[0].num)); write_add(); break;
							case '-': pre_calcul((yyvsp[-2].num), (yyvsp[0].num)); write_sub(); break;
							default: yyerror("unknown operand");
						}
					}
#line 2515 "tcompil.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 1090 "tcompil.y" /* yacc.c:1646  */
    {(yyval.num) = check_type((yyvsp[-2].num), (yyvsp[0].num));  
						switch ((yyvsp[-1].divstar)) { 
							case '*': pre_calcul((yyvsp[-2].num), (yyvsp[0].num)); write_mul(); break; 
							case '/': pre_calcul((yyvsp[-2].num), (yyvsp[0].num)); write_div(); break; 
							default: yyerror("unknow operand"); 
						} 
					}
#line 2527 "tcompil.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 1099 "tcompil.y" /* yacc.c:1646  */
    {check_type((yyvsp[0].num), ENT); if ((yyvsp[-1].addsub) == '-') {write_opposite();}}
#line 2533 "tcompil.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 1100 "tcompil.y" /* yacc.c:1646  */
    {check_type((yyvsp[0].num), ENT);}
#line 2539 "tcompil.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 1101 "tcompil.y" /* yacc.c:1646  */
    {(yyval.num) = (yyvsp[-1].num);}
#line 2545 "tcompil.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 1102 "tcompil.y" /* yacc.c:1646  */
    {(yyval.num) = get_type((yyvsp[0].ident));}
#line 2551 "tcompil.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 1103 "tcompil.y" /* yacc.c:1646  */
    {(yyval.num) = ENT; write_push((yyvsp[0].num)); }
#line 2557 "tcompil.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 1104 "tcompil.y" /* yacc.c:1646  */
    {(yyval.num) = CAR; write_push((yyvsp[0].caractere)); }
#line 2563 "tcompil.tab.c" /* yacc.c:1646  */
    break;

  case 67:
#line 1105 "tcompil.y" /* yacc.c:1646  */
    {if (i_arg >= MAX_WORD_SIZE) {perror("Too much nested functions calls"); /* TODO realloc */} 
		i_arg += 1; argcount[i_arg] = 0; strcpy(fctname[i_arg], (yyvsp[0].ident));}
#line 2570 "tcompil.tab.c" /* yacc.c:1646  */
    break;

  case 68:
#line 1106 "tcompil.y" /* yacc.c:1646  */
    {(yyval.num) = get_type((yyvsp[-4].ident)); check_argcount((yyvsp[-4].ident)); write_call((yyvsp[-4].ident)); printf("\tpush rax\n");}
#line 2576 "tcompil.tab.c" /* yacc.c:1646  */
    break;

  case 69:
#line 1109 "tcompil.y" /* yacc.c:1646  */
    {lookup_var((yyvsp[0].ident)); if (is_constant((yyvsp[0].ident))) write_push_cst((yyvsp[0].ident)); else write_push_var((yyvsp[0].ident));}
#line 2582 "tcompil.tab.c" /* yacc.c:1646  */
    break;

  case 70:
#line 1110 "tcompil.y" /* yacc.c:1646  */
    {lookup_var((yyvsp[-3].ident)); printf("\tpop rax\n"); if (is_constant((yyvsp[-3].ident))) write_push_cst((yyvsp[-3].ident)); else write_push_var_tab((yyvsp[-3].ident));}
#line 2588 "tcompil.tab.c" /* yacc.c:1646  */
    break;

  case 73:
#line 1116 "tcompil.y" /* yacc.c:1646  */
    {argcount[i_arg] += 1; check_argtype((yyvsp[0].num));}
#line 2594 "tcompil.tab.c" /* yacc.c:1646  */
    break;

  case 74:
#line 1117 "tcompil.y" /* yacc.c:1646  */
    {argcount[i_arg] += 1; check_argtype((yyvsp[0].num));}
#line 2600 "tcompil.tab.c" /* yacc.c:1646  */
    break;


#line 2604 "tcompil.tab.c" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 1119 "tcompil.y" /* yacc.c:1906  */


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
