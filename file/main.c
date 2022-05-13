#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>


int read_file(char * filename, char * mode)
{
        // open file for read
        FILE *fptr;

        fptr = fopen(filename,mode);
        if(fptr == NULL)
        {
                printf("ERROR opening the file %s \n",filename);
                return -1;
        }

        // open file for write
        FILE *fptr_write;

        fptr_write = fopen("weather","a");
        if(fptr == NULL)
        {
                printf("ERROR opening the file weather \n");
                return -1;
        }

        // // reading line by line, max 256 bytes
        // const unsigned MAX_LENGTH = 256;
        // char buffer[MAX_LENGTH];

        // while (fgets(buffer, MAX_LENGTH, fptr))
        //         printf("%s", buffer);
        // printf("\n");

        char ch;
        while ((ch = fgetc(fptr)) != EOF)
                //putchar(ch);
                fputc(ch,fptr_write);

        //printf("\n");
        
        // if(fprintf(fptr,"%d",i ) < 0 )
        // {   
        //         printf("error writing to file \n");    
        // }

        if(fclose(fptr) == EOF)
        {
                printf("error closing file\n");
                return -1;
        }
        if(fclose(fptr_write) == EOF)
        {
                printf("error closing file\n");
                return -1;
        }
}


int main()
{
        FILE *fptr;
        char * filename;
        char * mode;

        time_t now;
        struct tm ts;
        char  buf[80];

        while(1)
        {

                // open file for read
                fptr = fopen("weather","a");
                if(fptr == NULL)
                {
                        printf("ERROR opening the file weather\n");
                        return -1;
                }

                time ( &now );
                ts = *localtime ( &now );
                strftime(buf, sizeof(buf), "%a %Y-%m-%d %H:%M:%S %Z", &ts);
                //fprintf("%s   ", buf);
                fprintf(fptr,"%s  ", buf);

                fclose(fptr);
                
                

                // read met file
                filename = "met";
                mode = "r";
                if(read_file(filename, mode) == -1)
                {
                        printf("error read file\n");
                        return -1;
                }


                // read light intensity file
                filename = "light_intensity";
                mode = "r";
                if(read_file(filename, mode) == -1)
                {
                        printf("error read file\n");
                        return -1;
                }
                sleep(10);
        }        
        return 0;
}