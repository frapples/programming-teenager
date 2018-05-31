#include<stdio.h>

#include <apr_pools.h>
#include <apr_version.h>
#include <apr_strings.h>
#include <apr_tables.h>

void demo_apr_string(apr_pool_t *mp);
void demo_apr_array(apr_pool_t *mp);

int main(int argc, char *argv[])
{

    printf("apr版本：%s\n", apr_version_string());

    apr_initialize();
    apr_pool_t *mp;
    apr_status_t rv = apr_pool_create(&mp, NULL);

    char errbuf[256];
    apr_strerror(rv, errbuf, sizeof(errbuf));
    printf("%s\n", errbuf);

    if (rv != APR_SUCCESS) {
        printf("内存池分配失败");
        return 0;
    }

    int *p_n = apr_palloc(mp, sizeof(int));
    *p_n = 1;
    printf("%d\n", *p_n);

    demo_apr_string(mp);
    demo_apr_array(mp);


    apr_pool_destroy(mp);
    apr_terminate();
    return 0;
}


void demo_apr_string(apr_pool_t *mp)
{
    char *str = apr_pstrcat(mp, "this", "is", "a", "string", NULL);
    printf("%s\n", str);

    str = apr_psprintf(mp, "%s %s %s", "this", "is", "a", "string");
    printf("%s\n", str);

}

void demo_apr_array(apr_pool_t *mp)
{
    apr_array_header_t *arr;
    arr = apr_array_make(mp, 10, sizeof(int));
    APR_ARRAY_IDX(arr, 0, int) = 1;
    APR_ARRAY_PUSH(arr, int) = 10;
    APR_ARRAY_PUSH(arr, int) = 20;
    APR_ARRAY_PUSH(arr, int) = 30;

    printf("%d\n", APR_ARRAY_IDX(arr, 0, int));

    size_t i;
    for (i = 0; i < arr->nelts; i++) {
        printf("%d: %d\n", i, APR_ARRAY_IDX(arr, i, int));
}
}


