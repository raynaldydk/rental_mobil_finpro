// AVL tree implementation in C

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Create Node for Car Inventory
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

//Create Node for Invoice
struct Invoice{
    int noInvoice;
    char nama_sewa[50];
    char plat_no_sewa[10];
    char no_telp_sewa[15];
    char dok_sewa[25];
    char tgl_sewa[15];
    char tgl_kembali[15];
    int totalHarga;
    struct Invoice *left;
    struct Invoice *right;
    int height;
};

int max(int a, int b);
void menu_inventory(struct Node* root);
int height(struct Node *N);
struct Node *newNode(char nama_mobil[], char plat_no[], int kapasitas, char transmisi[], char status_sewa[], int harga);
struct Node *rightRotate(struct Node *y);
struct Node *leftRotate(struct Node *x);
int getBalance(struct Node *N);
struct Node *insertNode(struct Node *node, char nama_mobil[], char plat_no[], int kapasitas, char transmisi[], char status_sewa[], int harga);
struct Node *minValueNode(struct Node *node);
struct Node *deleteNode(struct Node *root, char plat_no[]);
struct Node *searchNode(struct Node *node, char plat_no[]);
void inv_inorder_traversal(struct Node* root);

// function for sewa_mobil
void menu_sewamobil(struct Node *root, struct Invoice **invRoot);
int heightInv(struct Invoice *N);
struct Invoice *newInv(int noInvoice, char *nama_sewa, char *plat_no_sewa, char *no_telp_sewa, char *dok_sewa, char *tgl_sewa, char *tgl_kembali, int totalHarga);
struct Invoice *rightRotateInv(struct Invoice *y);
struct Invoice *leftRotateInv(struct Invoice *x);
int getBalanceInv(struct Invoice *N);
struct Invoice *insertInvoice(struct Invoice *node, int noInvoice, char *nama_sewa, char *plat_no_sewa, char *no_telp_sewa, char *dok_sewa, char *tgl_sewa, char *tgl_kembali, int totalHarga);
struct Invoice *minValueInvoice(struct Invoice *node);
struct Invoice *deleteInvoice(struct Invoice *root, int noInvoice);
struct Invoice *searchInvoice(struct Invoice *node, int noInvoice);
void inorder_traversal(struct Invoice* root);
int generateInvoiceNumber();
void read_file(struct Node** root);
void print_receipt(int input_noInvoice, struct Node* root, char input_plat_no_sewa[], char input_nama_sewa[],
                   char input_no_telp_sewa[], char input_dok_sewa[], char input_tgl_sewa[], char input_tgl_kembali[], int input_totalHarga);

int main() {
    struct Node *root = NULL;
    struct Invoice *invRoot = NULL;
    int main_menu_option;
    read_file(&root);
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
                menu_sewamobil(root, &invRoot);
                break;
            case 0:
                exit(0);
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
    // read_file(&root);
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

                FILE *fp = fopen("data_mobil.txt", "a");
                fprintf(fp, "%s, %s, %d, %s, %s, %d\n", input_nama_mobil, input_plat_no, input_kapasitas, input_transmisi, input_status_sewa, input_harga);
                fclose(fp);
                break;
            case 2:
            if(root == NULL){
                printf("Data tidak ada!\n");
                system("pause");
            }else{
                printf("---------------------------------------------------------------------------------------\n");
                printf("| Nama Mobil           | Nomor Plat | Kapasitas | Transmisi | Status     | Harga Sewa |\n");
                printf("---------------------------------------------------------------------------------------\n");
                inv_inorder_traversal(root);
                printf("---------------------------------------------------------------------------------------\n");
                system("pause");
            }
                break;
            case 3:
                printf("Masukkan Nomor Kendaraan: ");
                scanf("%[^\n]", input_plat_no);
                getchar();

                struct Node *result = searchNode(root, input_plat_no);
                if(result == NULL){
                    printf("Data not found!\n");
                }else{
                    root = deleteNode(root, input_plat_no);
                }
                break;
            case 0:
                return;
            default:
                printf("Invalid input!\n");
                getchar();
                system("cls");
                break;
        }
    }while(inventory_option != 0);
}

