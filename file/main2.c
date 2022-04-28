#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main()
{
        FILE *fptr;
        int num ;

        if(fptr == NULL)
        {
                printf("ERROR!");
                exit(1);
        }
        while(1)
        {
                fptr = fopen("/tmp/test_file","r");
                fscanf(fptr,"%d", &num);
                printf("Value of n = %d \n", num);
                fclose(fptr); 
                sleep(3);
                
        }
        
        return 0;
}