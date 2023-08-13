#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 2097152
enum Color {
    RED, BLACK
};

struct Solution {
    long long data;
    struct Solution *next;
};

struct Station {
    long long data, cars[512];
    enum Color color;
    int visited;
    struct Station *parent;
    struct Station *left;
    struct Station *right;
};

struct Station *root = NULL;
struct Station *curr = NULL;
long long most_distant_station = -1, start, goal, to_reach, to_reach_tmp, to_exclude;
struct Solution *solution = NULL, *path = NULL;
char input[30];
int current_length, max_length, to_exclude_index;

int add_station();

int remove_station();

int add_car();

int remove_car();

int path_planner();

int explore_direct(struct Station *);

int explore_reverse();

int explore_reverse_new_path(struct Station *);

struct Station *createStation(long long);

void inOrderTraversal(struct Station *);

struct Station *getStation(struct Station *, long long);

void leftRotate(struct Station *x);

void rightRotate(struct Station *y);

void insertFixup(struct Station *z);

struct Station *findMin(struct Station *);

struct Station *insert(long long data);

void deleteFixup(struct Station *x);

struct Station *deleteStation(struct Station *, long long);

struct Station *minimumStation(struct Station *);

void transplant(struct Station *u, struct Station *v);

int get_input();

void print_stations();


void add_list(long long);

void add_path(long long);

void remove_path(long long);

struct Solution *copy_list(struct Solution *);

void resetter(struct Station *);

void print_reverse(struct Solution *);

struct Station *findMaxLessThanValue(struct Station *, long long);

struct Solution *reverseList(struct Solution *);

int main() {
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
            //inOrderTraversal(root);
            //print_stations();

            if (path_planner() == 1) printf("nessun percorso\n");

        } else {
            printf("problem");
        }
    }
    return 0;
}

int path_planner() {
    struct Solution *tmp_sol;
    int direction, i;
    if (fscanf(stdin, "%lli", &start) == EOF) return 1;
    if (fscanf(stdin, "%lli", &goal) == EOF) return 1;
    if (root == NULL) return 1;
    //if (getStation(root, start) == NULL || getStation(root, goal) == NULL) return 1;
    if (start == goal) {
        printf("%lli\n", start);
        return 0;
    }
    free(solution);
    free(path);
    path = NULL;
    solution = NULL;
    to_reach = goal;
    curr = root;
    direction = 1;
    if (start > goal) direction = -1;
    if (direction == 1) {
        add_list(goal);
        while (solution->data != start) {
            if (explore_direct(root) == 1) return 1;
        }
        if (solution == NULL) return 1;
        printf("%lli", solution->data);
        tmp_sol = solution->next;
        while (tmp_sol != NULL) {
            printf(" %lli", tmp_sol->data);
            tmp_sol = tmp_sol->next;
        }
        printf("\n");
        return 0;
    } else {
        struct Solution *inverse_x = NULL, *inverse_y = NULL, *inverse_z = NULL;
        max_length = 0;
        add_list(goal);
        while (solution->data != start) {
            max_length++;
            if (explore_reverse(root) == 1) return 1;
        }

        if (solution == NULL) return 1;

        to_exclude_index = 0;
        while (to_exclude != start) {
            tmp_sol = copy_list(solution);
            inverse_x = reverseList(tmp_sol);
            inverse_y = inverse_x;
            for (i = 0; i < to_exclude_index; i++) inverse_x = inverse_x->next;
            to_exclude = inverse_x->next->data;
            to_reach = inverse_x->data;
            curr = findMaxLessThanValue(root, to_exclude);

            free(path);
            path = NULL;
            while (curr != NULL && curr->data > to_reach) {
                free(path);
                path = NULL;
                if (curr->data - curr->cars[0] <= to_reach) {
                    to_reach_tmp = curr->data;
                    current_length = to_exclude_index;
                    inverse_z = inverse_y;
                    for (i = 0; i <= to_exclude_index; i++) {
                        add_path(inverse_z->data);
                        inverse_z = inverse_z->next;
                    }
                    add_path(curr->data);

                    while (path != NULL && path->data != start) {
                        current_length++;

                        if (current_length >= max_length || explore_reverse_new_path(root) == 1) {
                            free(path);
                            path = NULL;
                            current_length = 0;
                        }
                    }

                    if (path != NULL) {
                        free(solution);
                        solution = copy_list(path);
                    }
                }
                curr = findMaxLessThanValue(root, curr->data);
            }
            to_exclude_index++;
        }


        printf("%lli", solution->data);
        tmp_sol = solution->next;
        while (tmp_sol != NULL) {
            printf(" %lli", tmp_sol->data);
            tmp_sol = tmp_sol->next;
        }
        printf("\n");
        return 0;
    }

}

