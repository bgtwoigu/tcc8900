
#ifndef VOICE_SETTING_H
#define VOICE_SETTING_H

#include "debugMonitor.h"
#include "../project_config/common_config.h"

/* �����Ҫ��¼��־��VOICE_LOG����Ϊ1��������Ϊ0 */
/* �ƴ�Ѷ������ʶ�����־����ӡ��AitalkLog.esl �����ļ�����־��ͨ��printf��ӡ���*/

#define VOICE_LOG					(0)

#if		VOICE_LOG
	#define VOICE_PRINT    					printf
	#define BEGINFUNC_USING_BY_VOICE		printf("[%s] %s begin \n", __FILE__, __FUNCTION__);
	#define ENDFUNC_USING_BY_VOICE		printf("[%s] %s end \n", __FILE__, __FUNCTION__ );
	#define AISOUND						1

#else
	#define VOICE_PRINT
	#define BEGINFUNC_USING_BY_VOICE
	#define ENDFUNC_USING_BY_VOICE
	#define AISOUND						0
#endif




#define DBUG_LOG
#ifdef DBUG_LOG
#define dbug_out( fmt, arg... )  do{                        \
		IBD_PRINTF(4, fmt, ##arg); 			\
	}while( 0 )
#else
#define dbug_out( fmt, arg... )
#endif




/* ��/�ر�����ʶ���ģʽ */
/* 0 -- ��/�ر�����ʶ��Ქ����ʾ����*/
/* 1 -- ��/�ر�����ʶ��Ქ����ʾ��(�̵ܶ�����)*/
/* 2 -- ��/�ر�����ʶ�𲻻����κ���ʾ*/

#define VOICE_OPEN_CLOSE_MODE			(1)



//ABCDEF

//#define  PCM_DATA_FILE_MOTHER_NAME    "voicefile/pcmliu"



//char motherName[256];

//strcpy(motherName, ABCDEF);
//strcat(motherName, PCM_DATA_FILE_MOTHER_NAME);
/*���涨����voice����Ҫ�򿪵��ļ�( ����)��ȫ��*/

//PCM�����ļ���ĸ��������ʱ������˷��յ������ݣ�Ϊ�洢�������ĸ����ͷ���ļ���
#define PCM_DATA_FILE_MOTHER_NAME		INCAR_COMMON_MICRO_READ_WRITE_DIRECTORY"voicefile/pcmliu" 

//����ʶ�����Ӧ�ļ�������ʶ��ģ���ѧϰ��ͬ�˵ķ�����ѧϰ������¼������ļ���
#define VOICE_IDENTIFY_ADAPT_FILE			(INCAR_COMMON_MICRO_READ_WRITE_DIRECTORY"AitalkData.bin")

//����ʶ����־�ļ�
#define AITALK_LOG_FILE						INCAR_COMMON_MICRO_READ_WRITE_DIRECTORY"Aitalk.log"	

#define IVESR_FILE	"ivESR.irf"

#define RESOURCE_FILE	"Resource.irf"
#endif // VOICE_SETTING_H

