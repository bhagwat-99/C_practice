#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>


time_t now;
struct tm ts;
char  buf[80];

float read_humidity()
{

        FILE *fptr;
        char line[80];


        fptr = fopen("/tmp/met","r");
        if(fptr == NULL)
        {
                printf("ERROR opening the file met\n");
                return -1;
        }
        
        fseek(fptr,18,0);
        fgets(line,10,fptr);

        fclose(fptr);
        float humidity = atof(line);
 
        return humidity;
}

float read_temp_c()
{
        FILE *fptr;
        char line[80];


        fptr = fopen("/tmp/met","r");
        if(fptr == NULL)
        {
                printf("ERROR opening the file met\n");
                return -1;
        }

        int ch;
        //finding position of new line
        while(ch = fgetc(fptr) != '\n')
        {
               int dummy_var = 0;
        }

        long int pos = ftell(fptr);
        //printf("%ld\n",pos);

        fseek(fptr,pos+15,0);
        fgets(line,10,fptr);
        //printf("%s",line);
        float temp = atof(line);
        return temp;
}

float read_temp_f()
{
        FILE *fptr;
        char line[80];


        fptr = fopen("/tmp/met","r");
        if(fptr == NULL)
        {
                printf("ERROR opening the file met\n");
                return -1;
        }

        int ch;
        //finding position of new line
        while(ch = fgetc(fptr) != '\n')
        {
               int dummy_var = 0;
        }

        //finding position of second new line
        while(ch = fgetc(fptr) != '\n')
        {
               int dummy_var = 0;
        }

        long int pos = ftell(fptr);
        //printf("%ld\n",pos);

        fseek(fptr,pos+15,0);
        fgets(line,10,fptr);
        //printf("%s",line);
        float temp = atof(line);
        return temp;
}


float light_intensity()
{

        FILE *fptr;
        char line[80];


        fptr = fopen("/tmp/light_intensity","r");
        if(fptr == NULL)
        {
                printf("ERROR opening the file met\n");
                return -1;
        }
        
        fseek(fptr,16,0);
        fgets(line,10,fptr);

        fclose(fptr);
        float light_intensity = atof(line);
 
        return light_intensity;
}

char * date_time()
{
        time ( &now );
        ts = *localtime ( &now );
        strftime(buf, sizeof(buf), "%a %Y-%m-%d %H:%M:%S %Z", &ts);

        return buf;
}

int data_write()
{
        FILE * fptr;
        while(1)
        {
                float humidity = read_humidity();
                float temp_c = read_temp_c();
                float lux = light_intensity();
                char * timestamp = date_time();

                fptr = fopen("/media/mmcblk1p1/upload/weather.csv","a");
                if(fptr == NULL)
                {
                        printf("ERROR opening the file weather\n");
                        return -1;
                }

                fprintf(fptr,"%s, %.2f, %.2f, %.2f\n", timestamp, humidity, temp_c, lux);

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
        char * filename = "/media/mmcblk1p1/upload/weather.csv";
       
        if(checkIfFileExists(filename))
        {
                // open file for read
                FILE * fptr = fopen(filename,"w");
                if(fptr == NULL)
                {
                        printf("ERROR creating the file weather.csv\n");
                        return -1;
                }

                fprintf(fptr,"Date_time, Relative_Humidity(%%), Temperature(C), Light_intensity(Lux)\n");

                fclose(fptr);

        }

        data_write();

        return 0;
}