#include "math_custom.h"
#include <stdio.h>

float add(float num1, float num2)
{
        return (num1 + num2);
}

float substract(float num1, float num2)
{
        if(num1<num2)
        {
        return (num2 - num1);
        }
        else return (num1-num2);
}

float multiply(float num1, float num2)
{
        return num1*num2;
}

float divide(float num1, float num2)
{
        if(num1<num2)
        {
        return (num2/num1);
        }
        else return (num1/num2);
}

void calculate(float num1,float num2, float (*math_fuction)(float,float))
{
        printf("Result = %f\n",math_fuction(num1,num2));
}
