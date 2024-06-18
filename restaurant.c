#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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
#define DATABASE_ORDER "order.txt"
#define DATABASE_DELIVERY_ALLOCATION "delivery_allocation.txt"
#define DATABASE_FEEDBACK "feedbacks.txt"

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
        int id;
        char restaurant[MAX_RESTAURANT_NAME_LEN];
        char user[MAX_RESTAURANT_NAME_LEN];
        char item[MAX_ITEM_NAME_LEN];
        char category[MAX_CATEGORY_LEN];
        char type[MAX_CATEGORY_LEN];
        float price;

        if (sscanf(line, "%d,%[^,],%[^,],%[^,],%[^,],%[^,],%f", &id, restaurant, user, item, category, type, &price) == 7)
        {
            if (strcmp(user, username) == 0)
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

int getRestaurantLocation(char *data)
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

int setRestaurantLocation(char *data)
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

char *findResname(char *username)
{
    FILE *file = fopen(DATABASE_RESTAURANT, "r");
    if (!file)
    {
        printf("Error opening database file.\n");
        return NULL;
    }

    char line[500];
    char db_username[MAX_ITEM_NAME_LEN];
    char db_resname[MAX_ITEM_NAME_LEN];
    char db_name[MAX_ITEM_NAME_LEN];

    while (fgets(line, sizeof(line), file))
    {
        sscanf(line, "%[^,],%[^,],%[^,]", db_resname, db_name, db_username);
        if (strcmp(username, db_username) == 0)
        {
            fclose(file);
            char *resname = strdup(db_resname);
            if (!resname)
            {
                printf("Memory allocation failed.\n");
                return NULL;
            }
            return resname;
        }
    }
    fclose(file);
    return NULL; // Return NULL if username not found
}

int findID()
{
    FILE *file = fopen(DATABASE_MENU, "r");
    if (!file)
    {
        printf("Error opening database file.\n");
    }
    int id;
    char line[1000];
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
int deleteMenuItem(char *data)
{

    // Open the file for reading
    FILE *file = fopen(DATABASE_MENU, "r");
    if (file == NULL)
    {
        // printf("Error opening file for reading.\n");
        return 0;
    }

    // // Read lines from the file into an array
    char lines[500][1000];
    char line[1000];
    char id[10];
    int count = 0;

    while (fgets(line, sizeof(line), file))
    {
        sscanf(line, "%[^,]", id);
        if (strcmp(id, data))
        {
            strcpy(lines[count], line);
            count++;
        }
    }
    fclose(file);

    file = fopen(DATABASE_MENU, "w");
    if (file == NULL)
    {
        printf("Error opening file for reading.\n");
        return 0;
    }

    for (int i = 0; i < count; i++)
    {
        fprintf(file, "%s", lines[i]);
    }

    fclose(file);
    printf("1");
    fflush(stdout);
    return 1;
}

int editMenuItem(char *data)
{

    // Open the file for reading
    FILE *file = fopen(DATABASE_MENU, "r");
    if (file == NULL)
    {
        // printf("Error opening file for reading.\n");
        return 0;
    }
    char editid[10];
    char dataset[1000];

    sscanf(data, "%[^,],%[^\n]s", editid, dataset);
    printf("%s %s", editid, dataset);

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

    while (fgets(line, sizeof(line), file))
    {
        sscanf(line, "%[^,]", id);
        if (strcmp(editid, id) == 0)
        {
            sscanf(line, "%[^,],%[^,],%[^,]", dbid, dbresname, dbresusername);
            row = count;
        }
        strcpy(lines[count], line);
        count++;
    }
    fclose(file);

    file = fopen(DATABASE_MENU, "w");
    if (file == NULL)
    {
        printf("Error opening file for reading.\n");
        return 0;
    }

    for (int i = 0; i < count; i++)
    {
        if (i == row)
        {
            fprintf(file, "%s,%s,%s,%s\n", dbid, dbresname, dbresusername, dataset);
        }
        else
        {
            fprintf(file, "%s", lines[i]);
        }
    }

    fclose(file);
    printf("1");
    fflush(stdout);
    return 1;
}
int addMenuItem(char *data)
{
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
    sscanf(data, "%[^,],%[^,]", username, dataset);
    char *resname = findResname(username);
    id = findID();
    if (id > 0)
    {
        fprintf(file, "%d,%s,%s\n", id, resname, data);
        fclose(file);
        printf("1");
        fflush(stdout);
        return 1;
    }
    else
    {
        fclose(file);
        printf("0");
        fflush(stdout);
        return 0;
    }
}

void getRecentOrder(char *data)
{
    FILE *file = fopen(DATABASE_ORDER, "r");
    if (file == NULL)
    {
        perror("Error opening menu file");
        return;
    }

    char line[5000];
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

        if (sscanf(line, "%d,%[^,],%[^,],%[^,],%[^,],%d,%[^,],%[^,],%[^,],%d,%d,%[^,],%[^,],%[^,],%[^,],%[^\n]s", &orderid, user_name, username, restaurant_name, restaurant_username, &itemid, itemname, item_category, item_type, &qty, &price, contact, address, pincode, time, status) == 16)
        {
            // printf("%d", orderid);
            if ((strcmp(restaurant_username, data) == 0) && (unoid != orderid) && strcmp(status, "pending") == 0)
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
char *getAbsoluteStatus(char *orderid)
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

void getViewBill(char *data)
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
                printf("\"absolute_status\": \"%s\",", getAbsoluteStatus(orderid));
                printf("\"user_name\": \"%s\",", user_name);
                printf("\"username\": \"%s\",", username);
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

void replaceWord(char *str, const char *oldWord, const char *newWord)
{
    char *pos = strstr(str, oldWord); // Find the first occurrence of oldWord in str

    if (pos != NULL)
    {
        char temp[1000]; // Temporary buffer to hold modified string
        temp[0] = '\0';  // Initialize the buffer as an empty string

        // Copy characters from str to temp until the first occurrence of oldWord
        strncat(temp, str, pos - str);

        // Append newWord to temp
        strcat(temp, newWord);

        // Append the rest of the characters in str after oldWord
        strcat(temp, pos + strlen(oldWord));

        // Copy the modified string back to str
        strcpy(str, temp);
    }
}

void deliveryAllocate(char *oid)
{
    FILE *file = fopen(DATABASE_DELIVERY_ALLOCATION, "r");
    if (file == NULL)
    {
        perror("Error opening menu file");
        return;
    }
    char line[1000];
    int found = 0;

    while (fgets(line, sizeof(line), file))
    {
        char orderid[100];
        sscanf(line, "%[^,]", orderid);
        if (strcmp(orderid, oid) == 0)
        {
            found = 1;
        }
    }
    fclose(file);
    if (found == 0)
    {
        FILE *file = fopen(DATABASE_DELIVERY_ALLOCATION, "a");
        fprintf(file, "%s,Not Allocated,Preparing\n", oid);
        fclose(file);
    }
}

void changeOrderStatus(char *data)
{
    FILE *file = fopen(DATABASE_ORDER, "r");
    if (file == NULL)
    {
        perror("Error opening menu file");
        return;
    }

    char oid[100];
    char stat[100];
    char line[5000];
    char lines[500][1000];
    int count = 0;

    sscanf(data, "%[^,],%[^,]", oid, stat);

    deliveryAllocate(oid);

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
            if (strcmp(orderid, oid) == 0)
            {
                replaceWord(line, status, stat);
            }
            strcpy(lines[count], line);
            count++;
        }
    }
    fclose(file);

    file = fopen(DATABASE_ORDER, "w");
    if (file == NULL)
    {
        printf("Error opening file for reading.\n");
    }

    for (int i = 0; i < count; i++)
    {
        fprintf(file, "%s", lines[i]);
    }

    fclose(file);
    printf("Order ID: %s, %sed", oid, stat);
    fflush(stdout);
}
void getCurrentOrder(char *data)
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
        if (strcmp(status, "Delivered") != 0)
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
            for (int i = 0; i < oidcnt; i++)
            {
                if (oids[i] == orderid)
                {
                    check = 1;
                }
            }
            if ((strcmp(restaurant_username, data) == 0) && (unoid != orderid) && strcmp(status, "accept") == 0 && check == 1)
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
void getOldOrder(char *data)
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
            for (int i = 0; i < oidcnt; i++)
            {
                if (oids[i] == orderid)
                {
                    check = 1;
                }
            }
            if ((strcmp(restaurant_username, data) == 0) && (unoid != orderid) && strcmp(status, "accept") == 0 && check == 1)
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

void updateAvgRating(char *restaurant)
{
    FILE *file = fopen(DATABASE_FEEDBACK, "r");
    if (file == NULL)
    {
        perror("Error opening feedback file");
        return;
    }
    char line[1000];
    float rcnt = 0;
    float rsum = 0;
    while (fgets(line, sizeof(line), file))
    {
        char oid[100];
        char user[100];
        char rest[100];
        int rating;
        char comment[500];
        char rate[100];
        sscanf(line, "%[^,],%[^,],%[^,],%[^,],%[^\n]", oid, user, rest, rate, comment);
        sscanf(line, "%[^,],%[^,],%[^,],%d,%[^\n]", oid, user, rest, &rating, comment);
        if (strcmp(rate, "NULL") == 0)
        {
            rating = 0;
        }
        if (strcmp(rest, restaurant) == 0)
        {
            rcnt++;
            rsum += rating;
        }
    }
    fclose(file);

    // Avoid division by zero
    if (rcnt == 0)
    {
        fprintf(stderr, "No ratings found for the restaurant %s\n", restaurant);
        return;
    }

    float avgRating = rsum / rcnt;

    // Reading the restaurant file and updating the rating
    file = fopen(DATABASE_RESTAURANT, "r");
    if (file == NULL)
    {
        perror("Error opening restaurant file");
        return;
    }

    char lines[500][1000];
    int cnt = 0;
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
        float oldRating;

        if (sscanf(line, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%f", restaurantname, user, username, password, contact, address, pincode, &oldRating) == 8)
        {
            if (strcmp(username, restaurant) == 0)
            {
                sprintf(lines[cnt], "%s,%s,%s,%s,%s,%s,%s,%.2f\n", restaurantname, user, username, password, contact, address, pincode, avgRating);
            }
            else
            {
                strcpy(lines[cnt], line);
            }
            cnt++;
        }
    }
    fclose(file);

    // Writing the updated content back to the restaurant file
    file = fopen(DATABASE_RESTAURANT, "w");
    if (file == NULL)
    {
        perror("Error opening restaurant file for writing");
        return;
    }
    for (int i = 0; i < cnt; i++)
    {
        fprintf(file, "%s", lines[i]);
    }
    fclose(file);
}

void getRestaurantProfile(char *uname)
{
    FILE *file = fopen(DATABASE_RESTAURANT, "r");
    if (file == NULL)
    {
        perror("Error opening feedback file");
        return;
    }
    char line[1000];
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
        float oldRating;
        if (sscanf(line, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%f", restaurantname, user, username, password, contact, address, pincode, &oldRating) == 8)
        {
            if (strcmp(username, uname) == 0)
            {
                printf("{");
                printf("\"username\": \"%s\",", username);
                printf("\"name\": \"%s\",", user);
                printf("\"restaurantName\": \"%s\",", restaurantname);
                printf("\"contact\": \"%s\",", contact);
                printf("\"password\": \"%s\",", password);
                printf("\"address\": \"%s\",", address);
                printf("\"pincode\": \"%s\",", pincode);
                printf("\"rating\": %.2f", oldRating);
                printf("}");
                break;
            }
        }
    }
    fclose(file);
    fflush(stdout);
}

void editRestaurantProfile(char *data)
{
    FILE *file = fopen(DATABASE_RESTAURANT, "r");
    if (file == NULL)
    {
        perror("Error opening feedback file");
        return;
    }
    char line[1000];
    char editline[1000];
    char lines[1000][1000];
    int count = 0;
    char drname[100],dname[100],duname[100];
    sscanf(data,"%[^,],%[^,],%[^,]",drname,dname,duname);
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
        float oldRating;
        if (sscanf(line, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%f", restaurantname, user, username, password, contact, address, pincode, &oldRating) == 8)
        {
            if (strcmp(username, duname) == 0)
            {
                strcpy(lines[count], data);
                count++;
            }
            else
            {
                sscanf(line, "%[^\n]s", editline);
                strcpy(lines[count], editline);
                count++;
            }
        }
    }
    fclose(file);
    file = fopen(DATABASE_RESTAURANT, "w");
    for(int i = 0; i < count; i++){
        fprintf(file,"%s\n",lines[i]);
    }
    fclose(file);
    printf("Profile Edited");
    fflush(stdout);
}
void getFeedback(char *restaurant){
    FILE *file = fopen(DATABASE_FEEDBACK, "r");
    if (file == NULL)
    {
        perror("Error opening feedback file");
        return;
    }
    char line[1000];
    int isFirstItem = 1;
    printf("[");
     while (fgets(line, sizeof(line), file))
    {
        char oid[100];
        char user[100];
        char rest[100];
        char comment[500];
        char rate[100];
        sscanf(line, "%[^,],%[^,],%[^,],%[^,],%[^\n]", oid, user, rest, rate, comment);
        if (strcmp(rest, restaurant) == 0 && strcmp(rate, "NULL") != 0)
        {  
            if (!isFirstItem)
                {
                    printf(","); // Add comma for subsequent items
                }
                printf("{");
                printf("\"orderid\": %s,", oid);
                printf("\"user\": \"%s\",", user);
                printf("\"rating\": \"%s\",", rate);
                printf("\"comment\": \"%s\"", comment);
                printf("}");

                isFirstItem = 0; // Update flag after printing the first item
            }
    }
    printf("]");
    fclose(file);
    fflush(stdout);
}