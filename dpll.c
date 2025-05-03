#include "dpll.h"
#include "grilles.h"


int** lire_clauses(char* nom_fichier, int* nb_clauses, int* dimension) {
    
    char ligne_fichier[50];
    FILE *fichier_dimac = fopen(nom_fichier, "r");

    fgets(ligne_fichier, sizeof(ligne_fichier), fichier_dimac);
    sscanf(ligne_fichier, "p cnf %d %d", dimension, nb_clauses);

    *dimension = (int) sqrt(*dimension);


    printf("%d %d \n", *dimension, *nb_clauses);

    int** ensemble_clauses = malloc(sizeof(int*) * *nb_clauses);
    for (int i = 0; i < *nb_clauses; ++i) {
        ensemble_clauses[i] = calloc(*dimension + 1, sizeof(int)); // +1 pour le 0 terminal
    }
    int idx_ligne = 0;
    int clause_idx = 0;
    int coeff;

    while (fgets(ligne_fichier, sizeof(ligne_fichier), fichier_dimac)) {
        int literal;
        int literal_idx = 0;
        char* token = strtok(ligne_fichier, " \n");

        while (token != NULL) {
            literal = atoi(token);
            if (literal == 0) break;
            ensemble_clauses[clause_idx][literal_idx] = literal;
            literal_idx++;
            token = strtok(NULL, " \n");
        }
        clause_idx++;
    }
    fclose(fichier_dimac);
    return ensemble_clauses;
}

void afficher_clauses(int** ensemble_clauses, int nb_clauses, int dimension) {
    for (int i = 0; i < nb_clauses; i++) {
        int j = 0;
        while (ensemble_clauses[i][j] != 0) {
            printf ("%d ", ensemble_clauses[i][j]);
            j++;
        }
        printf("\n");
    } 
}

clauses_grille_solution algo_dpll(int** clauses, int nb_clauses, int dimension) {
    // printf("########## deb dpll ################\n");
    clauses_grille_solution clauses_grille;
    clauses_grille.clauses = clauses;
    clauses_grille.grille_solution = malloc(sizeof(int*) * dimension);
    for (int i = 0; i < dimension; ++i) {
        clauses_grille.grille_solution[i] = calloc(dimension, sizeof(int));
    }
    clauses_grille.validite = malloc(sizeof(int*));
    *(clauses_grille.validite) = 1;

    // printf("########## fin dpll ################\n");
    algo_dpll_rec(clauses_grille, nb_clauses, dimension);
    return clauses_grille;
}

void algo_dpll_rec(clauses_grille_solution clauses_grille, int nb_clauses, int dimension) {
    printf("########## deb rec ################\n");
    afficher_grille(clauses_grille.grille_solution, dimension);

    
    for (int i = 0; i < nb_clauses; i++) {
        int j = 0;
        while (clauses_grille.clauses[i][j] != 0) {
            if (est_isoler(clauses_grille.clauses, clauses_grille.clauses[i][j], nb_clauses) == 1) {
                int k = j;
                assigner_valeur_litteral(clauses_grille, clauses_grille.clauses[i][j], nb_clauses, dimension);
            }
            j++;
        }
    }
    /*
    for (int i = 0; i < nb_clauses; i++) {
        
        if (clauses_grille.clauses[i][1] == 0 && clauses_grille.clauses[i][0] != 0) {
            printf("\n%d :  \n", i);
            assigner_valeur_litteral(clauses_grille, clauses_grille.clauses[i][0], nb_clauses, dimension);
            
            i = 0;
        }
    }*/
    

    // On verifie s'il existe des clauses non vide
    int idx_clause_non_vide = -1;
    for (int i = 0; i < nb_clauses; i++) {
        //printf("%d\n", clauses_grille.clauses[i][0]);
        if (clauses_grille.clauses[i][0] != 0) {
            idx_clause_non_vide = i;
            break;
        }
    }

    clauses_grille_solution copie1 = copier_clauses_grille(clauses_grille, nb_clauses, dimension);
    clauses_grille_solution copie2 = copier_clauses_grille(clauses_grille, nb_clauses, dimension);

    if (*(clauses_grille.validite) == 0) {     // Si toutes les clauses sont toutes vides on renvoie ce résultat, on a résolue l'ensemble
        printf("########## fin 4 rec ################\n");
        printf("Ensemble non satisfaisable !!! \n");
        // return clauses_grille;
    } else if (idx_clause_non_vide == -1) {
        printf("########## fin 5 rec ################\n");
        printf("Ensemble des clauses vide, ensemble satisfaisable !!! \n");
        // return clauses_grille;    
    } else {    // Sinon on assigne une valeur au premier litteral de la première clauses non vide
        assigner_valeur_litteral(clauses_grille, clauses_grille.clauses[idx_clause_non_vide][0], nb_clauses, dimension);
        algo_dpll_rec(clauses_grille, nb_clauses, dimension);
        if (*(clauses_grille.validite) == 1) {      // On recupere la solution si une des deux est valide
            clauses_grille = copie1;
            printf("########## fin 1 rec ################\n");
            return;
        }
        *(clauses_grille.validite) = 1;

        assigner_valeur_litteral(copie1, copie1.clauses[idx_clause_non_vide][0] * (-1), nb_clauses, dimension);
        algo_dpll_rec(copie1, nb_clauses, dimension);
        if (*(copie1.validite) == 1) {
            clauses_grille = copier_clauses_grille(copie1, nb_clauses, dimension);
            printf("########## fin 2 rec ################\n");
            return;
        } 
        
        // Si aucun modèle n'est valide l'ensemble de clauses fournit est insatifaisaible
        clauses_grille = copie2;
        *(clauses_grille.validite) = 0;
        printf("Modele non valide !! \n");
        
    }
    //printf("########## fin 3 rec ################\n");
}

