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

struct StackNode {
    long long data;
    long long autonomy;
    struct StackNode *next;
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
struct Station *curr = NULL, *prv = NULL;
long long most_distant_station = -1, start, goal, to_reach;
struct Solution *solution = NULL, *path = NULL;
struct StackNode *stack = NULL;
char input[30];
struct Station *local_min = NULL;
int current_length, max_length;

int add_station();

int remove_station();

int add_car();

int remove_car();

int path_planner();

int explore_direct(struct Station *);

int explore_reverse();

void explore_reverse_function(struct Station *);

struct Station *createStation(long long);

void inOrderTraversal(struct Station *);

struct Station *getStation(struct Station *, long long);

void leftRotate(struct Station *x);

void rightRotate(struct Station *y);

void insertFixup(struct Station *z);

struct Station *insert(long long data);

void deleteFixup(struct Station *x);

void deleteStation(struct Station *z);

struct Station *minimumStation(struct Station *);

void transplant(struct Station *u, struct Station *v);

int get_input();

void print_stations();

void create_stack(struct Station *);

void add_stack(long long);

void add_list(long long);

void add_path(long long);

void copy_list(struct Solution *, struct Solution *);

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
    int direction, flag;
    if (fscanf(stdin, "%lli", &start) == EOF) return 1;
    if (fscanf(stdin, "%lli", &goal) == EOF) return 1;
    if (root == NULL) return 1;
    //if (getStation(root, start) == NULL || getStation(root, goal) == NULL) return 1;
    if (start == goal) {
        printf("%lli\n", start);
        return 0;
    }
    free(solution);
    solution = NULL;
    add_list(goal);
    to_reach = goal;
    curr = root;
    direction = 1;
    if (start > goal) direction = -1;
    if (direction == 1) {
        while (solution->data != start) {
            if (explore_direct(root) == 1) return 1;
        }
    } else {
        flag = 0;
        while(1) {
            current_length =0;
            while (explore_reverse(root) != 1) {
                if (to_reach == start) {
                    flag = 1;
                    break;
                }
            }
            if (flag == 0) {
                if (solution != NULL) return 0;
                else return 1;
            }else{
                if (solution == NULL){

                }else{

                }
            }
        }
    }
    printf("%lli", solution->data);
    tmp_sol = solution->next;
    while (tmp_sol != NULL) {
        printf(" %lli", tmp_sol->data);
        tmp_sol = tmp_sol->next;
    }
    printf("\n");
    free(stack);
    stack = NULL;
    return 0;
}

int explore_direct(struct Station *node) {
    if (node != NULL) {
        if (node->data >= start && node->data < to_reach && node->data + node->cars[0] >= to_reach && (node->visited == -1 || node->visited >= current_length)) {
            add_list(node->data);
            to_reach = node->data;
            current_length++;
            return 0;
        }
        if (explore_direct(node->left) == 0) return 0;
        if (explore_direct(node->right) == 0) return 0;
    }
    return 1;
}

int explore_reverse(struct Station *node) {
    if (node != NULL) {
        if (node->data <= start && node->data > to_reach && node->data - node->cars[0] <= to_reach) {
            current_length++;
            add_path(node->data);
            to_reach = node->data;
            return 0;
        }
        if (explore_direct(node->left) == 0) return 0;
        if (explore_direct(node->right) == 0) return 0;
    }
    return 1;
}

void create_stack(struct Station *node){
    if (node != NULL) {
        inOrderTraversal(node->right);
        if(node->data >= goal && node->data <= start) add_stack(node->data);
        inOrderTraversal(node->left);
    }
}

void add_stack(long long data) {
    struct StackNode *tmp = NULL;
    tmp = (struct StackNode *) malloc(sizeof(struct StackNode));
    if (tmp == NULL) return;
    if (stack != NULL) tmp->next = stack;
    else tmp->next = NULL;
    tmp->data = data;
    stack = tmp;
}

int add_station() {
    long long i, j, distance, n_auto, autonomy, pos;
    int flag;
    struct Station *tmp;

    if (fscanf(stdin, "%lli", &distance) == EOF) return 1;
    if (fscanf(stdin, "%lli", &n_auto) == EOF) return 1;
    if (getStation(root, distance) != NULL) {

        for (i = 0; i < n_auto; i++) if (fscanf(stdin, "%lli", &autonomy) == EOF) return 1;
        return 1;
    }
    tmp = insert(distance);

    if (n_auto == 0) return 0;

    for (i = 0; i < n_auto; i++) {
        flag = 0;
        if (fscanf(stdin, "%lli", &autonomy) == EOF) return 1;
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
    deleteStation(tmp);
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
    for (j = 0; tmp->cars[j] != 0; j++) {
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
    for (i = 0; tmp->cars[i] != 0; i++) {
        if (tmp->cars[i] > autonomy) tmp_cars[i] = tmp->cars[i];
        if (tmp->cars[i] == autonomy) first = 0;
        if (tmp->cars[i] < autonomy) {
            if (first == 0) {
                tmp_cars[i - 1] = tmp->cars[i];
            } else return 1;
        }
    }
    for (i = 0; tmp->cars[i] != 0; i++) {
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

void deleteStation(struct Station *z) {
    struct Station *y = z;
    struct Station *x;
    enum Color yOriginalColor = y->color;

    if (z->left == NULL) {
        x = z->right;
        transplant(z, z->right);
    } else if (z->right == NULL) {
        x = z->left;
        transplant(z, z->left);
    } else {
        y = minimumStation(z->right);
        yOriginalColor = y->color;
        x = y->right;
        if (y->parent == z && x != NULL)
            x->parent = y;
        else if (y->right != NULL) {
            transplant(y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        transplant(z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }
    if (yOriginalColor == BLACK)
        //deleteFixup(x);
        free(z);
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