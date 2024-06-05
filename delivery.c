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
#define DATABASE_ORDER "order.txt"
#define DATABASE_DELIVERY_ALLOCATION "delivery_allocation.txt"

int getDeliveryManLocation(char *data)
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

int setDeliveryManLocation(char *data)
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

typedef struct
{
    int orderid;
    char customer_name[100];
    char restaurant_name[100];
    char username[100];
    char restaurant_username[100];
    int distance;
    char path[300];
} newAlloc;

int compareByDistance2(const void *a, const void *b)
{
    return ((newAlloc *)a)->distance - ((newAlloc *)b)->distance;
}

void getNewAllocation(char *data)
{
    newAlloc alc[500];
    FILE *file = fopen(DATABASE_DELIVERY_ALLOCATION, "r");
    if (file == NULL)
    {
        printf("Error opening file for reading.\n");
        // return 0;
    }

    int orderids[500];
    char line[1000];
    int count = 0;
    int remaining = 0;
    while (fgets(line, sizeof(line), file))
    {
        int oid;
        char user[100];
        char status[100];
        sscanf(line, "%d,%[^,],%[^\n]", &oid, user, status);
        if (strcmp(user, "Not Allocated") == 0)
        {
            orderids[count] = oid;
            count++;
        }
        if (strcmp(user, data) == 0 && strcmp(status, "delivered") != 0)
        {
            remaining = 1;
        }
    }
    fclose(file);

    file = fopen(DATABASE_ORDER, "r");
    if (file == NULL)
    {
        printf("Error opening file for reading.\n");
    }
    int unoid = 0;

    // printf("%d", count);

    while (fgets(line, sizeof(line), file))
    {
        int orderid;
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

        if (sscanf(line, "%d,%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%d,%d,%[^,],%[^,],%[^,],%[^,],%[^\n]", &orderid, user_name, username, restaurant_name, restaurant_username, itemid, itemname, item_category, item_type, &qty, &price, contact, address, pincode, time, status) == 16)
        {
            // printf("%s\n", orderid);
            if (unoid != orderid)
            {
                unoid = orderid;
                for (int i = 0; i < count; i++)
                {
                    if (orderid == orderids[i])
                    {
                        // printf("%d\n", i);
                        alc[i].orderid = orderid;
                        strcpy(alc[i].customer_name, user_name);
                        strcpy(alc[i].username, username);
                        strcpy(alc[i].restaurant_name, restaurant_name);
                        strcpy(alc[i].restaurant_username, restaurant_username);
                        alc[i].distance = getDistance(data, restaurant_username);
                        strcpy(alc[i].path, getPath(data, restaurant_username));
                    }
                }
            }
        }
    }

    fclose(file);
    qsort(alc, count, sizeof(newAlloc), compareByDistance2);
    printf("[");
    if (remaining == 0)
    {
        for (int i = 0; i < count; i++)
        {
            if (i > 0)
            {
                printf(",");
            }
            printf("{");
            printf("\"orderid\": %d,", alc[i].orderid);
            printf("\"restaurantName\": \"%s\",", alc[i].restaurant_name);
            printf("\"customerName\": \"%s\",", alc[i].customer_name);
            printf("\"distance\": \"%d\",", alc[i].distance);
            printf("\"path\": \"%s\",", alc[i].path);
            printf("\"customer_username\": \"%s\",", alc[i].username);
            printf("\"restaurant_username\": \"%s\"", alc[i].restaurant_username);
            printf("}");
        }
    }

    printf("]");
    fflush(stdout);
}

void acceptDelivery(char *data)
{
    int oid;
    char username[100];

    sscanf(data, "%d,%s", &oid, username);

    FILE *file = fopen(DATABASE_DELIVERY_ALLOCATION, "r");
    if (file == NULL)
    {
        printf("Error opening file for reading.\n");
    }

    char line[1000];
    char lines[500][1000];
    int count = 0;

    while (fgets(line, sizeof(line), file))
    {
        int orderid;
        char db_username[100];
        char status[100];
        if (sscanf(line, "%d,%[^,],%[^\n]", &orderid, db_username, status) == 3)
        {
            if (orderid == oid)
            {
                replaceWord(line, db_username, username);
                replaceWord(line, status, "allocated");
            }
            strcpy(lines[count], line);
            count++;
        }
    }
    fclose(file);
    file = fopen(DATABASE_DELIVERY_ALLOCATION, "w");

    for (int i = 0; i < count; i++)
    {
        fprintf(file, "%s", lines[i]);
    }

    fclose(file);
    printf("You have Accepted the Delivery");
    fflush(stdout);
}