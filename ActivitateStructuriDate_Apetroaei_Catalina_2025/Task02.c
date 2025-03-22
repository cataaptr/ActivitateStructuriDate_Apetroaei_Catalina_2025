#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>


// nume: Apetroaei Catalina => CArte
struct Carte {
	int cod;
	char* titlu;
	float pret;
	int nrPagini;
};

struct Carte initializare(int cod,const char* titlu,float pret,int nrPagini) {
	struct Carte c;
	c.cod = cod;
	c.pret = pret;
	c.nrPagini = nrPagini;
	if (titlu != NULL) {
		c.titlu = titlu;
	}
	else {
		c.titlu = NULL;
	}
	return c;
}

void afisareCarte(struct Carte c) {
	printf("\n\n Cod carte: %d\n Titlu: %s Pret: %.2f\n Numar pagini: %d", c.cod,c.titlu,c.pret,c.nrPagini);
}

void afisareVector(struct Carte* vector, int nrElemente) {
	for (int i = 0; i < nrElemente; i++)
		afisareCarte(vector[i]);
}

//conidtie: sa fie sub un anumit pret -> prag 
void copiereCarti(struct Carte* vector, int nrCarti,int conditie,int * dimensiune,struct Carte** vectorNou) {
	(*dimensiune) = 0;
	for (int i = 0; i < nrCarti; i++) {
		if (vector[i].pret < conditie) {
			(*dimensiune)++;
		}
	}
	if (*dimensiune > 0) {
		int j = 0;
		*vectorNou = (struct Carte*)malloc(sizeof(struct Carte) * (*dimensiune));
		for (int i = 0; i < nrCarti; i++) {
			if (vector[i].pret < conditie) {
				(*vectorNou)[j] = vector[i];
				(*vectorNou)[j].titlu =(char*) malloc(sizeof(char) * (strlen(vector[i].titlu) + 1));
				strcpy((*vectorNou)[j].titlu, vector[i].titlu);
				j++;
			}
		}
	}
}

// conditie: un anumit nrPagini
void mutareCarti(struct Carte** vector, int* nrCarti, int pragPagini, struct Carte** vectorMutat, int* nrMutate) {
	*nrMutate = 0;
	for (int i = 0; i < *nrCarti; i++) {
		if ((*vector)[i].nrPagini > pragPagini) {
			(*nrMutate)++;
		}
	}
	*vectorMutat = (struct Carte*)malloc(sizeof(struct Carte) * (*nrMutate));
	struct Carte* vectorRamas = (struct Carte*)malloc(sizeof(struct Carte) * (*nrCarti - *nrMutate));

	int j = 0; // index vectorMutat
	int k = 0; // index vectorRamas

	for (int i = 0; i < *nrCarti; i++) {
		if ((*vector)[i].nrPagini > pragPagini) {
			(*vectorMutat)[j++] = (*vector)[i];  // mutare
		}
		else {
			vectorRamas[k++] = (*vector)[i];   // pastrare
		}
	}
	free(*vector); // dezalocare initial
	*nrCarti = *nrCarti - *nrMutate; // inlocuire
	*vector = vectorRamas;
}

struct Carte* concatenareVectori(struct Carte* vector1, int nr1, struct Carte* vector2, int nr2,int *dimensiune) {
	struct Carte* vectorNou = NULL;
	if (vector1 == NULL && vector2 == NULL) {
		return NULL;
	}
	if (vector1 == NULL) {
		return vector2;
	}
	if (vector2 == NULL) {
		return vector1;
	}
	*dimensiune = nr1 + nr2;
	vectorNou = (struct Carte*)malloc(sizeof(struct Carte) * (*dimensiune));
	for (int i = 0; i < nr1; i++) {
		vectorNou[i] = initializare(vector1[i].cod, vector1[i].titlu, vector1[i].pret, vector1[i].nrPagini);
	}
	for (int i = 0; i < nr2; i++) {
		vectorNou[nr1 + i] = initializare(vector2[i].cod, vector2[i].titlu, vector2[i].pret, vector2[i].nrPagini);
	}
	return vectorNou;
}

int main() {
	struct Carte* vectorCarti = NULL;
	int nrCarti = 5;
	vectorCarti = (struct Carte*)malloc(sizeof(struct Carte) * nrCarti);
	vectorCarti[0] = initializare(1,"Carte1",10,100);
	vectorCarti[1] = initializare(2, "Carte2", 20, 200);
	vectorCarti[2] = initializare(3, "Carte3", 30, 300);
	vectorCarti[3] = initializare(4, "Carte4", 40, 400);
	vectorCarti[4] = initializare(5, "Carte5", 50, 500);
	//afisareVector(vectorCarti,nrCarti);


	printf("\n\nCopiere daca pretul este mai mic:"); 
	struct Carte* vectorCopiere = NULL;
	int dimensiuneCopiere;
	copiereCarti(vectorCarti, nrCarti, 40, &dimensiuneCopiere, &vectorCopiere);
	//afisareVector(vectorCopiere,dimensiuneCopiere);

	printf("\n\nMutare in functie de nr pagini:");
	struct Carte* vectorMutare = NULL;
	int dimensiuneMutare;
	mutareCarti(&vectorCarti, &nrCarti, 150, &vectorMutare, &dimensiuneMutare);

	printf("\nCarti RAMASE:\n");
	//afisareVector(vectorCarti, nrCarti);

	printf("\nCarti MUTATE:\n");
	//afisareVector(vectorMutare, dimensiuneMutare);
	
	printf("\n\nConcatenare: ");
	int dimensiuneConcatenare;
	struct Carte* vectorConcatenare = concatenareVectori(vectorCarti, nrCarti, vectorCopiere,dimensiuneCopiere,&dimensiuneConcatenare);
	//afisareVector(vectorConcatenare,dimensiuneConcatenare);

	return 0;
}