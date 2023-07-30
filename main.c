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

int add_station(char *token, station_t **stations_table);

int remove_station(char *token, station_t **stations_table);

int add_car(char *token, station_t **stations_table);

int remove_car(char *token, station_t **stations_table);

int main() {
    char *input;
    input = malloc(SIZE);
    station_t *stations_table[1000] = {0};

    while (fgets(input, SIZE, stdin)) {
        char *token = strtok(input, " ");
        if (strcmp(token, "aggiungi-stazione") == 0) {
            if (add_station(token, stations_table) == 0) printf("aggiunta\n");
            else printf("non aggiunta\n");

        } else if (strcmp(token, "demolisci-stazione") == 0) {
            if (remove_station(token, stations_table) == 0) printf("demolita\n");
            else printf("non demolita\n");

        } else if (strcmp(token, "aggiungi-auto") == 0) {
            if (add_car(token, stations_table) == 0) printf("aggiunta\n");
            else printf("non aggiunta\n");

        } else if (strcmp(token, "rottama-auto") == 0) {
            if (remove_car(token, stations_table) == 0) printf("rottamata\n");
            else printf("non rottamata\n");

        } else if (strcmp(token, "pianifica-percorso") == 0) {
            // do something else
        }
        print_stations(stations_table);
    }
}


int add_station(char *token, station_t **stations_table) {
    ulong distance = 0, autonomy = 0;
    int n_auto = 0;
    int i = 0, j = 0, first = 0;
    ulong tmp_cars[512] = {0};
    station_t *tmp, *tmp2;
    station_t *new_station = malloc(sizeof(station_t));
    token = strtok(NULL, " ");
    distance = strtol(token, NULL, 10);

    tmp = stations_table[hash_function(distance)];

    new_station->distance = distance;
    new_station->visited = 0;
    token = strtok(NULL, " ");
    n_auto = atoi(token);
    for (i = 0; token != NULL && i < n_auto; i++) {
        token = strtok(NULL, " ");
        autonomy = strtol(token, NULL, 10);

        if (new_station->parked_cars[0] == 0) new_station->parked_cars[0] = autonomy;
        else {
            first = 1;
            for (j = 0; new_station->parked_cars[j] != 0; j++) {
                if (new_station->parked_cars[j] == autonomy) {
                    first = -1;
                    break;
                }
                if (new_station->parked_cars[j] > autonomy) tmp_cars[j] = new_station->parked_cars[j];
                if (new_station->parked_cars[j] < autonomy) {
                    if (first == 1) {
                        tmp_cars[j] = autonomy;
                        first = 0;
                    } else tmp_cars[j] = new_station->parked_cars[j - 1];
                }
            }
            tmp_cars[j] = new_station->parked_cars[j - 1];
            if (first != -1) {
                for (j = 0; tmp_cars[j] != 0; j++) {
                    new_station->parked_cars[j] = tmp_cars[j];
                }
            }
        }
    }

    if (tmp == 0) {
        stations_table[hash_function(distance)] = new_station;
        return 0;
    }
    if (tmp->distance < distance) {
        new_station->next = tmp;
        stations_table[hash_function(distance)] = new_station;
        return 0;
    }
    while (tmp != 0 && tmp->distance >= distance) {
        if (tmp->distance == distance) return 1;
        tmp = tmp->next;
    }
    if (tmp->next == 0) {
        tmp->next = new_station;
    } else {
        tmp2 = tmp->next;
        tmp->next = new_station;
        new_station->next = tmp2;
    }
    return 0;

}


int remove_station(char *token, station_t **stations_table) {
    ulong distance = 0;
    station_t *tmp, *prv;
    token = strtok(NULL, " ");
    distance = strtol(token, NULL, 10);
    tmp = stations_table[hash_function(distance)];
    if (tmp == 0) {
        return 1;
    }
    if (tmp->distance == distance) {
        stations_table[hash_function(distance)] = tmp->next;
        free(tmp);
        return 0;
    }
    prv = tmp;
    tmp = tmp->next;
    while (tmp != 0 && tmp->distance >= distance) {
        if (tmp->distance == distance) {
            prv->next = tmp->next;
            free(tmp);
            return 0;
        }
        prv = tmp;
        tmp = tmp->next;
    }
    return 1;
}

