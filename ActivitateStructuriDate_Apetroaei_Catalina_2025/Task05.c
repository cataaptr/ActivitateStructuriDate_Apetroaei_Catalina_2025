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


void inserareDupaPret(Nod** cap, Masina m) {
	Nod* nodNou = (Nod*)malloc(sizeof(Nod));
	nodNou->info = m;
	nodNou->next = NULL;

	if (*cap == NULL || m.pret < (*cap)->info.pret) { //inserare inceput
		nodNou->next = *cap;
		*cap = nodNou;
		return;
	}

	Nod* current = *cap;
	while (current->next != NULL && current->next->info.pret < m.pret) { //inserare dupa 
		current = current->next;
	}

	nodNou->next = current->next;
	current->next = nodNou;
}


Nod* citireListaMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	Nod* lista = NULL;
	while (!feof(f)) {
		//adaugaMasinaInLista(&lista, citireMasinaDinFisier(f));
		inserareDupaPret(&lista, citireMasinaDinFisier(f));
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


void stergereDupaIndex(Nod** cap,int index) { //apelata la citireFisier
	int contor = 1;
	if (index < 1 || *cap == NULL  ) {
		return ;
	}
	if (index == 1) {
		Nod* aux = *cap;
		*cap = (*cap)->next;
		if (aux->info.model) {
			free(aux->info.model);
		}
		if (aux->info.numeSofer ) {
			free(aux->info.numeSofer);
		}
		free(aux);
	}
	Nod* p = *cap;
	while((contor < index- 1) && p->next!=NULL) {
		p = p->next;
		contor++;
	}
	if (p->next) {
		Nod* aux = p->next;
		p->next = aux->next;
		if (aux->info.model) {
			free(aux->info.model);
		}
		if (aux->info.numeSofer) {
			free(aux->info.numeSofer);
		}
		free(aux);
	}
}

Masina* copiereMasiniSubPret(Nod* cap, float prag, int* dimensiune) { 
	*dimensiune = 0;

	Nod* p = cap;
	while (p) {
		if (p->info.pret < prag)
			(*dimensiune)++;
		p = p->next;
	}

	if (*dimensiune == 0)
		return NULL;

	Masina* vector = (Masina*)malloc(sizeof(Masina) * (*dimensiune));
	p = cap;
	int i = 0;

	while (p) {
		if (p->info.pret < prag) {
			vector[i].id = p->info.id;
			vector[i].nrUsi = p->info.nrUsi;
			vector[i].pret = p->info.pret;
			vector[i].serie = p->info.serie;

			vector[i].model = (char*)malloc(strlen(p->info.model) + 1);
			strcpy(vector[i].model, p->info.model);

			vector[i].numeSofer = (char*)malloc(strlen(p->info.numeSofer) + 1);
			strcpy(vector[i].numeSofer, p->info.numeSofer);

			i++;
		}
		p = p->next;
	}

	return vector;
}

void interschimbareNoduriPePozitii(Nod* cap, int poz1, int poz2) {
	if (poz1 == poz2 || cap == NULL) return;

	if (poz1 > poz2) {
		int temp = poz1;
		poz1 = poz2;
		poz2 = temp;
	}

	Nod* nod1 = NULL;
	Nod* nod2 = NULL;
	int index = 1;
	Nod* current = cap;

	while (current != NULL) {
		if (index == poz1)
			nod1 = current;
		if (index == poz2)
			nod2 = current;
		current = current->next;
		index++;
	}

	if (nod1 && nod2) {
		Masina temp = nod1->info;
		nod1->info = nod2->info;
		nod2->info = temp;
	}
}



int main() {

	Nod* lista = citireListaMasiniDinFisier("masini.txt");
	afisareListaMasini(lista);

	printf("Stergere index 1:\n");
	stergereDupaIndex(&lista, 1);
	afisareListaMasini(lista);

	printf("Stergere index 2:\n");
	stergereDupaIndex(&lista, 2);
	afisareListaMasini(lista); 

	float prag = 4000;
	int dim = 0;
	printf("Vector cu pret masini < decat : %.2f\n",prag);
	Masina* vector = copiereMasiniSubPret(lista, prag, &dim);
	for (int i = 0; i < dim; i++) {
		afisareMasina(vector[i]);
	}

	printf("Interschimbare: \n");
	interschimbareNoduriPePozitii(lista, 2, 5);
	afisareListaMasini(lista);



	dezalocareListaMasini(&lista);


	return 0;
}
