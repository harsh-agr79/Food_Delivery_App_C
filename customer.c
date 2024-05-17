#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_RESTAURANT_NAME_LEN 50
#define MAX_ITEM_NAME_LEN 50
#define MAX_CATEGORY_LEN 20
#define MAX_ITEMS 100
#define MAX_LINES 5000
#define MAX_LINE_LENGTH 5000

#define DATABASE_USER "users.txt"
#define DATABASE_RESTAURANT "restaurants.txt"
#define DATABASE_DELIVERYMAN "deliveryman.txt"
#define DATABASE_MENU "menu.txt"
#define DATABASE_LOCATION "location.txt"

void getRestaurants(){
     FILE *file = fopen(DATABASE_RESTAURANT, "r");
    if (file == NULL) {
        perror("Error opening menu file");
        return;
    }

    char line[5000];
    int isFirstItem = 1; // Flag to track the first item in JSON array

    printf("[");
    while (fgets(line, sizeof(line), file)) {
        // Parse line into variables
        char restaurantname[MAX_RESTAURANT_NAME_LEN];
        char user[MAX_RESTAURANT_NAME_LEN];
        char username[MAX_ITEM_NAME_LEN];
        char password[MAX_ITEM_NAME_LEN];
        char contact[MAX_CATEGORY_LEN];
        char address[MAX_CATEGORY_LEN];
        char pincode[MAX_CATEGORY_LEN];

        if (sscanf(line, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^\n]s",restaurantname, user, username, password, contact, address, pincode) == 7) {
            // if (strcmp(user, username) == 0) {
                if (!isFirstItem) {
                    printf(","); // Add comma for subsequent items
                }
                printf("{");
                printf("\"username\": \"%s\",", username);
                printf("\"restaurantName\": \"%s\",", restaurantname);
                printf("\"contact\": \"%s\",", contact);
                printf("\"address\": \"%s, %s\"", address,pincode);
                printf("}");

                isFirstItem = 0; // Update flag after printing the first item
            }
        // }
    }
    printf("]");
    fflush(stdout);
    fclose(file);
}