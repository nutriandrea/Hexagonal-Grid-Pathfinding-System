/*

	****************************************************
 	*                                                  *
	* Progetto  Algoritmi e Principi dell'Informatica  *
	*                                                  *
	*          realizzato da: Andrea Cacioppo          *
	*                                                  *
	*                    2024-2025                     *
	*                                                  *
	*            scritto con: Kate e Clion             *
	*    compilato con: GCC 11 su Ubuntu on Mac        *
	*                                                  *
	****************************************************

*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>


#define and &&
#define or ||
#define true 1
#define false 0

#define MAX_ROTTE 5
#define INF 999999


/*strutture mappa:
 Rotta: coordinate, costo, stato
 Cella: costo, rotte array, numero rotte
 Mappa: colonne e righe, celle

 */
typedef struct
{
    int dest_x, dest_y;
    int costo;
    int attiva;
} Rotta;

typedef struct
{
    int costo_uscita;
    Rotta rotte[MAX_ROTTE];
    int num_rotte;
} Cella;

typedef struct
{
    int colonn, righ;
    Cella *grid;
} Mappa;

typedef struct
{
    int x, y, dist;
} Nodo;


/*strutture BTS:
 QUEUE pila
 HEAP NOD nodo generico
 MIN HEAP nodo minimo
*/
typedef struct
{
    Nodo *info;
    int cima, coda, dim, cap;
} Queue;

typedef struct
{
    int x, y, cost;
} HeapNode;

typedef struct
{
    HeapNode *info;
    int size;
    int capacity;
}MinHeap;

/*inizializzo mappa*/
Mappa map = {0, 0, NULL};


/* FUNZIONI MAPPA*/

void inizializza(int c, int r)
{
    // controllo parametri
    if (c <= 0 or r <= 0) {
        printf("KO\n");
        return;}

    // controllo overflow nella moltiplicazione
    size_t cells = (size_t)c * (size_t)r;
    if (cells > SIZE_MAX / sizeof(Cella))
    {
        printf("KO\n");
        return;
    }
    // controllo se c'è già memoria allocata
    if (map.grid != NULL)
    {
        free(map.grid);
        map.grid = NULL;}

    map.colonn = c;
    map.righ = r;
    map.grid = (Cella*)malloc(c * r * sizeof(Cella));

    //controllo su creazione mappa
    if (!map.grid) {
        printf("KO\n");
        return;}

    // inizializzo tutte le celle una per una
    int total = c * r;
    int i;
    for (i = 0; i < total; i++)
    {   map.grid[i].costo_uscita = 1;
        map.grid[i].num_rotte = 0;

        // inizializzo anche tutte le rotte (doppio ciclo annidato)
        int j;
        for (j = 0; j < MAX_ROTTE; j++) {
            map.grid[i].rotte[j].attiva = 0;
            map.grid[i].rotte[j].dest_x = -1;
            map.grid[i].rotte[j].dest_y = -1;
            map.grid[i].rotte[j].costo = 0; }}

    //risposta crezione mappa
    printf("OK\n");}


//funzione ausiliara conversione - da rettangolare a  esagonale
int trova_cella(int x, int y) {
    if (x < 0 or y < 0 or x >= map.colonn or y >= map.righ) {
        return -1;}
    return y * map.colonn + x;
}


