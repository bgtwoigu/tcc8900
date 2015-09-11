#include "parseConfig.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "error_types.h"



/**********************************************************************************

0 -- //�ɹ�
-1 -- //û�ҵ�
-2 -- //�������ļ�����
-3 -- //buf�ռ䲻��
-4 -- //���﷨����
-5 -- //ö�ٳ�����Χ
//��ĸ(��Сд)���ո�table��_��-��Ϊ��Ч�ֶ�

***********************************************************************************/


static int gi_last_line = 0;									//��¼ǰһ�δ������ļ����������е��к�
static Line_Info	*gp_line_info_head = NULL;				//����Ϣ����ͷ
static char *gc_file_name = NULL;							//�����ļ���


//��ӡ�����ļ�������Ϣ
//i_line_id : �к�
//pc_str : ����ԭ��
int IBParseConfigTool:: _config_file_error_message(int i_line_id, char *pc_str = "")
{
	if( NULL != gc_file_name )
	{
		if( i_line_id >= 0 )
		{
			printf("[controler] An error occurred when parsing config file %s line %d because %s !\n", gc_file_name,  i_line_id, pc_str);
		}
		else
		{
			printf("[controler] An error occurred when parsing config file %s because %s !\n", gc_file_name,  pc_str);
		}
	}
	else
	{
		printf("[controler] there isn't config file opened!\n");
	}

	return 0;
}

//����һ���µ�Line_Info,����������Ϣ����
//����ֵ: �ɹ�����0
//                   ʧ�ܷ��ظ�ֵ
int IBParseConfigTool:: _add_info(int i_line_id, Line_Type line_type, char *pc_type_name, char *pc_content)
{
	Line_Info *pTemp = gp_line_info_head;
	Line_Info *pInfo = (Line_Info *)malloc(sizeof(Line_Info));
	if( NULL == pInfo )
	{
		printf("file(%s)function(%s) malloc error!!!\n", __FILE__, __FUNCTION__);
	}

	pInfo->pNext = NULL;
	pInfo->iLineNum = i_line_id;
	pInfo->LineType = line_type;
	pInfo->pcTypeName = NULL;
	pInfo->pcContent = NULL;
	if( NULL != pc_type_name )
	{
		pInfo->pcTypeName = (char*)malloc(strlen(pc_type_name)+1);
		if( NULL == pInfo->pcTypeName )
		{
			printf("malloc error\n");
			abort();
		}
		strcpy(pInfo->pcTypeName, pc_type_name);
	}
	if( NULL != pc_content )
	{
		pInfo->pcContent = (char*)malloc(strlen(pc_content)+1);
		if( NULL == pInfo->pcContent )
		{
			printf("malloc error\n");
			abort();
		}
		strcpy(pInfo->pcContent, pc_content);
	}
	while( 1 )
	{
		if( NULL == pTemp )
		{//����Ϊ��
			gp_line_info_head = pInfo;
			break;
		}

		if( NULL == pTemp->pNext)
		{//�ҵ���β
			pTemp->pNext = pInfo;
			break;
		}

		pTemp = pTemp->pNext;
	}

	return 0;	
}

//������Ϣ�������ҵ��Ͷ�������Ŀ��ƥ���Line_Info
//����ֵ: �����ҵ�������Ϣ
//                   �Ҳ�������NULL
 Line_Info* IBParseConfigTool:: _get_line_info(char* pc_section_name, char* pc_item_name)
{
	Line_Info *pTemp = gp_line_info_head;
	bool bFindSection = false;//�Ƿ��ֶ�
	
	while( NULL != pTemp )
	{
		if( LineSection == pTemp->LineType )
		{
			if( bFindSection )
			{//ԭ���Ѿ��ҵ��˶Σ�����������һ���¶�
				pTemp = NULL;
				break;
			}
						
			if( 0 == strcmp(pTemp->pcTypeName, pc_section_name) )
			{//�������
				bFindSection = true;
			}
		}
		else if( LineItem == pTemp->LineType )
		{
			if( bFindSection )
			{//�Ѿ��ҵ��˶Σ���ȥ����Ŀ
				if( 0 == strcmp(pTemp->pcTypeName, pc_item_name) )
				{//��Ŀ�����
					break;
				}
			}
		}

		pTemp = pTemp->pNext;
	}

	return pTemp;
}

