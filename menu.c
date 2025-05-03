#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "menu.h"
#include "grilles.h"
#include "regles.h"
#include "dimacs.h"


void afficher_indice(int **partielle_valide, int **grille_valide, int N) {
    int indices_a_ajouter = N;  // Nous voulons ajouter exactement N indices
    int cases_disponibles = 0;
    // Vérifier combien de cases vides il y a
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (partielle_valide[i][j] == -1) {
                cases_disponibles++;
            }
        }
    }
    if (cases_disponibles < indices_a_ajouter) {
        printf("Erreur : Il n'y a pas assez de cases vides pour ajouter des indices. L'aide n'est plus disponible.\n");
        return;  // L'aide n'est plus disponible, donc on sort de la fonction
    }
    // Ajouter exactement N indices à des positions aléatoires
    for (int i = 0; i <= indices_a_ajouter; i++) {
        int ligne, col;
        // Choisir une case aléatoire vide
        do {
            ligne = rand() % N;
            col = rand() % N;
        } while (partielle_valide[ligne][col] != -1); // Assurer que la case est vide
        // Ajouter l'indice dans la grille partielle valide
        partielle_valide[ligne][col] = grille_valide[ligne][col];
    }
    // Afficher la nouvelle grille partielle avec les indices ajoutés
    printf("\nNouvelle grille partielle avec indices ajoutés :\n");
    afficher_grille(partielle_valide, N);
}


void test_perf(int N, float densite){
    
    // === Grille valide complète ===
    printf("\n---------------------------------------------------------------------------------\n");
    printf("|                                 GRILLE VALIDE                                 |\n");
    printf("---------------------------------------------------------------------------------\n");

    int **grille_valide = creer_grille_valide(N);
    printf("\nGrille VALIDE complète :\n");
    afficher_grille(grille_valide, N);
    printf("-> %s\n", verifier_grille(grille_valide, N) ? "✅ VALIDE\n" : "❌ INVALIDE\n");
    FILE *f1 = fopen("grille_valide.cnf", "w");
    ecrire_grille_dimacs(f1, grille_valide, N);
    fclose(f1);

    printf("Réponse du SAT solver :\n");
    system("minisat grille_valide.cnf grille_valide.txt");
    printf("\n\nNous pouvons réécrire la grille de base avec les données du fichier .cnf\n");
    printf("Celà permet de vérifier que notre écriture en fichier dimacs est correcte.\n");
    printf("\nAffichage à partir du SAT solver : grille_valide.cnf\n");
    affichage("grille_valide.txt", N);
    printf("Grille identique à celle de base.\n");


    // === Grille valide partielle ===
    printf("\n---------------------------------------------------------------------------------\n");
    printf("|                            GRILLE VALIDE PARTIELLE                            |\n");
    printf("---------------------------------------------------------------------------------\n");

    int **partielle_valide = creer_grille_partielle_valide(grille_valide, N, densite);
    printf("\nGrille VALIDE partielle :\n");
    afficher_grille(partielle_valide, N);
    printf("-> %s\n", verifier_grille(partielle_valide, N) ? "✅ VALIDE\n" : "❌ INVALIDE\n");
    FILE *f2 = fopen("grille_valide_partielle.cnf", "w");
    ecrire_grille_dimacs(f2, partielle_valide, N); 
    fclose(f2);

    printf("Réponse du SAT solver :\n");
    system("minisat grille_valide_partielle.cnf grille_valide_partielle.txt");
    printf("\n\nLe SAT solver nous propose une solution à notre grille partielle.\n");
    printf("\nAffichage à partir du SAT solver : grille_valide_partielle.cnf\n");
    affichage("grille_valide_partielle.txt", N); 
    printf("La grille proposé est valide.\n");

    // === Grille invalide complète ===
    printf("\n---------------------------------------------------------------------------------\n");
    printf("|                                GRILLE INVALIDE                                |\n");
    printf("---------------------------------------------------------------------------------\n");

    int **grille_invalide = creer_grille_invalide(N);
    printf("\nGrille INVALIDE complète :\n");
    afficher_grille(grille_invalide, N);
    printf("-> %s\n", verifier_grille(grille_invalide, N) ? "✅ VALIDE\n" : "❌ INVALIDE\n");
    FILE *f3 = fopen("grille_invalide.txt", "w");
    ecrire_grille_dimacs(f3, grille_invalide, N);  // 🔧 Correction : f3 au lieu de f2
    fclose(f3);

    printf("Réponse du SAT solver :\n");
    system("minisat grille_invalide.cnf grille_invalide.txt");


    // === Grille invalide partielle ===
    printf("\n---------------------------------------------------------------------------------\n");
    printf("|                           GRILLE INVALIDE PARTIELLE                           |\n");
    printf("---------------------------------------------------------------------------------\n");

    int **partielle_invalide = creer_grille_partielle_invalide(N, densite);
    printf("\nGrille INVALIDE partielle :\n");
    afficher_grille(partielle_invalide, N);
    printf("-> %s\n", verifier_grille(partielle_invalide, N) ? "✅ VALIDE\n" : "❌ INVALIDE\n");
    FILE *f4 = fopen("grille_invalide_partielle.cnf", "w");
    ecrire_grille_dimacs(f4, partielle_invalide, N);  // 🔧 Correction ici
    fclose(f4);

    printf("Réponse du SAT solver :\n");
    system("minisat grille_invalide_partielle.cnf grille_invalide_partielle.txt");

    // === Libération mémoire ===
    liberer_grille(grille_valide, N);
    liberer_grille(partielle_valide, N);
    liberer_grille(grille_invalide, N);
    liberer_grille(partielle_invalide, N);

}

