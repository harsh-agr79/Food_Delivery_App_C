#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

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
#define DATABASE_ORDER "order.txt"

char* get_current_timestamp() {
    time_t t;
    struct tm *tmp;
    char *format = "%Y-%m-%d %H:%M:%S";  // Desired format: YYYY-MM-DD HH:MM:SS
    char buf[64];

    // Get the current time
    t = time(NULL);

    // Convert the time to a struct tm form
    tmp = localtime(&t);
    if (tmp == NULL) {
        perror("localtime");
        return NULL;
    }

    // Format the time as a string
    if (strftime(buf, sizeof(buf), format, tmp) == 0) {
        fprintf(stderr, "strftime returned 0");
        return NULL;
    }

    // Allocate memory for the timestamp string
    char *timestamp = malloc(strlen(buf) + 1);
    if (timestamp == NULL) {
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
} Restaurant;

int compareByDistance(const void *a, const void *b)
{
    return ((Restaurant *)a)->distance - ((Restaurant *)b)->distance;
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
        if (strcmp(username, customer) == 0 && strcmp(type, "restaurant") != 0)
        {
            node1 = node;
        }
        else if (strcmp(username, restaurant) == 0 && strcmp(type, "restaurant") == 0)
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
        if (strcmp(username, customer) == 0 && strcmp(type, "restaurant") != 0)
        {
            node1 = node;
        }
        else if (strcmp(username, restaurant) == 0 && strcmp(type, "restaurant") == 0)
        {
            node2 = node;
        }
    }
    char *res = callDKS(node1, node2);
    char *result = strdup(res);
    return result;
}

int findOrderID(){
    FILE *file = fopen(DATABASE_ORDER, "r");
    if (!file) {
        printf("Error opening database file.\n");
    }
    int id;
    char line[2000];
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

void confirmCart(char *data){
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

    while(fgets(line, sizeof(line), file)){
        sscanf(line, "%[^,]", username);
        if(strcmp(username, data) == 0){
            sscanf(line, "%[^\n]s", editline);
            strcpy(lines[count], editline);
            count++;
        }
        else{
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
    while(fgets(line, sizeof(lines), file)){
        sscanf(line, "%[^,],%[^,]", db_name, db_username);
        if(strcmp(db_username, data) == 0){
            sscanf(line, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^\n]s", db_name, db_username,db_pass, db_contact,db_address, db_pincode);
            break;
        }
    }


    //writing in order db
    file = fopen(DATABASE_ORDER, "a");
    if (file == NULL)
    {
        printf("Error opening file for reading.\n");
    }
    int orderid = findOrderID();
    for (int i = 0; i < count; i++)
    {
       fprintf(file,"%d,%s,%s,%s,%s,%s,%s,pending\n",orderid,db_name,lines[i],db_contact,db_address,db_pincode, timestamp);
    }
    fclose(file);

    //emptying the cart
    file = fopen(DATABASE_CART, "w");
    if (file == NULL)
    {
        printf("Error opening file for reading.\n");
    }
    for (int i = 0; i < clcnt; i++)
    {
       fprintf(file,"%s\n",cartlines[i]);
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

        if (sscanf(line, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^\n]s", restaurantname, user, username, password, contact, address, pincode) == 7)
        {
            strcpy(restaurants[count].restaurantName, restaurantname);
            strcpy(restaurants[count].username, username);
            strcpy(restaurants[count].contact, contact);
            strcpy(restaurants[count].address, address);
            strcpy(restaurants[count].pincode, pincode);
            restaurants[count].distance = getDistance(customer, username);
            strcpy(restaurants[count].path, getPath(customer, username));
            count++;
        }
    }
    fclose(file);

    // Sort the array of structs by distance
    qsort(restaurants, count, sizeof(Restaurant), compareByDistance);

    // Print the sorted array in JSON format
    printf("[");
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
        printf("\"address\": \"%s, %s\"", restaurants[i].address, restaurants[i].pincode);
        printf("}");
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

void getPathCart(char *data){
    char customer[500];
    char restaurant[500];

    sscanf(data, "%[^,],%[^,]", customer, restaurant);
    
    printf("%d|%s",getDistance(customer,restaurant), getPath(customer, restaurant));
    fflush(stdout);
}