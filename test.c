
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.c"

#define Users 20
#define Items 10
#define NameLen 50
#define USER_FILE "userData.txt"
#define STORE_FILE "storeData.txt"
int main() {
    SQUEUE queue;
    int choice;
    User user;

    DEF(&queue);

    load(USER_FILE, &queue);

    do {
        printf("\nMenu:\n");
        printf("1. Enqueue User\n");
        printf("2. Dequeue User\n");
        printf("3. Print Queue\n");
        printf("4. Add Items\n");
        printf("5. Show Available Items\n");
        printf("6. Show Item Price\n");
        printf("7. Quit\n");

        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter User (id name): ");
                scanf("%d %s", &user.id, user.name);

                printf("Number of items : ");
                scanf("%d", &user.count);

                for (int i = 0; i < user.count; ++i) {
                    printf("Enter Item %d (id name price quantity): ", i + 1);
                    scanf("%d %s %f %d", &user.items[i].id, user.items[i].name,
                          &user.items[i].price, &user.items[i].quantity);
                }

                EN(&queue, user);
                save(USER_FILE, user);
                UpdateS(STORE_FILE, &user);
                break;

            case 2:
                user = DE(&queue);
                printf("Dequeued User: ");
                UPrint(user);
                break;

            case 3:
                printf("Queue Users:\n");
                for (int i = 0; i < queue.size; ++i) {
                    UPrint(queue.users[(queue.front + i) % Users]);
                }
                break;

            case 4:
                printf("Enter the number of items to add to the store: ");
                int numItems;
                scanf("%d", &numItems);
                addItemsToStore(STORE_FILE, numItems);
                printf("Items added to the store.\n");
                break;

            case 5:
                AvailItems(STORE_FILE);
                break;

            case 6:
                printf("Enter Item ID: ");
                scanf("%d", &user.items[0].id);
                float price = Price(STORE_FILE, user.items[0].id);
                if (price > 0) {
                    printf("Price of Item ID %d: %.2f\n", user.items[0].id, price);
                } else if (price == 0) {
                    printf("Item ID %d not found\n", user.items[0].id);
                } else {
                    printf("Error\n");
                }
                break;

            case 7:
                printf("Exiting \n");
                break;

            default:
                printf("Invalid choice try again\n");
        }
    } while (choice != 7);

    return 0;
}
