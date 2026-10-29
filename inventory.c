
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "products.txt"
#define MAX_PRODUCTS 200

struct Product {
    int id;
    char name[50];
    int quantity;
    double price;
};

int loadProducts(struct Product products[]) {
    FILE *file = fopen(FILE_NAME, "r");
    if (file == NULL) return 0;

    int count = 0;

    while (count < MAX_PRODUCTS &&
           fscanf(file, "%d %49s %d %lf",
                  &products[count].id,
                  products[count].name,
                  &products[count].quantity,
                  &products[count].price) == 4) {
        count++;
    }

    fclose(file);
    return count;
}

int saveProducts(struct Product products[], int count) {
    FILE *file = fopen(FILE_NAME, "w");

    if (file == NULL) {
        printf("Error saving products!\n");
        return 0;
    }

    for (int i = 0; i < count; i++) {
        fprintf(file, "%d %s %d %.2f\n",
                products[i].id,
                products[i].name,
                products[i].quantity,
                products[i].price);
    }

    fclose(file);
    return 1;
}

int findProduct(struct Product products[], int count, int id) {
    for (int i = 0; i < count; i++) {
        if (products[i].id == id) return i;
    }

    return -1;
}

void addProduct(void) {
    struct Product products[MAX_PRODUCTS];
    int count = loadProducts(products);
    struct Product p;

    if (count >= MAX_PRODUCTS) {
        printf("Inventory is full!\n");
        return;
    }

    printf("Enter product ID: ");
    if (scanf("%d", &p.id) != 1) {
        printf("Invalid ID.\n");
        while (getchar() != '\n');
        return;
    }

    if (findProduct(products, count, p.id) != -1) {
        printf("Product ID already exists!\n");
        return;
    }

    printf("Enter product name (one word): ");
    if (scanf("%49s", p.name) != 1) return;

    printf("Enter quantity: ");
    if (scanf("%d", &p.quantity) != 1 || p.quantity < 0) {
        printf("Quantity must be zero or greater.\n");
        while (getchar() != '\n');
        return;
    }

    printf("Enter price: ");
    if (scanf("%lf", &p.price) != 1 || p.price < 0) {
        printf("Price must be zero or greater.\n");
        while (getchar() != '\n');
        return;
    }

    products[count] = p;

    if (saveProducts(products, count + 1)) {
        printf("Product added successfully!\n");
    }
}

void viewProducts(void) {
    struct Product products[MAX_PRODUCTS];
    int count = loadProducts(products);
    double inventoryValue = 0;

    if (count == 0) {
        printf("No products found.\n");
        return;
    }

    printf("\n%-8s %-18s %-10s %-12s\n",
           "ID", "Name", "Quantity", "Price");
    printf("------------------------------------------------\n");

    for (int i = 0; i < count; i++) {
        printf("%-8d %-18s %-10d Rs. %-8.2f\n",
               products[i].id,
               products[i].name,
               products[i].quantity,
               products[i].price);

        inventoryValue += products[i].quantity * products[i].price;
    }

    printf("\nTotal inventory value: Rs. %.2f\n", inventoryValue);
}

void searchProduct(void) {
    struct Product products[MAX_PRODUCTS];
    int count = loadProducts(products);
    int id;

    printf("Enter product ID: ");
    if (scanf("%d", &id) != 1) {
        printf("Invalid ID.\n");
        while (getchar() != '\n');
        return;
    }

    int index = findProduct(products, count, id);

    if (index == -1) {
        printf("Product not found.\n");
    } else {
        printf("Name: %s\n", products[index].name);
        printf("Quantity: %d\n", products[index].quantity);
        printf("Price: Rs. %.2f\n", products[index].price);
    }
}

void updateStock(void) {
    struct Product products[MAX_PRODUCTS];
    int count = loadProducts(products);
    int id, newQuantity;

    printf("Enter product ID: ");
    if (scanf("%d", &id) != 1) {
        printf("Invalid ID.\n");
        while (getchar() != '\n');
        return;
    }

    int index = findProduct(products, count, id);

    if (index == -1) {
        printf("Product not found.\n");
        return;
    }

    printf("Current quantity: %d\n", products[index].quantity);
    printf("Enter new quantity: ");

    if (scanf("%d", &newQuantity) != 1 || newQuantity < 0) {
        printf("Quantity must be zero or greater.\n");
        while (getchar() != '\n');
        return;
    }

    products[index].quantity = newQuantity;

    if (saveProducts(products, count)) {
        printf("Stock updated successfully!\n");
    }
}

int main(void) {
    int choice;

    do {
        printf("\n===== INVENTORY MANAGEMENT =====\n");
        printf("1. Add Product\n");
        printf("2. View Inventory\n");
        printf("3. Search Product\n");
        printf("4. Update Stock\n");
        printf("5. Exit\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Please enter a number.\n");
            while (getchar() != '\n');
            choice = 0;
            continue;
        }

        switch (choice) {
            case 1: addProduct(); break;
            case 2: viewProducts(); break;
            case 3: searchProduct(); break;
            case 4: updateStock(); break;
            case 5: printf("Inventory system closed.\n"); break;
            default: printf("Invalid choice!\n");
        }
    } while (choice != 5);

    return 0;
}
