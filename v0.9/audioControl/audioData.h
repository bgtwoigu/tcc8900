
#ifndef AUDIO_DATA_H
#define AUDIO_DATA_H

#include <QList>
#include <QString>
//#include <QMetaType>

namespace AudioControl1
{




#define AUDIO_CHANNEL_MAX_NUMBER		100
#define LEVEL_MAX_NUMBER					10
#define VOL_MAX_VALUE						255

typedef enum _WorkModeE
{
	WORK_MODE_NORMAL = 0,
	WORK_MODE_SPECIAL,
}WorkModeE;

typedef enum _AudioChannelIdE
{
	AUDIO_CHANNEL_ID_MIN = 0,
//����ͨ��
	AUDIO_CHANNEL_ID_MAIN,							//������(����)
	AUDIO_CHANNEL_ID_MAIN_LEFT,						//������
	AUDIO_CHANNEL_ID_MAIN_RIGHT,					//������
	AUDIO_CHANNEL_ID_RELATION,						//��������
		//ʵ�ʲ���: AUDIO_CHANNEL_ID_MAIN

//ģ��ͨ��
	AUDIO_CHANNEL_ID_ANALOG_RADIO,					//������
	AUDIO_CHANNEL_ID_ANALOG_TV,						//����
	AUDIO_CHANNEL_ID_ANALOG_PHONE,					//�����绰
	AUDIO_CHANNEL_ID_ANALOG_MIC,					//��˷�(�û����ã��ڲ�ʹ��)
	AUDIO_CHANNEL_ID_ANALOG_BLUETOOTH,				//����

//����ͨ��
	AUDIO_CHANNEL_ID_DIGITAL_MPLAYER,				//mplayerռ�õ�����
	AUDIO_CHANNEL_ID_DIGITAL_GPS,					//GPS����
	AUDIO_CHANNEL_ID_DIGITAL_GPS_RING,				//GPS��ʾ��
	AUDIO_CHANNEL_ID_DIGITAL_NEWS,					//���Ų���
	AUDIO_CHANNEL_ID_DIGITAL_SMS,					//���Ų���
	AUDIO_CHANNEL_ID_DIGITAL_VOICE_TIP,				//����ģ����ʾ 15
	AUDIO_CHANNEL_ID_DIGITAL_VOICE_IDENTIFY,		//����ʶ��	16
		//ʵ�ʲ���: AUDIO_CHANNEL_ID_ANALOG_MIC
	AUDIO_CHANNEL_ID_DIGITAL_VOICE_INPUT,			//��������  17
		//ʵ�ʲ���: AUDIO_CHANNEL_ID_ANALOG_MIC
	AUDIO_CHANNEL_ID_DIGITAL_ANNOUNCE,				//��ͨ����
	AUDIO_CHANNEL_ID_DIGITAL_RING,					//��������
	AUDIO_CHANNEL_ID_DIGITAL_VPHONE,				//��Ƶ�绰
	AUDIO_CHANNEL_ID_DIGITAL_KEYTONE,				//������
	AUDIO_CHANNEL_ID_DIGITAL_ILLEGALTIPS,			//Υ����ʾ
	AUDIO_CHANNEL_ID_DIGITAL_SMSRING,				//������ʾ��
//	AUDIO_CHANNEL_ID_DIGITAL_TONE,
//	AUDIO_CHANNEL_ID_DAI_DAI_VTALKI,
//	AUDIO_CHANNEL_ID_DAI_DAI_MONLINE,
//	AUDIO_CHANNEL_ID_DAI_DAI_VONLINE,
//	AUDIO_CHANNEL_ID_DAI_DAI_LBOOK,
//	AUDIO_CHANNEL_ID_DAI_DAI_MTRAFFIC,

	AUDIO_CHANNEL_ID_MAX,
	AUDIO_CHANNEL_ID_INVALID,
}AudioChannelIdE;

static const char*  g_AudioChannelName[] = 
{
	"",								//AUDIO_CHANNEL_ID_MIN = 0,
	"main",							//AUDIO_CHANNEL_ID_MAIN,							//������(����)
	"main_left",						//AUDIO_CHANNEL_ID_MAIN_LEFT,					//������
	"main_right",						//AUDIO_CHANNEL_ID_MAIN_RIGHT,					//������
	"relation",						//AUDIO_CHANNEL_ID_RELATION,						//��������

	"radio",							//AUDIO_CHANNEL_ID_ANALOG_RADIO,				//������
	"tv",								//AUDIO_CHANNEL_ID_ANALOG_TV,					//����
	"phone",							//AUDIO_CHANNEL_ID_ANALOG_PHONE,				//�����绰
	"",								//AUDIO_CHANNEL_ID_ANALOG_MIC,					//��˷�(�û����ã��ڲ�ʹ��)
	"bluetooth",						//AUDIO_CHANNEL_ID_ANALOG_BLUETOOTH,			//����

	"mplayer",						//AUDIO_CHANNEL_ID_DIGITAL_MPLAYER,				//mplayerռ�õ�����
	"gps",							//AUDIO_CHANNEL_ID_DIGITAL_GPS,					//GPS����
	"gps_ring",						//AUDIO_CHANNEL_ID_DIGITAL_GPS_RING,				//GPS��ʾ��
	"news",							//AUDIO_CHANNEL_ID_DIGITAL_NEWS,					//���Ų���
	"sms",							//AUDIO_CHANNEL_ID_DIGITAL_SMS,					//���Ų���
	"voice_tip",						//AUDIO_CHANNEL_ID_DIGITAL_VOICE_TIP,				//����ģ����ʾ
	"voice_identify",					//AUDIO_CHANNEL_ID_DIGITAL_VOICE_IDENTIFY,		//����ʶ��
	"voice_input",						//AUDIO_CHANNEL_ID_DIGITAL_VOICE_INPUT,			//��������
	"announce",						//AUDIO_CHANNEL_ID_DIGITAL_ANNOUNCE,			//��ͨ����
	"ring",							//AUDIO_CHANNEL_ID_DIGITAL_RING,					//��������
	"vphone",						//AUDIO_CHANNEL_ID_DIGITAL_VPHONE,				//��Ƶ�绰
	"keytone",						//AUDIO_CHANNEL_ID_DIGITAL_KEYTONE,				//������
	"illegaltips",						//AUDIO_CHANNEL_ID_DIGITAL_ILLEGALTIPS,			//Υ����ʾ
	"smsring",						//AUDIO_CHANNEL_ID_DIGITAL_SMSRING,				//������ʾ��
	
	"",								//AUDIO_CHANNEL_ID_MAX,
};

typedef enum _VolCorrespondenceE
{
	VOL_CORRESPONDENCE_LINER = 0,
	VOL_CORRESPONDENCE_LOG,
	VOL_CORRESPONDENCE_TABLE,
}VolCorrespondenceE;

typedef enum _AudioChannelStatusE
{
	AUDIO_CHANNEL_OPEN = 0,
	AUDIO_CHANNEL_CLOSE,
	AUDIO_CHANNEL_PAUSE,
}AudioChannelStatusE;

class AudioChannelC
{
public:
	AudioChannelC(){id=AUDIO_CHANNEL_ID_INVALID; idLevel=-1; status=AUDIO_CHANNEL_CLOSE;
		bMute=false; iVirtualVol=VOL_MAX_VALUE; iVirtualVolDefault=VOL_MAX_VALUE; iActualVol=VOL_MAX_VALUE; iActualVolMax=VOL_MAX_VALUE; iVolAdjustRange=0;
		bChanged=false; name=""; bRelated=false; bValid=false;}
	AudioChannelC(const AudioChannelC &other){*this=other;}
	
