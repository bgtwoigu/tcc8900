#ifndef _DEVICE_MANAGER_DBUS_MESSAGE_H_
#define _DEVICE_MANAGER_DBUS_MESSAGE_H_

#include <stdio.h>

#include "dbusMessage.h"

#define DEVICE_MANAGER_DBUS_SERVER_ID              (char*)"com.innov.devicemanager"
#define DEVICE_MANAGER_DBUS_SERVER_OBJ             (char*)"/devicemanager"

#define DEVICE_MANAGER_NOTF_ID_DRIVE_LOGIN          5600
#define DEVICE_MANAGER_NOTF_ID_FENCE_INOUT          5601
#define DEVICE_MANAGER_NOTF_ID_ACCOFF               5602    //�ػ�֪ͨ
#define DEVICE_MANAGER_NOTF_ID_REVERSE              5603    //����
#define DEVICE_MANAGER_NOTF_ID_SPEED_ABNORMAL       5604    //�ٶ�״̬�쳣
#define DEVICE_MANAGER_NOTF_ID_TIRED                5605    //��ʱ��ʻ
#define DEVICE_MANAGER_NOTF_ID_ALARM                5606    // ����
#define DEVICE_MANAGER_NOTF_ID_CAMERA               5607    //����
#define DEVICE_MANAGER_NOTF_ID_RUN_STOP             5608    //����״̬ ֹͣ״̬
#define DEVICE_MANAGER_NOTF_ID_DB9_CHECk            5609    //DB9 check 
#define DEVICE_MANAGER_NOTF_ID_UPDATE_MILEAGE       5610    //���¶�ȡ��ʼ���
#define DEVICE_MANAGER_NOTF_ID_SMS_ALARM_INFO       5611    //���������ı�SMS
#define DEVICE_MANAGER_NOTF_ID_ACC_INFO             5612    //acc ״̬on off ֪ͨ


#define REQ_ID_REBOOT_MACHINE                       5700
#define REQ_ID_SHUTDOWN_MACHINE                     5701
#define REQ_ID_CHECK_TIME                           5702
#define REQ_ID_CARRY_STATUS                         5703

#define REQ_ID_DATA_UP_PASSTHROUGH                  5704    //��������͸��
#define REQ_ID_SPEED_RATIO_UPDATE                   5705
#define REQ_ID_GPS_SPEED_REPLACE_VDR_SPEED          5706    //gps speed ��� vdr speed

class DeviceDriveLoginDbusNotifyC: public  DBusNotifyC
{
    DBUS_NOTIFY_OBJECT(DeviceDriveLoginDbusNotifyC)

public:
    DeviceDriveLoginDbusNotifyC() : DBusNotifyC(DEVICE_MANAGER_NOTF_ID_DRIVE_LOGIN) {}
        
    void print(FILE *fp) {fprintf(fp,"\tdriverid=%s\r\n",driverid);}

    int command; //
    int type; //
    char driverid[20]; 
};

class DeviceFenceInOutDbusNotifyC: public DBusNotifyC
{
    DBUS_NOTIFY_OBJECT(DeviceFenceInOutDbusNotifyC)

public:
    DeviceFenceInOutDbusNotifyC() : DBusNotifyC(DEVICE_MANAGER_NOTF_ID_FENCE_INOUT) {}
        
    void print(FILE *fp) {fprintf(fp,"\tflag=%d\r\n",flag);}

    int flag;     // 1:��������  0:�˳����� 2: UI����ʾ
    int cm_flag;  // 1:����ͨ�� 0:�ر�ͨ��
    int gnss_flag; // 1:�ɼ�GNSS,  0:���ɼ�GNSS
};

class DeviceAccOffDbusNotifyC: public DBusNotifyC
{
    DBUS_NOTIFY_OBJECT(DeviceAccOffDbusNotifyC)

public:
    DeviceAccOffDbusNotifyC() : DBusNotifyC(DEVICE_MANAGER_NOTF_ID_ACCOFF) {}
        
    void print(FILE *) {}
    int flag; // 0 normal , 1 remotectrl shutdown , 2 reboot
};

class DeviceReverseDbusNotifyC: public DBusNotifyC
{
    DBUS_NOTIFY_OBJECT(DeviceReverseDbusNotifyC)

public:
    DeviceReverseDbusNotifyC() : DBusNotifyC(DEVICE_MANAGER_NOTF_ID_REVERSE) {}
        
    void print(FILE *fp) {fprintf(fp,"\tflag=%d,icount=%d\r\n",flag,icount);}

