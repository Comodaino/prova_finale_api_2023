#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 2097152

typedef struct Solution {
    long long station;
    struct Solution *next;
} solution_t;

typedef struct Station {
    long long distance;
    long long parked_cars[512];
    int visited;
    struct Station *next;
    struct Solution *reachable_direct;
    struct Solution *reachable_inverse;
} station_t;

station_t **stations_table = NULL, **tmp_table = NULL;
solution_t *stations_created = NULL;
char input[30];
long long most_distant_station = 0;

void print_stations();

int add_station();

int remove_station();

int add_car();

int remove_car();

int path_planner();

void explore_direct(station_t *, solution_t **, solution_t **, long long, int *, int *);

void explore_inverse(station_t *, solution_t **, solution_t **, long long, int *, int *);

solution_t *add_list(long long, solution_t *);

solution_t *add_list_head(long long, solution_t *);

solution_t *remove_list(long long, solution_t *);

solution_t *copy_list(solution_t *);

solution_t *copy_partly(solution_t *, solution_t **);

void print_reverse(solution_t *);

void print_direct(solution_t *);

void resetter();

int get_input();

int add_to_table(station_t *);

int remove_table(long long);

//station_t *get_table(long long);

int main() {

    stations_table = (station_t **) malloc(SIZE * sizeof(station_t));

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

        } else {
            printf("problem");

        }
    }
    return 0;
}


