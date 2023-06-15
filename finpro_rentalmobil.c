// AVL tree implementation in C

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Create Node
struct Node {
    char nama_mobil[50];
    char plat_no[10];
    int kapasitas;
    char transmisi[10];
    char status_sewa[10];
    int harga;
    struct Node *left;
    struct Node *right;
    int height;
};

int max(int a, int b);
void menu_inventory(struct Node* root);

// Calculate height
int height(struct Node *N) {
    if (N == NULL)
        return 0;
    return N->height;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

// Create a node
struct Node *newNode(char nama_mobil[], char plat_no[], int kapasitas, char transmisi[], char status_sewa[], int harga) {
    struct Node *node = (struct Node *)malloc(sizeof(struct Node));
    strcpy(node->nama_mobil, nama_mobil);
    strcpy(node->plat_no, plat_no);
    node->kapasitas = kapasitas;
    strcpy(node->transmisi, transmisi);
    strcpy(node->status_sewa, status_sewa);
    node->harga = harga;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return (node);
}

// Right rotate
struct Node *rightRotate(struct Node *y) {
    struct Node *x = y->left;
    struct Node *T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

// Left rotate
struct Node *leftRotate(struct Node *x) {
    struct Node *y = x->right;
    struct Node *T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

// Get the balance factor
int getBalance(struct Node *N) {
    if (N == NULL)
        return 0;
    return height(N->left) - height(N->right);
}

// Insert node
struct Node *insertNode(struct Node *node, char nama_mobil[], char plat_no[], int kapasitas, char transmisi[], char status_sewa[], int harga) {
    // Find the correct position to insertNode the node and insertNode it
    if (node == NULL)
        return (newNode(nama_mobil, plat_no, kapasitas, transmisi, status_sewa, harga));

    if (plat_no < node->plat_no)
        node->left = insertNode(node->left, nama_mobil, plat_no, kapasitas, transmisi, status_sewa, harga);
    else if (plat_no > node->plat_no)
        node->right = insertNode(node->right, nama_mobil, plat_no, kapasitas, transmisi, status_sewa, harga);
    else
        return node;

    // Update the balance factor of each node and
    // Balance the tree
    node->height = 1 + max(height(node->left),
                           height(node->right));

    int balance = getBalance(node);
    if (balance > 1 && plat_no < node->left->plat_no)
        return rightRotate(node);

    if (balance < -1 && plat_no > node->right->plat_no)
        return leftRotate(node);

    if (balance > 1 && plat_no > node->left->plat_no) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    if (balance < -1 && plat_no < node->right->plat_no) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

struct Node *minValueNode(struct Node *node) {
    struct Node *current = node;

    while (current->left != NULL)
        current = current->left;

    return current;
}

// Delete a nodes
struct Node *deleteNode(struct Node *root, char plat_no[]) {
    // Find the node and delete it
    if (root == NULL)
        return root;

    if (plat_no < root->plat_no)
        root->left = deleteNode(root->left, plat_no);

    else if (plat_no > root->plat_no)
        root->right = deleteNode(root->right, plat_no);

    else {
        if ((root->left == NULL) || (root->right == NULL)) {
            struct Node *temp = root->left ? root->left : root->right;

            if (temp == NULL) {
                temp = root;
                root = NULL;
            } else
                *root = *temp;
            free(temp);
        } else {
            struct Node *temp = minValueNode(root->right);

            strcpy(root->plat_no, temp->plat_no);

            root->right = deleteNode(root->right, temp->plat_no);
        }
    }

    if (root == NULL)
        return root;

    // Update the balance factor of each node and
    // balance the tree
    root->height = 1 + max(height(root->left),
                           height(root->right));

    int balance = getBalance(root);
    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);

    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);

    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

void reverse_inorder_traversal(struct Node* root) {
    if (root != NULL){
        reverse_inorder_traversal(root->right);
        printf("| %-20s | %-10s | %-9d | %-9s | %-10s | %-10d |\n", root->nama_mobil, root->plat_no, root->kapasitas, root->transmisi, root->status_sewa, root->harga);
        reverse_inorder_traversal(root->left);
    }
}

int main() {
    struct Node *root = NULL;
    int main_menu_option;
    do{
        printf("Rental Mobil\n");
        printf("============\n");
        printf("1. Inventory\n");
        printf("2. Sewa Mobil\n");
        printf("0. Exit\n");
        printf("============\n");
        printf("Input: ");
        scanf("%d", &main_menu_option);
        getchar();
        switch (main_menu_option){
            case 1:
                menu_inventory(root);
                break;
            case 2:
                break;
            case 0:
                exit(0);
                break;
            default:
                printf("Invalid input!\n");
                getchar();
                system("cls");
                break;
        }
    }while(main_menu_option != 0);
    return 0;
}

void menu_inventory(struct Node* root){
    int inventory_option;
    char input_nama_mobil[50];
    char input_plat_no[10];
    int input_kapasitas;
    char input_transmisi[10];
    char input_status_sewa[10];
    int input_harga;
    do{
        printf("Inventory\n");
        printf("=========\n");
        printf("1. Input\n");
        printf("2. View Data\n");
        printf("3. Delete\n");
        printf("0. Back\n");
        printf("============\n");
        printf("Input: ");
        scanf("%d", &inventory_option);
        getchar();
        switch(inventory_option){
            case 1:
                printf("Nama Mobil: ");
                scanf("%[^\n]", input_nama_mobil);
                getchar();
                printf("Plat Nomor: ");
                scanf("%[^\n]", input_plat_no);
                getchar();
                printf("Kapasitas Mobil: ");
                scanf("%d", &input_kapasitas);
                getchar();
                printf("Transmisi Mobil: ");
                scanf("%s", input_transmisi);
                getchar();
                printf("Status Mobil: ");
                scanf("%[^\n]", input_status_sewa);
                getchar();
                printf("Harga Sewa per Hari: ");
                scanf("%d", &input_harga);
                getchar();
                root = insertNode(root, input_nama_mobil, input_plat_no, input_kapasitas, input_transmisi, input_status_sewa, input_harga);
                break;
            case 2:

                printf("---------------------------------------------------------------------------------------\n");
                printf("| Nama Mobil           | Nomor Plat | Kapasitas | Transmisi | Status     | Harga Sewa |\n");
                printf("---------------------------------------------------------------------------------------\n");
                reverse_inorder_traversal(root);
                printf("---------------------------------------------------------------------------------------\n");
                system("pause");
                break;
            case 3:
                break;
            case 0:
                main();
                break;
            default:
                printf("Invalid input!\n");
                getchar();
                system("cls");
                break;
        }
    }while(inventory_option != 0);
}

