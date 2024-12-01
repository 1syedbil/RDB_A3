#pragma warning (disable: 4996)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <mysql.h>

#define MAXSTRING 500

//prototypes
MYSQL* startConnection(char* password);
void clearWithEnter(void);
char* getInput();
int getNum(void);   
void addRental(MYSQL* conn); 
int getId(int table, MYSQL* conn);
void updateCustomer(MYSQL* conn); 
bool checkAvailability(MYSQL* conn, char* inventoryId); 
void getRentalDuration(MYSQL* conn, char* inventoryId); 

int main(void)
{
    char* input = NULL;
    char password[MAXSTRING] = ""; 
    int choice = 0;

    printf("Enter the password: ");

    input = getInput();  
    strcpy(password, input);

    MYSQL* connection = startConnection(password);

    clearWithEnter();

    if (connection == NULL)
    {
        printf("Incorrect password, program closing...\n");
        return 1;
    }

    clearWithEnter();

    while (choice != 5)
    {
        system("cls");
        printf("What would you like to do?\n\t1. Add New Rental\n\t2. Update Customer Info\n\t3. View Rental History\n\t4. Delete Customer Records\n\t5. Exit\n\nEnter a choice (1-5): ");

        choice = getNum();

        switch (choice)
        {
        case 1:
            addRental(connection); 
            system("pause"); 

            break;

        case 2:
            updateCustomer(connection); 
            system("pause"); 

            break;

        case 3:

            break;

        case 4:

            break;

        case 5:
            printf("Exiting...\n");
            break;

        default:
            printf("Invalid menu selection.\n");
            clearWithEnter();
            break;
        }
    }

    mysql_close(connection); 

    return 0;
}

void updateCustomer(MYSQL* conn)
{
    char* input = NULL;  
    char firstName[MAXSTRING] = ""; 
}

void addRental(MYSQL* conn)
{
    MYSQL_RES* res = NULL; 
    MYSQL_ROW row = NULL;   
    bool exists = false;
    char rentalQuery[MAXSTRING] = "INSERT INTO rental(rental_date, return_date, inventory_id, customer_id, staff_id) VALUES(NOW(), NULL,";
    char customer[MAXSTRING] = "";
    char inventoryRec[MAXSTRING] = "";
    char copy[MAXSTRING] = ""; 
    char staffMember[MAXSTRING] = "";
    int id = 0;

    id = getId(1, conn); 
    sprintf(customer, "%d", id);

    id = getId(2, conn);  
    sprintf(inventoryRec, "%d", id); 
    sprintf(copy, "%d", id);  

    if (!checkAvailability(conn, inventoryRec))
    {
        printf("The film with inventory ID {%s} is currently being rented out by someone else.\n\n", inventoryRec);

        return;
    }

    id = getId(3, conn);  
    sprintf(staffMember, "%d", id); 

    strcat(customer, ",");
    strcat(inventoryRec, ","); 
    strcat(staffMember, ")"); 

    strcat(rentalQuery, inventoryRec);
    strcat(rentalQuery, customer);
    strcat(rentalQuery, staffMember);

    if (mysql_query(conn, rentalQuery)) { 
        fprintf(stderr, "%s\n", mysql_error(conn));
        return;
    }

    if (mysql_query(conn, "SELECT * FROM rental ORDER BY rental_id DESC LIMIT 1")) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        return;
    }

    res = mysql_store_result(conn); 

    while (row = mysql_fetch_row(res))
    {
        printf("The following record was successfully added to the rental table:\n\n| ID: %s | Rental Date: %s | Inventory ID: %s | Customer ID: %s | Return Date: %s | Staff ID: %s | Last Update: %s |\n\n", row[0], row[1], row[2], row[3], row[4], row[5], row[6]);
    }

    getRentalDuration(conn, copy);  

    return;
}

void getRentalDuration(MYSQL* conn, char* inventoryId)
{
    MYSQL_RES* res = NULL;
    MYSQL_ROW row = NULL;
    char query[MAXSTRING] = "SELECT film.rental_duration FROM film JOIN inventory ON inventory.film_id = film.film_id WHERE inventory.inventory_id =";
    strcat(query, inventoryId);

    mysql_query(conn, query); 

    res = mysql_store_result(conn); 

    while (row = mysql_fetch_row(res)) 
    {
        printf("The longest this film can be rented out for is %s days.\n\n", row[0]); 
    }

    return;
}

