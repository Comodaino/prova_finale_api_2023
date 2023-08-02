#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NULL ((void *)0)
#define SIZE 40960

typedef struct Solution {
    ulong station;
    struct Solution *next;
} solution_t;

typedef struct Station {
    ulong distance;
    ulong parked_cars[512];
    struct Solution *reachable;
} station_t;

//void print_stations(station_t **stations_table_ptr);

int add_station(char *, station_t **, ulong *);

int remove_station(char *, station_t **, const ulong *);

int add_car(char *, station_t **);

int remove_car(char *, station_t **);

int path_planner(char *, station_t **);

void explore(station_t **, station_t *, solution_t **, solution_t **, ulong, int *, int *, int);

solution_t *add_list(ulong, solution_t *);

solution_t *remove_list(ulong, solution_t *);

solution_t *copy_list(solution_t *);

void print_reverse(solution_t *);

void print_direct(solution_t *);

int main() {
    char input[1000] = {'\000'}, *token = NULL;
    int result = 1;
    ulong most_distant_station = 0;
    station_t *stations_table[SIZE];
    for(result = 0; result < SIZE; result++) stations_table[result]= NULL;
    result = 1;
    while (fgets(input, sizeof(input), stdin)) {
        token = strtok(input, " ");
        if (strcmp(token, "aggiungi-stazione") == 0) {
            result = add_station(token, stations_table, &most_distant_station);
            if (result == 0) printf("aggiunta\n");
            else printf("non aggiunta\n");

        } else if (strcmp(token, "demolisci-stazione") == 0) {
            result = remove_station(token, stations_table, &most_distant_station);
            if (result == 0) printf("demolita\n");
            else printf("non demolita\n");

        } else if (strcmp(token, "aggiungi-auto") == 0) {
            result = add_car(token, stations_table);
            if (result == 0) printf("aggiunta\n");
            else printf("non aggiunta\n");

        } else if (strcmp(token, "rottama-auto") == 0) {
            result = remove_car(token, stations_table);
            if (result == 0) printf("rottamata\n");
            else printf("non rottamata\n");

        } else if (strcmp(token, "pianifica-percorso") == 0) {
            //print_stations(stations_table);
            result = path_planner(token, stations_table);

            if (result == 1) printf("nessun percorso\n");

        }

    }
    return 1;
}


int add_station(char *token, station_t **stations_table, ulong *mds) {
    ulong distance = 0, autonomy = 0, i=0;
    ulong n_auto = 0;
    int j = 0, first = 0;
    ulong tmp_cars[512] = {0};
    station_t *tmp = NULL;
    station_t *new_station = NULL;
    token = strtok(NULL, " ");
    distance = strtol(token, NULL, 10);

    if (stations_table[distance] != NULL) return 1;

    if (*mds < distance) *mds = distance;

    new_station = (station_t *) malloc(sizeof(station_t));
    new_station->distance = distance;
    new_station->reachable = NULL;

    for (i = 0; i < 512; i++) new_station->parked_cars[i] = 0;

    token = strtok(NULL, " ");
    n_auto = strtol(token, NULL, 10);
    if (n_auto > 0) {
        token = strtok(NULL, " ");
        new_station->parked_cars[0] = strtol(token, NULL, 10);
        token = strtok(NULL, " ");
        while (token != NULL) {
            autonomy = strtol(token, NULL, 10);
            first = 1;
            for (i = 0; new_station->parked_cars[i] != 0 && i < n_auto; i++) {
                if (new_station->parked_cars[i] == autonomy) {
                    first = -1;
                    break;
                }
                if (new_station->parked_cars[i] > autonomy) tmp_cars[i] = new_station->parked_cars[i];
                if (new_station->parked_cars[i] < autonomy) {
                    if (first == 1) {
                        first = 0;
                        tmp_cars[i] = autonomy;
                    } else tmp_cars[i] = new_station->parked_cars[i - 1];
                }
            }
            if (first == 1) tmp_cars[i] = autonomy;
            else tmp_cars[i] = new_station->parked_cars[i - 1];
            if (first != -1) {
                for (j = 0; tmp_cars[j] != 0; j++) {
                    new_station->parked_cars[j] = tmp_cars[j];
                }
            }
            token = strtok(NULL, " ");

        }
    }


    for (i = 0; i < *mds; i++) {
        if (stations_table[i] != NULL) {
            tmp = stations_table[i];

            if (i > distance) {
                if (i - tmp->parked_cars[0] <= distance) tmp->reachable = add_list(distance, tmp->reachable);
            } else if (i < distance) {
                if (tmp->parked_cars[0] + i >= distance) tmp->reachable = add_list(distance, tmp->reachable);
            }


        }
    }

    if (n_auto == 0) {
        stations_table[distance] = new_station;
        return 0;
    }

    i = 0;
    if (distance - new_station->parked_cars[0] > 0) i = distance - new_station->parked_cars[0];

    while (i <= distance + new_station->parked_cars[0]) {

        if (stations_table[i] != NULL && i != distance) {
            new_station->reachable = add_list(i, new_station->reachable);
        }
        i++;
    }

    stations_table[distance] = new_station;

    return 0;
}


