#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structura Cladire
typedef struct Cladire {
    int id;
    char* adresa;
    int anConstructie;
    int nrEtaje;
} Cladire;

// Nod pentru lista inlantuita
typedef struct Nod {
    Cladire info;
    struct Nod* next;
} Nod;

// Tabela de dispersie cu vector de liste
typedef struct {
    int dim;
    Nod** vector;
} HashTable;

int calculeazaHash(int an, int dim) {
    return an % dim;
}

HashTable initializareHashTable(int dim) {
    HashTable ht;
    ht.dim = dim;
    ht.vector = (Nod**)malloc(sizeof(Nod*) * dim);
    for (int i = 0; i < dim; i++) {
        ht.vector[i] = NULL;
    }
    return ht;
}

void adaugaCladireLaFinal(Nod** cap, Cladire c) {
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->info = c;
    nou->next = NULL;

    if (*cap == NULL) {
        *cap = nou;
    }
    else {
        Nod* p = *cap;
        while (p->next) {
            p = p->next;
        }
        p->next = nou;
    }
}

void inserareCladire(HashTable* ht, Cladire c) {
    int poz = calculeazaHash(c.anConstructie, ht->dim);
    adaugaCladireLaFinal(&ht->vector[poz], c);
}

void afisareCladire(Cladire c) {
    printf("\nID: %d", c.id);
    printf("\nAdresa: %s", c.adresa);
    printf("\nAn: %d", c.anConstructie);
    printf("\nEtaje: %d\n", c.nrEtaje);
}

void afisareCluster(HashTable ht, int an) {
    int poz = calculeazaHash(an, ht.dim);
    Nod* p = ht.vector[poz];
    while (p) {
        if (p->info.anConstructie == an)
            afisareCladire(p->info);
        p = p->next;
    }
}

void stergeCladire(HashTable* ht, int id, int an) {
    int poz = calculeazaHash(an, ht->dim);
    Nod* p = ht->vector[poz];
    Nod* anterior = NULL;
    while (p) {
        if (p->info.id == id) {
            if (anterior)
                anterior->next = p->next;
            else
                ht->vector[poz] = p->next;
            if (p->info.adresa != NULL)
                free(p->info.adresa);
            free(p);
            return;
        }
        anterior = p;
        p = p->next;
    }
}

void stergeCladireDupaID(HashTable* ht, int id) {
    for (int i = 0; i < ht->dim; i++) {
        Nod* p = ht->vector[i];
        Nod* anterior = NULL;
        while (p) {
            if (p->info.id == id) {
                if (anterior)
                    anterior->next = p->next;
                else
                    ht->vector[i] = p->next;
                free(p->info.adresa);
                free(p);
                return;
            }
            anterior = p;
            p = p->next;
        }
    }
}

Cladire* cladiriDinAn(HashTable ht, int an, int* nr) {
    *nr = 0;
    int poz = calculeazaHash(an, ht.dim);
    Nod* p = ht.vector[poz];

    while (p) {
        if (p->info.anConstructie == an)
            (*nr)++;
        p = p->next;
    }

    if (*nr == 0) return NULL;

    Cladire* vector = (Cladire*)malloc(sizeof(Cladire) * (*nr));
    int k = 0;
    p = ht.vector[poz];
    while (p) {
        if (p->info.anConstructie == an) {
            vector[k].id = p->info.id;
            vector[k].anConstructie = p->info.anConstructie;
            vector[k].nrEtaje = p->info.nrEtaje;
            vector[k].adresa = (char*)malloc(strlen(p->info.adresa) + 1);
            strcpy(vector[k].adresa, p->info.adresa);
            k++;
        }
        p = p->next;
    }
    return vector;
}

void modificaAnConstructie(HashTable* ht, int id, int vechiAn, int nouAn) {
    int poz = calculeazaHash(vechiAn, ht->dim);
    Nod* p = ht->vector[poz];
    Nod* anterior = NULL;
    while (p) {
        if (p->info.id == id) {
            if (anterior)
                anterior->next = p->next;
            else
                ht->vector[poz] = p->next;
            Cladire temp = p->info;
            temp.anConstructie = nouAn;
            inserareCladire(ht, temp);
            free(p);
            return;
        }
        anterior = p;
        p = p->next;
    }
}

int main() {
    HashTable ht = initializareHashTable(5);

    Cladire c1;
    c1.id = 1;
    c1.anConstructie = 1977;
    c1.nrEtaje = 8;
    c1.adresa = malloc(strlen("Str. Aviatorilor 10") + 1);
    strcpy(c1.adresa, "Str. Aviatorilor 10");

    Cladire c2;
    c2.id = 2;
    c2.anConstructie = 1985;
    c2.nrEtaje = 5;
    c2.adresa = malloc(strlen("Str. Dorobanti 5") + 1);
    strcpy(c2.adresa, "Str. Dorobanti 5");

    Cladire c3;
    c3.id = 3;
    c3.anConstructie = 1977;
    c3.nrEtaje = 10;
    c3.adresa = malloc(strlen("Str. Unirii 23") + 1);
    strcpy(c3.adresa, "Str. Unirii 23");

    Cladire c4;
    c4.id = 4;
    c4.anConstructie = 1990;
    c4.nrEtaje = 6;
    c4.adresa = malloc(strlen("Str. Victoriei 45") + 1);
    strcpy(c4.adresa, "Str. Victoriei 45");

    inserareCladire(&ht, c1);
    inserareCladire(&ht, c2);
    inserareCladire(&ht, c3);
    inserareCladire(&ht, c4);

    printf("\nCladiri construite in 1977");
    afisareCluster(ht, 1977);

    printf("\nCladiri construite in 1990");
    afisareCluster(ht, 1990);


    printf("\nStergere cladire ID 1 si an 1977\n");
    //stergeCladire(&ht, 1, 1977);
    //afisareCluster(ht, 1977);

    printf("\nStergere cladire ID 2\n");
    //stergeCladireDupaID(&ht,2);
    //afisareCluster(ht, 1985);


    printf("\nVector cladiri din 1977\n");
    int nr = 0;
    Cladire* v = cladiriDinAn(ht, 1977, &nr);
    for (int i = 0; i < nr; i++) {
        afisareCladire(v[i]);
        free(v[i].adresa);
    }
    free(v);

    
    printf("\nModificare cladire cu ID 3 -> an 2000 \n");
    modificaAnConstructie(&ht, 3, 1977, 2000);
    afisareCluster(ht, 1977);
    afisareCluster(ht, 2000); 

    

    return 0;
}