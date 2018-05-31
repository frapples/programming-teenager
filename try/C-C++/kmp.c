#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

const char* string_search(const char* str, const char* search);
const char* string_search2(const char* str, const char* search);
const char* string_search_kmp(const char* str, const char* search);

int main(void)
{
    srand(time(NULL));

    const char* s = "abcd";
    assert(string_search(s, "a") == s + 0);

    s = "abcdbcd";
    assert(string_search(s, "bcdbc") == s + 1);

    s = "abcd";
    assert(string_search(s, "f") == NULL);

    s = "abcd";
    assert(string_search(s, "") == NULL);


    s = "abcd";
    assert(string_search2(s, "a") == s + 0);

    s = "abcdbcd";
    assert(string_search2(s, "bcdbc") == s + 1);

    s = "abcd";
    assert(string_search2(s, "f") == NULL);

    s = "abcd";
    assert(string_search2(s, "") == NULL);


    s = "abcd";
    assert(string_search_kmp(s, "a") == s + 0);

    s = "abcdbcd";
    assert(string_search_kmp(s, "bcdbc") == s + 1);

    s = "abcd";
    assert(string_search_kmp(s, "f") == NULL);

    s = "abcd";
    assert(string_search_kmp(s, "") == NULL);

    return 0;
}

char* random_str(int max_size) {
    int search_size = rand() % max_size;
    char* s = malloc(sizeof(char) * (search_size + 1));
    for (int i = 0; i < search_size; i++) {
        s[i] = rand() % ('z' - 'a') + 'a';
    }
    s[search_size] = '\0';
    return s;
}

const char* string_search(const char* str, const char* search) {
    const char* i;
    for (i = str; *i != '\0'; i++) {
        if (*i == search[0]) {
            const char* p_str = i;
            const char* p_search = search;

            while (*p_search != '\0' && *p_str != '\0' && *p_search == *p_str) {
                p_search++;
                p_str++;
            }

            if (*p_search == '\0') {
                return i;
            }
        }
    }
    return NULL;
}


const char* string_search2(const char* str, const char* search) {
    /* 朴素想法的另一种实现 */

    const char* i = str;
    const char* j = search;
    while (*i != '\0' && *j != '\0') {

        assert(*j != '\0');

        if (*i == *j) {
            if (*(j + 1) == '\0') {
                return i - (j - search);
            }

            i++, j++;
        } else {
            i = (i - (j - search)) + 1;
            j = search;
        }
    }
    return NULL;
}

const char* string_search_kmp(const char* str, const char* search)
{
    int search_size = strlen(search);
    int* next = malloc(sizeof(int) * search_size);

    /* next 数组是str字符串固有的性质 */
    next[0] = -1;
    int first = -1;
    int end = 0;
    while (end < search_size - 1) {
        if (first == -1 || search[first] == search[end]) {
            first++, end++;
            next[end] = first;
        } else {
            first = next[first];
        }
    }

    /* 利用next数组，子串匹配失败时有效利用前面的结果 */
    const char* i = str;
    const char* j = search;
    while (*i != '\0' && j != '\0') {
        if (*i == *j) {
            if (*(j + 1) == '\0') {
                free(next);
                return i - (j - search);
            }
            i++, j++;
        } else {
            if (j - search == 0) {
                i++;
            } else {
                j = search + next[j - search];
            }
        }
    }

    free(next);
    return NULL;
}
