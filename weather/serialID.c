#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// unique serial ID of device
char serial_ID[20];
char file_path[] = "/media/mmcblk1p1/upload/";
char extension[] = ".csv";


time_t now;
struct tm ts;
char time_buf[50];


void date_time()
{
        time ( &now );
        ts = *localtime ( &now );
        strftime(time_buf, sizeof(time_buf), "%a %Y-%m-%d %H:%M:%S %Z", &ts);

}


void serial_id()
{
        char name[20];
        const char * PERSON_FORMAT_IN = "{\n    \"device\": {\n\t\"NAME\": \"%[^\"]\",\n\t\"SERIAL_ID\": \"%[^\"]\",}";

        FILE * file;
        //char *filename = "/etc/entomologist/ento.conf";
        char *filename = "ento.conf";
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

int main(int argc, char *argv[])
{

        serial_id();
        date_time();
        printf("serialID : %s\n",serial_ID);
        printf("timestamp : %s\n",time_buf);

        return 0;
}

