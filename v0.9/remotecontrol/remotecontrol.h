#ifndef _REMOTE_CONTROL_H_
#define _REMOTE_CONTROL_H_

#define CHAR_LEN 64

#include <string>
using namespace std;

struct RemoteCtrlInfo
{
    int sendpolicy;
    int sendnum;
    int interval;
    int startpolicy;  
};

struct PostInfo
{
    unsigned char device_id[CHAR_LEN];
    int     position;       //�Ƿ�λ�� 0 δ��λ�� 1��λ�ɹ�
    int     ilng;		    // ��ǰGPS���꾭��
    int     ilat;		    // ��ǰGPS����γ��
    int     itime;		    // ��ǰ���ն�ʱ��
    int     ialti;          //��ǰ������Ϣ
    int     lng_flag;       //��ǰgps����ľ��� ����---0   ����---1
    int     lat_flag;       //��ǰgps�����γ�� ��γ---0   ��γ---1
    char version[CHAR_LEN]; //�汾��Ϣ
    char vdr_model[CHAR_LEN];
};





int curl_upload(char *url, char *send_buffer, char *receive_buffer);






#endif