void menu_sewamobil(struct Node* root, struct Invoice** invRoot){
    int sewa_option;
    int input_noInvoice;
    char input_plat_no_sewa[10];
    char input_nama_sewa[50];
    char input_no_telp_sewa[15];
    char input_dok_sewa[25];
    char input_tgl_sewa[15];
    char input_tgl_kembali[15];
    int input_totalHarga;
    int input_tanggal_sewa;
    int input_bulan_sewa;
    int input_tahun_sewa;
    int input_tanggal_kembali;
    int input_bulan_kembali;
    int input_tahun_kembali;
    struct Node *result;
    do{
        printf("Data Sewa Mobil\n");
        printf("=========\n");
        printf("1. Input\n");
        printf("2. View Data\n");
        printf("3. Update\n");
        printf("0. Back\n");
        printf("============\n");
        printf("Input: ");
        scanf("%d", &sewa_option);
        getchar();
        switch(sewa_option){
            case 1:
                input_noInvoice = generateInvoiceNumber();
                printf("Plat Mobil yang Disewa: ");
                scanf("%[^\n]", input_plat_no_sewa);
                getchar();
                result = searchNode(root, input_plat_no_sewa);
                if(result == NULL){
                    printf("Data not found!\n");
                    break;
                }

                printf("Nama Penyewa: ");
                scanf("%[^\n]", input_nama_sewa);
                getchar();
                do{
                    printf("Nomor Telp Penyewa: ");
                    scanf("%s", input_no_telp_sewa);
                    getchar();
                }while(strlen(input_no_telp_sewa) < 10 || strlen(input_no_telp_sewa) > 13);

                do{
                    printf("Dokumen Penjamin[KTP/SIM/KK]: ");
                    scanf("%[^\n]", input_dok_sewa);
                    getchar();
                }while(strcmp(input_dok_sewa, "KTP") != 0 && strcmp(input_dok_sewa,"SIM") != 0 && strcmp(input_dok_sewa, "KK") != 0);
                do{
                    printf("Tanggal Sewa[dd/mm/yyyy]: ");
                    scanf("%d/%d/%d", &input_tanggal_sewa, &input_bulan_sewa, &input_tahun_sewa);
                    getchar();
                }while(input_tanggal_sewa < 0 || input_tanggal_sewa > 31 || input_bulan_sewa < 0 || input_bulan_sewa > 12 || input_tahun_sewa < 2000 || input_tahun_sewa > 2100);
                do{
                    printf("Tanggal Kembali Sewa[dd/mm/yyyy]: ");
                    scanf("%d/%d/%d", &input_tanggal_kembali, &input_bulan_kembali, &input_tahun_kembali);
                    getchar();
                }while(input_tanggal_kembali < 0 || input_tanggal_kembali > 31 || input_bulan_kembali < 0 || input_bulan_kembali > 12 || input_tahun_kembali < 2000 || input_tahun_kembali > 2100);

                sprintf(input_tgl_kembali, "%d/%d/%d", input_tanggal_kembali, input_bulan_kembali, input_tahun_kembali);
                sprintf(input_tgl_sewa, "%d/%d/%d", input_tanggal_sewa, input_bulan_sewa, input_tahun_sewa);
                input_totalHarga = ((atoi(input_tgl_kembali)) - (atoi(input_tgl_sewa))) * result->harga;
                strcpy(result->status_sewa, "Disewa");

                // printf("Total Harga: ");
                // scanf("%d", &input_totalHarga);
                // getchar();
                *invRoot = insertInvoice(*invRoot, input_noInvoice, input_nama_sewa, input_plat_no_sewa, input_no_telp_sewa, input_dok_sewa, input_tgl_sewa, input_tgl_kembali, input_totalHarga);
                print_receipt(input_noInvoice, root, input_plat_no_sewa, input_nama_sewa, input_no_telp_sewa, input_dok_sewa, input_tgl_sewa, input_tgl_kembali, input_totalHarga);
                break;
            case 2:
                if(*invRoot == NULL){
                    printf("Data tidak ada!\n");
                    system("pause");
                }else{
                    printf("=======================================================================================================\n");
                    printf("|  No  |    Nama    | Plat No |  No Telepon  | Dokumen | Tanggal Sewa | Tanggal Kembali | Total Harga |\n");
                    printf("=======================================================================================================\n");
                    inorder_traversal(*invRoot);
                    printf("=======================================================================================================\n");
                }
                break;
            case 3:
                printf("Masukkan Nomor Invoice: ");
                scanf("%d", &input_noInvoice);
                getchar();

                struct Invoice *resultInv = searchInvoice(*invRoot, input_noInvoice);
                result = searchNode(root, resultInv->plat_no_sewa);
                if(resultInv == NULL){
                    printf("Data not found!\n");
                }else{
                    strcpy(result->status_sewa, "Ada");
                    printf("Mobil dengan Plat Nomor %s Sudah dikembalikan\n", resultInv->plat_no_sewa);
                    // kalo mau delete
                    // invRoot = deleteInvoice(invRoot, input_noInvoice);
                }
                break;
            case 0:
                break;
            default:
                printf("Invalid input!\n");
                getchar();
                system("cls");
                break;
        }
    }while(sewa_option != 0);
}