	AudioChannelIdE		id;
	int					idLevel;
	AudioChannelStatusE	status;
	bool					bMute;
	int					iVirtualVol;
	int					iVirtualVolDefault;
	int					iActualVol;
	int					iActualVolMax;
	int					iVolAdjustRange;//����mixA����ʶiVirtualVol��Ҫ���ڵķ�Χ
	int					bChanged;//����mixA����ʶ�Ѿ��ı��˵�����
	QString				name;
	bool					bRelated;
	bool					bValid;
};

//Q_DECLARE_METATYPE(AudioChannelC)
	
typedef QList<AudioChannelC> AudioChannelList;

typedef enum _AudioLevelTypeE
{
	AUDIO_LEVEL_TYPE_COMMON = 0,
	AUDIO_LEVEL_TYPE_ALWAYS_OPEN,
}AudioLevelTypeE;

typedef enum _AudioLevelPropertyE
{
	AUDIO_LEVEL_PROPERTY_EXCLUSIVE = 0,
	AUDIO_LEVEL_PROPERTY_PAUSE,
	AUDIO_LEVEL_PROPERTY_INTERRUPT,
	AUDIO_LEVEL_PROPERTY_MIX_A,
	AUDIO_LEVEL_PROPERTY_MIX_B,
	AUDIO_LEVEL_PROPERTY_NOTHING,		//for level of type AUDIO_LEVEL_TYPE_ALWAYS_OPEN
}AudioLevelPropertyE;

typedef struct _AudioLevelS
{
	AudioLevelTypeE	type;
	AudioLevelPropertyE	property;
	AudioChannelList	audioChannelList;
}AudioLevelS;

typedef QList<AudioLevelS> AudioLevelList;


}


#endif

