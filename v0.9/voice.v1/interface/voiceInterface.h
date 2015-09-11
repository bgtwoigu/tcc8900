#ifndef MOVIEINTERFACE_H
#define MOVIEINTERFACE_H

#define MOVIE_MSG_BUF_SIZE 30

/*����ʶ��ӿ�*/
class AiTalkInterface
{
public:
	virtual ~AiTalkInterface(){}
	virtual bool GetIdentifyState() = 0;//�õ�����ʶ���ܵ�״̬��true��ʾ����ʶ���ܴ򿪣�false��ʾ����ʶ���ܹر�
};

/*�ṩ�����̵߳��õ�����ʶ��ӿ�*/
class AiTalkInterfaceForMainthread
{
public:
	virtual ~AiTalkInterfaceForMainthread(){}
	
	virtual bool GetIdentifyState() = 0;//��ѯ��������ʶ��������״̬���Ǵ򿪣����ǹرգ���ѯ�������Ϊң���뷢��
#if 0
	virtual void ProhibitEnterIdentifyMode() = 0;	//��ֹ��������ʶ��
	virtual void AllowEnterIdentifyMode() = 0;		//�����������ʶ��
#endif	
	virtual bool StartIdentify() = 0;//��ʼ����ʶ����
	virtual bool StopIdentify() = 0;//ֹͣ����ʶ����
};

/*�û����*/
typedef enum _TTSUser
{
	User_MainThread,
	User_AiTalk
}TTSUser;

/*�������ģʽ*/
typedef enum _SoundOutputMode
{
	GPRS_3G_Output_Mode,					//GPRS/3G ��Ƶ���ģʽ
	FM_Output_Mode,						//FM ��Ƶ���ģʽ
	DVD_Output_Mode,						//DVD���ģʽ
	DSP_Output_Mode						//DSP(cpu)���ģʽ
}SoundOutputMode;

/*�����ϳɽӿ�*/
class IvTTSInterface
{
public:
	virtual ~IvTTSInterface(){}
	virtual void PlayString(TTSUser user, const char* str) = 0;		//�ϳɡ������ַ���
};

#endif // MOVIEINTERFACE_H
