#ifndef PARSE_CONFIG_H
#define PARSE_CONFIG_H



namespace AudioControl1
{



/**********************************************************************************

0 -- //�ɹ�
-1 -- //û�ҵ�
-2 -- //�������ļ�����
-3 -- //buf�ռ䲻��
-4 -- //���﷨����

//��ĸ(��Сд)���ո�table��_��-��Ϊ��Ч�ֶ�

***********************************************************************************/


int testConfig();

//���������ļ���Ϣ���������ṹ������configInfo��
int read_config_load(const char *filename);
//��ȡpc_section_name����pc_item_name��Ŀ���ַ�����Ϣ
int read_config(char* pc_section_name, char* pc_item_name, char* pc_buf, int i_len);
//��ȡpc_section_name����pc_item_name��Ŀ����ֵ��Ϣ
int read_config(char* pc_section_name, char* pc_item_name, int *pi_value);
//��ȡpc_section_name����pc_item_name��Ŀ�Ĳ�����Ϣ
int read_config(char* pc_section_name, char* pc_item_name, bool *pb_judge);
//��ȡpc_section_name����pc_item_name��Ŀ��ö����Ϣ
int read_config(char* pc_section_name, char* pc_item_name, int *pi_num, int i_area);
//�ͷŽṹ���б��������������Ϣ
int read_config_unload();
//��ӡ�߼�������Ϣ
void read_config_logic_error();
//ȥ�ַ������ҿո�
void tools_trimStr(char* pStr);

int ParseStr(char* pOldStr, char **ppStr, int iStrArraylen);

bool CheckStrNum(char *pc_str);


}

#endif
