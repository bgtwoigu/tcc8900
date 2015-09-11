#ifndef DEVICE_MANAGER_SLEEP_MODE
#define DEVICE_MANAGER_SLEEP_MODE

#define	Hw31								0x80000000
#define	Hw30								0x40000000
#define	Hw29								0x20000000
#define	Hw28								0x10000000
#define	Hw27								0x08000000
#define	Hw26								0x04000000
#define	Hw25								0x02000000
#define	Hw24								0x01000000
#define	Hw23								0x00800000
#define	Hw22								0x00400000
#define	Hw21								0x00200000
#define	Hw20								0x00100000
#define	Hw19								0x00080000
#define	Hw18								0x00040000
#define	Hw17								0x00020000
#define	Hw16								0x00010000
#define	Hw15								0x00008000
#define	Hw14								0x00004000
#define	Hw13								0x00002000
#define	Hw12								0x00001000
#define	Hw11								0x00000800
#define	Hw10								0x00000400
#define	Hw9									0x00000200
#define	Hw8									0x00000100
#define	Hw7									0x00000080
#define	Hw6									0x00000040
#define	Hw5									0x00000020
#define	Hw4									0x00000010
#define	Hw3									0x00000008
#define	Hw2									0x00000004
#define	Hw1									0x00000002
#define	Hw0									0x00000001


//location status

#define LOCSTATUS_CARDOOR_LOCK              Hw12    //0: car door unlock; 1: lock
#define LOCSTATUS_CIRCUIT_OFF               Hw11    //0: circuit normal; 1: off
#define LOCSTATUS_OILWAY_OFF                Hw10    //0: oilway normal; 1: off
#define LOCSTATUS_LONGLATIT_ENCRYPT         Hw5     //0: longitude latitude not encrypt;  1:encrypt
#define LOCSTATUS_CAR_WORK_OFF              Hw4     //0: work; 1: not work
#define LOCSTATUS_EAST_WEST                 Hw3     //0: east; 1: west
#define LOCSTATUS_NORTH_SOUTH               Hw2     //0: north; 1:south
#define LOCSTATUS_FIXED_POSITION            Hw1		
#define LOCSTATUS_ACC_ONO                   Hw0		

//alarm
#define ALARM_STATUS_EMERGENCY_ALARM        Hw0
#define ALARM_STATUS_OVERSPEED_ALARM        Hw1
#define ALARM_STATUS_FATIGUE_DRIVING        Hw2
#define ALARM_STATUS_EARLY_WARNING          Hw3
#define ALARM_STATUS_GNSS_FAILURE           Hw4
#define ALARM_STATUS_GNSS_UNCONNECT         Hw5
#define ALARM_STATUS_GNSS_SHORT_CIRCUIT     Hw6
#define ALARM_STATUS_POWER_UNDER_VOLTAGE    Hw7
#define ALARM_STATUS_POWER_DOWN_ELECTRICITY Hw8
#define ALARM_STATUS_LCD_FAILURE            Hw9
#define ALARM_STATUS_TTS_FAILURE            Hw10
#define ALARM_STATUS_CAMERA_FAILURE         Hw11
#define ALARM_STATUS_SMOKE_ALARM            Hw12

#define ALARM_STATUS_DAY_DRIVING_OVERTIME   Hw18
#define ALARM_STATUS_PARKING_OVERTIIME      Hw19
#define ALARM_STATUS_IN_OUT_AREA            Hw20
#define ALARM_STATUS_IN_OUT_LINE            Hw21
#define ALARM_STATUS_ROAD_DRIVE_SHORT_LONG   Hw22
#define ALARM_STATUS_LINE_DEPARTURE_ALARM   Hw23
#define ALARM_STATUS_CAR_VSS_FAILURE        Hw24
#define ALARM_STATUS_CAR_OIL_FAILURE        Hw25
#define ALARM_STATUS_CAR_STOLEN_ALARM       Hw26
#define ALARM_STATUS_CAR_ILLEGAL_IGNITION   Hw27	//�Ƿ����
#define ALARM_STATUS_CAR_ILLEGAL_OFFSET     Hw28
#define ALARM_STATUS_CAR_COLLISION_ALARM    Hw29

#define SPEED_STATUS_OK                     0x01
#define SPEED_STATUS_ERROR                  0x02


typedef enum
{
    MCU_TIMER_DATA = 1,             
    UPLOAD_TIMER_DATA, 
    TEMP_TIMER_DATA,
    LINE_TIMER_DATA,            
    SPEED_TIMER_DATA,           
    PARK_TIMER_DATA,            
    STOP_TIMER_DATA,            
    START_TIMER_DATA,           
    DRIVE_TIMER_DATA,           
    FIRST_ALARM_TIMER_DATA,  
    SLEEP_TIMER_DATA,
    STATUS_TIMER_DATA,
}DevTimerData;

typedef enum
{
    DEFAULT_MODE = 0,    
    NOLOGIN_MODE,        
    SLEEP_MODE,         
    ALARM_MODE,         
}DevReportMode;

typedef enum
{
	PRP_TIMEING = 0,
	PRP_DISTANCE,
	PRP_TIMEING_DISTANCE,	
}PositionReportPolicy;

typedef enum
{
	PRS_ACC_STATUS = 0,
	PRS_LOGIN_ACC_STATUS,
}PositionReportScenario;

