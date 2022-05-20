#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

float temp_c,temp_f,humidity,Lux;
char latitude[20],longitude[20],gps_state[20],gps_status[20],altitude[20],satellites[20],gps_time[20];

float read_met_data()
{
        char temp_data1[20];
        char temp_data2[20];
        char temp_data3[20];


        const char * MET_FILE_FORMAT = "{\n\t\"Relative_humidity\":\"%[^\"]\",\n\t\"Temperature(C)\":\"%[^\"]\",\n\t\"Temperature(F)\":\"%[^\"]\"}";

        FILE * file;
        //char *filename = "/etc/entomologist/ento.conf";
        char *filename = "met";
        file = fopen(filename, "r");
        if(file == NULL)
        {       printf("can not open file %s\n",filename);
                exit(1);
        }

        if(fscanf(file,MET_FILE_FORMAT, temp_data1, temp_data2, temp_data3) == EOF)
        {
                printf("Error reading file %s\n",filename);
                exit(1);
        }
        humidity = atof(temp_data1);
        temp_c = atof(temp_data2);
        temp_f = atof(temp_data3);


}

float read_light_data()
{
        char temp_data1[20];

        const char * LIGHT_FILE_FORMAT = "{\n\t\"Light_intensity\":\"%[^\"]\"}";

        FILE * file;
        //char *filename = "/etc/entomologist/ento.conf";
        char *filename = "Light_intensity";
        file = fopen(filename, "r");
        if(file == NULL)
        {       printf("can not open file %s\n",filename);
                exit(1);
        }

        if(fscanf(file,LIGHT_FILE_FORMAT, temp_data1) == EOF)
        {
                printf("Error reading file %s\n",filename);
                exit(1);
        }
        Lux = atof(temp_data1);

}


float read_gps_data()
{

        const char * GPS_FILE_FORMAT = "{\n\t\"time\":\"%[^\"]\",\n\t\"gps_state\":\"%[^\"]\",\n\t\"location\":{\n\t\t\"status\":\"%[^\"]\",\n\t\t\"latitude\":\"%[^\"]\",\n\t\t\"longitude\":\"%[^\"]\",\n\t\t\"altitude\":\"%[^\"]\",\n\t\t\"satellites\":\"%[^\"]\"\n\t\t}\n}";

        FILE * file;
        //char *filename = "/etc/entomologist/ento.conf";
        char *filename = "gps";
        file = fopen(filename, "r");
        if(file == NULL)
        {       printf("can not open file %s\n",filename);
                exit(1);
        }

        if(fscanf(file,GPS_FILE_FORMAT, gps_time, gps_state, gps_status,latitude,longitude,altitude,satellites) == EOF)
        {
                printf("Error reading file %s\n",filename);
                exit(1);
        }


}

int main()
{

        read_met_data();
        read_light_data();
        read_gps_data();

        //printf("humidity : %.2f, temp_c : %.2f, temp_f : %.2f, lux : %.2f\n", humidity, temp_c, temp_f, Lux);
        //printf("latitude : %s, longitude : %s, status : %s, satellites: %s, altitude : %s, time : %s, gps_state : %s\n", latitude, longitude, gps_status, satellites,altitude,gps_time,gps_state);

        return 0;
}