/**
*<FH+>***************************************************************************
*
* ��Ȩ���� (C)2011, Զ�ؿƼ����޹�˾
* 
* �ļ����ƣ� AiPlay.h
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
#ifndef AIPLAY_H
#define AIPLAY_H

#include <QThread>
#include <QMutex>
#include <list>
#include "alsa/asoundlib.h"

using namespace std;

#define SAMPLERATE 16000
#define CH 1
#define VOICEDATALEN 50000

typedef struct _S_VOICE_DATA
{
    char voiceData[VOICEDATALEN];
    int iLen;
}Voice_Data;


/**
*<CLASS+>********************************************************************
* �� �� ��: AiPlayC
* ��������: �����ϳɲ���
* ����˵��: 
* �޸ļ�¼: 
* �޸�����			�� ��		�޸���		�޸�ժҪ
* -------------------------------------------------------------------------
* [2011/10/19]	    V1.0		������		������
*<CLASS->*********************************************************************
*/
class AiPlayC : public QThread
{
public:
    AiPlayC();
    virtual ~AiPlayC();

    static AiPlayC* object();
    static void releaseObject();

    int voicePlay(const char *strText);
	int writePcm(char *pData, int iLen);
	
private:
    static AiPlayC *thisObject;
    const char *m_strSessionId;
    list<Voice_Data> m_VoiceList;
    static QMutex m_MutexLock;
    volatile bool m_bStopFlag;

    snd_pcm_t *capture_handle;
    snd_pcm_stream_t stream;
    snd_pcm_uframes_t frames;


    int init();
    int sendText(const char *strText);
    int getVoice();
    int playText();
    void releaseSessionResourch();
    void run();

    int InitPcm();
    int pcmDataPlay();
};

#endif