int explore_reverse_new_path(struct Station *node) {
    if (node != NULL) {
        if (explore_reverse_new_path(node->right) == 0) return 0;
        if (node->data <= start && node->data > to_reach_tmp && node->data - node->cars[0] <= to_reach_tmp) {
            if (node->data != to_exclude && node->data != curr->data) {
                add_path(node->data);
                to_reach_tmp = node->data;
                return 0;
            }
        }
        if (explore_reverse_new_path(node->left) == 0) return 0;
    }
    return 1;
}

int explore_direct(struct Station *node) {
    if (node != NULL) {
        if (explore_direct(node->left) == 0) return 0;
        if (node->data >= start && node->data < to_reach && node->data + node->cars[0] >= to_reach) {
            add_list(node->data);
            to_reach = node->data;
            return 0;
        }
        if (explore_direct(node->right) == 0) return 0;
    }
    return 1;
}

int explore_reverse(struct Station *node) {
    if (node != NULL) {
        if (explore_reverse(node->right) == 0) return 0;
        if (node->data <= start && node->data > to_reach && node->data - node->cars[0] <= to_reach) {
            add_list(node->data);
            to_reach = node->data;
            return 0;
        }
        if (explore_reverse(node->left) == 0) return 0;
    }
    return 1;
}


struct Solution *reverseList(struct Solution *head) {
    struct Solution *prev = NULL;
    struct Solution *current = head;
    struct Solution *nextNode;

    while (current != NULL) {
        nextNode = current->next;
        current->next = prev;
        prev = current;
        current = nextNode;
    }

    return prev;
}

void resetter(struct Station *node) {
    if (node != NULL) {
        resetter(node->left);
        node->visited = 0;
        resetter(node->right);
    }
}

struct Solution *copy_list(struct Solution *node_p) {
    struct Solution *tmp_s, *head;
    tmp_s = (struct Solution *) malloc(sizeof(struct Solution));
    head = tmp_s;
    while (node_p != NULL) {
        tmp_s->data = node_p->data;
        tmp_s->next = NULL;
        node_p = node_p->next;
        if (node_p != NULL) tmp_s->next = (struct Solution *) malloc(sizeof(struct Solution));
        tmp_s = tmp_s->next;
    }
    return head;
}

int add_station() {
    long long i, j, distance, n_auto, autonomy, pos;
    int flag;
    struct Station *tmp;

    if (fscanf(stdin, "%lli", &distance) == EOF) return 1;
    if (fscanf(stdin, "%lli", &n_auto) == EOF) return 1;
    tmp = getStation(root, distance);
    if (tmp != NULL) {
        for (i = 0; i < n_auto; i++) if (fscanf(stdin, "%lli", &autonomy) == EOF) return 1;
        return 1;
    }
    tmp = insert(distance);
    for (i = 0; i < 512; i++) {
        tmp->cars[i] = 0;
    }


    if (n_auto == 0) return 0;
    autonomy = 0;


    for (i = 0; i < n_auto; i++) {
        flag = 0;
        if (fscanf(stdin, "%lli", &autonomy) == EOF) return 0;

        for (j = 0; j < 512; j++) {
            if (tmp->cars[j] == autonomy) flag = 1;
            if (tmp->cars[j] < autonomy) break;
        }
        if (flag == 0) {
            pos = j;
            for (j = 511; j >= pos; j--) tmp->cars[j + 1] = tmp->cars[j];
            tmp->cars[pos] = autonomy;
        }
    }

    return 0;
}

