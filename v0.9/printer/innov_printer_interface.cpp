#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "innov_printer_interface.h"
#include "serialbase.h"

static int prt_fd = -1;

int open_printer(void)
{
	prt_fd = OpenComPort(1, 9600, 8,  "1", 'O');
	if(prt_fd < 0)
	{		
		printf("Can not open the serial port\n");		
		return -1;	
	}
    return prt_fd;
}
int close_printer(void)
{
    if(prt_fd != -1)
        CloseComPort(prt_fd);
    return 0;
}

int write_printer(unsigned char *buf, int len)
{
    int ret = -1;
    ret = WriteComPort (prt_fd, buf, len);
    return ret;
}


unsigned char MPT_Char_Cmd[3] = {0x1B, 0x21, 0x31} ; //�Ŵ�����  ��������ֲ�11ҳ  ����16*16 ����8*16

unsigned char MPT_Init_Cmd[2] = {0x1B, 0x40}; //��ʼ����ӡ��  �����ӡ���ڲ��� buffer

// ���ô�ӡ����ʼ������
extern void MPT_INIT(void);
extern int Dec2String(char *psrc, int val);
extern int read_tired_driving(char *pBuf, int Num, StruInnovPrinterContent *content);
extern char *Time2String(const time_t timep, char *strDest);

void MPT_INIT(void)
{
    open_printer();

    write_printer(MPT_Init_Cmd, 2);

    write_printer(MPT_Char_Cmd, 3);

    close_printer();

}

