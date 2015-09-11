/**
*<FH+>***************************************************************************
*
* ��Ȩ���� (C)2011, Զ�ؿƼ����޹�˾
* 
* �ļ����ƣ� AiPlay.cpp
* �ļ���ʶ�� (�����ù���ƻ���)
* ����ժҪ�� 
* ����˵���� 
* ��ǰ�汾�� V1.0
* ��    �ߣ� ������
* ������ڣ� [2011/10/19]
* �޸ļ�¼1��
*    �޸����ڣ�
*    �� �� �ţ�
*    �� �� �ˣ�
*    �޸����ݣ� 
*<FH+>***************************************************************************
*/
#include <stdlib.h>
#include "AiPlay.h"
#include <stdio.h>
#include <string.h>
#include "qtts.h"

AiPlayC *AiPlayC::thisObject = NULL;
QMutex AiPlayC::m_MutexLock;

AiPlayC::AiPlayC()
{

	InitPcm();
}

AiPlayC::~AiPlayC()
{
}

/**
*<FUNC+>********************************************************************
* �������ƣ� object
* ���������� ��ȡ����
* ��������� ��
* ��������� ��
* �� ��	ֵ�� �ɹ����ض��󣬷��򷵻�null
* ����˵���� 
* �޸ļ�¼�� 
* 		�޸�����        �汾��     �޸���	      �޸�����
* --------------------------------------------------------------------------
* 	[2011/10/25]	    V1.0	   ������		  ��������
*<FUNC+>********************************************************************
*/
AiPlayC* AiPlayC::object()
{
    AiPlayC *pTmp = NULL;

    if (thisObject == NULL)
    {
        thisObject = new AiPlayC;
    }

    pTmp = thisObject;

    return pTmp;
}

/**
*<FUNC+>********************************************************************
* �������ƣ� releaseObject
* ���������� �ͷ���Դ
* ��������� ��
* ��������� ��
* �� ��	ֵ�� ��
* ����˵���� 
* �޸ļ�¼�� 
* 		�޸�����        �汾��     �޸���	      �޸�����
* --------------------------------------------------------------------------
* 	[2011/10/25]	    V1.0	   ������		  ��������
*<FUNC+>********************************************************************
*/
void AiPlayC::releaseObject()
{
    if (thisObject != NULL)
    {
        delete thisObject;
        thisObject = NULL;
    }
}
/**
*<FUNC+>********************************************************************
* �������ƣ� VoicePlay
* ���������� �ϳɲ����ı�
* ��������� strText��Ҫ�ϳɵ��ı�
* ��������� ��
* �� ��	ֵ�� 
* ����˵���� 
* �޸ļ�¼�� 
* 		�޸�����        �汾��     �޸���	      �޸�����
* --------------------------------------------------------------------------
* 	[2011/10/19]	    V1.0	   ������		  ��������
*<FUNC+>********************************************************************
*/
int AiPlayC::voicePlay(const char *strText)
{
    if (strText == NULL)
    {
        printf("voicePlay, strText is null");
        return -1;
    }

    int iRet = 0;
    m_VoiceList.clear();
    m_bStopFlag = false;

    // ��ʼ��
    iRet = init();
    if (iRet != 0)
    {
        printf("voicePlay, init failed %d\n", iRet);
        releaseSessionResourch();
        return iRet;
    }

    // �����ַ�����������
    iRet = sendText(strText);
    if (iRet != 0)
    {
        printf("voicePlay, sendText failed %d\n", iRet);
        releaseSessionResourch();
        return iRet;
    }

    // ��ȡ��Ƶ��������
    iRet = getVoice();
    if (iRet != 0)
    {
        printf("voicePlay, getVoice failed %d\n", iRet);
        releaseSessionResourch();
        return iRet;
    }


    wait();

    // �ͷ���Դ
    releaseSessionResourch();

    return iRet;
}

