#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FOOD_ITEMS 100
#define MAX_NAME_LENGTH 50
// #define MAX_LINE_LENGTH 256

#define MAX_RESTAURANT_NAME_LEN 50
#define MAX_ITEM_NAME_LEN 50
#define MAX_CATEGORY_LEN 20
#define MAX_ITEMS 100
#define MAX_LINES 5000
#define MAX_LINE_LENGTH 5000

#define DATABASE_MENU "menu.txt"
#define DATABASE_CART "cart.txt"

typedef struct
{
    char name[MAX_NAME_LENGTH];
    int frequency;
} FoodItem;

typedef struct
{
    char name[MAX_NAME_LENGTH];
    FoodItem coOccurrences[MAX_FOOD_ITEMS];
    int coOccurrenceCount;
} CoOccurrence;

int compare(const void *a, const void *b)
{
    return ((FoodItem *)b)->frequency - ((FoodItem *)a)->frequency;
}

void updateCoOccurrences(CoOccurrence coOccurrences[], int *coCount, const char *item, const char *coItem)
{
    // Find or create the co-occurrence record for the given item
    int found = 0;
    int i = 0;
    for (i = 0; i < *coCount; i++)
    {
        if (strcmp(coOccurrences[i].name, item) == 0)
        {
            // Update the co-occurrence frequency
            int coFound = 0;
            int j = 0;
            for (j = 0; j < coOccurrences[i].coOccurrenceCount; j++)
            {
                if (strcmp(coOccurrences[i].coOccurrences[j].name, coItem) == 0)
                {
                    coOccurrences[i].coOccurrences[j].frequency++;
                    coFound = 1;
                    break;
                }
            }
            if (!coFound && coOccurrences[i].coOccurrenceCount < MAX_FOOD_ITEMS)
            {
                strncpy(coOccurrences[i].coOccurrences[coOccurrences[i].coOccurrenceCount].name, coItem, MAX_NAME_LENGTH);
                coOccurrences[i].coOccurrences[coOccurrences[i].coOccurrenceCount].frequency = 1;
                coOccurrences[i].coOccurrenceCount++;
            }
            found = 1;
            break;
        }
    }

    // If the item was not found, add it
    if (!found && *coCount < MAX_FOOD_ITEMS)
    {
        strncpy(coOccurrences[*coCount].name, item, MAX_NAME_LENGTH);
        strncpy(coOccurrences[*coCount].coOccurrences[0].name, coItem, MAX_NAME_LENGTH);
        coOccurrences[*coCount].coOccurrences[0].frequency = 1;
        coOccurrences[*coCount].coOccurrenceCount = 1;
        (*coCount)++;
    }
}

void readDataset(const char *filename, CoOccurrence coOccurrences[], int *coCount)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file))
    {
        // Remove newline character
        line[strcspn(line, "\n")] = 0;

        // Split the line by commas
        char *tokens[MAX_FOOD_ITEMS];
        int tokenCount = 0;
        char *token = strtok(line, ",");
        while (token != NULL)
        {
            tokens[tokenCount++] = token;
            token = strtok(NULL, ",");
        }

        // Update co-occurrence counts
        int i = 0;
        for (i = 0; i < tokenCount; i++)
        {
            int j = 0;
            for (j = 0; j < tokenCount; j++)
            {
                if (i != j)
                {
                    updateCoOccurrences(coOccurrences, coCount, tokens[i], tokens[j]);
                }
            }
        }
    }

    fclose(file);
}

void recommendFood(char *data)
{
    CoOccurrence coOccurrences[MAX_FOOD_ITEMS];
    int coCount = 0;
    int topN = 3;
    char itemids[100][100];
    int itemcnt = 0;
    char itemidsCart[100][100];
    int itemcntCart = 0;
    char item[100];
    char user[100];

    sscanf(data, "%[^,],%[^,]", item, user);

    // Read the dataset
    readDataset("transaction_final.txt", coOccurrences, &coCount);

    // Find the co-occurrences for the given item
    for (int i = 0; i < coCount; i++)
    {
        if (strcmp(coOccurrences[i].name, item) == 0)
        {
            // Sort the co-occurrences by frequency
            qsort(coOccurrences[i].coOccurrences, coOccurrences[i].coOccurrenceCount, sizeof(FoodItem), compare);

            // Copy the top N recommendations
            for (int j = 0; j < topN && j < coOccurrences[i].coOccurrenceCount; j++)
            {
                strcpy(itemids[itemcnt], coOccurrences[i].coOccurrences[j].name);
                itemcnt++; // Increment item count
            }
        }
    }

    char line[5000];

    FILE *file = fopen(DATABASE_CART, "r");
    if (file == NULL)
    {
        perror("Error opening menu file");
        return;
    }
    while (fgets(line, sizeof(line), file))
    {
        // Parse line into variables
        char cusUN[255];
        char resname[255];
        char resUN[255];
        char id[100];
        char food[255];
        char category[255];
        char type[255];
        int quantity;
        int price;

        if (sscanf(line, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%d,%d", cusUN, resname, resUN, id, food, category, type, &quantity, &price) == 9)
        {
            if (strcmp(cusUN, user) == 0)
            {
                strcpy(itemidsCart[itemcntCart],id);
                itemcntCart++;
            }
        }
    }
    fclose(file);
    file = fopen(DATABASE_MENU, "r");
    if (file == NULL)
    {
        perror("Error opening menu file");
        return;
    }

    int isFirstItem = 1; // Flag to track the first item in JSON array

    printf("[");
    while (fgets(line, sizeof(line), file))
    {
        // Parse line into variables
        char id[100];
        char restaurant[MAX_RESTAURANT_NAME_LEN];
        char user[MAX_RESTAURANT_NAME_LEN];
        char item[MAX_ITEM_NAME_LEN];
        char category[MAX_CATEGORY_LEN];
        char type[MAX_CATEGORY_LEN];
        float price;
        int check = 0;

        if (sscanf(line, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%f", id, restaurant, user, item, category, type, &price) == 7)
        {
            for (int k = 0; k < itemcnt; k++)
            {
                if (strcmp(id, itemids[k]) == 0)
                {
                    check = 1;
                    break;
                }
            }
            for (int k = 0; k < itemcntCart; k++)
            {
                if (strcmp(id, itemidsCart[k]) == 0)
                {
                    check = 0;
                    break;
                }
            }
            if (check == 1)
            {
                if (!isFirstItem)
                {
                    printf(","); // Add comma for subsequent items
                }
                printf("{");
                printf("\"id\": \"%s\",", id);
                printf("\"food\": \"%s\",", item);
                printf("\"restaurantName\": \"%s\",", restaurant);
                printf("\"username\": \"%s\",", user);
                printf("\"category\": \"%s\",", category);
                printf("\"type\": \"%s\",", type);
                printf("\"price\": %.2f", price);
                printf("}");

                isFirstItem = 0; // Update flag after printing the first item
            }
        }
    }
    printf("]");
    fflush(stdout);
    fclose(file);
}