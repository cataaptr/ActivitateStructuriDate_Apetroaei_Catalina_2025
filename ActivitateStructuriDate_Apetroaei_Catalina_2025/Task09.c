#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char* nume;
    int varsta;
    char* simptome;
    int gradUrgenta; 
} Pacient;

typedef struct {
    Pacient* vect;
    int dim;
    int capacitate;
} Heap;


Pacient crearePacient(const char* nume, int varsta, const char* simptome, int gradUrgenta) {
    Pacient p;
    p.nume = malloc(strlen(nume) + 1);
    strcpy(p.nume, nume);

    p.varsta = varsta;

    p.simptome = malloc(strlen(simptome) + 1);
    strcpy(p.simptome, simptome);

    p.gradUrgenta = gradUrgenta;

    return p;
}

void afisarePacient(Pacient p) {
    printf("Nume: %s, Varsta: %d, Simptome: %s, Grad urgenta: %d\n",
        p.nume, p.varsta, p.simptome, p.gradUrgenta);
}

void dezalocarePacient(Pacient* p) {
    free(p->nume);
    free(p->simptome);
}

void initHeap(Heap* h, int capacitate) {
    h->vect = malloc(sizeof(Pacient) * capacitate);
    h->dim = 0;
    h->capacitate = capacitate;
}

void redimensionareHeap(Heap* h) {
    h->capacitate *= 2;
    h->vect = realloc(h->vect, sizeof(Pacient) * h->capacitate);
}

void swap(Pacient* a, Pacient* b) {
    Pacient temp = *a;
    *a = *b;
    *b = temp;
}

void filtrareBottomToTop(Heap* h, int index) {
    if (index > 0) {
        int parent = (index - 1) / 2;
        if (h->vect[index].gradUrgenta > h->vect[parent].gradUrgenta) {
            swap(&h->vect[index], &h->vect[parent]);
            filtrareBottomToTop(h, parent);
        }
    }
}

void inserareHeap(Heap* h, Pacient p) {
    if (h->dim == h->capacitate) {
        redimensionareHeap(h);
    }
    h->vect[h->dim] = p;
    filtrareBottomToTop(h, h->dim);
    h->dim++;
}

void filtrareTopToBottom(Heap* h, int index) {
    int st = 2 * index + 1;
    int dr = 2 * index + 2;
    int max = index;

    if (st < h->dim && h->vect[st].gradUrgenta > h->vect[max].gradUrgenta)
        max = st;
    if (dr < h->dim && h->vect[dr].gradUrgenta > h->vect[max].gradUrgenta)
        max = dr;

    if (max != index) {
        swap(&h->vect[max], &h->vect[index]);
        filtrareTopToBottom(h, max);
    }
}

Pacient extragePacientUrgent(Heap* h) {
    if (h->dim == 0) {
        Pacient gol = { NULL, 0, NULL, -1 };
        return gol;
    }
    Pacient rezultat = h->vect[0];
    h->vect[0] = h->vect[h->dim - 1];
    h->dim--;
    filtrareTopToBottom(h, 0);
    return rezultat;
}

void afisareHeap(Heap h) {
    for (int i = 0; i < h.dim; i++) {
        afisarePacient(h.vect[i]);
    }
}

void dezalocareHeap(Heap* h) {
    for (int i = 0; i < h->dim; i++) {
        dezalocarePacient(&h->vect[i]);
    }
    free(h->vect);
}

int main() {
    Heap heapUrgenta;
    initHeap(&heapUrgenta, 5);

    inserareHeap(&heapUrgenta, crearePacient("Andrei", 35, "durere piept", 7));
    inserareHeap(&heapUrgenta, crearePacient("Maria", 60, "accident rutier", 10)); // f urgent
    inserareHeap(&heapUrgenta, crearePacient("Ioana", 28, "tuse", 3));
    inserareHeap(&heapUrgenta, crearePacient("Vlad", 50, "febra", 5));

    printf("Lista pacienti in asteptare: \n");
    afisareHeap(heapUrgenta);

    printf("\n Pacient preluat (cel mai urgent): \n");
    Pacient preluat = extragePacientUrgent(&heapUrgenta);
    afisarePacient(preluat);
    dezalocarePacient(&preluat);

    printf("\n Lista ramasa in heap dupa preluare: \n");
    afisareHeap(heapUrgenta);

    dezalocareHeap(&heapUrgenta);
    return 0;
}