// trovavicini trovo tutti i vicini
int trova_vicini(int x, int y, int vicini[6][2]) {
    int trovat = 0;

    // per le righe pari i vicini sono diversi da quelle dispari
    if (y % 2 == 0) {

        // sinistra
        if (x > 0) { vicini[trovat][0] = x - 1; vicini[trovat][1] = y; trovat++; }

        // destra
        if (x < map.colonn - 1)
        { vicini[trovat][0] = x + 1; vicini[trovat][1] = y; trovat++; }

        // alto sinistra
        if (y > 0 and x > 0)
        { vicini[trovat][0] = x - 1; vicini[trovat][1] = y - 1; trovat++;}

        // alto destra
        if (y > 0) { vicini[trovat][0] = x; vicini[trovat][1] = y - 1; trovat++; }

        // basso sinistra
        if (y < map.righ - 1 and x > 0) {vicini[trovat][0] = x - 1; vicini[trovat][1] = y + 1; trovat++;}

        // basso destra
        if (y < map.righ - 1) { vicini[trovat][0] = x;  vicini[trovat][1] = y + 1; trovat++; }
    }

    else

    {

        // per le righe dispari
        // sinistra
        if (x > 0) { vicini[trovat][0] = x - 1; vicini[trovat][1] = y; trovat++;}


        // destra
        if (x < map.colonn - 1) { vicini[trovat][0] = x + 1; vicini[trovat][1] = y; trovat++;}


        // alto sinistra
        if (y > 0) { vicini[trovat][0] = x; vicini[trovat][1] = y - 1; trovat++;}


        // alto destra
        if (y > 0 and x < map.colonn - 1) { vicini[trovat][0] = x + 1; vicini[trovat][1] = y - 1; trovat++;}


        // basso sinistra
        if (y < map.righ - 1) { vicini[trovat][0] = x; vicini[trovat][1] = y + 1; trovat++;}

        // basso dest ra
        if (y < map.righ - 1 and x < map.colonn - 1) { vicini[trovat][0] = x + 1; vicini[trovat][1] = y + 1; trovat++;} }


    return trovat; //numero vicini
}

int getCost(int x, int y) {
    int idx = trova_cella(x, y);
    if (idx == -1) return -1;
    return map.grid[idx].costo_uscita;
}

void setCost(int x, int y, int cost) {
        int indice = trova_cella(x, y);

// se non trovo torno -1
if (indice == -1) {
return;
}

if (cost < 0) {
cost = 0;  // non negativo
}
if (cost > 100) {
cost = 100;  // no sopra 100
}
//nuovo costo uscita

map.grid[y * map.colonn + x].costo_uscita = cost;
}

//funzioni queue

void queue_inizz(Queue *q, int cap) {
    q->info = malloc(cap * sizeof(Nodo));
    q->cima = 0;
    q->coda = 0;
    q->dim = 0;
    q->cap = cap; }

void aggiungi_queue(Queue *q, int x, int y, int d) {
    if (q->dim >= q->cap) return;
    q->info[q->coda].x = x;
    q->info[q->coda].y = y;
    q->info[q->coda].dist = d;
    q->coda = (q->coda + 1) % q->cap;
    q->dim++; }

Nodo sfila_queue(Queue *q) {
    Nodo result = q->info[q->cima];
    q->cima = (q->cima + 1) % q->cap;
    q->dim--;
    return result;}

bool queue_vuot(Queue *q) {
    return q->dim == 0;}


// Funzione per calcolare distanza esagonale BFS
int calcola_distanza_esagonale(int x1, int y1, int x2, int y2) {
    if (x1 == x2 && y1 == y2) return 0;

    // Alloca memoria per visitato
    bool *visitato = malloc(map.colonn * map.righ * sizeof(bool));
    if (!visitato) return -1;

    for (int i = 0; i < map.colonn * map.righ; i++) {
        visitato[i] = false;
    }

    Queue q;
    queue_inizz(&q, map.colonn * map.righ);
    aggiungi_queue(&q, x1, y1, 0);
    visitato[y1 * map.colonn + x1] = true;

    while (!queue_vuot(&q)) {
        Nodo curr = sfila_queue(&q);

        int vicini[6][2];
        int num_vicini = trova_vicini(curr.x, curr.y, vicini);

        for (int i = 0; i < num_vicini; i++) {
            int nx = vicini[i][0];
            int ny = vicini[i][1];
            int idx = ny * map.colonn + nx;

            if (nx == x2 && ny == y2) {
                free(visitato);
                free(q.info);
                return curr.dist + 1;
            }

            if (!visitato[idx]) {
                visitato[idx] = true;
                aggiungi_queue(&q, nx, ny, curr.dist + 1);
            }
        }
    }

    free(visitato);
    free(q.info);
    return -1; // Non raggiungibile
}

/* FUNZIONI NODI*/

