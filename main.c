#pragma warning (disable: 4996)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <mysql.h>

#define MAXSTRING 200

int main(void)
{
    MYSQL* conn;
    MYSQL_RES* res;
    MYSQL_ROW row;

    char* server = "127.0.0.1";
    char* user = "root";
    char* database = "sakila";

    char password[MAXSTRING] = ""; 

    printf("Enter the password: ");

    fgets(password, (MAXSTRING - 1), stdin);
    if (password[strlen(password) - 1] == '\n')
    {
        password[strlen(password) - 1] = '\0';
    }

    conn = mysql_init(NULL);

    if (!mysql_real_connect(conn, server, 
        user, password, database, 0, NULL, 0)) { 
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }

    if (mysql_query(conn, "show tables")) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }

    res = mysql_use_result(conn);

    printf("MySQL Tables in mysql database:\n\n");

    while ((row = mysql_fetch_row(res)) != NULL)
    {
        printf("%s \n", row[0]); 
    }

    mysql_free_result(res); 
    mysql_close(conn);
}