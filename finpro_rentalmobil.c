#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Inventory{
    char nama_mobil[50];
    char plat_no[10];
    int kapasitas;
    char transmisi[10];
    char status_sewa[10];
    struct Inventory *left;
    struct Inventory *right;
    int height;
};

struct Inventory* root = NULL;

struct Invoice{
    int noInvoice;
    char nama[25];
    char no_telp[15];
    char doc[10];
    int tanggal_sewa;
    int bulan_sewa;
    int tahun_sewa;
    int tanggal_kembali;
    int bulan_kembali;
    int tahun_kembali;
    int harga;
};


struct Inventory* create_node(char nama_mobil[], char plat_no[], int kapasitas, char transmisi[], char status_sewa[]);
struct Inventory* insert(struct Inventory *root, char nama_mobil[], char plat_no[], int kapasitas, char transmisi[], char status_sewa[]);
void inorder_traversal(struct Inventory* root);
struct Inventory* search(struct Inventory* root, char plat_no[]);
void menu_inventory();
void readFileCar();
int get_height(struct Inventory* node);
int get_balance_factor(struct Inventory* node);
void update_height(struct Inventory* node);
struct Inventory* rotate_right(struct Inventory* node);
struct Inventory* rotate_left(struct Inventory* node);
struct Inventory* balance_tree(struct Inventory* node);
struct Inventory* minValueNode(struct Inventory* node);
int max(int a, int b);

int main(){
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
                menu_inventory();
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

void menu_inventory(){
	readFileCar();
    int inventory_option;
    char input_nama_mobil[50];
    char input_plat_no[10];
    int input_kapasitas;
    char input_transmisi[10];
    char input_status_sewa[10];
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
                root = insert(root, input_nama_mobil, input_plat_no, input_kapasitas, input_transmisi, input_status_sewa);
                break;
            case 2:
                
                printf("------------------------------------------------\n");
                printf("| Nama Mobil | Nomor Plat | Kapasitas | Status |\n");
                printf("------------------------------------------------\n");
//                readFileCar();
                inorder_traversal(root);
                printf("------------------------------------------------\n");
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

void readFileCar(){
    FILE *fp;
    fp = fopen("data_mobil.txt", "r+");

    if(fp == NULL){
        printf("File doesn't exist !!!\n");
        return;
    }

    char temp[100];
    fscanf(fp, "%[^\n]%*c", temp);
	

    char nama_mobil[50];
    char plat_no[10];
    int kapasitas;
    char transmisi[10];
    char status_sewa[10];

    while(fscanf(fp, "%[^#]#%[^#]#%d#%[^#]#%[^#]#\r\n", nama_mobil, plat_no, &kapasitas, transmisi, status_sewa) != EOF){
        root = insert(root, nama_mobil, plat_no, kapasitas, transmisi, status_sewa);        
    }

    fclose(fp);
}

struct Inventory* create_node(char nama_mobil[], char plat_no[], int kapasitas, char transmisi[], char status_sewa[]){
    struct Inventory* new_node = (struct Inventory*) malloc(sizeof(struct Inventory));
    new_node->left = NULL;
    new_node->right = NULL;
    strcpy(new_node->nama_mobil, nama_mobil);
    strcpy(new_node->plat_no, plat_no);
    new_node->kapasitas = kapasitas;
    strcpy(new_node->transmisi, transmisi);
    strcpy(new_node->status_sewa, status_sewa);
    new_node->height = 1;
    return new_node;
}

struct Inventory* insert(struct Inventory *root, char nama_mobil[], char plat_no[], int kapasitas, char transmisi[], char status_sewa[]){
    if(root == NULL){
        root = create_node(nama_mobil, plat_no, kapasitas, transmisi, status_sewa);
        return root;
    }

    if(plat_no < root->plat_no){
        root->left = insert(root->left, nama_mobil, plat_no, kapasitas, transmisi, status_sewa);
    }
    else if(plat_no > root->plat_no){
        root->right = insert(root->right, nama_mobil, plat_no, kapasitas, transmisi, status_sewa);
    }
    else
        return root;

    return balance_tree(root);
}

void inorder_traversal(struct Inventory* root) {
    if (root == NULL){
        return;
    }
    inorder_traversal(root->left);
    printf("| %s | %s | %d | %s | %s |\n", root->nama_mobil, root->plat_no, root->kapasitas, root->transmisi, root->status_sewa);
    inorder_traversal(root->right);
}

struct Inventory* search(struct Inventory* root, char plat_no[]){
    if(root == NULL || strcmp(root->plat_no, plat_no) == 0){
        return root;
    }

    if(plat_no < root->plat_no)
        return search(root->left, plat_no);
    else
        return search(root->right, plat_no);
}

int get_height(struct Inventory* node){
    if(node == NULL)
        return 0;
    return node->height;
}

int get_balance_factor(struct Inventory* node){
    if(node == NULL)
        return 0;
    return get_height(node->left - get_height(node->right));
}

void update_height(struct Inventory* node){
    int left_height = get_height(node->left);
    int right_height = get_height(node->right);
    node->height = (left_height > right_height ? left_height : right_height) + 1;
}

struct Inventory* rotate_right(struct Inventory* node){
    struct Inventory* new_root = node->left;
    node->left = new_root->right;
    new_root->right = node;
    update_height(node);
    update_height(new_root);
    return new_root;
}

struct Inventory* rotate_left(struct Inventory* node){
    struct Inventory* new_root = node->right;
    node->right = new_root->left;
    new_root->left = node;
    update_height(node);
    update_height(new_root);
    return new_root;
}

struct Inventory* balance_tree(struct Inventory* node){
    update_height(node);
    int balance_factor = get_balance_factor(node);

    // LL Rotation
    if(balance_factor > 1 && get_balance_factor(node->left) >= 0)
        return rotate_right(node);

    // RR Rotation
    if(balance_factor < -1 && get_balance_factor(node->right) <= 0)
        return rotate_left(node);

    // LR Rotation
    if(balance_factor > 1 && get_balance_factor(node->left) < 0){
        node->left = rotate_left(node->left);
        return rotate_right(node);
    }

    // RL Rotation
    if(balance_factor < -1 && get_balance_factor(node->right) > 0){
        node->right = rotate_right(node->right);
        return rotate_left(node);
    }

    return node;
}

struct Inventory* minValueNode(struct Inventory* node) {
    struct Inventory* current = node;
    while (current->left != NULL)
        current = current->left;
    return current;
}

// Function to get the maximum of two integers
int max(int a, int b) {
    return (a > b) ? a : b;
}


