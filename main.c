/*
* FILE          : main.c
* PROJECT       : Relational Databases Assignment 3
* PROGRAMMER    : Bilal Syed
* FIRST VERSION : 2024-11-28
* DESCRIPTION   : This program interacts with a film rental store database through the MYSQL C library to 
*                 perform operations like adding new rentals, updating customer information, viewing
*                 rental history, and deleting customer records. It connects to the MySQL database using 
*                 the password provided by the user and provides a text-based menu for various operations.
*/

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
bool checkEmail(char* email); 
void deleteCustomer(MYSQL* conn); 
void viewRentalHistory(MYSQL* conn); 
bool validateDate(char* date); 

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
            viewRentalHistory(connection); 
            system("pause");  

            break;

        case 4:
            deleteCustomer(connection);  
            system("pause"); 

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

/*
* FUNCTION    : viewRentalHistory()
* DESCRIPTION : This function retrieves and displays rental history for a specific customer
*               within a date range specified by the user.
* PARAMETERS  : MYSQL* conn - The MySQL connection object.
* RETURNS     : void
*/
void viewRentalHistory(MYSQL* conn) 
{
    MYSQL_RES* res = NULL;
    MYSQL_ROW row = NULL;  
    bool validDate = false;
    int id = 0;
    char* input = NULL; 
    char customerId[MAXSTRING] = "";
    char date1[MAXSTRING] = ""; 
    char date2[MAXSTRING] = "";
    char startDate[MAXSTRING] = "'";
    char endDate[MAXSTRING] = "'";
    char query[MAXSTRING] = "SELECT * FROM rental WHERE customer_id =";

    id = getId(1, conn); 
    sprintf(customerId, "%d", id); 
    strcat(query, customerId); 
    strcat(query, " AND rental_date <=");

    while (!validDate) 
    {
        printf("Enter a start date (the latest date you would like to see rentals from): "); 
        input = getInput();  
        strcpy(date1, input);  

        validDate = validateDate(date1);  
        system("cls");
    }
    strcat(startDate, date1); 
    strcat(startDate, "'"); 

    strcat(query, startDate);  
    strcat(query, " AND rental_date >=");

    validDate = false;  

    while (!validDate)
    {
        printf("Enter an end date (the earliest date you would like to see rentals from): ");
        input = getInput();
        strcpy(date1, input);

        validDate = validateDate(date1);
        system("cls");
    }
    strcat(endDate, date1);
    strcat(endDate, "'");

    strcat(query, endDate);
    strcat(query, " ORDER BY rental_date DESC");  

    if (mysql_query(conn, query)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        return;
    } 

    res = mysql_store_result(conn); 

    printf("Here are all the records within your specified date range:\n\n");

    while (row = mysql_fetch_row(res))
    {
        printf("| ID: %s | Rental Date: %s | Inventory ID: %s | Customer ID: %s |\n\n", row[0], row[1], row[2], row[3]);
        system("pause"); 
    }
}

/*
* FUNCTION    : validateDate()
* DESCRIPTION : This function validates the date entered by the user to ensure it is in the format YYYY-MM-DD.
* PARAMETERS  : char* date - The date entered by the user.
* RETURNS     : bool - Returns true if the date is valid and flase if it is invalid.
*/
bool validateDate(char* date) 
{
    if (strlen(date) != 10)
    {
        printf("Invalid date format.\n\n");
        system("pause");

        return false;  
    }

    for (int i = 0; i < strlen(date); i++) 
    {
        if (i == 0 || i == 1 || i == 2 || i == 3)
        {
            if (date[i] != '0' && date[i] != '1' && date[i] != '2' && date[i] != '3' && date[i] != '4' && date[i] != '5' && date[i] != '6' && date[i] != '7' && date[i] != '8' && date[i] != '9')
            {
                printf("Invalid date format.\n\n");
                system("pause");

                return false;
            }
        }

        if (i == 4)
        {
            if (date[i] != '-')
            {
                printf("Invalid date format.\n\n");
                system("pause");

                return false;
            }
        }

        if (i == 5 || i == 6)
        {
            if (date[i] != '0' && date[i] != '1' && date[i] != '2' && date[i] != '3' && date[i] != '4' && date[i] != '5' && date[i] != '6' && date[i] != '7' && date[i] != '8' && date[i] != '9')
            {
                printf("Invalid date format.\n\n");
                system("pause");

                return false;
            }
        }

        if (i == 7)
        {
            if (date[i] != '-')
            {
                printf("Invalid date format.\n\n");
                system("pause");

                return false;
            }
        }

        if (i == 8 || i == 9)
        {
            if (date[i] != '0' && date[i] != '1' && date[i] != '2' && date[i] != '3' && date[i] != '4' && date[i] != '5' && date[i] != '6' && date[i] != '7' && date[i] != '8' && date[i] != '9')
            {
                printf("Invalid date format.\n\n");
                system("pause");

                return false;
            }
        }
    }

    if (date[6] == '0' || date[9] == '0')
    {
        printf("Invalid date format.\n\n");
        system("pause");

        return false;
    }

    return true; 
}

