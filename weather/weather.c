#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define MET_RAM_FILE_PATH "/tmp/met"
#define LIGHT_RAM_FILE_PATH "/tmp/light_intensity"
#define GPS_FILE_PATH "/tmp/gps"
#define CONF_FILE_PATH "/etc/entomologist/ento.conf"

#define RAM_FILE_MODE "r"


char file_path[] = "/media/mmcblk1p1/upload/";
char file_extension[] = ".csv";
char serial_ID[20];



time_t now;
struct tm ts;
char  time_buf[30];
char time_str[30];

char *csv_filename;

float temp_c = 0,temp_f = 0,humidity = 0,Lux = 0;
char latitude[20] = {0},longitude[20]={0},gps_state[20]={0},gps_status[20]={0},altitude[20]={0},satellites[20]={0},gps_time[20]={0};


void serial_id()
{
        char name[20];
        const char * SERIAL_ID_FORMAT = "{\n    \"device\": {\n\t\"NAME\": \"%[^\"]\",\n\t\"SERIAL_ID\": \"%[^\"]\",}";

        FILE * file;
        char *filename = CONF_FILE_PATH;
        //char *filename = "ento.conf";
        file = fopen(filename, RAM_FILE_MODE);
        if(file == NULL)
        {       printf("can not open file %s\n",filename);
                exit(1);
        }

        if(fscanf(file,SERIAL_ID_FORMAT, name, serial_ID) == EOF)
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

        csv_filename = malloc(strlen(time_str) + strlen(file_path) + strlen(file_extension) + 1);
        if(csv_filename == NULL)
        {
                printf("Error allocating memory for filename\n");
                exit(1);
        }
        csv_filename[0] = '\0';   // ensures the memory is an empty string
        strcat(csv_filename,file_path);
        strcat(csv_filename,time_str);
        strcat(csv_filename,serial_ID);
        strcat(csv_filename,file_extension);

}

float read_met_data()
{
        char temp_data1[20];
        char temp_data2[20];
        char temp_data3[20];


        const char * MET_FILE_FORMAT = "{\n\t\"Relative_humidity\":\"%[^\"]\",\n\t\"Temperature(C)\":\"%[^\"]\",\n\t\"Temperature(F)\":\"%[^\"]\"}";

        FILE * file;
        char *filename = MET_RAM_FILE_PATH;
        //char *filename = "met";
        file = fopen(filename,RAM_FILE_MODE);
        if(file == NULL)
        {       printf("can not open file %s\n",filename);
                //exit(1);
        }

        if(fscanf(file,MET_FILE_FORMAT, temp_data1, temp_data2, temp_data3) == EOF)
        {
                printf("Error reading file %s\n",filename);
                //exit(1);
        }
        humidity = atof(temp_data1);
        temp_c = atof(temp_data2);
        temp_f = atof(temp_data3);


}


float read_gps_data()
{

        const char * GPS_FILE_FORMAT = "{\n\t\"time\":\"%[^\"]\",\n\t\"gps_state\":\"%[^\"]\",\n\t\"location\":{\n\t\t\"status\":\"%[^\"]\",\n\t\t\"latitude\":\"%[^\"]\",\n\t\t\"longitude\":\"%[^\"]\",\n\t\t\"altitude\":\"%[^\"]\",\n\t\t\"satellites\":\"%[^\"]\"\n\t\t}\n}";

        FILE * file;
        //char *filename = "/etc/entomologist/ento.conf";
        char *filename = GPS_FILE_PATH;
        file = fopen(filename, "r");
        if(file == NULL)
        {       printf("can not open file %s\n",filename);
                //exit(1);
        }

        if(fscanf(file,GPS_FILE_FORMAT, gps_time, gps_state, gps_status,latitude,longitude,altitude,satellites) == EOF)
        {
                printf("Error reading file %s\n",filename);
                //exit(1);
        }


}

float read_light_data()
{
        char temp_data1[20];

        const char * LIGHT_FILE_FORMAT = "{\n\t\"Light_intensity\":\"%[^\"]\"}";

        FILE * file;
        char *filename = LIGHT_RAM_FILE_PATH;
        file = fopen(filename, "r");
        if(file == NULL)
        {       printf("can not open file %s\n",filename);
                //exit(1);
        }

        if(fscanf(file,LIGHT_FILE_FORMAT, temp_data1) == EOF)
        {
                printf("Error reading file %s\n",filename);
                //exit(1);
        }
        Lux = atof(temp_data1);

}

int data_write()
{
        FILE * fptr;
        while(1)
        {
                read_light_data();
                read_gps_data();
                read_met_data();

                // time data
                date_time_fuc();

                fptr = fopen(csv_filename,"a");
                if(fptr == NULL)
                {
                        printf("ERROR opening the file %s\n",csv_filename);
                        exit(1);
                }

                fprintf(fptr,"%s, %.2f, %.2f, %.2f, %s, %s\n", time_buf, humidity, temp_c, Lux, latitude, longitude);

                fclose(fptr);


                sleep(60);
        }
        return 0;
}

int checkIfFileExists(const char * filename)
{
    FILE *file;
    if (file = fopen(filename, "r"))
    {
        fclose(file);
        return 0;
    }

    return 1;
}

int main()
{
        filename_func();
       
        if(checkIfFileExists(csv_filename))
        {
                // open file for read
                FILE * fptr = fopen(csv_filename,"w");
                if(fptr == NULL)
                {
                        printf("ERROR creating the file %s\n",csv_filename);
                        exit(1);
                }

                fprintf(fptr,"Date_time, Relative_Humidity(%%), Temperature(C), Light_intensity(Lux), Latitude, Longitude\n");

                fclose(fptr);

        }

        data_write();

        return 0;
}