
#include "phoneMessage.h"


//��guiͨ�ŵ������Ӧ��֪ͨ��Ϣ��
PhoneReqC::PhoneReqC():DBusRequestC(REQ_ID_PHONE)
{
}

void PhoneReqC::print(FILE *fp)
{
	fprintf(fp,"\tShow=%d\n",m_bServiceState);
}

PhoneRepC::PhoneRepC():DBusReplyC(REP_ID_PHONE)
{
}

void PhoneRepC::print(FILE */*fp*/)
{
}

PhoneNotifyC::PhoneNotifyC():DBusNotifyC(NOTF_ID_PHONE)
{
	
}
PhoneNotifyC::~PhoneNotifyC()
{
}
void PhoneNotifyC::print(FILE */*fp*/)
{
}

PhoneMissCallNotifyC::PhoneMissCallNotifyC():DBusNotifyC(NOTF_ID_PHONE_MISSCALL)
{
}
PhoneMissCallNotifyC::~PhoneMissCallNotifyC()
{
}
void PhoneMissCallNotifyC::print(FILE */*fp*/)
{
}



PhoneNewMessageFlagNotifyC::PhoneNewMessageFlagNotifyC():DBusNotifyC(NOTF_ID_PHONE_NEWMSG)
{
	
}
PhoneNewMessageFlagNotifyC::~PhoneNewMessageFlagNotifyC()
{
}
void PhoneNewMessageFlagNotifyC::print(FILE */*fp*/)
{
}
#ifdef SEND_MSG_TO_NAVI
PhoneSendMSGTONaviNotifyC::PhoneSendMSGTONaviNotifyC():DBusNotifyC(NOTF_ID_SEND_MSG_TO_NAVI)
{
}
PhoneSendMSGTONaviNotifyC::~PhoneSendMSGTONaviNotifyC()
{
}
void PhoneSendMSGTONaviNotifyC::print(FILE *fp)
{
}
#endif

PhoneSMSToSetupNotifyC::PhoneSMSToSetupNotifyC(void):DBusNotifyC(NOTF_PHONE_SMS_TO_SETUP)
{
	success = false;
	fun = FUN_SUM;
}
PhoneSMSToSetupNotifyC::~PhoneSMSToSetupNotifyC(void)
{
}

//һ��ͨ����Ӧ��֪ͨ��Ϣ��
PhoneKeyReqC::PhoneKeyReqC():DBusRequestC(REQ_ID_PHONEKEY)
{
	m_bPhoneKeyState=false;
	keyrequest_show=PHONEKEY_REQ_INIT;
	memset((void*)&info,'\0',sizeof(keyInfo));
}

void PhoneKeyReqC::print(FILE *fp)
{
	fprintf(fp,"\tm_bPhoneKeyState=%d\n",m_bPhoneKeyState);
	fprintf(fp,"\tkeyrequest_show=%d\n ",keyrequest_show);
	fprintf(fp,"\tinfo.chNumber=%s\n ",info.chNumber);
}

PhoneKeyRepC::PhoneKeyRepC():DBusReplyC(REP_ID_PHONEKEY)
{
	
}

void PhoneKeyRepC::print(FILE */*fp*/)
{
}

PhoneKeyNotifyC::PhoneKeyNotifyC():DBusNotifyC(NOTF_ID_PHONEKEY)
{
	
}

PhoneKeyNotifyC::~PhoneKeyNotifyC()
{
}

void PhoneKeyNotifyC::print(FILE */*fp*/)
{
}

//��������
PhoneMessageReqC::PhoneMessageReqC():DBusRequestC(REQ_ID_PHONEMESSAGE)
{
}

void PhoneMessageReqC::print(FILE */*fp*/)
{
}

PhoneMessageRepC::PhoneMessageRepC():DBusReplyC(REP_ID_PHONEMESSAGE)
{
}

void PhoneMessageRepC::print(FILE */*fp*/)
{
}

//δ����������
PhoneMissCallReqC::PhoneMissCallReqC():DBusRequestC(REQ_ID_PHONEMISSCALL)
{
}

void PhoneMissCallReqC::print(FILE */*fp*/)
{
}

PhoneMissCallRepC::PhoneMissCallRepC():DBusReplyC(REP_ID_PHONEMISSCALL)
{
}

void PhoneMissCallRepC::print(FILE */*fp*/)
{
}

//�����������
PhoneLiveServiceReqC::PhoneLiveServiceReqC():DBusRequestC(REQ_ID_PHONELIVESERVICE)
{
	memset((void*)&call_number,'\0',sizeof(call_number));
}

void PhoneLiveServiceReqC::print(FILE */*fp*/)
{

}

PhoneLiveServiceRepC::PhoneLiveServiceRepC():DBusReplyC(REP_ID_PHONELIVESERVICE)
{
}

void PhoneLiveServiceRepC::print(FILE */*fp*/)
{
}

PhoneLiveServiceNotifyC::PhoneLiveServiceNotifyC():DBusNotifyC(NOTF_ID_PHONELIVESERVICE)
{	
}
void PhoneLiveServiceNotifyC::print(FILE */*fp*/)
{
}

//����������
PhoneWhereMeReqC::PhoneWhereMeReqC():DBusRequestC(REQ_ID_PHONEWHEREME)
{
	memset((void*)&call_number,'\0',sizeof(call_number));
	memset((void*)&call_name,'\0',sizeof(call_name));
}

void PhoneWhereMeReqC::print(FILE */*fp*/)
{

}

PhoneWhereMeRepC::PhoneWhereMeRepC():DBusReplyC(REP_ID_PHONEWHEREME)
{
	
}
	
void PhoneWhereMeRepC::print(FILE */*fp*/)
{
	
}

//����������
PhoneWhereYouReqC::PhoneWhereYouReqC():DBusRequestC(REQ_ID_PHONEWHEREYOU)
{
	memset((void*)&call_number,'\0',sizeof(call_number));
	memset((void*)&call_name,'\0',sizeof(call_name));
}

void PhoneWhereYouReqC::print(FILE */*fp*/)
{
	
}

PhoneWhereYouRepC::PhoneWhereYouRepC():DBusReplyC(REP_ID_PHONEWHEREYOU)
{
	
}
	
void PhoneWhereYouRepC::print(FILE */*fp*/)
{
	
}

//������Ϣ�ϱ���security��Ϊ�����
PhoneCarStatusReqC::PhoneCarStatusReqC():DBusRequestC(REQ_ID_CARSTATUS)
{
}

void PhoneCarStatusReqC::print(FILE *fp)
{
}

PhoneCarStatusRepC::PhoneCarStatusRepC():DBusReplyC(REP_ID_CARSTATUS)
{
}
	
void PhoneCarStatusRepC::print(FILE */*fp*/)
{
	
}

//��ȡ3Gģ��汾
Phone3GModuleVersionReqC::Phone3GModuleVersionReqC():DBusRequestC(REQ_ID_3GMODULEVERSION)
{
}

void Phone3GModuleVersionReqC::print(FILE *fp)
{
}

Phone3GModuleVersionRepC::Phone3GModuleVersionRepC():DBusReplyC(REP_ID_3GMODULEVERSION)
{
	memset((void*)&m_version,'\0',sizeof(m_version));

}
	
void Phone3GModuleVersionRepC::print(FILE */*fp*/)
{
	
}

