#ifndef DIMACS_H
#define DIMACS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "grilles.h"
#include "regles.h"

/**
 * Ajoute les contraintes d’unicité entre lignes et colonnes pour une grille donnée.
 * @param f Fichier dans lequel écrire les clauses.
 * @param grid Grille du puzzle, linéarisée (taille N * N), avec -1 pour les cases vides.
 * @param nb_clauses Pointeur vers le nombre de clauses, qui sera incrémenté.
 */
void ajouter_contraintes_unicite(FILE *f, int **grid, int N, int *nb_clauses);

/**
 * Ajoute des contraintes de parité (limitation du nombre de 1 ou 0).
 * @param f Fichier dans lequel écrire les clauses.
 * @param indices Tableau d’indices des variables.
 * @param taille Taille du tableau indices.
 * @param nb_clauses Pointeur vers le nombre de clauses, qui sera incrémenté.
 * @param valeur 1 pour interdire trop de 1, 0 pour interdire trop de 0.
 */
void ajouter_parite(FILE *f, int *indices, int taille, int *nb_clauses, int valeur);

/**
 * Génère et écrit toutes les contraintes DIMACS à partir d'une grille.
 * @param f Fichier de sortie au format DIMACS.
 * @param grid Grille initiale linéarisée (taille N * N), avec 1, 0 ou -1 (inconnu).
 */
void ecrire_grille_dimacs(FILE *f, int **grid, int N) ;


int affichage(const char *nom_fichier,int N ); 

#endif // DIMACS_H