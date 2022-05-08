#include <stdio.h>


typedef unsigned short uint16_t;
typedef unsigned char uint8_t;
typedef char int8_t;

int a = -2;

int test_func()
{
    return 0xff;
    printf("in test_func\n");
}

int main()
{
    uint8_t  p_ret_val = test_func();
    // if(p_ret_val == -1){
    // printf("Hello\n");
    // }
    printf("a = %d\n",a);
    printf("hex = %d\n",p_ret_val);
    return 0;
}