void changeCost(int startx, int starty, int val, int raggio)
{
    if (startx < 0 or starty < 0 or startx >= map.colonn or starty >= map.righ or
        raggio <= 0 or val < -10 or val > 10)
    {
        printf("KO\n");
        return;
    }

    // Usa UN SOLO BFS per trovare tutte le celle nel raggio
    bool *visitato = malloc(map.colonn * map.righ * sizeof(bool));
    if (!visitato) {
        printf("KO\n");
        return;
    }

    for (int i = 0; i < map.colonn * map.righ; i++) {
        visitato[i] = false;
    }

    Queue q;
    queue_inizz(&q, map.colonn * map.righ);
    aggiungi_queue(&q, startx, starty, 0);
    visitato[starty * map.colonn + startx] = true;

    while (!queue_vuot(&q)) {
        Nodo curr = sfila_queue(&q);
        int dist = curr.dist;

        if (dist < raggio) {
            float reduction_factor = (float)(raggio - dist) / raggio;
            int cost_change = (int)floor(val * reduction_factor);

            int curr_cost = getCost(curr.x, curr.y);
            int nuovo_costo = curr_cost + cost_change;
            if (nuovo_costo < 0) nuovo_costo = 0;
            if (nuovo_costo > 100) nuovo_costo = 100;
            setCost(curr.x, curr.y, nuovo_costo);

            // Aggiorna rotte aeree
            int cell_index = curr.y * map.colonn + curr.x;
            Cella *curr_cell = &map.grid[cell_index];
            for (int i = 0; i < MAX_ROTTE; i++) {
                if (curr_cell->rotte[i].attiva) {
                    int old_route_cost = curr_cell->rotte[i].costo;
                    int new_route_cost = old_route_cost + cost_change;
                    if (new_route_cost <= 0) new_route_cost = 1;
                    if (new_route_cost > 100) new_route_cost = 100;
                    curr_cell->rotte[i].costo = new_route_cost;
                }
            }
        }

        // Espandi ai vicini se possiamo ancora andare più lontano
        if (dist + 1 <= raggio) {
            int vicini[6][2];
            int num_vicini = trova_vicini(curr.x, curr.y, vicini);

            for (int i = 0; i < num_vicini; i++) {
                int nx = vicini[i][0];
                int ny = vicini[i][1];
                int idx = ny * map.colonn + nx;

                if (!visitato[idx]) {
                    visitato[idx] = true;
                    aggiungi_queue(&q, nx, ny, dist + 1);
                }
            }
        }
    }

    free(visitato);
    free(q.info);
    printf("OK\n");
}


void toggle_air_route(int x1, int y1, int x2, int y2) {
    // controllo coordinate valide
    if (x1 < 0 or x1 >= map.colonn or y1 < 0 or y1 >= map.righ or
        x2 < 0 or x2 >= map.colonn or y2 < 0 or y2 >= map.righ)
    {
        printf("KO\n");
        return;
    }

    int partenza = y1 * map.colonn + x1;
    Cella *cella_origine = &map.grid[partenza];

    // cerco se la rotta esiste già
    int i;
    for (i = 0; i < MAX_ROTTE; i++)
    {
        if (cella_origine->rotte[i].attiva and
            cella_origine->rotte[i].dest_x == x2 and
            cella_origine->rotte[i].dest_y == y2)
        {
            // esiste: la disattivo
            cella_origine->rotte[i].attiva = 0;
            cella_origine->num_rotte--;
            printf("OK\n");
            return;
        }}

    // se ha già MAX_ROTTE rotte attive non può aggiungerne altre
    if (cella_origine->num_rotte >= MAX_ROTTE)
    {
        printf("KO\n");
        return;
    }

    // calcolo la media dei costi (uscita + rotte attive)
    int totale = cella_origine->costo_uscita;
    int conteggio = 1; // includo sempre il costo uscita
    for (i = 0; i < MAX_ROTTE; i++) {
        if (cella_origine->rotte[i].attiva)
        {
            totale += cella_origine->rotte[i].costo;
            conteggio++;
        }}

    int nuovo_costo = totale / conteggio; // media intera

    // controllo i limiti
    if (nuovo_costo < 0)      nuovo_costo = 0;
    else if (nuovo_costo > 100) nuovo_costo = 100;

    // trovo uno slot libero
    for (i = 0; i < MAX_ROTTE; i++) {
        if (!cella_origine->rotte[i].attiva) {
            cella_origine->rotte[i].dest_x = x2;
            cella_origine->rotte[i].dest_y = y2;
            cella_origine->rotte[i].costo = nuovo_costo;
            cella_origine->rotte[i].attiva = 1;
            cella_origine->num_rotte++;
            printf("OK\n");
            return;}}

    printf("KO\n"); // se per qualche motivo non trova slot
}



