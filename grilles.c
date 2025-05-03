#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "grilles.h"
#include "regles.h"

// Initialise une grille NxN avec toutes les cases à -1
int **initialiser_grille(int N) {
    int **g = malloc(N * sizeof(int *));
    for (int i = 0; i < N; i++) {
        g[i] = malloc(N * sizeof(int));
        for (int j = 0; j < N; j++) g[i][j] = -1;
    }
    return g;
}

// Libère la mémoire allouée à une grille NxN
void liberer_grille(int **g, int N) {
    for (int i = 0; i < N; i++) free(g[i]);
    free(g);
}

// Mélange aléatoirement les éléments d'une ligne
void melanger(int *ligne, int N) {
    for (int i = N - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int tmp = ligne[i];
        ligne[i] = ligne[j];
        ligne[j] = tmp;
    }
}

// Remplit la grille ligne par ligne tout en respectant les règles du jeu
int remplir_grille(int **g, int N, int ligne_actuelle) {
    if (ligne_actuelle == N) return 1;  // grille complète

    // Génère une ligne de base équilibrée avec moitié de 0 et moitié de 1
    int base[N];
    for (int i = 0; i < N / 2; i++) base[i] = 0;
    for (int i = N / 2; i < N; i++) base[i] = 1;

    // Tente de placer une ligne valide jusqu'à 1000 fois
    for (int essais = 1000; essais > 0; essais--) {
        melanger(base, N);
        if (!ligne_valide(base, N) || ligne_dupliquee(g, base, ligne_actuelle, N)) continue;

        int ok = 1;
        // Place temporairement la ligne et vérifie la validité des colonnes
        for (int col = 0; col < N; col++) {
            g[ligne_actuelle][col] = base[col];
            if (!colonne_valide(g, N, col, ligne_actuelle)) {
                ok = 0;
                break;
            }
        }

        // Vérifie l'unicité des colonnes et passe à la ligne suivante
        if (ok && !colonnes_dupliquees(g, N) && remplir_grille(g, N, ligne_actuelle + 1)) return 1;

        // Si échec, on réinitialise la ligne
        for (int col = 0; col < N; col++) g[ligne_actuelle][col] = -1;
    }

    return 0;  // impossible de remplir la grille à partir de cette configuration
}

// Crée une grille complètement remplie et valide selon les règles
int **creer_grille_valide(int N) {
    int **g = initialiser_grille(N);
    if (remplir_grille(g, N, 0)) return g;
    liberer_grille(g, N);
    return NULL;
}

// Crée une grille aléatoire invalide en s'assurant qu'elle enfreint au moins une règle
int **creer_grille_invalide(int N) {
    int **g = NULL;
    for (int tentative = 0; tentative < 100; tentative++) {
        if (g) liberer_grille(g, N);
        g = initialiser_grille(N);

        // Remplit la grille aléatoirement
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                g[i][j] = rand() % 2;

        // Vérifie si elle est invalide (enfreint au moins une règle)
        if (!(verifier_parite(g, N) && verifier_consecutive(g, N) && verifier_unicite(g, N)))
            return g;
    }
    if (g) liberer_grille(g, N);
    return NULL;
}

// Crée une version partiellement remplie d'une grille valide selon une densité donnée
int **creer_grille_partielle_valide(int **grille_complete, int N, float densite) {
    int **partielle = initialiser_grille(N);
    for (int i = 0; i < N; i++)
        memcpy(partielle[i], grille_complete[i], N * sizeof(int));

    int total_cases = N * N;
    int cases_a_supprimer = (int)((1.0 - densite) * total_cases);

    // Supprime aléatoirement des cases pour créer une grille partielle
    while (cases_a_supprimer > 0) {
        int i = rand() % N;
        int j = rand() % N;
        if (partielle[i][j] != -1) {
            partielle[i][j] = -1;
            cases_a_supprimer--;
        }
    }

    return partielle;
}

// Crée une grille partielle invalide : on part d'une grille valide et on injecte une erreur
int **creer_grille_partielle_invalide(int N, float densite) {
    int **grille = creer_grille_valide(N);  // on part d'une grille valide
    int **partielle = initialiser_grille(N);

    // Génère une version partielle avec la densité demandée
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++) {
            float p = (float)rand() / (float)RAND_MAX;
            partielle[i][j] = (p < densite) ? grille[i][j] : -1;
        }

    // Injection volontaire d'une erreur : trois 1 consécutifs dans une ligne
    int ligne = rand() % N;
    int col = rand() % (N - 2);
    partielle[ligne][col] = 1;
    partielle[ligne][col + 1] = 1;
    partielle[ligne][col + 2] = 1;

    liberer_grille(grille, N);
    return partielle;
}


// Affichage d'une grille : -1 => '.', sinon affiche la valeur binaire
void afficher_grille(int **grille, int N) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (grille[i][j] == -1)
                printf(". ");
            else
                printf("%d ", grille[i][j]);
        }
        printf("\n");
    }
}

// Vérifie la validité d'une grille selon 3 règles
int verifier_grille(int **grille, int N) {
    return verifier_consecutive(grille, N) &&
           verifier_parite(grille, N) &&
           verifier_unicite(grille, N);
}