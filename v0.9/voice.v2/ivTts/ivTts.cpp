#include <sys/time.h>
#include "ivTts.h"
#include "debugMonitor.h"
#include "audioControlLib.h"
#include "pcmPlayerManager.h"


/* constant for TTS heap size */
#define ivTTS_HEAP_SIZE		50000 /* ��ϣ���Ч */


/* Message */
ivTTSErrID DoMessage()
{
	/* ��ȡ��Ϣ���û�ʵ�� */
	if(1)
	{
		/* �����ϳ� */
		return ivTTS_ERR_OK;
	}
	else
	{
		/* �˳��ϳ� */
		return ivTTS_ERR_EXIT;
	}
}


/* output callback */
ivTTSErrID OnOutput(
		ivUInt16		nCode,			/* [in] output data code */
		ivCPointer		pcData,			/* [in] output data buffer */
		ivSize			nSize )			/* [in] output data size */
{
	PcmPlayerManagerC::object()->playPcm((char*)pcData, nSize,  AudioControl1::AUDIO_CHANNEL_ID_DIGITAL_VOICE_TIP, true);

	return ivTTS_ERR_OK;
}

/* read resource callback */
ivBool ivCall ReadResCB(
		ivPointer		pParameter,		/* [in] user callback parameter */
		ivPointer		pBuffer,		/* [out] read resource buffer */
		ivResAddress	iPos,			/* [in] read start position */
		ivResSize		nSize )			/* [in] read size */
{
	FILE* pFile = (FILE*)pParameter;
	int err;

	fseek(pFile, iPos, SEEK_SET);
	err = fread(pBuffer, 1, nSize, pFile);

	if ( err == (int)nSize )
		return ivTrue;
	else
		return ivFalse;
}

/* output callback */
ivTTSErrID ivCall OutputCB(
		ivPointer		pParameter,		/* [in] user callback parameter */
		ivUInt16		nCode,			/* [in] output data code */
		ivCPointer		pcData,			/* [in] output data buffer */
		ivSize			nSize )			/* [in] output data size */
{
	/* ��ȡ�߳���Ϣ���Ƿ��˳��ϳ� */
	pParameter = NULL;
	ivTTSErrID tErr = DoMessage();

	if ( tErr != ivTTS_ERR_OK ) return tErr;

	/* ������������ȥ���� */
	return OnOutput(nCode, pcData, nSize);
}





IvTtsC* IvTtsC::thisObject = NULL;

IvTtsC* IvTtsC::object()
{
	IvTtsC *pTmp = NULL;
	if (thisObject == NULL)
	{
		thisObject = new IvTtsC();
	}

	pTmp = thisObject;

	return pTmp;
}

void IvTtsC::releaseObject()
{
	if (thisObject != NULL)
	{
		delete thisObject;
		thisObject = NULL;
	}
}

IvTtsC::IvTtsC()
{
	_ttsCreate();
}

IvTtsC::~IvTtsC()
{
	_ttsRelease();
}

void IvTtsC::_ttsCreate(void)
{
	ivTTSErrID ivReturn;

	usleep(60000);
	/* ����� */
	m_pHeap = (ivPByte)malloc(ivTTS_HEAP_SIZE);
	memset(m_pHeap, 0, ivTTS_HEAP_SIZE);

	/* ��ʼ����Դ */
	/* �����ж����Դ�������Էְ�*/
	m_tResPackDesc.pCBParam = fopen("Resource.irf", "rb");
	if( NULL == m_tResPackDesc.pCBParam )
	{
		printf("can not open Resource.irf\n");
		abort();
	}
	
	m_tResPackDesc.pfnRead = ReadResCB;
	m_tResPackDesc.pfnMap = NULL;
	m_tResPackDesc.nSize = 0;
	
	/* TTS�ڲ�ʹ�� */
	m_tResPackDesc.pCacheBlockIndex = NULL;
	m_tResPackDesc.pCacheBuffer = NULL;
	m_tResPackDesc.nCacheBlockSize = 0;
	m_tResPackDesc.nCacheBlockCount = 0;
	m_tResPackDesc.nCacheBlockExt = 0;

		
	/* ���� TTS ʵ�� */
//	ivReturn = ivTTS_Create(&hTTS, (ivPointer)pHeap, ivTTS_HEAP_SIZE, ivNull, (ivPResPackDescExt)&tResPackDesc, (ivSize)1,&tUserInfo);
	ivReturn = ivTTS_Create(&m_hTTS, (ivPointer)m_pHeap, ivTTS_HEAP_SIZE, ivNull, (ivPResPackDescExt)&m_tResPackDesc, (ivSize)1, NULL);

	/* ������Ƶ����ص� */
	ivReturn = ivTTS_SetParam(m_hTTS, ivTTS_PARAM_OUTPUT_CALLBACK, (ivUInt32)OutputCB);
	
	/* ���������ı�����ҳ */
	ivReturn = ivTTS_SetParam(m_hTTS, ivTTS_PARAM_INPUT_CODEPAGE, ivTTS_CODEPAGE_GBK);

	/* �������� */
	ivReturn = ivTTS_SetParam(m_hTTS, ivTTS_PARAM_LANGUAGE, ivTTS_LANGUAGE_AUTO);	

	/* �������� */
	ivReturn = ivTTS_SetParam(m_hTTS, ivTTS_PARAM_VOLUME, ivTTS_VOLUME_NORMAL);

	/*��������*/
	ivReturn = ivTTS_SetParam(m_hTTS, ivTTS_PARAM_VOICE_SPEED, ivTTS_VOLUME_NORMAL);
	

	/************************************************************************
		��ʽ�ϳ�
	************************************************************************/
	/* ���÷�����Ϊ XIAOYAN */
	ivReturn = ivTTS_SetParam(m_hTTS, ivTTS_PARAM_ROLE, ivTTS_ROLE_XIAOYAN);
}

void IvTtsC::_ttsRelease()
{
	/* 逆初始化 */

	ivTTS_Destroy(m_hTTS);

	m_hTTS = NULL;

	if( m_tResPackDesc.pCacheBlockIndex )
	{
		free(m_tResPackDesc.pCacheBlockIndex);
		m_tResPackDesc.pCacheBlockIndex = NULL;
	}

	if( m_tResPackDesc.pCacheBuffer )
	{
		free(m_tResPackDesc.pCacheBuffer);
		m_tResPackDesc.pCacheBuffer = NULL;
	}

	if( m_pHeap )
	{
		free(m_pHeap);
		m_pHeap= NULL;
	}

/*	if( m_tResPackDesc.pCBParam )
	{
		if(g_pRes != NULL)
		{
			free(g_pRes);
			g_pRes = NULL;
		}

		m_tResPackDesc.pCBParam= NULL;
	}*/
}

int IvTtsC::PlayText(const char* words)
{
	int i = 0;
	char cStr[512];
	sprintf(cStr, "��%s", words);

	int iRet = AudioControl1::AudioControlLibC::object()->open(AudioControl1::AUDIO_CHANNEL_ID_DIGITAL_VOICE_TIP, true);

	printf("[Voice1Test] open Voice tip audio channel ret[%d]\n", iRet);
	if (iRet < AC_E_OK)
	{
	    perror("[Voice1Test] open Voice tip audio channel error:");
	}
	
	ivTTS_SynthText(m_hTTS, ivText(cStr), -1);
	AudioControl1::AudioControlLibC::object()->close(AudioControl1::AUDIO_CHANNEL_ID_DIGITAL_VOICE_TIP, true);
	return 0;
}



