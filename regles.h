#ifndef REGLES_H
#define REGLES_H

int verifier_consecutive(int **grid, int N);
int verifier_parite(int **grid, int N);
int ligne_valide(const int *ligne, int N);
int colonne_valide(int **g, int N, int col, int ligne_actuelle);
int ligne_dupliquee(int **g, const int *ligne, int nb_lignes, int N);
int colonnes_dupliquees(int **g, int N);
int verifier_unicite(int **grid, int N);

#endif