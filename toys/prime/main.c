#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>
#include<assert.h>
#include"sqlite3.h"

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

/* ------------------------------------------------------------- */

sqlite3 *database_init(void);
   void  database_prime_table_fill(sqlite3 *db);
    int  database_prime_table_find(sqlite3 *db, unsigned long num);

/* ------------------------------------------------------------- */

void  prime_sieve(char num[], size_t max);
 int  file_exists(const char *path);

/* ------------------------------------------------------------- */

enum {
    ERR_DB = 1,
    ERR_FILE,
};
void report_error(int code, void *data);

/* ------------------------------------------------------------- */

int main(void)
{
    sqlite3 *db = database_init();

    puts("输入一个数：");
    unsigned long num;
    scanf("%lu", &num);

    if(num > UINT_MAX - 1) {
        puts("超出范围");
    } else {
        if(database_prime_table_find(db, num))
            puts("是素数");
        else
            puts("不是素数");
    }

    return 0;
}

sqlite3 *database_init(void)
{
    const char *dbfilePath = "./prime.db"; 
    int first = !file_exists(dbfilePath);

    sqlite3 *db;
    int dbErrCode = sqlite3_open(dbfilePath, &db);    
    if(dbErrCode != SQLITE_OK)
        report_error(ERR_DB, db);

    if(first) {
        puts("提示：数据库为空，将建立数据表。");
        const char *sqlTable = "CREATE TABLE prime(number INT PRIMARY KEY)";
        dbErrCode = sqlite3_exec(db, sqlTable, NULL, NULL, NULL);
        if(dbErrCode != SQLITE_OK)
            report_error(ERR_DB, db);
        database_prime_table_fill(db);
    }
    return db;
}


#define SQL_BUF_LEN 128

void database_prime_table_fill(sqlite3 *db)
{
    //数组太大，栈分配会导致栈溢出
    assert(0 && "");
    char *num = malloc(sizeof(*num) * UINT_MAX);
    memset(num, TRUE, UINT_MAX);
    prime_sieve(num, UINT_MAX);
    free(num);

    size_t i;
    for(i = 2; i < UINT_MAX; i++)
        if(num[i]) {
            char sql[SQL_BUF_LEN];
            sprintf(sql, "INSERT INTO prime VALUES (%lu)", (unsigned long)i);
            int dbErrCode = sqlite3_exec(db, sql, NULL, NULL, NULL);

            if(dbErrCode != SQLITE_OK)
                report_error(ERR_DB, db);
        }
}

int database_prime_table_find(sqlite3 *db, unsigned long num)
{
    char sql[SQL_BUF_LEN];
    sprintf(sql, "SELECT * FROM prime WHERE number = %lu", num);

    char **result; 
    int nRow, nCol;
    int dbErrCode = sqlite3_get_table(db, sql, &result, &nRow, &nCol, NULL);

    if(dbErrCode != SQLITE_OK)
        report_error(ERR_DB, db);

    return nRow > 1;
}


void prime_sieve(char num[], size_t max)
{

    size_t base;
    for(base = 2; base < max; ) {
        size_t i;
        for(i = 2 * base; i < max; i += base)
            num[i] = FALSE;

        for(; base < max; base++)
            if(num[i])
                break;
    }
}

/* ------------------------------------------------------------- */

int file_exists(const char *path)
{
    FILE *fp = fopen(path, "r");
    if(fp) {
        fclose(fp);
        return TRUE;
    }
    return FALSE;
}

void report_error(int code, void *data)
{
    switch(code) {
        case ERR_DB:
            if(data)
                fprintf(stderr, "error:数据库相关错误!错误信息:%s\n", sqlite3_errmsg((sqlite3 *)data));
            else
                fprintf(stderr, "error:数据库相关错误！\n");
            exit(EXIT_FAILURE);
        break;
        case ERR_FILE:
            fprintf(stderr, "error:文件相关错误!\n");
        break;
    }
}
