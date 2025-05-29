#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//trebuie sa folositi fisierul masini.txt
//sau va creati un alt fisier cu alte date

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

//STACK
// LIFO
//  inserare si extragere la acelasi capat
// putem reprezenta o stiva prin LSI, LDI sau vector
// deplasarea se face doar cu POP

// pop & deq - iau elementul si l elimin
// cand vreau sa extrag trb sa le scot si sa le punem undeva

// lista simpla
typedef struct Nod Nod;
struct Nod {
	Masina info;
	Nod* next;
};

void pushStack(Nod** stiva, Masina masina) {
	Nod* nodNou = (Nod*)malloc(sizeof(Nod));
	nodNou->info = masina;
	nodNou->next = *stiva;
	*stiva = nodNou;
}

Masina popStack(Nod** stiva) {
	if ((*stiva) != NULL) {
		Masina m = (*stiva)->info;
		Nod* aux = *stiva;
		*stiva = (*stiva)->next;
		free(aux);
		return m;
	}
	Masina m1;
	m1.id = -1;
	return m1;
}

unsigned char emptyStack(Nod* stiva) { // merge si int
	return stiva == NULL;
}

void afisareStiva(Nod* stiva) {
	while (stiva) {
		afisareMasina(stiva->info);
		stiva = stiva->next;
	}
}

Nod* citireStackMasiniDinFisier(const char* numeFisier) {
	FILE* file = fopen(numeFisier, "r");
	Nod* lista = NULL;
	while (!feof(file)) {
		pushStack(&lista, citireMasinaDinFisier(file));
	}
	fclose(file);
	return lista;
}

void dezalocareStivaDeMasini(Nod** stiva) {
	/*while (*stiva) {
		Nod* aux = *stiva;
		*stiva = (*stiva)->next;
		free(aux->info.numeSofer);
		free(aux->info.model);
		free(aux);
	}*/

	// sau:
	while ((*stiva) != NULL) {
		Masina m = popStack(stiva);
		free(m.numeSofer);
		free(m.model);
	}
}

int size(Nod* stiva) {
	int nr = 0;
	if (stiva == NULL) {
		return 0;
	}
	else {
		while (stiva) { // aici nu mai mergem la stiva de next ptc mergem pana la final
			stiva = stiva->next;
			nr++;
		}
	}
	return nr;
}

//QUEUE
// FIFO
// putem reprezenta o coada prin LSI, LDI sau vector
// daca facem inserare la un capat, extragerea trb celalt

// lista dubla
typedef struct NodDublu NodDublu;
struct NodDublu {
	Masina info;
	Nod* next;
	Nod* prev;
};

typedef struct ListaDubla ListaDubla;
struct ListaDubla {
	NodDublu* prim;
	NodDublu* ultim;
};

// inserare la final
void enqueue(ListaDubla* coada, Masina masina) {
	NodDublu* nodNou = (NodDublu*)malloc(sizeof(NodDublu));
	nodNou->info = masina;
	nodNou->next = NULL;
	nodNou->prev = coada->ultim;
	if (coada->ultim != NULL) {
		coada->ultim->next = nodNou;
	}
	else {
		coada->prim = nodNou;
	}
	coada->ultim = nodNou;
}

// extragere inceput
Masina dequeue(ListaDubla* coada) {
	Masina m;
	NodDublu* aux = coada->prim;

	if (coada->ultim != NULL) {
		m = coada->prim->info;
		if (coada->prim->next != NULL) {
			coada->prim = coada->prim->next;
			coada->prim->prev = NULL;
		}
		else {//extragem ultimul nod si ramane lista goala
			coada->prim = NULL;
			coada->ultim = NULL;
		}
		//coada->prim = coada->prim->next;
		free(aux);
	}
	return m;
}

ListaDubla citireCoadaDeMasiniDinFisier(const char* numeFisier) {
	FILE* file = fopen(numeFisier, "r");
	ListaDubla coada;
	coada.prim = NULL;
	coada.ultim = NULL;
	while (!feof(file)) {
		enqueue(&coada, citireMasinaDinFisier(file));
	}
	fclose(file);
	return coada;
}

void afisareCoada(Nod* coada) {
	while (coada) {
		afisareMasina(coada->info);
		coada = coada->next;
	}
}

void dezalocareCoadaDeMasini(ListaDubla* coada) {
	while (coada->prim != NULL) {
		Masina m = dequeue(coada);
		free(m.model);
		free(m.numeSofer);
	}
}

Masina getMasinaByID(ListaDubla* coada, int id) {
	ListaDubla temp;
	temp.prim = NULL;
	temp.ultim = NULL;
	Masina gasita;
	gasita.id = -1; // implicit, nu e gasita

	while (coada->prim) {
		Masina m = dequeue(coada);
		if (m.id == id && gasita.id == -1) {
			gasita = m; // retinem referinta
		}
		enqueue(&temp, m); // pastram structura
	}
	// restauram coada originala
	coada->prim = temp.prim;
	coada->ultim = temp.ultim;

	return gasita;
}


float calculeazaPretTotal(ListaDubla* coada) {
	float suma = 0;
	ListaDubla nou;
	nou.prim = NULL;
	nou.ultim = NULL;
	while ((*coada).prim ){
		Masina m = dequeue(coada);
		suma += m.pret;
		enqueue(&nou, m);
	}
	coada->prim = nou.prim;
	coada->ultim = nou.ultim;
	return suma;
}

int main() {
	// stiva
	Nod* stiva = citireStackMasiniDinFisier("masini.txt");
	//printf("\nSize: %d \n", size(stiva));

	//afisareStiva(stiva);
	Masina ultimaDinStiva = popStack(&stiva);
	//afisareMasina(ultimaDinStiva);
	//trb sa stergem nume sofer ca raman mem leak
	free(ultimaDinStiva.model);
	free(ultimaDinStiva.numeSofer);


	dezalocareStivaDeMasini(&stiva);

	// coada 
	ListaDubla coada = citireCoadaDeMasiniDinFisier("masini.txt");
	Masina m = dequeue(&coada);
	afisareMasina(m);

	printf("\nAfisare total: %.2f \n", calculeazaPretTotal(&coada));


	int cautatID = 2;
	Masina mGasita = getMasinaByID(&coada, cautatID);

	if (mGasita.id != -1) {
		printf("\nMasina cu ID %d a fost gasita:\n", cautatID);
		afisareMasina(mGasita);
	}
	else {
		printf("\nNu exista masina cu ID %d in coada.\n", cautatID);
	}

	dezalocareCoadaDeMasini(&coada);

	return 0;
}