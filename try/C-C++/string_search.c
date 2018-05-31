#include<iostream>
#include<stdio.h>
#include<string>
#include<assert.h>
#include<stdlib.h>

bool string_search(std::string text, std::string search, bool case_sensitive, size_t *p_pos);
bool string_search_char(std::string text, size_t start, char s, size_t *p_pos);
bool substring_equal(std::string str1, size_t start1, std::string str2, size_t start2, size_t size);
void string_lower(std::string *p_str);

void merge_sort(int array[], size_t size);
bool array_sorted_merge(int array1[], size_t size1, int array2[], size_t size2, int *result);
void array_copy(int src[], int des[], size_t size);
void array_dump(int array[], size_t size);

static void merge_sort_(int array[], size_t size, int merge_buffer[]);

int main(void)
{
    size_t pos = 0;
    string_search("this is a Test", "te", false, &pos);
    std::cout << pos << std::endl;

    int a[5] = {1, 2 ,4 ,7, 8};
    int b[5] = {3, 4 ,7 ,8, 9};
    int res[10];
    array_sorted_merge(a, 5, b, 5, res);
    array_dump(a, 5);
    array_dump(b, 5);
    array_dump(res, 10);

    int c[10] = {34, 43, 13, 1, 4, 5, 6, 12 ,35, 1};
    array_dump(c, 10);
    merge_sort(c, 10);
    array_dump(c, 10);

    return 0;
}



bool string_search(std::string text, std::string search, bool case_sensitive, size_t *p_pos)
{
    if (search.empty())
        return false;

    if (!case_sensitive) {
        string_lower(&text);
        string_lower(&search);
    }

    char head_char = search[0];
    for (size_t i = 0; i < text.size(); i++) {
        size_t pos;
        if (!string_search_char(text, i, head_char, &pos)) {
            return false;
        } else if (substring_equal(text, pos, search, 0, search.size())) {
            *p_pos = pos;
            return true;
        }
    }
    return false;
}

bool string_search_char(std::string text, size_t start, char s, size_t *p_pos)
{
    for (size_t i = start; i < text.size(); i++) {
        if (text[i] == s) {
            *p_pos = i;
            return true;
        }
    }
    return false;
}


void string_lower(std::string *p_str)
{
    for (size_t i = 0; i < p_str->size(); i++) {
        if ( 'A' < (*p_str)[i] && (*p_str)[i] < 'Z') {
            (*p_str)[i] = (*p_str)[i] - ('A' - 'a');
        }
    }
}


bool substring_equal(std::string str1, size_t start1, std::string str2, size_t start2, size_t size)
{
    for (size_t i = 0; i < size; i++) {
        bool is_1_end = !(start1 + i < str1.size());
        bool is_2_end = !(start2 + i < str2.size());
        if (is_1_end && is_2_end) {
            return true;
        } else if (is_1_end || is_2_end) {
            return false;
        } else if (str1[start1 + i] != str2[start2 + i]){
            return false;
        }
    }
    return true;
}


void merge_sort(int array[], size_t size)
{
    int *merge_buffer = (int *)malloc(sizeof(*merge_buffer) * size);
    merge_sort_(array, size, merge_buffer);
    free(merge_buffer);
}

static void merge_sort_(int array[], size_t size, int merge_buffer[])
{
    if (size <= 1) {
        return;
    }
    merge_sort_(array, size / 2, merge_buffer);
    merge_sort_(array + size / 2, size - (size / 2), merge_buffer);

    array_sorted_merge(array, size / 2, array + size / 2, size - (size / 2), merge_buffer);
    array_copy(merge_buffer, array, size);
}

bool array_sorted_merge(int array1[], size_t size1, int array2[], size_t size2, int *result)
{
    int *p_result_top = result;
    size_t top1 = 0;
    size_t top2 = 0;

    while (top1 < size1 || top2 < size2) {
        if (!(top2 < size2)) {
            *(p_result_top++) = array1[top1++];
        } else if (!(top1 < size1)) {
            *(p_result_top++) = array2[top2++];
        } else if (array1[top1] <= array2[top2]) {
            *(p_result_top++) = array1[top1++];
        } else if (array2[top2] < array1[top1]) {
            *(p_result_top++) = array2[top2++];
        } else {
            assert("不可能！！！");
        }
        assert((size_t)(p_result_top - result) <= size1 + size2);
    }

    return true;
}

void array_dump(int array[], size_t size)
{
    for (size_t i = 0; i < size; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
}

void array_copy(int src[], int des[], size_t size)
{
    for (size_t i = 0; i < size; i++) {
        des[i] = src[i];
    }
}
