#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#define MAX_RESTAURANT_NAME_LEN 500
#define MAX_ITEM_NAME_LEN 100
#define MAX_CATEGORY_LEN 200
#define MAX_ITEMS 1000
#define MAX_LINES 5000
#define MAX_LINE_LENGTH 5000

#define DATABASE_USER "users.txt"
#define DATABASE_RESTAURANT "restaurants.txt"
#define DATABASE_DELIVERYMAN "deliveryman.txt"
#define DATABASE_MENU "menu.txt"
#define DATABASE_LOCATION "location.txt"
#define DATABASE_CART "cart.txt"
#define DATABASE_ORDER "order.txt"
#define DATABASE_DELIVERY_ALLOCATION "delivery_allocation.txt"
#define DATABASE_FEEDBACK "feedbacks.txt"

char *get_current_timestamp()
{
    time_t t;
    struct tm *tmp;
    char *format = "%Y-%m-%d %H:%M:%S"; // Desired format: YYYY-MM-DD HH:MM:SS
    char buf[64];

    // Get the current time
    t = time(NULL);

    // Convert the time to a struct tm form
    tmp = localtime(&t);
    if (tmp == NULL)
    {
        perror("localtime");
        return NULL;
    }

    // Format the time as a string
    if (strftime(buf, sizeof(buf), format, tmp) == 0)
    {
        fprintf(stderr, "strftime returned 0");
        return NULL;
    }

    // Allocate memory for the timestamp string
    char *timestamp = malloc(strlen(buf) + 1);
    if (timestamp == NULL)
    {
        perror("malloc");
        return NULL;
    }

    // Copy the formatted time into the allocated memory
    strcpy(timestamp, buf);

    return timestamp;
}

typedef struct
{
    char restaurantName[MAX_RESTAURANT_NAME_LEN];
    char username[MAX_ITEM_NAME_LEN];
    char contact[MAX_CATEGORY_LEN];
    char address[MAX_CATEGORY_LEN];
    char pincode[MAX_CATEGORY_LEN];
    int distance;
    char path[500]; // Assuming getPath() returns a string less than 500 chars
    float rating;
} Restaurant;

int compareByRatingAndDistance(const void *a, const void *b)
{
    Restaurant *entryA = (Restaurant *)a;
    Restaurant *entryB = (Restaurant *)b;

    if (entryA->distance != entryB->distance)
        return entryA->distance - entryB->distance;

    if (entryA->rating < entryB->rating)
        return 1;
    if (entryA->rating > entryB->rating)
        return -1;

    return 0;
}

int getDistance(char *customer, char *restaurant)
{
    FILE *file = fopen(DATABASE_LOCATION, "r");
    if (file == NULL)
    {
        // return 0;
    }
    char line[1000];
    char username[1000];
    char type[1000];
    int node;
    int node1 = 0;
    int node2 = 0;
    int result;
    while (fgets(line, sizeof(line), file))
    {
        sscanf(line, "%[^,],%[^,],%d", username, type, &node);
        if (strcmp(username, customer) == 0)
        {
            node1 = node;
        }
        else if (strcmp(username, restaurant) == 0)
        {
            node2 = node;
        }
    }

    result = callDKS2(node1, node2);
    // printf("%d,%d\n", node1, node2);

    return result;
}

char *getPath(char *customer, char *restaurant)
{
    FILE *file = fopen(DATABASE_LOCATION, "r");
    if (file == NULL)
    {
        // return 0;
    }
    char line[1000];
    char username[1000];
    char type[1000];
    int node;
    int node1 = 0;
    int node2 = 0;
    // int result;
    while (fgets(line, sizeof(line), file))
    {
        sscanf(line, "%[^,],%[^,],%d", username, type, &node);
        if (strcmp(username, customer) == 0)
        {
            node1 = node;
        }
        else if (strcmp(username, restaurant) == 0)
        {
            node2 = node;
        }
    }
    char *res = callDKS(node1, node2);
    char *result = strdup(res);
    return result;
}

int findOrderID()
{
    FILE *file = fopen(DATABASE_ORDER, "r");
    if (!file)
    {
        printf("Error opening database file.\n");
    }
    int id;
    char line[2000];
    int max = 0;
    while (fgets(line, sizeof(line), file))
    {
        sscanf(line, "%d", &id);
        if (id > max)
        {
            max = id;
        }
    }
    max = max + 1;
    return max;
}

