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
		printf("\n Id masina: %d\n Nr locuri: %d\n Capacitate: %2.f\n Marca: %s\n Euro: %c", m.id, m.nrLocuri, m.capacitateC, m.marca, m.normaPoluare);
	}
	else {
		printf("\n Id masina: %d\n Nr locuri: %d\n Capacitate: %2.f\n Euro: %c", m.id, m.nrLocuri, m.capacitateC, m.normaPoluare);
	}
}

void modificaNrLocuri(struct Masina* m, int nrNou) {
	if (nrNou > 0) {
		m->nrLocuri = nrNou;
	}
}

void dezalocare(struct Masina* m) {
	if (m->marca != NULL) {
		free(m->marca);
		//(*m).marca = NULL; sau:
		m->marca = NULL;
	}
}

int main() {
	struct Masina masina;
	masina = initializare(1, 5, "Dacia", 1.5, '4');
	afisare(masina);
	modificaNrLocuri(&masina,2);
	//afisare(masina);
	dezalocare(&masina);
	afisare(masina);
	//printf("\n%d", sizeof(struct Masina*));
	//printf("\n%d", sizeof(masina));

	return 0;
}