#include <sys/time.h>
#include "AisoundTTS.h"
#include "voice_setting.h"
#include "debugMonitor.h"

/*�����ϳɵ�PCM�ļ��ľ��*/
//static FILE*	fpOutput = NULL;

static snd_pcm_t* handle;


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
	BEGINFUNC_USING_BY_VOICE
	#if 1
	/* play */
	/* ����ʵ��ƽ̨���������ݴ��������ӿڣ�����ֻ�Ǽ򵥵Ľ��������ݱ������ļ��� */
	nCode = NULL;
	int err;
#if 1
	err = snd_pcm_writei(handle, pcData, nSize/2);
	if ( err == -EPIPE)
	{
		dbug_out("underrun occurred! err: %s\n", snd_strerror(err));
		if( (err = snd_pcm_prepare(handle)) < 0)
		{
			dbug_out("snd_pcm_prepare_failed! err: %s\n", snd_strerror(err));
		}
	}
#endif
	#endif
	ENDFUNC_USING_BY_VOICE
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

/* parameter change callback */
ivTTSErrID ivCall Aisound::ParamChangeCB(
		ivPointer       pParameter,		/* [in] user callback parameter */
		ivUInt32		nParamID,		/* [in] parameter id */
		ivUInt32		nParamValue )	/* [in] parameter value */
{
	pParameter = NULL;
	nParamID = NULL;
	nParamValue = NULL;
	return ivTTS_ERR_OK;
}

/* progress callback */
ivTTSErrID ivCall Aisound::ProgressCB(
		ivPointer       pParameter,		/* [in] user callback parameter */
		ivUInt32		iProcPos,		/* [in] current processing position */
		ivUInt32		nProcLen )		/* [in] current processing length */
{
	pParameter = NULL;
	iProcPos = NULL;
	nProcLen = NULL;
	return ivTTS_ERR_OK;
}


ivUInt16 ivCall LogCB(
					  ivPointer	pParameter,			//[out] user callback parameter 
					  ivCPointer	pcData,				// [in] output data buffer 
					  ivSize		nSize				// [in] output data size
					  )
{
	FILE *pFile = (FILE*)pParameter;
	ivSize nWriten = fwrite(pcData,1,nSize,pFile);
	if( nWriten == nSize )
		return ivTTS_ERR_OK;
	else
		return ivTTS_ERR_FAILED;
}


Aisound::Aisound()
{
	ivTTSErrID ivReturn;

	usleep(60000);
	/* ����� */
	pHeap = (ivPByte)malloc(ivTTS_HEAP_SIZE);
	memset(pHeap, 0, ivTTS_HEAP_SIZE);

	/* ��ʼ����Դ */
	/* �����ж����Դ�������Էְ�*/
	tResPackDesc.pCBParam = fopen(RESOURCE_FILE, "rb");
	if( NULL == tResPackDesc.pCBParam )
	{
		throw;
	}
	
	tResPackDesc.pfnRead = ReadResCB;
	tResPackDesc.pfnMap = NULL;
	tResPackDesc.nSize = 0;
	
	/* TTS�ڲ�ʹ�� */
	tResPackDesc.pCacheBlockIndex = NULL;
	tResPackDesc.pCacheBuffer = NULL;
	tResPackDesc.nCacheBlockSize = 0;
	tResPackDesc.nCacheBlockCount = 0;
	tResPackDesc.nCacheBlockExt = 0;

		
	/* ���� TTS ʵ�� */
//	ivReturn = ivTTS_Create(&hTTS, (ivPointer)pHeap, ivTTS_HEAP_SIZE, ivNull, (ivPResPackDescExt)&tResPackDesc, (ivSize)1,&tUserInfo);
	ivReturn = ivTTS_Create(&hTTS, (ivPointer)pHeap, ivTTS_HEAP_SIZE, ivNull, (ivPResPackDescExt)&tResPackDesc, (ivSize)1, NULL);

	/* ������Ƶ����ص� */
	ivReturn = ivTTS_SetParam(hTTS, ivTTS_PARAM_OUTPUT_CALLBACK, (ivUInt32)OutputCB);
	
	/* ���������ı�����ҳ */
	ivReturn = ivTTS_SetParam(hTTS, ivTTS_PARAM_INPUT_CODEPAGE, ivTTS_CODEPAGE_GBK);

	/* �������� */
	ivReturn = ivTTS_SetParam(hTTS, ivTTS_PARAM_LANGUAGE, ivTTS_LANGUAGE_AUTO);	

	/* �������� */
	ivReturn = ivTTS_SetParam(hTTS, ivTTS_PARAM_VOLUME, ivTTS_VOLUME_NORMAL);

	/*��������*/
	ivReturn = ivTTS_SetParam(hTTS, ivTTS_PARAM_VOICE_SPEED, ivTTS_VOLUME_NORMAL);
	

	/************************************************************************
		��ʽ�ϳ�
	************************************************************************/
	/* ���÷�����Ϊ XIAOYAN */
	ivReturn = ivTTS_SetParam(hTTS, ivTTS_PARAM_ROLE, ivTTS_ROLE_XIAOYAN);
}

