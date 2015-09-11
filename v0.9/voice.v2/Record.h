/**
*<FH+>***************************************************************************
*
* ��Ȩ���� (C)2011, Զ�ؿƼ����޹�˾
* 
* �ļ����ƣ� VoiceRecord.h
* �ļ���ʶ�� (�����ù���ƻ���)
* ����ժҪ�� ����¼������
* ����˵���� 
* ��ǰ�汾�� V1.0
* ��    �ߣ� ������
* ������ڣ� [2011/10/17]
* �޸ļ�¼1��
*    �޸����ڣ�
*    �� �� �ţ�
*    �� �� �ˣ�
*    �޸����ݣ� 
*<FH+>***************************************************************************
*/ 
#ifndef VOICERECORD_H
#define VOICERECORD_H

#include <list>
#include <QThread>
#include <QMutex>
#include "alsa/asoundlib.h"

#define SAMPLERATE 16000
#define SAMPLERATEEIGHT 8000
#define CH 1

using namespace std;

typedef struct _S_AUDIO_DATA
{
    char audioData[1024];
    int iLen;
}Audio_Data;


/**
*<CLASS+>********************************************************************
* �� �� ��: VoiceRecordC
* ��������: ������˷�¼��
* ����˵��: 
* �޸ļ�¼: 
* �޸�����			�� ��		�޸���		�޸�ժҪ
* -------------------------------------------------------------------------
* [2011/10/17]	    V1.0		������		������
*<CLASS->*********************************************************************
*/
class RecordC : public QThread
{
public:
    RecordC();
    virtual ~RecordC();

    static RecordC* object();
    static void releaseObject();

    void getAudioData(list<Audio_Data> &audioData);

    int startVoiceRecord();
    void stopVoiceRecord();

private:
    static RecordC *thisObject;
    snd_pcm_stream_t m_stream;
    snd_pcm_t *m_capture_handle;
    snd_pcm_uframes_t m_frames;

    // ������У��洢��Ƶ����
    static list<Audio_Data> m_AudioData;

    // ֹͣ��־
    volatile bool m_StopFlag;

    static QMutex m_MutexLock;

    void run();
    int initVoiceRecord();
};

#endif


