#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 50000

typedef struct Solution {
    long station;
    struct Solution *next;
    struct Solution *prev;
} solution_t;

typedef struct Station {
    long distance;
    long parked_cars[512];
    int visited;
    struct Solution *reachable;
} station_t;

long most_distant_station = -1;
station_t *stations_table[SIZE];
char *token = NULL, *trash = NULL, input[100];

solution_t *add_list(long, solution_t *);

solution_t *remove_list(long, solution_t *);

void print_stations();

int add_station();

int add_car();

int remove_station();

int remove_car();

int main() {
    int i = 0;
    for (i = 0; i < SIZE; i++) stations_table[i] = NULL;
    while (fscanf(stdin, "%s", input) != EOF) {


        if (strcmp(input, "aggiungi-stazione") == 0) {
            if (add_station() == 1) printf("non ");
            printf("aggiunta\n");
            //print_stations();
        } else if (strcmp(input, "demolisci-stazione") == 0) {
            if (remove_station() == 1) printf("non ");
            printf("rottamata\n");
        } else if (strcmp(input, "aggiungi-auto") == 0) {

        } else if (strcmp(input, "rottama-auto") == 0) {

        } else if (strcmp(input, "pianifica-percorso") == 0) {
            //print_stations(stations_table);
        }

    }
    return 0;
}

int add_station() {
    long n_auto = 0, distance = 0, new_car = 0;
    station_t *new_station = NULL, *tmp = NULL;
    int i = 0, j = 0, pos = 0, flag = 0;
    long li = 0;

    if (fscanf(stdin, "%s", input) == EOF) return 1;
    distance = atol(input);
    if (stations_table[distance] != NULL) return 1;
    if (fscanf(stdin, "%s", input) == EOF) return 1;
    n_auto = atol(input);

    new_station = (station_t *) malloc(sizeof(station_t));
    new_station->reachable = NULL;
    new_station->distance = distance;
    for (i = 0; i < 512; i++) new_station->parked_cars[i] = 0;
    new_station->visited = 0;
    stations_table[distance] = new_station;

    if (most_distant_station == -1 || most_distant_station < distance) most_distant_station = distance;

    //check for other stations
    for (li = 0; li <= most_distant_station; li++) {

        if (stations_table[li] != NULL && li != distance) {
            tmp = stations_table[li];
            if (li < distance && tmp->parked_cars[0] + li > distance) {
                tmp->reachable = add_list(distance, tmp->reachable);
            }
            if (li > distance && li - tmp->parked_cars[0] < distance) {
                tmp->reachable = add_list(distance, tmp->reachable);
            }
        }
    }
    if (distance == 1163) print_stations();
    if (n_auto == 0) return 0;

    for (i = 0; i < n_auto; i++) {
        flag = 0;
        if (fscanf(stdin, "%s", input) == EOF) return 1;
        new_car = atol(input);
        for (j = 0; j < 512; j++) {
            if (new_station->parked_cars[j] == new_car) flag = 1;
            if (new_station->parked_cars[j] < new_car) break;
        }
        if (flag == 0) {
            pos = j;
            for (j = 511; j >= pos; j--) new_station->parked_cars[j + 1] = new_station->parked_cars[j];
            new_station->parked_cars[pos] = new_car;
        }
    }

    //check for itself
    li = 0;
    if (distance > new_station->parked_cars[0]) li = distance - new_station->parked_cars[0];
    while (li <= distance + new_station->parked_cars[0]) {

        if (stations_table[li] != NULL && li != distance) new_station->reachable = add_list(li, new_station->reachable);
        li++;
    }
    return 0;
}

int remove_station() {
    long distance = 0, i = 0;
    if (fscanf(stdin, "%s", input) == EOF) return 1;
    distance = atol(input);
    printf("REMOVING: %lu", distance);
    if (stations_table[distance] == NULL) return 1;
    stations_table[distance] = NULL;

    for (i = 0; i <= most_distant_station; i++) {
        if (stations_table[i] != NULL) {
            stations_table[i]->reachable = remove_list(distance, stations_table[i]->reachable);
            if (i - stations_table[i]->parked_cars[0] <= distance &&
                i + stations_table[i]->parked_cars[0] >= distance) {

                stations_table[i]->reachable = remove_list(distance, stations_table[i]->reachable);
            }
        }
    }
    return 0;
}



solution_t *add_list(long distance, solution_t *head) {
    solution_t *p = NULL, *tmp = NULL;
    if (head == NULL) {
        head = (solution_t *) malloc(sizeof(solution_t));
        head->next = NULL;
        head->prev = NULL;
        head->station = distance;
        return head;
    }
    p = head;
    while (p->next != NULL && p->station >= distance) {
        if (p->station == distance) return head;
        p = p->next;
    }
    tmp = (solution_t *) malloc(sizeof(solution_t));
    tmp->next = p->next;
    tmp->prev = p;
    tmp->station = distance;
    p->next = tmp;
    return head;
}

solution_t *remove_list(long distance, solution_t *head) {
    solution_t *p = NULL;
    if (head == NULL) return NULL;
    p = head;
    while (p->next != NULL) {
        if (p->station == distance) {
            if (p->prev != NULL) p->prev->next = p->next;
            if (p->next != NULL) p->next->prev = p->prev;
        }
        p = p->next;
    }
    if (p->station != distance) return head;
    if (p == head) {
        p->next->prev = NULL;
        return p->next;
    }
    if (p->prev != NULL) p->prev->next = p->next;
    if (p->next != NULL) p->next->prev = p->prev;
    return head;
}

void print_stations() {
    printf("Stations:\n");
    int i = 0, j = 0;
    solution_t *tmp = NULL;
    for (i = 0; i < SIZE; i++) {
        if (stations_table[i] != NULL) {
            printf("[%d]: (", i);
            for (j = 0; stations_table[i]->parked_cars[j] != 0; j++) printf(" %lu ", stations_table[i]->parked_cars[j]);
            printf(") reachable:");

            tmp = stations_table[i]->reachable;

            while (tmp != NULL) {
                if (tmp->next != NULL && tmp->station == tmp->next->station) {
                    printf("ERROR");
                    return;
                }
                printf(" %lu", tmp->station);
                tmp = tmp->next;
            }
            printf("\n");
        }
    }
    printf("\n");
}