int add_station() {
    long long n_auto = 0, distance = 0, autonomy = 0, i = 0;
    station_t *new_station = NULL, *tmp = NULL;
    int j = 0, pos = 0, flag = 0;
    solution_t *st_l = NULL;


    if (fscanf(stdin, "%lli", &distance) == EOF) return 1;
    if (fscanf(stdin, "%lli", &n_auto) == EOF) return 1;

    if (stations_table[distance] != NULL) {
        for (i = 0; i < n_auto; i++) {
            if (fscanf(stdin, "%lli", &autonomy) == EOF) return 1;
        }
        return 1;
    }

    new_station = (station_t *) malloc(sizeof(station_t));
    new_station->reachable_direct = NULL;
    new_station->next = NULL;
    new_station->distance = distance;
    for (i = 0; i < 512; i++) new_station->parked_cars[i] = 0;
    new_station->visited = 0;

    if (most_distant_station == -1 || most_distant_station < distance) most_distant_station = distance;
    if (distance > most_distant_station && distance > SIZE) {
        *tmp_table = (station_t *) malloc(most_distant_station * sizeof(station_t));
        for (i = 0; i < most_distant_station; i++) {
            tmp_table[i] = stations_table[i];//assigning elements of p to q
        }

        free(stations_table);//releasing the memory held by pointer p

        stations_table = tmp_table; //assigning the address held by q to p for the array
        tmp_table = NULL;
    }

    stations_table[distance] = new_station;

    stations_created = add_list_head(distance, stations_created);

    //check for other stations
    st_l = stations_created;
    while (st_l != NULL) {
        tmp = stations_table[st_l->station];
        if (tmp != NULL && i != distance) {
            if (i < distance && tmp->parked_cars[0] + tmp->distance > distance) {
                tmp->reachable_direct = add_list(distance, tmp->reachable_direct);
            }
            if (i > distance && tmp->distance - tmp->parked_cars[0] < distance) {
                tmp->reachable_inverse = add_list(distance, tmp->reachable_inverse);
            }
        }
        st_l = st_l->next;
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
    i = 0;
    if (distance > new_station->parked_cars[0]) i = distance - new_station->parked_cars[0];
    while (i <= distance + new_station->parked_cars[0]) {
        if (stations_table[i] != NULL && i != distance) {
            if (i > distance) new_station->reachable_direct = add_list(i, new_station->reachable_direct);
            else if (i < distance) new_station->reachable_inverse = add_list(i, new_station->reachable_inverse);
        }
        i++;
    }

    return 0;
}


int remove_station() {
    long long distance;
    station_t *tmp = NULL;
    solution_t *st_l = NULL;
    if (fscanf(stdin, "%lli", &distance) == EOF) return 1;
    if (stations_table[distance] == NULL) return 1;
    stations_table[distance] = NULL;
    stations_created = remove_list(distance, stations_created);
    st_l = stations_created;
    while (st_l != NULL) {
        tmp = stations_table[st_l->station];
        if (tmp != NULL) {
            if (st_l->station < distance) tmp->reachable_direct = remove_list(distance, tmp->reachable_direct);
            else tmp->reachable_inverse = remove_list(distance, tmp->reachable_inverse);
        }
        st_l = st_l->next;
    }
    return 0;
}


int add_car() {
    int pos = 0, j = 0, flag = 0;
    long long distance, autonomy, i = 0;
    station_t *tmp = NULL;
    if (fscanf(stdin, "%lli", &distance) == EOF) return 1;
    if (fscanf(stdin, "%lli", &autonomy) == EOF) return 1;
    if (autonomy == 0) return 0;
    tmp = stations_table[distance];
    if (tmp == NULL) return 1;
    if (tmp->parked_cars[0] == 0) {
        tmp->parked_cars[0] = autonomy;
        return 0;
    }
    flag = 0;
    for (j = 0; tmp->parked_cars[j] != 0; j++) {
        if (tmp->parked_cars[j] == autonomy) flag = 1;
        if (tmp->parked_cars[j] < autonomy) break;
    }
    pos = j;
    for (j = 511; j >= pos; j--) {
        tmp->parked_cars[j + 1] = tmp->parked_cars[j];
    }
    tmp->parked_cars[pos] = autonomy;
    if (flag == 0) {
        i = 0;
        if (distance > tmp->parked_cars[0]) i = distance - tmp->parked_cars[0];
        while (i <= distance + tmp->parked_cars[0]) {
            if (stations_table[i] != NULL) {
                if (i > distance) tmp->reachable_direct = add_list(i, tmp->reachable_direct);
                else if (i < distance) tmp->reachable_inverse = add_list(i, tmp->reachable_inverse);
            }
            i++;
        }
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

    tmp2 = tmp->reachable_direct;
    while (tmp2 != NULL) {
        if (tmp2->station > distance + tmp->parked_cars[0])
            tmp->reachable_direct = remove_list(tmp2->station, tmp->reachable_direct);
        tmp2 = tmp2->next;
    }
    tmp2 = tmp->reachable_inverse;
    while (tmp2 != NULL) {
        if (tmp2->station < distance + tmp->parked_cars[0])
            tmp->reachable_inverse = remove_list(tmp2->station, tmp->reachable_inverse);
        tmp2 = tmp2->next;
    }
    return 0;
}


int path_planner() {
    int max_length = -1, current_length = 0, direction = 1;
    long long start, goal;
    station_t *current_node = NULL, *tmp = NULL;
    solution_t *current_solution = NULL, *current_path = NULL, *reachable_inverse = NULL, *reachable_direct = NULL;

    if (fscanf(stdin, "%lli", &start) == EOF) return 1;
    if (fscanf(stdin, "%lli", &goal) == EOF) return 1;

    if (start == goal) {
        printf("%lli\n", start);
        return 0;
    }

    current_node = stations_table[start];

    current_path = add_list_head(start, current_path);

    current_solution = NULL;
    if (goal < start) direction = -1;
    max_length = -1;
    current_length = 1;

    if (direction == 1) {
        reachable_direct = current_node->reachable_direct;
        if (reachable_direct == NULL) return 1;
        while (reachable_direct != NULL) {
            if (reachable_direct->station > current_node->distance && reachable_direct->station <= goal) {
                tmp = stations_table[reachable_direct->station];
                if (tmp->visited == 0 || tmp->visited < current_length) {
                    current_length = current_length + 1;
                    current_path = add_list_head(tmp->distance, current_path);
                    explore_direct(tmp, &current_path, &current_solution, goal, &max_length, &current_length);
                    current_path = remove_list(tmp->distance, current_path);
                    current_length = current_length - 1;
                }
            }
            reachable_direct = reachable_direct->next;
        }
    } else {
        reachable_inverse = current_node->reachable_inverse;
        if (reachable_inverse == NULL) return 1;
        while (reachable_inverse != NULL) {
            if (reachable_inverse->station < current_node->distance && reachable_inverse->station >= goal) {
                tmp = stations_table[reachable_inverse->station];
                if (tmp->visited == 0 || tmp->visited < current_length) {
                    current_length = current_length + 1;
                    current_path = add_list_head(tmp->distance, current_path);
                    explore_inverse(tmp, &current_path, &current_solution, goal, &max_length, &current_length);
                    current_path = remove_list(tmp->distance, current_path);
                    current_length = current_length - 1;
                }
            }
            reachable_inverse = reachable_inverse->next;
        }
    }


    if (current_solution == NULL) return 1;
    if (direction == -1) print_reverse(current_solution);
    else print_direct(current_solution);

    printf("\n");
    return 0;
}

void explore_direct(station_t *node, solution_t **path, solution_t **solution, long long goal,
                    int *max_length_ptr, int *current_length_ptr) {

    solution_t *tmp_s = NULL, *tmp_p = NULL, *reachable_direct = NULL;
    station_t *tmp = NULL;
    if (node->distance == goal) {
        if (*solution == NULL || *current_length_ptr < *max_length_ptr) {
            *solution = copy_list(*path);
            *max_length_ptr = *current_length_ptr;
            return;
        }
        if (*max_length_ptr == *current_length_ptr) {
            tmp_s = *solution;
            tmp_p = *path;
            while (tmp_s != NULL && tmp_p != NULL) {
                if (tmp_s->station < tmp_p->station) return;
                if (tmp_s->station > tmp_p->station) {
                    *solution = copy_list(*path);
                    *max_length_ptr = *current_length_ptr;
                    return;
                }
                tmp_s = tmp_s->next;
                tmp_p = tmp_p->next;
            }
        }
    }
    if (*max_length_ptr != -1 && *current_length_ptr >= *max_length_ptr) return;
    reachable_direct = node->reachable_direct;
    while (reachable_direct != NULL) {
        if (reachable_direct->station > node->distance && reachable_direct->station <= goal) {
            tmp = stations_table[reachable_direct->station];
            if (tmp->visited == 0 || tmp->visited > *current_length_ptr) {
                tmp->visited = *current_length_ptr;
                *current_length_ptr = *current_length_ptr + 1;
                *path = add_list_head(tmp->distance, *path);
                explore_direct(tmp, path, solution, goal, max_length_ptr, current_length_ptr);
                *path = remove_list(tmp->distance, *path);
                *current_length_ptr = *current_length_ptr - 1;
            }
        }
        reachable_direct = reachable_direct->next;
    }
}

void explore_inverse(station_t *node, solution_t **path, solution_t **solution, long long goal,
                     int *max_length_ptr, int *current_length_ptr) {

    solution_t *tmp_s = NULL, *tmp_p = NULL, *reachable_inverse = NULL; /* *tmp_reach = NULL */
    station_t *tmp = NULL;

    if (node->distance == goal) {

        if (*solution == NULL || *current_length_ptr < *max_length_ptr) {
            *solution = copy_list(*path);
            *max_length_ptr = *current_length_ptr;
            return;
        }
        if (*max_length_ptr == *current_length_ptr) {

            tmp_s = *solution;
            tmp_p = *path;
            while (tmp_s != NULL && tmp_p != NULL) {
                if (tmp_s->station < tmp_p->station) return;
                if (tmp_s->station > tmp_p->station) {
                    *solution = copy_list(*path);
                    *max_length_ptr = *current_length_ptr;
                    return;
                }
                tmp_s = tmp_s->next;
                tmp_p = tmp_p->next;
            }
        }
    }
    if (*max_length_ptr != -1 && *current_length_ptr >= *max_length_ptr) return;
    reachable_inverse = node->reachable_inverse;
    while (reachable_inverse != NULL) {
        if (reachable_inverse->station < node->distance && reachable_inverse->station >= goal) {
            tmp = stations_table[reachable_inverse->station];
            if (tmp->visited == 0 || tmp->visited > *current_length_ptr) {
                tmp->visited = *current_length_ptr;
                *current_length_ptr = *current_length_ptr + 1;
                *path = add_list_head(tmp->distance, *path);
                explore_inverse(tmp, path, solution, goal, max_length_ptr, current_length_ptr);
                *path = remove_list(tmp->distance, *path);
                *current_length_ptr = *current_length_ptr - 1;
            }
            /*
            if (*solution != NULL && tmp->visited != 0 && tmp->visited == *current_length_ptr) {
                tmp_reach = *solution;
                while (tmp_reach != NULL && tmp_reach->station == tmp->distance) tmp_reach = tmp_reach->next;
                if (tmp_reach != NULL) {
                    tmp_reach = tmp_reach->next;
                    if (node->distance < tmp_reach->station) {
                        //copy_partly(*path, solution);
                    }
                }
            }
             */
        }
        reachable_inverse = reachable_inverse->next;
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

solution_t *copy_partly(solution_t *path, solution_t **sol) {
    solution_t *p = NULL, *head = NULL;

    if (path == NULL) return NULL;
    if (sol == NULL || *sol == NULL) return path;
    p = *sol;
    while (p != NULL && p->station != path->station) {
        p = p->next;
    }
    while (p != NULL) {
        p->station = path->station;
        path = path->next;
        p = p->next;
    }

    return head;
}

void print_stations() {
    printf("Stations:\n");
    long long i = 0;
    station_t *tmp_s = NULL;
    for (i = 0; i < SIZE; i++) {
        tmp_s = stations_table[i];
        if (tmp_s != NULL) {
            printf("[%lli]:", i);
            while (tmp_s != NULL) {
                printf(" %lli", tmp_s->distance);
                tmp_s = tmp_s->next;
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
        tmp = stations_table[i];
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
    for (; i < 30; i++) {
        input[i] = '\0';
    }
    return 0;
}
