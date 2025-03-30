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
	Nod* next;
};

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	char* aux;
	Masina m1;

	fgets(buffer, 100, file);

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
	printf("Nr usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

void afisareListaMasini(Nod* cap) {
	while (cap) {
		afisareMasina(cap->info);
		cap = cap->next;
	}
}

void adaugaMasinaInLista(Nod** cap, Masina masinaNoua) {
	Nod* nodNou = (Nod*)malloc(sizeof(Nod));
	nodNou->info = masinaNoua;
	nodNou->next = NULL;

	if ((*cap) == NULL){
		*cap = nodNou;
	} else {
		Nod* p = *cap;
		while (p->next) {
			p = p->next;
		}
		p->next = nodNou;
	}
}

void adaugaLaInceputInLista(Nod** cap, Masina masinaNoua) {
	Nod* nodNou = (Nod*)malloc(sizeof(Nod));
	nodNou->info = masinaNoua;
	nodNou->next = *cap;
	*cap = nodNou;
}

Nod* citireListaMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	Nod* lista = NULL;
	while (!feof(f)) {
		adaugaLaInceputInLista(&lista, citireMasinaDinFisier(f));
	}
	fclose(f);
	return lista;
}

void dezalocareListaMasini(Nod** cap) {
	while ((*cap) != NULL) {
		Nod* aux = (*cap);
		*cap = (*cap)->next;
		free(aux->info.numeSofer);
		free(aux->info.model);
		free(aux);
	}
}

float calculeazaPretMediu(Nod* lista) {
	//if (!lista) return 0;
	int count = 0;
	float sum = 0;
	while (lista) {
		sum += lista->info.pret;
		count++;
		lista = lista->next;
	}
	return (count>0) ? sum/count : 0;
}

void stergeMasiniDinSeria(Nod** cap,char serieCautata) {
	while((*cap) && (*cap)->info.serie == serieCautata) { //stergere daca este pe prima pozitie
		Nod* aux = *cap;
		(*cap) = aux->next;
		if (aux->info.numeSofer) {
			free(aux->info.numeSofer);
		}
		if (aux->info.model) {
			free(aux->info.model);
		}
		free(aux);
	}
	if (*cap) { // mai am elem in lista dupa stergerea primelor
		Nod* p = *cap; // sa nu modificam pozitia
		while (p) {
			while (p->next && p->next->info.serie != serieCautata) {
				p = p->next;
			}
			if (p->next) {
				Nod* aux = p->next; //facem leg strcutura 
				p->next = p->next->next; // sau aux->next
				if (aux->info.numeSofer) {
					free(aux->info.numeSofer);
				}
				if (aux->info.model) {
					free(aux->info.model);
				}
				free(aux);
			}
			else {
				p = NULL;
			}
		}
	}

}

float calculeazaPretulMasinilorUnuiSofer(Nod* lista, const char* numeSofer) { //suma de masini cu acelasi sofer
	float sum = 0;
	while (lista) {
		if (strcmp(numeSofer, lista->info.numeSofer) == 0) {
			sum += lista->info.pret;
		}
		lista = lista->next;
	}
	return sum;
}

char* getCeaMaiScumpaMasina(Nod* lista) { //return nume
	float pretMaxim = 0;
	char* modelScump = NULL;

	while (lista) {
		if (lista->info.pret > pretMaxim) {
			pretMaxim = lista->info.pret;
			modelScump = lista->info.model;
		}
		lista = lista->next;
	}
	if (pretMaxim > 0) {
		char* nou = malloc(strlen(modelScump) + 1);
		strcpy_s(nou, strlen(modelScump) + 1, modelScump);
		return nou;
	}
	return NULL;
}

int main() {

	Nod* lista = citireListaMasiniDinFisier("masini.txt");
	afisareListaMasini(lista);

	printf("Pret mediu: %.2f \n\n", calculeazaPretMediu(lista));
	printf("Pret total dupa nume sofer: %.2f \n\n", calculeazaPretulMasinilorUnuiSofer(lista,"Ionescu"));
	char* model = getCeaMaiScumpaMasina(lista);
	printf("Model cel mai scump: %s\n\n", model);
	if (model) {
		free(model);
	}

	printf("Stergere seria A:\n");
	stergeMasiniDinSeria(&lista, 'A');
	afisareListaMasini(lista);
	printf("Stergere seria B:\n");
	stergeMasiniDinSeria(&lista, 'B');
	afisareListaMasini(lista);

	dezalocareListaMasini(&lista);


	return 0;
}
