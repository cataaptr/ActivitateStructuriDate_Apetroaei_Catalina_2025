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

struct Nod { //creare structura pentru un nod dintr-o lista dublu inlantuita
	Masina info;
	struct Nod* precedent;
	struct Nod* urmator;
};
typedef struct Nod Nod;

struct Lista { //creare structura pentru Lista Dubla 
	Nod* primul;
	Nod* ultimul;
};
typedef struct Lista Lista;

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

void afisareListaMasiniInceput(Lista lista) {
	Nod* p = lista.primul;
	while (p) {
		afisareMasina(p->info);
		p = p->urmator;
	}
}

void afisareListaMasiniFinal(Lista lista) {
	Nod* p = lista.ultimul;
	while (p) {
		afisareMasina(p->info);
		p = p->precedent;
	}
}

void adaugaMasinaInLista(Lista* lista, Masina masinaNoua) { //la final
	Nod* p = (Nod*)malloc(sizeof(Nod));
	p->info = masinaNoua;
	p->precedent = lista->ultimul;
	p->urmator = NULL;
	if (lista->ultimul) {
		lista->ultimul->urmator = p;
	}
	else { //caz e nula lista
		lista->primul = p;
	}
	lista->ultimul = p;
}

void adaugaLaInceputInLista(Lista* lista, Masina masinaNoua) {
	Nod* p = (Nod*)malloc(sizeof(Nod));
	p->info = masinaNoua;
	p->urmator = lista->primul; //!! PRIMA OARA LEGATURA
	p->precedent = NULL;
	if (lista->primul) {
		lista->primul->precedent = p;
	}
	else {
		lista->ultimul = p;
	}
	lista->primul = p;
	
}

Lista citireLDMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	Lista lista;
	lista.primul = NULL;
	lista.ultimul = NULL;
	while (!feof(f)) {
		adaugaMasinaInLista(&lista, citireMasinaDinFisier(f));
		//adaugaLaInceputInLista(&lista, citireMasinaDinFisier(f));
	}
	fclose(f);
	return lista;
}

void dezalocareLDMasini(Lista* lista) {
	Nod* p = lista->primul;
	while(p->urmator != NULL) {
		free(p->info.model);
		free(p->info.numeSofer);
		p = p->urmator;
		free(p->precedent);
	}
	free(p->info.model); //pt ultimul nod
	free(p->info.numeSofer);
	free(p);
	lista->primul = NULL;
	lista->ultimul = NULL;
}

float calculeazaPretMediu(Lista lista) {
	Nod* aux = lista.primul;
	float suma = 0;
	int contor = 0;
	while (aux) {
		suma += aux->info.pret;
		aux = aux->urmator;
		contor++;
	}

	return suma/contor;
}

void stergeMasinaDupaID(Lista* lista, int id) {
	if (lista->primul == NULL)
		return;

	Nod* p = lista->primul;
	while (p != NULL && p->info.id != id) {
		p = p->urmator;
	}
	if (p == NULL)
		return;

	if (p->precedent == NULL) { // este primul
		lista->primul = p->urmator;
		if (lista->primul != NULL)
			lista->primul->precedent = NULL;
	}
	else {
		p->precedent->urmator = p->urmator;
	}

	if (p->urmator == NULL) { // este ultimul
		lista->ultimul = p->precedent;
	}
	else {
		p->urmator->precedent = p->precedent;
	}

	free(p->info.model);
	free(p->info.numeSofer);
	free(p);
}


char* getNumeSoferMasinaScumpa(Lista lista) {
	if (lista.primul == NULL)
		return NULL;

	Nod* p = lista.primul;
	Nod* maxNod = p;

	while (p != NULL) {
		if (p->info.pret > maxNod->info.pret) {
			maxNod = p;
		}
		p = p->urmator;
	}

	if (maxNod->info.numeSofer != NULL) {
		char* nume = (char*)malloc(strlen(maxNod->info.numeSofer) + 1);
		strcpy(nume, maxNod->info.numeSofer);
		return nume;
	}
	return NULL;
}


int main() {
	Lista lista=citireLDMasiniDinFisier("masini.txt");
	//afisareListaMasiniInceput(lista);
	afisareListaMasiniFinal(lista);

	printf("\n\nPret mediu: ");
	float pretMediu = calculeazaPretMediu(lista);
	printf("%2.f", pretMediu);

	char* sofer = getNumeSoferMasinaScumpa(lista);
	if (sofer) {
		printf("\nSoferul cu masina cea mai scumpa: %s\n", sofer);
		free(sofer);
	}
	
	printf("\n\nStergere:\n");
	stergeMasinaDupaID(&lista, 8);
	afisareListaMasiniInceput(lista);


	dezalocareLDMasini(&lista);

	return 0;
}


/*
* TEST:
* -pana la hashTable
* -creare structura + inserare + procesare (idk medie) + afisare + dezalocare
* -combinatii structuri (iei din vector si sa o pui in lista, invers)
* -de la 0
*/