/**
*<FUNC+>********************************************************************
* �������ƣ� init
* ���������� ��ʼ��
* ��������� ��
* ��������� ��
* �� ��	ֵ�� �ɹ�����0�����򷵻ش�����
* ����˵���� 
* �޸ļ�¼�� 
* 		�޸�����        �汾��     �޸���	      �޸�����
* --------------------------------------------------------------------------
* 	[2011/11/1]	    V1.0	   ������		  ��������
*<FUNC+>********************************************************************
*/
int AiPlayC::init()
{
    int iRet = 0;
    const char *strParams = "ssm=1, auf=audio/L16;rate=16000, aue=speex-wb, ent=intp65";

    iRet = QTTSInit("");
    if (0 != iRet)
    {
        printf("QTTSInit failed, error code is %d\n", iRet);
        return iRet;
    }

    // ����һ·�Ự
    m_strSessionId = QTTSSessionBegin(strParams, &iRet);
    if (0 != iRet)
    {
        printf("QTTSSessionBegin failed, error code is %d\n", iRet);
        return iRet;
    }

    return iRet;
}

/**
*<FUNC+>********************************************************************
* �������ƣ� sendText
* ���������� ����Ҫ�ϳɵ��ı�����������
* ��������� strText��Ҫ�ϳ��ı�
* ��������� ��
* �� ��	ֵ�� �ɹ�����0�����򷵻ش�����
* ����˵���� 
* �޸ļ�¼�� 
* 		�޸�����        �汾��     �޸���	      �޸�����
* --------------------------------------------------------------------------
* 	[2011/11/1]	    V1.0	   ������		  ��������
*<FUNC+>********************************************************************
*/
int AiPlayC::sendText(const char *strText)
{
    if (strText == NULL)
    {
        printf("sendText, strText is null\n");
        return -1;
    }

    int iRet = 0;

    iRet = QTTSTextPut(m_strSessionId, strText, strlen(strText), NULL);
    if (iRet != 0)
    {
        printf("QTTSTextPut failed, error code is %d\n", iRet);
        return iRet;
    }
	return 0;
}

/**
*<FUNC+>********************************************************************
* �������ƣ� getVoice
* ���������� ��ȡ��������
* ��������� ��
* ��������� ��
* �� ��	ֵ�� �ɹ�����0�����򷵻ش�����
* ����˵���� 
* �޸ļ�¼�� 
* 		�޸�����        �汾��     �޸���	      �޸�����
* --------------------------------------------------------------------------
* 	[2011/11/1]	    V1.0	   ������		  ��������
*<FUNC+>********************************************************************
*/
int AiPlayC::getVoice()
{
    int iRet = 0;
    unsigned int iVoiceDataLen = 0;
    int iVoiceStatus = 0;
    const char *voiceData = NULL;
    int iStartFlag = 0;

    while (TTS_FLAG_DATA_END != iVoiceStatus)
    {
        voiceData = (char *)QTTSAudioGet(m_strSessionId, &iVoiceDataLen, &iVoiceStatus, &iRet);
        if (0 != iRet)
        {
            printf("QTTSAudioGet failed, error code is %d\n", iRet);
            m_bStopFlag = true;
            break;
        }

        if (voiceData != NULL && iVoiceDataLen != 0)
        {
            Voice_Data tempVoiceData;

            if (iVoiceDataLen <= VOICEDATALEN)
            {
                tempVoiceData.iLen = iVoiceDataLen;

                printf("iVoiceLen = %d\n", iVoiceDataLen);
                memset(tempVoiceData.voiceData, 0, VOICEDATALEN);
                memcpy(tempVoiceData.voiceData, voiceData, iVoiceDataLen);
            }
            else
            {
                tempVoiceData.iLen = VOICEDATALEN;

                printf("iVoiceLen = %d\n", iVoiceDataLen);
                memset(tempVoiceData.voiceData, 0, VOICEDATALEN);
                memcpy(tempVoiceData.voiceData, voiceData, VOICEDATALEN);
            }

            m_MutexLock.lock();
            m_VoiceList.push_back(tempVoiceData);
            m_MutexLock.unlock();

            // ��һ�λ�ȡ�����ݣ���ʼ�̲߳���
            if (iStartFlag == 0)
            {
                playText();
            }

            iStartFlag++;
        }
    }

    // ֹͣ�����߳�
    m_bStopFlag = true;

    return iRet;
}

