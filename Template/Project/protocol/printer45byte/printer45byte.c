#include "printer45byte.h"

/****************************/
/******** start flag ********/
/****************************/

volatile bool flag45byteLabViewPrinter = true;

void enable45byteLabViewPrinter()
{
    flag45byteLabViewPrinter = true;
}
void disable45byteLabViewPrinter()
{
    flag45byteLabViewPrinter = false;
}

/****************************/
/******** set etc data ******/
/****************************/

#define etcDataNum      6
static uint16_t etcDataRaw[etcDataNum] = {0, };

void saveBPM(uint16_t bpmVal)
{
    etcDataRaw[0] = bpmVal;
}

void saveEcgDataErrorCount(uint16_t ecgDataErrorVal)
{
    etcDataRaw[1] = ecgDataErrorVal;
}

void saveMAX30205(uint16_t MAX30205Val)
{
    etcDataRaw[2] = MAX30205Val;
}

void saveEcgThres(uint16_t ecgThresVal)
{
    etcDataRaw[3] = ecgThresVal;
}

void saveMAX30101DataErrorCount(uint16_t MAX30101DataErrorVal)
{
    etcDataRaw[4] = MAX30101DataErrorVal;
}

void saveBMA400DataErrorCount(uint16_t BMA400DataErrorVal)
{
    etcDataRaw[5] = BMA400DataErrorVal;
}

/****************************/
/******** set ecg data ******/
/****************************/

#define ecgRawSampleNum     5			
#define ecgRawVarNum				1
static uint16_t ecgErrorCount = 0;
static uint8_t ecgIndex = 0;
static uint16_t ecgRaw[ecgRawVarNum] = {0, };

void saveEcgADC(uint16_t EcgVal)
{
    if(ecgIndex >= ecgRawSampleNum)
    {
        ecgErrorCount++;
        saveEcgDataErrorCount(ecgErrorCount);
    }
		else
		{
        ecgRaw[ecgIndex] = EcgVal;
    }

    ecgIndex++;
}

/****************************/
/******** set acc data ******/
/****************************/

#define BMA400RawSampleNum    1
#define BMA400RawVarNum       3
static uint8_t BMA400DataErrorCount = 0;
static uint8_t BMA400Index = 0;
static uint16_t BMA400Raw[BMA400RawVarNum] = {0, };

void saveBMA400(int16_t accXVal, int16_t accYVal, int16_t accZVal)
{
    if(BMA400Index >= BMA400RawSampleNum)
    {
        BMA400DataErrorCount++;
        saveBMA400DataErrorCount(BMA400DataErrorCount);
    }
		else
		{
        BMA400Raw[0] = accXVal;
        BMA400Raw[1] = accYVal;
        BMA400Raw[2] = accZVal;
    }

    BMA400Index++;
}

/****************************/
/******** set ppg data ******/
/****************************/

#define MAX30101RawSampleNum    4
#define MAX30101RawVarNum       3
static uint8_t MAX30101DataErrorCount = 0;
static uint8_t MAX30101Index = 0;
static uint16_t MAX30101Raw[MAX30101RawSampleNum][MAX30101RawVarNum] = {0, };

void saveMAX30101(uint16_t RedVal, uint16_t IrVal, uint16_t GreenVal)
{
    if(MAX30101Index >= MAX30101RawSampleNum)
    {
        MAX30101DataErrorCount++;
        saveMAX30101DataErrorCount(MAX30101DataErrorCount);
    }
		else
		{
        MAX30101Raw[MAX30101Index][0] = RedVal;
        MAX30101Raw[MAX30101Index][1] = IrVal;
        MAX30101Raw[MAX30101Index][2] = GreenVal;
    }

    MAX30101Index++;
}


static uint8_t EtcIndex = 0;
void print45byteLabView()
{
    if(flag45byteLabViewPrinter)
		{
				putchar((unsigned char) 0xFE);
				putchar((unsigned char) 45);

				for (int i = 0; i < ecgRawSampleNum; i++)
				{
//						putchar((unsigned char) (ecgRaw[i] >> 8)); 
//						putchar((unsigned char) (ecgRaw[i] >> 0));
						putchar(255); 
						putchar(255);
				}
				ecgIndex = 0;


				for (int i = 0; i < BMA400RawVarNum; i++)
				{
						putchar((unsigned char) (BMA400Raw[i] >> 8));
						putchar((unsigned char) (BMA400Raw[i] >> 0));
				}
				BMA400Index = 0;


				for (int i = 0; i < MAX30101RawSampleNum; i++)
				{
						for (int j = 0; j < MAX30101RawVarNum; j++)
						{
								putchar((unsigned char) (MAX30101Raw[i][j] >> 8));
								putchar((unsigned char) (MAX30101Raw[i][j] >> 0));
						}
				}
				MAX30101Index = 0;


				if (EtcIndex >= 4)
				{
						EtcIndex = 0;
				}
				putchar((unsigned char) EtcIndex);
				putchar((unsigned char) (etcDataRaw[EtcIndex] >> 8));
				putchar((unsigned char) (etcDataRaw[EtcIndex] >> 0));
				EtcIndex++;

				disable45byteLabViewPrinter();
		}
}