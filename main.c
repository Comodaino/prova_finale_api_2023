#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 1024

typedef struct Station {
    ulong distance;
    ulong parked_cars[512];
    int visited;
    struct Station *next;
} station_t;

void print_stations(station_t **stations_table_ptr);
int hash_function(ulong);
int add_station(char *token, station_t **stations_table_ptr);

int main() {
    char *input;
    input = malloc(SIZE);
    station_t *stations_table[1000] = {NULL};

    while (fgets(input, SIZE, stdin)) {
        char *token = strtok(input, " ");
        if (strcmp(token, "aggiungi-stazione") == 0) {
            if (add_station(token, stations_table) == 0) printf("aggiunta\n");
            else printf("non aggiunta\n");
            print_stations(stations_table);
        } else if (strcmp(token, "demolisci-stazione") == 0) {
            // do something else
        } else if (strcmp(token, "aggiungi-auto") == 0) {
            // do something else
        } else if (strcmp(token, "rottama-auto") == 0) {
            // do something else
        } else if (strcmp(token, "pianifica-percorso") == 0) {
            // do something else
        }
    }
}


int add_station(char *token, station_t **stations_table) {
    ulong distance = 0;
    int n_auto = 0;
    char *trash_can;
    int i = 0;
    station_t *tmp, *tmp2;
    station_t *new_station = malloc(sizeof(station_t));
    token = strtok(NULL, " ");
    distance = strtol(token, NULL, 10);

    tmp = stations_table[hash_function(distance)];
    if (tmp == NULL) {
        printf("DEBUG");
        new_station->distance = distance;
        new_station->visited = 0;
        token = strtok(NULL, " ");
        n_auto = atoi(token);
        for (i = 0; token != NULL && i < n_auto; i++) {
            token = strtok(NULL, " ");
            new_station->parked_cars[i] = strtol(token, trash_can, 10);
        }
        stations_table[hash_function(distance)] = new_station;
        return 0;
    }
    while (tmp != NULL && tmp->distance >= distance) {
        if (tmp->distance == distance) return 1;
        tmp = tmp->next;
    }
    new_station->distance = distance;
    new_station->visited = 0;
    token = strtok(NULL, " ");
    n_auto = atoi(token);
    for (i = 0; token != NULL && i < n_auto; i++) {
        token = strtok(NULL, " ");
        new_station->parked_cars[i] = strtol(token, trash_can, 10);
    }

    while (tmp->next != NULL && tmp->distance >= distance) {
    }
    if (tmp->next == NULL) {
        tmp->next = new_station;
    } else {
        tmp2 = tmp->next;
        tmp->next = new_station;
        new_station->next = tmp2;
    }
    return 0;

}

int hash_function(ulong x) {
    int tmp = 0, i = 0;
    char int_str[20] = {NULL};
    sprintf(int_str, "%d", x);
    if (x == 0) return 0;
    for (i = 0; i < 3; i++) {
        if (int_str[i] != NULL) {
            tmp = int_str[i] - 48 + tmp * 10;
        }
    }
    return tmp;
}


void print_stations(station_t **stations_table){
    printf("Stations:");
    int i=0;
    station_t* tmp;
    for(i=0; i<1000; i++){
        if(stations_table[i] != NULL){
            tmp = stations_table[i];
            while(tmp!=NULL){
                printf(" %lu", tmp->distance);
                tmp = tmp->next;
            }
        }
    }
}

//aggiungi-stazione 10 2 100 200