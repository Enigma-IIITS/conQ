#include <stdio.h>
#include <stdlib.h>

#define Users 20
#define Items 10
#define NameLen 50
#define USER_FILE "userData.txt"
#define STORE_FILE "storeData.txt"

typedef struct {
    int id;
    float price;
    char name[NameLen];
    int quantity;
} Item;

typedef struct {
    int id;
    char name[NameLen];
    Item items[Items];
    int count;
} User;

typedef struct {
    User users[Users];
    int size;
    int front;
    int rear;
} SQUEUE;

void DEF(SQUEUE *queue) {
    queue->size = 0;
    queue->front = 0;
    queue->rear = -1;
}

void EN(SQUEUE *queue, User user) {
    if (queue->size >= Users) {
        fprintf(stderr, "Full\n");
        return;
    }

    queue->rear = (queue->rear + 1) % Users;
    queue->users[queue->rear] = user;
    queue->size++;
}

User DE(SQUEUE *queue) {
    User user = {0};  

    if (queue->size <= 0) {
        fprintf(stderr, "Empty\n");
        return user;
    }

    user = queue->users[queue->front];
    queue->front = (queue->front + 1) % Users;
    queue->size--;

    return user;
}

void UPrint(User user) {
    printf("User ID: %d, Name: %s\n", user.id, user.name);
    printf("Items Purchased:\n");
    for (int i = 0; i < user.count; ++i) {
        printf("  Item ID: %d, Name: %s, Price: %.2f, Quantity: %d\n",
               user.items[i].id, user.items[i].name, user.items[i].price, user.items[i].quantity);
    }
}

void save(const char *filename, User user) {
    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        fprintf(stderr, "Error %s\n", filename);
        return;
    }

    fprintf(file, "%d %s %d", user.id, user.name, user.count);

    for (int i = 0; i < user.count; ++i) {
        fprintf(file, " %d %s %.2f %d", user.items[i].id, user.items[i].name,
                user.items[i].price, user.items[i].quantity);
    }

    fprintf(file, "\n");

    fclose(file);
}

void load(const char *filename, SQUEUE *queue) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error %s\n", filename);
        return;
    }

    User user;
    while (fscanf(file, "%d %s %d", &user.id, user.name, &user.count) == 3) {
        for (int i = 0; i < user.count; ++i) {
            fscanf(file, " %d %s %f %d", &user.items[i].id, user.items[i].name,
                   &user.items[i].price, &user.items[i].quantity);
        }
        EN(queue, user);
    }

    fclose(file);
}

void saveStore(const char *filename, const SQUEUE *queue) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        fprintf(stderr, "Error %s\n", filename);
        return;
    }

    for (int i = 0; i < queue->size; ++i) {
        User user = queue->users[(queue->front + i) % Users];

        fprintf(file, "User ID: %d, Name: %s\n", user.id, user.name);
        fprintf(file, "Items Purchased:\n");

        for (int j = 0; j < user.count; ++j) {
            fprintf(file, "  Item ID: %d, Name: %s, Price: %.2f, Quantity: %d\n",
                    user.items[j].id, user.items[j].name, user.items[j].price, user.items[j].quantity);
        }

        fprintf(file, "\n");
    }

    fclose(file);
}

void UpdateS(const char *filename, const User *user) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file %s\n", filename);
        return;
    }

    FILE *tempFile = fopen("tempStoreData.txt", "w");
    if (tempFile == NULL) {
        fprintf(stderr, "Error\n");
        fclose(file);
        return;
    }

    int id, quantity;
    float price;
    while (fscanf(file, "%d %*s %f %d", &id, &price, &quantity) == 3) {
        for (int i = 0; i < user->count; ++i) {
            if (user->items[i].id == id) {
                quantity -= user->items[i].quantity;
            }
        }

        if (quantity >= 0) {
            fprintf(tempFile, "%d %s %.2f %d\n", id, "ItemName", price, quantity);
        } else {
            fprintf(stderr, "Error: Negative quantity %d\n", id);
        }
    }

    fclose(file);
    fclose(tempFile);

    remove(filename);
    rename("tempStoreData.txt", filename);
}

void AvailItems(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error %s\n", filename);
        return;
    }

    printf("Available Items:\n");

    int id, quantity;
    while (fscanf(file, "%d %*s %*f %d", &id, &quantity) == 2) {
        printf("Item ID: %d, Quantity: %d\n", id, quantity);
    }

    fclose(file);
}

float Price(const char *filename, int itemId) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error %s\n", filename);
        return -1.0; 
    }

    int id;
    float price;
    while (fscanf(file, "%d %*s %f %*d", &id, &price) == 2) {
        if (id == itemId) {
            fclose(file);
            return price;
        }
    }

    fclose(file);
    return -1.0; 
}
void addItemsToStore(const char *filename, int numItems) {
    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        fprintf(stderr, "Error %s\n", filename);
        return;
    }

    Item newItem;
    for (int i = 0; i < numItems; ++i) {
        printf("Enter details for Item %d (id name price quantity): ", i + 1);
        scanf("%d %s %f %d", &newItem.id, newItem.name, &newItem.price, &newItem.quantity);
        fprintf(file, "%d %s %.2f %d\n", newItem.id, newItem.name, newItem.price, newItem.quantity);
    }

    fclose(file);
}