/*
* FUNCTION    : deleteCustomer()
* DESCRIPTION : This function deletes a customer record from the database after ensuring
*               that the customer does not have any incomplete rental transactions. To
*               delete the customer entirely, the customer's entire rental and payment 
*               history must be deleted as well. As both the payment and rental entities 
*               are reliant on the customer entity's existance.
* PARAMETERS  : MYSQL* conn - The MySQL connection object.
* RETURNS     : void
*/
void deleteCustomer(MYSQL* conn)
{
    MYSQL_RES* res = NULL;
    MYSQL_ROW row = NULL;
    int choice = 0;
    int id = 0;
    char customerId[MAXSTRING] = "";
    char query[MAXSTRING] = ""; 

    id = getId(1, conn);
    sprintf(customerId, "%d", id);

    while (true) 
    {
        printf("Are you sure you would like to delete this customer's record? If you do then any rental and payment records associated with the customer will be deleted.\n\t1. Yes\n\t2. No\nEnter a choice (1-2): ");

        choice = getNum();

        switch (choice)
        {
        case 1:
            strcpy(query, "SELECT * FROM rental WHERE return_date IS NULL AND customer_id =");
            strcat(query, customerId); 

            if (mysql_query(conn, query)) { 
                fprintf(stderr, "%s\n", mysql_error(conn)); 
                return; 
            }

            res = mysql_store_result(conn); 

            if (res->row_count > 0)
            {
                printf("The customer you requested to delete cannot be deleted because they have yet to return all of their rented films.\n\n");

                return;
            }

            strcpy(query, "DELETE FROM payment WHERE customer_id =");
            strcat(query, customerId); 

            if (mysql_query(conn, query)) {
                fprintf(stderr, "%s\n", mysql_error(conn));
                return;
            }

            strcpy(query, "DELETE FROM rental WHERE customer_id =");
            strcat(query, customerId); 

            if (mysql_query(conn, query)) { 
                fprintf(stderr, "%s\n", mysql_error(conn)); 
                return;
            }

            strcpy(query, "DELETE FROM customer WHERE customer_id =");
            strcat(query, customerId);

            if (mysql_query(conn, query)) {
                fprintf(stderr, "%s\n", mysql_error(conn));
                return;
            }

            printf("The customer record with ID {%s} and all its associated records have been deleted.\n\n", customerId); 

            return;

        case 2:

            return;

        default:
            printf("Invalid menu selection.\n");
            clearWithEnter();
            break;
        }
    }
}

