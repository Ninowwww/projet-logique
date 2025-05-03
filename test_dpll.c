#include "dpll.h"
#include "grilles.h"

int main() {
    int dimension, nb_clauses;
    int** ensemble_clauses = lire_clauses("grille_valide_partielle.cnf", &nb_clauses, &dimension);

    clauses_grille_solution solution = algo_dpll(ensemble_clauses, nb_clauses, dimension);

    if (*(solution.validite) == 0) {
        printf("La grille n'a pas de solution !!!\n");
    } else {
        printf("La grille est valide, voici le résultat : \n");
        afficher_grille(solution.grille_solution, dimension);
    }

    return 0;
}
