#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

#define ARRAY_SIZE(arr) (sizeof(arr)/sizeof(arr[0]))

/* UI page function */
bool login_page(char *login_id);
void function_page(char *login_id);

void function_record_page(char *login_id);
void function_view_page(char *login_id);
void function_about_info_page(void);

void head_page_part(char *login_id);
void foot_page_part(void);

/* input function */
int input_int_by_line(void);
void input_str_by_line(char *str, size_t max);



/* database function */
typedef struct {
    char isbn[21];
    char book_name[21];
    char borrow_date[21];
    char borrow_limit[21];
    bool is_overdue;
}BookRecord;

bool add_record_to_database(char *login_id, const BookRecord* record);

BookRecord* get_record_from_database(char *login_id, size_t* p_size);

#define ID_MAX_LEN 15

const char *database_path = "database";

int main(void)
{
    /* login_id 是用于登录的学号 */
    char login_id[ID_MAX_LEN + 1];
    bool r = login_page(login_id);
    if (r)
        function_page(login_id);
    else
        puts("登录失败！");
    return 0;
}


bool login_page(char *login_id)
{
    head_page_part(NULL);

    puts("您还未登录！请先登录。");
    puts("请输入学号：");

    input_str_by_line(login_id, ID_MAX_LEN + 1);

    return true;
}


void head_page_part(char *login_id)
{
    system("cls");

    if (login_id == NULL)
        printf("========== 图书管理系统 ===========\n");
    else
        printf("========== 图书管理系统(登录用户：%s) ===========\n",  login_id);
}



void function_page(char *login_id)
{
    while (true) {

        head_page_part(login_id);
        puts("温馨提示：请选择对应的功能选单，以使用对应的功能。");
        puts("----------------");

        puts("欢迎！");
        puts("功能选单：");
        puts("\t1. 登记借阅图书");
        puts("\t2. 查看借阅信息");
        puts("\t3. 关于本系统");
        puts("\t4. 退出");


        int select_code = input_int_by_line();

        if (select_code == 1)
            function_record_page(login_id);
        else if (select_code == 2)
            function_view_page(login_id);
        else if (select_code == 3)
            function_about_info_page();
        else if (select_code == 4)
            return;
    }


}
void function_record_page(char *login_id)
{
    head_page_part(login_id);
    puts("您好，您在借阅图书之后，请登记借阅信息。祝您阅读愉快！");
    puts("----------------");
    puts("");

    BookRecord record;
    printf("请注意，最大可输入20个字符，超过的字符会被截断忽略！");
    printf("请输入图书编号：");
    input_str_by_line(record.isbn, ARRAY_SIZE(record.isbn));
    printf("请输入图书书名：");
    input_str_by_line(record.book_name, ARRAY_SIZE(record.book_name));
    printf("请输入借阅时间：");
    input_str_by_line(record.borrow_date, ARRAY_SIZE(record.borrow_date));
    printf("请输入借阅期限：");
    input_str_by_line(record.borrow_limit, ARRAY_SIZE(record.borrow_limit));
    printf("请输入是否过期(yes/no)：");

    char yes_or_no[4];
    input_str_by_line(yes_or_no, 4);
    record.is_overdue = strcmp(yes_or_no, "yes") == 0;

    puts("--------");
    puts("");
    if (add_record_to_database(login_id, &record))
        puts("登记成功！");
    else
        puts("登记失败。请联系程序开发者以获取更多信息。");

    puts("按回车回到菜单。");
    input_str_by_line(NULL, 0);
}

