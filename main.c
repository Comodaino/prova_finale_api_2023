#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 2097152
enum Color {
    RED, BLACK
};


struct Station {
    long long data, cars[512];
    enum Color color;
    struct Station *parent;
    struct Station *left;
    struct Station *right;
};

struct Station *root = NULL;
long long most_distant_station = -1;
struct Station **stations_table = NULL;
char input[30];


int add_station();

int remove_station();

int add_car();

int remove_car();

struct Station *createStation(long long data);

void leftRotate(struct Station *x);

void rightRotate(struct Station *y);

void insertFixup(struct Station *z);

void insert(int data);

void deleteFixup(struct Station *x);

void deleteStation(struct Station *z);

struct Station *minimumStation(struct Station *station);

void transplant(struct Station *u, struct Station *v);

int get_input();

void print_stations();

int main() {
    stations_table = (struct Station **) malloc(SIZE * sizeof(struct Station));

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
            print_stations();
            //if (path_planner() == 1) printf("nessun percorso\n");

        } else {
            printf("problem");

        }
    }
    return 0;
}

int add_station(){
    long long i, j, distance, n_auto, autonomy, pos;
    int flag;
    struct Station *tmp, **tmp_table = NULL;

    if (fscanf(stdin, "%lli", &distance) == EOF) return 1;
    if (fscanf(stdin, "%lli", &n_auto) == EOF) return 1;
    if(stations_table[distance] != NULL) return 1;
    tmp = createStation(distance);
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

    if (most_distant_station == -1 || most_distant_station < distance) most_distant_station = distance;
    if (distance > most_distant_station && distance > SIZE) {
        tmp_table = (struct Station **) malloc(most_distant_station * sizeof(struct Station));
        for (i = 0; i < most_distant_station; i++) {
            tmp_table[i] = stations_table[i];//assigning elements of p to q
        }
        free(stations_table);//releasing the memory held by pointer p
        stations_table = tmp_table; //assigning the address held by q to p for the array
        tmp_table = NULL;
    }
    stations_table[distance] = tmp;
    return 0;
}

int remove_station(){
    long long distance;
    struct Station *tmp;
    if (fscanf(stdin, "%lli", &distance) == EOF) return 1;
    tmp = stations_table[distance];
    if(tmp == NULL) return 1;
    deleteStation(tmp);
    return 0;
}

int add_car(){
    int pos = 0, j = 0;
    long long distance, autonomy;
    struct Station *tmp = NULL;
    if (fscanf(stdin, "%lli", &distance) == EOF) return 1;
    if (fscanf(stdin, "%lli", &autonomy) == EOF) return 1;
    if (autonomy == 0) return 0;
    tmp = stations_table[distance];
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

int remove_car(){
    int i, first = 1;
    long long distance, autonomy;
    long long tmp_cars[512] = {0};
    struct Station *tmp = NULL;

    if (fscanf(stdin, "%lli", &distance) == EOF) return 1;
    if (fscanf(stdin, "%lli", &autonomy) == EOF) return 1;
    tmp = stations_table[distance];
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
    while (z->parent != NULL && z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            struct Station *y = z->parent->parent->right;
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
            // Symmetric cases
            // ...
        }
    }
    (root)->color = BLACK;
}

void insert(int data) {
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

    insertFixup(z);
}

void deleteFixup(struct Station* x) {
    while (x != root && (x == NULL || x->color == BLACK)) {
        if (x == x->parent->left) {
            struct Station* w = x->parent->right;
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
            // Symmetric cases
            // ...
        }
    }
    if (x != NULL)
        x->color = BLACK;
}

void deleteStation(struct Station* z) {
    struct Station* y = z;
    struct Station* x;
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
        if (y->parent == z)
            x->parent = y;
        else {
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
        deleteFixup(x);
    free(z);
}

// Utility function to find the minimum Station in a subtree
struct Station* minimumStation(struct Station* station) {
    while (station->left != NULL)
        station = station->left;
    return station;
}

// Utility function to replace one subtree with another
void transplant(struct Station* u, struct Station* v) {
    if (u->parent == NULL)
        root = v;
    else if (u == u->parent->left)
        u->parent->left = v;
    else
        u->parent->right = v;
    if (v != NULL)
        v->parent = u->parent;
}

void print_stations(){
    long long max, i, j;
    if(most_distant_station == -1) max = SIZE;
    else max = most_distant_station;
    for(i=0; i<max; i++){
        if(stations_table[i]!=NULL){
            printf("%lli :", stations_table[i]->data);
            for(j=0; j<512; j++){
                printf(" %lli", stations_table[i]->cars[j]);
            }
            printf("\n");
        }
    }
}