int add_car(char *token, station_t **stations_table) {
    int i = 0, first = 1;
    ulong distance = 0, new_car = 0;
    ulong tmp_cars[512] = {0};
    station_t *tmp;
    token = strtok(NULL, " ");
    distance = strtol(token, NULL, 10);
    token = strtok(NULL, " ");
    new_car = strtol(token, NULL, 10);
    tmp = stations_table[hash_function(distance)];
    if (tmp == 0) return 1;
    while (tmp != 0 && tmp->distance >= strtol(token, NULL, 10)) {
        if (tmp->distance == distance) break;
        tmp = tmp->next;
    }
    if (tmp->distance != distance) return 1;


    for (i = 0; tmp->parked_cars[i] != 0; i++) {
        if (tmp->parked_cars[i] == new_car) return 1;
        if (tmp->parked_cars[i] > new_car) tmp_cars[i] = tmp->parked_cars[i];
        if (tmp->parked_cars[i] < new_car) {
            if (first == 1) {
                tmp_cars[i] = new_car;
                first = 0;
            } else tmp_cars[i] = tmp->parked_cars[i - 1];
        }
    }
    tmp_cars[i] = tmp->parked_cars[i - 1];
    for (i = 0; tmp_cars[i] != 0; i++) {
        tmp->parked_cars[i] = tmp_cars[i];
    }
    return 0;
}

int remove_car(char *token, station_t **stations_table) {
    int i = 0, first = 1;
    ulong distance = 0, new_car = 0;
    ulong tmp_cars[512] = {0};
    station_t *tmp;
    token = strtok(NULL, " ");
    distance = strtol(token, NULL, 10);
    token = strtok(NULL, " ");
    new_car = strtol(token, NULL, 10);
    tmp = stations_table[hash_function(distance)];
    if (tmp == 0) return 1;
    while (tmp != 0 && tmp->distance >= strtol(token, NULL, 10)) {
        if (tmp->distance == distance) break;
        tmp = tmp->next;
    }
    if (tmp->distance != distance) return 1;

    for (i = 0; tmp->parked_cars[i] != 0; i++) {
        if (tmp->parked_cars[i] > new_car) tmp_cars[i] = tmp->parked_cars[i];
        if(tmp->parked_cars[i] == new_car) first = 0;
        if (tmp->parked_cars[i] < new_car) {
            if (first == 0) {
                tmp_cars[i-1] = tmp->parked_cars[i];
            } else return 1;
        }
    }

    for (i = 0; tmp->parked_cars[i]!= 0; i++) {
        tmp->parked_cars[i] = tmp_cars[i];
    }
    //tmp_cars[i] = tmp->parked_cars[i];
    return 0;
}

int hash_function(ulong x) {
    int tmp = 0, i = 0;
    char int_str[20] = {0};
    sprintf(int_str, "%ld", x);
    if (x == 0) return 0;
    for (i = 0; i < 3; i++) {
        if (int_str[i] != 0) {
            tmp = int_str[i] - 48 + tmp * 10;
        }
    }
    return tmp;
}


void print_stations(station_t **stations_table) {
    printf("Stations:\n");
    int i = 0, j = 0;
    station_t *tmp;
    for (i = 0; i < 1000; i++) {
        if (stations_table[i] != 0) {
            printf("[%d]: ", i);
            tmp = stations_table[i];
            while (tmp != 0) {
                printf(" %lu(", tmp->distance);
                for (j = 0; tmp->parked_cars[j] != 0; j++) printf(" %lu ", tmp->parked_cars[j]);
                printf(")");
                tmp = tmp->next;
            }
            printf("\n");
        }
    }
    printf("\n");
}

/*
 * aggiungi-stazione 10 2 100 200
 * demolisci-stazione 10
 * aggiungi-auto 10 300
 * rottama-auto 10 300
 *
 * */