//У��һ���ַ����Ƿ�Ϸ�
//����ֵ: �Ϸ�����true
//                   ���Ϸ�����false
bool IBParseConfigTool::_check_str_valid(char* p_str, Check_Type type)
{
	//'\r' ASCII ��13      �س�������ǰλ���Ƶ�����ͷ
	//'\n' ASCII��10      ���У�����ǰλ���Ƶ���һ��
	//ASCII 9��TAB��
	char all[] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',
		             'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
		             '0','1','2','3','4','5','6','7','8','9',
		             '-','_',' ','/','.',13,10,9,0};

	char blank[] = {'-','_',' ',13,10,9,0};		

	char *pCollect = NULL;//����У����ַ���
	char *pTemp = NULL;
	char *pTempCollect = NULL;
	bool bResult = true;//У����
	
	if( CheckAll == type )
	{
		pCollect = all;
	}
	else if( CheckBlank == type )
	{
		pCollect = blank;
	}

	if( NULL != pCollect )
	{
		pTemp = p_str;
		while( 0 != *pTemp )
		{
			pTempCollect = pCollect;
			bool bFind = false;//�Ƿ��ҵ���ĳ���ַ�
			//��ʼУ��ĳ���ַ�
			while( 0 != *pTempCollect )
			{
				if( *pTempCollect == *pTemp )
				{
					bFind = true;
					break;
				}
				pTempCollect++;
			}
			if( !bFind )
			{
				//û�ҵ���˵������ַ���������У����ַ�����
				printf(" value = %d \n",*pTemp);
				bResult = false;
				break;
			}
			pTemp++;
		}
	}

	return bResult;
}

//У��һ���ַ����Ƿ�������
//����ֵ: �����ַ���true
//                   �������ַ���false
bool IBParseConfigTool:: _check_str_num(char *pc_str)
{
	if( NULL == pc_str )
	{
		return false;
	}

	for(int i=0; i<(int)strlen(pc_str); i++)
	{
		if( *(pc_str+i) < 48 || *(pc_str+i) >57 )
		{
			return false;
		}	
	}

	return true;
}

//ȥ�ַ������ҿո�
void IBParseConfigTool::_trim_str(char* p_str)
{
	char *pFirst = NULL;//��һ���ǿ��ַ�
	char *pLast = NULL;//���һ���ǿ��ַ�

	if( NULL == p_str )
	{
		return;
	}
	
	int len = strlen(p_str);
	if( 0 == len )
	{//���ַ���û��Ҫȥ�ո�
		return;
	}
	
	//�����ҵ���һ���ǿ��ַ�
	pFirst = p_str;
	while(1)
	{
		if( ' ' != *pFirst && 9 != *pFirst && 10 != *pFirst && 13 != *pFirst )
		{
			break;
		}

		pFirst++;
	}

	//�ҵ����һ���ǿ��ַ�
	pLast = p_str+len-1;
	while( pLast >= p_str )
	{
		if( ' ' != *pLast && 9 != *pLast && 10 != *pLast && 13 != *pLast )
		{
			break;
		}

		pLast--;
	}

	//�����ǿ��ַ���
	if( pLast >= pFirst )
	{
		*(pLast+1) = 0;
		strcpy(p_str, pFirst);
	}
	else
	{
		strcpy(p_str, "");
	}

	return;
}