Aisound::~Aisound()
{
	/* �ͷ�TTS ʵ��*/
	ivTTS_Destroy(hTTS);

	if( NULL != tResPackDesc.pCacheBlockIndex )
	{
		free(tResPackDesc.pCacheBlockIndex);

		tResPackDesc.pCacheBlockIndex = NULL;
	}

	if( NULL != tResPackDesc.pCacheBuffer )
	{
		free(tResPackDesc.pCacheBuffer);

		tResPackDesc.pCacheBuffer = NULL;
	}

	if( NULL != tResPackDesc.pCBParam )
	{
		fclose((FILE*)tResPackDesc.pCBParam);

		tResPackDesc.pCBParam = NULL;
	}

	if ( NULL != pHeap )
	{
		free(pHeap);

		pHeap = NULL;
	}
}

int Aisound::Open_Pcm()
{
	BEGINFUNC_USING_BY_VOICE
	int err;
	int dir;
	int size;
	char *buffer;
	unsigned int val;
	snd_pcm_uframes_t frames;
	snd_pcm_hw_params_t* params;
	int channels = 0;
	if ( (err = snd_pcm_open(&handle, "plug:cmd", SND_PCM_STREAM_PLAYBACK, 0)) < 0)
	{
		dbug_out("cannot open audio device (%s)\n", snd_strerror (err));
		goto OpenPcmFail;
	}
	dbug_out("After snd_pcm_open for SND_PCM_STREAM_PLAYBACK");
	
	snd_pcm_hw_params_alloca(&params);//为参数变量分配空间
	
	if( (err = snd_pcm_hw_params_any(handle, params)) < 0)//参数初始化
	{
		dbug_out("snd_pcm_hw_params_any failed!	 err = %d\n", err);
		goto OpenPcmFail;
	}
	if( (err = snd_pcm_hw_params_set_access(handle, params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0)//设置为交错模式
	{
		dbug_out("snd_pcm_hw_params_set_access failed!	 err = %d\n", err);
		goto OpenPcmFail;
	}
	if( (err = snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_S16_LE)) < 0)//使用16位样本
	{
		dbug_out("snd_pcm_hw_params_set_format failed!	 err = %d\n", err);
		goto OpenPcmFail;
	}
	channels = val = 1;
	if( (err = snd_pcm_hw_params_set_channels(handle, params, val)) < 0)//设置为立体声
	{
		dbug_out("snd_pcm_hw_params_set_channels failed!	 err = %d\n", err);
		goto OpenPcmFail;
	}
	val = 16000;
	if( (err = snd_pcm_hw_params_set_rate_near(handle, params, &val, &dir)) < 0)//设置采样率为44.1KHz
	{
		dbug_out("snd_pcm_hw_params_set_rate_near failed!   err = %d\n", err);
		goto OpenPcmFail;
	}


#if 1
	frames = 32;
	if( (err = snd_pcm_hw_params_set_period_size_near(handle, params, &frames, &dir)) < 0)
	{
		dbug_out("snd_pcm_hw_params_set_period_size_near failed!	err = %d\n", err);
		goto OpenPcmFail;
	}
	if ( (err = snd_pcm_hw_params(handle, params)) < 0)
	{
		dbug_out("snd_pcm_hw_params-failed!	err = %d err: %s\n", err, snd_strerror(err));
		goto OpenPcmFail;
	}
	if( (err = snd_pcm_hw_params_get_period_size(params, &frames, &dir)) < 0)
	{
		dbug_out("snd_pcm_hw_params_get_period_size failed!	 err = %d\n", err);
		goto OpenPcmFail;
	}
	size = frames * channels * 2;
	buffer = (char *)malloc(sizeof(char) * size);

	if( (err = snd_pcm_hw_params_get_period_time(params, &val, &dir)) < 0)
	{
		dbug_out("snd_pcm_hw_params_get_period_time failed!	err = %d\n", err);
		goto OpenPcmFail;
	}
	
	return 0;
	
OpenPcmFail:
	return -1;
#endif 
	ENDFUNC_USING_BY_VOICE
}

void Aisound::Release_Pcm()
{
	BEGINFUNC_USING_BY_VOICE
	snd_pcm_close(handle);
	ENDFUNC_USING_BY_VOICE
}

int Aisound::PlayText(const char* words)
{
	BEGINFUNC_USING_BY_VOICE
	int i = 0;
	char cStr[512];
	sprintf(cStr, "��%s", words);

OpenAG:
	if(handle != NULL)
		Release_Pcm();

//	usleep(30000);

	if(Open_Pcm() < 0)
	{
		if(i < 5)
		{
			i++;
			Release_Pcm();
			goto OpenAG;
		}
		else
		{
			dbug_out("Open Pcm ERROR....");
			return -1;
		}
	}
	sleep(1);

	ivTTS_SynthText(hTTS, ivText(cStr), -1);

	return 0;
ENDFUNC_USING_BY_VOICE
}



