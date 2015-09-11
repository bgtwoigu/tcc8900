#ifndef INNOV_BOX_TYPES_H
#define INNOV_BOX_TYPES_H

//Innov Box Return values 
typedef enum _Innov_Box_Retval
{
	R_INNOV_BOX_END = -10000,

	R_INNOV_BOX_COMMON_END = -599,
	R_INNOV_BOX_COMMON_PARAMETER_ERR,							//��������
	R_INNOV_BOX_COMMON_LOAD_CONFIG_ERR,						//load�����ļ�ʧ��
	R_INNOV_BOX_COMMON_DEBUG_UNINITIALIZED,					//û���ҵ�ָ��ģ�飬����debugû�г�ʼ��
	R_INNOV_BOX_COMMON_DEBUG_LEVEL_ERR,						//����debug_printf ��debug_printf_modʱi_level�����ò���ȷ
	R_INNOV_BOX_COMMON_ITEM_NOFOUND,							//û���ҵ�ָ����Ŀ
	R_INNOV_BOX_COMMON_BUF_LACKED,								//buf�ռ䲻��
	R_INNOV_BOX_COMMON_ITEM_NOT_NUMERICAL,					//��Ŀ���ݲ�����ֵ
	R_INNOV_BOX_COMMON_ITEM_NOT_ENUMERATED,					//��Ŀ���ݲ���ö��ֵ
	R_INNOV_BOX_COMMON_ITEM_NOT_BOOLEAN,						//��Ŀ���ݲ��ǲ���ֵ
	R_INNOV_BOX_COMMON_ITEM_ENUMER_OUT_RANGE,				//��Ŀ������ö�����ͣ����������趨�ķ�Χ
	//����common�ķ���ֵ
	R_INNOV_BOX_COMMON_BEGIN = -300,

	R_INNOV_BOX_CONTROL_ERR_END = -299,
	R_INNOV_BOX_CONTROL_COMMUNICATION_WITH_MONITOR_ERR,	//��controler_monitor���̽���ͨѶ���Ӵ���
	R_INNOV_BOX_CONTROL_INI_ERR,									//��ȡ�����ļ�controler.ini����
	R_INNOV_BOX_CONTROL_SERVER_INIT_ERR,						//controler���������̵�ͨѶ�����ʼ������
	R_INNOV_BOX_CONTROL_CFG_ERR,								//���������������ļ�control.cfg����
	R_INNOV_BOX_CONTROL_SELECT_ERR,								//select����
	R_INNOV_BOX_CONTROL_FORK_ERR,								//fork����
	R_INNOV_BOX_CONTROL_SOCKET_ERR,								//socket����
	R_INNOV_BOX_CONTROL_PARAMETER_ERR,							//��������
	R_INNOV_BOX_CONTROL_INVALID_MESSAGE,						//�Ƿ���Ϣ
	//����controler�Ĵ��󷵻�ֵ......
	R_INNOV_BOX_CONTROL_ERR_BEGIN = -200,

	
	R_INNOV_BOX_NET_END = -199,
	//����netmanager�ķ���ֵ
	R_INNOV_BOX_NET_IWLIST_ERR,
	R_INNOV_BOX_NET_MASK_ERR,
	R_INNOV_BOX_NET_IP_ERR,
	R_INNOV_BOX_NET_3GINIT_ERR,
	R_INNOV_BOX_NET_WIFIINIT_ERR,
	R_INNOV_BOX_NET_INIT_ERR,
	R_INNOV_BOX_NET_INIT_ONCE,
	R_INNOV_BOX_NET_BEGIN = -100,


	//����diskmanager�ķ���ֵ
	R_INNOV_BOX_DISK_END = -99,
	R_INNOV_BOX_DISK_MOUNT_ERR,
	R_INNOV_BOX_DISK_CHECK_OK,
	R_INNOV_BOX_DISK_INIT = -20,
	

	R_INNOV_BOX_SOCK_CREATE_ERR ,
	R_INNOV_BOX_SOCK_BIND_ERR,
	R_INNOV_BOX_SOCK_RECV_ERR,
	R_INNOV_BOX_SOCK_BUFLEN_ERR,
	R_INNOV_BOX_SOCK_BUFMALLOC_ERR,
	R_INNOV_BOX_SOCK_TIMEOUT,
	R_INNOV_BOX_SOCK_OPT_ERR = -10,


	R_INNOV_BOX_ERROR = -1,
	R_INNOV_BOX_OK = 0,


	R_INNOV_BOX_CONTROL_MESSAGE_BEGIN = 200,
	R_INNOV_BOX_CONTROL_TIMEOUT,								//select����Ϣ��ʱ
	//����controler����Ϣ����ֵ......
	R_INNOV_BOX_CONTROL_MESSAGE_END = 299,
	
}Innov_Box_Retval;

typedef int 		IB_CONN_HNDL;
typedef void *  	XML_ELEMENT;


//innnov box debug  level 
#define IB_DBGL_FATAL			0x01       // will exit/abort
#define IB_DBGL_ERR			       0x02       // continues
#define IB_DBGL_WARN			0x04       // only warning
#define IB_DBGL_INFO				0x08      // help message
#define IB_DBGL_DEBUG             	0x10      // debug 

#define IB_DBGL_ON				0x1F
#define IB_DBGL_OFF 				0x10

#define IB_CONN_ETH                       0x01
#define IB_CONN_WIFI                     0x02
#define IB_CONN_3G                         0x04
#define IB_CONN_ALL                       0x1F

#define IB_CONN_HNDL_NULL   		R_INNOV_BOX_ERROR
#define XML_ELEMENT_NULL     		NULL

#endif
