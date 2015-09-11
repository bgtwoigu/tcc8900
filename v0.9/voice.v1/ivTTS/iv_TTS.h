#ifndef IVTTS_H
#define IVTTS_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include "voiceInterface.h"
#include "voice_setting.h"
#include "AisoundTTS.h"

//�����ϳ�����
class TTSRequest
{
public:
	TTSRequest(TTSUser user, const char* str);
	~TTSRequest();

	void addNextRequest(TTSRequest* pNext);
	TTSRequest* getNextRequest();
	void wait();//�Ⱥϳ���ɵ��ź�
	void complete();//�ϳɽ��������ͺϳ���ɵ��ź�
	const char* getStr();//���������ַ���
	TTSUser getUser();//���ط���������û������


private:
	TTSRequest*		m_pNext;//��һ������
	char*			m_pStr;//Ҫ�ϳɵ��ַ���
	QWaitCondition	m_condComplete;//�ϳ���ɵ��ź�
	QMutex			m_mutexWaitCondition;//����m_condComplete�Ļ�����
	TTSUser			m_TTSUser;//����������û����
};

/*�����ϳ��߳���*/
class IvTTS: public QThread, public IvTTSInterface
{
	Q_OBJECT
		
public:
	IvTTS(QThread *parent = 0);
	~IvTTS();

	/*ʵ��IvTTSInterface�ӿ�*/
	void PlayString(TTSUser user, const char* str);//�ϳɡ������ַ���
			
private:
	volatile bool				m_bExitRun;//�Ƿ��˳�run����
	
	/*�����������ʵ�ֶ��̲߳���*/
	volatile bool				m_bClosing;//�Ƿ����ڹر��Լ�
									      //����������������������ʱ�����ã�
									      //֮����������ͨ��PlayString�ĵ��ö�����������
	TTSRequest*				m_pCurrentRequest;//��ǰ����
	TTSRequest*				m_pLastRequest;//���һ������
	QWaitCondition			m_condRequest;//֪ͨ��������ź�
	QMutex					m_mutexRequest;//����m_pCurrentRequest��m_pLastRequest��m_condRequest��m_bClosing
											 //�Ļ�����
											 //�������������TTSRequest�ڲ������ݣ�TTSRequest�ڲ����ܻ����Լ��Ļ�����	

	/*TODO����ı������������ںϳɲ��ŵľ���ʵ��*/
	Aisound					m_Aisound;

private:
	void AppendRequest(TTSRequest* pRequest);//����һ������
	void exitFromRun();//֪ͨ�̴߳�run�������˳�
	void run();
};

#endif // IVTTS_H

