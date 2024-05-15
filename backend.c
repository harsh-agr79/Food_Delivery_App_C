// backend.c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "restaurant.c"

#define MAX_USERNAME_LENGTH 50
#define MAX_PASSWORD_LENGTH 50
#define DATABASE_USER "users.txt"
#define DATABASE_RESTAURANT "restaurants.txt"
#define DATABASE_DELIVERYMAN "deliveryman.txt"

int authenticate(char username[], char password[]) {
    FILE *file = fopen(DATABASE_USER, "r");
    if (!file) {
        printf("Error opening database file.\n");
        return 0;
    }
    FILE *file2 = fopen(DATABASE_RESTAURANT, "r");
    if (!file) {
        printf("Error opening database file.\n");
        return 0;
    }
    FILE *file3 = fopen(DATABASE_DELIVERYMAN, "r");
    if (!file) {
        printf("Error opening database file.\n");
        return 0;
    }

    char line[500]; // Assuming a maximum line length of 100 characters
    char db_username[MAX_USERNAME_LENGTH];
    char db_password[MAX_PASSWORD_LENGTH];
    char db_name[MAX_PASSWORD_LENGTH];
    char db_owner[MAX_PASSWORD_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        sscanf(line,"%[^,],%[^,],%[^,]",db_name, db_username, db_password);
        if (strcmp(username, db_username) == 0 && strcmp(password, db_password) == 0) {
            fclose(file);
            return 1;
        }
    }
        while (fgets(line, sizeof(line), file2)) {
        sscanf(line,"%[^,],%[^,],%[^,],%[^,]",db_name,db_owner,db_username, db_password);
        if (strcmp(username, db_username) == 0 && strcmp(password, db_password) == 0) {
            fclose(file);
            return 1;
        }
    }
        while (fgets(line, sizeof(line), file3)) {
        sscanf(line,"%[^,],%[^,],%[^,]",db_name, db_username, db_password);
        if (strcmp(username, db_username) == 0 && strcmp(password, db_password) == 0) {
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}

void finduser(char un[], char type[]){
FILE *file = fopen(DATABASE_USER, "r");
    if (!file) {
        printf("Error opening database file.\n");
    }
    FILE *file2 = fopen(DATABASE_RESTAURANT, "r");
    if (!file) {
        printf("Error opening database file.\n");
    }
    FILE *file3 = fopen(DATABASE_DELIVERYMAN, "r");
    if (!file) {
        printf("Error opening database file.\n");
    }

    char line[500]; // Assuming a maximum line length of 100 characters
    char db_username[MAX_USERNAME_LENGTH];
    char db_password[MAX_PASSWORD_LENGTH];
    char db_name[MAX_PASSWORD_LENGTH];
    char db_resname[MAX_PASSWORD_LENGTH];
    char db_owner[MAX_PASSWORD_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        sscanf(line,"%[^,],%[^,],%[^,]",db_name, db_username, db_password);
        if (strcmp(un, db_username) == 0) {
            fclose(file);
	    strcpy(type, "customer");
        }
    }
        while (fgets(line, sizeof(line), file2)) {
         sscanf(line,"%[^,],%[^,],%[^,]",db_resname, db_owner, db_username);
        if (strcmp(un, db_username) == 0) {
            fclose(file2);
            strcpy(type, "restaurant");
        }
    }
        while (fgets(line, sizeof(line), file3)) {
        sscanf(line,"%[^,],%[^,],%[^,]",db_name, db_username, db_password);
        if (strcmp(un, db_username) == 0) {
            fclose(file3);
            strcpy(type, "delivery");
        }
    }

    fclose(file);
    fclose(file2);
    fclose(file3);
}

void login(char *data){
    int check = 0,authenticated = 0;
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    char type[30];
    sscanf(data, "%[^,],%[^,]", username, password);
    authenticated = authenticate(username, password);
    if (authenticated)
    {
        check = 1;
        finduser(username, type);
        printf("%d,%s,%s\n",check, username, type);
        fflush(stdout);
    }
    else{
        check = 0;
        printf("%d,%s\n", check,username);
        fflush(stdout);
    }
}

int username_validate(char username[]){
	FILE *file = fopen(DATABASE_USER, "r");
	FILE *file2 = fopen(DATABASE_RESTAURANT, "r");
	FILE *file3 = fopen(DATABASE_DELIVERYMAN, "r");
    	if (!file || !file2 || !file3) {
       	 printf("Error opening database file.\n");
        	return 0;
    	}

    char line[500]; // Assuming a maximum line length of 100 characters
    char db_username[MAX_USERNAME_LENGTH];
    char db_username2[MAX_USERNAME_LENGTH];
    char db_password[MAX_PASSWORD_LENGTH];
    char db_name[MAX_PASSWORD_LENGTH];
    char db_resname[MAX_PASSWORD_LENGTH];
    char db_owner[MAX_PASSWORD_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        sscanf(line,"%[^,],%[^,]",db_name, db_username);
        if (strcmp(username, db_username) == 0) {
            fclose(file);
            return 1;
        }
    }
    while (fgets(line, sizeof(line), file2)) {
        sscanf(line,"%[^,],%[^,],%[^,]",db_resname, db_owner, db_username);
        if (strcmp(username, db_username) == 0) {
            fclose(file);
            return 1;
        }
    }
     while (fgets(line, sizeof(line), file3)) {
        sscanf(line,"%[^,],%[^,]",db_name, db_username);
        if (strcmp(username, db_username) == 0) {
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    fclose(file2);
    fclose(file3);
    return 0;
}

int registerUser(char *data)
{

    // char name[155], username[155], password[155], phone[155], address[155], pincode[155];

    // sscanf(data, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,]", name, username, password, phone, address, pincode);

    FILE *file = fopen(DATABASE_USER, "a");
    if (!file)
    {
        printf("Error opening database file.\n");
        return 0;
    }
     char username[MAX_USERNAME_LENGTH];
     char name[MAX_USERNAME_LENGTH];
     sscanf(data,"%[^,],%[^,]",name, username);
     if(username_validate(username) == 0){
        fprintf(file, "%s\n", data);
        fclose(file);
        return 1;
     }
     else{
        fclose(file);
        return 500;
     }

    // fprintf(file, "%s,%s,%s,%s,%s,%s\n", name, username, password, phone, address, pincode);
    return 500;
}


int registerRes(char *data)
{

    // char name[155], username[155], password[155], phone[155], address[155], pincode[155];

    // sscanf(data, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,]", name, username, password, phone, address, pincode);

    FILE *file = fopen(DATABASE_RESTAURANT, "a");
    if (!file)
    {
        printf("Error opening database file.\n");
        return 0;
    }

    // fprintf(file, "%s,%s,%s,%s,%s,%s\n", name, username, password, phone, address, pincode);
    char username[MAX_USERNAME_LENGTH];
     char name[MAX_USERNAME_LENGTH];
     char resname[MAX_USERNAME_LENGTH];
     sscanf(data,"%[^,],%[^,],%[^,]", resname,name, username);
     if(username_validate(username) == 0){
        fprintf(file, "%s\n", data);
        fclose(file);
        return 1;
     }
     else{
        fclose(file);
        return 500;
     }

    return 500;
}
int registerDm(char *data)
{

    // char name[155], username[155], password[155], phone[155], address[155], pincode[155];

    // sscanf(data, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,]", name, username, password, phone, address, pincode);

    FILE *file = fopen(DATABASE_DELIVERYMAN, "a");
    if (!file)
    {
        printf("Error opening database file.\n");
        return 0;
    }

    // fprintf(file, "%s,%s,%s,%s,%s,%s\n", name, username, password, phone, address, pincode);
   char username[MAX_USERNAME_LENGTH];
     char name[MAX_USERNAME_LENGTH];
     sscanf(data,"%[^,],%[^,]",name, username);
     if(username_validate(username) == 0){
        fprintf(file, "%s\n", data);
        fclose(file);
        return 1;
     }
     else{
        fclose(file);
        return 500;
     }

    // fprintf(file, "%s,%s,%s,%s,%s,%s\n", name, username, password, phone, address, pincode);
    return 500;
}

int main()
{
    char function[500];
    char data[5000];

    // Get username and password from Electron.js
    while (scanf("%s %[^\n]s", function, data) == 2)
    {
        if (strcmp(function, "registerUser") == 0)
        {
            int res = registerUser(data);
            printf("%d\n", res);
            fflush(stdout);
        }
        else if (strcmp(function, "registerRes") == 0)
        {
            int res = registerRes(data);
            printf("%d\n", res);
            fflush(stdout);
        }
          else if (strcmp(function, "registerDm") == 0)
        {
            int res = registerDm(data);
            printf("%d\n", res);
            fflush(stdout);
        }
         else if (strcmp(function, "login") == 0)
        {
           login(data);
        }
         else if (strcmp(function, "getMenu") == 0)
        {
           getMenu(data);
        }
         else if (strcmp(function, "addMenuItem") == 0)
        {
            addMenuItem(data);
        }
         else if (strcmp(function, "deleteMenuItem") == 0)
        {
           deleteMenuItem(data);
        }
        else if (strcmp(function, "editMenuItem") == 0)
        {
           editMenuItem(data);
        }
          else if (strcmp(function, "setRestaurantLocation") == 0)
        {
           setRestaurantLocation(data);
        }
         else if (strcmp(function, "getRestaurantLocation") == 0)
        {
           getRestaurantLocation(data);
        }
    }

    return 0;
}