bool checkAvailability(MYSQL* conn, char* inventoryId)
{
    MYSQL_RES* res = NULL; 
    MYSQL_ROW row = NULL;  
    char query[MAXSTRING] = "SELECT * FROM rental WHERE return_date IS NULL AND inventory_id =";
    strcat(query, inventoryId);
    strcat(query, " ORDER BY rental_id DESC LIMIT 1");

    mysql_query(conn, query); 

    res = mysql_store_result(conn); 

    if (res->row_count > 0) 
    {
        return false;
    }
    else
    {
        return true; 
    }
}

int getId(int table, MYSQL* conn) 
{
    MYSQL_RES* res = NULL;
    int id = NULL;  
    MYSQL_ROW row = NULL;
    bool exists = false;
    char customerQuery[MAXSTRING] = "SELECT * FROM customer WHERE customer_id=";
    char inventoryQuery[MAXSTRING] = "SELECT * FROM inventory WHERE inventory_id=";
    char staffQuery[MAXSTRING] = "SELECT * FROM staff WHERE staff_id=";
    char query[MAXSTRING] = ""; 
    char stringId[MAXSTRING] = ""; 

    switch (table)
    {
    case 1:
        strcpy(query, customerQuery);
        break;

    case 2:
        strcpy(query, inventoryQuery);
        break;

    case 3:
        strcpy(query, staffQuery); 
        break;
    }

    while (!exists)
    {
        switch (table)
        {
        case 1:
            printf("Enter a customer ID: ");
            id = getNum(); 
            sprintf(stringId, "%d", id);
            strcat(query, stringId);

            if (mysql_query(conn, query)) {
                fprintf(stderr, "%s\n", mysql_error(conn));
                strcpy(query, customerQuery); 
                system("pause");
                system("cls");
                continue;
            }
            break;

        case 2:
            printf("Enter an inventory ID: ");
            id = getNum();
            sprintf(stringId, "%d", id);
            strcat(query, stringId);

            if (mysql_query(conn, query)) {
                fprintf(stderr, "%s\n", mysql_error(conn));
                strcpy(query, inventoryQuery); 
                system("pause");
                system("cls");
                continue;
            }
            break;

        case 3:
            printf("Enter a staff ID: "); 
            id = getNum();
            sprintf(stringId, "%d", id);
            strcat(query, stringId);

            if (mysql_query(conn, query)) {
                fprintf(stderr, "%s\n", mysql_error(conn));
                strcpy(query, staffQuery); 
                system("pause");
                system("cls");
                continue;
            }
            break;
        }

        res = mysql_store_result(conn);

        if (res->row_count == 0)
        {
            switch (table)
            {
            case 1:
                strcpy(query, customerQuery); 
                printf("A customer corresponding with the ID you entered does not exist. Try again...\n\n");
                break;

            case 2:
                strcpy(query, inventoryQuery);  
                printf("An inventory record corresponding with the ID you entered does not exist. Try again...\n\n"); 
                break;

            case 3:
                strcpy(query, staffQuery);  
                printf("A staff member corresponding with the ID you entered does not exist. Try again...\n\n");
                break;
            }

            system("pause");
            system("cls");
        }
        else
        {
            return id;  
        }
    }
} 

MYSQL* startConnection(char* password)
{
    MYSQL* conn;

    char* server = "127.0.0.1";
    char* user = "root";
    char* database = "sakila";

    conn = mysql_init(NULL);

    if (!mysql_real_connect(conn, server,
        user, password, database, 0, NULL, 0)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        return NULL;
    }

    printf("\nConnection Successful\n\n");

    return conn;
}

//taken from my "TextBasedRPG" repo on my github 1syedbil
void clearWithEnter(void)
{
    system("pause");
    system("cls");
}

char* getInput()
{
    char input[MAXSTRING] = ""; 

    fgets(input, MAXSTRING, stdin);
    if (input[strlen(input) - 1] == '\n')
    {
        input[strlen(input) - 1] = '\0';
    }

    return input;
}

//this code is derived from our first year C/C++ Programming course in the SET program
int getNum(void)
{
    char record[MAXSTRING] = { 0 }; 
    int number = 0;

    while (number <= 0)
    {
        fgets(record, MAXSTRING, stdin);

        if (sscanf(record, "%d", &number) != 1)
        {
            printf("\nInvalid input. Must be an integer greater than 0. Try again:\n");

            if (number <= 0)
            {
                continue; 
            }
            else
            {
                number = -1;    
            }
        }
    }

    system("cls");
    
    return number;
}