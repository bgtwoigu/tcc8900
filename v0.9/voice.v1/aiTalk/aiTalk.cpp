#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/soundcard.h>

#include <QMessageBox>
#include <QKeyEvent>
#include <QWidget>
#include <QtGui/QApplication>
#include <QFile>

#include "aiTalk.h"
#include "aiTalk_setting.h"
#include "parseConfig.h"
#include "error_types.h"

#include "audio_control_server.h"
#include "audio_control_client.h"

#include "audio_control.h"
#include "debugMonitor.h"

#define MCU_CHIPADDR 	0x50



#define USER_WORKBUFFER_ADDRESS		(0x01013d14)
	/* �����ڴ�ߴ磨�ֽ�����*/
#define USER_WORKBUFFER_BYTES		(640*1024)
	/* TODO:�û�ָ����̬��פ�ڴ��ַ*/
#define USER_RESIDENTBUFFER_ADDRESS	(0x01313d14)
	/* ��פ�ڴ�ߴ磨�ֽ�����*/
#define USER_RESIDENTBUFFER_BYTES	(32*1024)




/* �Ƿ�Ҫ�����������*/
/* �����������꣬��������ݣ�Ҫ�ܵ�ʶ���̵߳Ŀ���*/
/* �������������꣬��һֱ������ݣ�����ʶ���̵߳Ŀ���*/
#define AITALK_CONTROL_APPEND_DATA		1

/* ¼�������� */
#define AITALK_SAMPLERATE	16000

#define AITALK_CH		1


extern AudioCtrlServerC *audioServer;


/*�������ôʺ�ң�����Ӧ��ϵ������*/
Lexicon_Setting *g_aitalk_lexicon_setting = NULL;

/*���鳤��*/
int g_aitalk_lexicon_setting_num = 0;

/*�������ң�����������ʶ���*/
char* g_aitalk_open_keycode = NULL;

/*�������ң�����������ʶ��ر�*/
char* g_aitalk_close_keycode = NULL;


/* �Ƿ��յ���PCM���ݴ��ļ�*/
//#define AITALK_SAVE_PCM_DATA_TO_FILE		1
#ifdef	AITALK_SAVE_PCM_DATA_TO_FILE
	static FILE *g_pPCMDataFile = NULL;
	static char *g_sPCMDataFileMotherName = PCM_DATA_FILE_MOTHER_NAME;
	static char g_sPCMDataFileName[256];
	static int    g_iPCMDataFileCount = 0;
#endif

static snd_pcm_uframes_t frames;
extern AudioCtrlClientC *g_pAudioClientCtl;

PCMDataLoader::PCMDataLoader(ivPointer hESRObj, QThread *parent)
    :QThread(parent),
     m_bExitRun(false),
     m_bAppendData(false),
     m_hESRObj(hESRObj)
{
	BEGINFUNC_USING_BY_VOICE
	IBD_PRINTF(4, "in PCMDataLoader...\n");
	ENDFUNC_USING_BY_VOICE
}

PCMDataLoader::~PCMDataLoader()
{
	BEGINFUNC_USING_BY_VOICE
	ReleasePcm();
	ENDFUNC_USING_BY_VOICE
}

void PCMDataLoader::exitFromRun()
{
	BEGINFUNC_USING_BY_VOICE

	m_bExitRun = true;

	ENDFUNC_USING_BY_VOICE
}

void PCMDataLoader::StartAppend()
{
	IBD_PRINTF(4, "StartAppend............\n");
	m_bAppendData = true;
}

void PCMDataLoader::StopAppend() 
{
	
	IBD_PRINTF(4, "StopAppend............\n");
	m_bAppendData = false;
}

