/***
* @file: nmea_stdlib.cpp
* nmea_stdlib.cppʵ�ֵĹ��ܺ�C��׼���stdlib.h���ƣ���Ҫ���NMEA��Ϣ������Ӧ�ã���д��
* strtod()��strtol()������������ֱ�Ϊstr2dx()��str2lx()���ӿں�ʵ�ֵ�˼��ͱ�׼�����ơ�
*/

#include "nmea_type.h"

#define ISSPACE(x) ((x)==' ')
#define DEC_CHAR(x) ((x) >= 48 && (x) <= 57 || (x) >= 97 && (x) <= 102 || (x) >= 65 && (x) <= 70)
#define HEX_CHAR(x) (DEC_CHAR(x) || (x) >= 97 && (x) <= 102 || (x) >= 65 && (x) <= 70)
#define FLAOT_CHAR(x) (DEC_CHAR(x) || (x)=='.')
#define LETTER_CHAR(x) ((x)>='a' && (x)<='z' || (x)>='A' && (x)<='Z')
#define NMEA_CHAR(x) (FLAOT_CHAR(x) || LETTER_CHAR(x) || (x)==',' || (x)=='*' || (x)=='$' || (x)==0x0D || (x)==0x0A)

// �ַ�ת��Ϊ���֣����ְ���{0-9,A,B,C,D,E,F}
char char2int(char c)
{
	if (c >= 48 && c <= 57)
	{
		return c-48;
	}
	else if (c >= 97 && c <= 102)
	{
		return (c-87);
	}
	else if(c >= 65 && c <= 70)
	{
		return (c-55);
	}
	else
	{
		return 0;
	}
}

// �ַ���ת��Ϊ�з�������
// base�ǻ�����������10��DEC����16��HEX��
// endptr:�����ַ���ĩβ�ĵ�ַ��
// ����ϸ��˵������ο�c��׼��strtol�Ľ���(stdlib.h)
long str2lx(const char *s,char **endptr,int base)
{
	const char *sc;
	char sign;
	long x,y;

	for (sc=s;ISSPACE(*sc);++sc) ;

	sign = (*sc=='-' || *sc=='+')?*sc++ : '+';

	switch (base)
	{
	case 10:
		break;
	case 16:
		if (*sc == '0' && (sc[1]=='x' || sc[1]=='X'))
		{
			sc += 2;
		}
		break;
	default:
		if (endptr)
		{
			*endptr = (char*)s;
		}
		return 0;
	}

	for (; *sc == '0'; ++sc) ;
	y = 0;

	// exp: 56732 = 5*10000+6*1000+7*100+3*10+2 = 2 + 10*( 3 + 10*( 7+ 10*( 6 + 10*5 ) ) )
	for (;HEX_CHAR(*sc);++sc)
	{
		x = char2int(*sc);
		y = y*base + x;
	}

	*endptr = (char*)sc;
	y *= ((sign=='-')?-1:1);

	return y;
}

// �ַ���ת��Ϊ������
// endptr:�����ַ���ĩβ�ĵ�ַ��
// ����ϸ��˵������ο�c��׼��strtod�Ľ���(stdlib.h)
double str2dx(const char *s,char **endptr)
{
	const char *sc;
	const char *sd;
	char sign;
	long x,y;
	double y1;

	for (sc=s;ISSPACE(*sc);++sc) ; // ȥ��ǰ��Ŀո�

	sign = (*sc=='-' || *sc=='+')?*sc++ : '+';

	for (; *sc == '0'; ++sc) ; // ȥ����λ0
	y = 0;

	// ȡ����
	for (;HEX_CHAR(*sc);++sc)
	{
		x = char2int(*sc);
		y = y*10 + x;
	}

	if (*sc == '.')
	{
		// ȡС��
		y1 = 0.0;
		for (sd = sc+1;DEC_CHAR(*sd);++sd) ;
		*endptr = (char*)(sd);

		for (--sd;sd != sc;--sd)
		{
			x = char2int(*sd);
			y1 = (x+y1)*0.1;
		}

		y1 += y;
	}
	else
	{
		*endptr = (char*)sc;
		y1 = y;
	}
	y1 *= ((sign=='-')?-1:1);

	return y1;
}

int strlenx(const char* pstr)
{
	int len;

	for(len=0;*pstr != '\0';len++) pstr++;

	return len;
}

unsigned char CheckSum(const char *pMsg,int *Idx)
{
	unsigned char ucChkSum = 0;

	*Idx = 1;
	for (;*pMsg != '$';++pMsg) (*Idx)++;

	for (++pMsg;*pMsg != '*';++pMsg,(*Idx)++)
	{
		if (!NMEA_CHAR(*pMsg))
			break;
		ucChkSum ^= *pMsg;
	}

	return ucChkSum;
}