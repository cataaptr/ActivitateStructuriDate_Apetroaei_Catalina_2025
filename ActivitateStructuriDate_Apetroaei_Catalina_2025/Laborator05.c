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
		//adaugaMasinaInLista(&lista, citireMasinaDinFisier(f));
		adaugaLaInceputInLista(&lista, citireMasinaDinFisier(f));
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

void stergeMasinaDupaID(/*lista masini*/ int id) {
	//sterge masina cu id-ul primit.
	//tratati situatia ca masina se afla si pe prima pozitie, si pe ultima pozitie
}

char* getNumeSoferMasinaScumpa(/*lista dublu inlantuita*/) {
	//cauta masina cea mai scumpa si 
	//returneaza numele soferului acestei maasini.
	return NULL;
}

int main() {
	Lista lista=citireLDMasiniDinFisier("masini.txt");
	//afisareListaMasiniInceput(lista);
	afisareListaMasiniFinal(lista);

	printf("\n\nValoare medie: ");
	float pretMediu = calculeazaPretMediu(lista);
	printf("%2.f", pretMediu);

	dezalocareLDMasini(&lista);

	return 0;
}


/*
* TEST:
* -pana la hashTable
* -pana la hashTable
* -creare structura + inserare + procesare (idk medie) + afisare + dezalocare
* -combinatii structuri (iei din vector si sa o pui in lista, invers)
* -de la 0
*/