//���������ļ���Ϣ���������ṹ������configInfo��
//����ֵ: �ɹ�0
//                   װ�ش���R_INNOV_BOX_COMMON_LOAD_CONFIG_ERR
//check_type         0 -- ֻУ���������Ŀ������У����Ŀ����
//                         1 -- У���������Ŀ������Ŀ����
int IBParseConfigTool::read_config_load(const char *filename, const char check_type)
{
	FILE *pf_fd;
	char tmpbuf[1023+2];
	int retval = R_INNOV_BOX_OK, i_linetype = 0;
	int line = 0;

	read_config_unload();

	pf_fd = fopen(filename,"r");
	if(!pf_fd)
	{
		printf("[common] error: config file (%s) cann't be opened\n", filename);
		return R_INNOV_BOX_COMMON_LOAD_CONFIG_ERR;
	}

	gc_file_name = (char*)malloc(strlen(filename)+1);
	if( NULL == gc_file_name )
	{
		printf("[controler] malloc error!\n");
		abort();
	}
	strcpy(gc_file_name, filename);
		
	while(fgets(tmpbuf,1023+2,pf_fd) )
	{
		line ++;
		//�����ж϶��������Ƿ�С�ڵ���1023
		if( strlen(tmpbuf) > 1023 )
		{
			_config_file_error_message(line, (char *)"length of line is up to 1023");
			read_config_unload();	
			return R_INNOV_BOX_COMMON_LOAD_CONFIG_ERR;
		}
		
		char *pc_str1 = NULL , *pc_str2 = NULL;
		pc_str1 = tmpbuf;
		pc_str2 = strsep(&pc_str1,"#");
		if(strchr(pc_str2,'[') && strchr(pc_str2,']'))
		{
			i_linetype = 1;//����
		}
		else if(strchr(pc_str2,'='))
		{
			i_linetype = 2;//��Ŀ��
		}
		else
		{
			i_linetype = 3;//����
		}

		switch(i_linetype)
		{
			case 1:
			{
				pc_str1 = strsep(&pc_str2,"[");
				//���"["ǰ���ַ����Ƿ�Ϊ��
				if(!_check_str_valid(pc_str1,CheckBlank))
				{					
					_config_file_error_message(line, (char *)"there cann't be any character before '['");
					read_config_unload();	
					return R_INNOV_BOX_COMMON_LOAD_CONFIG_ERR;
				}
				
				pc_str1 = strsep(&pc_str2,"]");
				//���"]"����ַ����Ƿ�Ϊ��
				if(!_check_str_valid(pc_str2,CheckBlank))
				{
					_config_file_error_message(line, (char *)"there cann't be any character after ']'");
					read_config_unload();	
					return R_INNOV_BOX_COMMON_LOAD_CONFIG_ERR;
				}

				//У��"["��"]"֮����ַ���
				if(!_check_str_valid(pc_str1,CheckAll))
				{
					_config_file_error_message(line, (char *)"there is invalid character between '[' and ']'");
					read_config_unload();	
					return R_INNOV_BOX_COMMON_LOAD_CONFIG_ERR;
				}
				_trim_str(pc_str1);
				//�ж϶����Ƿ�Ϊ��
				if(strcmp(pc_str1, "") == 0) 
				{
					_config_file_error_message(line, (char *)"section name cann't be blank");
					read_config_unload();
					return R_INNOV_BOX_COMMON_LOAD_CONFIG_ERR;
				}
				//�ж϶����Ƿ񳬹�255��������
				if( strlen(pc_str1) > 255 )
				{
					_config_file_error_message(line, (char *)"length of section name is up to 255");
					read_config_unload();	
					return R_INNOV_BOX_COMMON_LOAD_CONFIG_ERR;
				}
				//���������ڴ�
				_add_info(line,LineSection,pc_str1,NULL);
				break;
			}
			case 2:
			{
				pc_str1 = strsep(&pc_str2,"=");

				_trim_str(pc_str1);
				_trim_str(pc_str2);

				if( !_check_str_valid(pc_str1,CheckAll) )
				{
					char tmp[1024];
					sprintf(tmp, "there is invalid character in item name (%s)", pc_str1);
					_config_file_error_message(line, tmp);
					read_config_unload();	
					return R_INNOV_BOX_COMMON_LOAD_CONFIG_ERR;
				}

				//У����Ŀ������Ŀ����
				if( check_type ==1 )
				{

					if( !_check_str_valid(pc_str2,CheckAll) )
					{
						char tmp[1024];
						sprintf(tmp, "there is invalid character in item content (%s)", pc_str2);
						_config_file_error_message(line, tmp);
						read_config_unload();	
						return R_INNOV_BOX_COMMON_LOAD_CONFIG_ERR;

					}
				}

				//�ж���Ŀ���Ƿ�Ϊ��
				if(strcmp(pc_str1, "") == 0)
				{
					_config_file_error_message(line, (char *)"item name cann't be blank");
					read_config_unload();
					return R_INNOV_BOX_COMMON_LOAD_CONFIG_ERR;
				}
				//�ж���Ŀ���Ƿ񳬹�255��������
				if( strlen(pc_str1) > 255 )
				{
					_config_file_error_message(line, (char *)"length of item name is up to 255");
					read_config_unload();	
					return R_INNOV_BOX_COMMON_LOAD_CONFIG_ERR;
				}

				//�ж���Ŀ�����Ƿ񳬹�255��������
				if( strlen(pc_str2) > 255 )
				{
					_config_file_error_message(line, (char *)"length of item content is up to 255");
					read_config_unload();	
					return R_INNOV_BOX_COMMON_LOAD_CONFIG_ERR;
				}

				_add_info(line,LineItem,pc_str1,pc_str2);
				break;
			}
			case 3:
			{				
				if(!_check_str_valid(pc_str2,CheckBlank))
				{
					_config_file_error_message(line, (char *)"this line cann't be parsed");
					read_config_unload();	
					return R_INNOV_BOX_COMMON_LOAD_CONFIG_ERR;
				}
			}

				
		}
		if(retval<0)
			break;
	}
	fclose(pf_fd);
	return retval;

}

