//Example for battery gauge communication using Linux User Space I²C /dev interface
//V1.0
//© 2016 Texas Instruments Inc.
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include "gauge.h"

#define GAUGE_DEVICE_ADDRESS 0xAA

typedef struct
{
        int nI2C;
        unsigned char nAddress;
} TI2C;

int gauge_read(void *pHandle, unsigned char nRegister, unsigned char *pData, unsigned char nLength)
{
        TI2C *pI2C = (TI2C *) pHandle;
        int n;

        if (nLength < 1) return 0;

        pData[0] = nRegister;
        n = write(pI2C->nI2C, pData, 1); // write register address

        n = read(pI2C->nI2C, pData, nLength); // read data from register

        usleep(100);

        return n;
}

int gauge_write(void *pHandle, unsigned char nRegister, unsigned char *pData, unsigned char nLength)
{
        TI2C *pI2C = (TI2C *) pHandle;
        unsigned char pWriteData[nLength + 1];
        int n;

        if (nLength < 1) return 0;

        pWriteData[0] = nRegister; // write register address before writing data
        memcpy(pWriteData + 1, pData, nLength);

        n = write(pI2C->nI2C, pWriteData, nLength + 1);

        usleep(100);

        return n - 1;
}

void gauge_address(void *pHandle, unsigned char nAddress)
{
        TI2C *pI2C = (TI2C *) pHandle;
        if (nAddress != pI2C->nAddress)
                ioctl(pI2C->nI2C, I2C_SLAVE, nAddress >>1);
        pI2C->nAddress = nAddress;
}

void print_data(unsigned char *pData, unsigned int nLength)
{
        unsigned int n;

        printf(" ");
        for (n = 0; n < nLength; n++)
        {
                printf("%02X ", pData[n]);
                if (!((n + 1) % 16)) printf("\n\r ");
        }
        printf("\n\r");
}

#define SOURCE_FILE "test.gm.fs"

#define CMD_VOLTAGE 0x04

#define SUB_CMD_FW_VERSION 0x0002
#define SUB_CMD_CONTROL_STATUS 0x0000

#define DC_STATE 0x52
#define DC_STATE_LENGTH 64
#define DESIGN_CAPACITY 3210 //[mAh]
#define NOMINAL_VOLTAGE 3.7 //[V]
#define DESIGN_ENERGY ((unsigned int) (DESIGN_CAPACITY * NOMINAL_VOLTAGE))
#define TERMINATE_VOLTAGE 3000 //[mV]
#define TAPER_CURRENT 115 //[mA]
#define TAPER_RATE ((unsigned int) (DESIGN_CAPACITY / (0.1 * TAPER_CURRENT)))

int main()
{
        TI2C i2c;
                void *pHandle = (void *) & i2c;
        int nSourceFile;
        struct stat st;
        long n;
        int nSeconds;
        unsigned int nResult;
        char *pFileBuffer;

        unsigned char pData[DC_STATE_LENGTH];

        printf("gauge test\n\r");

        if ((i2c.nI2C = open("/dev/i2c-1", O_RDWR)) <0)
        {
                printf("cannot open I2C bus\n\r");
                exit(1);
        }

        printf("openend I2C bus\n\r");

        gauge_address(pHandle, GAUGE_DEVICE_ADDRESS);

        nResult = gauge_control(pHandle, SUB_CMD_FW_VERSION);
        printf(" FW_VERSION = 0x%04X\n\r", nResult);

        nResult = gauge_cmd_read(pHandle, CMD_VOLTAGE);
        printf(" VOLTAGE = %04d [mV]\n\r", nResult);

        nResult = gauge_control(pHandle, SUB_CMD_CONTROL_STATUS);
        printf(" CONTROL_STATUS = 0x%04X\n\r", nResult);

        stat(SOURCE_FILE, &st);

        printf("source file '%s', size = %d\n\r", SOURCE_FILE, st.st_size);
        if ((nSourceFile = open(SOURCE_FILE, O_RDONLY)) < 0)
        {
                printf("cannot open data classes source file\n\r");
                exit(1);
        }
        pFileBuffer = malloc(st.st_size);
        if (!pFileBuffer) exit(1);
        read(nSourceFile, pFileBuffer, st.st_size);
        close(nSourceFile);

        printf(gauge_execute_fs(pHandle, pFileBuffer));

        free(pFileBuffer);

        //read data class DC_STATE:
        n = gauge_read_data_class(pHandle, DC_STATE, pData, DC_STATE_LENGTH);
        if (n) printf("Error reading data class, %d\n\r", n);

        printf("Data Class 'State' (0x52):\n\r");
        print_data(pData, DC_STATE_LENGTH);

        // this was for bq2742x - change offsets for your gauge
        pData[10] = (DESIGN_CAPACITY & 0xFF00) >> 8;
        pData[11] = DESIGN_CAPACITY & 0xFF;
        pData[12] = (DESIGN_ENERGY & 0xFF00) >> 8;
        pData[13] = DESIGN_ENERGY & 0xFF;
        pData[16] = (TERMINATE_VOLTAGE & 0xFF00) >>8;
        pData[17] = TERMINATE_VOLTAGE & 0xFF;
        pData[27] = (TAPER_RATE & 0xFF00) >> 8;
        pData[28] = TAPER_RATE & 0xFF;

        //write data class DC_STATE:
        gauge_cfg_update(pHandle);
        n = gauge_write_data_class(pHandle, DC_STATE, pData, DC_STATE_LENGTH);
        if (n) printf("Error writing data class, %d\n\r", n);
        gauge_exit(pHandle, SOFT_RESET);
        close(i2c.nI2C);
        printf("closed I2C bus\n\r");
        return 0;
}