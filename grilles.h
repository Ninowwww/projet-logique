#ifndef GRILLES_H
#define GRILLES_H

int **initialiser_grille(int N);
void liberer_grille(int **g, int N);
int remplir_grille(int **g, int N, int ligne_actuelle);
int **creer_grille_valide(int N);
int **creer_grille_invalide(int N);
int **creer_grille_partielle_valide(int **grille_complete, int N, float densite);
int **creer_grille_partielle_invalide(int N, float densite);

// Affichage d'une grille : -1 => '.', sinon affiche la valeur binaire
void afficher_grille(int **grille, int N);

// Vérifie la validité d'une grille selon 3 règles
int verifier_grille(int **grille, int N);


#endif