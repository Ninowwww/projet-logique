#ifndef MENU_H
#define MENU_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

// Déclarations des fonctions

/**
 * Ajoute des indices à une grille partielle.
 * @param partielle_valide Grille partielle à compléter.
 * @param grille_valide Grille solution complète.
 * @param N Taille de la grille.
 */
void afficher_indice(int **partielle_valide, int **grille_valide, int N);

/**
 * Teste les performances sur des grilles valides et invalides.
 * @param N Taille de la grille.
 * @param densite Densité des indices dans la grille partielle.
 */
void test_perf(int N, float densite);

/**
 * Permet à l'utilisateur de résoudre une grille en affichant des indices ou la solution.
 * @param N Taille de la grille.
 * @param densite Densité de la grille partielle.
 */
void test_grille(int N, float densite);

int verifier_grille_humaine(int **grille, int N);
/**
 * Permet à l'utilisateur d'entrer manuellement une grille.
 * @param N Taille de la grille.
 * @return Grille remplie par l'utilisateur.
 */
int **grille_humaine(int N);

#endif