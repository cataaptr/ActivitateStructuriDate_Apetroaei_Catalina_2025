#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//trebuie sa folositi fisierul masini.txt
//sau va creati un alt fisier cu alte date

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

//creare structura pentru un nod dintr-un arbore binar de cautare
typedef struct Nod Nod;
struct Nod {
	Masina info;
	struct Nod* st;
	struct Nod* dr;
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
	m1.pret= atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	m1.model = malloc(strlen(aux) + 1);
	strcpy_s(m1.model, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy_s(m1.numeSofer, strlen(aux) + 1, aux);

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
	
	if (!(*arbore))
	{
		Nod* nou = (Nod*)malloc(sizeof(Nod));
		nou->info = masinaNoua;
		nou->st = NULL;
		nou->dr = NULL;
		(*arbore) = nou;
	}
	else
	{
		if ((*arbore)->info.id > masinaNoua.id)
		{
			adaugaMasinaInArbore(&((*arbore)->st), masinaNoua);
		}
		else if ((*arbore)->info.id < masinaNoua.id)
		{
			adaugaMasinaInArbore(&((*arbore)->dr), masinaNoua);
		}
	}
}

Nod* citireArboreDeMasiniDinFisier(const char* numeFisier) {
	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
	//prin apelul repetat al functiei citireMasinaDinFisier()
	//ATENTIE - la final inchidem fisierul/stream-ul
	FILE* f = fopen(numeFisier, "r");
	Nod* arbore = NULL;
	while (!feof(f))
	{
		Masina m = citireMasinaDinFisier(f);
		adaugaMasinaInArbore(&arbore, m);
	}
	fclose(f);
	return arbore;
}

void afisareInOrdine(Nod* arbore)
{
	if (arbore)
	{
		afisareInOrdine(arbore->st);
		afisareMasina(arbore->info);
		afisareInOrdine(arbore->dr);
	}
}

void afisarePreOrdine(Nod* arbore)
{
	if (arbore)
	{

		afisareMasina(arbore->info);

		afisarePreOrdine(arbore->st);
		afisarePreOrdine(arbore->dr);
	}
}
void afisarePostOrdine(Nod* arbore)
{
	if (arbore)
	{
		afisarePostOrdine(arbore->st);
		afisarePostOrdine(arbore->dr);
		afisareMasina(arbore->info);


	}
}

void afisareMasiniDinArbore(Nod* arbore) {
	printf("\n--- Parcurgere InOrdine ---\n");
	afisareInOrdine(arbore);
	printf("\n--- Parcurgere PreOrdine ---\n");
	afisarePreOrdine(arbore);
	printf("\n--- Parcurgere PostOrdine ---\n");
	afisarePostOrdine(arbore);
}

void dezalocareArboreDeMasini(Nod** arbore) {
	//sunt dezalocate toate masinile si arborele de elemente
	if (*arbore) {
		dezalocareArboreDeMasini(&(*arbore)->st);
		dezalocareArboreDeMasini(&(*arbore)->dr);
		free((*arbore)->info.model);
		free((*arbore)->info.numeSofer);
		free(*arbore);
		(*arbore) = NULL;
	
	}


}
//radacina=6, id=8
Masina getMasinaByID(Nod* arbore,int id) {
	Masina m;
	m.id = -1;
	if (arbore) {
		if (arbore->info.id < id) {
			return getMasinaByID(arbore->dr, id);

		}else
		if (arbore->info.id > id) {
			return getMasinaByID(arbore->st, id);

		}
		else {
			return arbore->info;
		}
	}


	return m;
}

int determinaNumarNoduri(Nod* arbore) {
	if (arbore == NULL) {
		return 0;
	}
	return 1 + determinaNumarNoduri(arbore->st) + determinaNumarNoduri(arbore->dr);
}

int calculeazaInaltimeArbore(Nod* arbore) {
	if (arbore == NULL) {
		return 0;
	}
	int inaltimeStanga = calculeazaInaltimeArbore(arbore->st);
	int inaltimeDreapta = calculeazaInaltimeArbore(arbore->dr);
	return 1 + (inaltimeStanga > inaltimeDreapta ? inaltimeStanga : inaltimeDreapta);
}

float calculeazaPretTotal(Nod*arbore) {
	//calculeaza pretul tuturor masinilor din arbore.
	if (arbore == NULL) {
		return 0;
	}
	float totalStanga= calculeazaPretTotal(arbore->st);
	float totalDreapta=calculeazaPretTotal(arbore->dr);
	return arbore->info.pret + totalStanga + totalDreapta;
}

float calculeazaPretulMasinilorUnuiSofer(Nod* arbore, const char* numeSofer) {
	if (arbore == NULL) {
		return 0;
	}
	float suma = 0;
	if (strcmp(arbore->info.numeSofer, numeSofer) == 0) {
		suma = arbore->info.pret;
	}
	return suma +
		calculeazaPretulMasinilorUnuiSofer(arbore->st, numeSofer) +
		calculeazaPretulMasinilorUnuiSofer(arbore->dr, numeSofer);
}

int main() {
	Nod* radacina = NULL;
	radacina = citireArboreDeMasiniDinFisier("masini_arbore.txt");

	afisareMasiniDinArbore(radacina);

	int id = 8;
	printf("\n Cautare masina dupa ID = %d \n", id);
	Masina m = getMasinaByID(radacina, id);
	if (m.id != -1)
		afisareMasina(m);
	else
		printf("Masina cu id %d nu a fost gasita.\n", id);

	printf("\n Pret total masini \n");
	float pret = calculeazaPretTotal(radacina);
	printf("Pretul total al masinilor este: %.2f\n", pret);

	printf("\n Numar noduri in arbore \n");
	printf("Numar noduri: %d\n", determinaNumarNoduri(radacina));

	printf("\n Inaltime arbore \n");
	printf("Inaltimea arborelui: %d\n", calculeazaInaltimeArbore(radacina));

	printf("\n Pret masini pentru un sofer \n");
	const char* numeSofer = "Ion";
	float pretSofer = calculeazaPretulMasinilorUnuiSofer(radacina, numeSofer);
	printf("Pret total masini pentru soferul %s: %.2f\n", numeSofer, pretSofer);

	dezalocareArboreDeMasini(&radacina);
	return 0;
}