void confirmCart(char *data)
{
    FILE *file = fopen(DATABASE_CART, "r");
    if (file == NULL)
    {
        printf("couldnt access the file\n");
    }
    char lines[500][1000];
    char cartlines[500][1000];
    char line[1000];
    char username[1000];
    char editline[1000];
    int count = 0;
    int clcnt = 0;
    char *timestamp = get_current_timestamp();
    int itemidst[100];
    int iitcnt = 0;

    while (fgets(line, sizeof(line), file))
    {
        char username[1000];
        char resusername[1000];
        char db_username[1000];
        char db_resname[1000];
        char db_resusername[1000];
        char db_food[1000];
        char db_category[1000];
        char db_type[1000];
        int itemid;
        int qty;
        sscanf(line, "%[^,],%[^,],%[^,],%d,%[^,],%[^,],%[^,],%d", username, db_resname, db_resusername, &itemid, db_food, db_category, db_type, &qty);
        
        if (strcmp(username, data) == 0)
        {
            sscanf(line, "%[^\n]s", editline);
            strcpy(lines[count], editline);
            count++;
            itemidst[iitcnt] = itemid;
            iitcnt++;
        }
        else
        {
            sscanf(line, "%[^\n]s", editline);
            strcpy(cartlines[clcnt], editline);
            clcnt++;
        }
    }
    fclose(file);
    file = fopen(DATABASE_USER, "r");
    if (file == NULL)
    {
        printf("couldnt access the file\n");
    }
    char db_username[1000];
    char db_name[1000];
    char db_pass[1000];
    char db_contact[1000];
    char db_address[1000];
    char db_pincode[1000];
    while (fgets(line, sizeof(lines), file))
    {
        sscanf(line, "%[^,],%[^,]", db_name, db_username);
        if (strcmp(db_username, data) == 0)
        {
            sscanf(line, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^\n]s", db_name, db_username, db_pass, db_contact, db_address, db_pincode);
            break;
        }
    }

    // writing in order db
    file = fopen(DATABASE_ORDER, "a");
    if (file == NULL)
    {
        printf("Error opening file for reading.\n");
    }
    int orderid = findOrderID();
    for (int i = 0; i < count; i++)
    {
        fprintf(file, "%d,%s,%s,%s,%s,%s,%s,pending\n", orderid, db_name, lines[i], db_contact, db_address, db_pincode, timestamp);
    }
    fclose(file);

    // emptying the cart
    file = fopen(DATABASE_CART, "w");
    if (file == NULL)
    {
        printf("Error opening file for reading.\n");
    }
    for (int i = 0; i < clcnt; i++)
    {
        fprintf(file, "%s\n", cartlines[i]);
    }
    fclose(file);
    file = fopen("transaction_final.txt", "a");
    for(int i = 0; i < iitcnt; i++){
        if(i == iitcnt - 1){
            fprintf(file, "%d\n", itemidst[i]);
        }
        else{
            fprintf(file, "%d,", itemidst[i]);
        }
    }
    fclose(file);
    printf("Your Order has been sent to the Restaurant");
    fflush(stdout);
}

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

int getUserCartInfo(char *data)
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
    
    if (rescheck)
    {
        printf("Restaurant Change");
    }
    else
    {
        printf("No Change");
    }
    fflush(stdout);
    return 1;
}


