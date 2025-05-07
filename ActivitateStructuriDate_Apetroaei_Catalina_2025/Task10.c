#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

typedef struct Nod Nod;
struct Nod {
	Masina info;
	Nod* st;
	Nod* dr;
};

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	m1.model = malloc(strlen(aux) + 1);
	strcpy(m1.model, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy(m1.numeSofer, aux);

	m1.serie = *strtok(NULL, sep);
	return m1;
}

void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

void adaugaMasinaInArbore(Nod** arbore, Masina masinaNoua) {
	if (!(*arbore)) {
		Nod* nou = (Nod*)malloc(sizeof(Nod));
		nou->info = masinaNoua;
		nou->st = NULL;
		nou->dr = NULL;
		(*arbore) = nou;
	}
	else {
		if ((*arbore)->info.id > masinaNoua.id) {
			adaugaMasinaInArbore(&((*arbore)->st), masinaNoua);
		}
		else if ((*arbore)->info.id < masinaNoua.id) {
			adaugaMasinaInArbore(&((*arbore)->dr), masinaNoua);
		}
	}
}

Nod* citireArboreDeMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	Nod* arbore = NULL;
	while (!feof(f)) {
		Masina m = citireMasinaDinFisier(f);
		adaugaMasinaInArbore(&arbore, m);
	}
	fclose(f);
	return arbore;
}

void afisareInOrdine(Nod* arbore) {
	if (arbore) {
		afisareInOrdine(arbore->st);
		afisareMasina(arbore->info);
		afisareInOrdine(arbore->dr);
	}
}

void dezalocareArboreDeMasini(Nod** arbore) {
	if (*arbore) {
		dezalocareArboreDeMasini(&(*arbore)->st);
		dezalocareArboreDeMasini(&(*arbore)->dr);
		free((*arbore)->info.model);
		free((*arbore)->info.numeSofer);
		free(*arbore);
		(*arbore) = NULL;
	}
}

int determinaNumarNoduri(Nod* arbore) {
	if (arbore == NULL) return 0;
	return 1 + determinaNumarNoduri(arbore->st) + determinaNumarNoduri(arbore->dr);
}

int calculeazaInaltimeArbore(Nod* arbore) {
	if (arbore == NULL) return 0;
	int hSt = calculeazaInaltimeArbore(arbore->st);
	int hDr = calculeazaInaltimeArbore(arbore->dr);
	return 1 + (hSt > hDr ? hSt : hDr);
}

Nod* stergereNod(Nod* radacina, int id) {
	if (radacina == NULL) return NULL;

	if (id < radacina->info.id) {
		radacina->st = stergereNod(radacina->st, id);
	}
	else if (id > radacina->info.id) {
		radacina->dr = stergereNod(radacina->dr, id);
	}
	else {
		if (radacina->st == NULL) {
			Nod* temp = radacina->dr;
			free(radacina->info.model);
			free(radacina->info.numeSofer);
			free(radacina);
			return temp;
		}
		else if (radacina->dr == NULL) {
			Nod* temp = radacina->st;
			free(radacina->info.model);
			free(radacina->info.numeSofer);
			free(radacina);
			return temp;
		}
		else {
			Nod* succ = radacina->dr;
			while (succ->st != NULL)
				succ = succ->st;
			radacina->info = succ->info;
			radacina->dr = stergereNod(radacina->dr, succ->info.id);
		}
	}
	return radacina;
}

Nod* subarboreCuInaltimeaMaiMare(Nod* radacina) {
	if (!radacina) return NULL;
	int hSt = calculeazaInaltimeArbore(radacina->st);
	int hDr = calculeazaInaltimeArbore(radacina->dr);
	return (hSt >= hDr) ? radacina->st : radacina->dr;
}

int numarNoduriSubordonate(Nod* radacina) {
	if (!radacina) return 0;
	return determinaNumarNoduri(radacina) - 1;
}

Nod* subarboreCuMaiMultiDescendenti(Nod* radacina) {
	if (!radacina) return NULL;
	int nrSt = determinaNumarNoduri(radacina->st);
	int nrDr = determinaNumarNoduri(radacina->dr);
	return (nrSt >= nrDr) ? radacina->st : radacina->dr;
}

int main() {
	Nod* radacina = citireArboreDeMasiniDinFisier("masini_arbore.txt");
	printf("\nAfisare InOrdine \n");
	afisareInOrdine(radacina);

	printf("\nStergere nod cu ID = 8\n");
	radacina = stergereNod(radacina, 8);
	afisareInOrdine(radacina);

	printf("\nSubarbore cu inaltime mai mare\n");
	Nod* inalt = subarboreCuInaltimeaMaiMare(radacina);
	if (inalt) afisareInOrdine(inalt);

	printf("\nNumar de noduri subordonate radacinii\n");
	printf("%d\n", numarNoduriSubordonate(radacina));

	printf("\nSubarbore cu mai multi descendenti\n");
	Nod* multi = subarboreCuMaiMultiDescendenti(radacina);
	if (multi) afisareInOrdine(multi);

	dezalocareArboreDeMasini(&radacina);
	return 0;
}