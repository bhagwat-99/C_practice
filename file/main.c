#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main()
{
        FILE *fptr;
        int arr[2];

        if(fptr == NULL)
        {
                printf("ERROR!");
                exit(1);
        }
        while(1)
        {
                for (__uint8_t i=100 ; i>0; i--)
                {
                        fptr = fopen("/tmp/test_file","w");
                        if(fprintf(fptr,"%d",i )<0)
                        {   
                                printf("error writing to file \n");     
                        }
                        fclose(fptr);
                        sleep(2);
                }
        }
        
        return 0;
}