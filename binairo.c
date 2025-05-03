#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "menu.h"
#include "grilles.h"
#include "regles.h"
#include "dimacs.h"
#include "dpll.h"


int main() {
    srand(time(NULL)); // Initialisation de l'aléatoire

    int choix = 1;
    int N;
    float densite;

    while (choix == 1 || choix == 2 || choix == 3 || choix == 4) {

        printf("\n---------------------------------------------------------------------------------\n");
        printf("|                                BINAIRO - MENU                                 |\n");
        printf("---------------------------------------------------------------------------------\n");
        printf("\nVous pouvez choisir votre mode : \n");
        printf("   -> 1  Tester les performances.\n");
        printf("   -> 2  Résoudre une grille.\n");
        printf("   -> 3  Tester une grille manuellement.\n");
        printf("   -> 4  Résolution avec DPLL.\n");
        printf("   -> 5  Fin du programme.\n");

        // Boucle pour forcer un bon choix
        do {
            printf("Votre choix : ");
            if (scanf("%d", &choix) != 1) {
                printf("Erreur : vous devez entrer un entier entre 1 et 5.\n");
                while (getchar() != '\n'); // Vider tout ce qui traîne
                choix = 0; // Forcer la répétition
            } else if (choix < 1 || choix > 5) {
                printf("Erreur : choix invalide. Veuillez entrer 1, 2, 3, 4 ou 5.\n");
            }
        } while (choix < 1 || choix > 5);
        

        if (choix == 1 || choix == 2) {
        
            printf("\nPlus la grille est grande, plus sa résolution sera difficile.\n");
            do {
                printf("Entrez la taille paire de la grille (entre 4 et 16) : ");
                if (scanf("%d", &N) != 1) {
                    printf("Erreur : vous devez entrer un entier.\n");
                    while (getchar() != '\n'); // Vider le buffer
                    N = 0; // Forcer à reboucler
                } else if (N < 4 || N > 16 || N % 2 != 0) {
                    printf("Erreur : la taille doit être un entier pair entre 4 et 16.\n");
                }
            } while (N < 4 || N > 16 || N % 2 != 0);

            printf("\nPlus la densité est basse, plus la résolution de la grille sera difficile.\n");
            do {
                printf("Entrez la densité (entier entre 40 et 60) : ");
                if (scanf("%f", &densite) != 1) {
                    printf("Erreur : vous devez entrer un nombre.\n");
                    while (getchar() != '\n'); // Vider le buffer
                    densite = 0; // Forcer à reboucler
                } else if (densite < 40.0f || densite > 60.0f || (int)densite != densite) {
                    printf("Erreur : la densité doit être un ENTIER entre 40 et 60.\n");
                }
            } while (densite < 40.0f || densite > 60.0f || (int)densite != densite);


            densite /= 100.0f; // Conversion %

            if (choix == 1) {
                test_perf(N, densite);
            } else if (choix == 2) {
                test_grille(N, densite);
            }

            printf("\nFin de l'exécution.\n");
        } 
        else if (choix == 3) {
            printf("\nPlus la grille est grande, plus sa résolution sera difficile.\n");
            do {
                printf("Entrez la taille paire de la grille (entre 4 et 16) : ");
                if (scanf("%d", &N) != 1) {
                    printf("Erreur : vous devez entrer un entier.\n");
                    while (getchar() != '\n'); // Vider le buffer
                    N = 0; // Forcer à reboucler
                } else if (N < 4 || N > 16 || N % 2 != 0) {
                    printf("Erreur : la taille doit être un entier pair entre 4 et 16.\n");
                }
            } while (N < 4 || N > 16 || N % 2 != 0);

            int **grille = grille_humaine(N);
            // Il faudrait sûrement aussi libérer la mémoire après...
        } 
        else if (choix == 4) {
            printf("Heu... Nino tu fais quoi ?\n");
            printf("\nFin de l'exécution.\n");
        }

    }

    printf("\nProgramme terminé.\n");
    printf("\n\n--- Created by JAHIER Nino, LAIB Hannah & MAURICE-DEMOURIOUX Rémy ---\n\n");

    return 0;
}