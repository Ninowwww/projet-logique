#include "regles.h"

/// Vérifie qu'aucune ligne ou colonne ne contient trois valeurs identiques consécutives (0 ou 1)
int verifier_consecutive(int **grid, int N) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N - 2; j++) {
            // Vérifie ligne : trois cases consécutives identiques
            int a = grid[i][j], b = grid[i][j+1], c = grid[i][j+2];
            if (a != -1 && a == b && a == c) return 0;

            // Vérifie colonne : trois cases consécutives identiques
            a = grid[j][i], b = grid[j+1][i], c = grid[j+2][i];
            if (a != -1 && a == b && a == c) return 0;
        }
    }
    return 1;
}

/// Vérifie que chaque ligne et colonne contient un nombre équilibré de 0 et de 1
int verifier_parite(int **grid, int N) {
    for (int i = 0; i < N; i++) {
        int ligne_0 = 0, ligne_1 = 0, col_0 = 0, col_1 = 0;
        for (int j = 0; j < N; j++) {
            // Comptage pour la ligne
            if (grid[i][j] == 0) ligne_0++;
            else if (grid[i][j] == 1) ligne_1++;

            // Comptage pour la colonne
            if (grid[j][i] == 0) col_0++;
            else if (grid[j][i] == 1) col_1++;
        }

        // Si la ligne ou colonne est complète et déséquilibrée, grille invalide
        if ((ligne_0 + ligne_1 == N && ligne_0 != ligne_1) ||
            (col_0 + col_1 == N && col_0 != col_1)) {
            return 0;
        }
    }
    return 1;
}

/// Vérifie qu'une ligne donnée respecte les règles :
/// pas de trois identiques consécutifs et max N/2 de chaque valeur
int ligne_valide(const int *ligne, int N) {
    int count0 = 0, count1 = 0;
    for (int i = 0; i < N; i++) {
        if (ligne[i] == 0) count0++;
        else if (ligne[i] == 1) count1++;

        // Trois identiques consécutifs
        if (i >= 2 && ligne[i] == ligne[i - 1] && ligne[i] == ligne[i - 2]) return 0;
    }

    // Trop de 0 ou 1 ?
    return (count0 <= N / 2 && count1 <= N / 2);
}

/// Vérifie qu'une colonne jusqu'à une certaine ligne respecte les règles :
/// pas de trois identiques consécutifs, max N/2 de chaque valeur
int colonne_valide(int **g, int N, int col, int ligne_actuelle) {
    int count0 = 0, count1 = 0;
    for (int i = 0; i <= ligne_actuelle; i++) {
        if (g[i][col] == 0) count0++;
        else if (g[i][col] == 1) count1++;

        // Trois identiques consécutifs
        if (i >= 2 && g[i][col] == g[i - 1][col] && g[i][col] == g[i - 2][col]) return 0;
    }

    // Trop de 0 ou 1 ?
    return (count0 <= N / 2 && count1 <= N / 2);
}

/// Vérifie si une ligne est identique à une autre ligne déjà existante (complètement remplie)
int ligne_dupliquee(int **g, const int *ligne, int nb_lignes, int N) {
    for (int i = 0; i < nb_lignes; i++) {
        int identique = 1;
        for (int j = 0; j < N; j++) {
            // Si une case est vide (-1) ou différente, ce n'est pas une duplication
            if (ligne[j] == -1 || g[i][j] == -1 || ligne[j] != g[i][j]) {
                identique = 0;
                break;
            }
        }
        if (identique) return 1;
    }
    return 0;
}

/// Vérifie si deux colonnes complètes sont identiques
int colonnes_dupliquees(int **g, int N) {
    for (int c1 = 0; c1 < N - 1; c1++) {
        for (int c2 = c1 + 1; c2 < N; c2++) {
            int identique = 1;
            for (int i = 0; i < N; i++) {
                // Si une case est vide (-1) ou différente, ce n'est pas une duplication
                if (g[i][c1] == -1 || g[i][c2] == -1 || g[i][c1] != g[i][c2]) {
                    identique = 0;
                    break;
                }
            }
            if (identique) return 1;
        }
    }
    return 0;
}

/// Vérifie l’unicité des lignes et colonnes de la grille (pas de doublons complets)
int verifier_unicite(int **grid, int N) {
    // Vérifie d'abord les colonnes
    if (colonnes_dupliquees(grid, N)) return 0;

    // Vérifie ensuite les lignes déjà présentes
    for (int i = 0; i < N - 1; i++) {
        if (ligne_dupliquee(grid, grid[i], i, N)) return 0;
    }

    return 1;
}