#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

int main()
{
        FILE * fptr;
        // open file for read
        fptr = fopen("weather.csv","w+");
        if(fptr == NULL)
        {
                printf("ERROR opening the file weather\n");
                return -1;
        }
        fprintf(fptr,"c1, c2, c3, c4\n");
        fprintf(fptr,"%d, %s, %s, %s\n", 1, "bhagwat", "shinde", "fdd");
        fprintf(fptr,"%d, %s, %s, %s\n", 2, "aniket", "shinde", "fdffd");

        fclose(fptr);
}