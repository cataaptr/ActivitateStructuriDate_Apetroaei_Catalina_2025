#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// arbore echilibrat, AVL
// ne dam seama ca este neechi din cauza inaltimi subarbori, calc dif dintre aceste inaltimi
// fiecare nod o sa aiba grad de echi = inaltime stg - inaltime subarbore dr
// -1,0,1 -> cei neechilibrati complet sunt acceptati nod principal si unul singur in jos (indif stg sau dr e acelasi echi)
// la val de sus nu se face echil 
// dezechi 2 si -2. daca e cu minus -> rotire stg (doar daca am facut stg-dr)
// echilib de fiecare data cand inseram
// echi -2 +1 -alternare de semne, situatie complexa -> rotire dr-stg
// nu o sa ajungem sa avem 3 si -3, ptc de fiecare data echilib

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
	int gradEchilibru;
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

int calculeazaInaltimeArbore(Nod* radacina) {
	if (radacina != NULL) {
		int intaltimeSt = calculeazaInaltimeArbore(radacina->st);
		int intaltimeDr = calculeazaInaltimeArbore(radacina->dr);

		return 1 + (intaltimeSt > intaltimeDr ? intaltimeSt : intaltimeDr);
	}
	else {
		return 0;
	}
}

//ALTE FUNCTII NECESARE: -rotiri
void rotireStanga(Nod** arbore) { // avem dezechi dr
	Nod* aux = (*arbore)->dr;
	(*arbore)->dr = aux->st;
	aux->st = (*arbore);
	(*arbore) = aux;
	(*arbore)->gradEchilibru--;
}

void rotireDreapta(Nod** arbore) { // avem dezechi stg
	Nod* aux = (*arbore)->st;
	(*arbore)->st = aux->dr;
	aux->dr = (*arbore);
	(*arbore) = aux;
	(*arbore)->gradEchilibru++;
}

void adaugaMasinaInArboreEchilibrat(Nod** radacina, Masina masinaNoua) {
	if ((*radacina) != NULL) {
		if ((*radacina)->info.id > masinaNoua.id) { // egalitate va fi in dreapta
			adaugaMasinaInArboreEchilibrat(&((*radacina)->st), masinaNoua);
			(*radacina)->gradEchilibru++;
		}
		else {
			adaugaMasinaInArboreEchilibrat(&((*radacina)->dr), masinaNoua);
			(*radacina)->gradEchilibru--;
		}
		if ((*radacina)->gradEchilibru == 2) { // dezechi in stg
			//avem dezechilibru in stanga
			if ((*radacina)->st->gradEchilibru == 1) {
				//rotire la dreapta
				rotireDreapta(radacina); // fara nimic ca s-ar anula
			}
			else  
			{
				rotireStanga(&((*radacina)->st));
				rotireDreapta(radacina);
			} //merge sa pui -1 si sa scoti afara rotire dr ca se repeta
		}
		if ((*radacina)->gradEchilibru == -2) // situatie complexa
		{
			//rotire la stanga
			if ((*radacina)->dr->gradEchilibru == 1)
			{
				rotireDreapta(&((*radacina)->dr));
			}
			rotireStanga(radacina);
		}
	}

	else
	{ // 0-0 nu trb echilib
		Nod* radacinaNoua = malloc(sizeof(Nod)); // nod frunza
		radacinaNoua->info = masinaNoua;
		radacinaNoua->dr = NULL;
		radacinaNoua->st = NULL;
		radacinaNoua->gradEchilibru = 0;
		(*radacina) = radacinaNoua;
	}
}

Nod* citireArboreDeMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	Nod* arbore = NULL;
	while (!feof(f))
	{
		Masina m = citireMasinaDinFisier(f);
		adaugaMasinaInArboreEchilibrat(&arbore, m);
	}
	fclose(f);
	return arbore;
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

void dezalocareArboreDeMasini(Nod** radacina) {
	if (*radacina) {
		dezalocareArboreDeMasini(&(*radacina)->st);
		dezalocareArboreDeMasini(&(*radacina)->dr);
		free((*radacina)->info.model);
		free((*radacina)->info.numeSofer);
		free(*radacina);
		(*radacina) = NULL;
	}
}

////radacina=6, id=8
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
	Nod* arbore = citireArboreDeMasiniDinFisier("masini.txt");
	afisarePreOrdine(arbore);

	printf("\nAfisare masina cu id 2:\n");
	Masina m = getMasinaByID(arbore, 2);
	afisareMasina(m);
	printf("\nNr noduri: %d\n", determinaNumarNoduri(arbore));
	printf("\nCalculeaza pret total: %.2f\n", calculeazaPretTotal(arbore));
	printf("\nCalculeaza pret masini Gigel Popescu: %.2f\n", calculeazaPretulMasinilorUnuiSofer(arbore,"Gigel Popescu"));

	dezalocareArboreDeMasini(&arbore);

	return 0;
}