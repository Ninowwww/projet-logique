#include "dimacs.h"

void ajouter_contraintes_unicite(FILE *f, int **grid, int N, int *nb_clauses) {
    // Unicité des lignes
    for (int i = 0; i < N; i++) {
        for (int j = i + 1; j < N; j++) {
            int ligne_complete = 1;
            for (int k = 0; k < N; k++) {
                if (grid[i][k] == -1 || grid[j][k] == -1) {
                    ligne_complete = 0;
                    break;
                }
            }
            if (!ligne_complete) continue;

            // Si les deux lignes sont identiques → interdire cette combinaison
            int identiques = 1;
            for (int k = 0; k < N; k++) {
                if (grid[i][k] != grid[j][k]) {
                    identiques = 0;
                    break;
                }
            }
            if (identiques) {
                // Ajouter une clause qui empêche cette duplication
                // Exemple simple : -v1 -v2 ... -vN -u1 -u2 ... -uN 0
                for (int k = 0; k < N; k++) {
                    int vi = grid[i][k] ? -(i * N + k + 1) : (i * N + k + 1);
                    int vj = grid[j][k] ? -(j * N + k + 1) : (j * N + k + 1);
                    fprintf(f, "%d %d 0\n", vi, vj);
                    (*nb_clauses)++;
                }
            }
        }
    }

    // Unicité des colonnes
    for (int i = 0; i < N; i++) {
        for (int j = i + 1; j < N; j++) {
            int colonne_complete = 1;
            for (int k = 0; k < N; k++) {
                if (grid[k][i] == -1 || grid[k][j] == -1) {
                    colonne_complete = 0;
                    break;
                }
            }
            if (!colonne_complete) continue;

            int identiques = 1;
            for (int k = 0; k < N; k++) {
                if (grid[k][i] != grid[k][j]) {
                    identiques = 0;
                    break;
                }
            }
            if (identiques) {
                for (int k = 0; k < N; k++) {
                    int vi = grid[k][i] ? -(k * N + i + 1) : (k * N + i + 1);
                    int vj = grid[k][j] ? -(k * N + j + 1) : (k * N + j + 1);
                    fprintf(f, "%d %d 0\n", vi, vj);
                    (*nb_clauses)++;
                }
            }
        }
    }
}



void ajouter_parite(FILE *f, int *indices, int taille, int *nb_clauses, int valeur) {
    for (int i = 0; i < (1 << taille); i++) {
        int count = 0;
        for (int b = 0; b < taille; b++) {
            if ((i >> b) & 1) count++;
        }

        if (count == (taille / 2) + 1) {
            for (int b = 0; b < taille; b++) {
                int var = indices[b];
                if ((i >> b) & 1)
                    fprintf(f, valeur ? "-%d " : "%d ", var);
            }
            fprintf(f, "0\n");
            (*nb_clauses)++;
        }
    }
}

void ecrire_grille_dimacs(FILE *f, int **grid, int N) {
    int nb_vars = N * N;
    int nb_clauses = 0;

    char *clause_buf = NULL;
    size_t clause_size = 0;
    FILE *tmp = open_memstream(&clause_buf, &clause_size);

    // Contraintes des cases déjà remplies
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            int var = i * N + j + 1;
            int val = grid[i][j];
            if (val == 1) {
                fprintf(tmp, "%d 0\n", var);
                nb_clauses++;
            } else if (val == 0) {
                fprintf(tmp, "-%d 0\n", var);
                nb_clauses++;
            }
        }
    }

    // Interdire trois 1 consécutifs lignes
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N - 2; j++) {
            int a = i * N + j + 1;
            int b = i * N + j + 2;
            int c = i * N + j + 3;
            fprintf(tmp, "-%d -%d -%d 0\n", a, b, c);
            nb_clauses++;
        }
    }

    // Interdire trois 1 consécutifs colonnes
    for (int j = 0; j < N; j++) {
        for (int i = 0; i < N - 2; i++) {
            int a = i * N + j + 1;
            int b = (i + 1) * N + j + 1;
            int c = (i + 2) * N + j + 1;
            fprintf(tmp, "-%d -%d -%d 0\n", a, b, c);
            nb_clauses++;
        }
    }

    // Interdire trois 0 consécutifs lignes
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N - 2; j++) {
            int a = i * N + j + 1;
            int b = i * N + j + 2;
            int c = i * N + j + 3;
            fprintf(tmp, "%d %d %d 0\n", a, b, c);
            nb_clauses++;
        }
    }

    // Interdire trois 0 consécutifs colonnes
    for (int j = 0; j < N; j++) {
        for (int i = 0; i < N - 2; i++) {
            int a = i * N + j + 1;
            int b = (i + 1) * N + j + 1;
            int c = (i + 2) * N + j + 1;
            fprintf(tmp, "%d %d %d 0\n", a, b, c);
            nb_clauses++;
        }
    }

    // Parité lignes
    for (int i = 0; i < N; i++) {
        int indices[N];
        for (int j = 0; j < N; j++) {
            indices[j] = i * N + j + 1;
        }
        ajouter_parite(tmp, indices, N, &nb_clauses, 1);
        ajouter_parite(tmp, indices, N, &nb_clauses, 0);
    }

    // Parité colonnes
    for (int j = 0; j < N; j++) {
        int indices[N];
        for (int i = 0; i < N; i++) {
            indices[i] = i * N + j + 1;
        }
        ajouter_parite(tmp, indices, N, &nb_clauses, 1);
        ajouter_parite(tmp, indices, N, &nb_clauses, 0);
    }

    // Contraintes d'unicité
    ajouter_contraintes_unicite(tmp, grid, N, &nb_clauses);

    fclose(tmp);

    // Écrire en-tête DIMACS + contenu du buffer
    fprintf(f, "p cnf %d %d\n", nb_vars, nb_clauses);
    fwrite(clause_buf, 1, clause_size, f);
    free(clause_buf);
}


int affichage(const char *nom_fichier, int N) {
    FILE* fichier = fopen(nom_fichier, "r");
    if (fichier == NULL) {
        perror("Erreur d'ouverture du fichier");
        return 1;
    }

    char mot[16];
    int i = 0;

    // Lire et ignorer le premier mot (normalement "SAT")
    fscanf(fichier, "%s", mot);

    while (fscanf(fichier, "%s", mot) != EOF) {
        int val = atoi(mot);

        if (val == 0) {
            continue;
        }

        if (val > 0) {
            printf("1 ");
        } else {
            printf("0 ");
        }

        i++;

        if (i == N) {
            printf("\n");
            i = 0;
        }
    }
    
    printf("\n");
    fclose(fichier);
    return 0;
}