int PCMDataLoader::InitPcm()
{
	int err = 0;
	
	unsigned int val;
	int dir=0;

	quiet_mode = 0;

	snd_pcm_hw_params_t *hw_params = NULL;

	snd_pcm_hw_params_alloca(&hw_params);

	stream = SND_PCM_STREAM_CAPTURE;

	if ((err = snd_pcm_open (&capture_handle, "plug:tcc", SND_PCM_STREAM_CAPTURE, 0)) < 0) 
	{
		dbug_out("cannot open audio device (%s)\n", snd_strerror (err));
		return -1;
	}
	dbug_out("After snd_pcm_open for SND_PCM_STREAM_CAPTURE");
	if ((err = snd_pcm_hw_params_any (capture_handle, hw_params)) < 0) 
	{
		IBD_PRINTF(4, "cannot initialize hardware parameter structure (%s)\n",
			 snd_strerror (err));
		return -1;
	}

	if ((err = snd_pcm_hw_params_set_access (capture_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0)
	{
		IBD_PRINTF(4, "cannot set access type (%s)\n",
			 snd_strerror (err));
		return -1;
	}

	if ((err = snd_pcm_hw_params_set_format (capture_handle, hw_params, SND_PCM_FORMAT_S16_LE)) < 0)
	{
		IBD_PRINTF(4, "cannot set sample format (%s)\n",
			 snd_strerror (err));
		return -1;
	}
	val = AITALK_SAMPLERATE;
	if ((err = snd_pcm_hw_params_set_rate_near (capture_handle, hw_params, &val, 0)) < 0) 
	{
		IBD_PRINTF(4, "cannot set sample rate (%s)\n",
			 snd_strerror (err));
		return -1;
	}
	 val = AITALK_CH;	
	if ((err = snd_pcm_hw_params_set_channels (capture_handle, hw_params, val)) < 0)
	{
		IBD_PRINTF(4, "cannot set channel count (%s)\n",
			 snd_strerror (err));
		return -1;
	}

	if ((err = snd_pcm_hw_params (capture_handle, hw_params)) < 0)
	{
		IBD_PRINTF(4, "cannot set parameters (%s)\n",
			 snd_strerror (err));
		return -1;
	}
	
	if ((err = snd_pcm_prepare (capture_handle)) < 0)
	{
		IBD_PRINTF(4, "cannot prepare audio interface for use (%s)\n",
			 snd_strerror (err));
		return -1;
	}
	
	frames = 6144;
	
	if( (err = snd_pcm_hw_params_set_period_size_near(capture_handle, hw_params, &frames, &dir)) < 0)
	{
		IBD_PRINTF(4,"snd_pcm_hw_params_set_period_size_near failed!   err = %d\n", err);
		exit(1);
	}

	snd_pcm_hw_params_get_period_size(hw_params, &frames, 0);

	return 0;
#if 0	
	size = frames * 1 * 2;
	PcmBuf = (char *)malloc(sizeof(char) * size);
#endif
}

void PCMDataLoader::ReleasePcm()
{
	snd_pcm_close(capture_handle);
}


void PCMDataLoader::run()
{
	BEGINFUNC_USING_BY_VOICE

	int iInitTimes = 0;
	snd_pcm_uframes_t err;
	ivESRStatus iStatus;
	char mono[PCMBUFFSIZE];

#ifdef	AITALK_SAVE_PCM_DATA_TO_FILE
	if( NULL == g_pPCMDataFile )
	{
		sprintf(g_sPCMDataFileName,"%s%d.wav", g_sPCMDataFileMotherName, g_iPCMDataFileCount);
		g_iPCMDataFileCount++;
		g_pPCMDataFile = fopen(g_sPCMDataFileName,"w+");
	}
#endif	

AgInit:
	if(InitPcm() == -1)
	{
		while(iInitTimes < 5)
		{
			iInitTimes++;
			dbug_out("InitPcm Error...will Init agin...");
			goto AgInit;
		}
		dbug_out("sInitPcm Error...");
	}
	
	EsrEngineReset(m_hESRObj); 
//	frames = 2048;
	frames = frames * 2;

#ifdef VOICE_LOG_OUT
	FILE* voiceLog;
	voiceLog = fopen(VOICE_LOG_PATH, "a");
	int logret = 0;
#endif

	while( !m_bExitRun )
	{	
		memset(mono, 0, sizeof(char) * PCMBUFFSIZE);
#if 1
		err = snd_pcm_readi(capture_handle, mono, frames);

#ifdef VOICE_LOG_OUT
		logret = fwrite(mono, sizeof(char), sizeof(char) * frames * 2, voiceLog);
		if(logret < 0)
			IBD_PRINTF(4, "fwrite voicelog file fail\n");
#endif

		if(err != frames)
		{
			IBD_PRINTF(4, "read from audio interface failed (%s)\n", snd_strerror(err));
			int myPrePareErr;
			if( (myPrePareErr = snd_pcm_prepare(capture_handle)) < 0)
			{
				IBD_PRINTF(4,"snd_pcm_prepare_failed! err: %s\n", snd_strerror(myPrePareErr));
			}
		}
#endif

#ifdef	AITALK_SAVE_PCM_DATA_TO_FILE
				if( NULL != g_pPCMDataFile )
				{
					fwrite(mono,1,frames*2,g_pPCMDataFile);
				}			
#endif
		if(m_bAppendData) 
		{
			iStatus = EsrAppendData(m_hESRObj, mono, frames);//��ΪҪ����������16bit�ģ�����
															//����Ӧ����mono�ĳ���(size/2)��
															//����2,��size/4
			if( ivESR_OK == iStatus )
			{
//				IBD_PRINTF(4,"[aiTalk.cpp] ok!\n");
				usleep(30000);
			   /*�ɹ�������������*/
			}
			else if( ivESR_BUFFERFULL == iStatus )
			{
			/*Aitalk CE 2.2�ڲ���Ƶ���ݻ���������������׷��ʧ�ܣ������ѱ�Aitalk CE 2.2����*/
				IBD_PRINTF(4,"[aiTalk.cpp] huan chong qu yi man , fa song shi bai  size[%d]!\n", (int)(frames/2));
			iStatus = EsrEngineReset(m_hESRObj);
			IBD_PRINTF(4,"[aiTalk.cpp]Engine buf full  Reset!!\n");
			}
			else if( ivESR_ASR_ENDED == iStatus )
			{
			/*ʶ��������ֹͣ����Ҫ����"EsrEngineReset" API����ESR����*/
				iStatus = EsrEngineReset(m_hESRObj);
				IBD_PRINTF(4,"[aiTalk.cpp]Engine Stop Reset!!\n");
			}
			else if( ivESR_INVCAL == iStatus )
			{
			/*����ĵ��ã��������*/
				IBD_PRINTF(4,"[aiTalk.cpp] dui xiang cuo wu !!\n");
			}
			else if( ivESR_INVARG == iStatus )
			{
			/*����ĵ��ã��������*/
				IBD_PRINTF(4,"[aiTalk.cpp] can shu cuo wu !!! \n");
			}
			else
			{
			/*Ӧ�ò��ᵽ����*/
			}
		}
	}

//	system("caraction mutegf");
	ReleasePcm();
//	system("caraction unmutegf");

#ifdef	AITALK_SAVE_PCM_DATA_TO_FILE
	if( NULL != g_pPCMDataFile )
	{
		fclose(g_pPCMDataFile);
		g_pPCMDataFile = NULL;
	}
#endif

	m_bExitRun = false;
	ENDFUNC_USING_BY_VOICE
}






void RealseIdentifyInfo()
{
	if( NULL != g_aitalk_open_keycode )
	{
		free(g_aitalk_open_keycode);
		g_aitalk_open_keycode = NULL;
	}

	if( NULL != g_aitalk_close_keycode )
	{
		free(g_aitalk_close_keycode);
		g_aitalk_close_keycode = NULL;
	}

	if( NULL != g_aitalk_lexicon_setting )
	{
		for(int i=0; i<200; i++)
		{
			if( NULL != g_aitalk_lexicon_setting[i].item )
			{
				free(g_aitalk_lexicon_setting[i].item);
				g_aitalk_lexicon_setting[i].item = NULL;
			}

			if( NULL != g_aitalk_lexicon_setting[i].KeyCode)
			{
				free(g_aitalk_lexicon_setting[i].KeyCode);
				g_aitalk_lexicon_setting[i].KeyCode = NULL;
			}
		}

		free(g_aitalk_lexicon_setting);
		g_aitalk_lexicon_setting = NULL;
	}
	
	g_aitalk_lexicon_setting_num = 0;

	return;
}


bool LoadIdentifyInfo()
{
	int ret;
	char buffer[256];
	char key[64];

	//װ�������ļ�
	if(0 > IBParseConfigTool::read_config_load("./voice.ini"))
	{
		IBD_PRINTF(4,"[voice] cann't open voice.ini file!\n");
		return false;
	}

	g_aitalk_lexicon_setting = (Lexicon_Setting*)malloc(sizeof(Lexicon_Setting)*200);
	if(NULL == g_aitalk_lexicon_setting)
	{
		IBD_PRINTF(4,"[voice] malloc error!    size(%d)(%d)\n", sizeof(Lexicon_Setting), sizeof(Lexicon_Setting)*200);
		return false;
	}

	for(int i=0; i<200; i++)
	{
		g_aitalk_lexicon_setting[i].item = NULL;
		g_aitalk_lexicon_setting[i].KeyCode = NULL;
	}

	char item_name[16];
	g_aitalk_lexicon_setting_num=0;
	//���ؼ��ʼ����Ӧ��ң����
	for(int i=0; i<200; i++)
	{
		sprintf(item_name, "word%d", i);
		if(R_INNOV_BOX_COMMON_ITEM_NOFOUND == (ret = IBParseConfigTool::read_config((char *)"identify word", (char*)item_name, key, 64)))
		{
			//IBD_PRINTF(4,"cann't find %s on config file\n", item_name);
		}
		else if( ret < 0 )
		{
			//IBD_PRINTF(4,"read config file error!\n");
		}
		else
		{
			sprintf(item_name, "KeyCode%d", i);
			if(R_INNOV_BOX_COMMON_ITEM_NOFOUND == (ret = IBParseConfigTool::read_config((char *)"identify word", (char*)item_name, buffer, 256)))
			{
				//IBD_PRINTF(4,"cann't find %s on config file\n", name);
			}
			else if( ret < 0 )
			{
				//IBD_PRINTF(4,"read config file error!\n");
			}
			else
			{
				//�������Ĺؼ��ʺͶ�Ӧ��ң�������������
				g_aitalk_lexicon_setting[g_aitalk_lexicon_setting_num].item = (char*)malloc(strlen(key)+1);
				if(NULL == g_aitalk_lexicon_setting[g_aitalk_lexicon_setting_num].item)
				{
					IBD_PRINTF(4,"[voice] malloc error!\n");
					return false;
				}

				strcpy(g_aitalk_lexicon_setting[g_aitalk_lexicon_setting_num].item, key);

				g_aitalk_lexicon_setting[g_aitalk_lexicon_setting_num].KeyCode = (char*)malloc(strlen(buffer)+1+1);
				if(NULL == g_aitalk_lexicon_setting[g_aitalk_lexicon_setting_num].KeyCode)
				{
					IBD_PRINTF(4,"[voice] malloc error!\n");
					return false;
				}
				strcpy(g_aitalk_lexicon_setting[g_aitalk_lexicon_setting_num].KeyCode, buffer);
				g_aitalk_lexicon_setting[g_aitalk_lexicon_setting_num].KeyCode[strlen(buffer)] = 10;
				g_aitalk_lexicon_setting[g_aitalk_lexicon_setting_num].KeyCode[strlen(buffer)+1] = 0;

				g_aitalk_lexicon_setting_num++;
			}
		}
	}

	//ж�������ļ�
	IBParseConfigTool::read_config_unload();
	
	return true;
}

AiTalk::AiTalk(QThread *parent)
	:QThread(parent),
	m_bExitRun(false),
	m_bState(false),
	m_hESRObj(NULL),
	m_pResourceFile(NULL),
	m_pIvTTSInterface(NULL),
	m_bAllowEnterIdentifyMode(true)
{
	BEGINFUNC_USING_BY_VOICE

	ivBool bRet;
	
	audioServerBusy = false;

	/* ��voice.ini������ʶ����Ϣ*/
	if( !LoadIdentifyInfo() )
	{
		IBD_PRINTF(4, "[aiTalk.cpp] load identify info error from voice.ini !\n");
		AitalkDestryObject();
		throw;
	}

	bRet = AitalkCreateObject();
	if( !bRet )
	{
		IBD_PRINTF(4, "[Aitalk.cpp]--{Aitalk}: AitalkCreateObject Error........\n");
		throw;
	}
	
	/* �ʵ䣬��ʽ����������ƺ����� */
	bRet = AitalkInputTextData();
	if( !bRet )
	{
		AitalkDestryObject();
		IBD_PRINTF(4, "[Aitalk.cpp]--{Aitalk}: AitalkInputTextData Error........\n");
		throw;
	}

	/* ����װ����ƵPCM���ݵ��߳�*/
	m_pPCMDataLoader = new PCMDataLoader(m_hESRObj);
	if( NULL == m_pPCMDataLoader )
	{
		AitalkDestryObject();
		IBD_PRINTF(4, "[Aitalk.cpp]--{Aitalk}: New m_pPCMDataLoader Error........\n");
		throw;
	}

	ENDFUNC_USING_BY_VOICE
}


AiTalk::~AiTalk()
{
	BEGINFUNC_USING_BY_VOICE

	ivBool bRet;

	/* ����װ����ƵPCM���ݵ��߳�*/
	if( NULL != m_pPCMDataLoader )
	{
		m_pPCMDataLoader->exitFromRun();
		m_pPCMDataLoader->wait();
		delete m_pPCMDataLoader;
		m_pPCMDataLoader = NULL;
	}

	/*ֹͣ����ʶ���߳�*/
	exitFromRun();
	wait();
	
	/* Aitalk CE 2.2�������� */
	bRet = AitalkDestryObject();

	/* �ر���Դ�ļ�*/
	if( NULL != m_pResourceFile )
	{
		fclose(m_pResourceFile);
		m_pResourceFile = NULL;
	}

	RealseIdentifyInfo();
	
	ENDFUNC_USING_BY_VOICE
}

void AiTalk::exitFromRun()
{
	BEGINFUNC_USING_BY_VOICE

	m_bExitRun = true;

	ENDFUNC_USING_BY_VOICE
}

void AiTalk::run()
{
	BEGINFUNC_USING_BY_VOICE

	int fd;
	
	ivESRStatus    iStatus;
	
	fd = open("/dev/key", O_RDWR);

#if( VOICE_OPEN_CLOSE_MODE == 0 )
	usleep(2000000);//����"��ʼ����ʶ��"��Ҫ��Լ2��࣬����û��Ҫ��ô�������ʶ��
#elif( VOICE_OPEN_CLOSE_MODE == 1 )
	usleep(500000);//���Ŷ��ݵ���ʾ����Ҫ��Լ0.5��࣬����û��Ҫ��ô�������ʶ��
#elif( VOICE_OPEN_CLOSE_MODE == 2 )
	//ʲô��ʾҲû�У����õȴ�
#else

#endif
	/* ��ʼʶ��¼�����ݵĴ��� */
	iStatus = EsrStartA(m_hESRObj, (ivCStrA)"141����");
	EsrEngineReset(m_hESRObj);	
#if 1
	if( NULL != m_pPCMDataLoader )
	{
		/* ���ÿ�ʼ�������*/
		/* ע��: �������һ��Ҫ��ǰ����ã������߳̿�ʼ����
		                  �󣬾ͻ������������*/
		m_pPCMDataLoader->StartAppend();

		/* ��ʼ������ƵPCM���ݵ��߳�*/
		m_pPCMDataLoader->start();
	}
#endif
	while(!m_bExitRun)
	{	
		ivUInt32		iSyntax;
		ivPUInt32	pCmdID;
		ivUInt32		nSame;
		
		iStatus = EsrRunStep(m_hESRObj);
		
		if( ivESR_RESULT == iStatus )
		{
			m_pPCMDataLoader->StopAppend();
			IBD_PRINTF(4, "\n[aiTalk.cpp] get a result\n");

			/* ��ȡʶ����ľ�ʽ*/
			if( ivESR_OK != (iStatus = EsrGetResultSyntax(m_hESRObj,&iSyntax)) )
			{
				/* �õ���ʽ����*/
				IBD_PRINTF(4, "[aiTalk.cpp] EsrGetResultSyntax error! iStatus[%ld]\n", iStatus);
			}
			else if( 1 != iSyntax )
			{
				/* �õ��˴���ľ�ʽID������ֻ��ID = 1 �ľ�ʽ*/
				IBD_PRINTF(4, "[aiTalk.cpp] iSyntax isn't 1, but we only have iSyntax whose id is 1!!!\n");
			}
			/* ��ȡʶ����Ĵ���(141�ʵ�) */
			else if( ivESR_OK != (iStatus = EsrGetResultParameterA(m_hESRObj, &pCmdID, &nSame, (ivCStrA)"141�ʵ�")) )
			{
				/* �õ��˴���ľ�ʽID������ֻ��ID = 1 �ľ�ʽ*/
				IBD_PRINTF(4, "[aiTalk.cpp] EsrGetResultParameterA error! iStatus[%ld]\n", iStatus);
			}
			else if( 1 != nSame )
			{
				/* ���ǵľ�ʽÿ��ֻ�ܻ��һ��������������1 �϶�����*/
				IBD_PRINTF(4, "[aiTalk.cpp] nSame isn't 1, but we only can get one item everytime!!!\n");
			}
			else
			{
				while(audioServerBusy){
					dbug_out("Get a resault Aitalk wait audioServerBusy");
					usleep(10000);
				}
				m_mutexAiTalkAudioServerBusy.lock();
				audioServerBusy = true;
				m_mutexAiTalkAudioServerBusy.unlock();

#if 1
				audioServer->closeAudio(IBAC_DEV_ID_MIC);
#else
				if(g_pAudioClientCtl == NULL){
					dbug_out("g_pAudioClientCtl is NULL");
					continue;
				}

				int resault = 0;
				resault = g_pAudioClientCtl->closeAudio(IBAC_DEV_ID_MIC);
				if(resault != 0)
				{
					dbug_out("Close mic error");			
				}
#endif
				/* ���ڣ����ǻ������ȷ�Ĵ���*/
				ioctl(fd, atoi(g_aitalk_lexicon_setting[pCmdID[0]-1].KeyCode), NULL);

				/*�ϳɡ�����ʶ����Ĵ�*/
				if( NULL != m_pIvTTSInterface )
				{
					m_pIvTTSInterface->PlayString(User_AiTalk, g_aitalk_lexicon_setting[pCmdID[0]-1].item);
				}
				sleep(2);
#if 1
				audioServer->openAudio(IBAC_DEV_ID_MIC, -1);
#else
				if(g_pAudioClientCtl == NULL){
					dbug_out("g_pAudioClientCtl is NULL");
					continue;
				}

				resault = g_pAudioClientCtl->openAudio(IBAC_DEV_ID_MIC, -1);
				if(resault != 0)
				{
					dbug_out("Open mic error");			
				}
#endif
				m_mutexAiTalkAudioServerBusy.lock();
				audioServerBusy = false;
				m_mutexAiTalkAudioServerBusy.unlock();
			}

			EsrEngineReset(m_hESRObj);
			usleep(300000);
			m_pPCMDataLoader->StartAppend();
		}
		else if(ivESR_FORCERESULT == iStatus)
		{
			/* ǿ�ƻ�ȡʶ������ʶ����̷�����������������ʱ�����û��ֹ�������ʶ����̲Ż᷵��ivESR_FORCERESULT */
			dbug_out("[aiTalk.cpp] get a force result! \n");
			EsrEngineReset(m_hESRObj);
		}
		else if(ivESR_RESPONSETIMEOUT == iStatus)
		{
			/* �û���Ӧ��ʱ�����û�����ʶ��󣬳�ʱ�䲻˵�����������趨�˷�Ӧ��ʱʱ�� */
			dbug_out("[aiTalk.cpp] time out !\n");
			EsrEngineReset(m_hESRObj);	
		}
		else if(ivESR_ASR_ENDED == iStatus)
		{
			/* ʶ������Ѿ���������Ҫ����EsrReset����ʶ���ֹ�������ʶ����̺�Aitalk CE 2.2 �Ѿ����������л����¼�����ݲ�������ivESR_FORCERESULT���������EsrRunStep�᷵��ivESR_ASR_ENDED */
			dbug_out("[aiTalk.cpp] identify is ended\n");
			EsrEngineReset(m_hESRObj);	
		}
		else if(ivESR_SPEECHSTART == iStatus)
		{
			/* ��⵽������ʼ֪ͨ��Aitalk CE 2.2���Լ�⵽�û��Ƿ�ʼ˵�� */
			dbug_out("[aiTalk.cpp] aitalk start !\n");
		}
		else if(ivESR_BUFFEREMPTY == iStatus)
		{
			/* Aitalk CE 2.2�����¼�������Ѿ��������ˣ������ô��������������������Ϣ20���� */
			usleep(20000);
		}
		else if(ivESR_OK == iStatus)
		{
			/* ����������һ֡¼�����ݣ����Լ�������EsrRunStep��������Aitalk CE 2.2�����¼������ */
			//VOICE_PRINT("[aiTalk.cpp] do one PCM frame!");
		}
		else
		{
			/* �д�����������ݷ���ֵ���Խ�����⣬���޷��������ϵ����֧�� */
			dbug_out("EsrRunStep error, we will reset the engine!\n");
			EsrEngineReset(m_hESRObj);
		}
	}

	iStatus = EsrStop(m_hESRObj);
	dbug_out("EsrStop iStatus[%ld]\n", iStatus);
#if 1
	/* �ر�������ƵPCM���ݵ��߳�, ��������ֹͣ�������*/	
	if( NULL != m_pPCMDataLoader )
	{
		m_pPCMDataLoader->exitFromRun();
		m_pPCMDataLoader->wait();
		m_pPCMDataLoader->StopAppend();
	}
#endif
	m_bExitRun = false;
#if 1

#endif

	close(fd);
	ENDFUNC_USING_BY_VOICE
}

bool AiTalk::StartIdentify()
{
	BEGINFUNC_USING_BY_VOICE

	m_mutexAiTalk.lock();

	if(!m_bAllowEnterIdentifyMode)
	{
		m_mutexAiTalk.unlock();
		return false;
	}
	if(m_bState)
	{
		m_mutexAiTalk.unlock();
		return true;	
	}

	/*��������ʶ��״̬*/
	/*ע��:	m_bState�ĸı䣬�������StartIdentify��ǰ������Ϊm_bStateΪtrue��־��voiceҪռ��һЩ�豸(dsp��i2sin)��
				��ռ����Щ�豸���Ǳ�����ִ����ȥ������*/
	m_bState = true;

	/*�����ض�ң����֪ͨ�������̣�����ʶ�����Ѿ���*/
	NotifyState();
	
	/*��ʼ����ʶ���߳�*/
	start();


	m_mutexAiTalk.unlock();

	ENDFUNC_USING_BY_VOICE

	return true;
}

bool AiTalk::StopIdentify()
{
	BEGINFUNC_USING_BY_VOICE

	m_mutexAiTalk.lock();

	if( !m_bState )
	{
		m_mutexAiTalk.unlock();
		return true;	
	}
	
	/*ֹͣ����ʶ���߳�*/
	exitFromRun();

	wait();

	/*��������ʶ��״̬*/
	/*ע��:	m_bState�ĸı䣬�������StartIdentify��ǰ������Ϊm_bStateΪtrue��־��voiceҪռ��һЩ�豸(dsp��i2sin)��
				��ռ����Щ�豸���Ǳ�����ִ����ȥ������*/
	m_bState = false;

	/*�����ض�ң����֪ͨ�������̣�����ʶ�����Ѿ��ر�*/
	NotifyState();

	m_mutexAiTalk.unlock();

	ENDFUNC_USING_BY_VOICE

	return true;
}

bool AiTalk::GetIdentifyState()
{
	BEGINFUNC_USING_BY_VOICE

	ENDFUNC_USING_BY_VOICE

	return m_bState;
	
}

void AiTalk::NotifyState()
{
	BEGINFUNC_USING_BY_VOICE
	int fd;
	
	fd = open("/dev/key", O_RDWR);

	if(m_bState)
	{
		/*����ʶ�����Ѿ���*/
		ioctl(fd, SEND_AITALK_AISTART, NULL);
	}
	else
	{
		/*����ʶ�����Ѿ��ر�*/
		ioctl(fd, SEND_AITALK_AISTOP, NULL);
	}
	close(fd);
	ENDFUNC_USING_BY_VOICE

}

#if 0
void AiTalk::ProhibitEnterIdentifyMode()
{
	BEGINFUNC_USING_BY_VOICE
	
	m_bAllowEnterIdentifyMode = false;
	usleep(300);	//���棬����Ҫ����m_pAiTalkInterface->StopIdentify()���˳�����ʶ��ģʽ
				//���usleep����ȷ��ң�����߳����������m_pAiTalkInterface->StartIdentify()
				//һ�������ǵ���m_pAiTalkInterface->StopIdentify()֮ǰ�����У��������
				//����ֹͣ����ʶ��ң�����ִ�����ʶ����ɺ������
				//�Ǻǣ���ʵ���ֿ�����Ӧ�ú�΢С��
				
	StopIdentify();
	
	ENDFUNC_USING_BY_VOICE
}

void AiTalk::AllowEnterIdentifyMode()
{
	BEGINFUNC_USING_BY_VOICE
	
	m_bAllowEnterIdentifyMode = true;

	ENDFUNC_USING_BY_VOICE
}
#endif

/* ��¼��־�ص����� */
static void ivCall UserLogCB(ivPointer pUserParam, ivCPointer pLogData, ivSize nBytes)
{
	/* TODO: �ͻ�ƽ̨��ȡ��Դ����������ʵ������Windowsƽ̨Ϊ�������ο� */

	FILE *fp = (FILE *)pUserParam;
	fwrite(pLogData, nBytes, 1, fp);
	fflush(fp);
}

/* ��ȡ��Դ�ص����� */
static void ivCall UserReadResCB(ivPointer pParameter, ivPointer pBuffer, ivResAddress iPos, ivResSize nSize)
{
	/* TODO: �ͻ�ƽ̨��ȡ��Դ����������ʵ������Windowsƽ̨Ϊ�������ο� */

	FILE* pFile = (FILE*)pParameter;
	fseek(pFile, iPos, SEEK_SET);
	fread(pBuffer, nSize, 1, pFile);
}

/* �ڴ����ص����� */
static ivPointer ivCall UserReallocCB(ivPointer pUserParam, ivPointer pMemBlock, ivSize nBytes)
{
	pUserParam = NULL;
	return realloc(pMemBlock, nBytes);
}

/* �ڴ��ͷŻص����� */
static void ivCall UserFreeCB(ivPointer pUserParam, ivPointer pMemBlock)
{
	pUserParam = NULL;
	return free(pMemBlock);
}

/* �������ݻص����� */
static ivBool ivCall UserSaveDataCB(ivPointer pUserParam, ivPointer pSrcBuffer, ivSize nBufferBytes)
{
     FILE* fp;
	  pUserParam = NULL;
     fp = fopen(VOICE_IDENTIFY_ADAPT_FILE, "wb+");
     fwrite(pSrcBuffer, nBufferBytes, 1, fp);
     fclose(fp);

     return ivTrue;
}

/* װ�����ݻص����� */
static ivBool ivCall UserLoadDataCB(ivPointer pUserParam, ivPointer pDstBuffer, ivSize ivPtr pnBufferBytes)
{
        FILE* fp;
		  pUserParam = NULL;
        ivUInt32 nSize;
        fp = fopen(VOICE_IDENTIFY_ADAPT_FILE, "rb");
        if(ivNull == fp){
                return ivFalse;
        }
        fseek(fp, 0, SEEK_END);
        nSize = ftell(fp);
        if(nSize > *pnBufferBytes){
                fclose(fp);
                return ivFalse;
        }
        fseek(fp, 0, SEEK_SET);
        fread(pDstBuffer, nSize, 1, fp);
        fclose(fp);

        *pnBufferBytes = nSize;

        return ivTrue;
}

ivBool AiTalk::AitalkDestryObject()
{
	BEGINFUNC_USING_BY_VOICE

	ivESRStatus		iStatus;

	if(m_hESRObj)
	{
		iStatus = EsrDestroy(m_hESRObj);
		if(ivESR_OK != iStatus)
		{
			return ivFalse;
		}
	}

	ENDFUNC_USING_BY_VOICE

	return ivTrue;
}

ivBool AiTalk::AitalkCreateObject()
{
	BEGINFUNC_USING_BY_VOICE

	TUserSys		tUserSys;			/* �û�ϵͳ�ṹ�� */
	ivTResPackDesc	tResPackDesc;		/* ��Դ���������ṹ�� */
	ivESRStatus		iStatus;			/* API���÷���ֵ */


/* ��Aitalk CE 2.2ʹ�þ�̬�ڴ� */
#if 0
	memset(pAiWorkBuffer, 0, sizeof(char) * AIWORKBUFFSIZE);
	memset(pAiResidentBuffer, 0, sizeof(char) * AIRESIBUFFSIZE);

	tUserSys.pWorkBuffer = pAiWorkBuffer;
	tUserSys.nWorkBufferBytes = AIWORKBUFFSIZE;
	tUserSys.pResidentBuffer = pAiResidentBuffer;
	tUserSys.nResidentBufferBytes = AIRESIBUFFSIZE;
	tUserSys.pfnRealloc = ivNull;
	tUserSys.pfnFree = ivNull;
	
#else

	/* ��Aitalk CE 2.2ʹ�ö�̬�ڴ���� */
	tUserSys.pWorkBuffer = ivNull;
	tUserSys.nWorkBufferBytes = 0;
	tUserSys.pResidentBuffer = ivNull;
	tUserSys.nResidentBufferBytes = 0;
	tUserSys.pfnRealloc = UserReallocCB;
	tUserSys.pfnFree = UserFreeCB;
#endif

	tUserSys.pfnSaveData = UserSaveDataCB;
	tUserSys.pfnLoadData = UserLoadDataCB;


	tUserSys.pfnStartRecord = ivNull;
	tUserSys.pfnStopRecord = ivNull;

	tUserSys.bCheckResource = ivTrue;

#if 0
	tUserSys.pCBParam = fopen(AITALK_LOG_FILE, "wb+");
	tUserSys.pfnLog = UserLogCB;
#else
	tUserSys.pCBParam = fopen(AITALK_LOG_FILE, "wb+");
	tUserSys.pfnLog = UserLogCB;
#endif
	tResPackDesc.nCacheBlockCount = 0;
	tResPackDesc.nCacheBlockExt = 0;
	tResPackDesc.nCacheBlockSize = 0;
	tResPackDesc.nSize = 0;
	tResPackDesc.pCacheBlockIndex = ivNull;
	tResPackDesc.pCacheBuffer = ivNull;
	tResPackDesc.pfnMap = ivNull;
	tResPackDesc.pCBParam = m_pResourceFile = fopen(IVESR_FILE, "rb");
	tResPackDesc.pfnRead = UserReadResCB;

	if( NULL == tResPackDesc.pCBParam )
	{
		IBD_PRINTF(4, "[aiTalk.cpp] open ivESR.irf file error\n");
		return ivFalse;
	}

	if(m_hESRObj)
	{
		iStatus = EsrDestroy(m_hESRObj);
		if(ivESR_OK != iStatus)
		{
			/* TODO: ����Aitalk CE 2.2����ʧ�ܺ���� */
			return ivFalse;
		}
	}

	m_hESRObj = ivNull;

	/* ����ʶ������ */
	iStatus = EsrCreate(&m_hESRObj, &tUserSys, &tResPackDesc, (ivSize)1);

	if(ivESR_OK != iStatus)
	{
		/* TODO: ����Aitalk CE 2.2����ʧ�ܺ�������������ϲ������� */
		return ivFalse;
	}

	ENDFUNC_USING_BY_VOICE

	return ivTrue;
}

ivBool AiTalk::AitalkInputTextData()
{
	BEGINFUNC_USING_BY_VOICE

	ivESRStatus iStatus;

	/* ����ʵ� */
	iStatus = EsrSetACP(m_hESRObj, ivESR_CP_GBK);//��ʼ
	
	iStatus = EsrBeginLexiconA(m_hESRObj, (ivCStrA)"141�ʵ�");//�ʵ�����

	for( int i=0; i<g_aitalk_lexicon_setting_num; i++)
	{
		iStatus = EsrAddLexiconItemA(m_hESRObj, (ivCStrA)(g_aitalk_lexicon_setting[i].item), i+1);
	}

	iStatus = EsrEndLexicon(m_hESRObj);//����

	/* ���볡�� */
	iStatus = EsrBeginSceneA(m_hESRObj, (ivCStrA)"141����");

	/* �����ʽ-- ����ֻ��һ����ʽ*/
	iStatus = EsrAddSyntaxA(m_hESRObj, (ivCStrA)"{141�ʵ�}", 1);

	iStatus = EsrEndScene(m_hESRObj);//����

	ENDFUNC_USING_BY_VOICE

	return ivTrue;
}

bool AiTalk::RegisterInterface(IvTTSInterface* pInterface)
{
	bool retval = false;

	if(NULL != pInterface)
	{
		m_pIvTTSInterface = pInterface;
		retval = true;
	}

	return retval;
}