void function_view_page(char *login_id)
{
    head_page_part(login_id);
    puts("以下列出的是您借阅的书籍情况，请查看。祝知识永远伴随着你！");
    puts("-----------------------");
    puts("");

    size_t size;
    BookRecord* records = get_record_from_database(login_id, &size);

    BookRecord** overdue_record = malloc(sizeof(BookRecord*) * size);
    size_t overdue_count = 0;
    size_t i;
    for (i = 0; i < size; i++) {
        if (records[i].is_overdue)
            overdue_record[overdue_count++] = records + i;
    }
    if (overdue_count > 0) {
        puts("");
        printf("提醒：您所借书籍已有%zd本已经超期！\n分别为：", overdue_count);
        for (i = 0; i < overdue_count; i++)
            printf("《%s》", overdue_record[i]->book_name);
        printf("\n请尽快归还。逾期不还者将按规定给予一定的罚金处罚。\n");
    }
    free(overdue_record);

    puts("");
    puts("以下为您的借阅记录：");
    if (records != NULL) {
        for (i = 0; i < size; i++) {
            printf("------ 记录%zd ------\n", i + 1);
            printf("ISBN: %s\n", records[i].isbn);
            printf("书名: 《%s》\n", records[i].book_name);
            printf("借阅时间: %s\n", records[i].borrow_date);
            printf("借阅期限: %s\n", records[i].borrow_limit);
            printf("是否超期: %s\n", records[i].is_overdue ? "是" : "否");
        }


        free(records);
    }


    puts("");
    puts("按回车回到菜单。");
    input_str_by_line(NULL, 0);
}

void function_about_info_page(void)
{
    head_page_part(NULL);
    puts("此程序为图书馆管理而设计，其特点有：");
    puts("\t1. UI友好，功能易用，用户轻松即可上手。");
    puts("\t3. 系统功能简单实用，无冗余。方便使用也方便维护。");
    puts("\t2. 采用无图形界面设计，纯终端操作，内存占用率和CPU资源消耗低到可忽略不计。");
    puts("\t4. 传统管理方法与现代信息技术相结合，提高图书馆工作人员效率。");
    puts("\t5. 本系统为alpha测试1.0版本，可能有些许bug，如有问题请联系程序开发者获取技术支持。");
    puts("\t\t\t\t\t\t ----- power by 神哥");
    puts("");
    puts("按回车回到菜单。");
    input_str_by_line(NULL, 0);
}

bool add_record_to_database(char *login_id, const BookRecord* record)
{
    char file_path[FILENAME_MAX];
    sprintf(file_path, "%s/%s.txt", database_path, login_id);

    FILE* fp = fopen(file_path, "a+b");

    if (fp == NULL) {
        fprintf(stderr, "文件打开失败\n");
        return false;
    }

    fprintf(fp, "%s %s %s %s %d\n", record->isbn, record->book_name, record->borrow_date, record->borrow_limit, (int)record->is_overdue);
    fclose(fp);
    return true;

}

BookRecord* get_record_from_database(char *login_id, size_t *p_size)
{
    char file_path[FILENAME_MAX];
    sprintf(file_path, "%s/%s.txt", database_path, login_id);

    FILE* fp = fopen(file_path, "a+b");
    if (fp == NULL) {
        fprintf(stderr, "文件打开失败\n");
        *p_size = 0;
        return NULL;
    }

    int c;
    size_t line_count = 0;
    while ((c = fgetc(fp)) != EOF)
        if (c == '\n')
            line_count++;
    fseek(fp, 0, SEEK_SET);

    *p_size = line_count;
    if (line_count == 0) {
        fclose(fp);
        return NULL;
    }

    BookRecord *records = malloc(sizeof(BookRecord) * line_count);
    BookRecord *record;
    for (record = records; (size_t)(record - records) < line_count; record++) {
        char line[1024];
        fgets(line, ARRAY_SIZE(line), fp);
        int is_overdue;
        sscanf(line, "%s%s%s%s%d", record->isbn, record->book_name, record->borrow_date, record->borrow_limit, &is_overdue);
        record->is_overdue = (bool)is_overdue;
    }

    fclose(fp);
    return records;
}

int input_int_by_line(void)
{
    char buf[BUFSIZ];
    fgets(buf, BUFSIZ, stdin);

    int int_data = 0;
    sscanf(buf, "%d", &int_data);
    return int_data;
}

void input_str_by_line(char *str, size_t max)
{
    // 凭空吃掉一行
    char buf[BUFSIZ];

    /* 这个函数可能有点小bug ，关于max参数的 */
    fgets(buf, BUFSIZ, stdin);

    if (str == NULL || max == 0) {
        return;
    }

    size_t size = strlen(buf);
    if (buf[size - 1] == '\n')
        buf[size - 1] = '\0';

    memcpy(str, buf, max - 1);
    str[max - 1] = '\0';


}
