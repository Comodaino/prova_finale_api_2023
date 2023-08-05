#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 50000

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

station_t *stations_table[SIZE];
char input[100];
ulong most_distant_station = 0;

void print_stations();

int add_station();

int remove_station();

int add_car();

int remove_car();

int path_planner();

void explore(station_t *, solution_t **, solution_t **, ulong, int *, int *, int);

solution_t *add_list(ulong, solution_t *);

solution_t *remove_list(ulong, solution_t *);

solution_t *copy_list(solution_t *);

void print_reverse(solution_t *);

void print_direct(solution_t *);

void resetter();

int main() {
    int i;


    for (i = 0; i < SIZE; i++) stations_table[i] = NULL;
    while (fscanf(stdin, "%s", input) != EOF) {

        if (strcmp(input, "aggiungi-stazione") == 0) {
            if (add_station() == 0) printf("aggiunta\n");
            else printf("non aggiunta\n");

        } else if (strcmp(input, "demolisci-stazione") == 0) {
            if (remove_station() == 0) printf("demolita\n");
            else printf("non demolita\n");

        } else if (strcmp(input, "aggiungi-auto") == 0) {
            if (add_car() == 0) printf("aggiunta\n");
            else printf("non aggiunta\n");

        } else if (strcmp(input, "rottama-auto") == 0) {
            if (remove_car() == 0) printf("rottamata\n");
            else printf("non rottamata\n");

        } else if (strcmp(input, "pianifica-percorso") == 0) {
            //print_stations(stations_table);
            resetter(most_distant_station);
            if (path_planner() == 1) printf("nessun percorso\n");
        }

    }
    return 0;
}