void assigner_valeur_litteral(clauses_grille_solution clauses_grille, int litteral, int nb_clauses, int dimension) {
    printf("assigner valeur : %d  \n", litteral);
    // clauses_grille_solution nouveau = copier_clauses_grille(clauses_grille, dimension, nb_clauses);

    int j, k, l;
    for (int i = 0; i < nb_clauses; i++) {
        j = 0; 
        while (clauses_grille.clauses[i][j] != 0) { 
            if (clauses_grille.clauses[i][j] == litteral) {     // cas le litteral est présent avec le meme signe (on retire la clause)
                l = 0;
                while (clauses_grille.clauses[i][l] != 0) {
                    clauses_grille.clauses[i][l] = 0;
                    l++; 
                }
            } else if (clauses_grille.clauses[i][j] == litteral * (-1)) {    // cas le litteral est présent de signe inverse (on retire le litteral de la clause)
                if (clauses_grille.clauses[i][1] == 0) {    // Si la clause comporte seulement le litteral inverse l'ensemble est invalide
                    clauses_grille.clauses[i][0] = 0;
                    printf("CONTRADICTION !!!\n");
                    *(clauses_grille.validite) = 0; 
                    // printf("\n#################### fin assigner valeur ########################\n");

                    // return nouveau;                    
                } else {
                    k = j;
                    while (clauses_grille.clauses[i][k] != 0) {
                        clauses_grille.clauses[i][k] = clauses_grille.clauses[i][k + 1];
                        k++;
                    }
                }
            }
            j++;
        }
    } 
    // printf("\nlitteral: %d;i : %d; j : %d \n", litteral, abs(litteral)/dimension, abs(litteral) % dimension - 1);
    int temp_litteral = abs(litteral) - 1;
    if (litteral < 0) {
        clauses_grille.grille_solution[temp_litteral/dimension][temp_litteral % dimension] = 0;
    } else {
        clauses_grille.grille_solution[temp_litteral/dimension][temp_litteral % dimension] = 1;
    }

    if (est_isoler(clauses_grille.clauses, litteral, nb_clauses) != 0) {
        printf("Probleme assignation !! \n");
        exit(1);
    }
    //printf("\n#################### fin assigner valeur ########################\n");
    
}

int est_isoler(int** clauses, int litteral, int nb_clauses) {
    int compteur = 0;

    for (int i = 0; i < nb_clauses; i++) {
        int j = 0;
        while (clauses[i][j] != 0) {
            if (abs(clauses[i][j]) == abs(litteral)) {
                compteur++;
            }
            j++;
        }
    }
    return compteur;
    /*if (compteur != 1) {
        return 0;
    } else {
        return 1;
    }*/
}

clauses_grille_solution copier_clauses_grille(clauses_grille_solution original, int nb_clauses, int dimension) {
    clauses_grille_solution copie;

    copie.validite = original.validite;

    // Copie des clauses
    int j;
    copie.clauses = malloc(sizeof(int*) * nb_clauses);
    for (int i = 0; i < nb_clauses; ++i) {
        copie.clauses[i] = calloc(dimension + 1, sizeof(int));
    }
    for (int i = 0; i < nb_clauses; ++i) {
        j = 0;
        while (original.clauses[i][j] != 0) {
            copie.clauses[i][j] = original.clauses[i][j];
            j++;
        }
    }

    // Copie de la grille solution
    copie.grille_solution = malloc(sizeof(int*) * dimension);
    for (int i = 0; i < dimension; ++i) {
        copie.grille_solution[i] = calloc(dimension, sizeof(int));
    }
    for (int i = 0; i < dimension; ++i) {    
        j = 0;
        while (j < dimension) {
            //printf("i : %d; j : %d, dimension : %d\n", i, j, dimension);
            copie.grille_solution[i][j] = original.grille_solution[i][j];
            j++;
        }
    }

    // Copie de la validité
    copie.validite = original.validite;

    return copie;
}
