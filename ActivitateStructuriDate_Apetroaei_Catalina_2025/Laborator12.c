#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ponderat-are val pe muchie
// orientat-directie de parcurgere
// complet
// daca are ciclu nu e arbore
// arbore = graf fara ciclu, conex = toate nodurile sunt conectate intre ele
// o lista cu noduri si o lista in care tinem pt fiecare ce leg are 
// la cele de jos iau pointer la masina respectiva din lista principala, ca nu stam de f multe ori masina 
// matrice adiacenta: 0 pe diagnola principala, si pui 1 unde este muchie, daca nu este orientat - este simetrica

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

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
typedef struct NodListaSecundara NodListaSecundara;
typedef struct NodListaPrincipala NodListaPrincipala;

struct NodListaSecundara {
	NodListaPrincipala* info;
	NodListaSecundara* next;
};

struct NodListaPrincipala {
	NodListaPrincipala* next;
	Masina info;
	NodListaSecundara* vecini;
};

// inserare la sfarsit, nu modificam cap ci il returnam
NodListaPrincipala* inserareListaPrincipala(NodListaPrincipala* cap, Masina m) {
	NodListaPrincipala* nodNou = (NodListaPrincipala*)malloc(sizeof(NodListaPrincipala));
	nodNou->info = m;
	nodNou->next = NULL;
	nodNou->vecini = NULL;

	if (!cap) {
		return nodNou;
	}
	NodListaPrincipala* aux = cap; // tot trb aux
	while (aux->next) {
		aux= aux->next;
	}
	aux->next = nodNou;
	return cap;
}

// inserare la inceput
NodListaSecundara* inserareListaSecundara(NodListaSecundara* cap,NodListaPrincipala* info) {
	NodListaSecundara* nodNou = (NodListaSecundara*)malloc(sizeof(NodListaSecundara));
	nodNou->info = info;
	nodNou->next = NULL;
	nodNou->next = cap;
	return nodNou;
}

// functie de cautarea in lista principala dupa ID
NodListaPrincipala* cautaNodDupaID(NodListaPrincipala* listaPrincipala, int id) { 
	while (listaPrincipala && listaPrincipala->info.id != id) { // aici merge si cu ea in sine
		listaPrincipala= listaPrincipala->next;
	}
	return listaPrincipala;
}

// inserare muchie
void inserareMuchie(void* listaPrincipala, int idStart, int idStop) {
	NodListaPrincipala* nodStart = cautaNodDupaID(listaPrincipala, idStart);
	NodListaPrincipala* nodStop = cautaNodDupaID(listaPrincipala, idStop);
	if (nodStart && nodStop) {
		nodStart->vecini = inserareListaSecundara(nodStart->vecini, nodStop); // apelam de 2 ori ptc este neorientat, daca era apelam o singura data
		nodStop->vecini = inserareListaSecundara(nodStop->vecini, nodStart);
	}
}

NodListaPrincipala* citireNoduriMasiniDinFisier(const char* numeFisier) {
	NodListaPrincipala* listaPrincipala = NULL;
	FILE* file = fopen(numeFisier, "r");
	while (!feof(file)) {
		inserareListaPrincipala(listaPrincipala, citireMasinaDinFisier(file));
	}
	fclose(file);
	return listaPrincipala;
}

void citireMuchiiDinFisier(const char* numeFisier, NodListaPrincipala* listaPrincipala) {
	FILE* file = fopen(numeFisier, "r");
	int idStart, idStop;
	while (!feof(file)) {
		fscanf(file, "%d %d",&idStart,&idStop);
		inserareMuchie(listaPrincipala, idStart, idStop);
	}
	fclose(file);
}

void stergeListaSecundara(NodListaSecundara** cap) {
	NodListaSecundara* aux;
	while (*cap) {
		aux = (*cap)->next;
		free(*cap);
		(*cap) = aux;
	}
}

void dezalocareNoduriGraf(NodListaPrincipala** listaPrincipala) {
	NodListaPrincipala* aux;
	while (*listaPrincipala) {
		aux = (*listaPrincipala)->next;
		stergeListaSecundara(&(*listaPrincipala)->vecini);
		if ((*listaPrincipala)->info.model) {
			free((*listaPrincipala)->info.model);
		}
		if ((*listaPrincipala)->info.numeSofer) {
			free((*listaPrincipala)->info.numeSofer);
		}
		free(*listaPrincipala);
		(*listaPrincipala) = aux;
	}
}

int main() {
	NodListaPrincipala* graf = citireNoduriMasiniDinFisier("masini.txt");
	citireMuchiiDinFisier("muchii.txt", graf);

	dezalocareNoduriGraf(&graf);

	return 0;
}