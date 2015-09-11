#ifndef _PHONEMESSAGE_H_20100724_
#define _PHONEMESSAGE_H_20100724_

#include "dbusMessage.h"

#define ServerPhone 		(char*)"com.innov.phone" 		//��ʽcom.innov.<������>
#define ObjectPhone 		(char*)"/phone"				//��ʽ /<ģ����>

#define	REQ_ID_PHONE	107	//������Ϣ��ʶ����REQ_ID_<ģ����>_<������>
#define	REP_ID_PHONE	107	//Ӧ����Ϣ��ʶ����REP_ID_<ģ����>_<������>
#define	NOTF_ID_PHONE	107	//֪ͨ��Ϣ��ʶ����NOTF_ID_<ģ����>_<������>

#define	NOTF_ID_PHONE_MISSCALL	108
#define   NOTF_ID_PHONE_NEWMSG	109
#ifdef SEND_MSG_TO_NAVI
        #define   NOTF_ID_SEND_MSG_TO_NAVI	110
#endif
#define NOTF_PHONE_SMS_TO_SETUP	111

#define	REQ_ID_PHONEKEY	1030	
#define	REP_ID_PHONEKEY	1030	
#define	NOTF_ID_PHONEKEY	1030	

#define	REQ_ID_PHONEMESSAGE	 1031	
#define	REP_ID_PHONEMESSAGE	 1031	

#define	REQ_ID_PHONELIVESERVICE	 1032	
#define	REP_ID_PHONELIVESERVICE	 1032	
#define	NOTF_ID_PHONELIVESERVICE	 1032	

#define 	REQ_ID_PHONEWHEREME		1033
#define	REP_ID_PHONEWHEREME		 1033	

#define 	REQ_ID_PHONEWHEREYOU	1034
#define	REP_ID_PHONEWHEREYOU	 1034

#define   REQ_ID_PHONEMISSCALL 1035
#define   REP_ID_PHONEMISSCALL  1035

#define 	REQ_ID_3GMODULEVERSION	  1036
#define	REP_ID_3GMODULEVERSION	   1036

//������Ϣ�ϱ�
#define 	REQ_ID_CARSTATUS  			1037
#define	REP_ID_CARSTATUS	         	1037


enum enuPhoneRequest
{
	PHONE_REQ_SHOW = 1
};
enum enuPhoneNotify
{
	PHONE_NEW_CALL = 1,
	PHONE_NEW_MSG
};

enum enuPhoneKeyRequest
{
	PHONEKEY_REQ_INIT=-1,	
	PHONEKEY_REQ_SHOW = 1
};
enum enuPhoneKeyNotify
{
	PHONEKEY_NEW_CALL = 1,
	PHONEKEY_NEW_MSG
};
//�ź�ǿ�ȣ��������ͣ���������
typedef enum _SignalValue
{
	SignalValue_0 = 0,
	SignalValue_1 = 1,
	SignalValue_2 = 2,
	SignalValue_3 = 3,
	SignalValue_4 = 4,
	SignalValue_5 = 5
}SignalValue;

enum netWorkType
{
	NOTIFY_SIG_TYPE_NONE = 0,
	 NOTIFY_SIG_TYPE_2G = 2,
	 NOTIFY_SIG_TYPE_3G  
};

typedef enum _PhoneNetworkType
{
	PhoneNetwork_NoNet = 0,
	PhoneNetwork_Limited,
	PhoneNetwork_2G,
	PhoneNetwork_3G
}PhoneNetworkType;

typedef enum _PhoneServiceType
{
	PhoneService_PhoneOnly = 0,
	PhoneService_NetOnly,
	PhoneService_PhoneAndNet,
	PhoneService_NoService
}PhoneServiceType;
 
typedef struct  _KeyInfo
{
	char chNumber[30];       // �绰����          
}keyInfo;


//��Gui����������Ӧ��֪ͨ��Ϣ��
class PhoneReqC:public DBusRequestC
{
	DBUS_REQUEST_OBJECT(PhoneReqC)

public:
	PhoneReqC();
	void print(FILE *fp);

	bool	m_bServiceState;
	enuPhoneRequest m_enuRequest;
};

class PhoneRepC: public  DBusReplyC
{
	DBUS_REPLY_OBJECT(PhoneRepC)

public:
	PhoneRepC();
	void print(FILE *fp);
};

class PhoneNotifyC: public  DBusNotifyC
{
	DBUS_NOTIFY_OBJECT(PhoneNotifyC)

public:
	PhoneNotifyC();
	~PhoneNotifyC();
	void print(FILE *fp);

	PhoneNetworkType m_netWorkType;
	SignalValue m_signalValue;
	PhoneServiceType m_serviceType;
	int signal_strength;
	
private:
	enuPhoneNotify m_enuNotify;
};
//δ������֪ͨ�������Ŀͻ�����gui
class PhoneMissCallNotifyC: public  DBusNotifyC
{
	DBUS_NOTIFY_OBJECT(PhoneMissCallNotifyC)

public:
	PhoneMissCallNotifyC();
	bool m_ifShow;
	
	~PhoneMissCallNotifyC();
	void print(FILE *fp);
	int missedCallCount;

};
//�¶���֪ͨ�������Ŀͻ�����gui
class PhoneNewMessageFlagNotifyC: public  DBusNotifyC
{
	DBUS_NOTIFY_OBJECT(PhoneNewMessageFlagNotifyC)

public:
	PhoneNewMessageFlagNotifyC();
	bool m_ifShow;
	int newSMSCount;

	~PhoneNewMessageFlagNotifyC();
	void print(FILE *fp);

};