struct PRETIRED_DRIVING{
	char VehicleDriverID[18]; 
	int TiredTime;            
	int StoppingTime;         
	int TiredLongitude;       
	int TiredLatitude;        
	int TiredAltitude;        
	int StopLongitude;        
	int StopLatitude;         
	int StopAltitude;         
	int RunningTime;          
	int TripMeterCurDay;      
	int RunningTimeCurDay;    
	int Flag_TiredDrive;          
};
struct VDRSettingParam 
{
    int report_policy;	    //λ�û㱨����
    int	report_scenario;	//λ�û㱨����
    
    int nologin_report_interval;       //��ʻԱδ��¼�㱨ʱ����
    int sleep_report_interval;         //����ʱ�㱨ʱ����
    int alarm_report_interval;         //��������ʱ�㱨ʱ����
    int default_report_interval;       //ȱʡ�㱨ʱ����

    int default_report_distance;       //ȱʡ����㱨���
    int nologin_report_distance;       //��ʻԱδ��¼�㱨������
    int sleep_report_distance;         //����ʱ�㱨������
    int alarm_report_distance;         //��������ʱ�㱨������

    int warning_mask;
    int warning_photo_mask;
    int warning_photosave_mask;

    int max_speed;                     //����ٶ�
    int over_speed_duration;           //���ٳ���ʱ��
    int continue_drive_threshold;      //������ʻʱ������
    int day_drive_time_threshold;      //�����ۼƼ�ʻʱ������
    int min_resting_time;              //��С��Ϣʱ��
    int max_parking_time;              //�ͣ��ʱ��

    int collision_time;            //��ײʱ��
    int collision_acceleration;    //��ײ���ٶ�
    int rollover_angle;            //�෭��������: �෭�Ƕ�
};

struct VDRCurrentInfo
{
    int is_time_report;
    int is_distance_report;
    int report_mode;
    int report_time;
    int report_distance;
    int temp_report_validity;

    int status_enable;
    int status_level;
    
    int day_time;
    int day_drive_dura;
    int day_tmp_mileage;
    float day_drive_mileage;
    float total_mileage;      // m
    int max_speed;
    int over_speed_dura;
    int day_info_flag;

    int speed_timer_flag;  
    int park_timer_flag;
    int stop_timer_flag;
    int start_timer_flag;
    int drive_timer_flag;
    int sleep_timer_flag;
    int status_timer_flag;
    int first_alarm_timer_flag;
    int is_restart_first_alarm;
    int emergency_alarm_flag;
    int emergency_alarm_num;
    int car_reverse_flag;
    int car_reverse_count;
    int car_smoke_flag;
    int car_collision_flag;
    int undervoltage_flag;

    int gps_mcu_speed_flag;
    int gps_speed_count;
        
    int acc_on_time;
    int acc_off_time;
    int login_time;
//add 2 flag for tired arithmetic
    int stop_flag;
    int running_flag;
};

struct LineInflexionInfo
{
    int id;
    int lineid;
    int lineattribute;
    int maxtime;
    int mintime;
    int maxspeed;
    int overspeeddura;
};

struct ElectronicFenceData
{
    int instatus;
        
    int nowpoint;
    int temptime;
    
    int etype;
    int regionid;
    int attribute;

    struct LineInflexionInfo inflexion_info[0];
};
    
struct SpeedMileageInfo
{
    int index;
    int signal;             // 1sec status signal
    int sec_speed[60];      // 1sec speed       km/h
    float sec_distance[60];   // 1sec distcance  m
};

struct AdditionalInformation
{
    int overspeed_alarm_flag;
    int overspeed_region_id;
    int inout_alarm_flag;
    int inout_alarm_region_id;
    int inout_alarm_direction;
    int short_long_line_id;
    int short_long_drive_time;
    int short_long_result;
    int io_status;
    int analog_quantity;
    int wireless_intensity;
    int gnss_satellite_sum;
};

struct DriverLoginInfo
{
    int itime;
    int status;
    char driverid[18];
    char validate[3];
    char qualificationID[18];
    char name[16];
};

struct NaviGPSInfo
{
    int     position;
    int 	fX;				// ��ǰGPS����X
    int 	fY;				// ��ǰGPS����Y
    int     iTime;			// ��ǰ��GPSʱ��
    int     iSysTime;		// ��ǰ��ϵͳʱ��
    int     iOrientation;	// ������ʻ��λ��
    int     iVelocity;		// ������ʻ�ٶ�              km/h
    int     ilatitude;  //��ǰgps�����γ�� ��γ---0   ��γ---1
    int     ilongitude; //��ǰgps����ľ��� ����---0   ����---1
    int     ialtitude;  //��ǰ������Ϣ
};

struct SpeedStatusJudge
{
    int index;
    int beginsec;
    int result; 
    int day_speed_status;
    int vdr_speed[60];     // km/h
    int gps_speed[60];     // km/h
};

struct LocStatusData
{
    int type;
    int m_flag;     //full min flag 
    int up_alarm_flag;
    int upload_flag;
    int itime;
    int alarm_signal;
    int status_falg;
    int latitude;
    int longitude;
    int altitude;
    int orientation;
    int gps_speed;
    int total_mileage;
    int oil_mass;
    int vdr_status_old_hb;
    int vdr_status_new_hb;
    int vdr_speed;
    int vdr_distance;
    int overspeed_flag;
    int overspeed_id;
    int fence_inout_alarm;
    int fence_inout_id;
    int fence_inout_direction;
    int fence_line_id;
    int fence_line_time;
    int fence_line_result;
    int emergency_num;    
    int io_status;
    int analog_quantity;
    int wireless_intensity;
    int gnss_satellite_sum;
}

#endif