//��ȡpc_section_name����item_num����Ŀ������Ŀֵ
//����ֵ: R_INNOV_BOX_COMMON_ITEM_NOFOUND -- û���ҵ���Ŀ
int IBParseConfigTool::read_config_items(char* pc_section_name,int item_num, char **item_buf, char ** context_buf)
{
	int retval = R_INNOV_BOX_OK;	

   	Line_Info *pTemp = gp_line_info_head;
	bool bFindSection = false;//�Ƿ��ֶ�
	int num=0;
	
	while( NULL != pTemp )
	{
		if( LineSection == pTemp->LineType )
		{
			if( bFindSection )
			{//ԭ���Ѿ��ҵ��˶Σ�����������һ���¶�
				pTemp = NULL;
				break;
			}
						
			if( 0 == strcmp(pTemp->pcTypeName, pc_section_name) )
			{//�������
				bFindSection = true;
			}
		}
		else if( LineItem == pTemp->LineType )
		{
                        if( bFindSection )
			{//�Ѿ��ҵ��˶Σ���ȥ����Ŀ          
                            *item_buf=pTemp->pcTypeName; 
                            *context_buf=pTemp->pcContent; 
                            item_buf++;
                            context_buf++;
                            num++;
                            if(num >= item_num)
                                break;
			}
		}

		pTemp = pTemp->pNext;
	}

      if( !bFindSection)
            retval = R_INNOV_BOX_COMMON_ITEM_NOFOUND; 
	return retval;
    
}

//��ȡpc_section_name����pc_item_name��Ŀ���ַ�����Ϣ
//����ֵ: R_INNOV_BOX_COMMON_ITEM_NOFOUND -- û���ҵ�ָ����Ŀ
//                   R_INNOV_BOX_COMMON_BUF_LACKED -- buf�ռ䲻��
int IBParseConfigTool::read_config(char* pc_section_name, char* pc_item_name, char* pc_buf, int i_len)
{
	int retval = R_INNOV_BOX_OK;		
	gi_last_line = -1;//�����һ�ζ�ȡ���еļ�¼
	const Line_Info *p_tmpline = _get_line_info(pc_section_name ,pc_item_name);
	if(p_tmpline)
	{
		if( p_tmpline->pcContent )
		{
			if( i_len < ((int)strlen(p_tmpline->pcContent)+1) )
			{
				retval = R_INNOV_BOX_COMMON_BUF_LACKED;
			}
			else
			{
				gi_last_line = p_tmpline->iLineNum;
				strcpy(pc_buf, p_tmpline->pcContent);
			}
		}
		else
		{
			if( i_len > 0 )
			{
				pc_buf[0] = 0;
			}
		}
	}
	else
	{
		retval = R_INNOV_BOX_COMMON_ITEM_NOFOUND;	
	}
	return retval;
}

//��ȡpc_section_name����pc_item_name��Ŀ����ֵ��Ϣ
//����ֵ: R_INNOV_BOX_COMMON_ITEM_NOFOUND -- û���ҵ�ָ����Ŀ
//                   R_INNOV_BOX_COMMON_ITEM_NOT_NUMERICAL -- ��Ŀ���ݲ�����ֵ
int IBParseConfigTool::read_config(char* pc_section_name, char* pc_item_name, int *pi_value)
{
	int retval = R_INNOV_BOX_OK;	
	gi_last_line = -1;//�����һ�ζ�ȡ���еļ�¼
	const Line_Info *p_tmpline =  _get_line_info(pc_section_name ,pc_item_name);
	if(p_tmpline)
	{
		if( _check_str_num(p_tmpline->pcContent) )
		{
			gi_last_line = p_tmpline->iLineNum;
			*pi_value = atoi(p_tmpline->pcContent);
		}
		else
		{
			retval = R_INNOV_BOX_COMMON_ITEM_NOT_NUMERICAL;			
		}
	}
	else
	{
		retval = R_INNOV_BOX_COMMON_ITEM_NOFOUND;	
	}
	return retval;
}