// Calculate height
int height(struct Node *N) {
    if (N == NULL)
        return 0;
    return N->height;
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

int max(int a, int b) {
    return (a > b) ? a : b;
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

    if (strcmp(plat_no, node->plat_no) < 0)
        node->left = insertNode(node->left, nama_mobil, plat_no, kapasitas, transmisi, status_sewa, harga);
    else if (strcmp(plat_no, node->plat_no) > 0)
        node->right = insertNode(node->right, nama_mobil, plat_no, kapasitas, transmisi, status_sewa, harga);
    else
        return node;

    // Update the balance factor of each node and
    // Balance the tree
    node->height = 1 + max(height(node->left),
                           height(node->right));

    int balance = getBalance(node);
    if (balance > 1 && strcmp(plat_no, node->left->plat_no) < 0)
        return rightRotate(node);

    if (balance < -1 && strcmp(plat_no, node->right->plat_no) > 0)
        return leftRotate(node);

    if (balance > 1 && strcmp(plat_no, node->left->plat_no) > 0) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    if (balance < -1 && strcmp(plat_no, node->right->plat_no) < 0) {
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

    if (strcmp(plat_no, root->plat_no) < 0)
        root->left = deleteNode(root->left, plat_no);

    else if (strcmp(plat_no, root->plat_no) > 0)
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

struct Node *searchNode(struct Node *node, char plat_no[]){
    if(node == NULL){
        return node;
    }else if(strcmp(plat_no, node->plat_no) == 0){
        return node;
    }else if(strcmp(plat_no, node->plat_no) < 0){
        return searchNode(node->left, plat_no);
    }else if(strcmp(plat_no, node->plat_no) > 0){
        return searchNode(node->right, plat_no);
    }
}

void inv_inorder_traversal(struct Node* root) {
    if (root != NULL){
        inv_inorder_traversal(root->left);
        printf("| %-20s | %-10s | %-9d | %-9s | %-10s | %-10d |\n", root->nama_mobil, root->plat_no, root->kapasitas, root->transmisi, root->status_sewa, root->harga);
        inv_inorder_traversal(root->right);
    }
}

void read_file(struct Node** root){
    char nama_mobil[50], plat_no[25], transmisi[10], status_sewa[10];
    int kapasitas, harga;
    char buffer[100];
    FILE *fp = fopen("data_mobil.txt", "r+");
    if(fp == NULL){
        printf("File not found!");
    }
    fgets(buffer, 100, fp);
    while(fscanf(fp, "%[^,], %[^,], %d, %[^,], %[^,], %d\n", nama_mobil, plat_no, &kapasitas, transmisi, status_sewa, &harga) != EOF){
        *root = insertNode(*root, nama_mobil, plat_no, kapasitas, transmisi, status_sewa, harga);
    }
    fclose(fp);
}

int heightInv(struct Invoice *N){
    if (N == NULL)
        return 0;
    return N->height;
}

struct Invoice *newInv(int noInvoice, char *nama_sewa, char *plat_no_sewa, char *no_telp_sewa, char *dok_sewa, char *tgl_sewa, char *tgl_kembali, int totalHarga){
    struct Invoice *node = (struct Invoice *)malloc(sizeof(struct Invoice));
    node->noInvoice = noInvoice;
    strcpy(node->nama_sewa, nama_sewa);
    strcpy(node->plat_no_sewa, plat_no_sewa);
    strcpy(node->no_telp_sewa, no_telp_sewa);
    strcpy(node->dok_sewa, dok_sewa);
    strcpy(node->tgl_sewa, tgl_sewa);
    strcpy(node->tgl_kembali, tgl_kembali);
    node->totalHarga = totalHarga;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return (node);
}

struct Invoice *rightRotateInv(struct Invoice *y){
    struct Invoice *x = y->left;
    struct Invoice *T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(heightInv(y->left), heightInv(y->right)) + 1;
    x->height = max(heightInv(x->left), heightInv(x->right)) + 1;

    return x;
}

struct Invoice *leftRotateInv(struct Invoice *x){
    struct Invoice *y = x->right;
    struct Invoice *T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(heightInv(x->left), heightInv(x->right)) + 1;
    y->height = max(heightInv(y->left), heightInv(y->right)) + 1;

    return y;
}

int getBalanceInv(struct Invoice *N){
    if (N == NULL)
        return 0;
    return heightInv(N->left) - heightInv(N->right);
}

struct Invoice *insertInvoice(struct Invoice *node, int noInvoice, char *nama_sewa, char *plat_no_sewa, char *no_telp_sewa, char *dok_sewa, char *tgl_sewa, char *tgl_kembali, int totalHarga){
    // Find the correct position to insertNode the node and insertNode it
    if (node == NULL)
        return (newInv(noInvoice, nama_sewa, plat_no_sewa, no_telp_sewa, dok_sewa, tgl_sewa, tgl_kembali, totalHarga));

    if (noInvoice < node->noInvoice)
        node->left = insertInvoice(node->left, noInvoice, nama_sewa, plat_no_sewa, no_telp_sewa, dok_sewa, tgl_sewa, tgl_kembali, totalHarga);
    else if (noInvoice > node->noInvoice)
        node->right = insertInvoice(node->right, noInvoice, nama_sewa, plat_no_sewa, no_telp_sewa, dok_sewa, tgl_sewa, tgl_kembali, totalHarga);
    else
        return node;

    // Update the balance factor of each node and
    // Balance the tree
    node->height = 1 + max(heightInv(node->left),
                           heightInv(node->right));

    int balance = getBalanceInv(node);
    if (balance > 1 && noInvoice < node->left->noInvoice)
        return rightRotateInv(node);

    if (balance < -1 && noInvoice > node->right->noInvoice)
        return leftRotateInv(node);

    if (balance > 1 && noInvoice > node->left->noInvoice) {
        node->left = leftRotateInv(node->left);
        return rightRotateInv(node);
    }

    if (balance < -1 && noInvoice < node->right->noInvoice) {
        node->right = rightRotateInv(node->right);
        return leftRotateInv(node);
    }

    return node;
}

struct Invoice *minValueInvoice(struct Invoice *node){
    struct Invoice *current = node;

    while (current->left != NULL)
        current = current->left;

    return current;
}

struct Invoice *deleteInvoice(struct Invoice *root, int noInvoice){
    // Find the node and delete it
    if (root == NULL)
        return root;

    if (noInvoice < root->noInvoice)
        root->left = deleteInvoice(root->left, noInvoice);

    else if (noInvoice > root->noInvoice)
        root->right = deleteInvoice(root->right, noInvoice);

    else {
        if ((root->left == NULL) || (root->right == NULL)) {
            struct Invoice *temp = root->left ? root->left : root->right;

            if (temp == NULL) {
                temp = root;
                root = NULL;
            } else
                *root = *temp;
            free(temp);
        } else {
            struct Invoice *temp = minValueInvoice(root->right);

            root->noInvoice = temp->noInvoice;
            // strcpy(root->plat_no, temp->plat_no);

            root->right = deleteInvoice(root->right, temp->noInvoice);
        }
    }

    if (root == NULL)
        return root;

    // Update the balance factor of each node and
    // balance the tree
    root->height = 1 + max(heightInv(root->left),
                           heightInv(root->right));

    int balance = getBalanceInv(root);
    if (balance > 1 && getBalanceInv(root->left) >= 0)
        return rightRotateInv(root);

    if (balance > 1 && getBalanceInv(root->left) < 0) {
        root->left = leftRotateInv(root->left);
        return rightRotateInv(root);
    }

    if (balance < -1 && getBalanceInv(root->right) <= 0)
        return leftRotateInv(root);

    if (balance < -1 && getBalanceInv(root->right) > 0) {
        root->right = rightRotateInv(root->right);
        return leftRotateInv(root);
    }

    return root;
}

struct Invoice *searchInvoice(struct Invoice *node, int noInvoice){
    if(node == NULL){
        return node;
    }else if(noInvoice == node->noInvoice){
        return node;
    }else if(noInvoice < node->noInvoice){
        return searchInvoice(node->left, noInvoice);
    }else if(noInvoice > node->noInvoice){
        return searchInvoice(node->right, noInvoice);
    }
    else{
        return node;
    }
}

void inorder_traversal(struct Invoice* root){
       if (root != NULL){
        inorder_traversal(root->left);
        printf("| %-3d | %-10s | %-7s | %-10s | %-7s | %-12s | %-15s | %-11d |\n", root->noInvoice, root->nama_sewa, root->plat_no_sewa, root->no_telp_sewa, root->dok_sewa, root->tgl_sewa, root->tgl_kembali, root->totalHarga);
        inorder_traversal(root->right);
    }
}

int generateInvoiceNumber() {
    // Get the current time as a seed for random number generation
    srand(time(NULL));

    // Generate a random number between 1000 and 9999
    int invoiceNumber = rand() % 9000 + 1000;

    return invoiceNumber;
}

void print_receipt(int input_noInvoice, struct Node* root, char input_plat_no_sewa[], char input_nama_sewa[],
        char input_no_telp_sewa[], char input_dok_sewa[], char input_tgl_sewa[], char input_tgl_kembali[], int input_totalHarga){
    struct Node* temp = searchNode(root, input_plat_no_sewa);
    printf("Rental Mobil Joss\n");
    printf("===================================================\n");
    printf("Receipt order #%d\n\n", input_noInvoice);
    printf("Nama Penyewa: %s\n", input_nama_sewa);
    printf("No Telp: %s\n", input_no_telp_sewa);
    printf("Mobil yang Disewa: %s\n", temp->nama_mobil);
    printf("Plat Nomor: %s\n", input_plat_no_sewa);
    printf("Dokumen Penjamin: %s\n", input_dok_sewa);
    printf("Tanggal Sewa: %s\n", input_tgl_sewa);
    printf("Tanggal Kembali: %s\n", input_tgl_kembali);
    printf("Total Harga Sewa: %d\n", input_totalHarga);
    printf("==================================================\n");
}
