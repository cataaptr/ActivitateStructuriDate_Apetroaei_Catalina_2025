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
	strcpy(m1.model, aux);
	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy(m1.numeSofer, aux);
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

#define DIM 100

typedef struct {
	Masina stiva[DIM];
	int varf;
} StivaVector;

void pushVec(StivaVector* s, Masina m) {
	s->stiva[s->varf++] = m;
}

Masina popVec(StivaVector* s) {
	return s->stiva[--s->varf];
}

typedef struct NodS {
	Masina info;
	struct NodS* next;
} NodS;

void pushLS(NodS** s, Masina m) {
	NodS* nou = malloc(sizeof(NodS));
	nou->info = m;
	nou->next = *s;
	*s = nou;
}

Masina popLS(NodS** s) {
	NodS* tmp = *s;
	Masina m = tmp->info;
	*s = tmp->next;
	free(tmp);
	return m;
}

typedef struct NodD {
	Masina info;
	struct NodD* prev, * next;
} NodD;

void pushLD(NodD** cap, Masina m) {
	NodD* nou = malloc(sizeof(NodD));
	nou->info = m;
	nou->prev = NULL;
	nou->next = *cap;
	if (*cap) (*cap)->prev = nou;
	*cap = nou;
}

Masina popLD(NodD** cap) {
	NodD* tmp = *cap;
	Masina m = tmp->info;
	*cap = tmp->next;
	if (*cap) (*cap)->prev = NULL;
	free(tmp);
	return m;
}

typedef struct {
	Masina coada[DIM];
	int start, end;
} CoadaVector;

void enqueueVec(CoadaVector* c, Masina m) {
	c->coada[c->end++] = m;
}

Masina dequeueVec(CoadaVector* c) {
	return c->coada[c->start++];
}

void enqueueLS(NodS** c, Masina m) {
	NodS* nou = malloc(sizeof(NodS));
	nou->info = m;
	nou->next = NULL;
	if (*c == NULL) *c = nou;
	else {
		NodS* tmp = *c;
		while (tmp->next) tmp = tmp->next;
		tmp->next = nou;
	}
}

Masina dequeueLS(NodS** c) {
	NodS* tmp = *c;
	Masina m = tmp->info;
	*c = tmp->next;
	free(tmp);
	return m;
}

void enqueueLD(NodD** cap, NodD** coada, Masina m) {
	NodD* nou = malloc(sizeof(NodD));
	nou->info = m;
	nou->next = NULL;
	nou->prev = *coada;
	if (*coada) (*coada)->next = nou;
	else *cap = nou;
	*coada = nou;
}

Masina dequeueLD(NodD** cap, NodD** coada) {
	NodD* tmp = *cap;
	Masina m = tmp->info;
	*cap = tmp->next;
	if (*cap) (*cap)->prev = NULL;
	else *coada = NULL;
	free(tmp);
	return m;
}

int main() {
	FILE* f = fopen("masini.txt", "r");
	StivaVector sv = { .varf = 0 };
	NodS* ls = NULL;
	NodD* ld = NULL;
	CoadaVector cv = { .start = 0, .end = 0 };
	NodS* cls = NULL;
	NodD* cld = NULL, * tail = NULL;
	while (!feof(f)) {
		Masina m = citireMasinaDinFisier(f);
		pushVec(&sv, m);
		pushLS(&ls, m);
		pushLD(&ld, m);
		enqueueVec(&cv, m);
		enqueueLS(&cls, m);
		enqueueLD(&cld, &tail, m);
	}
	fclose(f);

	printf("\nStiva Vector:\n");
	while (sv.varf) afisareMasina(popVec(&sv));

	printf("\nStiva Lista simpla:\n");
	while (ls) afisareMasina(popLS(&ls));

	printf("\nStiva Lista dubla:\n");
	while (ld) afisareMasina(popLD(&ld));

	printf("\nCoada Vector:\n");
	while (cv.start < cv.end) afisareMasina(dequeueVec(&cv));

	printf("\nCoada Lista simpla:\n");
	while (cls) afisareMasina(dequeueLS(&cls));

	printf("\nCoada Lista dubla:\n");
	while (cld) afisareMasina(dequeueLD(&cld, &tail));

	return 0;
}
