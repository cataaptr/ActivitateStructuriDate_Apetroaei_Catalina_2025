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

typedef struct NodSimplu NodSimplu;
typedef struct NodSimplu {
	Masina info;
	struct NodSimplu* urmator;
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


void inserareSortataDupaPret(Lista* lista, Masina masinaNoua) { // apelata in fnct citire fisier
	Nod* p = (Nod*)malloc(sizeof(Nod));
	p->info = masinaNoua;
	p->precedent = NULL;
	p->urmator = NULL;

	if (!lista->primul) {
		lista->primul = lista->ultimul = p;
		return;
	}

	Nod* curent = lista->primul;
	while (curent && curent->info.pret < masinaNoua.pret) {
		curent = curent->urmator;
	}

	if (!curent) { // inserare la final
		p->precedent = lista->ultimul;
		lista->ultimul->urmator = p;
		lista->ultimul = p;
	}
	else if (!curent->precedent) { // inserare la inceput
		p->urmator = curent;
		curent->precedent = p;
		lista->primul = p;
	}
	else { // inserare in mijloc
		p->precedent = curent->precedent;
		p->urmator = curent;
		curent->precedent->urmator = p;
		curent->precedent = p;
	}
}

Lista citireLDMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	Lista lista;
	lista.primul = NULL;
	lista.ultimul = NULL;
	while (!feof(f)) {
		//adaugaMasinaInLista(&lista, citireMasinaDinFisier(f));
		//adaugaLaInceputInLista(&lista, citireMasinaDinFisier(f));
		inserareSortataDupaPret(&lista, citireMasinaDinFisier(f));
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

void stergereNodPozitie(Lista* lista, int pozitie) {
	if (pozitie < 0) return;

	Nod* p = lista->primul;
	int i = 0;
	while (p && i < pozitie-1) { // -1 ca sa sterga fix ce vr
		p = p->urmator;
		i++;
	}

	if (!p) return; // nu exista nod pe poz asta

	if (p->precedent)
		p->precedent->urmator = p->urmator;
	else
		lista->primul = p->urmator;

	if (p->urmator)
		p->urmator->precedent = p->precedent;
	else
		lista->ultimul = p->precedent;

	free(p->info.model);
	free(p->info.numeSofer);
	free(p);
}

NodSimplu* copiereMasiniScumpe(Lista lista, float prag) { // mai mare de un anume pret
	NodSimplu* cap = NULL, * sfarsit = NULL;
	Nod* p = lista.primul;
	while (p) {
		if (p->info.pret > prag) {
			Masina m;
			m.id = p->info.id;
			m.nrUsi = p->info.nrUsi;
			m.pret = p->info.pret;
			m.serie = p->info.serie;
			m.model = malloc(strlen(p->info.model) + 1);
			strcpy(m.model, p->info.model);
			m.numeSofer = malloc(strlen(p->info.numeSofer) + 1);
			strcpy(m.numeSofer, p->info.numeSofer);

			NodSimplu* nou = (NodSimplu*)malloc(sizeof(NodSimplu));
			nou->info = m;
			nou->urmator = NULL;

			if (!cap) cap = sfarsit = nou;
			else {
				sfarsit->urmator = nou;
				sfarsit = nou;
			}
		}
		p = p->urmator;
	}
	return cap;
}

void afisareListaSimpla(NodSimplu* cap) {
	NodSimplu* p = cap;
	while (p) {
		afisareMasina(p->info);
		p = p->urmator;
	}
}

void interschimbaElemente(Lista* lista, int poz1, int poz2) {
	if (poz1 == poz2) return;

	Nod* p1 = lista->primul;
	int i = 0;
	while (p1 && i < poz1-1) {
		p1 = p1->urmator;
		i++;
	}

	Nod* p2 = lista->primul;
	i = 0;
	while (p2 && i < poz2-1) {
		p2 = p2->urmator;
		i++;
	}

	if (p1 && p2) {
		Masina aux = p1->info;
		p1->info = p2->info;
		p2->info = aux;
	}
}


int main() {
	Lista lista = citireLDMasiniDinFisier("masini.txt"); // functie inserare in functie de pret cresc
	afisareListaMasiniInceput(lista);
	

	printf("\nSTERGERE\n");
	stergereNodPozitie(&lista, 1);
	afisareListaMasiniInceput(lista);

	printf("\nLISTA SIMPLA\n");
	NodSimplu* cap = copiereMasiniScumpe(lista, 5000);
	afisareListaSimpla(cap);

	printf("\nINTERSCHIMBARE\n");
	interschimbaElemente(&lista, 3, 4);
	afisareListaMasiniInceput(lista);

	return 0;
}