/* FUNZIONI HEAP*/

void heap_init(MinHeap *h, int cap)
{
    h->info = malloc(cap * sizeof(HeapNode));
    h->size = 0;
    h->capacity = cap;}

void heap_push(MinHeap *h, int x, int y, int cost)
{
    if (h->size >= h->capacity)
    {
        return;
    }

    int i = h->size;
    h->info[i].x = x;
    h->info[i].y = y;
    h->info[i].cost = cost;
    h->size++;

    while (i > 0) {
        int p = (i - 1) / 2;
        if (h->info[i].cost < h->info[p].cost) {
            HeapNode temp = h->info[i];
            h->info[i] = h->info[p];
            h->info[p] = temp;
            i = p;
        }
        else
        {
            break;
        }}}

HeapNode heap_pop(MinHeap *h)
{
    HeapNode root = {0,0,0};

    if (h->size == 0)
    {
        return root;
    }

    root = h->info[0];
    h->info[0] = h->info[h->size - 1];
    h->size--;

    int i = 0;
    while (1) {
        int l = 2*i + 1;
        int r = 2*i + 2;
        int min = i;

        if (l < h->size and h->info[l].cost < h->info[min].cost)
            min = l;
        if (r < h->size and h->info[r].cost < h->info[min].cost)
            min = r;

        if (min != i)
        {
            HeapNode temp = h->info[i];
            h->info[i] = h->info[min];
            h->info[min] = temp;
            i = min;
        }
        else
        {
            break;
        }}

    return root;}


bool heap_empty(MinHeap *h) {
    if (h == NULL)
        return true;
    else return h->size == 0;}


int heap_size(MinHeap *h) {
    if (h == NULL)
        return 0;
    return h->size;}

void free_heap(MinHeap *h) {
    if (h and h->info) {
        free(h->info);
        h->info = NULL;
        h->size = 0;
        h->capacity = 0;}}