int allowedToOrder(char *user)
{
    FILE *file = fopen(DATABASE_ORDER, "r");
    if (file == NULL)
    {
        perror("Error opening menu file");
    }
    char line[5000];
    int res = 1;
    char orderids[300][100];
    int ocnt = 0;
    while (fgets(line, sizeof(line), file))
    {
        // Parse line into variables
        char oid[100];
        char user_name[MAX_RESTAURANT_NAME_LEN];
        char username[MAX_RESTAURANT_NAME_LEN];
        char restaurant_name[MAX_ITEM_NAME_LEN];
        char restaurant_username[MAX_CATEGORY_LEN];
        char itemid[MAX_CATEGORY_LEN];
        char itemname[MAX_CATEGORY_LEN];
        char item_category[MAX_CATEGORY_LEN];
        char item_type[MAX_CATEGORY_LEN];
        int qty;
        int price;
        char contact[MAX_CATEGORY_LEN];
        char address[MAX_CATEGORY_LEN];
        char pincode[MAX_CATEGORY_LEN];
        char time[MAX_LINE_LENGTH];
        char status[MAX_LINE_LENGTH];

        if (sscanf(line, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%d,%d,%[^,],%[^,],%[^,],%[^,],%[^\n]", oid, user_name, username, restaurant_name, restaurant_username, itemid, itemname, item_category, item_type, &qty, &price, contact, address, pincode, time, status) == 16)
        {
            if (strcmp(status, "pending") == 0 && strcmp(username, user) == 0)
            {
                res = 0;
            }
            if (strcmp(status, "pending") != 0 && strcmp(username, user) == 0)
            {
                strcpy(orderids[ocnt], oid);
                // printf("%s\n", orderids[ocnt]);
                ocnt++;
            }
        }
    }
    fclose(file);
    file = fopen(DATABASE_DELIVERY_ALLOCATION, "r");
    while (fgets(line, sizeof(line), file))
    {
        char oid[100];
        char user[100];
        char status[100];
        sscanf(line, "%[^,],%[^,],%[^\n]", oid, user, status);
        for (int i = 0; i < ocnt; i++)
        {
            if (strcmp(user, "Not Allocated") == 0 && strcmp(oid, orderids[i]) == 0)
            {
                res = 0;
            }
            if (strcmp(status, "Picked up. On the Way") == 0 && strcmp(oid, orderids[i]) == 0)
            {
                res = 0;
            }
            if (strcmp(status, "Delivered") == 0 && strcmp(oid, orderids[i]) == 0)
            {

            }
        }
    }
   return res;
}

void getRestaurants(char *customer)
{
    FILE *file = fopen(DATABASE_RESTAURANT, "r");

    if (file == NULL)
    {
        perror("Error opening menu file");
        return;
    }

    Restaurant restaurants[1000]; // Assuming we have at most 1000 restaurants
    int count = 0;

    char line[5000];
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
        float rating;

        if (sscanf(line, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%f", restaurantname, user, username, password, contact, address, pincode,&rating) == 8)
        {
            strcpy(restaurants[count].restaurantName, restaurantname);
            strcpy(restaurants[count].username, username);
            strcpy(restaurants[count].contact, contact);
            strcpy(restaurants[count].address, address);
            strcpy(restaurants[count].pincode, pincode);
            restaurants[count].distance = getDistance(customer, username);
            strcpy(restaurants[count].path, getPath(customer, username));
            restaurants[count].rating = rating;
            count++;
        }
    }
    fclose(file);

    // Sort the array of structs by distance
    qsort(restaurants, count, sizeof(Restaurant), compareByRatingAndDistance);

    // Print the sorted array in JSON format
    printf("[");
    if (allowedToOrder(customer))
    {
        for (int i = 0; i < count; i++)
        {
            if (i > 0)
            {
                printf(",");
            }
            printf("{");
            printf("\"username\": \"%s\",", restaurants[i].username);
            printf("\"restaurantName\": \"%s\",", restaurants[i].restaurantName);
            printf("\"contact\": \"%s\",", restaurants[i].contact);
            printf("\"distance\": \"%d\",", restaurants[i].distance);
            printf("\"path\": \"%s\",", restaurants[i].path);
            printf("\"rating\": %f,", restaurants[i].rating);
            printf("\"address\": \"%s, %s\"", restaurants[i].address, restaurants[i].pincode);
            printf("}");
        }
    }

    printf("]");
    fflush(stdout);
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
void getCart(char *data)
{
    FILE *file = fopen(DATABASE_CART, "r");
    if (file == NULL)
    {
        // printf("Error opening file for reading.\n");
        // return 0;
    }
    // // Read lines from the file into an array
    char line[5000];
    int isFirstItem = 1;
    printf("[");
    while (fgets(line, sizeof(line), file))
    {
        // Parse line into variables
        char cusUN[255];
        char resname[255];
        char resUN[255];
        int id;
        char food[255];
        char category[255];
        char type[255];
        int quantity;
        int price;

        if (sscanf(line, "%[^,],%[^,],%[^,],%d,%[^,],%[^,],%[^,],%d,%d", cusUN, resname, resUN, &id, food, category, type, &quantity, &price) == 9)
        {
            if (strcmp(cusUN, data) == 0)
            {
                if (!isFirstItem)
                {
                    printf(","); // Add comma for subsequent items
                }
                printf("  {");
                printf("    \"customerUsername\": \"%s\",", cusUN);
                printf("    \"resname\": \"%s\",", resname);
                printf("    \"restaurantUsername\": \"%s\",", resUN);
                printf("    \"id\": %d,", id);
                printf("    \"food\": \"%s\",", food);
                printf("    \"category\": \"%s\",", category);
                printf("    \"type\": \"%s\",", type);
                printf("    \"quantity\": %d,", quantity);
                printf("    \"price\": %d", price);
                printf("  }");

                isFirstItem = 0; // Update flag after printing the first item
            }
        }
    }
    printf("]");
    fflush(stdout);
    fclose(file);
}

void getPathCart(char *data)
{
    char customer[500];
    char restaurant[500];

    sscanf(data, "%[^,],%[^,]", customer, restaurant);

    printf("%d|%s", getDistance(customer, restaurant), getPath(customer, restaurant));
    fflush(stdout);
}

char *getAbsoluteStatusCustomer(char *orderid)
{
    FILE *file = fopen(DATABASE_ORDER, "r");
    if (file == NULL)
    {
        perror("Error opening menu file");
    }
    char line[5000];
    char res[100];
    while (fgets(line, sizeof(line), file))
    {
        // Parse line into variables
        char oid[100];
        char user_name[MAX_RESTAURANT_NAME_LEN];
        char username[MAX_RESTAURANT_NAME_LEN];
        char restaurant_name[MAX_ITEM_NAME_LEN];
        char restaurant_username[MAX_CATEGORY_LEN];
        char itemid[MAX_CATEGORY_LEN];
        char itemname[MAX_CATEGORY_LEN];
        char item_category[MAX_CATEGORY_LEN];
        char item_type[MAX_CATEGORY_LEN];
        int qty;
        int price;
        char contact[MAX_CATEGORY_LEN];
        char address[MAX_CATEGORY_LEN];
        char pincode[MAX_CATEGORY_LEN];
        char time[MAX_LINE_LENGTH];
        char status[MAX_LINE_LENGTH];

        if (sscanf(line, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%d,%d,%[^,],%[^,],%[^,],%[^,],%[^\n]", oid, user_name, username, restaurant_name, restaurant_username, itemid, itemname, item_category, item_type, &qty, &price, contact, address, pincode, time, status) == 16)
        {
            if (strcmp(status, "pending") == 0 && strcmp(oid, orderid) == 0)
            {
                strcpy(res, "pending");
            }
        }
    }
    fclose(file);
    file = fopen(DATABASE_DELIVERY_ALLOCATION, "r");
    while (fgets(line, sizeof(line), file))
    {
        char oid[100];
        char user[100];
        char status[100];
        sscanf(line, "%[^,],%[^,],%[^\n]", oid, user, status);
        if (strcmp(user, "Not Allocated") == 0 && strcmp(oid, orderid) == 0)
        {
            strcpy(res, "Accepted, preparing");
        }
        if (strcmp(user, "Not Allocated") != 0 && strcmp(oid, orderid) == 0)
        {
            strcpy(res, "Delivery Man Allocated, Preparing");
        }
        if (strcmp(status, "Picked up. On the Way") == 0 && strcmp(oid, orderid) == 0)
        {
            strcpy(res, "Picked up. On the Way");
        }
        if (strcmp(status, "Delivered") == 0 && strcmp(oid, orderid) == 0)
        {
            strcpy(res, "Delivered");
        }
    }
    fclose(file);
    char *result = strdup(res);
    return result;
}

char *getDeliveryMan(char *orderid)
{
    FILE *file = fopen(DATABASE_ORDER, "r");
    if (file == NULL)
    {
        perror("Error opening menu file");
    }
    char line[5000];
    char res[100];
    while (fgets(line, sizeof(line), file))
    {
        // Parse line into variables
        char oid[100];
        char user_name[MAX_RESTAURANT_NAME_LEN];
        char username[MAX_RESTAURANT_NAME_LEN];
        char restaurant_name[MAX_ITEM_NAME_LEN];
        char restaurant_username[MAX_CATEGORY_LEN];
        char itemid[MAX_CATEGORY_LEN];
        char itemname[MAX_CATEGORY_LEN];
        char item_category[MAX_CATEGORY_LEN];
        char item_type[MAX_CATEGORY_LEN];
        int qty;
        int price;
        char contact[MAX_CATEGORY_LEN];
        char address[MAX_CATEGORY_LEN];
        char pincode[MAX_CATEGORY_LEN];
        char time[MAX_LINE_LENGTH];
        char status[MAX_LINE_LENGTH];

        if (sscanf(line, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%d,%d,%[^,],%[^,],%[^,],%[^,],%[^\n]", oid, user_name, username, restaurant_name, restaurant_username, itemid, itemname, item_category, item_type, &qty, &price, contact, address, pincode, time, status) == 16)
        {
            if (strcmp(status, "pending") == 0 && strcmp(oid, orderid) == 0)
            {
                strcpy(res, "Not Allocated");
            }
        }
    }
    fclose(file);
    file = fopen(DATABASE_DELIVERY_ALLOCATION, "r");
    while (fgets(line, sizeof(line), file))
    {
        char oid[100];
        char user[100];
        char status[100];
        sscanf(line, "%[^,],%[^,],%[^\n]", oid, user, status);
        if (strcmp(user, "Not Allocated") == 0 && strcmp(oid, orderid) == 0)
        {
            strcpy(res, "Not Allocated");
        }
        if (strcmp(user, "Not Allocated") != 0 && strcmp(oid, orderid) == 0)
        {
            strcpy(res, user);
        }
        if (strcmp(status, "Picked up. On the Way") == 0 && strcmp(oid, orderid) == 0)
        {
            strcpy(res, user);
        }
        if (strcmp(status, "Delivered") == 0 && strcmp(oid, orderid) == 0)
        {
            strcpy(res, user);
        }
    }
    fclose(file);
    char *result = strdup(res);
    return result;
}

char *getCurrentOid(char *user){
     FILE *file = fopen(DATABASE_ORDER, "r");
    if (file == NULL)
    {
        perror("Error opening menu file");
    }
    char line[5000];
    char res[100] = "blah";
    char orderids[300][100];
    int ocnt = 0;
    while (fgets(line, sizeof(line), file))
    {
        // Parse line into variables
        char oid[100];
        char user_name[MAX_RESTAURANT_NAME_LEN];
        char username[MAX_RESTAURANT_NAME_LEN];
        char restaurant_name[MAX_ITEM_NAME_LEN];
        char restaurant_username[MAX_CATEGORY_LEN];
        char itemid[MAX_CATEGORY_LEN];
        char itemname[MAX_CATEGORY_LEN];
        char item_category[MAX_CATEGORY_LEN];
        char item_type[MAX_CATEGORY_LEN];
        int qty;
        int price;
        char contact[MAX_CATEGORY_LEN];
        char address[MAX_CATEGORY_LEN];
        char pincode[MAX_CATEGORY_LEN];
        char time[MAX_LINE_LENGTH];
        char status[MAX_LINE_LENGTH];

        if (sscanf(line, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%d,%d,%[^,],%[^,],%[^,],%[^,],%[^\n]", oid, user_name, username, restaurant_name, restaurant_username, itemid, itemname, item_category, item_type, &qty, &price, contact, address, pincode, time, status) == 16)
        {
            if (strcmp(status, "pending") == 0 && strcmp(username, user) == 0)
            {
                 strcpy(res, oid);
            }
            if (strcmp(status, "pending") != 0 && strcmp(username, user) == 0)
            {
                strcpy(orderids[ocnt], oid);
                ocnt++;
            }
        }
    }
    fclose(file);
    file = fopen(DATABASE_DELIVERY_ALLOCATION, "r");
    while (fgets(line, sizeof(line), file))
    {
        char oid[100];
        char user[100];
        char status[100];
        sscanf(line, "%[^,],%[^,],%[^\n]", oid, user, status);
        for (int i = 0; i < ocnt; i++)
        {
            if (strcmp(user, "Not Allocated") == 0 && strcmp(oid, orderids[i]) == 0)
            {
                 strcpy(res, oid);
            }
            if (strcmp(status, "allocated") == 0 && strcmp(oid, orderids[i]) == 0)
            {
                 strcpy(res, oid);
            }
            if (strcmp(status, "Picked up. On the Way") == 0 && strcmp(oid, orderids[i]) == 0)
            {
                strcpy(res, oid);
            }
            if (strcmp(status, "Delivered") == 0 && strcmp(oid, orderids[i]) == 0)
            {
            }
        }
    }
    fclose(file);
    char *result = strdup(res);
    return result;
}
char *getReviewOid(char *user){
    FILE *file = fopen(DATABASE_ORDER, "r");
    if (file == NULL)
    {
        perror("Error opening menu file");
    }
    char line[5000];
    char res[100] = "blah";
    char orderids[300][100];
    int ocnt = 0;
    while (fgets(line, sizeof(line), file))
    {
        // Parse line into variables
        char oid[100];
        char user_name[MAX_RESTAURANT_NAME_LEN];
        char username[MAX_RESTAURANT_NAME_LEN];
        char restaurant_name[MAX_ITEM_NAME_LEN];
        char restaurant_username[MAX_CATEGORY_LEN];
        char itemid[MAX_CATEGORY_LEN];
        char itemname[MAX_CATEGORY_LEN];
        char item_category[MAX_CATEGORY_LEN];
        char item_type[MAX_CATEGORY_LEN];
        int qty;
        int price;
        char contact[MAX_CATEGORY_LEN];
        char address[MAX_CATEGORY_LEN];
        char pincode[MAX_CATEGORY_LEN];
        char time[MAX_LINE_LENGTH];
        char status[MAX_LINE_LENGTH];

        if (sscanf(line, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%d,%d,%[^,],%[^,],%[^,],%[^,],%[^\n]", oid, user_name, username, restaurant_name, restaurant_username, itemid, itemname, item_category, item_type, &qty, &price, contact, address, pincode, time, status) == 16)
        {
            if (strcmp(status, "pending") != 0 && strcmp(username, user) == 0)
            {
                strcpy(orderids[ocnt], oid);
                ocnt++;
            }
        }
    }
    fclose(file);
    file = fopen(DATABASE_DELIVERY_ALLOCATION, "r");

    int maxid = 0;
    char maxidc[100] = "blah";

    while (fgets(line, sizeof(line), file))
    {
        char oid[100];
        int oidint;
        char user[100];
        char status[100];
        sscanf(line, "%[^,],%[^,],%[^\n]", oid, user, status);
        sscanf(line, "%d,%[^,],%[^\n]", &oidint, user, status);
        for (int i = 0; i < ocnt; i++)
        {
            if (strcmp(status, "Delivered") == 0 && strcmp(oid, orderids[i]) == 0)
            {
                if(oidint >= maxid){
                    maxid = oidint;
                    strcpy(maxidc, oid);
                }
            }
        }
    }
    fclose(file);

    file = fopen(DATABASE_FEEDBACK, "r");
    if (file == NULL)
    {
        perror("Error opening menu file");
    }
    int chkfin = 1;
    while (fgets(line, sizeof(line), file))
    {
        char oid[100];
        char user[100];
        char rest[100];
        int rating;
        char comment[500];
        sscanf(line, "%[^,],%[^,],%[^,],%d,%[^\n]", oid, user, rest, &rating, comment);
        if(strcmp(oid,maxidc) == 0){
            chkfin = 0;
        }
    }
    if(chkfin){
        strcpy(res, maxidc);
    }
    else{
        strcpy(res, "blah");
    }
    fclose(file);
    char *result = strdup(res);
    return result;
}



void getCurrent(char *data)
{
    FILE *file = fopen(DATABASE_ORDER, "r");
    if (file == NULL)
    {
        // printf("Error opening file for reading.\n");
        // return 0;
    }
    // // Read lines from the file into an array
    char line[5000];
    int isFirstItem = 1;
    char *gotoid = getCurrentOid(data);
    char mainid[100];
    int chkrev = 0;
    if(strcmp(gotoid, "blah") != 0){
        strcpy(mainid, gotoid);
    }
    else{
        char *reviewoid = getReviewOid(data);
        if(strcmp(reviewoid, "blah") != 0){
            strcpy(mainid, reviewoid);
            chkrev = 1;
        }
        else{
            strcpy(mainid, "blah");
        }
    }
    printf("[");
    while (fgets(line, sizeof(line), file))
    {
        char oid[100];
        char user_name[MAX_RESTAURANT_NAME_LEN];
        char username[MAX_RESTAURANT_NAME_LEN];
        char restaurant_name[MAX_ITEM_NAME_LEN];
        char restaurant_username[MAX_CATEGORY_LEN];
        char itemid[MAX_CATEGORY_LEN];
        char itemname[MAX_CATEGORY_LEN];
        char item_category[MAX_CATEGORY_LEN];
        char item_type[MAX_CATEGORY_LEN];
        int qty;
        int price;
        char contact[MAX_CATEGORY_LEN];
        char address[MAX_CATEGORY_LEN];
        char pincode[MAX_CATEGORY_LEN];
        char time[MAX_LINE_LENGTH];
        char status[MAX_LINE_LENGTH];

        if (sscanf(line, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%d,%d,%[^,],%[^,],%[^,],%[^,],%[^\n]", oid, user_name, username, restaurant_name, restaurant_username, itemid, itemname, item_category, item_type, &qty, &price, contact, address, pincode, time, status) == 16)
        {
            if (strcmp(username, data) == 0 && strcmp(oid, mainid) == 0)
            {
                if (!isFirstItem)
                {
                    printf(","); // Add comma for subsequent items
                }
                printf("{");

                printf("\"orderid\": \"%s\",", oid);
                printf("\"customerUsername\": \"%s\",", username);
                printf("\"resname\": \"%s\",", restaurant_name);
                printf("\"absolute_status\": \"%s\",", getAbsoluteStatusCustomer(oid));
                printf("\"deliveryMan\": \"%s\",", getDeliveryMan(oid));
                printf("\"restaurantUsername\": \"%s\",", restaurant_username);
                printf("\"id\": %d,", itemid);
                printf("\"food\": \"%s\",", itemname);
                printf("\"category\": \"%s\",", item_category);
                printf("\"type\": \"%s\",", item_type);
                printf("\"quantity\": %d,", qty);
                printf("\"price\": %d,", price);
                if(chkrev){
                printf("\"review\": \"on\"");
                }
                else{
                printf("\"review\": \"off\"");
                }
                printf("}");

                isFirstItem = 0; // Update flag after printing the first item
            }
        }
    }
    printf("]");
    fflush(stdout);
    fclose(file);
}

void getPathCurrent(char *data)
{
    char customer[500];
    char restaurant[500];

    sscanf(data, "%[^,],%[^,]", customer, restaurant);

    printf("%d|%s", getDistance(customer, restaurant), getPath(customer, restaurant));
    fflush(stdout);
}

// searching part.

#define MAX_RESTAURANTS 100
#define MATCHES 100

// Define the structure for a menu entry
typedef struct
{
    int itemID;
    char restaurantName[50];
    char username[50];
    char itemName[50];
    char itemCategory[50];
    char foodPreference[10];
    float price;
} MenuEntry;

typedef struct
{
    char username[50];
    char restaurantName[50];
    char owner[50];
    char password[50];
    char address[100];
    char contact[15];
    char pincode[10];
    int distance;
    char path[50];
    float rating;
} RestaurantEntry;

int compareByDistance3(const void *a, const void *b)
{
    RestaurantEntry *entryA = (RestaurantEntry *)a;
    RestaurantEntry *entryB = (RestaurantEntry *)b;

    if (entryA->distance != entryB->distance)
        return entryA->distance - entryB->distance;

    if (entryA->rating < entryB->rating)
        return 1;
    if (entryA->rating > entryB->rating)
        return -1;

    return 0;
}

int searchRestaurantByID(char *restaurantIDs[], int numRestaurants, char *user)
{
    FILE *file = fopen(DATABASE_RESTAURANT, "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return 1;
    }

    RestaurantEntry entries[MAX_RESTAURANTS];

    char each_line[256];
    int count = 0;
    int found = 0;

    // Read the file each_line by each_line
    while (fgets(each_line, sizeof(each_line), file))
    {
        // Remove new line character from the end of the line if present
        each_line[strcspn(each_line, "\n")] = '\0';

        RestaurantEntry entry;

        // Parse the line and populate the restaurant entry
        char *token = strtok(each_line, ",");
        if (token != NULL)
            strncpy(entry.restaurantName, token, sizeof(entry.restaurantName) - 1);

        token = strtok(NULL, ",");
        if (token != NULL)
            strncpy(entry.owner, token, sizeof(entry.owner) - 1);

        token = strtok(NULL, ",");
        if (token != NULL)
            strncpy(entry.username, token, sizeof(entry.username) - 1);

        token = strtok(NULL, ",");
        if (token != NULL)
            strncpy(entry.password, token, sizeof(entry.password) - 1);

        token = strtok(NULL, ",");
        if (token != NULL)
            strncpy(entry.contact, token, sizeof(entry.contact) - 1);

        token = strtok(NULL, ",");
        if (token != NULL)
            strncpy(entry.address, token, sizeof(entry.address) - 1);

        token = strtok(NULL, ",");
        if (token != NULL)
            strncpy(entry.pincode, token, sizeof(entry.pincode) - 1);

        token = strtok(NULL, ",");
        if (token != NULL)
            entry.rating = atof(token);

        entry.distance = getDistance(user, entry.username);
        strcpy(entry.path, getPath(user, entry.username)); // USE DIJKSTRA FROM dijkstra.c to access those functions

        // Check if the restaurant ID matches any of the IDs in the array
        int i;
        for (i = 0; i < numRestaurants; i++)
        {
            char buffer[30];
            strcpy(buffer, entry.username);
            int k = 0;
            for (k = 0; buffer[k]; k++)
            {
                buffer[k] = tolower(buffer[k]);
            }

            if (strcmp(buffer, restaurantIDs[i]) == 0)
            {
                if (count < MAX_RESTAURANTS)
                {
                    entries[count++] = entry;
                }
                break;
            }
        }
    }

    fclose(file);

    qsort(entries, count, sizeof(RestaurantEntry), compareByDistance3);

    printf("[");
    int i = 0;
    if (allowedToOrder(user))
    {
        for (i = 0; i < count; i++)
        {
            if (i > 0)
            {
                printf(",");
            }
            printf("{");
            printf("\"username\": \"%s\",", entries[i].username);
            printf("\"restaurantName\": \"%s\",", entries[i].restaurantName);
            printf("\"contact\": \"%s\",", entries[i].contact);
            printf("\"distance\": %d,", entries[i].distance);
            printf("\"path\": \"%s\",", entries[i].path);
            printf("\"address\": \"%s, %s\",", entries[i].address, entries[i].pincode);
            printf("\"rating\": %f", entries[i].rating);
            printf("}");
        }
    }
    printf("]");
    fflush(stdout);
    return 0;
}

// Function to search for substrings in each each_line of the menu database
void searchMenuForItemGiven(char *given)
{

    char user[50];
    char ItemGiven[50];
    char *token = strtok(given, ","); // TO separate substring into username and item to be searched
    strcpy(user, token);
    token = strtok(NULL, ",");
    strcpy(ItemGiven, token);
    char uname[100];

    FILE *file = fopen(DATABASE_MENU, "r");
    if (file == NULL)
    {
        perror("Error opening file");
        exit(1);
    }

    // Allocate memory for the array of restaurant IDs
    char **restaurantIDs = (char **)malloc(MATCHES * sizeof(char *));
    if (restaurantIDs == NULL)
    {
        perror("Error allocating memory");
        exit(1);
    }

    char each_line[256];
    char buffer[256];
    int found = 0;

    while (fgets(each_line, sizeof(each_line), file))
    {
        // Check for new line to remove
        each_line[strcspn(each_line, "\n")] = '\0';

        int k = 0;
        for (k = 0; each_line[k]; k++)
        {
            each_line[k] = tolower(each_line[k]);
        }

        // Check if any of the item are found in the line
        int i;
        for (i = 0; i < 1; i++)
        {
            if (strstr(each_line, ItemGiven) != NULL)
            {
                // To get username of restaurant
                char *token = strtok(each_line, ",");
                token = strtok(NULL, ",");
                token = strtok(NULL, ",");
                strcpy(uname, token);
                token = strtok(NULL, ",");
                token = strtok(NULL, ",");
                token = strtok(NULL, ",");
                // restaurant array storing
                restaurantIDs[found] = (char *)malloc((strlen(uname) + 1) * sizeof(char));
                if (restaurantIDs[found] == NULL)
                {
                    perror("Error allocating memory");
                    exit(1);
                }
                if(strcmp(ItemGiven, "veg") == 0 && strcmp(token, "veg") == 0){
                    strcpy(restaurantIDs[found], uname);
                    found++;
                }
                else if(strcmp(ItemGiven, "non-veg") == 0 && strcmp(token, "non-veg") == 0){
                    strcpy(restaurantIDs[found], uname);
                    found++;
                }
                else if(strcmp(ItemGiven, "non-veg") != 0 && strcmp(ItemGiven, "veg") != 0){
                    strcpy(restaurantIDs[found], uname);
                    found++;
                }
               
                break;
            }
        }
    }

    // Close the file
    fclose(file);

    searchRestaurantByID(restaurantIDs, found, user);

    int i;
    for (i = 0; i < found; i++)
    {
        free(restaurantIDs[i]);
    }
    free(restaurantIDs);
}

void getViewBillCustomer(char *data)
{
    FILE *file = fopen(DATABASE_ORDER, "r");
    if (file == NULL)
    {
        perror("Error opening menu file");
        return;
    }

    char line[5000];
    int isFirstItem = 1; // Flag to track the first item in JSON array
    // int unoid = 0;

    printf("[");
    while (fgets(line, sizeof(line), file))
    {
        // Parse line into variables
        char orderid[MAX_ITEM_NAME_LEN];
        char user_name[MAX_RESTAURANT_NAME_LEN];
        char username[MAX_RESTAURANT_NAME_LEN];
        char restaurant_name[MAX_ITEM_NAME_LEN];
        char restaurant_username[MAX_CATEGORY_LEN];
        char itemid[MAX_CATEGORY_LEN];
        char itemname[MAX_CATEGORY_LEN];
        char item_category[MAX_CATEGORY_LEN];
        char item_type[MAX_CATEGORY_LEN];
        int qty;
        int price;
        char contact[MAX_CATEGORY_LEN];
        char address[MAX_CATEGORY_LEN];
        char pincode[MAX_CATEGORY_LEN];
        char time[MAX_LINE_LENGTH];
        char status[MAX_LINE_LENGTH];

        if (sscanf(line, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%d,%d,%[^,],%[^,],%[^,],%[^,],%[^\n]", orderid, user_name, username, restaurant_name, restaurant_username, itemid, itemname, item_category, item_type, &qty, &price, contact, address, pincode, time, status) == 16)
        {
            // printf("%d", orderid);
            if (strcmp(orderid, data) == 0)
            {
                if (!isFirstItem)
                {
                    printf(","); // Add comma for subsequent items
                }
                printf("{");
                printf("\"orderid\": %s,", orderid);
                printf("\"absolute_status\": \"%s\",", getAbsoluteStatusCustomer(orderid));
                printf("\"user_name\": \"%s\",", user_name);
                printf("\"username\": \"%s\",", username);
                printf("\"restaurantName\": \"%s\",", restaurant_name);
                printf("\"itemid\": %s,", itemid);
                printf("\"itemname\": \"%s\",", itemname);
                printf("\"item_category\": \"%s\",", item_category);
                printf("\"item_type\": \"%s\",", item_type);
                printf("\"quantity\": %d,", qty);
                printf("\"price\": %d,", price);
                printf("\"address\": \"%s\",", address);
                printf("\"time\": \"%s\",", time);
                printf("\"contact\": \"%s\",", contact);
                printf("\"status\": \"%s\",", status);
                printf("\"pincode\": \"%s\"", pincode);
                printf("}");
                isFirstItem = 0; // Update flag after printing the first item
            }
        }
    }
    printf("]");
    fflush(stdout);
    fclose(file);
}


void getOldOrderCustomer(char *data)
{
    int oids[1000];
    char line[5000];
    int oidcnt = 0;
    FILE *file = fopen(DATABASE_DELIVERY_ALLOCATION, "r");
    if (file == NULL)
    {
        perror("Error opening menu file");
        return;
    }
    while (fgets(line, sizeof(line), file))
    {
        int oid;
        char user[100];
        char status[100];
        sscanf(line, "%d,%[^,],%[^\n]", &oid, user, status);
        if (strcmp(status, "Delivered") == 0)
        {
            oids[oidcnt] = oid;
            oidcnt++;
        }
    }
    fclose(file);
    file = fopen(DATABASE_ORDER, "r");
    if (file == NULL)
    {
        perror("Error opening menu file");
        return;
    }

    int isFirstItem = 1; // Flag to track the first item in JSON array
    int unoid = 0;
  


    printf("[");
    while (fgets(line, sizeof(line), file))
    {
        // Parse line into variables
        int orderid;
        char user_name[MAX_RESTAURANT_NAME_LEN];
        char username[MAX_RESTAURANT_NAME_LEN];
        char restaurant_name[MAX_ITEM_NAME_LEN];
        char restaurant_username[MAX_CATEGORY_LEN];
        int itemid;
        char itemname[MAX_CATEGORY_LEN];
        char item_category[MAX_CATEGORY_LEN];
        char item_type[MAX_CATEGORY_LEN];
        int qty;
        int price;
        char contact[MAX_CATEGORY_LEN];
        char address[MAX_CATEGORY_LEN];
        char pincode[MAX_CATEGORY_LEN];
        char time[MAX_LINE_LENGTH];
        char status[MAX_LINE_LENGTH];
        int check = 0;

        if (sscanf(line, "%d,%[^,],%[^,],%[^,],%[^,],%d,%[^,],%[^,],%[^,],%d,%d,%[^,],%[^,],%[^,],%[^,],%[^\n]s", &orderid, user_name, username, restaurant_name, restaurant_username, &itemid, itemname, item_category, item_type, &qty, &price, contact, address, pincode, time, status) == 16)
        {
            // printf("%d", orderid);
            for(int i = 0; i < oidcnt; i++){
                if(oids[i] == orderid){
                    check = 1;
                }
            }
            if ((strcmp(username, data) == 0) && (unoid != orderid) && strcmp(status, "accept") == 0 && check == 1)
            {
                unoid = orderid;
                if (!isFirstItem)
                {
                    printf(","); // Add comma for subsequent items
                }
                printf("{");
                printf("\"orderid\": %d,", orderid);
                printf("\"user_name\": \"%s\",", user_name);
                printf("\"username\": \"%s\",", username);
                printf("\"restaurant_name\": \"%s\",", restaurant_name);
                printf("\"restaurant_username\": \"%s\",", restaurant_username);
                printf("\"address\": \"%s\",", address);
                printf("\"time\": \"%s\",", time);
                printf("\"contact\": \"%s\",", contact);
                printf("\"pincode\": \"%s\"", pincode);
                printf("}");

                isFirstItem = 0; // Update flag after printing the first item
            }
        }
    }
    printf("]");
    fflush(stdout);
    fclose(file);
}

void submitReview(char *dataset){
    FILE *file = fopen(DATABASE_FEEDBACK, "r");
    if (file == NULL)
    {
        perror("Error opening menu file");
    }
    int chkfin = 1;
    char line[1000];
    char did[100],drest[100],dcust[100],drate[100],dreview[100];
    sscanf(dataset,"%[^,],%[^,],%[^,],%[^,],%[^\n]",did,dcust,drest,drate,dreview);
    while (fgets(line, sizeof(line), file))
    {
        char oid[100];
        char user[100];
        char rest[100];
        int rating;
        char comment[500];
        sscanf(line, "%[^,],%[^,],%[^,],%d,%[^\n]", oid, user, rest, &rating, comment);
        if(strcmp(oid,did) == 0){
            chkfin = 0;
        }
    }
    fclose(file);
    if(chkfin){
        file = fopen(DATABASE_FEEDBACK, "a");
        fprintf(file,"%s\n",dataset);
        printf("Review has been submitted!");
    }
    else{
        printf("Cannot Submit this review");
    }
    fclose(file);
    fflush(stdout);
    updateAvgRating(drest);
}

void addRecToCart(char *dataset){
    char username[100];
    char itemid[100];
    char quantity[100];

    sscanf(dataset,"%[^,],%[^,],%[^,]", username,itemid,quantity);
    FILE *file = fopen(DATABASE_MENU, "r");
    if (file == NULL)
    {
        perror("Error opening menu file");
        return;
    }

    char line[5000];
    int isFirstItem = 1; // Flag to track the first item in JSON array
    char result[1000];
    while (fgets(line, sizeof(line), file))
    {
        // Parse line into variables
        char id[100];
        char restaurant[MAX_RESTAURANT_NAME_LEN];
        char user[MAX_RESTAURANT_NAME_LEN];
        char item[MAX_ITEM_NAME_LEN];
        char category[MAX_CATEGORY_LEN];
        char type[MAX_CATEGORY_LEN];
        char price[100];

        if (sscanf(line, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^\n]", id, restaurant, user, item, category, type, price) == 7)
        {
            if (strcmp(itemid, id) == 0)
            {
               sprintf(result, "%s,%s,%s,%s,%s,%s,%s,%s,%s", username,restaurant,user,id,item,category,type,quantity,price);
            }
        }
    }
    fclose(file);
    file = fopen(DATABASE_CART, "a");
    fprintf(file, "%s\n", result);
    fclose(file);

    printf("Item Added To Cart");
    fflush(stdout);
}