    int flag;  // 1 reverse, 0  cancel
    int icount;
};

class DeviceSpeedAbnormalDbusNotifyC: public DBusNotifyC
{
    DBUS_NOTIFY_OBJECT(DeviceSpeedAbnormalDbusNotifyC)

public:
    DeviceSpeedAbnormalDbusNotifyC() : DBusNotifyC(DEVICE_MANAGER_NOTF_ID_SPEED_ABNORMAL) {}
        
    void print(FILE *fp) {fprintf(fp,"\tflag=%d\r\n",flag);}
    int flag;   //0:������1:�쳣
};

class DeviceTiredDbusNotifyC: public DBusNotifyC
{
    DBUS_NOTIFY_OBJECT(DeviceTiredDbusNotifyC)

public:
    DeviceTiredDbusNotifyC() : DBusNotifyC(DEVICE_MANAGER_NOTF_ID_TIRED),flag(0),dura(0) {
	    memset(driverid,0,18);
    }
        
    void print(FILE *fp) {fprintf(fp,"\tflag=%d\r\n",flag);}

    int flag;   // 0 :(tired_time-30)��tired_time 1: tired_time��tired_time+30 2:����tired_time+30 3:���ƣ�ͼ�ʻ (tired_timeΪƣ�ͼ�ʻʱ������)
    int dura;   // ������ʻʱ��
    char driverid[18];
};

typedef enum
{
    DEV_ALARM_EMERGENCY_ALARM = 0,      // 0: ��������
    DEV_ALARM_SMOKE_ALARM,              // 1:  ������
    DEV_ALARM_COLLISION_ALARM,          // 2: ��ײ
    DEV_ALARM_PARKING_OVERTIIME,        // 3: ��ʱͣ��
    DEV_ALARM_POWER_UNDER_VOLTAGE,       // 4: ����ԴǷѹ
    DEV_ALARM_TURNOVER_ALARM,            // 5: �෭
    DEV_ALARM_OVERSPEED_WARNING,         // 6: ����Ԥ�� 
    DEV_ALARM_OVERSPEED_ALARM           // 7: ���ٱ���
}ENUM_DEV_ALARM_COL;

class DeviceAlarmDbusNotifyC: public DBusNotifyC
{
    DBUS_NOTIFY_OBJECT(DeviceAlarmDbusNotifyC)

public:
    DeviceAlarmDbusNotifyC() : DBusNotifyC(DEVICE_MANAGER_NOTF_ID_ALARM) {}
        
    void print(FILE *fp) {fprintf(fp,"\tflag=%d\r\n",flag);}

    int type;       
    int flag;       //  1:  ����  0 : �������
};

class DeviceCameraDbusNotifyC: public DBusNotifyC
{
    DBUS_NOTIFY_OBJECT(DeviceCameraDbusNotifyC)

public:
    DeviceCameraDbusNotifyC() : DBusNotifyC(DEVICE_MANAGER_NOTF_ID_CAMERA){}

    void print(FILE *fp)
    {
        fprintf(fp,"\tm_channel=[%d]\r\n", m_channel);
        fprintf(fp,"\tm_command=[%d]\r\n", m_command);
        fprintf(fp,"\tm_interval=[%d]\r\n", m_interval);
        fprintf(fp,"\tm_flag=[%d]\r\n", m_flag);
        fprintf(fp,"\tm_resolution=[%d]\r\n", m_resolution);
        fprintf(fp,"\tm_event=[%d]\r\n", m_event);
    }
    
    int m_channel;      // ͨ�����ر�־ λ����  0 :������  1:����
    int m_command;      //��������     0:ֹͣ����   0xFFFF:¼��     n:  Number of Pictures
    int m_interval;     //���ռ�� n  0:��Сʱ����
    int m_flag;         // �����־   λ����      0: �洢   1: �ϴ� 
    int m_resolution;   // �ֱ���        0x02: 640*480
    int m_event;       
                        //�¼�����   0:ƽ̨�·�   1:��ʱ 2:���ٱ��� 3:��ײ�෭
                        //                        4:�ſ� 5:�Ź� 6: �ſ���20<speed  7:����8:����
};

class DeviceRunStopDbusNotifyC: public DBusNotifyC
{
    DBUS_NOTIFY_OBJECT(DeviceRunStopDbusNotifyC)

public:
    DeviceRunStopDbusNotifyC() : DBusNotifyC(DEVICE_MANAGER_NOTF_ID_RUN_STOP) {}
        
