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

typedef struct Carte Carte;

void afisareCarte(Carte c) {
	printf("Cod: %d\nTitlu: %s\nPret: %.2f\nPagini: %d\n\n", c.cod, c.titlu, c.pret, c.nrPagini);
}

void afisareVector(Carte* vector, int nr) {
	for (int i = 0; i < nr; i++) {
		afisareCarte(vector[i]);
	}
}

void adaugaCarteInVector(Carte** carti, int* nrCarti, Carte carteNoua) {
	Carte* aux = (Carte*)malloc(sizeof(Carte) * ((*nrCarti) + 1));
	for (int i = 0; i < *nrCarti; i++) {
		aux[i] = (*carti)[i];
	}
	aux[*nrCarti] = carteNoua;
	(*nrCarti)++;
	free(*carti);
	*carti = aux;
}

Carte citireCarteFisier(FILE* file) {
	char buffer[256];
	char sep[] = ",\n";
	char* aux;
	Carte c;

	if (fgets(buffer, sizeof(buffer), file) == NULL) {
		c.titlu = NULL;
		return c;
	}

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

Carte* citireVectorCartiFisier(const char* numeFisier, int* nrCartiCitite) {
	FILE* file = fopen(numeFisier, "r");
	Carte* carti = NULL;

	while (!feof(file)) {
		Carte c = citireCarteFisier(file);
		if (c.titlu != NULL) adaugaCarteInVector(&carti, nrCartiCitite, c);
	}

	fclose(file);
	return carti;
}

void grupareCartiInMatrice(Carte* vector, int nrCarti, Carte*** matrice, int** dimensiuni, int* nrLinii) {
	*nrLinii = 3;
	*matrice = (Carte**)malloc(sizeof(Carte*) * (*nrLinii));
	*dimensiuni = (int*)calloc(*nrLinii, sizeof(int));

	for (int i = 0; i < *nrLinii; i++) {
		(*matrice)[i] = NULL;
	}

	for (int i = 0; i < nrCarti; i++) {
		int linie = 0;
		if (vector[i].nrPagini < 200) linie = 0;
		else if (vector[i].nrPagini <= 400) linie = 1;
		else linie = 2;

		(*matrice)[linie] = (Carte*)realloc((*matrice)[linie], sizeof(Carte) * ((*dimensiuni)[linie] + 1));
		(*matrice)[linie][(*dimensiuni)[linie]].cod = vector[i].cod;
		(*matrice)[linie][(*dimensiuni)[linie]].titlu = malloc(strlen(vector[i].titlu) + 1);
		strcpy((*matrice)[linie][(*dimensiuni)[linie]].titlu, vector[i].titlu);
		(*matrice)[linie][(*dimensiuni)[linie]].pret = vector[i].pret;
		(*matrice)[linie][(*dimensiuni)[linie]].nrPagini = vector[i].nrPagini;
		(*dimensiuni)[linie]++;
	}
}

void afisareMatrice(Carte** matrice, int* dimensiuni, int nrLinii) {
	for (int i = 0; i < nrLinii; i++) {
		printf("\nGrupa %d (%d carti):\n", i, dimensiuni[i]);
		for (int j = 0; j < dimensiuni[i]; j++) {
			printf("  %d. %s - %.2f lei - %d pagini\n",
				matrice[i][j].cod,
				matrice[i][j].titlu,
				matrice[i][j].pret,
				matrice[i][j].nrPagini);
		}
	}
}

void mutaLiniiSortate(Carte*** matrice, int** dimensiuni, int nrLinii) {
	for (int i = 0; i < nrLinii - 1; i++) {
		for (int j = i + 1; j < nrLinii; j++) {
			if ((*dimensiuni)[i] > (*dimensiuni)[j]) {
				int auxDim = (*dimensiuni)[i];
				(*dimensiuni)[i] = (*dimensiuni)[j];
				(*dimensiuni)[j] = auxDim;
				Carte* auxLinie = (*matrice)[i];
				(*matrice)[i] = (*matrice)[j];
				(*matrice)[j] = auxLinie;
			}
		}
	}
}

void dezalocareMatrice(Carte** matrice, int* dimensiuni, int nrLinii) {
	for (int i = 0; i < nrLinii; i++) {
		for (int j = 0; j < dimensiuni[i]; j++) {
			free(matrice[i][j].titlu);
		}
		free(matrice[i]);
	}
	free(matrice);
	free(dimensiuni);
}

int main() {
	int nrCarti = 0;
	Carte* carti = citireVectorCartiFisier("Carti.txt", &nrCarti);

	Carte** matrice = NULL;
	int* dimensiuni = NULL;
	int nrLinii = 0;

	grupareCartiInMatrice(carti, nrCarti, &matrice, &dimensiuni, &nrLinii);
	mutaLiniiSortate(&matrice, &dimensiuni, nrLinii);
	afisareMatrice(matrice, dimensiuni, nrLinii);
	dezalocareMatrice(matrice, dimensiuni, nrLinii);

	for (int i = 0; i < nrCarti; i++) {
		free(carti[i].titlu);
	}
	free(carti);

	return 0;
}