void test_grille(int N, float densite) {
    int **grille_valide = creer_grille_valide(N);
    int **partielle_valide = creer_grille_partielle_valide(grille_valide, N, densite);

    printf("\nGrille VALIDE partielle :\n");
    afficher_grille(partielle_valide, N);

    int choix;
    do {
        do {
            printf("\nSouhaitez-vous de l'aide ou voir la solution ?\n");
            printf("   -> 1 Afficher un indice (aide).\n");
            printf("   -> 2 Afficher la solution complète.\n");
            printf("   -> 3 Retour au menu.\n");
            printf("Votre choix : ");

            if (scanf("%d", &choix) != 1) {
                printf("Erreur : vous devez entrer un nombre entier (1, 2 ou 3).\n");
                while (getchar() != '\n'); // Vide le buffer pour �viter boucle infinie
                choix = 0; // Force � reboucler
            } else if (choix < 1 || choix > 3) {
                printf("Erreur : veuillez entrer 1, 2 ou 3.\n");
            }

        } while (choix < 1 || choix > 3);

        if (choix == 1) {
            afficher_indice(partielle_valide, grille_valide, N);
        } else if (choix == 2) {
            printf("\nSolution complète :\n");
            afficher_grille(grille_valide, N);
        }

    } while (choix == 1); // Si choix == 2 ou 3, on sort de la boucle
}



// Fonction pour vérifier si la grille est pleine ou partielle
int verifier_grille_humaine(int **grille, int N) {
    int est_pleine = 1;

    // Vérifier si la grille est pleine (pas de case vide)
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (grille[i][j] == -1) {
                est_pleine = 0; // Si on trouve un -1, la grille est partielle
                break;
            }
        }
        if (!est_pleine) break;
    }

    // Si la grille est pleine, vérifier les règles de la grille
    if (est_pleine) {
        return verifier_grille(grille, N);
    } else {
        // Si la grille est partielle, vérifier les règles supplémentaires
        for (int i = 0; i < N; i++) {
            int count_1_line = 0;
            int count_0_line = 0;
            int count_1_col = 0;
            int count_0_col = 0;
            for (int j = 0; j < N; j++) {
                // Vérifier les lignes
                if (grille[i][j] == 1) {
                    count_1_line++;
                    count_0_line = 0;
                } else if (grille[i][j] == 0) {
                    count_0_line++;
                    count_1_line = 0;
                }

                // Vérifier les colonnes
                if (grille[j][i] == 1) {
                    count_1_col++;
                    count_0_col = 0;
                } else if (grille[j][i] == 0) {
                    count_0_col++;
                    count_1_col = 0;
                }

                // Vérifier les règles de plus de 2 fois consécutives
                if (count_1_line > 2 || count_0_line > 2 || count_1_col > 2 || count_0_col > 2) {
                    //printf("Erreur : La ligne %d ou la colonne %d contient plus de deux 1 ou 0 consécutifs.\n", i + 1, i + 1);
                    return 0;  // Grille invalide
                }
            }

            // Vérifier si la ligne ou colonne contient plus de N 1 ou 0
            if (count_1_line > N || count_0_line > N || count_1_col > N || count_0_col > N) {
                //printf("Erreur : La ligne %d ou la colonne %d contient plus de N valeurs (1 ou 0).\n", i + 1, i + 1);
                return 0;  // Grille invalide
            }
        }

        return 1;  // Grille partielle valide
    }
}


int **grille_humaine(int N) {
    // Allouer dynamiquement la grille
    int **grille = malloc(N * sizeof(int *));
    for (int i = 0; i < N; i++) {
        grille[i] = malloc(N * sizeof(int));
    }

    printf("\nEntrez les valeurs pour chaque ligne de la grille (0, 1, -1 pour les cases vides) :\n");
    printf("Attention à bien mettre des espaces entre chaque valeur.\n");

    // Nettoyer le buffer avant de commencer � lire les lignes
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    // Lire les lignes une par une
    for (int i = 0; i < N; i++) {
        int valeurs_correctes = 0;
        while (!valeurs_correctes) {
            printf("Ligne %d : ", i + 1);
            // Lire la ligne compl�te
            char ligne[256];  // Buffer temporaire pour lire la ligne
            if (fgets(ligne, sizeof(ligne), stdin) != NULL) {
                int j = 0;
                valeurs_correctes = 1; // On suppose que la ligne est correcte jusqu'� preuve du contraire
                char *token = strtok(ligne, " ");
                while (token != NULL) {
                    if (j >= N) {
                        valeurs_correctes = 0; // Trop de valeurs dans la ligne
                        break;
                    }
                    int val = atoi(token);
                    if (val == 0 || val == 1 || val == -1) {
                        grille[i][j] = val;
                    } else {
                        valeurs_correctes = 0; // Valeur incorrecte
                        break;
                    }
                    j++;
                    token = strtok(NULL, " ");
                }

                // Si le nombre de valeurs est incorrect, on r�p�te la lecture
                if (j != N) {
                    valeurs_correctes = 0; // Pas le bon nombre de valeurs
                }

                // Si les valeurs sont incorrectes, on affiche un message d'erreur
                if (!valeurs_correctes) {
                    printf("Erreur : vous devez entrer exactement %d entiers valides (0, 1 ou -1). Veuillez réessayer.\n", N);
                }
            } else {
                valeurs_correctes = 0; // Lecture �chou�e, recommencer
            }
        }
    }

    // Affichage de la grille entr�e par l'utilisateur
    printf("\nVoici votre grille :\n");
    afficher_grille(grille, N);
    printf("-> %s\n", verifier_grille_humaine(grille, N) ? "✅ VALIDE\n" : "❌ INVALIDE\n");
    
    // Lib�ration de la m�moire
    liberer_grille(grille, N);

    return NULL; // Plus besoin de renvoyer la grille
}