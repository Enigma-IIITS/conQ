#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define MAX_USERS 20
#define MAX_ITEMS 10
#define MAX_NAME_LENGTH 50
#define USER_FILE "userData.txt"
#define STORE_FILE "storeData.txt"

typedef struct {
    int id;
    float price;
    char name[MAX_NAME_LENGTH];
    int quantity;
} Item;

typedef struct {
    int id;
    char name[MAX_NAME_LENGTH];
    Item items[MAX_ITEMS];
    int itemCount;
} User;

typedef struct {
    User users[MAX_USERS];
    int size;
    int front;
    int rear;
     pthread_mutex_t *mutex;
} SQUEUE;

void DEF(SQUEUE *queue) {
     pthread_mutex_lock(queue->mutex);

    queue->size = 0;
    queue->front = 0;
    queue->rear = -1;
    pthread_mutex_unlock(queue->mutex);
}

void enqueue(SQUEUE *queue, User user) {
      pthread_mutex_lock(queue->mutex);
    if (queue->size >= MAX_USERS) {
        fprintf(stderr, "Queue is full\n");
        return;
    }

    queue->rear = (queue->rear + 1) % MAX_USERS;
    queue->users[queue->rear] = user;
    queue->size++;
    pthread_mutex_unlock(queue->mutex);
}

User dequeue(SQUEUE *queue) {
     pthread_mutex_lock(queue->mutex);
    User user = {0};  // Default values for error condition

    if (queue->size <= 0) {
        fprintf(stderr, "Queue is empty\n");
        return user;
    }

    user = queue->users[queue->front];
    queue->front = (queue->front + 1) % MAX_USERS;
    queue->size--;

pthread_mutex_unlock(queue->mutex); 
    return user;    
    
}

void printUser(User user) {
    printf("User ID: %d, Name: %s\n", user.id, user.name);
    printf("Items Purchased:\n");
    for (int i = 0; i < user.itemCount; ++i) {
        printf("  Item ID: %d, Name: %s, Price: %.2f, Quantity: %d\n",
               user.items[i].id, user.items[i].name, user.items[i].price, user.items[i].quantity);
    }
}

void saveToFile(const char *filename, User user) {
    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        fprintf(stderr, "Error opening file %s\n", filename);
        return;
    }

    fprintf(file, "%d %s %d", user.id, user.name, user.itemCount);

    for (int i = 0; i < user.itemCount; ++i) {
        fprintf(file, " %d %s %.2f %d", user.items[i].id, user.items[i].name,
                user.items[i].price, user.items[i].quantity);
    }

    fprintf(file, "\n");

    fclose(file);
}

void loadFromFile(const char *filename, SQUEUE *queue) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file %s\n", filename);
        return;
    }

    User user;
    while (fscanf(file, "%d %s %d", &user.id, user.name, &user.itemCount) == 3) {
        for (int i = 0; i < user.itemCount; ++i) {
            fscanf(file, " %d %s %f %d", &user.items[i].id, user.items[i].name,
                   &user.items[i].price, &user.items[i].quantity);
        }
        enqueue(queue, user);
    }

    fclose(file);
}

void saveStoreToFile(const char *filename, const SQUEUE *queue) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        fprintf(stderr, "Error opening file %s\n", filename);
        return;
    }

    for (int i = 0; i < queue->size; ++i) {
        User user = queue->users[(queue->front + i) % MAX_USERS];

        fprintf(file, "User ID: %d, Name: %s\n", user.id, user.name);
        fprintf(file, "Items Purchased:\n");

        for (int j = 0; j < user.itemCount; ++j) {
            fprintf(file, "  Item ID: %d, Name: %s, Price: %.2f, Quantity: %d\n",
                    user.items[j].id, user.items[j].name, user.items[j].price, user.items[j].quantity);
        }

        fprintf(file, "\n");
    }

    fclose(file);
}

void updateStoreFile(const char *filename, const User *user) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file %s\n", filename);
        return;
    }

    FILE *tempFile = fopen("tempStoreData.txt", "w");
    if (tempFile == NULL) {
        fprintf(stderr, "Error creating temp file\n");
        fclose(file);
        return;
    }

    int id, quantity;
    while (fscanf(file, "%d %*s %*f %d", &id, &quantity) == 2) {
        for (int i = 0; i < user->itemCount; ++i) {
            if (user->items[i].id == id) {
                fprintf(tempFile, "%d %s %.2f %d\n", id, user->items[i].name,
                        user->items[i].price, quantity - user->items[i].quantity);
            }
        }
    }

    fclose(file);
    fclose(tempFile);

    remove(filename);
    rename("tempStoreData.txt", filename);
}

void showAvailableItems(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file %s\n", filename);
        return;
    }

    printf("Available Items:\n");

    int id, quantity;
    while (fscanf(file, "%d %*s %*f %d", &id, &quantity) == 2) {
        printf("Item ID: %d, Quantity: %d\n", id, quantity);
    }

    fclose(file);
}

float getItemPrice(const char *filename, int itemId) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file %s\n", filename);
        return -1.0; // Indicates error
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
    return -1.0; // Indicates item not found
}

void buyItemsAtDiscount(const char *filename, const User *user) {
    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        fprintf(stderr, "Error opening file %s\n", filename);
        return;
    }

    for (int i = 0; i < user->itemCount; ++i) {
        fprintf(file, "%d %s %.2f %d\n", user->items[i].id, user->items[i].name,
                user->items[i].price * 0.8, user->items[i].quantity);
    }

    fclose(file);
}

float calculateProfit(const char *storeFile, const char *discountFile) {
    FILE *storeFilePtr = fopen(storeFile, "r");
    FILE *discountFilePtr = fopen(discountFile, "r");

    if (storeFilePtr == NULL || discountFilePtr == NULL) {
        fprintf(stderr, "Error opening files\n");
        return -1.0; // Indicates error
    }

    int storeId, discountId, storeQuantity, discountQuantity;
    float storePrice, discountPrice;
    float profit = 0.0;

    while (fscanf(storeFilePtr, "%d %*s %f %d", &storeId, &storePrice, &storeQuantity) == 3 &&
           fscanf(discountFilePtr, "%d %*s %f %d", &discountId, &discountPrice, &discountQuantity) == 3) {
        profit += (storePrice - discountPrice) * discountQuantity;
    }

    fclose(storeFilePtr);
    fclose(discountFilePtr);

    return profit;
}

