#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 262144

typedef struct Solution {
    long long station;
    struct Solution *next;
} solution_t;

typedef struct Station {
    long long distance;
    long long parked_cars[512];
    int visited;
    struct Station *next;
    struct Solution *reachable;
} station_t;

station_t *stations_table[SIZE];
char input[30];
long long most_distant_station = 0;

void print_stations();

int add_station();

int remove_station();

int add_car();

int remove_car();

int path_planner();

void explore(station_t *, solution_t **, solution_t **, long long, int *, int *, int);

solution_t *add_list(long long, solution_t *);

solution_t *add_list_head(long long, solution_t *);

solution_t *remove_list(long long, solution_t *);

solution_t *copy_list(solution_t *);

void print_reverse(solution_t *);

void print_direct(solution_t *);

void resetter();

int get_input();

int add_to_table(station_t *);

int remove_table(long long);

station_t *get_table(long long);

int main() {
    // long i = 0;
    //for (i = 0; i < SIZE; i++) stations_table[i] = NULL;
    while (get_input() == 0) {

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
    long long n_auto = 0, distance = 0, autonomy = 0, i = 0;
    station_t *new_station = NULL, *tmp = NULL;
    int j = 0, pos = 0, flag = 0;
    long long li = 0;


    if (fscanf(stdin, "%lli", &distance) == EOF) return 1;
    if (fscanf(stdin, "%lli", &n_auto) == EOF) return 1;

    if (get_table(distance) != NULL) {

        for (i = 0; i < n_auto; i++) {

            if (fscanf(stdin, "%lli", &autonomy) == EOF) return 1;
        }
        return 1;
    }

    new_station = (station_t *) malloc(sizeof(station_t));
    new_station->reachable = NULL;
    new_station->next = NULL;
    new_station->distance = distance;
    for (i = 0; i < 512; i++) new_station->parked_cars[i] = 0;
    new_station->visited = 0;
    if (add_to_table(new_station) == 1) return 1;

    if (most_distant_station == -1 || most_distant_station < distance) most_distant_station = distance;

    //check for other stations
    for (i = 0; i <= most_distant_station; i++) {
        tmp = get_table(i);
        if (tmp != NULL && i != distance) {
            if (i < distance && tmp->parked_cars[0] + tmp->distance > distance) {
                tmp->reachable = add_list(distance, tmp->reachable);
            }
            if (i > distance && tmp->distance - tmp->parked_cars[0] < distance) {
                tmp->reachable = add_list(distance, tmp->reachable);
            }
        }
    }

    if (n_auto == 0) return 0;
    for (i = 0; i < n_auto; i++) {

        flag = 0;
        if (fscanf(stdin, "%lli", &autonomy) == EOF) return 1;
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
        if (get_table(li) != NULL && li != distance) new_station->reachable = add_list(li, new_station->reachable);
        li++;
    }

    return 0;
}


int remove_station() {
    long long distance;
    station_t *tmp = NULL;
    solution_t *x = NULL, *y = NULL;
    long long i;
    if (fscanf(stdin, "%lli", &distance) == EOF) return 1;
    if (remove_table(distance) == 1) return 1;

    for (i = 0; i < most_distant_station; i++) {
        tmp = get_table(i);
        if (tmp != NULL) {
            tmp->reachable = remove_list(distance, tmp->reachable);

        }
    }
    x = get_table(i)->reachable;
    while (x != NULL) {
        y = x;
        x = x->next;
        free(y);
        y = NULL;
    }

    return 0;
}


int add_car() {
    int pos = 0, j = 0;
    long long distance, autonomy, i = 0;
    station_t *tmp = NULL;
    if (fscanf(stdin, "%lli", &distance) == EOF) return 1;
    if (fscanf(stdin, "%lli", &autonomy) == EOF) return 1;
    if (autonomy == 0) return 0;
    tmp = get_table(distance);
    if (tmp == NULL) return 1;
    if (tmp->parked_cars[0] == autonomy) return 1;
    if (tmp->parked_cars[0] == 0) {
        tmp->parked_cars[0] = autonomy;
        return 0;
    }

    for (j = 0; tmp->parked_cars[j] != 0; j++) {
        if (tmp->parked_cars[j] == autonomy) return 1;
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
        if (get_table(i) != NULL && i != distance) tmp->reachable = add_list(i, tmp->reachable);
        i++;
    }

    return 0;
}

int remove_car() {
    int i, first = 1;
    long long distance, autonomy;
    long long tmp_cars[512] = {0};
    station_t *tmp = NULL;
    solution_t *tmp2 = NULL;


    if (fscanf(stdin, "%lli", &distance) == EOF) return 1;
    if (fscanf(stdin, "%lli", &autonomy) == EOF) return 1;
    tmp = get_table(distance);
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
    tmp = get_table(distance);
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
    long long start, goal, distance;
    station_t *current_node = NULL, *tmp = NULL;
    solution_t *current_solution = NULL, *current_path = NULL, *reachable = NULL;

    if (fscanf(stdin, "%lli", &start) == EOF) return 1;
    if (fscanf(stdin, "%lli", &goal) == EOF) return 1;

    if (start == goal) {
        printf("%lli\n", start);
        return 0;
    }

    current_node = get_table(start);

    current_path = add_list(start, current_path);

    current_solution = NULL;
    if (goal < start) direction = -1;
    max_length = -1;
    current_length = 1;
    reachable = current_node->reachable;
    if (reachable == NULL) return 1;


    while (reachable != NULL) {
        distance = reachable->station;
        tmp = get_table(distance);
        if (tmp != NULL) {
            tmp->visited = 1;
            if ((reachable->station > start && direction == 1) || (reachable->station < start && direction == -1)) {

                current_path = add_list(distance, current_path);
                explore(get_table(reachable->station), &current_path, &current_solution, goal,
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

void explore(station_t *node, solution_t **path, solution_t **solution, long long goal,
             int *max_length_ptr, int *current_length_ptr, int direction) {

    solution_t *tmp_s = NULL, *tmp_p = NULL, *reachable = NULL;
    solution_t *x = NULL, *y = NULL;
    station_t *tmp = NULL, *tmp_s_s = NULL, *tmp_p_s = NULL;
    long long distance;
    int choice = 0; // 1 = solution, -1 = path
    if (*max_length_ptr != -1 && *current_length_ptr == *max_length_ptr && node->distance != goal) return;
    if (node == NULL) return;
    if ((direction == 1 && node->distance > goal )||(direction == -11 && node->distance < goal )) return;
    if (node->distance == goal) {
        if (*max_length_ptr == -1 || *current_length_ptr < *max_length_ptr) {
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
                    tmp_s_s = get_table(tmp_s->station);
                    tmp_p_s = get_table(tmp_p->station);
                    if (tmp_s_s->distance == tmp_p_s->distance) {}
                    else if (tmp_s_s->distance < tmp_p_s->distance)
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
                tmp_s_s = get_table(tmp_s->station);
                tmp_p_s = get_table(tmp_p->station);
                if (tmp_s_s->distance == tmp_p_s->distance) {}
                else if (tmp_s_s->distance < tmp_p_s->distance) return;
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
        tmp = get_table(distance);
        if (tmp != NULL && !(solution == NULL && tmp->visited > 0 )) {
            if (distance != node->distance && (tmp->visited == 0 || *current_length_ptr <= tmp->visited)) {

                tmp->visited = *current_length_ptr;
                if ((distance > node->distance && direction == 1) ||
                    (distance < node->distance && direction == -1)) {

                    distance = reachable->station;
                    *current_length_ptr = *current_length_ptr + 1;
                    *path = add_list_head(distance, *path);
                    explore(get_table(reachable->station), path, solution, goal, max_length_ptr,
                            current_length_ptr, direction);

                    *current_length_ptr = *current_length_ptr - 1;
                    *path = remove_list(distance, *path);
                }
            }
        }
        reachable = reachable->next;
    }

}


solution_t *add_list(long long val, solution_t *head) {
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

solution_t *add_list_head(long long val, solution_t *head) {
    solution_t *tmp = NULL;
    tmp = (solution_t *) malloc(sizeof(solution_t));
    if (tmp == NULL) return NULL;
    tmp->next = head;
    tmp->station = val;
    return tmp;
}


solution_t *remove_list(long long val, solution_t *head) {
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
    long i = 0, j = 0;
    solution_t *tmp = NULL;
    station_t *tmp_s = NULL;
    for (i = 0; i < 4096; i++) {
        tmp_s = get_table(i);
        if (tmp_s != NULL) {
            printf("[%li]: (", i);
            for (j = 0; tmp_s->parked_cars[j] != 0; j++)
                printf(" %lli ", tmp_s->parked_cars[j]);
            printf(") reachable:");

            tmp = tmp_s->reachable;

            while (tmp != NULL) {
                if (tmp->next != NULL && tmp->station == tmp->next->station) {
                    printf("ERROR");
                    return;
                }
                printf(" %lli", tmp->station);
                tmp = tmp->next;
            }
            printf("\n");
        }
    }
    printf("\n");
}

void resetter() {
    long long i = 0;
    station_t *tmp = NULL;
    for (i = 0; i <= most_distant_station; i++) {
        tmp = get_table(i);
        if (tmp != NULL) tmp->visited = 0;
    }

}

void print_reverse(solution_t *head) {
    if (head == NULL) return;
    print_reverse(head->next);

    if (head->next != NULL) printf(" ");
    printf("%lli", head->station);
}

void print_direct(solution_t *head) {

    printf("%lli", head->station);
    head = head->next;
    while (head != NULL) {
        printf(" %lli", head->station);
        head = head->next;
    }
}

int get_input() {
    int i = 0;
    char tmp = '\0';
    for (i = 0; i < 30; i++) {
        tmp = (char) getc_unlocked(stdin);
        if (tmp == '\n') tmp = (char) getc_unlocked(stdin);
        if (tmp == EOF) return 1;
        if (tmp == ' ') break;
        input[i] = tmp;
    }
    for (i = i; i < 30; i++) {
        input[i] = '\0';
    }
    return 0;
}

int add_to_table(station_t *new_station) {
    long index;
    station_t *p = NULL;
    index = new_station->distance % SIZE;
    if (stations_table[index] == NULL || stations_table[index]->distance <= new_station->distance) {
        new_station->next = stations_table[index];
        stations_table[index] = new_station;
        return 0;
    }
    p = stations_table[index];
    while (p->next != NULL && p->next->distance > new_station->distance) {
        p = p->next;
    }
    if (p->next->distance == new_station->distance) return 1;
    new_station->next = p->next;
    p->next = new_station;
    return 0;
}

int remove_table(long long distance) {
    long index = distance % SIZE;
    station_t *p = NULL, *prv = NULL;
    if (stations_table[index] == NULL) return 1;
    prv = stations_table[index];
    p = prv->next;
    while (p != NULL && p->distance >= distance) {
        if (p->distance == distance) {
            prv->next = p->next;
            p->next = NULL;
            free(p);
            return 0;
        }
        prv = p;
        p = p->next;
    }
    return 1;

}

station_t *get_table(long long distance) {
    long index = distance % SIZE;
    station_t *p = NULL;
    if (stations_table[index] == NULL) return NULL;
    p = stations_table[index];
    while (p != NULL && p->distance >= distance) {
        if (p->distance == distance) return p;
        p = p->next;
    }
    return NULL;
}