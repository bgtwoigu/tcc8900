/**
*<FH+>***************************************************************************
*
* ��Ȩ���� (C)2011, Զ�ؿƼ����޹�˾
* 
* �ļ����ƣ� AiTalk.h
* �ļ���ʶ�� (�����ù���ƻ���)
* ����ժҪ�� 
* ����˵���� 
* ��ǰ�汾�� V1.0
* ��    �ߣ� ������
* ������ڣ� [2011/10/20]
* �޸ļ�¼1��
*    �޸����ڣ�
*    �� �� �ţ�
*    �� �� �ˣ�
*    �޸����ݣ� 
*<FH+>***************************************************************************
*/ 

#ifndef AITALK_H
#define AITAKL_H

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
using namespace AudioControl1;

/**
*<CLASS+>********************************************************************
* �� �� ��: AiTalkC
* ��������: ����ʶ������
* ����˵��: 
* �޸ļ�¼: 
* �޸�����			�� ��		�޸���		�޸�ժҪ
* -------------------------------------------------------------------------
* [2011/10/20]	    V1.0		������		������
*<CLASS->*********************************************************************
*/
class AiTalkC : public QThread
{
public:
    AiTalkC();
    virtual ~AiTalkC();

    static AiTalkC* object();
    static void releaseObject();

    int startVoiceRecorg();
    int stopVoiceRecorg();

private:
    static AiTalkC *thisObject;

    const char *m_strSessionId;
    char *m_strUploadId;
    string m_strResult;
    volatile bool m_bFlag;

    void run();
    void getGrammId(string &strGrammId);
    int init();
    int uploadKeyWord();
    int sendAudioData(char *audioData, size_t iWaveLen);
    string readAudioResult(int iRsltStatus, int &iRtnValue);
    void releaseSessionResourch();
    string parseResult(string strResult);
    void closeAudioControl();
    void playCloseVoice(const char *strText);
};

#endif