int remove_station(char *token, station_t **stations_table, const ulong *mds) {
    ulong distance = 0;
    station_t *tmp = NULL;
    solution_t *x = NULL, *y = NULL;
    int i = 0;
    token = strtok(NULL, " ");
    distance = strtol(token, NULL, 10);
    if (stations_table[distance] == NULL) return 1;

    for (i = 0; i < *mds; i++) {
        if (stations_table[i] != NULL) {
            stations_table[i]->reachable = remove_list(distance, stations_table[i]->reachable);
        }
    }
    x = stations_table[distance]->reachable;
    while (x != NULL) {
        y = x;
        x = x->next;
        free(y);
    }

    tmp = stations_table[distance];

    free(tmp);

    //free(stations_table[distance]);
    stations_table[distance] = NULL;
    return 0;
}


int add_car(char *token, station_t **stations_table) {
    int first = 1;
    ulong i=0;
    ulong distance = 0, new_car = 0;
    ulong tmp_cars[512] = {0};
    station_t *tmp = NULL;


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
        if (tmp->parked_cars[i] == new_car) return 0;
        if (tmp->parked_cars[i] > new_car) tmp_cars[i] = tmp->parked_cars[i];
        if (tmp->parked_cars[i] < new_car) {
            if (first == 1) {
                tmp_cars[i] = new_car;
                first = 0;
            } else tmp_cars[i] = tmp->parked_cars[i - 1];
        }
    }
    if (first == 1) tmp_cars[i] = new_car;
    else tmp_cars[i] = tmp->parked_cars[i - 1];

    for (i = 0; tmp_cars[i] != 0; i++) {
        tmp->parked_cars[i] = tmp_cars[i];
    }


    i = 0;
    if (distance > tmp->parked_cars[0]) i = distance - tmp->parked_cars[0];

    while (i < distance + tmp->parked_cars[0]) {
        if (stations_table[i] != NULL && i != distance) {
            tmp->reachable = add_list(i, tmp->reachable);
        }
        i++;
    }

    return 0;
}


int remove_car(char *token, station_t **stations_table) {
    int i = 0, first = 1;
    ulong distance = 0, new_car = 0;
    ulong tmp_cars[512] = {0};
    station_t *tmp = NULL;
    solution_t *tmp2 = NULL;


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
    //TODO MIGLIORABILE
    first = 1;
    while (first != 0) {
        first = 0;
        tmp2 = tmp->reachable;
        if (tmp2 == NULL) return 0;
        while (tmp2 != NULL && first == 0) {
            if (tmp2->station > tmp->parked_cars[0] + distance) {
                tmp->reachable = remove_list(tmp2->station, tmp->reachable);
                first = 1;
            }
            tmp2 = tmp2->next;
        }

    }

    return 0;
}


int path_planner(char *token, station_t **stations_table) {
    int max_length = -1, current_length = 0, direction = 1;
    ulong start = 0, goal = 0, distance = 0;
    station_t *current_node = NULL;
    solution_t *current_solution = NULL, *current_path = NULL, *reachable = NULL;

    token = strtok(NULL, " ");
    start = strtol(token, NULL, 10);
    token = strtok(NULL, " ");
    goal = strtol(token, NULL, 10);

    if (start == goal) {
        printf("%lu\n", start);
        return 0;
    }

    current_node = stations_table[start];

    current_path = add_list(start, current_path);

    current_solution = NULL;
    if (goal < start) direction = -1;
    max_length = -1;
    current_length = 1;
    reachable = current_node->reachable;
    if (reachable == NULL) return 1;
    while (reachable != NULL) {
        distance = reachable->station;
        if ((reachable->station > start && direction == 1) || (reachable->station < start && direction == -1)) {

            current_path = add_list(distance, current_path);
            explore(stations_table, stations_table[reachable->station], &current_path, &current_solution, goal,
                    &max_length,
                    &current_length, direction);
            current_path = remove_list(distance, current_path);
        }

        reachable = reachable->next;
        current_length = 1;

    }
    if (current_solution == NULL) return 1;

    if (direction == -1) print_direct(current_solution);
    else print_reverse(current_solution);

    printf("\n");
    return 0;
}