int add_station() {
    long n_auto = 0, distance = 0, autonomy = 0;
    station_t *new_station = NULL, *tmp = NULL;
    int i = 0, j = 0, pos = 0, flag = 0;
    long li = 0;


    if (fscanf(stdin, "%s", input) == EOF) return 1;
    distance = atol(input);
    if (fscanf(stdin, "%s", input) == EOF) return 1;
    n_auto = atol(input);

    if (stations_table[distance] != NULL) return 1;

    new_station = (station_t *) malloc(sizeof(station_t));
    new_station->reachable = NULL;
    new_station->distance = distance;
    for (i = 0; i < 512; i++) new_station->parked_cars[i] = 0;
    new_station->visited = 0;
    stations_table[distance] = new_station;

    if (most_distant_station == -1 || most_distant_station < distance) most_distant_station = distance;

    //check for other stations
    for (i = 0; i <= most_distant_station; i++) {
        if (stations_table[i] != NULL && i != distance) {
            tmp = stations_table[i];
            if (i < distance && tmp->parked_cars[0] + tmp->distance > distance) {
                tmp->reachable = add_list(distance, tmp->reachable);
            } else if (tmp->distance - tmp->parked_cars[0] < distance) {
                tmp->reachable = add_list(distance, tmp->reachable);
            }
        }
    }

    if (n_auto == 0) return 0;
    for (i = 0; i < n_auto; i++) {
        flag = 0;
        if (fscanf(stdin, "%s", input) == EOF) return 1;
        autonomy = atol(input);
        for (j = 0; j < 512; j++) {
            if (new_station->parked_cars[j] == autonomy) flag = 1;
            if (new_station->parked_cars[j] < autonomy) break;
        }
        if (flag == 0) {
            pos = j;
            for (j = 511; j >= pos; j--) new_station->parked_cars[j + 1] = new_station->parked_cars[j];
            new_station->parked_cars[pos] = autonomy;
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
    ulong distance;
    station_t *tmp = NULL;
    solution_t *x = NULL, *y = NULL;
    int i;
    if (fscanf(stdin, "%s", input) == EOF) return 1;
    distance = atol(input);
    if (stations_table[distance] == NULL) return 1;

    for (i = 0; i < most_distant_station; i++) {

        if (stations_table[i] != NULL) {
            tmp = stations_table[i];

            stations_table[i]->reachable = remove_list(distance, stations_table[i]->reachable);

        }
    }
    x = stations_table[distance]->reachable;
    while (x != NULL) {
        y = x;
        x = x->next;
        free(y);
        y = NULL;
    }

    tmp = stations_table[distance];

    free(tmp);
    tmp = NULL;

    //free(stations_table[distance]);
    stations_table[distance] = NULL;
    return 0;
}


int add_car() {
    int pos = 0, j = 0;
    ulong distance, autonomy, i = 0;
    station_t *tmp = NULL;
    if (fscanf(stdin, "%s", input) == EOF) return 1;
    distance = atol(input);
    if (fscanf(stdin, "%s", input) == EOF) return 1;
    autonomy = atol(input);
    if (autonomy == 0) return 0;
    tmp = stations_table[distance];
    if (tmp == NULL) return 1;
    if (tmp->parked_cars[0] == 0) {
        tmp->parked_cars[0] = autonomy;
        return 0;
    }

    for (j = 0; tmp->parked_cars[j] != 0; j++) {
        if (tmp->parked_cars[j] < autonomy) break;
    }
    pos = j;
    for (j = 511; j >= pos; j--) {
        tmp->parked_cars[j + 1] = tmp->parked_cars[j];
    }
    tmp->parked_cars[pos] = autonomy;

    i = 0;
    if (distance > tmp->parked_cars[0]) i = distance - tmp->parked_cars[0];


    while (i <= distance + tmp->parked_cars[0]) {
        if (stations_table[i] != NULL && i != distance) {
            tmp->reachable = add_list(i, tmp->reachable);
        }
        i++;
    }

    return 0;
}

int remove_car() {
    int i, first = 1;
    ulong distance, autonomy;
    ulong tmp_cars[512] = {0};
    station_t *tmp = NULL;
    solution_t *tmp2 = NULL;


    if (fscanf(stdin, "%s", input) == EOF) return 1;
    distance = atol(input);
    if (fscanf(stdin, "%s", input) == EOF) return 1;
    autonomy = atol(input);
    tmp = stations_table[distance];
    if (tmp == NULL) return 1;

    for (i = 0; tmp->parked_cars[i] != 0; i++) {
        if (tmp->parked_cars[i] > autonomy) tmp_cars[i] = tmp->parked_cars[i];
        if (tmp->parked_cars[i] == autonomy) first = 0;
        if (tmp->parked_cars[i] < autonomy) {
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


int path_planner() {
    int max_length = -1, current_length = 0, direction = 1;
    ulong start, goal, distance;
    station_t *current_node = NULL;
    solution_t *current_solution = NULL, *current_path = NULL, *reachable = NULL;

    if (fscanf(stdin, "%s", input) == EOF) return 1;
    start = atol(input);
    if (fscanf(stdin, "%s", input) == EOF) return 1;
    goal = atol(input);

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
        if (stations_table[distance] != NULL) {
            stations_table[distance]->visited = 1;
            if ((reachable->station > start && direction == 1) || (reachable->station < start && direction == -1)) {

                current_path = add_list(distance, current_path);
                explore(stations_table[reachable->station], &current_path, &current_solution, goal,
                        &max_length,
                        &current_length, direction);
                current_path = remove_list(distance, current_path);
            }
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

void explore(station_t *node, solution_t **path, solution_t **solution, ulong goal,
             int *max_length_ptr, int *current_length_ptr, int direction) {

    solution_t *tmp_s = NULL, *tmp_p = NULL, *reachable = NULL;
    solution_t *x = NULL, *y = NULL;
    station_t *tmp = NULL;
    ulong distance;
    int choice = 0; // 1 = solution, -1 = path
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
                y = NULL;
            }
            *solution = copy_list(*path);

            *max_length_ptr = *current_length_ptr;

            return;
        }
        if (*current_length_ptr == *max_length_ptr) {
            tmp_p = *path;
            tmp_s = *solution;

            if (direction == -1) {

                while (tmp_s != NULL && tmp_p != NULL) {

                    if (stations_table[tmp_s->station]->distance == stations_table[tmp_p->station]->distance) {}
                    else if (stations_table[tmp_s->station]->distance < stations_table[tmp_p->station]->distance)
                        choice = 1;
                    else {
                        choice = -1;
                        //free(*solution);
                    }
                    tmp_s = tmp_s->next;
                    tmp_p = tmp_p->next;
                }
                if (choice == -1) {
                    x = *solution;
                    while (x != NULL) {
                        y = x;
                        x = x->next;
                        free(y);
                        y = NULL;
                    }
                    *solution = copy_list(*path);
                    return;
                }
            }

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
                        y = NULL;
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
        if (stations_table[distance] != NULL) {
            tmp = stations_table[distance];

            if (distance != node->distance && (tmp->visited == 0 || *current_length_ptr <= tmp->visited)) {

                stations_table[distance]->visited = *current_length_ptr;
                if ((distance > node->distance && direction == 1) ||
                    (distance < node->distance && direction == -1)) {

                    distance = reachable->station;
                    *current_length_ptr = *current_length_ptr + 1;
                    *path = add_list(distance, *path);
                    explore(stations_table[reachable->station], path, solution, goal, max_length_ptr,
                            current_length_ptr, direction);

                    *current_length_ptr = *current_length_ptr - 1;
                    *path = remove_list(distance, *path);

                }
            }
        }
        reachable = reachable->next;
    }

}


solution_t *add_list(ulong val, solution_t *head) {
    solution_t *current = NULL, *tmp = NULL;

    if (head == NULL) {
        head = (solution_t *) malloc(sizeof(solution_t));
        //if (head == NULL) return NULL;
        head->next = NULL;
        head->station = val;

        return head;
    }
    if (head->station == val) return head;
    if (head->station < val) {
        tmp = (solution_t *) malloc(sizeof(solution_t));
        if (tmp == NULL) return NULL;
        tmp->next = head;
        tmp->station = val;
        return tmp;
    }

    current = head;
    while (current->next != NULL && current->next->station > val) {
        if (current->station == val) return head;
        current = current->next;
    }

    if (current->next == NULL) {
        tmp = (solution_t *) malloc(sizeof(solution_t));
        if (tmp == NULL) return NULL;
        tmp->next = NULL;
        current->next = tmp;
        tmp->station = val;
    }
    if (current->next->station == val) return head;
    tmp = (solution_t *) malloc(sizeof(solution_t));
    if (tmp == NULL) return NULL;
    tmp->next = NULL;
    tmp->next = current->next;
    current->next = tmp;
    tmp->station = val;


    return head;
}

solution_t *remove_list(ulong val, solution_t *head) {
    solution_t *current = head, *prv = NULL;
    if (head == NULL) return NULL;
    if (head->station == val) {
        prv = head->next;
        //free(head);
        head = NULL;
        return prv;
    }
    prv = current;
    current = current->next;
    while (current != NULL) {
        if (current->station == val) {
            prv->next = current->next;
            free(current);
            current = NULL;
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
    if (head == NULL) return NULL;
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
        if (p->next == NULL) return NULL;
        p->next->station = 0;
        p->next->next = NULL;
        p = p->next;
        path = path->next;
    }
    return head;
}

void print_stations() {
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

void resetter() {
    ulong i = 0;
    for (i = 0; i <= most_distant_station; i++) {
        if (stations_table[i] != NULL) stations_table[i]->visited = 0;
    }

}

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