/* FUNZIONI TRAVEL COST*/
void travel_cost(int x1, int y1, int x2, int y2) {
    // controllo coordinate valide
    if (x1 < 0 || x1 >= map.colonn || y1 < 0 || y1 >= map.righ ||
        x2 < 0 || x2 >= map.colonn || y2 < 0 || y2 >= map.righ) {
        printf("-1\n");
        return;
    }

    if (x1 == x2 && y1 == y2) {
        printf("0\n");
        return;
    }

    // inizializzo la tabella dei nodi
    int celle_tot = map.righ * map.colonn;

    int *distanze = malloc(celle_tot * sizeof(int));
    bool *visitato = malloc(celle_tot * sizeof(bool));

    // controllo che l'allocazione sia andata a buon fine
    if (!distanze || !visitato) {
        if (distanze) free(distanze);
        if (visitato) free(visitato);
        printf("-1\n");
        return;
    }

    // definisco l'heap minimo
    MinHeap heap;
    heap_init(&heap, celle_tot * 4); // heap più grande per gestire duplicati

    //inizializzo distanze e visitato
    for (int i = 0; i < celle_tot; i++) {
        distanze[i] = INF;
        visitato[i] = false;
    }

    int cella_iniz = trova_cella(x1, y1);
    distanze[cella_iniz] = 0;
    heap_push(&heap, x1, y1, 0);

    bool found = false;
    int result = -1;

    while (!heap_empty(&heap) && !found) {
        // prendo nodo con distanza minore
        HeapNode curr = heap_pop(&heap);

        int curr_x = curr.x;
        int curr_y = curr.y;
        int curr_cost = curr.cost;

        //calcolo indice cella corrente
        int curr_cell = trova_cella(curr_x, curr_y);

        //verifico se ho già visitato - IMPORTANTE: ignora nodi già visitati
        if (visitato[curr_cell]) {
            continue;
        }

        // IMPORTANTE: verifica che il costo estratto corrisponda alla distanza ottimale
        if (curr_cost > distanze[curr_cell]) {
            continue;
        }

        // marco come visitato
        visitato[curr_cell] = true;

        //se sono a destinazione mi fermo
        if (curr_x == x2 && curr_y == y2) {
            result = curr_cost;
            found = true;
            break;
        }

        int costo_uscita_corrente = getCost(curr_x, curr_y);

        // controllo i vicini normali (solo se posso uscire da questa cella)
        if (costo_uscita_corrente > 0) {
            int vicini[6][2];
            int num_vicini = trova_vicini(curr_x, curr_y, vicini);

            for (int j = 0; j < num_vicini; j++) {
                int x_vicino = vicini[j][0];
                int y_vicino = vicini[j][1];
                int indice_vicino = trova_cella(x_vicino, y_vicino);

                // IMPORTANTE: non processare vicini già visitati
                if (visitato[indice_vicino]) {
                    continue;
                }

                // calcolo nuovo costo per raggiungere il vicino
                int nuovo_costo = curr_cost + costo_uscita_corrente;

                // IMPORTANTE: verifica overflow
                if (nuovo_costo < curr_cost) {
                    continue; // overflow, ignora
                }

                // se ho trovato un percorso migliore
                if (nuovo_costo < distanze[indice_vicino]) {
                    distanze[indice_vicino] = nuovo_costo;
                    heap_push(&heap, x_vicino, y_vicino, nuovo_costo);
                }
            }
        }

        // controllo le rotte aeree da questa cella
        if (costo_uscita_corrente > 0) {
            int indice_cella = curr_y * map.colonn + curr_x;
            Cella *cella_corrente = &map.grid[indice_cella];

            for (int k = 0; k < MAX_ROTTE; k++) {
                if (cella_corrente->rotte[k].attiva) {
                    int x_dest = cella_corrente->rotte[k].dest_x;
                    int y_dest = cella_corrente->rotte[k].dest_y;
                    int costo_rotta = cella_corrente->rotte[k].costo;
                    int indice_dest = trova_cella(x_dest, y_dest);

                    // IMPORTANTE: verifica che la destinazione sia valida
                    if (x_dest < 0 || x_dest >= map.colonn ||
                        y_dest < 0 || y_dest >= map.righ) {
                        continue;
                    }

                    // IMPORTANTE: non processare destinazioni già visitate
                    if (visitato[indice_dest]) {
                        continue;
                    }

                    // controllo che la rotta abbia costo > 0
                    if (costo_rotta > 0) {
                        // calcolo nuovo costo per la rotta aerea
                        int nuovo_costo = curr_cost + costo_rotta;

                        // IMPORTANTE: verifica overflow
                        if (nuovo_costo < curr_cost) {
                            continue; // overflow, ignora
                        }

                        // se ho trovato un percorso migliore
                        if (nuovo_costo < distanze[indice_dest]) {
                            distanze[indice_dest] = nuovo_costo;
                            heap_push(&heap, x_dest, y_dest, nuovo_costo);
                        }
                    }
                }
            }
        }
    }

    // stampa risultato
    printf("%d\n", result);

    // cleanup
    free(distanze);
    free(visitato);
    free_heap(&heap);
}



int main(void)
{
    char linea[100];

    // leggo i comandi fino a quando non finiscono
    while (fgets(linea, 100, stdin) != NULL)
    {
        // tolgo il carattere di a capo alla fine
        int i = 0;

        while (linea[i] != '\0')
        {if (linea[i] == '\n')
            {linea[i] = '\0';
                break;}
            i++;}

        // variabili per i comandi
        int colonne;
        int righe;
        int x1, y1;
        int x2, y2;
        int x, y;
        int valore;
        int raggio;

        // controllo che tipo di comando è
        if (sscanf(linea, "init %d %d", &colonne, &righe) == 2)
        {inizializza(colonne, righe);
        }
        else if (sscanf(linea, "toggle_air_route %d %d %d %d", &x1, &y1, &x2, &y2) == 4)
        {toggle_air_route(x1, y1, x2, y2);
        }

        else if (sscanf(linea, "change_cost %d %d %d %d", &x, &y, &valore, &raggio) == 4)
        {changeCost(x, y, valore, raggio);}
        else if (sscanf(linea, "travel_cost %d %d %d %d", &x1, &y1, &x2, &y2) == 4)
        {travel_cost(x1, y1, x2, y2);
        }
    }




    //libero la memoria
    if (map.grid != NULL)
    {
        free(map.grid);
        map.grid = NULL;
    }

    return 0;
}

