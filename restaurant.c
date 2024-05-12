#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_RESTAURANT_NAME_LEN 50
#define MAX_ITEM_NAME_LEN 50
#define MAX_CATEGORY_LEN 20
#define MAX_ITEMS 100

#define DATABASE_USER "users.txt"
#define DATABASE_RESTAURANT "restaurants.txt"
#define DATABASE_DELIVERYMAN "deliveryman.txt"
#define DATABASE_MENU "menu.txt"

typedef struct
{
    char restaurant[MAX_RESTAURANT_NAME_LEN];
    char username[MAX_RESTAURANT_NAME_LEN];
    char item[MAX_ITEM_NAME_LEN];
    char category[MAX_CATEGORY_LEN];
    float price;
} MenuItem;

void getMenu(char *username)
{
    FILE *file = fopen(DATABASE_MENU, "r");
    if (file == NULL) {
        perror("Error opening menu file");
        return;
    }

    char line[5000];
    int isFirstItem = 1; // Flag to track the first item in JSON array

    printf("[");
    while (fgets(line, sizeof(line), file)) {
        // Parse line into variables
        int id;
        char restaurant[MAX_RESTAURANT_NAME_LEN];
        char user[MAX_RESTAURANT_NAME_LEN];
        char item[MAX_ITEM_NAME_LEN];
        char category[MAX_CATEGORY_LEN];
        float price;

        if (sscanf(line, "%d,%[^,],%[^,],%[^,],%[^,],%f",&id,restaurant, user, item, category, &price) == 6) {
            if (strcmp(user, username) == 0) {
                if (!isFirstItem) {
                    printf(","); // Add comma for subsequent items
                }
                printf("  {");
                printf("    \"id\": \"%d\",", id);
                printf("    \"food\": \"%s\",", item);
                printf("    \"category\": \"%s\",", category);
                printf("    \"price\": %.2f", price);
                printf("  }");

                isFirstItem = 0; // Update flag after printing the first item
            }
        }
    }
    printf("]");
    fflush(stdout);
    fclose(file);
}

char* findResname(char *username) {
    FILE *file = fopen(DATABASE_RESTAURANT, "r");
    if (!file) {
        printf("Error opening database file.\n");
        return NULL;
    }

    char line[500];
    char db_username[MAX_ITEM_NAME_LEN];
    char db_resname[MAX_ITEM_NAME_LEN];

    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%[^,],%[^,]", db_resname, db_username);
        if (strcmp(username, db_username) == 0) {
            fclose(file);
            char *resname = strdup(db_resname);
            if (!resname) {
                printf("Memory allocation failed.\n");
                return NULL;
            }
            return resname;
        }
    }
    fclose(file);
    return NULL; // Return NULL if username not found
}

int findID(){
    FILE *file = fopen(DATABASE_MENU, "r");
    if (!file) {
        printf("Error opening database file.\n");
    }
    int id;
    char line[1000];
    int max = 0;
    while (fgets(line, sizeof(line), file)) {
         sscanf(line,"%d",&id);
        if (id > max) {
           max = id;
        }
    }
    max = max + 1;
    return max;
}

int addMenuItem(char *data){
     FILE *file = fopen(DATABASE_MENU, "a");
    if (!file)
    {
        printf("Error opening database file.\n");
        return 0;
    }

    // fprintf(file, "%s,%s,%s,%s,%s,%s\n", name, username, password, phone, address, pincode);
    char username[MAX_RESTAURANT_NAME_LEN];
    char dataset[5000];
    int id;
    sscanf(data,"%[^,],%[^,]", username,dataset);
    char* resname = findResname(username);
    id = findID();
    if(id > 0){
        fprintf(file, "%d,%s,%s\n",id,resname,data);
        fclose(file);
        printf("1");
        fflush(stdout);
        return 1;
    }
    else{
        fclose(file);
        printf("0");
        fflush(stdout);
        return 0;
    }
   
}