#ifdef SEND_MSG_TO_NAVI
class PhoneSendMSGTONaviNotifyC: public  DBusNotifyC
{
	DBUS_NOTIFY_OBJECT(PhoneSendMSGTONaviNotifyC)

public:
	PhoneSendMSGTONaviNotifyC();
	~PhoneSendMSGTONaviNotifyC();
	void print(FILE *fp);

};
#endif

typedef enum{
	PRO = 0,
	CTY,
	HBT,
	MIL,
	RIV,
	FUN_SUM,
}SMS_TO_SETUP_FUN;

class PhoneSMSToSetupNotifyC: public  DBusNotifyC
{
	DBUS_NOTIFY_OBJECT(PhoneSMSToSetupNotifyC)

public:
	PhoneSMSToSetupNotifyC();
	~PhoneSMSToSetupNotifyC();
	bool success;
	SMS_TO_SETUP_FUN fun;
	
};

//һ��ͨ����Ӧ��֪ͨ��Ϣ��
class PhoneKeyReqC:public DBusRequestC
{
	DBUS_REQUEST_OBJECT(PhoneKeyReqC)

public:
	PhoneKeyReqC();
	void print(FILE *fp);

	bool	m_bPhoneKeyState;
	enuPhoneKeyRequest keyrequest_show;
	keyInfo info;	
};

class PhoneKeyRepC: public  DBusReplyC
{
	DBUS_REPLY_OBJECT(PhoneKeyRepC)

public:
	PhoneKeyRepC();
	void print(FILE *fp);
};

class PhoneKeyNotifyC: public  DBusNotifyC
{
	DBUS_NOTIFY_OBJECT(PhoneKeyNotifyC)

public:
	PhoneKeyNotifyC();
	~PhoneKeyNotifyC();
	void print(FILE *fp);

private:
	enuPhoneKeyNotify key_notify;
};

//δ����������
class PhoneMissCallReqC:public DBusRequestC
{
	DBUS_REQUEST_OBJECT(PhoneMissCallReqC)

public:
	PhoneMissCallReqC();
	void print(FILE *fp);
};

class PhoneMissCallRepC: public  DBusReplyC
{
	DBUS_REPLY_OBJECT(PhoneMissCallRepC)

public:
	PhoneMissCallRepC();
	void print(FILE *fp);
};


//��������
class PhoneMessageReqC:public DBusRequestC
{
	DBUS_REQUEST_OBJECT(PhoneMessageReqC)

public:
	PhoneMessageReqC();
	void print(FILE *fp);
};

class PhoneMessageRepC: public  DBusReplyC
{
	DBUS_REPLY_OBJECT(PhoneMessageRepC)

public:
	PhoneMessageRepC();
	void print(FILE *fp);
};

//�����������
class PhoneLiveServiceReqC:public DBusRequestC
{
	DBUS_REQUEST_OBJECT(PhoneLiveServiceReqC)

public:
	PhoneLiveServiceReqC();
	void print(FILE *fp);

	char call_number[100];
};

class PhoneLiveServiceRepC: public  DBusReplyC
{
	DBUS_REPLY_OBJECT(PhoneLiveServiceRepC)

public:
	PhoneLiveServiceRepC();
	void print(FILE *fp);
};

class PhoneLiveServiceNotifyC: public DBusNotifyC 
{
	DBUS_NOTIFY_OBJECT(PhoneLiveServiceNotifyC)

public:
	PhoneLiveServiceNotifyC();
	void print(FILE *fp);
	bool sendInfoToNet;
};

//����������
class PhoneWhereMeReqC:public DBusRequestC
{
	DBUS_REQUEST_OBJECT(PhoneWhereMeReqC)

public:
	PhoneWhereMeReqC();
	void print(FILE *fp);
	
	char call_number[30];
	char call_name[30];
};

class PhoneWhereMeRepC: public  DBusReplyC
{
	DBUS_REPLY_OBJECT(PhoneWhereMeRepC)

public:
	PhoneWhereMeRepC();
	void print(FILE *fp);
};

//����������
class PhoneWhereYouReqC:public DBusRequestC
{
	DBUS_REQUEST_OBJECT(PhoneWhereYouReqC)

public:
	PhoneWhereYouReqC();
	void print(FILE *fp);
	
	char call_number[30];
	char call_name[30];
};

class PhoneWhereYouRepC: public  DBusReplyC
{
	DBUS_REPLY_OBJECT(PhoneWhereYouRepC)

public:
	PhoneWhereYouRepC();
	void print(FILE *fp);
};

//��ȡ3Gģ��汾
class Phone3GModuleVersionReqC:public DBusRequestC
{
	DBUS_REQUEST_OBJECT(Phone3GModuleVersionReqC)

public:
	Phone3GModuleVersionReqC();
	void print(FILE *fp);
	
};

class Phone3GModuleVersionRepC: public  DBusReplyC
{
	DBUS_REPLY_OBJECT(Phone3GModuleVersionRepC)

public:
	Phone3GModuleVersionRepC();
	
	char m_version[100];

	void print(FILE *fp);
};

//������Ϣ�ϱ���security��Ϊ�����
class PhoneCarStatusReqC:public DBusRequestC
{
	DBUS_REQUEST_OBJECT(PhoneCarStatusReqC)

public:
	PhoneCarStatusReqC();
	void print(FILE *fp);
	
};
class PhoneCarStatusRepC: public  DBusReplyC
{
	DBUS_REPLY_OBJECT(PhoneCarStatusRepC)

public:
	PhoneCarStatusRepC();
	void print(FILE *fp);
};

#endif //_PHONEMESSAGE_H_20100724_