    void print(FILE *fp) {fprintf(fp,"\tflag=%d�� itime=%d\r\n",flag,itime);}

    int flag;       //  0:����״̬  1 : ͣ��״̬
    int itime;      //  ����ʱ��
};

class DeviceDB9CheckDbusNotifyC: public DBusNotifyC
{
    DBUS_NOTIFY_OBJECT(DeviceDB9CheckDbusNotifyC)

public:
    DeviceDB9CheckDbusNotifyC() : DBusNotifyC(DEVICE_MANAGER_NOTF_ID_DB9_CHECk) {}
        
    void print(FILE *fp) {fprintf(fp,"\tflag=%d\r\n",flag);}

    int flag;       //  0:�˳��춨״̬  1 : ����춨״̬
};

class DeviceUpdateMileageNotifyC: public DBusNotifyC
{
    DBUS_NOTIFY_OBJECT(DeviceUpdateMileageNotifyC)

public:
    DeviceUpdateMileageNotifyC() : DBusNotifyC(DEVICE_MANAGER_NOTF_ID_UPDATE_MILEAGE) {}
    
    int mileage;// ���µĳ�ʼ��� ��λ: 0.1km
};

class DeviceSMSAlarmNotifyC: public DBusNotifyC
{
    DBUS_NOTIFY_OBJECT(DeviceSMSAlarmNotifyC)

public:
    DeviceSMSAlarmNotifyC() : DBusNotifyC(DEVICE_MANAGER_NOTF_ID_SMS_ALARM_INFO) {}
    
    int s_alarm; //����λ
    int s_itime; //����ʱ��
    int s_fx;
    int s_fy;
};

class DeviceACCInfoDbusNotifyC: public DBusNotifyC
{
    DBUS_NOTIFY_OBJECT(DeviceACCInfoDbusNotifyC)

public:
    DeviceACCInfoDbusNotifyC() : DBusNotifyC(DEVICE_MANAGER_NOTF_ID_ACC_INFO) {}
        
    void print(FILE *fp) {fprintf(fp,"\tflag=%d\r\n",flag);}

    int flag;       //  0: acc off;  1: acc on 
};


class RebootMachineReqC: public DBusRequestC 
{ 
    DBUS_REQUEST_OBJECT(RebootMachineReqC) 

public: 
    RebootMachineReqC():DBusRequestC(REQ_ID_REBOOT_MACHINE){} 
};

class ShutdownMachineReqC: public DBusRequestC 
{ 
    DBUS_REQUEST_OBJECT(ShutdownMachineReqC) 

public: 
    ShutdownMachineReqC():DBusRequestC(REQ_ID_SHUTDOWN_MACHINE){} 
};

class CheckTimeReqC: public DBusRequestC 
{ 
    DBUS_REQUEST_OBJECT(CheckTimeReqC) 

public: 
    CheckTimeReqC():DBusRequestC(REQ_ID_CHECK_TIME){} 
};

class CarryStatusReqC: public DBusRequestC 
{ 
    DBUS_REQUEST_OBJECT(CarryStatusReqC) 

public: 
    CarryStatusReqC():DBusRequestC(REQ_ID_CARRY_STATUS){} 
    int carry_status;//0:�ճ�   1:����    2:����    3:����
};

class DeviceDataUpPassReqC: public DBusRequestC 
{ 
    DBUS_REQUEST_OBJECT(DeviceDataUpPassReqC) 

public: 
    DeviceDataUpPassReqC():DBusRequestC(REQ_ID_DATA_UP_PASSTHROUGH){} 
    void print(FILE *fp) {fprintf(fp,"\tflag=%d\r\n",flag);}

    int flag;  // 1: ��������͸��
};

class SpeedRatioUpdateReqC: public DBusRequestC 
{ 
    DBUS_REQUEST_OBJECT(SpeedRatioUpdateReqC) 

public: 
    SpeedRatioUpdateReqC():DBusRequestC(REQ_ID_SPEED_RATIO_UPDATE){} 
};

class GPSReplaceVDRSpeedReqC: public DBusRequestC 
{ 
    DBUS_REQUEST_OBJECT(GPSReplaceVDRSpeedReqC) 

public: 
    GPSReplaceVDRSpeedReqC():DBusRequestC(REQ_ID_GPS_SPEED_REPLACE_VDR_SPEED){} 
    void print(FILE *fp) {fprintf(fp,"\tflag=%d\r\n",flag);}

    int flag;  //  1:  gps speed replace vdr speed;  0: cancel;
};

#endif
