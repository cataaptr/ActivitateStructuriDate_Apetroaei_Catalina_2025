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

struct Carte initializareTastatura() {
	struct Carte c;

	// citire tastatura
	printf("Introduceti codul cartii: ");
	scanf("%d", &c.cod);

	printf("Introduceti pretul cartii: ");
	scanf("%f", &c.pret);

	printf("Introduceti numarul de pagini ale cartii: ");
	scanf("%d", &c.nrPagini);

	char buffer[100];
	printf("Introduceti titlu cartii: ");
	getchar();
	fgets(buffer, sizeof(buffer), stdin);
	c.titlu = (char*)malloc(strlen(buffer) + 1);
	if (c.titlu != NULL) {
		strcpy(c.titlu, buffer);
	}
	return c;
}

void afisareCarte(struct Carte c) {
	printf("\n Cod carte: %d\n Titlu: %s Pret: %.2f\n Numar pagini: %d", c.cod,c.titlu,c.pret,c.nrPagini);
}

float costPerPagina(struct Carte c) {
	if (c.nrPagini > 0 && c.pret > 0) {
		return c.pret / c.nrPagini;
	}
	return 0;
}

void modificarePret( struct Carte* c,float pretNou) {
	if (pretNou > 0) {
		c->pret = pretNou;
	}
}

int main() {
	struct Carte carte;
	carte=initializareTastatura();
	afisareCarte(carte);

	printf("\n Cost per pagina: %.2f",costPerPagina(carte));

	modificarePret(&carte,50);
	afisareCarte(carte);

	return 0;
}