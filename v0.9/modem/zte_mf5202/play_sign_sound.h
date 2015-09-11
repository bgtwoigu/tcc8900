#ifndef PLAYSIGNSOUND_H
#define PLAYSIGNSOUND_H

/*
ʹ��˵��:

��������:����"PlaySignSound"�еĺ���"void play_sign_sound(char *file_name, int loop_play)"
���в���"file_name"��Ҫ���ŵ������ļ���·����������"loop_play"������
��ѭ�����ţ�ѭ������:loop_play = 1��ֻ����һ��:loop_play = 0

ֹͣ��������:����"PlaySignSound"�еĺ���"void stop_sign_sound()"

PlaySignSound���еĽ��������ź�:ÿ������һ�������ļ���PlaySignSound���ڲ��ᷢ��
һ���ź�"finishedPlay()"������ͨ�����źż���Ƿ��ꡣ
*/

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include "asoundlib.h"

class PlaySignSound: public QThread
{
	Q_OBJECT
		
public:
	typedef enum _SoundType
	{
		Sound_Type_Ring = 0,
		Sound_Type_Smsring,
	}SoundType;

	PlaySignSound();
	~PlaySignSound();


	void play_sign_sound(char *file_name, int loop_play, SoundType type);
	void stop_sign_sound();

signals:
	void finishedPlay();
	
private:
	int is_loop_play;

	int asound_status;
	char sound_file_name[100];

	QMutex m_mutex;
	QWaitCondition  m_quitCond;

private:
	int fd;	
	int asound_size;
	long loops;
	int err;
	char *buffer;
	char *p_buffer;
	int channels;
	unsigned int val;
	int pcm_dir;
	snd_pcm_uframes_t frames;
	snd_pcm_hw_params_t* params;
	snd_pcm_t* handle;
	SoundType m_soundType;

	int init_asound();
	int play_sign_sound();
	void release_asound();

	void run();
};

#endif // WIDGET_H
