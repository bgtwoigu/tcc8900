#ifndef _GPS_INTERFACE_H_
#define _GPS_INTERFACE_H_

#include <stdio.h>
#include <map>
#include <vector>
#include <time.h>
#include <cstring>

using namespace std;

#define POSITION_INT_TO_FLOAT(x)          ((double)(x)) / ((double)(10*10*10*10*10*10))

// Read tuid from hardware.
// Output: tuid vector
// Return: 0 success; other fail
int InnovReadTuid(vector<unsigned char> &tuid);

typedef enum FenceType
{
    FENCE_TYPE_CIRCLE = 0,
    FENCE_TYPE_RECTANGLE,
    FENCE_TYPE_POLYGON,
    FENCE_TYPE_PATH,
    FENCE_TYPE_BUTT
} EnumFenceType;

class Point
{
public:
	Point(double x,double y, int lat, int lon)
	{
		px = x;
		py = y;
        latitude = lat;
        longitude = lon;
	}
	double getx(){return px;}
	double gety(){return py;}
	int getlatitude() {return latitude;}
	int getlongitude() {return longitude;}
private:
	int latitude;//γ�ȱ�ʾ ��γ---0   ��γ---1
	int longitude;//���ȱ�ʾ  ����---0   ����---1
	double px;//���x���꣬γ��
	double py;//���x���꣬����
};

/****ElectronicFenceData�ṹ�巵��ֵ˵��========
===���ͣ�Բ
point_count = 1
radius = Բ�İ뾶����λ��
points��ֻ��һ���㣬��Բ�ĵ�����
===���ͣ�����
point_count = 2
radius��ֵ��Ч��
points���������㣬�����Ͻ����꣬���½�����
===���ͣ������
point_count = N ������εĶ�������
radius��ֵ��Ч��
points����N���㣬����������
===���ͣ��߶�
point_count = N ���߶ε���ʼ���յ��Լ������������
radius = �߶εĿ�ȣ���λ��
points����N���㣬���߶ε���ʼ�㣬�յ㣬����������
====ʱ��
��ʼ��ʾʱ���������ʾʱ��ȷ����һ��ʱ��Σ��ڸ�ʱ���ڵ�ͼ����ʾΧ��*/
typedef struct ElectronicFenceData{
        int eid;
        long radius;//�뾶
        int point_count;//��ĸ���
        vector<unsigned char> tm_start;//��ʼ��ʾʱ��
        vector<unsigned char> tm_stop;//������ʾʱ��
        vector<Point> points;
}StruElectronicFenceData;

//����ֵ0��ȡ�ɹ�����0��ȡʧ��
int getElectronicFenceByType(EnumFenceType type,int eid,struct ElectronicFenceData *pdata);//typeΧ�������ͣ�eidΧ�������ݿ��е�id�ţ�Χ�������ݱ�����pdata��

int getAllElectronicFenceByType(EnumFenceType type, vector<struct ElectronicFenceData> &data);

#endif
