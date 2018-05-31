#include<stdio.h>
#include<string.h>
#include<stdlib.h>

/*


  时间限制：1秒

  空间限制：32768K
  给出两个字符串（可能包含空格）,找出其中最长的公共连续子串,输出其长度。
  输入描述:

  输入为两行字符串（可能包含空格），长度均小于等于50.



  输出描述:

  输出为一个整数，表示最长公共连续子串的长度。


  输入例子1:

  abcde
  abgde


  输出例子1:

  2


 */
#include<stdio.h>
#include<string.h>
#include<stdlib.h>


int str_common_count(char* str1, char* str2);
int long_common_substring(char* str1, char* str2);
int long_common_substring_dp(char* str1, char* str2);


int main(void)
{
    #define MAX_LEN 50
    char str1[MAX_LEN + 1];
    char str2[MAX_LEN + 1];
    fgets(str1, sizeof(str1), stdin);
    fgets(str2, sizeof(str2), stdin);

    str1[strlen(str1) - 1] = '\0';
    str2[strlen(str2) - 1] = '\0';

    printf("%d\n", long_common_substring_dp(str1, str2));
    return 0;
}


int long_common_substring(char* str1, char* str2) {
    if (str1[0] == '\0' || str2[0] == '\0') {
        return 0;
    } else {
        /* str1[0] != str2[0]时， count = 0 */
        int count = str_common_count(str1, str2);
        int len1 = long_common_substring(str1 + 1, str2);
        int len2 = long_common_substring(str1, str2 + 1);
        int max = len1 > len2 ? len1 : len2;
        max = count > max ? count : max;
        return max;
    }
}


int long_common_substring_dp(char* str1, char* str2) {
    int len1 = strlen(str1);
    int len2 = strlen(str2);

    int* table = (int*) malloc(sizeof(int) * (len1 + 1) * (len2 + 1));
#define GET(i, j) table[(i) * (len2 + 1) + (j)]

    for (int i = len1; i >= 0; i--) {
        for (int j = len2; j >= 0; j--) {
            if (i == len1 || j == len2) {
                GET(i, j) = 0;
            } else {
                int count = str_common_count(str1 + i, str2 + j);
                int r1 = GET(i + 1, j);
                int r2 = GET(i, j + 1);
                int max = r1 > r2 ? r1 : r2;
                max = count > max ? count : max;
                GET(i, j) = max;
            }
        }
    }
    return GET(0, 0);
}

int str_common_count(char* p1, char* p2) {
    int count = 0;
    while (*p1 != '\0' && *p2 != '\0' && *p1 == *p2) {
        count++;
        p1++;
        p2++;
    }
    return count;
}
