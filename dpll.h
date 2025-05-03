#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>

typedef struct clauses_grille_solution_ {
    int** clauses;
    int** grille_solution;
    int* validite; 
} clauses_grille_solution;

int** lire_clauses(char* nom_fichier, int* nb_clauses, int* dimension);

void afficher_clauses(int** ensemble_clauses, int nb_clauses, int dimension);

clauses_grille_solution algo_dpll(int** clauses, int nb_clauses, int dimension);

void algo_dpll_rec(clauses_grille_solution clauses_grille, int nb_clauses, int dimension);

void assigner_valeur_litteral(clauses_grille_solution clauses_grille, int litteral, int nb_clauses, int dimension);

int est_isoler(int** clauses, int litteral, int nb_clauses);

int** copier_clauses(int** clauses, int nb_clauses, int dimension);

clauses_grille_solution copier_clauses_grille(clauses_grille_solution original, int nb_clauses, int dimension);