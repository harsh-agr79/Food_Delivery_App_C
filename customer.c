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
#define DATABASE_CART "cart.txt"

int checkCart(int id, char *customer)
{
    FILE *file = fopen(DATABASE_CART, "r");
    if (file == NULL)
    {
        return 0;
    }
    char lines[500][1000];
    char line[1000];
    char username[1000];
    char resusername[1000];
    char db_username[1000];
    char db_resname[1000];
    char db_resusername[1000];
    char db_food[1000];
    char db_category[1000];
    char db_type[1000];
    char dataset[5000];
    char editline[1000];
    int itemid;
    int qty;

    while (fgets(line, sizeof(line), file))
    {
        sscanf(line, "%[^,],%[^,],%[^,],%d,%[^,],%[^,],%[^,],%d", username, db_resname, db_resusername, &itemid, db_food, db_category, db_type, &qty);
        if (strcmp(username, customer) == 0 && id == itemid)
        {
            return qty;
        }
    }

    return 0;
}

void getMenuCustomer(char *data)
{
    char customerUN[1000];
    char restaurantUN[1000];
    FILE *file = fopen(DATABASE_MENU, "r");
    if (file == NULL)
    {
        perror("Error opening menu file");
        return;
    }

    char line[5000];
    int isFirstItem = 1; // Flag to track the first item in JSON array
    sscanf(data, "%[^,],%[^,]", restaurantUN, customerUN);
    // printf("%s %s", restaurantUN, customerUN);
    printf("[");
    while (fgets(line, sizeof(line), file))
    {
        // Parse line into variables
        int id;
        char restaurant[MAX_RESTAURANT_NAME_LEN];
        char user[MAX_RESTAURANT_NAME_LEN];
        char item[MAX_ITEM_NAME_LEN];
        char category[MAX_CATEGORY_LEN];
        char type[MAX_CATEGORY_LEN];
        float price;

        if (sscanf(line, "%d,%[^,],%[^,],%[^,],%[^,],%[^,],%f", &id, restaurant, user, item, category, type, &price) == 7)
        {
            if (strcmp(user, restaurantUN) == 0)
            {
                if (!isFirstItem)
                {
                    printf(","); // Add comma for subsequent items
                }
                printf("  {");
                printf("    \"id\": \"%d\",", id);
                printf("    \"food\": \"%s\",", item);
                printf("    \"restaurantName\": \"%s\",", restaurant);
                printf("    \"username\": \"%s\",", user);
                printf("    \"category\": \"%s\",", category);
                printf("    \"type\": \"%s\",", type);
                printf("    \"quantity\": \"%d\",", checkCart(id, customerUN));
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

int setUserCart(char *data)
{
    FILE *file = fopen(DATABASE_CART, "r");
    if (file == NULL)
    {
        // printf("Error opening file for reading.\n");
        return 0;
    }
    // // Read lines from the file into an array
    char lines[500][1000];
    char line[1000];
    char username[1000];
    char resusername[1000];
    char db_username[1000];
    char db_resname[1000];
    char db_resusername[1000];
    char dataset[5000];
    char editline[1000];
    char id[10];
    int count = 0;
    char *values[MAX_LINES];
    int value_count = 0;
    int rescheck = 0;

    sscanf(data, "%[^,],%[^,],%[^\n]s", username, resusername, dataset);

    char *token = strtok(dataset, "|");

    while (token != NULL)
    {
        values[value_count] = token;
        value_count++;
        token = strtok(NULL, "|");
    }

    while (fgets(line, sizeof(line), file))
    {
        sscanf(line, "%[^,],%[^,],%[^,]", db_username, db_resname, db_resusername);
        if (strcmp(username, db_username))
        {
            sscanf(line, "%[^\n]s", editline);
            strcpy(lines[count], editline);
            count++;
        }
        else
        {
            if (strcmp(db_resusername, resusername))
            {
                rescheck = 1;
            }
        }
    }

    fclose(file);
    file = fopen(DATABASE_CART, "w");
    if (file == NULL)
    {
        printf("Error opening file for reading.\n");
        return 0;
    }
    for (int i = 0; i < count; i++)
    {
        fprintf(file, "%s\n", lines[i]);
    }
    for (int i = 0; i < value_count; i++)
    {
        fprintf(file, "%s\n", values[i]);
    }

    fclose(file);
    if (rescheck)
    {
        printf("Only One Restaurant Can be Selected, Previous Cart Cleared!");
    }
    else
    {
        printf("Food Added To cart!");
    }
    fflush(stdout);
    return 1;
}

void getRestaurants()
{
    FILE *file = fopen(DATABASE_RESTAURANT, "r");
    if (file == NULL)
    {
        perror("Error opening menu file");
        return;
    }

    char line[5000];
    int isFirstItem = 1; // Flag to track the first item in JSON array

    printf("[");
    while (fgets(line, sizeof(line), file))
    {
        // Parse line into variables
        char restaurantname[MAX_RESTAURANT_NAME_LEN];
        char user[MAX_RESTAURANT_NAME_LEN];
        char username[MAX_ITEM_NAME_LEN];
        char password[MAX_ITEM_NAME_LEN];
        char contact[MAX_CATEGORY_LEN];
        char address[MAX_CATEGORY_LEN];
        char pincode[MAX_CATEGORY_LEN];

        if (sscanf(line, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^\n]s", restaurantname, user, username, password, contact, address, pincode) == 7)
        {
            // if (strcmp(user, username) == 0) {
            if (!isFirstItem)
            {
                printf(","); // Add comma for subsequent items
            }
            printf("{");
            printf("\"username\": \"%s\",", username);
            printf("\"restaurantName\": \"%s\",", restaurantname);
            printf("\"contact\": \"%s\",", contact);
            printf("\"address\": \"%s, %s\"", address, pincode);
            printf("}");

            isFirstItem = 0; // Update flag after printing the first item
        }
        // }
    }
    printf("]");
    fflush(stdout);
    fclose(file);
}

int getCustomerLocation(char *data)
{
    FILE *file = fopen(DATABASE_LOCATION, "r");
    if (file == NULL)
    {
        // printf("Error opening file for reading.\n");
        return 0;
    }
    // // Read lines from the file into an array
    char line[1000];
    char dataline[1000];
    char editline[1000];
    char id[10];

    while (fgets(line, sizeof(line), file))
    {
        sscanf(line, "%[^,]", id);
        if (strcmp(data, id) == 0)
        {
            sscanf(line, "%[^\n]s", editline);
            strcpy(dataline, editline);
        }
    }

    fclose(file);

    printf("%s", dataline);
    fflush(stdout);

    return 0;
}

int setCustomerLocation(char *data)
{
    FILE *file = fopen(DATABASE_LOCATION, "r");
    if (file == NULL)
    {
        // printf("Error opening file for reading.\n");
        return 0;
    }
    char editid[10];
    char dataset[1000];
    char loc[10];

    sscanf(data, "%[^,],%[^,],%[^,]", editid, dataset, loc);
    // printf("%s %s", editid, dataset);

    // // Read lines from the file into an array
    char lines[500][1000];
    char line[1000];
    char dbid[100];
    char dbresname[100];
    char dbresusername[100];
    char editline[1000];
    char id[10];
    int count = 0;
    int row = 0;
    int exist = 0;

    while (fgets(line, sizeof(line), file))
    {
        sscanf(line, "%[^,]", id);
        if (strcmp(editid, id) == 0)
        {
            strcpy(lines[count], data);
            exist = 1;
            count++;
        }
        else
        {
            sscanf(line, "%[^\n]s", editline);
            strcpy(lines[count], editline);
            count++;
        }
    }
    if (count == 0 && exist == 0)
    {
        strcpy(lines[0], data);
        count++;
    }
    else if (count != 0 && exist == 0)
    {
        strcpy(lines[count], data);
        count++;
    }
    fclose(file);

    file = fopen(DATABASE_LOCATION, "w");
    if (file == NULL)
    {
        printf("Error opening file for reading.\n");
        return 0;
    }

    for (int i = 0; i < count; i++)
    {
        fprintf(file, "%s\n", lines[i]);
    }

    fclose(file);
    printf("%s", loc);
    fflush(stdout);
    return 1;
}
