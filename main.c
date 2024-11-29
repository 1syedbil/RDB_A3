#pragma warning (disable: 4996)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <mysql.h>

#define MAXSTRING 200

//prototypes
MYSQL* startConnection(char* password);
void clearWithEnter(void);
char* getInput();

int main(void)
{
    bool loop = true;
    char* input = NULL;
    char password[MAXSTRING] = ""; 

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

    mysql_close(connection); 

    return 0;
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
    printf("\n\tPress Enter to continue");

    char enter[MAXSTRING] = ""; 

    fgets(enter, MAXSTRING, stdin); 

    if (strchr(enter, '\n') != NULL)
    {
        system("cls");
    }
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