int StartPrint(StruInnovPrinterContent *content)
{
    char *pPrinter;
    int tempLen = 0;
    int tempperLen = 0;
    char *printerBuf = NULL;
    char TiredBuf[240];
    char timeStringBuf[32];
    unsigned int tempRecordNum;
    int i;
    time_t print_time;
    int malloc_size = 0;

    malloc_size = 850 + 160 * content->tired_driving_records_number;
    printerBuf = (char *)malloc(malloc_size);
    if(printerBuf == NULL)
    {
        perror("malloc()");
        return -1;
    }

    MPT_INIT();
    memset(printerBuf, 0, sizeof(printerBuf));

    pPrinter = "\r\n";
    tempperLen = strlen(pPrinter);
    memcpy((void *)printerBuf, (void *)pPrinter, tempperLen);
    tempLen += tempperLen;
    /******�ȴ��� �˴�������Ҫ����ʱ**/
    /**********���ƺ���******/
    pPrinter ="���������ƺ���:";
    tempperLen = strlen(pPrinter);
    memcpy((void *)(printerBuf + tempLen), (void *)pPrinter, tempperLen);
    tempLen += tempperLen;

    pPrinter = (char *) & (content->car_plate_number[0]);
    if(((unsigned char)pPrinter[0] < 0x80) || ((unsigned char)pPrinter[2] > 0x80))
    {
        memcpy((void *)(printerBuf + tempLen), (void *)"δ֪����", 8);
        tempLen += 8;
    }
    else
    {
        tempperLen = strlen(pPrinter);
        memcpy((void *)(printerBuf + tempLen), (void *)pPrinter, tempperLen);
        tempLen += tempperLen;
    }
    pPrinter = "\r\n";
    tempperLen = strlen(pPrinter);
    memcpy((void *)(printerBuf + tempLen), (void *)pPrinter, tempperLen);
    tempLen += tempperLen;
    /*********************���Ʒ���********************************************/
    pPrinter = "���������Ʒ���:";
    tempperLen = strlen(pPrinter);
    memcpy((void *)(printerBuf + tempLen), (void *)pPrinter, tempperLen);
    tempLen += tempperLen;

    pPrinter = (char *) & (content->car_plate_type[0]);
    tempperLen = strlen(pPrinter);
    memcpy((void *)(printerBuf + tempLen), (void *)pPrinter, tempperLen);
    tempLen += tempperLen;    
    pPrinter = "\r\n";
    tempperLen = strlen(pPrinter);
    memcpy((void *)(printerBuf + tempLen), (void *)pPrinter, tempperLen);
    tempLen += tempperLen;
    
    /*********************����ʶ�����************************/
    pPrinter = "����ʶ�����:\r\n    ";
    tempperLen = strlen(pPrinter);
    memcpy((void *)(printerBuf + tempLen), (void *)pPrinter, tempperLen);
    tempLen += tempperLen;

    pPrinter = (char *) & (content->car_vin_num[0]);
    tempperLen = strlen(pPrinter);
    memcpy((void *)(printerBuf + tempLen), (void *)pPrinter, tempperLen);
    tempLen += tempperLen;
    pPrinter = "\r\n";
    tempperLen = strlen(pPrinter);
    memcpy((void *)(printerBuf + tempLen), (void *)pPrinter, tempperLen);
    tempLen += tempperLen;
    
    /*****************************��֤********/
    pPrinter = "��������ʻ֤����:\r\n    ";
    tempperLen = strlen(pPrinter);
    memcpy((void *)(printerBuf + tempLen), (void *)pPrinter, tempperLen);
    tempLen += tempperLen;

    pPrinter = (char *) & (content->driving_license_number[0]);
    tempperLen = strlen(pPrinter);
    memcpy((void *)(printerBuf + tempLen), (void *)pPrinter, tempperLen);
    tempLen += tempperLen;

    pPrinter = "\r\n";
    tempperLen = strlen(pPrinter);
    memcpy((void *)(printerBuf + tempLen), (void *)pPrinter, tempperLen);
    tempLen += tempperLen;

    /**********************�ٶ�״̬********************/
    pPrinter = "�ٶ�״̬:";
    tempperLen = strlen(pPrinter);
    memcpy((void *)(printerBuf + tempLen), (void *)pPrinter, tempperLen);
    tempLen += tempperLen;

    switch(content->car_speed_status)
    {
        case CAR_SPEED_STATUS_ABNORMAL:
            pPrinter = "�쳣";
            tempperLen = strlen(pPrinter);
            memcpy((void *)(printerBuf + tempLen), (void *)pPrinter, tempperLen);
            tempLen += tempperLen;
            break;
        case CAR_SPEED_STATUS_NORMAL:
            pPrinter = "����";
            tempperLen = strlen(pPrinter);
            memcpy((void *)(printerBuf + tempLen), (void *)pPrinter, tempperLen);
            tempLen += tempperLen;
            break;
    }
    pPrinter = "\r\n";
    tempperLen = strlen(pPrinter);
    memcpy((void *)(printerBuf + tempLen), (void *)pPrinter, tempperLen);
    tempLen += tempperLen;

    /***********************************��ӡʱ��************/
    pPrinter = "��ӡʱ��:\r\n    ";
    tempperLen = strlen(pPrinter);
    memcpy((void *)(printerBuf + tempLen), (void *)pPrinter, tempperLen);
    tempLen += tempperLen;

    print_time = time(NULL);
    
    pPrinter = Time2String(print_time, timeStringBuf);
    tempperLen = strlen(pPrinter);
    memcpy((void *)(printerBuf + tempLen), (void *)pPrinter, tempperLen);
    tempLen += tempperLen;

    pPrinter = "\r\n\r\n";
    tempperLen = strlen(pPrinter);
    memcpy((void *)(printerBuf + tempLen), (void *)pPrinter, tempperLen);
    tempLen += tempperLen;

    /***********************************ͣ��ʱ��************/
    pPrinter = "ͣ��ʱ��:\r\n    ";
    tempperLen = strlen(pPrinter);
    memcpy((void *)(printerBuf + tempLen), (void *)pPrinter, tempperLen);
    tempLen += tempperLen;
 
    char stop_time[50];
    
    pPrinter = Time2String(content->stopDateTime, stop_time);
    tempperLen = strlen(pPrinter);
    memcpy((void *)(printerBuf + tempLen), (void *)pPrinter, tempperLen);
    tempLen += tempperLen;
    
    pPrinter = "\r\n\r\n";
    tempperLen = strlen(pPrinter);
    memcpy((void *)(printerBuf + tempLen), (void *)pPrinter, tempperLen);
    tempLen += tempperLen;
    
    /*********************ͣ��ǰ15������ÿ����ƽ���ٶ�************************/
    pPrinter = "ͣ��ǰ15������ÿ����ƽ���ٶ�:\r\n";
    tempperLen = strlen(pPrinter);
    memcpy((void *)(printerBuf + tempLen), (void *)pPrinter, tempperLen);
    tempLen += tempperLen;

    struct tm tmp_t; 
    int hour;
    int minute;
    char str[30] = {0};
    char tmp_str[2][5];
    int count;
    time_t t_i;

    t_i = content->stop_time;
    localtime_r(&t_i, &tmp_t);
    hour = tmp_t.tm_hour;
    minute = tmp_t.tm_min;

    for(count = 0; count < 15; count++)
    {
        if(hour == 0)
            sprintf(tmp_str[0], "00");
        else if(hour /10 == 0)
            sprintf(tmp_str[0], "0%d", hour);
        else
            sprintf(tmp_str[0], "%d", hour);
        
        if(minute== 0)
            sprintf(tmp_str[1], "00");
        else if(minute /10 == 0)
            sprintf(tmp_str[1], "0%d", minute);
        else
            sprintf(tmp_str[1], "%d", minute);

        sprintf(str, "%s:%s     %3d km/h\r\n", tmp_str[0], tmp_str[1], content->stop_speed[count]);
        tempperLen = strlen(str);
        memcpy((void *)(printerBuf + tempLen), (void *)str, tempperLen);
        tempLen += tempperLen;
        if(minute == 0)
        {
            if(hour == 0)
                hour = 23;
            else
                hour--;
            minute = 59;
        }
        else
            minute--;
    }
    pPrinter = "\r\n";
    tempperLen = strlen(pPrinter);
    memcpy((void *)(printerBuf + tempLen), (void *)pPrinter, tempperLen);
    tempLen += tempperLen;

    /**********************************ƣ�ͼ�ʻ��¼��ӡ*****************/
    
    pPrinter = "2���������ڳ�ʱ��ʻ��¼:\r\n";
    tempperLen = strlen(pPrinter);
    memcpy((void *)(printerBuf + tempLen), (void *)pPrinter, tempperLen);
    tempLen += tempperLen;

    if( ((content->tired_driving_records_number) <= 0) || ((content->tired_driving_records_number) > 99) )
    {
        pPrinter = "\r\n        �޼�¼!\r\n";
        tempperLen = strlen(pPrinter);
        memcpy((void *)(printerBuf + tempLen), (void *)pPrinter, tempperLen);
        tempLen += tempperLen;
    }
    else
    {
        tempRecordNum = content->tired_driving_records_number;
        for(i = 0; i < tempRecordNum;)
        {
            memset(TiredBuf, '\0', sizeof(TiredBuf));
            tempperLen = read_tired_driving(TiredBuf, i, content);
            memcpy((void *)(printerBuf + tempLen), (void *)TiredBuf, tempperLen);
            tempLen += tempperLen;
            i++;
        }
    }

    /**********************************״̬��״̬��ӡ*****************/

    pPrinter = "\r\n    ";
    tempperLen = strlen(pPrinter);
    memcpy((void *)(printerBuf + tempLen), (void *)pPrinter, tempperLen);
    tempLen += tempperLen;
    
   /* for(i =0;i<STATUS_LEN;i++)
    {
            if(strcmp(content->status[i],"δ��"))
            {
                    pPrinter = (char *) & (content->status[i]);
                    tempperLen = strlen(pPrinter);
                    memcpy((void *)(printerBuf + tempLen), (void *)pPrinter, tempperLen);
                    tempLen += tempperLen;

                    pPrinter = ":��Ч\r\n    ";
                    tempperLen = strlen(pPrinter);
                    memcpy((void *)(printerBuf + tempLen), (void *)pPrinter, tempperLen);
                    tempLen += tempperLen;
            }
    }*/

        pPrinter = "ACC";
        tempperLen = strlen(pPrinter);
        memcpy((void *)(printerBuf + tempLen), (void *)pPrinter, tempperLen);
        tempLen += tempperLen;

        pPrinter = ":��Ч\r\n    ";
        tempperLen = strlen(pPrinter);
        memcpy((void *)(printerBuf + tempLen), (void *)pPrinter, tempperLen);
        tempLen += tempperLen;

        pPrinter = "��ɲ";
        tempperLen = strlen(pPrinter);
        memcpy((void *)(printerBuf + tempLen), (void *)pPrinter, tempperLen);
        tempLen += tempperLen;

        pPrinter = ":��Ч\r\n    ";
        tempperLen = strlen(pPrinter);
        memcpy((void *)(printerBuf + tempLen), (void *)pPrinter, tempperLen);
        tempLen += tempperLen;

        pPrinter = "��ɲ";
        tempperLen = strlen(pPrinter);
        memcpy((void *)(printerBuf + tempLen), (void *)pPrinter, tempperLen);
        tempLen += tempperLen;

        pPrinter = ":��Ч\r\n    ";
        tempperLen = strlen(pPrinter);
        memcpy((void *)(printerBuf + tempLen), (void *)pPrinter, tempperLen);
        tempLen += tempperLen;

        pPrinter = "����";
        tempperLen = strlen(pPrinter);
        memcpy((void *)(printerBuf + tempLen), (void *)pPrinter, tempperLen);
        tempLen += tempperLen;

        pPrinter = ":��Ч\r\n    ";
        tempperLen = strlen(pPrinter);
        memcpy((void *)(printerBuf + tempLen), (void *)pPrinter, tempperLen);
        tempLen += tempperLen;

        pPrinter = "����";
        tempperLen = strlen(pPrinter);
        memcpy((void *)(printerBuf + tempLen), (void *)pPrinter, tempperLen);
        tempLen += tempperLen;

        pPrinter = ":��Ч\r\n    ";
        tempperLen = strlen(pPrinter);
        memcpy((void *)(printerBuf + tempLen), (void *)pPrinter, tempperLen);
        tempLen += tempperLen;

        pPrinter = "Զ��";
        tempperLen = strlen(pPrinter);
        memcpy((void *)(printerBuf + tempLen), (void *)pPrinter, tempperLen);
        tempLen += tempperLen;

        pPrinter = ":��Ч\r\n    ";
        tempperLen = strlen(pPrinter);
        memcpy((void *)(printerBuf + tempLen), (void *)pPrinter, tempperLen);
        tempLen += tempperLen;

        pPrinter = "��ת";
        tempperLen = strlen(pPrinter);
        memcpy((void *)(printerBuf + tempLen), (void *)pPrinter, tempperLen);
        tempLen += tempperLen;

        pPrinter = ":��Ч\r\n    ";
        tempperLen = strlen(pPrinter);
        memcpy((void *)(printerBuf + tempLen), (void *)pPrinter, tempperLen);
        tempLen += tempperLen;

        pPrinter = "��ת";
        tempperLen = strlen(pPrinter);
        memcpy((void *)(printerBuf + tempLen), (void *)pPrinter, tempperLen);
        tempLen += tempperLen;

        pPrinter = ":��Ч\r\n    ";
        tempperLen = strlen(pPrinter);
        memcpy((void *)(printerBuf + tempLen), (void *)pPrinter, tempperLen);
        tempLen += tempperLen;

        pPrinter = "����";
        tempperLen = strlen(pPrinter);
        memcpy((void *)(printerBuf + tempLen), (void *)pPrinter, tempperLen);
        tempLen += tempperLen;

        pPrinter = ":��Ч\r\n    ";
        tempperLen = strlen(pPrinter);
        memcpy((void *)(printerBuf + tempLen), (void *)pPrinter, tempperLen);
        tempLen += tempperLen;

        pPrinter = "�ٶ�";
        tempperLen = strlen(pPrinter);
        memcpy((void *)(printerBuf + tempLen), (void *)pPrinter, tempperLen);
        tempLen += tempperLen;

        pPrinter = ":��Ч\r\n    ";
        tempperLen = strlen(pPrinter);
        memcpy((void *)(printerBuf + tempLen), (void *)pPrinter, tempperLen);
        tempLen += tempperLen;
    
    pPrinter = "\r\nǩ��:\r\n\r\n      __________\r\n\r\n\r\n";
    tempperLen = strlen(pPrinter);
    memcpy((void *)(printerBuf + tempLen), (void *)pPrinter, tempperLen);
    tempLen += tempperLen;

    pPrinter =  "\r\n\r\n\r\n\r\n\r\n\r\n\r\n";
    tempperLen = strlen(pPrinter);
    memcpy((void *)(printerBuf + tempLen), (void *)pPrinter, tempperLen);
    tempLen += tempperLen;
    printerBuf[tempLen] = '\0';
    open_printer();
    write_printer( (unsigned char *)printerBuf, tempLen);
    /******���б�Ҫ ����ʱ*************/
    close_printer();
    printf("%s",printerBuf);
    free(printerBuf);

    return 1;
}

