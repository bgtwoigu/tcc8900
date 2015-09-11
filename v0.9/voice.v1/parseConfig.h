#ifndef PARSE_CONFIG_H
#define PARSE_CONFIG_H

//���﷨��������
typedef enum _Check_Type
{
	CheckAll = 0,						//�ַ�Ҫ����ĸ(��Сд)�����֡�-��_��/����. ���ո�table��/n��/r�ķ�Χ�ڣ�������Ƿ�
	CheckBlank,							//�ַ�Ҫ�ڿո�table��/n��/r�ķ�Χ�ڣ�������Ƿ�
}Check_Type;

//������
typedef enum _Line_Type
{
	LineSection = 0,						//����
	LineItem,							//��Ŀ��
}Line_Type;

//����Ϣ�ṹ��
typedef struct _Line_Info{
	int iLineNum;										//��¼�ýṹ����Ϣ�������ļ��е��к�
	Line_Type LineType;								//�ýṹ���������
	char *pcTypeName;								//��¼��Ŀ���͵����ֻ��߶����͵�����
	char *pcContent;									//����ṹ��������Ŀ���ͣ���¼��Ŀ���͵�ֵ
	struct _Line_Info *pNext;							//ָ����һ���ڵ��ָ��
}Line_Info;


class IBParseConfigTool
{
public:
	//���������ļ���Ϣ���������ṹ������configInfo��
	static int read_config_load(const char *filename, const char check_type=0);
	//��ȡpc_section_name����item_num ����Ŀ����Ŀֵ
	static int read_config_items(char* pc_section_name, int item_num, char **item_buf, char ** context_buf);
	//��ȡpc_section_name����pc_item_name��Ŀ���ַ�����Ϣ
	static int read_config(char* pc_section_name, char* pc_item_name, char* pc_buf, int i_len);
	//��ȡpc_section_name����pc_item_name��Ŀ����ֵ��Ϣ
	static int read_config(char* pc_section_name, char* pc_item_name, int *pi_value);
	//��ȡpc_section_name����pc_item_name��Ŀ�Ĳ�����Ϣ
	static int read_config(char* pc_section_name, char* pc_item_name, bool *pb_judge);
	//��ȡpc_section_name����pc_item_name��Ŀ��ö����Ϣ
	static int read_config(char* pc_section_name, char* pc_item_name, int *pi_num, int i_area);
	//�ͷŽṹ���б��������������Ϣ
	static int read_config_unload();
	//��ӡ�߼�������Ϣ
	static void read_config_logic_error(char *pc_error_message);

private:
	static int _config_file_error_message(int i_line_id, char *pc_str);
	static int  _add_info(int i_line_id, Line_Type line_type, char *pc_type_name, char *pc_content);
	static  Line_Info*  _get_line_info(char* pc_section_name, char* pc_item_name);
	static bool _check_str_valid(char* p_str, Check_Type type);
	static bool  _check_str_num(char *pc_str);
	static void _trim_str(char* p_str);

};


#endif