int remove_station() {
    long long distance;
    struct Station *tmp;
    if (fscanf(stdin, "%lli", &distance) == EOF) return 1;
    tmp = getStation(root, distance);
    if (tmp == NULL) return 1;
    deleteStation(root, distance);
    //free(tmp);
    return 0;
}

int add_car() {
    int pos = 0, j = 0;
    long long distance, autonomy;
    struct Station *tmp = NULL;
    if (fscanf(stdin, "%lli", &distance) == EOF) return 1;
    if (fscanf(stdin, "%lli", &autonomy) == EOF) return 1;
    if (autonomy == 0) return 0;
    tmp = getStation(root, distance);

    if (tmp == NULL) return 1;
    if (tmp->cars[0] == 0) {
        tmp->cars[0] = autonomy;
        return 0;
    }
    for (j = 0; tmp->cars[j] != 0 && j < 512; j++) {
        if (tmp->cars[j] < autonomy) break;
    }
    pos = j;
    for (j = 511; j >= pos; j--) {
        tmp->cars[j + 1] = tmp->cars[j];
    }
    tmp->cars[pos] = autonomy;
    return 0;
}

int remove_car() {
    int i, first = 1;
    long long distance, autonomy;
    long long tmp_cars[512] = {0};
    struct Station *tmp = NULL;

    if (fscanf(stdin, "%lli", &distance) == EOF) return 1;
    if (fscanf(stdin, "%lli", &autonomy) == EOF) return 1;
    tmp = getStation(root, distance);

    if (tmp == NULL) return 1;
    for (i = 0; tmp->cars[i] != 0 && i < 512; i++) {
        if (tmp->cars[i] > autonomy) tmp_cars[i] = tmp->cars[i];
        if (tmp->cars[i] == autonomy) first = 0;

        if (tmp->cars[i] < autonomy) {
            if (first == 0) {
                tmp_cars[i - 1] = tmp->cars[i];
            } else return 1;
        }
    }
    for (i = 0; tmp->cars[i] != 0 && i < 512; i++) {
        tmp->cars[i] = tmp_cars[i];
    }
    return 0;
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

void inOrderTraversal(struct Station *node) {
    if (node != NULL) {
        inOrderTraversal(node->left);
        printf("%lli", node->data);
        inOrderTraversal(node->right);
    }
}

struct Station *findMaxLessThanValue(struct Station *node, long long value) {
    if (node == NULL) {
        return NULL;
    }

    if (node->data >= value) {
        return findMaxLessThanValue(node->left, value);
    }

    struct Station *rightMax = findMaxLessThanValue(node->right, value);
    if (rightMax != NULL) {
        return rightMax;
    }

    return (node->data < value) ? node : NULL;
}

struct Station *createStation(long long data) {
    struct Station *newStation = (struct Station *) malloc(sizeof(struct Station));
    newStation->data = data;
    newStation->color = RED;
    newStation->parent = NULL;
    newStation->left = NULL;
    newStation->right = NULL;
    return newStation;
}

void leftRotate(struct Station *x) {
    struct Station *y = x->right;
    x->right = y->left;

    if (y->left != NULL)
        y->left->parent = x;

    y->parent = x->parent;

    if (x->parent == NULL)
        root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;

    y->left = x;
    x->parent = y;
}

void rightRotate(struct Station *y) {
    struct Station *x = y->left;
    y->left = x->right;

    if (x->right != NULL)
        x->right->parent = y;

    x->parent = y->parent;

    if (y->parent == NULL)
        root = x;
    else if (y == y->parent->left)
        y->parent->left = x;
    else
        y->parent->right = x;

    x->right = y;
    y->parent = x;
}

void insertFixup(struct Station *z) {
    struct Station *y;
    while (z->parent != NULL && z->parent->color == RED) {
        if (z->parent->parent == NULL) break;

        if (z->parent == z->parent->parent->left) {
            y = z->parent->parent->right;
            if (y != NULL && y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    leftRotate(z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rightRotate(z->parent->parent);
            }
        } else {
            y = z->parent->parent->left;
            if (y != NULL && y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    rightRotate(z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                leftRotate(z->parent->parent);
            }

        }
    }
}

struct Station *insert(long long data) {
    struct Station *z = createStation(data);
    struct Station *y = NULL;
    struct Station *x = root;

    while (x != NULL) {
        y = x;
        if (z->data < x->data)
            x = x->left;
        else
            x = x->right;
    }

    z->parent = y;

    if (y == NULL)
        root = z;
    else if (z->data < y->data)
        y->left = z;
    else
        y->right = z;

    //insertFixup(z);

    return z;
}

void deleteFixup(struct Station *x) {
    struct Station *w;
    while (x != root && (x == NULL || x->color == BLACK)) {
        if (x == x->parent->left) {
            w = x->parent->right;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                leftRotate(x->parent);
                w = x->parent->right;
            }
            if ((w->left == NULL || w->left->color == BLACK) &&
                (w->right == NULL || w->right->color == BLACK)) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->right == NULL || w->right->color == BLACK) {
                    if (w->left != NULL)
                        w->left->color = BLACK;
                    w->color = RED;
                    rightRotate(w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                if (w->right != NULL)
                    w->right->color = BLACK;
                leftRotate(x->parent);
                x = root;
            }
        } else {
            w = x->parent->right;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                rightRotate(x->parent);
                w = x->parent->left;
            }
            if ((w->right == NULL || w->right->color == BLACK) &&
                (w->left == NULL || w->left->color == BLACK)) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->left == NULL || w->left->color == BLACK) {
                    if (w->right != NULL)
                        w->right->color = BLACK;
                    w->color = RED;
                    rightRotate(w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                if (w->right != NULL)
                    w->right->color = BLACK;
                leftRotate(x->parent);
                x = root;
            }
        }
    }
    if (x != NULL)
        x->color = BLACK;
}

struct Station *deleteStation(struct Station *node, long long data) {
    if (node == NULL) {
        return node;
    }

    if (data < node->data) {
        node->left = deleteStation(node->left, data);
    } else if (data > node->data) {
        node->right = deleteStation(node->right, data);
    } else {
        if (node->left == NULL) {
            struct Station *temp = node->right;
            if (temp != NULL) {
                temp->parent = node->parent;
            }
            free(node);
            return temp;
        } else if (node->right == NULL) {
            struct Station *temp = node->left;
            if (temp != NULL) {
                temp->parent = root->parent;
            }
            free(node);
            return temp;
        }

        struct Station *temp = findMin(node->right);
        node->data = temp->data;
        int i = 0;
        for (i = 0; i < 512; i++) node->cars[i] = temp->cars[i];
        node->right = deleteStation(node->right, temp->data);
    }
    return node;
}

struct Station *findMin(struct Station *node) {
    while (node->left != NULL) {
        node = node->left;
    }
    return node;
}

// Utility function to find the minimum Station in a subtree
struct Station *minimumStation(struct Station *station) {
    if (station == NULL) return NULL;
    while (station->left != NULL)
        station = station->left;
    return station;
}

// Utility function to replace one subtree with another
void transplant(struct Station *u, struct Station *v) {
    if (u->parent == NULL)
        root = v;
    else if (u == u->parent->left)
        u->parent->left = v;
    else
        u->parent->right = v;
    if (v != NULL)
        v->parent = u->parent;
}

void add_list(long long data) {
    struct Solution *tmp = NULL;
    tmp = (struct Solution *) malloc(sizeof(struct Solution));
    if (tmp == NULL) return;
    if (solution != NULL) tmp->next = solution;
    else tmp->next = NULL;
    tmp->data = data;
    solution = tmp;
}

void add_path(long long data) {
    struct Solution *tmp = NULL;
    tmp = (struct Solution *) malloc(sizeof(struct Solution));
    if (tmp == NULL) return;
    if (path != NULL) tmp->next = path;
    else tmp->next = NULL;
    tmp->data = data;
    path = tmp;
}

void remove_path(long long data) {
    struct Solution *tmp = path->next;
    free(path);
    path = tmp;
}

struct Station *getStation(struct Station *node, long long data) {

    if (node == NULL || node->data == data) return node;
    if (node->data > data) return getStation(node->left, data);
    return getStation(node->right, data);
}

void print_stations() {
    long long max, i, j;
    if (most_distant_station == -1) max = SIZE;
    else max = most_distant_station;
    for (i = 0; i < max; i++) {
        if (getStation(root, i) != NULL) {
            printf("%lli :", getStation(root, i)->data);
            for (j = 0; j < 512; j++) {
                printf(" %lli", getStation(root, i)->cars[j]);
            }
            printf("\n");
        }
    }
}

void print_reverse(struct Solution *head) {
    if (head == NULL) return;
    print_reverse(head->next);

    if (head->next != NULL) printf(" ");
    printf("%lli", head->data);
}



/*
int explore_reverse(struct Station *node) {
    long long index;
    struct Solution *tmp_s, *tmp_p;
    struct Station *reachable;
    if (node->data == goal) {

        if (solution == NULL || current_length < max_length) {
            solution = copy_list(path);
            max_length = current_length;
            return 0;
        }
        if (max_length == current_length) {
            tmp_s = solution;
            tmp_p = path;
            while (tmp_s != NULL && tmp_p != NULL) {
                if (tmp_s->data < tmp_p->data) return 0;
                if (tmp_s->data > tmp_p->data) {
                    free(solution);
                    solution = copy_list(path);
                    return 0;
                }
                tmp_s = tmp_s->next;
                tmp_p = tmp_p->next;
            }
        }
    }

    if (max_length != -1 && current_length >= max_length) return 1;
    reachable = node;
    while (1) {
        reachable = findMaxLessThanValue(root, reachable->data);
        if (reachable == NULL || !(reachable->data < node->data && reachable->data >= goal && reachable->data >= node->data - node->cars[0])) return 0;
        index = reachable->data;
        if (reachable->visited == 0 || reachable->visited >= current_length) {
            reachable->visited = current_length;
            current_length++;
            add_path(index);
            explore_reverse(reachable);
            remove_path(index);
            current_length--;
        }
    }
    return 0;
}
*/


/*
int exit = 0;
long long index;
struct Station *node, *reachable;
resetter(root);
free(path);
add_path(start);
max_length = -1;
current_length = 0;
node = getStation(root, start);
if (node == NULL) return 1;
reachable = node;
while (exit == 0) {
    reachable = findMaxLessThanValue(root, reachable->data);
    index = reachable->data;
    if (reachable == NULL || !(reachable->data < node->data && reachable->data >= goal && reachable->data >= node->data - node->cars[0])) exit = 1;
    if (exit == 0) {
        if (reachable->visited == 0 || reachable->visited < current_length) {
            reachable->visited = current_length;
            current_length++;
            add_path(index);
            explore_reverse(reachable);
            remove_path(index);
            current_length--;
        }
    }
}
if (solution == NULL) return 1;
print_reverse(solution);
printf("\n");
return 0; */