int read_tired_driving(char *pBuf, int Num, StruInnovPrinterContent *content)
{
    char *pPrinter;
    char TimeBuf[30];
    int tempLen = 0;
    int tempperLen = 0;
    char *buf_temp;

    StruTiredDrivingRecord *ptr2;
    StruInnovPrinterContent *tired_printer;

    buf_temp = TimeBuf;
    
    tired_printer = content;
    ptr2 = tired_printer->tired_driving_records;

    pPrinter = "��¼";
    tempperLen = strlen(pPrinter);
    memcpy((void *)(pBuf + tempLen), (void *)pPrinter, tempperLen);
    tempLen += tempperLen;

    tempperLen = Dec2String((char *)TimeBuf, (Num + 1));
    memcpy((void *)(pBuf + tempLen), (void *)TimeBuf, tempperLen);
    tempLen += tempperLen;

    pPrinter = ":\r\n";
    tempperLen = strlen(pPrinter);
    memcpy((void *)(pBuf + tempLen), (void *)pPrinter, tempperLen);
    tempLen += tempperLen;


    ptr2 += Num;
    pPrinter = "��������ʻ֤����:";
    tempperLen = strlen(pPrinter);
    memcpy((void *)(pBuf + tempLen), (void *)pPrinter, tempperLen);
    tempLen += tempperLen;
    
    pPrinter = "\r\n    ";
    tempperLen = strlen(pPrinter);
    memcpy((void *)(pBuf + tempLen), (void *)pPrinter, tempperLen);
    tempLen += tempperLen;

    pPrinter = (char *) & (ptr2->driving_license_number[0]);
    tempperLen = strlen(pPrinter);
    memcpy((void *)(pBuf + tempLen), (void *)pPrinter, tempperLen);
    tempLen += tempperLen;

    pPrinter = "\r\n";
    tempperLen = strlen(pPrinter);
    memcpy((void *)(pBuf + tempLen), (void *)pPrinter, tempperLen);
    tempLen += tempperLen;

    pPrinter = "������ʻ��ʼʱ��:";
    tempperLen = strlen(pPrinter);
    memcpy((void *)(pBuf + tempLen), (void *)pPrinter, tempperLen);
    tempLen += tempperLen;
    
    pPrinter = "\r\n    ";
    tempperLen = strlen(pPrinter);
    memcpy((void *)(pBuf + tempLen), (void *)pPrinter, tempperLen);
    tempLen += tempperLen;

    buf_temp = Time2String(ptr2->start_time, TimeBuf);
    tempperLen = strlen(buf_temp);
    memcpy((void *)(pBuf + tempLen), (void *)buf_temp, tempperLen);
    tempLen += tempperLen;
    
    pPrinter = "\r\n";
    tempperLen = strlen(pPrinter);
    memcpy((void *)(pBuf + tempLen), (void *)pPrinter, tempperLen);
    tempLen += tempperLen;
    
    pPrinter = "������ʻ����ʱ��:";
    tempperLen = strlen(pPrinter);
    memcpy((void *)(pBuf + tempLen), (void *)pPrinter, tempperLen);
    tempLen += tempperLen;
    
    pPrinter = "\r\n    ";
    tempperLen = strlen(pPrinter);
    memcpy((void *)(pBuf + tempLen), (void *)pPrinter, tempperLen);
    tempLen += tempperLen;

    buf_temp = Time2String(ptr2->stop_time, TimeBuf);
    tempperLen = strlen(buf_temp);
    memcpy((void *)(pBuf + tempLen), (void *)buf_temp, tempperLen);
    tempLen += tempperLen;

    pPrinter = "\r\n\r\n";
    tempperLen = strlen(pPrinter);
    memcpy((void *)(pBuf + tempLen), (void *)pPrinter, tempperLen);
    tempLen += tempperLen;

    return tempLen;
}
int Dec2String(char *psrc, int val)
{
    char local[11];
    char *p = &local[10];
    int result = 0;
    int count = 0;

    *p-- = '\0';
    result = val;
    do
    {
        *p-- = '0' + result % 10;
        result /= 10;
        count++;
    }
    while (result > 0);

    memmove(psrc, p + 1, count + 1);
    return count;
}
/*******************************************************************************/
/**
*** �� �� ��:  char *Time2String(const time_t timep, char *strDest)
*** ��������:  ��ʱ��ת���ɹ����ʽ���ַ���
*** ȫ�ֱ���: 		
*** ��  ��: 			
							const time_t timep :��õ�ʱ��
							char *strDest:�洢ת������ַ���
							
*** ��  ��: 				ת������ַ�����ַ
*** �� �� �ˣ�muxuelong     ���ڣ�2012-8-3   	
*** �� �� �ˣ�              ���ڣ�					
*** ����˵��:  
******************************************************************************/
char *Time2String(const time_t timep, char *strDest)
{
    char *address = strDest;
   // assert(strDest != NULL);
    struct tm* tm;
    time_t tempTime = timep;
    char *bars = (char *)"-",*colon = (char *)":", 
         *space = (char *)" ";
    
    tm = localtime(&tempTime);
    tm->tm_year += 1900;
    tm->tm_mon += 1;
    sprintf(strDest, "%04d%s%02d%s%02d%s%02d%s%02d%s%02d", tm->tm_year, bars, tm->tm_mon, 
            bars, tm->tm_mday, space, tm->tm_hour, colon, tm->tm_min, colon, tm->tm_sec);

    return address;
}
