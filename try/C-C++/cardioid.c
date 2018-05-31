#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include <apr_tables.h>

#include <assert.h>

typedef struct {
    char* data;
    int height;
    int width;
}buffer_t;

typedef struct {
    double x;
    double y;
}point_t;

void buffer_init(buffer_t* buf, int height, int width);
/* 左上角(0, 0)，横向x轴纵向y轴 */
void buffer_set(buffer_t* buf, int x, int y, char c);
char buffer_get(buffer_t* buf, int x, int y);
void buffer_show(buffer_t* buf);
void buffer_clear(buffer_t* buf);
void buffer_destory(buffer_t* buf);

void draw(buffer_t* buf, apr_array_header_t* arr);
void reverse_by_y(buffer_t* buf);

double cardioid(double theta);

double min(double n1, double n2);

int main(void)
{
    apr_initialize();

    apr_pool_t *mp;
    apr_status_t rv = apr_pool_create(&mp, NULL);


    buffer_t buf;
    buffer_init(&buf, 30, 60);


    apr_array_header_t* arr = apr_array_make(mp, 1, sizeof(point_t));


    double step = M_PI / 100;
    for (double theta = 0; theta <= 2 * M_PI; theta += step) {
        double r = cardioid(theta);

        point_t p;
        p.x = r * cos(theta);
        p.y = r * sin(theta);

        APR_ARRAY_PUSH(arr, point_t) = p;
    }

    draw(&buf, arr);

    buffer_show(&buf);
    buffer_destory(&buf);

    apr_terminate();
    return 0;
}

double cardioid(double theta)
{
    return 12 * (1 - sin(theta));
}

void draw(buffer_t* buf, apr_array_header_t* arr)
{
    double x_min = APR_ARRAY_IDX(arr, 1, point_t).x;
    double y_min = APR_ARRAY_IDX(arr, 1, point_t).y;
    for (int i = 0; i < arr->nelts; i++) {
        point_t p = APR_ARRAY_IDX(arr, i, point_t);
        x_min = min(x_min, p.x);
        y_min = min(y_min, p.y);
    }

    for (int i = 0; i < arr->nelts; i++) {
        point_t p = APR_ARRAY_IDX(arr, i, point_t);
        p.x = x_min < 0 ? p.x + fabs(x_min) : p.x;
        p.y = y_min < 0 ? p.y + fabs(y_min) : p.y;

        APR_ARRAY_IDX(arr, i, point_t) = p;
    }


    for (int i = 0; i < arr->nelts; i++) {
        point_t p = APR_ARRAY_IDX(arr, i, point_t);
        buffer_set(buf, (int)p.x, (int)p.y, '*');
    }

    reverse_by_y(buf);
}


void reverse_by_y(buffer_t* buf)
{
    for (int y = 0; y < buf->height / 2; y++) {
        for (int x = 0; x < buf->width; x++) {
            int y_ = (buf->height - 1) - y;
            char tmp  = buffer_get(buf, x, y);
            buffer_set(buf, x, y, buffer_get(buf, x, y_));
            buffer_set(buf, x, y_, tmp);
        }
    }
}

/* ------------------- */

double min(double n1, double n2)
{
    return n1 < n2 ? n1 : n2;
}



/* ------------------------- */
void buffer_init(buffer_t* buf, int height, int width)
{
    buf->height = height;
    buf->width = width;
    buf->data = malloc(sizeof(char) * height * width);
    buffer_clear(buf);
}

void buffer_set(buffer_t* buf, int x, int y, char c)
{
    if (x >= 0 && x < buf->width && y >= 0 && y < buf->height) {
        *(buf->data + (y * buf->width + x)) = c;
    } else {
        puts("越界! \n");
    }
}
char buffer_get(buffer_t* buf, int x, int y)
{
    if (x >= 0 && x < buf->width && y >= 0 && y < buf->height) {
            return *(buf->data + (y * buf->width + x));
        } else {
            return '0';
        }
}

void buffer_show(buffer_t* buf)
{
    int x, y;
    for (y = 0; y < buf->height; y++) {
        for (x = 0; x < buf->width; x++) {
            putchar(buffer_get(buf, x, y));
        }
        putchar('\n');
    }
}
void buffer_clear(buffer_t* buf)
{
    int x, y;
    for (y = 0; y < buf->height; y++) {
        for (x = 0; x < buf->width; x++) {
            buffer_set(buf, x, y, ' ');
        }
    }
}

void buffer_destory(buffer_t *buf)
{
    free(buf->data);
    buf->width = 0;
    buf->height = 0;
}
