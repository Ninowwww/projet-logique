CC = clang

# Règle principale : compilation de l'exécutable principal
all: binairo test_dpll

# Compilation de regles.o
regles.o: regles.c regles.h
	$(CC) -c regles.c

# Compilation de grilles.o 
grilles.o: grilles.c grilles.h regles.h
	$(CC) -c grilles.c

# Compilation de dimacs.o 
dimacs.o: dimacs.c dimacs.h regles.h grilles.h
	$(CC) -c dimacs.c

# Compilation de menu.o
menu.o: menu.c grilles.h regles.h dimacs.h
	$(CC) -c menu.c

# Compilation de binairo.o
binairo.o: binairo.c grilles.h regles.h dimacs.h menu.h
	$(CC) -c binairo.c

# Compilation de dpll.o
dpll.o: dpll.c dpll.h grilles.h
	$(CC) -c dpll.c

# Compilation de test_dpll.o
test_dpll.o: test_dpll.c dpll.h grilles.h
	$(CC) -c test_dpll.c



# Création de l'exécutable final binairo
binairo: grilles.o regles.o binairo.o dimacs.o menu.o
	$(CC) grilles.o regles.o dimacs.o binairo.o menu.o -o binairo
	
# Création de l'exécutable final test_dpll
test_dpll: test_dpll.o dpll.o grilles.o regles.o
	$(CC) test_dpll.o dpll.o grilles.o regles.o -lm -o test_dpll

# Nettoyage des fichiers objets et de l'exécutable
clean:
	rm -f binairo *.o