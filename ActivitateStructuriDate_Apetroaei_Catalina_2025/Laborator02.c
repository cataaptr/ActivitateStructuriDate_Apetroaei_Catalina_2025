#include<stdio.h>
#include<stdlib.h>
#include <malloc.h>

struct Masina {
	int id;
	int nrLocuri ;
	char* marca;
	float capacitateC;
	char normaPoluare;
};

struct Masina initializare(int id, int nrLocuri, const char* marca, float capacitateC, char normaPoluare) {
	struct Masina m;
	m.id = id;
	m.nrLocuri = nrLocuri;
	if (marca != NULL) {
		m.marca = (char*)malloc(sizeof(char) * (strlen(marca) + 1));
		strcpy_s(m.marca, strlen(marca) + 1, marca);
	} else {
		m.marca = NULL;
	}
	m.capacitateC = capacitateC;
	m.normaPoluare = normaPoluare;
	return m;
}

void afisare(struct Masina m) {
	if (m.marca != NULL) {
		printf("\n Id masina: %d\n Nr locuri: %d\n Capacitate: %.2f\n Marca: %s\n Euro: %c", m.id, m.nrLocuri, m.capacitateC, m.marca, m.normaPoluare);
	}
	else {
		printf("\n Id masina: %d\n Nr locuri: %d\n Capacitate: %.2f\n Euro: %c", m.id, m.nrLocuri, m.capacitateC, m.normaPoluare);
	}
}

void afisareVector(struct Masina* vector, int nrElemente) {
	for (int i = 0; i < nrElemente; i++)
		afisare(vector[i]);
}

struct Masina* copiazaPrimeleNElemente(struct Masina* vector, int nrElemente, int nrElementeCopiate) {
	if (nrElementeCopiate > nrElemente)
				nrElementeCopiate = nrElemente;
	struct Masina *vectorNou=NULL;
	vectorNou = (struct Masina*)malloc(sizeof(struct Masina) * nrElementeCopiate);
	for (int i = 0; i < nrElementeCopiate; i++) {
		vectorNou[i] = initializare(vector[i].id, vector[i].nrLocuri, vector[i].marca, vector[i].capacitateC, vector[i].normaPoluare);
		// sau:
		// vectorNou[i] = vector[i];
		// vectorNou[i].marca = (char*)malloc(strlen(vector[i].marca) + 1);
		// strcpy(vectorNou[i].marca, strlen(vector[i].marca) + 1, vector[i].marca);
	}
	return vectorNou;
}

void dezalocare(struct Masina** vector, int* nrElemente) {
	for (int i = 0; i < *nrElemente; i++){
				free((*vector)[i].marca);
		}
	free(*vector);
	*vector = NULL;
	*nrElemente = 0;
}

void copiazaMasiniCuCCMare(struct Masina* vector, char nrElemente, float prag, struct Masina** vectorNou, int* dimensiune) {
	
	 *dimensiune = 0;
	 for (char i = 0; i < nrElemente; i++) {
		 if (prag < vector[i].capacitateC) {
			 (*dimensiune)++;
		 }
	 }
	 if (*dimensiune > 0) {
		 int j = 0;
		 *vectorNou = (struct Masina*)malloc((*dimensiune) * sizeof(struct Masina));
		 for (int i = 0; i < nrElemente; i++) {
			 if (prag < vector[i].capacitateC) {
				 (*vectorNou)[j] = vector[i];
				 (*vectorNou)[j].marca = malloc(sizeof(char)*(strlen(vector[i].marca)+1));
				 strcpy_s((*vectorNou)[j].marca, strlen(vector[i].marca) + 1, vector[i].marca);
				 j++;
			 }
		 }
	 }
}

struct Masina getPrimaMasinaDupaMarca(struct Masina* vector, int nrElemente, const char* conditie) {
	
	for (int i = 0; i < nrElemente; i++)
	{
		if (strcmp(vector[i].marca, conditie) == 0)
			return vector[i];
	}
	return initializare(1, 0, NULL, 0, '0');
}

int main() {
	struct Masina* vector=NULL;
	int nrMasini = 3;
	vector = (struct Masina*)malloc(sizeof(struct Masina) * nrMasini);
	vector[0] = initializare(1, 3, "Dacia", 40, '5');
	vector[1] = initializare(2, 5, "Honda", 30, '6');
	vector[2] = initializare(3, 4, "Dacia", 50, '3');
	//afisareVector(vector,nrMasini);

	printf("\nPrimele telefoane:");
	int nrElemCop = 2;
	struct Masina* vector2 = copiazaPrimeleNElemente(vector, nrMasini, nrElemCop);
	afisareVector(vector2, nrElemCop);

	printf("\nDezalocare:");
	dezalocare(&vector2, &nrElemCop);
	afisareVector(vector2, nrElemCop);


	printf("\nMasini cu capacitate mai mare:");
	copiazaMasiniCuCCMare(vector, nrMasini, 40, &vector2, &nrElemCop);
	afisareVector(vector2, nrElemCop);

	printf("\nMasina dupa marca: ");
	struct Masina m1 = getPrimaMasinaDupaMarca(vector, nrMasini, "Dacia");
	afisare(m1);

	return 0;
}