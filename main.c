#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NULL ((void *)0)

typedef struct Solution {
    ulong station;
    struct Solution *next;
} solution_t;

typedef struct Station {
    ulong distance;
    ulong parked_cars[512];
    int visited;
    struct Solution *reachable;
} station_t;


void print_stations(station_t **stations_table_ptr);

int add_station(char *token, station_t **stations_table);

int remove_station(char *token, station_t **stations_table);

int add_car(char *token, station_t **stations_table);

int remove_car(char *token, station_t **stations_table);

//int path_planner(char *token, station_t **stations_table);

solution_t *add_list(ulong x, solution_t *l);

solution_t *remove_list(ulong val, solution_t *head);

int main() {
    char *input;
    input = malloc(4096);
    station_t *stations_table[4096] = {NULL};

    while (fgets(input, 4096, stdin)) {
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
    station_t *tmp;
    station_t *new_station;
    token = strtok(NULL, " ");
    distance = strtol(token, NULL, 10);

    if (stations_table[distance] != NULL) return 1;
    tmp = stations_table[distance];
    new_station = malloc(sizeof(station_t));
    new_station->distance = distance;
    new_station->visited = 0;
    new_station->reachable = NULL;
    token = strtok(NULL, " ");
    n_auto = atoi(token);
    token = strtok(NULL, " ");
    new_station->parked_cars[0] = strtol(token, NULL, 10);
    token = strtok(NULL, " ");
    while (token != NULL) {
        autonomy = strtol(token, NULL, 10);
        first = 1;
        for (i = 0; new_station->parked_cars[i] != 0 && i<n_auto; i++) {
            if (new_station->parked_cars[i] == autonomy){
                first = -1;
                break;
            }
            if (new_station->parked_cars[i] > autonomy) tmp_cars[i] = new_station->parked_cars[i];
            if (new_station->parked_cars[i] < autonomy){
                if(first == 1){
                    first = 0;
                    tmp_cars[i] = autonomy;
                }else tmp_cars[i] = new_station->parked_cars[i-1];
            }
        }
        if(first == 1) tmp_cars[i]=autonomy;
            else tmp_cars[i] = new_station->parked_cars[i-1];
        if (first != -1) {
            for (j = 0; tmp_cars[j] != 0; j++) {
                new_station->parked_cars[j] = tmp_cars[j];
            }
        }
        token = strtok(NULL, " ");
    }



    for (i = distance - 1; i >= 0; i--) {
        if (stations_table[i] != NULL) {
            tmp = stations_table[i];
            if (tmp->parked_cars[0] + tmp->distance >= distance) tmp->reachable = add_list(distance, tmp->reachable);

        }
    }

    for (i = distance; i < distance + new_station->parked_cars[0]; i++) {
        if (stations_table[i] != NULL) {
            new_station->reachable = add_list(i, new_station->reachable);
        }
    }

    stations_table[distance] = new_station;

    return 0;
}


int remove_station(char *token, station_t **stations_table) {
    ulong distance = 0;
    int i = 0;
    token = strtok(NULL, " ");
    distance = strtol(token, NULL, 10);
    if (stations_table[distance] == NULL) return 1;

    for (i = distance - 1; i >= 0; i--) {
        if (stations_table[i] != NULL) {
            stations_table[i]->reachable = remove_list(distance, stations_table[i]->reachable);
        }
    }

    stations_table[distance] = NULL;
    return 0;
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
    tmp = stations_table[distance];
    if (tmp == NULL) return 1;
    if (tmp->parked_cars[0] == 0) {
        tmp->parked_cars[0] = new_car;
        return 0;
    }
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
    if(first == 1) tmp_cars[i]=new_car;
    else tmp_cars[i] = tmp->parked_cars[i - 1];;

    for (i = 0; tmp_cars[i] != 0; i++) {
        tmp->parked_cars[i] = tmp_cars[i];
    }


    for (i = distance + 1; i < distance + tmp->parked_cars[0]; i++) {
        if (stations_table[i] != NULL) {
            tmp->reachable = add_list(i, tmp->reachable);
        }
    }


    return 0;
}


int remove_car(char *token, station_t **stations_table) {
    int i = 0, first = 1;
    ulong distance = 0, new_car = 0;
    ulong tmp_cars[512] = {0};
    station_t *tmp;
    solution_t *tmp2, *tmp3;
    token = strtok(NULL, " ");
    distance = strtol(token, NULL, 10);
    token = strtok(NULL, " ");
    new_car = strtol(token, NULL, 10);
    tmp = stations_table[distance];
    if (tmp == NULL) return 1;

    for (i = 0; tmp->parked_cars[i] != 0; i++) {
        if (tmp->parked_cars[i] > new_car) tmp_cars[i] = tmp->parked_cars[i];
        if (tmp->parked_cars[i] == new_car) first = 0;
        if (tmp->parked_cars[i] < new_car) {
            if (first == 0) {
                tmp_cars[i - 1] = tmp->parked_cars[i];
            } else return 1;
        }
    }

    for (i = 0; tmp->parked_cars[i] != 0; i++) {
        tmp->parked_cars[i] = tmp_cars[i];
    }
    tmp = stations_table[distance];

    tmp2 = tmp->reachable;
    tmp3 = tmp2->next;
    while (tmp2 != NULL) {

        if (tmp2->station > distance + tmp->parked_cars[0]) {
            tmp->reachable = remove_list(tmp2->station, tmp->reachable);
        }
        tmp2 = tmp3;
        if (tmp2 == NULL) break;
        tmp3 = tmp2->next;
    }

    return 0;
}


/*
int path_planner(char *token, station_t **stations_table) {

    ulong start, goal;
    int current_length = -1, max_lenght = -1;
    int end = 0;
    station_t *current_station;
    ulong current_range = 0, current_distance = 0;
    solution_t *solution_list, *current_solution;
    token = strtok(NULL, " ");
    start = strtol(token, NULL, 10);
    token = strtok(NULL, " ");
    goal = strtol(token, NULL, 10);
    if (goal == start) printf("%lu\n", goal);

    current_distance = start;
    current_station = stations_table[current_distance];
    current_range = current_station->parked_cars[0];


    end = 0;

}
*/

solution_t *add_list(ulong val, solution_t *head) {
    solution_t *current = head, *tmp;

    if (head == NULL) {
        head = (solution_t *) malloc(sizeof(solution_t));
        head->next = NULL;
        head->station = val;
        return head;
    }
    if (head->station == val) return head;
    if (head->station < val) {
        tmp = (solution_t *) malloc(sizeof(solution_t));
        tmp->next = head;
        tmp->station = val;
        return tmp;
    }

    while (current->next != NULL && current->next->station > val) {
        if (current->station == val) return head;
        current = current->next;
    }

    if (current->station == val) return head;
    tmp = (solution_t *) malloc(sizeof(solution_t));

    tmp->next = current->next;
    current->next = tmp;

    tmp->station = val;

    return head;
}

solution_t *remove_list(ulong val, solution_t *head) {
    solution_t *current = head, *tmp;
    if (head->station == val) {
        head = head->next;
        current->next = NULL;
        free(current);
        return head;
    }

    tmp = current;
    while (current->next != NULL && current->next->station > val) {
        tmp = current;
        current = current->next;
    }

    if (current->station == val) {
        tmp->next = current->next;
        current->next = NULL;
        free(current);
    }

    return head;

}


void print_stations(station_t **stations_table) {
    printf("Stations:\n");
    int i = 0, j = 0;
    solution_t *tmp;
    for (i = 0; i < 4096; i++) {
        if (stations_table[i] != NULL) {
            printf("[%d]: (", i);
            for (j = 0; stations_table[i]->parked_cars[j] != 0; j++) printf(" %lu ", stations_table[i]->parked_cars[j]);
            printf(") reachable:");

            tmp = stations_table[i]->reachable;
            while (tmp != NULL) {
                printf(" %lu", tmp->station);
                tmp = tmp->next;
            }
            printf("\n");
        }
    }
    printf("\n");
}

/*
 * aggiungi-stazione 10 2 100 200
 * aggiungi-stazione 200 2 100 200
 * demolisci-stazione 10
 * aggiungi-auto 10 300
 * rottama-auto 10 200
 *
 * */