/**
*<FUNC+>********************************************************************
* �������ƣ� playText
* ���������� ���Ż����е�����
* ��������� ��
* ��������� �� 
* �� ��	ֵ�� �ɹ�����0�����򷵻ش�����
* ����˵���� 
* �޸ļ�¼�� 
* 		�޸�����        �汾��     �޸���	      �޸�����
* --------------------------------------------------------------------------
* 	[2011/11/1]	    V1.0	   ������		  ��������
*<FUNC+>********************************************************************
*/
int AiPlayC::playText()
{
    int iRet = 0;

    iRet = InitPcm();
    if (iRet < 0)
    {
        // ��ʼ��
        printf("playText, InitPcm failed, %d\n", iRet);
        return iRet;
    }

    start();

    return iRet;
}

/**
*<FUNC+>********************************************************************
* �������ƣ� InitPcm
* ���������� ��ʼ��������
* ��������� ��
* ��������� ��
* �� ��	ֵ�� �ɹ�����0�����򷵻ش�����
* ����˵���� 
* �޸ļ�¼�� 
* 		�޸�����        �汾��     �޸���	      �޸�����
* --------------------------------------------------------------------------
* 	[2011/11/1]	    V1.0	   ������		  ��������
*<FUNC+>********************************************************************
*/
int AiPlayC::InitPcm()
{
    int err;
    int dir;
    int size;
    unsigned int val;
    snd_pcm_hw_params_t* params;
    int channels = 0;

    if ( (err = snd_pcm_open(&capture_handle, "plug:tcc", SND_PCM_STREAM_PLAYBACK, 0)) < 0)
    {
        fprintf(stderr, "cannot open audio device (%s)", snd_strerror (err));
        goto OpenPcmFail;
    }

    snd_pcm_hw_params_alloca(&params);

    if( (err = snd_pcm_hw_params_any(capture_handle, params)) < 0)
    {
        fprintf(stderr, "snd_pcm_hw_params_any failed!  err = %d\n", err);
        goto OpenPcmFail;
    }

    if( (err = snd_pcm_hw_params_set_access(capture_handle, params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0)
    {
        fprintf(stderr, "snd_pcm_hw_params_set_access failed! err = %d\n", err);
        goto OpenPcmFail;
    }
    if( (err = snd_pcm_hw_params_set_format(capture_handle, params, SND_PCM_FORMAT_S16_LE)) < 0)
    {
        fprintf(stderr, "snd_pcm_hw_params_set_format failed!  err = %d\n", err);
        goto OpenPcmFail;
    }

    channels = CH;
    if( (err = snd_pcm_hw_params_set_channels(capture_handle, params, channels)) < 0)
    {
        fprintf(stderr, "snd_pcm_hw_params_set_channels failed!	 err = %d\n", err);
        goto OpenPcmFail;
    }

    val = SAMPLERATE;
    if( (err = snd_pcm_hw_params_set_rate_near(capture_handle, params, &val, &dir)) < 0)
    {
        fprintf(stderr, "snd_pcm_hw_params_set_rate_near failed!	err = %d\n", err);
        goto OpenPcmFail;
    }

    frames = 4096;

    if( (err = snd_pcm_hw_params_set_period_size_near(capture_handle, params, &frames, &dir)) < 0)
    {
        fprintf(stderr, "snd_pcm_hw_params_set_period_size_near failed!	err = %d\n", err);
        goto OpenPcmFail;
    }
    if ( (err = snd_pcm_hw_params(capture_handle, params)) < 0)
    {
        fprintf(stderr, "snd_pcm_hw_params-failed! err = %d err: %s\n", err, snd_strerror(err));
        goto OpenPcmFail;
    }

    if( (err = snd_pcm_hw_params_get_period_size(params, &frames, &dir)) < 0)
    {
        fprintf(stderr, "snd_pcm_hw_params_get_period_size failed!  err = %d\n", err);
        goto OpenPcmFail;
    }
    size = 160;

    if( (err = snd_pcm_hw_params_get_period_time(params, &val, &dir)) < 0)
    {
        fprintf(stderr, "snd_pcm_hw_params_get_period_time failed! err = %d\n", err);
        goto OpenPcmFail;
    }

    return 0;

OpenPcmFail:
    return -1;
}

/**
*<FUNC+>********************************************************************
* �������ƣ� pcmDataPlay
* ���������� ��������
* ��������� ��
* ��������� ��
* �� ��	ֵ�� �ɹ�����0�����󷵻ش�����
* ����˵���� 
* �޸ļ�¼�� 
* 		�޸�����        �汾��     �޸���	      �޸�����
* --------------------------------------------------------------------------
* 	[2011/11/1]	    V1.0	   ������		  ��������
*<FUNC+>********************************************************************
*/
int AiPlayC::pcmDataPlay()
{
    int iRet = 0;

    m_MutexLock.lock();
    list<Voice_Data>::iterator voiceIter = m_VoiceList.begin();
    for (; voiceIter != m_VoiceList.end(); ++voiceIter)
    {
        // ���Ȳ���2��������
        iRet = snd_pcm_writei(capture_handle, voiceIter->voiceData, voiceIter->iLen / 2);
    }

    m_VoiceList.clear();
    m_MutexLock.unlock();

    return iRet;
}

int AiPlayC::writePcm(char *pData, int iLen)
{
    int err = 0;

    //m_MutexLock.lock();

    err = snd_pcm_writei(capture_handle, pData, iLen/2);
        if( err == -EPIPE)
        {
            fprintf(stderr,"snd_pcm_writei error! err: %s\n", snd_strerror(err));
            if( (err = snd_pcm_prepare(capture_handle)) < 0)
            {
                    fprintf(stderr,"snd_pcm_prepare_failed! err: %s\n", snd_strerror(err));
            }
        }
        else if (err < 0)
        {
            fprintf(stderr,"err writei!\n");
            //break;
        }
        else
        {
           //position+=playLen;
        }
		
    //m_MutexLock.unlock();

    return err;
}

/**
*<FUNC+>********************************************************************
* �������ƣ� releaseSessionResourch
* ���������� �ͷŻỰ��Դ
* ��������� ��
* ��������� ��
* �� ��	ֵ�� ��
* ����˵���� 
* �޸ļ�¼�� 
* 		�޸�����        �汾��     �޸���	      �޸�����
* --------------------------------------------------------------------------
* 	[2011/11/1]	    V1.0	   ������		  ��������
*<FUNC+>********************************************************************
*/
void AiPlayC::releaseSessionResourch()
{
    if (m_strSessionId != NULL)
    {
        QTTSSessionEnd(m_strSessionId, NULL);
        m_strSessionId = NULL;
    }

    QTTSFini();
}

/**
*<FUNC+>********************************************************************
* �������ƣ� run
* ���������� �߳�ִ�к���
* ��������� ��
* ��������� ��
* �� ��	ֵ�� ��
* ����˵���� 
* �޸ļ�¼�� 
* 		�޸�����        �汾��     �޸���	      �޸�����
* --------------------------------------------------------------------------
* 	[2011/11/11]	    V1.0	   ������		  ��������
*<FUNC+>********************************************************************
*/
// ��������
void AiPlayC::run()
{
    while (1)
    {
        // ��鵱ǰ�������Ƿ������ݣ������ݲ�����Ҫֹͣ��ֹͣ�����򲥷������Ļ�����ֹͣ
        if (m_bStopFlag && m_VoiceList.empty())
        {
            break;
        }

        pcmDataPlay();
    }
}
