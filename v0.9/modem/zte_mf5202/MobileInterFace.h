#ifndef _MOBILEINTERFACE_H_
#define _MOBILEINTERFACE_H_

#include <QObject>
#include <QMutex>
#include <QApplication>
#include <QThread>
#include <QStringList>

#include "phoneMonitorVoiceReportBase.h"

//#define PHONE_MODULE_ZTEMF5200_AUTH

/********************************************************************************/

//ͨ�ô�����
#define	PHONE_MONITOR_SUCCESS				(0)			//���óɹ�
#define	PHONE_MONITOR_FAIL					(-2)			//����ʧ��(�в�����ȷ����ԭ��)
#define	PHONE_MONITOR_DEV_INVALID			(-5)		//�ƶ��豸�ն˲�����
#define	PHONE_MONITOR_SIM_INVALID			(-6)		//SIM��������
#define	PHONE_MONITOR_NET_INVALID			(-7)		//δ��ע�ᵽ����
#define	PHONE_MONITOR_CALL_BUSY				(-8)		//����ͨ��������
#define	PHONE_MONITOR_OPER_TIMEOUT			(-9)		//������ʱ��δ�ܷ���ִ�н��
#define	PHONE_MONITOR_WRONG_STATUS			(-10)		//�����ִ��״̬
#define	PHONE_MONITOR_CALLBACK_NULL			(-11)		//��δע��ص�����
#define	PHONE_MONITOR_CMD_BUSY				(-12)		//��һ��������δ���
#define	PHONE_MONITOR_CALLBACK_INVALID		(-13)		//�ص����󲻿���
#define	PHONE_MONITOR_PHONE_NUMBER_INVALID	(-14)		//�绰��������

#define	PHONE_MONITOR_INTENT_HANG			(-15)		//�Լ��Ҷϵ绰
#define	PHONE_MONITOR_OPPOSITE_HANG			(-16)		//�Է��Ҷϵ绰
#define	PHONE_MONITOR_TIMEOUT_HANG			(-17)		//��ʱ���˽���

#define	PHONE_MONITOR_CANNOT_GET_MY_LOCATION			(-18)		

#define PHONE_STATUS_NOT_INIT	0
#define PHONE_STATUS_INITTING	1
#define PHONE_STATUS_INITED	2

#define	PHONE_STATUS_CALLING	(3)


#define        CALL_IDLE                   0           
#define        CALL_CALLING                1           
#define        CALL_TALKING                2          
#define        CALL_COMING_CALL            3         



class PhoneConnectionC;
class VoicePhoneWorkThreadC;
class VoicePhoneInitThreadC;
class PhoneMonitorVoiceReportBase;
class VideoPhoneMonitorC;

class PhoneMonitorVoiceInterface: public QThread
{
	Q_OBJECT

public:
	PhoneMonitorVoiceInterface();
 	~PhoneMonitorVoiceInterface();

public:
	//�ƶ��ն������绰���Ź��ܹ���
	int start(PhoneMonitorVoiceReportBase *reportHandle);						//��������ʼ���绰���ģ��
	int stop();																//ֹͣʹ�õ绰���ģ��
	
	//����ͨ������
	int dial(const char* cpNumber);	//���š�
	int pickUp();						//����
	int hangUp();						//�Ҷ�
	int rtDial(char cbyte);			//���β��š�
	
	//���Ź���
	int sendSMS( const char* pcContent, const char* cpNumber );					//������ͨ����Ϣ
	int sendSMSWithId(const char* pcContent, const char* pcNumber, unsigned int* id);

	//��������
	int	getSKVol();						//��ȡ��Ͳ����
	int	setSKVol(int val);				//������Ͳ����
	int	getMicSatus();					//��ȡMic״̬
	int	muteMic();						//����Mic
	int 	unMuteMic();					//ȡ������MIC
	
	
	//��������
	int	getRSSI();						//��ȡ�ź�ǿ��
	signed  char 	getNetworkType(); //error:<0; 2:2G  3:3G
	int getNetworkTypeAndServiceType(NetworkType &net, ServiceType &service);
	int getSignalStrength(SignalStrength &signal,int &signal_strength);
	int getNetworkTypeAndServiceType(NetworkType &net);
	int getNetworkTypeAndServiceType(ServiceType &service);

	int queryCallState(void);//-1:error; 0:success but not be calling; PHONE_STATUS_CALLING:success and be calling
	int getCallState(void){return m_callStatus;}
#ifdef PHONE_MODULE_ZTEMF5200_AUTH
private:
	int authentication();
	void getMd5Data(char *input, char *output);
#endif	
//Ϊ��ԭ�ӿڼ��ݣ���ʱ�����Ľӿڲ���
public:
	bool init3gfun();
	bool close3gfun();
	int FaceTime_Dial(char* cpNumber);		//������ӵ绰������ֵ: -2��ʾ����ͨ���У�-1��ʾ����ʧ�ܣ�0��ʾ���ųɹ�
	bool FaceTime_PickUp();				//�������ӵ绰
	bool FaceTime_HangUp();			//�ҶϿ��ӵ绰
	bool setMicVol(int val);

private:
	void run();

protected:
	PhoneConnectionC		*m_pPhoneConnection;

private:
	PhoneMonitorVoiceReportBase	*m_pCallback;

	int				 m_callStatus;
	char				 m_phoneNumber[30];

	int				 m_phoneStatus;
	QMutex          		 m_mutex;

	VoicePhoneInitThreadC		 *m_pInitThread;
	VoicePhoneWorkThreadC		 *m_pWorkThread;

	VideoPhoneMonitorC		*m_videoPhone;

	
private:
	friend class VoicePhoneInitThreadC;
	friend class VoicePhoneWorkThreadC;
	friend class PhoneMonitorVoiceReportBase;

signals:
	void phoneInitedSignal(void);//for bug:calling,auto-system reboot suddently because of unpredictable factor,calling is going on
};

#endif

