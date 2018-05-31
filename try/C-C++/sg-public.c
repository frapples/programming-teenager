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
    /* login_id �����ڵ�¼��ѧ�� */
    char login_id[ID_MAX_LEN + 1];
    bool r = login_page(login_id);
    if (r)
        function_page(login_id);
    else
        puts("��¼ʧ�ܣ�");
    return 0;
}


bool login_page(char *login_id)
{
    head_page_part(NULL);

    puts("����δ��¼�����ȵ�¼��");
    puts("������ѧ�ţ�");

    input_str_by_line(login_id, ID_MAX_LEN + 1);

    return true;
}


void head_page_part(char *login_id)
{
    system("cls");

    if (login_id == NULL)
        printf("========== ͼ�����ϵͳ ===========\n");
    else
        printf("========== ͼ�����ϵͳ(��¼�û���%s) ===========\n",  login_id);
}



void function_page(char *login_id)
{
    while (true) {

        head_page_part(login_id);
        puts("��ܰ��ʾ����ѡ���Ӧ�Ĺ���ѡ������ʹ�ö�Ӧ�Ĺ��ܡ�");
        puts("----------------");

        puts("��ӭ��");
        puts("����ѡ����");
        puts("\t1. �Ǽǽ���ͼ��");
        puts("\t2. �鿴������Ϣ");
        puts("\t3. ���ڱ�ϵͳ");
        puts("\t4. �˳�");


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
    puts("���ã����ڽ���ͼ��֮����Ǽǽ�����Ϣ��ף���Ķ���죡");
    puts("----------------");
    puts("");

    BookRecord record;
    printf("��ע�⣬��������20���ַ����������ַ��ᱻ�ضϺ��ԣ�");
    printf("������ͼ���ţ�");
    input_str_by_line(record.isbn, ARRAY_SIZE(record.isbn));
    printf("������ͼ��������");
    input_str_by_line(record.book_name, ARRAY_SIZE(record.book_name));
    printf("���������ʱ�䣺");
    input_str_by_line(record.borrow_date, ARRAY_SIZE(record.borrow_date));
    printf("������������ޣ�");
    input_str_by_line(record.borrow_limit, ARRAY_SIZE(record.borrow_limit));
    printf("�������Ƿ����(yes/no)��");

    char yes_or_no[4];
    input_str_by_line(yes_or_no, 4);
    record.is_overdue = strcmp(yes_or_no, "yes") == 0;

    puts("--------");
    puts("");
    if (add_record_to_database(login_id, &record))
        puts("�Ǽǳɹ���");
    else
        puts("�Ǽ�ʧ�ܡ�����ϵ���򿪷����Ի�ȡ������Ϣ��");

    puts("���س��ص��˵���");
    input_str_by_line(NULL, 0);
}

void function_view_page(char *login_id)
{
    head_page_part(login_id);
    puts("�����г����������ĵ��鼮�������鿴��ף֪ʶ��Զ�������㣡");
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
        printf("���ѣ��������鼮����%zd���Ѿ����ڣ�\n�ֱ�Ϊ��", overdue_count);
        for (i = 0; i < overdue_count; i++)
            printf("��%s��", overdue_record[i]->book_name);
        printf("\n�뾡��黹�����ڲ����߽����涨����һ���ķ��𴦷���\n");
    }
    free(overdue_record);

    puts("");
    puts("����Ϊ���Ľ��ļ�¼��");
    if (records != NULL) {
        for (i = 0; i < size; i++) {
            printf("------ ��¼%zd ------\n", i + 1);
            printf("ISBN: %s\n", records[i].isbn);
            printf("����: ��%s��\n", records[i].book_name);
            printf("����ʱ��: %s\n", records[i].borrow_date);
            printf("��������: %s\n", records[i].borrow_limit);
            printf("�Ƿ���: %s\n", records[i].is_overdue ? "��" : "��");
        }


        free(records);
    }


    puts("");
    puts("���س��ص��˵���");
    input_str_by_line(NULL, 0);
}

void function_about_info_page(void)
{
    head_page_part(NULL);
    puts("�˳���Ϊͼ��ݹ������ƣ����ص��У�");
    puts("\t1. UI�Ѻã��������ã��û����ɼ������֡�");
    puts("\t3. ϵͳ���ܼ�ʵ�ã������ࡣ����ʹ��Ҳ����ά����");
    puts("\t2. ������ͼ�ν�����ƣ����ն˲������ڴ�ռ���ʺ�CPU��Դ���ĵ͵��ɺ��Բ��ơ�");
    puts("\t4. ��ͳ���������ִ���Ϣ�������ϣ����ͼ��ݹ�����ԱЧ�ʡ�");
    puts("\t5. ��ϵͳΪalpha����1.0�汾��������Щ��bug��������������ϵ���򿪷��߻�ȡ����֧�֡�");
    puts("\t\t\t\t\t\t ----- power by ���");
    puts("");
    puts("���س��ص��˵���");
    input_str_by_line(NULL, 0);
}

bool add_record_to_database(char *login_id, const BookRecord* record)
{
    char file_path[FILENAME_MAX];
    sprintf(file_path, "%s/%s.txt", database_path, login_id);

    FILE* fp = fopen(file_path, "a+b");

    if (fp == NULL) {
        fprintf(stderr, "�ļ���ʧ��\n");
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
        fprintf(stderr, "�ļ���ʧ��\n");
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
    // ƾ�ճԵ�һ��
    char buf[BUFSIZ];

    /* ������������е�Сbug ������max������ */
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
