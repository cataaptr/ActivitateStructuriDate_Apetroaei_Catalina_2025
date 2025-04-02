#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>	
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
	struct Nod* next;
};

struct HashTable {
	int dim;
	Nod** v;	// vectori adrese la nod
};
typedef struct HashTable HashTable;

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
	printf("\nId: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

void afisareListaMasini(Nod* lista) {
	while (lista != NULL) { // lista->next nu il afiseaza si pe ultimul
		afisareMasina(lista->info);
		lista = lista->next;
	}
}

void adaugaMasinaInLista(Nod* cap, Masina masinaNoua) { // adaugare la final; 
	Nod* nodNou = (Nod*)malloc(sizeof(Nod));
	nodNou->next = NULL;
	nodNou->info = masinaNoua;
	while (cap->next != NULL) {
		cap = cap->next;
	}
	cap->next = nodNou;
}

HashTable initializareHashTable(int dimensiune) { //initializeaza vectorul de liste si seteaza fiecare lista ca fiind NULL;
	HashTable ht;
	ht.dim = dimensiune;
	ht.v = (Nod**)malloc(sizeof(Nod*) * dimensiune); //nod** ptc este pointer la pointer
	for (int i = 0; i < dimensiune; i++) {
		ht.v[i] = NULL;
	}
	return ht;
}

int calculeazaHash(const char* numeSofer, int dimensiune) { //dim sa stiu pana la ce output am 
	if (dimensiune == 0) {
		return 0;
	}
	int suma = 0;
	for (int i = 0; i < strlen(numeSofer) ; i++) { 
		suma += numeSofer[i]; // se face cast la integer de la ASCII implicit
	}
	return suma % dimensiune;
}

void inserareMasinaInTabela(HashTable hash, Masina masina) {
	//este folosit mecanismul CHAINING - este determinata pozitia si se realizeaza inserarea pe pozitia respectiva
	int poz = calculeazaHash(masina.numeSofer, hash.dim);
	if (hash.v[poz] == NULL) { // nu avem coliziune, lista e vida
		Nod* nodNou =(Nod*) malloc(sizeof(Nod));
		nodNou->info = masina;
		nodNou->next = NULL;
		hash.v[poz] = nodNou;
	} else { // avem coliziune, avem deja un nod
		adaugaMasinaInLista(hash.v[poz], masina);
	}
}

HashTable citireMasiniDinFisier(const char* numeFisier, int dimensiune) {
	FILE* file = fopen(numeFisier, "r");
	HashTable hashTable = initializareHashTable(dimensiune);
	while (!feof(file)) {
		inserareMasinaInTabela(hashTable, citireMasinaDinFisier(file));
	}
	return hashTable;
}

void afisareTabelaDeMasini(HashTable ht) {
	for (int i = 0; i < ht.dim; i++) {
		printf("\nCluster %d\n", i + 1);
		afisareListaMasini(ht.v[i]);
	}
}

void dezalocareTabelaDeMasini(HashTable* ht) {
	if (ht == NULL || ht->v == NULL)
		return;

	for (int i = 0; i < ht->dim; i++) {
		Nod* current = ht->v[i];
		while (current != NULL) {
			Nod* temp = current;
			current = current->next;

			if (temp->info.model) {
				free(temp->info.model);
			}
			if (temp->info.numeSofer) {
				free(temp->info.numeSofer);
			}
			free(temp);
		}
	}

	free(ht->v);
	ht->v = NULL;
	ht->dim = 0;
}

float* calculeazaPreturiMediiPerClustere(HashTable ht, int* nrClustere) {
	*nrClustere = 0;

	float* medii = (float*)malloc(sizeof(float) * ht.dim);

	for (int i = 0; i < ht.dim; i++) {
		Nod* current = ht.v[i];
		int nrMasini = 0;
		float suma = 0;

		while (current != NULL) {
			suma += current->info.pret;
			nrMasini++;
			current = current->next;
		}

		if (nrMasini > 0) {
			medii[*nrClustere] = suma / nrMasini;
			(*nrClustere)++;
		}
	}

	medii = (float*)realloc(medii, sizeof(float) * (*nrClustere));
	return medii;
}

Masina getMasinaDupaCheie(HashTable ht,const char* numeSofer) {
	Masina m;
	m.id = -1;
	int hash = calculeazaHash(numeSofer,ht.dim);
	if (ht.v[hash] != NULL) {
		Nod* p = ht.v[hash]; // avem in lista
		while (p && (strcmp(numeSofer,p->info.numeSofer) !=0)) {
			p = p->next;
		}
		if (p) { //inseamna ca a gasit si vr deep copy
			m = p->info;
			m.model = malloc(strlen(p->info.model) + 1);
			strcpy(m.model, p->info.model);

			m.numeSofer = malloc(strlen(p->info.numeSofer) + 1);
			strcpy(m.numeSofer, p->info.numeSofer);

		}
	}
	return m;
}

int main() {
	HashTable ht = citireMasiniDinFisier("masini.txt", 5);
	afisareTabelaDeMasini(ht);

	Masina m = getMasinaDupaCheie(ht, "Gigel");
	printf("\n\nCautam masina lui Gigel");
	if (m.id != -1) {
		afisareMasina(m);
		free(m.model);
		free(m.numeSofer);
	}
	else {
		printf("\n\nNu a fost gasita");
	}

	printf("\nPreturi medii:");
	int nrClustere = 0;
	float* preturiMedii = calculeazaPreturiMediiPerClustere(ht, &nrClustere);
	for (int i = 0; i < nrClustere; i++) {
		printf("Pret mediu cluster %d: %.2f\n", i + 1, preturiMedii[i]);
	}
	free(preturiMedii);

	dezalocareTabelaDeMasini(&ht);



	return 0;
}
