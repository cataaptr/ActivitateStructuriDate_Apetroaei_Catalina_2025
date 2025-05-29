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
	int gradEchilibru;
};

typedef struct ListaSimpla {
	Masina info;
	struct ListaSimpla* next;
} ListaSimpla;

typedef struct ListaDubla {
	Masina info;
	struct ListaDubla* next;
	struct ListaDubla* prev;
} ListaDubla;


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
int nrNoduri(Nod* arbore) {
	if (!arbore) return 0;
	return 1 + nrNoduri(arbore->st) + nrNoduri(arbore->dr);
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

void salvareInVectorPreordine(Nod* arbore, Masina* vector, int* index) {
	if (arbore) {
		vector[(*index)++] = arbore->info;
		salvareInVectorPreordine(arbore->st, vector, index);
		salvareInVectorPreordine(arbore->dr, vector, index);
	}
}

void salvareInVectorInordine(Nod* arbore, Masina* vector, int* index) {
	if (arbore) {
		salvareInVectorInordine(arbore->st, vector, index);
		vector[(*index)++] = arbore->info;
		salvareInVectorInordine(arbore->dr, vector, index);
	}
}

void salvareInVectorPostordine(Nod* arbore, Masina* vector, int* index) {
	if (arbore) {
		salvareInVectorPostordine(arbore->st, vector, index);
		salvareInVectorPostordine(arbore->dr, vector, index);
		vector[(*index)++] = arbore->info;
	}
}

void inserareSfarsitListaSimpla(ListaSimpla** cap, Masina m) {
	ListaSimpla* nou = malloc(sizeof(ListaSimpla));
	nou->info = m;
	nou->next = NULL;
	if (*cap == NULL)
		*cap = nou;
	else {
		ListaSimpla* temp = *cap;
		while (temp->next)
			temp = temp->next;
		temp->next = nou;
	}
}

void salvareListaSimplaPreordine(Nod* arbore, ListaSimpla** cap) {
	if (arbore) {
		inserareSfarsitListaSimpla(cap, arbore->info);
		salvareListaSimplaPreordine(arbore->st, cap);
		salvareListaSimplaPreordine(arbore->dr, cap);
	}
}

void inserareSfarsitListaDubla(ListaDubla** cap, ListaDubla** coada, Masina m) {
	ListaDubla* nou = malloc(sizeof(ListaDubla));
	nou->info = m;
	nou->next = NULL;
	nou->prev = *coada;
	if (*cap == NULL) {
		*cap = nou;
		*coada = nou;
	}
	else {
		(*coada)->next = nou;
		*coada = nou;
	}
}

void salvareListaDublaPreordine(Nod* arbore, ListaDubla** cap, ListaDubla** coada) {
	if (arbore) {
		inserareSfarsitListaDubla(cap, coada, arbore->info);
		salvareListaDublaPreordine(arbore->st, cap, coada);
		salvareListaDublaPreordine(arbore->dr, cap, coada);
	}
}


int main() {
	Nod* arbore = citireArboreDeMasiniDinFisier("masini.txt");
	afisarePreOrdine(arbore);

	printf("\nAfisare masina cu id 2:\n");
	Masina m = getMasinaByID(arbore, 2);
	afisareMasina(m);
	

	printf("\nVECTOR\n\n");
	int dim = nrNoduri(arbore);
	Masina* vector = malloc(sizeof(Masina) * dim);
	int index = 0;

	printf("\nVector Preordine\n");
	index = 0;
	salvareInVectorPreordine(arbore, vector, &index);
	for (int i = 0; i < dim; i++) afisareMasina(vector[i]);

	printf("\nVector Inordine\n");
	index = 0;
	salvareInVectorInordine(arbore, vector, &index);
	for (int i = 0; i < dim; i++) afisareMasina(vector[i]);

	index = 0;
	salvareInVectorPostordine(arbore, vector, &index);
	for (int i = 0; i < dim; i++) afisareMasina(vector[i]);
	free(vector);



	printf("\nLISTA SIMPLA\n\n");
	ListaSimpla* listaS = NULL;
	salvareListaSimplaPreordine(arbore, &listaS);
	ListaSimpla* tempS = listaS;
	while (tempS) {
		afisareMasina(tempS->info);
		tempS = tempS->next;
	}
	// dezalocare
	tempS = listaS;
	while (tempS) {
		ListaSimpla* aux = tempS;
		tempS = tempS->next;
		free(aux);
	}



	printf("\nLISTA DUBLA\n\n");
	ListaDubla* listaD = NULL;
	ListaDubla* coadaD = NULL;
	salvareListaDublaPreordine(arbore, &listaD, &coadaD);
	ListaDubla* tempD = listaD;
	while (tempD) {
		afisareMasina(tempD->info);
		tempD = tempD->next;
	}
	
	// dezalocare
	tempD = listaD;
	while (tempD) {
		ListaDubla* aux = tempD;
		tempD = tempD->next;
		free(aux);
	}


	dezalocareArboreDeMasini(&arbore);

	return 0;
}