/*
* FUNCTION    : updateCustomer()
* DESCRIPTION : Updates customer information in the database.
* PARAMETERS  : MYSQL* conn - The connection object for the MySQL database.
* RETURNS     : void
*/
void updateCustomer(MYSQL* conn)
{
    MYSQL_RES* res = NULL;
    MYSQL_ROW row = NULL;
    char* input = NULL;
    int id = 0;
    char temp[MAXSTRING] = "";
    char firstName[MAXSTRING] = "'";
    char lastName[MAXSTRING] = "'";
    char email[MAXSTRING] = "'";
    char addressId[MAXSTRING] = "";
    char customerId[MAXSTRING] = "";
    char query[MAXSTRING] = "UPDATE customer SET first_name =";
    bool matchFormat = false;

    id = getId(1, conn);
    sprintf(customerId, "%d", id);

    printf("Enter a first name: ");
    input = getInput();
    strcat(firstName, input); 
    strcat(firstName, "',");
    system("cls");

    printf("Enter a last name: ");
    input = getInput();
    strcat(lastName, input);
    strcat(lastName, "',"); 
    system("cls");

    while (!matchFormat)
    {
        printf("Enter an email address: ");
        input = getInput();
        strcpy(temp, input); 

        matchFormat = checkEmail(temp);   
        system("cls");
    } 
    strcat(email, temp); 
    strcat(email, "',"); 

    id = getId(4, conn);
    sprintf(addressId, "%d", id);  

    strcat(query, firstName);
    strcat(query, "last_name =");
    strcat(query, lastName); 
    strcat(query, "email =");
    strcat(query, email); 
    strcat(query, "address_id =");
    strcat(query, addressId); 
    strcat(query, " WHERE customer_id =");
    strcat(query, customerId); 

    if (mysql_query(conn, query)) { 
        fprintf(stderr, "%s\n", mysql_error(conn));
        return;
    }

    printf("Update was successful! Here is the updated customer record:\n\n");

    strcpy(query, "SELECT * FROM customer WHERE customer_id =");
    strcat(query, customerId); 

    if (mysql_query(conn, query)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        return;
    }

    res = mysql_store_result(conn); 

    while (row = mysql_fetch_row(res))
    {
        printf("| Customer ID: %s | Store ID: %s | First Name: %s | Last Name: %s | Email: %s | Address ID: %s | Active: %s | Create Date: %s | Last Update: %s |\n\n", row[0], row[1], row[2], row[3], row[4], row[5], row[6], row[7], row[8]);
    }
}

//the code for this function was inspired by this youtube video: https://www.youtube.com/watch?v=ViqyHIyfHYo 
/*
* FUNCTION    : checkEmail()
* DESCRIPTION : Validates an email address based on specific format rules such as
*               checking for the presence of '@' and a valid period after '@',
*               ensuring no invalid characters are at the start or end, and
*               no spaces or special characters appear within the email.
* PARAMETERS  : char* email - a string representing the email to be validated.
* RETURNS     : bool - returns true if the email is valid and false if it is invalid.
*/
bool checkEmail(char* email)
{
    int countAtSymb = 0;
    int saveAtSymbPos = 0;
    char* period = NULL;  

    if (email[0] == ' ' || email[0] == '/' || email[0] == ':'
        || email[0] == ';' || email[0] == '<' || email[0] == '>'
        || email[0] == ',' || email[0] == '[' || email[0] == ']' || email[0] == '@' || email[0] == '.')
    {
        printf("Invalid email format.\n\n");
        system("pause");

        return false; 
    }

    if (email[(strlen(email) - 1)] == ' ' || email[(strlen(email) - 1)] == '/' || email[(strlen(email) - 1)] == ':'
        || email[(strlen(email) - 1)] == ';' || email[(strlen(email) - 1)] == '<' || email[(strlen(email) - 1)] == '>'
        || email[(strlen(email) - 1)] == ',' || email[(strlen(email) - 1)] == '[' || email[(strlen(email) - 1)] == ']' || email[(strlen(email) - 1)] == '@' || email[(strlen(email) - 1)] == '.') 
    {
        printf("Invalid email format.\n\n");
        system("pause");

        return false;
    }

    for (int i = 0; i < strlen(email); i++)
    {
        if (email[i] == '@')
        {
            countAtSymb++;

            if (countAtSymb > 1) 
            {
                printf("Invalid email format.\n\n");
                system("pause");

                return false;
            }

            saveAtSymbPos = i; 
        }

        if (email[i] == ' ' || email[i] == '/' || email[i] == ':'
            || email[i] == ';' || email[i] == '<' || email[i] == '>'
            || email[i] == ',' || email[i] == '[' || email[i] == ']')
        {
            printf("Invalid email format.\n\n"); 
            system("pause"); 

            return false; 
        }

        if (i == (saveAtSymbPos + 1))
        {
            if (email[i] == '.')
            {
                printf("Invalid email format.\n\n");
                system("pause");

                return false;
            }
        }
    }

    period = strchr(email, '.');

    if (period == NULL || period < strchr(email, '@'))  
    {
        printf("Invalid email format.\n\n");
        system("pause");

        return false;
    }

    return true; 
}

