/**
*<FH+>***************************************************************************
*
* ��Ȩ���� (C)2011, Զ�ؿƼ����޹�˾
* 
* �ļ����ƣ� KeyWordsMgr.h
* �ļ���ʶ�� (�����ù���ƻ���)
* ����ժҪ�� 
* ����˵���� 
* ��ǰ�汾�� V1.0
* ��    �ߣ� ������
* ������ڣ� [2011/11/14]
* �޸ļ�¼1��
*    �޸����ڣ�
*    �� �� �ţ�
*    �� �� �ˣ�
*    �޸����ݣ� 
*<FH+>***************************************************************************
*/ 

#ifndef KEYWORDSMGR_H
#define KEYWORDSMGR_H

#include <sqlite3.h>
#include <QList>
#include <QString>
#include "voiceDefine.h"

/**
*<CLASS+>********************************************************************
* �� �� ��: KeyWordsMgrC
* ��������: �ؼ��ʹ���
* ����˵��: 
* �޸ļ�¼: 
* �޸�����			�� ��		�޸���		�޸�ժҪ
* -------------------------------------------------------------------------
* [2011/11/14]	    V1.0		������		������
*<CLASS->*********************************************************************
*/
class KeyWordsMgrC
{
public:
    static KeyWordsMgrC *object();
    static void releaseObject();

    int insertKeyWord(KeyWordList keyWordList);
    int modifyKeyWord(const QString strSourceKeyWord, const QString strObjKeyWord);
    int getKeyWord(KeyWordList &keywordList);
    int getKeyWordId(QString &strKeywordId);
    int insertKeyWordId(const char *strKeyWordId);

private:
    KeyWordsMgrC();
    virtual ~KeyWordsMgrC();

    static KeyWordsMgrC *thisObject;

    bool _keyWordExist();
    int _createDatabase();
    int _uploadKeyword();
};

#endif