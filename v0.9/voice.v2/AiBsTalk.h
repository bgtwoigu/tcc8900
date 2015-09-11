/**
*<FH+>***************************************************************************
*
* ��Ȩ���� (C)2011, Զ�ؿƼ����޹�˾
* 
* �ļ����ƣ� VoiceRecogImpl.h
* �ļ���ʶ�� (�����ù���ƻ���)
* ����ժҪ�� ����ʶ���ʵ��
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

#ifndef VOICERECOGIMPL_H
#define VOICERECOGIMPL_H

#include "qisr.h"
#include <stdio.h>
#include <string>
#include <list>
#include <QThread>
#include <QMutex>
#include "msp_errors.h"
#include "myDbus.h"

using namespace std;
using namespace Voice1;

/**
*<CLASS+>********************************************************************
* �� �� ��: VoiceRecogImplC
* ��������: ����ʶ���ʵ��
* ����˵��: 
* �޸ļ�¼: 
* �޸�����			�� ��		�޸���		�޸�ժҪ
* -------------------------------------------------------------------------
* [2011/10/17]	    V1.0		������		������
*<CLASS->*********************************************************************
*/
class AiBsTalkC : public QThread
{
public:
    AiBsTalkC();
    virtual ~AiBsTalkC();

    static AiBsTalkC* object();
    static void releaseObject();

    int openVoiceRecog();
    int closeVoiceRecog();

private:
    const char *m_strSessionId;
    string m_strResult;

    static AiBsTalkC *thisObject;
    volatile bool m_bFlag;
    int m_iTimerId;
    //QTimer *m_stopThread;

    void run();
    int initVoiceRecog();
    int sendAudioData(char *audioData, size_t iWaveLen);
    string readAudioResult(int iRsltStatus, int &iRtnValue);
    void releaseSessionResourch();
    void closeSoundControl();
    void playVoiceFinishSound();
    void stopRecorze();
    void timerEvent(QTimerEvent *event);
};

#endif