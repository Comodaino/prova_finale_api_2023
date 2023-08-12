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
long long most_distant_station = -1, start, goal, to_reach;
struct Solution *solution = NULL, *path = NULL;
char input[30];
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
    int direction;
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
        add_list(goal);
        while (solution->data != start) {
            if (explore_reverse(root) == 1) return 1;
        }
        if (solution == NULL) return 1;

        struct Solution *tmp_s;
        struct Solution *first;
        struct Solution *middle;
        struct Solution *last;
        struct Station *tmp_middle = NULL, *tmp_first = NULL, *tmp_last = NULL;
        tmp_s = solution;
        first = tmp_s;
        middle = first->next;
        if (middle->next != NULL) {
            last = middle->next;
            while (last != NULL) {
                tmp_first = getStation(root, first->data);
                if (tmp_first->data - tmp_first->cars[0] <= last->data) {
                    free(middle);
                    first->next = last;

                    first = last;
                    middle = last->next;
                    if (middle == NULL) break;
                    last = middle->next;

                } else {
                    first = middle;
                    middle = last;
                    last = last->next;
                }
            }
        }
        solution = tmp_s;
        tmp_s = reverseList(solution);
        first = tmp_s;
        middle = first->next;
        if (middle->next != NULL) {
            last = middle->next;
            while (last != NULL) {

                tmp_middle = findMaxLessThanValue(root, middle->data);
                tmp_last = getStation(root, last->data);
                while (tmp_middle != NULL && tmp_middle->data > first->data) {
                    if (tmp_middle->data - tmp_middle->cars[0] <= first->data &&
                        tmp_last->data - tmp_last->cars[0] <= tmp_middle->data) {
                        middle->data = tmp_middle->data;
                    }
                    tmp_middle = findMaxLessThanValue(root, tmp_middle->data);
                }
                first = middle;
                middle = last;
                last = last->next;
            }
        }
        solution = reverseList(tmp_s);
        tmp_s = solution;
        first = tmp_s;
        middle = first->next;
        if (middle->next != NULL) {
            last = middle->next;
            while (last != NULL) {
                tmp_first = getStation(root, first->data);
                if (tmp_first->data - tmp_first->cars[0] <= last->data) {
                    free(middle);
                    first->next = last;

                    first = last;
                    middle = last->next;
                    if (middle == NULL) break;
                    last = middle->next;

                } else {
                    first = middle;
                    middle = last;
                    last = last->next;
                }
            }
        }
        tmp_s = solution;
        first = tmp_s;
        middle = first->next;
        if (middle->next != NULL) {
            last = middle->next;
            while (last != NULL) {
                tmp_first = getStation(root, first->data);
                if (tmp_first->data - tmp_first->cars[0] <= last->data) {
                    free(middle);
                    first->next = last;

                    first = last;
                    middle = last->next;
                    if (middle == NULL) break;
                    last = middle->next;

                } else {
                    first = middle;
                    middle = last;
                    last = last->next;
                }
            }
        }
        solution = tmp_s;

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

struct Station *deleteStation(struct Station *root, long long data) {
    if (root == NULL) {
        return root;
    }

    if (data < root->data) {
        root->left = deleteStation(root->left, data);
    } else if (data > root->data) {
        root->right = deleteStation(root->right, data);
    } else {
        if (root->left == NULL) {
            struct Station *temp = root->right;
            if (temp != NULL) {
                temp->parent = root->parent;
            }
            free(root);
            return temp;
        } else if (root->right == NULL) {
            struct Station *temp = root->left;
            if (temp != NULL) {
                temp->parent = root->parent;
            }
            free(root);
            return temp;
        }

        struct Station *temp = findMin(root->right);
        root->data = temp->data;
        root->right = deleteStation(root->right, temp->data);
    }
    return root;
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