//��ȡpc_section_name����pc_item_name��Ŀ�Ĳ�����Ϣ
//����ֵ: R_INNOV_BOX_COMMON_ITEM_NOFOUND -- û���ҵ�ָ����Ŀ
//                   R_INNOV_BOX_COMMON_ITEM_NOT_BOOLEAN -- ��Ŀ���ݲ��ǲ�������
int IBParseConfigTool::read_config(char* pc_section_name, char* pc_item_name, bool *pb_judge)
{
	int retval = R_INNOV_BOX_OK;
	gi_last_line = -1;//�����һ�ζ�ȡ���еļ�¼
	const Line_Info *p_tmpline = _get_line_info(pc_section_name ,pc_item_name);
	if(p_tmpline)
	{
		if( 0 == strcmp(p_tmpline->pcContent, "yes") )
		{
			gi_last_line = p_tmpline->iLineNum;
			*pb_judge = true;
		}
		else if( 0 == strcmp(p_tmpline->pcContent, "no") )
		{
			gi_last_line = p_tmpline->iLineNum;
			*pb_judge = false;
		}
		else
		{
			_config_file_error_message(p_tmpline->iLineNum, (char *)"item content isn't a bool type");
			retval = R_INNOV_BOX_COMMON_ITEM_NOT_BOOLEAN;
		}
	}
	else
	{
		retval = R_INNOV_BOX_COMMON_ITEM_NOFOUND;	
	}
	return retval;
}
//��ȡpc_section_name����pc_item_name��Ŀ��ö����Ϣ
//����ֵ: R_INNOV_BOX_COMMON_ITEM_NOFOUND -- û���ҵ�ָ����Ŀ
//                   R_INNOV_BOX_COMMON_ITEM_NOT_ENUMERATED -- ��Ŀ���ݲ���ö������
//                   R_INNOV_BOX_COMMON_ITEM_ENUMER_OUT_RANGE -- ��Ŀ�����������ö������Խ��
int IBParseConfigTool::read_config(char* pc_section_name, char* pc_item_name, int *pi_num, int i_area)
{
	int retval = R_INNOV_BOX_OK;
	gi_last_line = -1;//�����һ�ζ�ȡ���еļ�¼
	const Line_Info *p_tmpline = _get_line_info(pc_section_name ,pc_item_name);
	if(p_tmpline)
	{
		if( _check_str_num(p_tmpline->pcContent) )
		{
			gi_last_line = p_tmpline->iLineNum;
			*pi_num = atoi(p_tmpline->pcContent);
			retval = 0;			
			if( i_area < *pi_num || 0 > *pi_num )
			{
				_config_file_error_message(p_tmpline->iLineNum, (char *)"item content is over area");
				retval = R_INNOV_BOX_COMMON_ITEM_ENUMER_OUT_RANGE;
			}
		}
		else
		{
			_config_file_error_message(p_tmpline->iLineNum, (char *)"item content isn't a enum type");
			retval = R_INNOV_BOX_COMMON_ITEM_NOT_ENUMERATED;
		}
	}
	else
	{
		retval = R_INNOV_BOX_COMMON_ITEM_NOFOUND;		
	}
	return retval;
}

//�ͷŽṹ������gpp_line_info���������������Ϣ
int IBParseConfigTool::read_config_unload()
{
	Line_Info *p_next,*p_tmpline ;
	p_tmpline = gp_line_info_head;
	while(p_tmpline)
	{
		p_next = p_tmpline->pNext;
		if( NULL != p_tmpline->pcTypeName )
		{
			free(p_tmpline->pcTypeName);
			p_tmpline->pcTypeName = NULL;
		}
		if( NULL != p_tmpline->pcContent )
		{
			free(p_tmpline->pcContent);
			p_tmpline->pcContent = NULL;
		}
		free(p_tmpline);
		p_tmpline = p_next;
	}

	gp_line_info_head = NULL;
	
	if( NULL != gc_file_name )
	{
		free(gc_file_name);
		gc_file_name = NULL;
	}
	
	return R_INNOV_BOX_OK;
}

//��ӡ��һ�е��߼�������Ϣ
void IBParseConfigTool::read_config_logic_error(char * pc_error_message)
{
    	_config_file_error_message(gi_last_line, pc_error_message);
}