void explore(station_t **stations_table, station_t *node, solution_t **path, solution_t **solution, ulong goal,
             int *max_length_ptr, int *current_length_ptr, int direction) {

    solution_t *tmp_s = NULL, *tmp_p= NULL, *reachable= NULL;
    solution_t *x = NULL, *y = NULL;
    ulong distance = 0;
    if (*max_length_ptr != -1 && *current_length_ptr == *max_length_ptr && node->distance != goal) return;


    if (node == NULL) return;


    //*path = add_list(node->distance, *path);

    if (node->distance == goal) {

        if (*max_length_ptr == -1 || *current_length_ptr < *max_length_ptr) {
            //free(*solution);

            x = *solution;
            while (x != NULL) {
                y = x;
                x = x->next;
                free(y);
            }
            *solution = copy_list(*path);

            *max_length_ptr = *current_length_ptr;

            return;
        }
        if (*current_length_ptr == *max_length_ptr) {
            tmp_p = *path;
            tmp_s = *solution;
            while (tmp_s != NULL && tmp_p != NULL) {
                if (stations_table[tmp_s->station]->distance == stations_table[tmp_p->station]->distance) {}
                else if (stations_table[tmp_s->station]->distance < stations_table[tmp_p->station]->distance) return;
                else {
                    //free(*solution);
                    x = *solution;
                    while (x != NULL) {
                        y = x;
                        x = x->next;
                        free(y);
                    }

                    *solution = copy_list(*path);

                    return;
                }
                tmp_s = tmp_s->next;
                tmp_p = tmp_p->next;
            }

        }
        return;
    }

    if (node->reachable == NULL) return;

    reachable = node->reachable;
    while (reachable != NULL) {
        distance = reachable->station;
        if (distance != node->distance) {

            if ((distance > node->distance && direction == 1) ||
                (distance < node->distance && direction == -1)) {
                distance = reachable->station;
                *current_length_ptr = *current_length_ptr + 1;
                *path = add_list(distance, *path);
                explore(stations_table, stations_table[reachable->station], path, solution, goal, max_length_ptr,
                        current_length_ptr, direction);

                *current_length_ptr = *current_length_ptr - 1;
                *path = remove_list(distance, *path);

            }
        }

        reachable = reachable->next;
    }

}


solution_t *add_list(ulong val, solution_t *head) {
    solution_t *current = NULL, *tmp = NULL;

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

    current = head;
    while (current->next != NULL && current->next->station > val) {
        if (current->station == val) return head;
        current = current->next;
    }

    if (current->station == val) return head;
    tmp = (solution_t *) malloc(sizeof(solution_t));
    tmp->next = NULL;
    if (current->next != NULL) tmp->next = current->next;
    current->next = tmp;

    tmp->station = val;

    return head;
}

solution_t *remove_list(ulong val, solution_t *head) {
    solution_t *current = head, *prv = NULL;
    if (head == NULL) return NULL;
    if (head->station == val) {
        prv = head->next;
        free(head);
        return prv;
    }
    prv = current;
    current = current->next;
    while (current != NULL) {
        if (current->station == val) {
            prv->next = current->next;
            free(current);
            return head;
        }
        prv = current;
        current = current->next;
    }


    return head;
}


solution_t *copy_list(solution_t *path) {
    solution_t *p = NULL, *head = NULL;

    if (path == NULL) return NULL;
    head = (solution_t *) malloc(sizeof(solution_t));
    head->next = NULL;
    head->station = 0;
    p = head;

    while (path != NULL) {
        p->station = path->station;
        p->next = NULL;
        if (path->next == NULL) {
            p->next = NULL;
            return head;
        }
        p->next = (solution_t *) malloc(sizeof(solution_t));
        p->next->station = 0;
        p->next->next = NULL;
        p = p->next;
        path = path->next;
    }
    return head;
}

/*
void print_stations(station_t **stations_table) {
    printf("Stations:\n");
    int i = 0, j = 0;
    solution_t *tmp = NULL;
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
*/
void print_reverse(solution_t *head) {
    if (head == NULL)
        return;
    print_reverse(head->next);

    if (head->next != NULL) printf(" ");
    printf("%lu", head->station);
}

void print_direct(solution_t *head) {

    printf("%lu", head->station);
    head = head->next;
    while (head != NULL) {
        printf(" %lu", head->station);
        head = head->next;
    }
}
