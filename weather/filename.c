#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

char file_path[] = "/media/mmcblk1p1/upload/";
char file_extension[] = ".csv";
char serial_ID[20];



time_t now;
struct tm ts;
char  time_buf[30];
char time_str[30];

char *filename;


void serial_id()
{
        char name[20];
        const char * PERSON_FORMAT_IN = "{\n    \"device\": {\n\t\"NAME\": \"%[^\"]\",\n\t\"SERIAL_ID\": \"%[^\"]\",}";

        FILE * file;
        //char *filename = "/etc/entomologist/ento.conf";
        char *filename = "/etc/entomologist/ento.conf";
        file = fopen(filename, "r");
        if(file == NULL)
        {       printf("can not open file %s\n",filename);
                exit(1);
        }

        if(fscanf(file,PERSON_FORMAT_IN, name, serial_ID) == EOF)
        {
                printf("Error reading file %s\n",filename);
                exit(1);
        }

}

void date_time_fuc()
{
        time ( &now );
        ts = *localtime ( &now );

        // time format to save in csv file
        strftime(time_buf, sizeof(time_buf), "%a %Y-%m-%d %H:%M:%S %Z", &ts);

        // timestamp for file_name
        strftime(time_str, sizeof(time_str), "weather_%d-%m-%Y_%H_", &ts);
        
}

void filename_func()
{
        date_time_fuc();
        serial_id();

        filename = malloc(strlen(time_str) + strlen(file_path) + strlen(file_extension) + 1);
        if(filename == NULL)
        {
                printf("Error allocating memory for filename\n");
                exit(1);
        }
        filename[0] = '\0';   // ensures the memory is an empty string
        strcat(filename,file_path);
        strcat(filename,time_str);
        strcat(filename,serial_ID);
        strcat(filename,file_extension);

}

int main()
{
        filename_func();
        printf("%s\n",filename);
        //printf("serialID : %s\n",serial_ID);
        return 0;
}