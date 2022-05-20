#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

char latitude[20],longitude[20],gps_state[20],gps_status[20],altitude[20],satellites[20],gps_time[20];

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

        read_gps_data();

        printf("latitude : %s, longitude : %s, status : %s, satellites: %s, altitude : %s, time : %s, gps_state : %s\n", latitude, longitude, gps_status, satellites,altitude,gps_time,gps_state);
        //printf("%s\n",time_data);
        return 0;
}