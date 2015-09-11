#ifndef _REMOTE_CONTROL_H_
#define _REMOTE_CONTROL_H_

#define STR_LEN 32

#include <string>
using namespace std;

struct updateInfo
{
    char device_id[STR_LEN]; //tuid
    char start[STR_LEN]; // ������ʼʱ��
    char end[STR_LEN]; // ��������ʱ��
    char old_version[STR_LEN]; //��ѡ���������ǰ�ް汾���򲻰������ֶ�
    char new_version[STR_LEN]; //��ǰ�汾
    char result[STR_LEN]; //����������ɹ���Ϊ"OK", ����Ϊʧ��ԭ��
    char old_model[STR_LEN]; //�ɰ汾���ն��ͺ�
    char new_model[STR_LEN]; //�°汾���ն��ͺ�
};

#endif

