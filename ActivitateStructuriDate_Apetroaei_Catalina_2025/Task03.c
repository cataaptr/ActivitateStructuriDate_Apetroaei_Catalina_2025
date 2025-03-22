#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Carte {
	int cod;
	char* titlu;
	float pret;
	int nrPagini;
};

void afisareCarte(struct Carte c) {
	printf("Cod: %d\nTitlu: %s\nPret: %.2f\nPagini: %d\n\n", c.cod, c.titlu, c.pret, c.nrPagini);
}

void afisareVector(struct Carte* vector, int nr) {
	for (int i = 0; i < nr; i++) {
		afisareCarte(vector[i]);
	}
}

void adaugaCarteInVector(struct Carte** carti, int* nrCarti, struct Carte carteNoua) {
	struct Carte* aux = (struct Carte*)malloc(sizeof(struct Carte) * ((*nrCarti) + 1));
	for (int i = 0; i < *nrCarti; i++) {
		aux[i] = (*carti)[i];
	}
	aux[*nrCarti] = carteNoua;
	(*nrCarti)++;
	free(*carti);
	*carti = aux;
}

struct Carte citireCarteFisier(FILE* file) {
	char buffer[256];
	char sep[] = ",\n";
	char* aux;
	struct Carte c;

	fgets(buffer, sizeof(buffer), file);

	aux = strtok(buffer, sep);
	c.cod = atoi(aux);

	aux = strtok(NULL, sep);
	c.titlu = (char*)malloc(strlen(aux) + 1);
	strcpy(c.titlu, aux);

	aux = strtok(NULL, sep);
	c.pret = atof(aux);

	aux = strtok(NULL, sep);
	c.nrPagini = atoi(aux);

	return c;
}

struct Carte* citireVectorCartiFisier(const char* numeFisier, int* nrCartiCitite) {
	FILE* file = fopen(numeFisier, "r");
	struct Carte* carti = NULL;

	while (!feof(file)) {
		struct Carte c = citireCarteFisier(file);
		adaugaCarteInVector(&carti, nrCartiCitite, c);
	}

	fclose(file);
	return carti;
}

void salveazaCarteInFisier(const char* numeFisier, struct Carte c) {
	FILE* file = fopen(numeFisier, "a"); 

	if (file != NULL) {
		fprintf(file, "%d,%s,%.2f,%d\n", c.cod, c.titlu, c.pret, c.nrPagini);  
		fclose(file);
	}
	else {
		printf("Eroare la deschiderea fisierului %s pentru scriere.\n", numeFisier);
	}
}

void salveazaVectorCartiInFisier(const char* numeFisier, struct Carte* vector, int nrCarti) {
	FILE* file = fopen(numeFisier, "w"); 

	if (file != NULL) {
		for (int i = 0; i < nrCarti; i++) {
			fprintf(file, "%d,%s,%.2f,%d\n",
				vector[i].cod,
				vector[i].titlu,
				vector[i].pret,
				vector[i].nrPagini);
		}
		fclose(file);
	}
	else {
		printf("Eroare la deschiderea fisierului %s pentru scriere.\n", numeFisier);
	}
}


int main() {
	int nrCarti = 0;
	struct Carte* carti = citireVectorCartiFisier("Carti.txt", &nrCarti);
	afisareVector(carti, nrCarti);
	//struct Carte c = { 11, "Amintiri din copilarie", 33.50, 245 };

	//salveazaCarteInFisier("Carti.txt", c);

	salveazaVectorCartiInFisier("Carti1.txt", carti, nrCarti);



	return 0;
}