/*
* FUNCTION    : addRental()
* DESCRIPTION : Adds a new rental record to the database by retreiving customer,
*               inventory, and staff IDs from the user input. It then checks the 
*               availability of the selected inventory item, and performing the 
*               database insert. It also prints the details of the newly added 
*               rental record.
* PARAMETERS  : MYSQL* conn - a pointer to the MySQL database connection.
* RETURNS     : void
*/
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

/*
* FUNCTION    : getRentalDuration()
* DESCRIPTION : Retrieves and prints the rental duration for a given film
*               based on its inventory ID from the database.
* PARAMETERS  : MYSQL* conn - a pointer to the MySQL database connection.
*               char* inventoryId - a string representing the inventory ID of the film.
* RETURNS     : void
*/
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

/*
* FUNCTION    : checkAvailability()
* DESCRIPTION : Checks if a film's inventory item is available for rent by
*               verifying if there is a current rental record with a NULL
*               return date.
* PARAMETERS  : MYSQL* conn - a pointer to the MySQL database connection.
*               char* inventoryId - a string representing the inventory ID.
* RETURNS     : bool - returns true if the inventory item is available and false if it is unavailable.
*/
bool checkAvailability(MYSQL* conn, char* inventoryId)
{
    MYSQL_RES* res = NULL; 
    MYSQL_ROW row = NULL;  
    char returnDateQuery[MAXSTRING] = "SELECT * FROM rental WHERE return_date IS NULL AND inventory_id =";
    strcat(returnDateQuery, inventoryId);
    strcat(returnDateQuery, " ORDER BY rental_id DESC LIMIT 1");

    mysql_query(conn, returnDateQuery); 

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

/*
* FUNCTION    : getId()
* DESCRIPTION : Retrieves a valid ID from a specified table in the database
*               (customer, inventory, staff, address) and returns the ID.
*               The function prompts the user for input and checks if the
*               record exists in the corresponding table.
* PARAMETERS  : int table - the table number for the query (1=customer, 2=inventory, 3=staff, 4=address).
*               MYSQL* conn - a pointer to the MySQL database connection.
* RETURNS     : int - the ID retrieved from the database.
*/
int getId(int table, MYSQL* conn) 
{
    MYSQL_RES* res = NULL;
    int id = NULL;  
    MYSQL_ROW row = NULL;
    bool exists = false;
    char customerQuery[MAXSTRING] = "SELECT * FROM customer WHERE customer_id=";
    char inventoryQuery[MAXSTRING] = "SELECT * FROM inventory WHERE inventory_id=";
    char staffQuery[MAXSTRING] = "SELECT * FROM staff WHERE staff_id=";
    char addressQuery[MAXSTRING] = "SELECT * FROM address WHERE address_id=";
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

    case 4:
        strcpy(query, addressQuery);
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

        case 4:
            printf("Enter an address ID: ");
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

            case 4:
                strcpy(query, addressQuery);
                printf("An address corresponding with the ID you entered does not exist. Try again...\n\n");
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

/*
* FUNCTION    : startConnection()
* DESCRIPTION : Establishes a connection to the MySQL database using the provided password.
*               If the connection is successful, it returns the connection pointer.
* PARAMETERS  : char* password - the password used for the database connection.
* RETURNS     : MYSQL* - a pointer to the MySQL database connection if successful, NULL if failure.
*/
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

/*
* FUNCTION    : clearWithEnter()
* DESCRIPTION : Pauses the program and clears the screen, providing a clean prompt for the user.
* PARAMETERS  : void
* RETURNS     : void
*/
void clearWithEnter(void)
{
    system("pause");
    system("cls");
}

/*
* FUNCTION    : getInput()
* DESCRIPTION : Reads a line of input from the user, ensuring it is null-terminated.
* PARAMETERS  : void
* RETURNS     : char* - returns the string input by the user.
*/
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
/*
* FUNCTION    : getNum()
* DESCRIPTION : Prompts the user to input a positive integer, validating the input.
*               If the input is not valid, it asks the user to try again.
* PARAMETERS  : void
* RETURNS     : int - the validated positive integer.
*/
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