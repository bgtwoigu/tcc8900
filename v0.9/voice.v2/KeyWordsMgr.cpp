/**
*<FH+>***************************************************************************
*
* ��Ȩ���� (C)2011, Զ�ؿƼ����޹�˾
* 
* �ļ����ƣ� KeyWordsMgr.cpp
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

#include "KeyWordsMgr.h"
#include "common_config.h"
#include "qisr.h"

KeyWordsMgrC *KeyWordsMgrC::thisObject = NULL;


/**
*<FUNC+>********************************************************************
* �������ƣ� KeyWordsMgrC
* ���������� ���캯��
* ��������� ��
* ��������� ��
* �� ��	ֵ�� ��
* ����˵���� 
* �޸ļ�¼�� 
* 		�޸�����        �汾��     �޸���	      �޸�����
* --------------------------------------------------------------------------
* 	[2011/11/14]	    V1.0	   ������		  ��������
*<FUNC+>********************************************************************
*/
KeyWordsMgrC::KeyWordsMgrC()
{
}

/**
*<FUNC+>********************************************************************
* �������ƣ� KeyWordsMgrC
* ���������� ��������
* ��������� ��
* ��������� ��
* �� ��	ֵ�� ��
* ����˵���� 
* �޸ļ�¼�� 
* 		�޸�����        �汾��     �޸���	      �޸�����
* --------------------------------------------------------------------------
* 	[2011/11/14]	    V1.0	   ������		  ��������
*<FUNC+>********************************************************************
*/
KeyWordsMgrC::~KeyWordsMgrC()
{
}

/**
*<FUNC+>********************************************************************
* �������ƣ� object
* ���������� �������
* ��������� ��
* ��������� ��
* �� ��	ֵ�� ���ض���ָ��
* ����˵���� 
* �޸ļ�¼�� 
* 		�޸�����        �汾��     �޸���	      �޸�����
* --------------------------------------------------------------------------
* 	[2011/11/14]	    V1.0	   ������		  ��������
*<FUNC+>********************************************************************
*/
KeyWordsMgrC* KeyWordsMgrC::object()
{
    KeyWordsMgrC *pTmp = NULL;

    if(thisObject == NULL)
    {
        thisObject = new KeyWordsMgrC;
    }

    pTmp = thisObject;

    return pTmp;
}

/**
*<FUNC+>********************************************************************
* �������ƣ� releaseObject
* ���������� �ͷŶ���
* ��������� ��
* ��������� ��
* �� ��	ֵ�� ��
* ����˵���� 
* �޸ļ�¼�� 
* 		�޸�����        �汾��     �޸���	      �޸�����
* --------------------------------------------------------------------------
* 	[2011/11/14]	    V1.0	   ������		  ��������
*<FUNC+>********************************************************************
*/
void KeyWordsMgrC::releaseObject()
{
    if (thisObject != NULL)
    {
        delete thisObject;
        thisObject = NULL;
    }
}

/**
*<FUNC+>********************************************************************
* �������ƣ� insertKeyWord
* ���������� ����ؼ��ʵ����е����ݿ��У�û�оʹ������ݿ��
* ��������� keyWordList���ؼ����б�
* ��������� ��
* �� ��	ֵ�� �ɹ�����0�����򷵻ش�����
* ����˵���� 
* �޸ļ�¼�� 
* 		�޸�����        �汾��     �޸���	      �޸�����
* --------------------------------------------------------------------------
* 	[2011/11/14]	    V1.0	   ������		  ��������
*<FUNC+>********************************************************************
*/
int KeyWordsMgrC::insertKeyWord(KeyWordList keyWordList)
{
    if (keyWordList.empty())
    {
        printf("insertKeyWord, keyWordList is empty\n");
        return -1;
    }

    int iRet = 0;
    sqlite3 *pDb = NULL;
    sqlite3_stmt *pStmt = NULL;
    char *strError = NULL;

    // ������ݿ����Ƿ��б�
    if (!_keyWordExist())
    {
        // ���û�У��ȴ������ݱ�����������
        iRet = _createDatabase();
        if (iRet != 0)
        {
            printf("insertKeyWord, _createDatabase failed %d\n", iRet);
            return iRet;
        }
    }

    // �����ݴ�����ݿ���
    // �����ݿ�
    iRet = sqlite3_open(INCAR_COMMON_MICRO_DATABASE_NAME_WITH_DIRECTORY, &pDb);
    if (iRet != SQLITE_OK)
    {
        printf("insertKeyWord, openDB failed %s\n", sqlite3_errmsg(pDb));
        sqlite3_close(pDb);
        return -1;
    }

    KeyWordList::const_iterator keyWordIter = keyWordList.begin();
    for (; keyWordIter != keyWordList.end(); ++keyWordIter)
    {
        char strSqlCmd[2048];
        memset(strSqlCmd, 0, sizeof(strSqlCmd));

        // ƴ��SQL���
        sprintf(strSqlCmd, "insert into KeywordTable values('%s')", keyWordIter->toUtf8().data());
        printf("��������SQL���%s\n", strSqlCmd);

        sqlite3_exec(pDb, strSqlCmd, 0, 0, &strError);
    }

    // ����keywordstatus���Ƿ������ݣ�����������޸����еģ�û�������һ������
    char *strSearchStatusCmd = (char *)"select count(*) from KeywordStatusTable";
    int iRowNum = 0;

    iRet = sqlite3_prepare(pDb, strSearchStatusCmd, strlen(strSearchStatusCmd), &pStmt, (const char**)&strError);
    if (iRet != SQLITE_OK)
    {
        printf("insertKeyWord, sqlite3_prepare failed %d\n", iRet);
        sqlite3_close(pDb);

        return iRet;
    }

    while (sqlite3_step(pStmt) == SQLITE_ROW)
    {
        iRowNum = sqlite3_column_int(pStmt,0);
        sqlite3_finalize(pStmt);
    }

    if (iRowNum > 0)
    {
        // �����ݣ��޸�����Uploadֵ
        char *strSqlCmd = (char *)"update KeywordStatusTable SET StatusValue='false' where StatusName='upload'";
        sqlite3_exec(pDb, strSqlCmd, 0, 0, &strError);
    }
    else
    {
        // û�����ݣ���������
        char *strSqlCmd =(char *)"insert into KeywordStatusTable values('upload','false')";
        sqlite3_exec(pDb, strSqlCmd, 0, 0, &strError);

        strSqlCmd = (char *)"insert into KeywordStatusTable values('id','')";
        sqlite3_exec(pDb, strSqlCmd, 0, 0, &strError);
    }

    sqlite3_finalize(pStmt);
    sqlite3_close(pDb);

    // �ϴ�����������
    _uploadKeyword();
    return iRet;
}

/**
*<FUNC+>********************************************************************
* �������ƣ� modifyKeyWord
* ���������� �޸Ĺؼ���
* ��������� strSourceKeyWord��ԭ���Ĺؼ��ʣ�strObjKeyWord��Ҫ�޸ĵĹؼ���
* ��������� ��
* �� ��	ֵ�� �ɹ�����0�����򷵻ش�����
* ����˵���� 
* �޸ļ�¼�� 
* 		�޸�����        �汾��     �޸���	      �޸�����
* --------------------------------------------------------------------------
* 	[2011/11/14]	    V1.0	   ������		  ��������
*<FUNC+>********************************************************************
*/
int KeyWordsMgrC::modifyKeyWord(const QString strSourceKeyWord, 
                                const QString strObjKeyWord)
{
    return 0;
}

/**
*<FUNC+>********************************************************************
* �������ƣ� getKeyWord
* ���������� ��ȡ�ؼ���
* ��������� ��
* ��������� keywordList���ؼ����б�
* �� ��	ֵ�� �ɹ�����0�����򷵻ش�����
* ����˵���� 
* �޸ļ�¼�� 
* 		�޸�����        �汾��     �޸���	      �޸�����
* --------------------------------------------------------------------------
* 	[2011/11/14]	    V1.0	   ������		  ��������
*<FUNC+>********************************************************************
*/
int KeyWordsMgrC::getKeyWord(KeyWordList &keywordList)
{
    int iRet = 0;
    sqlite3 *pDb = NULL;
    sqlite3_stmt *pStmt = NULL;
    char *strError = NULL;
    keywordList.clear();

    // �����ݿ�
    iRet = sqlite3_open(INCAR_COMMON_MICRO_DATABASE_NAME_WITH_DIRECTORY, &pDb);
    if (iRet != SQLITE_OK)
    {
        printf("getKeyWord, openDB failed %s\n", sqlite3_errmsg(pDb));
        sqlite3_close(pDb);
        return iRet;
    }

    char *strSqlCmd = (char *)"select * from KeywordTable";
    iRet = sqlite3_prepare(pDb, strSqlCmd, strlen(strSqlCmd), &pStmt, (const char**)&strError);
    if (iRet != SQLITE_OK)
    {
        printf("getKeyWord, sqlite3_prepare failed %s\n", sqlite3_errmsg(pDb));
        sqlite3_close(pDb);
        return iRet;
    }

    // ��ȡ����
    while (1)
    {
        iRet = sqlite3_step(pStmt);

        if (iRet != SQLITE_ROW)
        {
            break;
        }

        const unsigned char *strKeyword = sqlite3_column_text(pStmt, 0);

        printf("Get from database %s\n", strKeyword);

        keywordList.append(QString::fromUtf8((const char*)strKeyword));
    }

    sqlite3_finalize(pStmt);
    sqlite3_close(pDb);

    return iRet;
}

/**
*<FUNC+>********************************************************************
* �������ƣ� getKeyWordId
* ���������� ��ȡ�ؼ���ID
* ��������� ��
* ��������� strKeywordId���ؼ���ID
* �� ��	ֵ�� �ɹ�����0�����򷵻ش�����
* ����˵���� 
* �޸ļ�¼�� 
* 		�޸�����        �汾��     �޸���	      �޸�����
* --------------------------------------------------------------------------
* 	[2011/11/22]	    V1.0	   ������		  ��������
*<FUNC+>********************************************************************
*/
int KeyWordsMgrC::getKeyWordId(QString &strKeywordId)
{
    int iRet = 0;
    sqlite3 *pDb = NULL;
    sqlite3_stmt *pStmt = NULL;
    char *strError = NULL;
    strKeywordId.clear();

    // �����ݿ�
    iRet = sqlite3_open(INCAR_COMMON_MICRO_DATABASE_NAME_WITH_DIRECTORY, &pDb);
    if (iRet != SQLITE_OK)
    {
        printf("getKeyWord, openDB failed %s\n", sqlite3_errmsg(pDb));
        sqlite3_close(pDb);
        return iRet;
    }

    // �Ȼ�ȡ״̬�����״̬Ϊtrue�����ID����ȥ
    char *strSqlCmd = (char *)"select StatusValue from KeywordStatusTable where StatusName='upload'";

    // �·�SQL���
    iRet = sqlite3_prepare(pDb, strSqlCmd, strlen(strSqlCmd), &pStmt, (const char**)&strError);
    if (iRet != SQLITE_OK)
    {
        printf("getKeyWord, sqlite3_prepare failed %s\n", sqlite3_errmsg(pDb));
        sqlite3_close(pDb);
        return iRet;
    }

    // ��ȡ����
    iRet = sqlite3_step(pStmt);
    const unsigned char *strStatus = NULL;
    strStatus = sqlite3_column_text(pStmt, 0);

    printf("��ȡupload���%s %d\n", strStatus, iRet);

    // ���û�н����ֱ�ӷ���
    if (iRet != SQLITE_ROW && iRet != SQLITE_DONE)
    {
        printf("getKeyWord, sqlite3_step failed %s\n", sqlite3_errmsg(pDb));
        sqlite3_finalize(pStmt);
        sqlite3_close(pDb);
        return iRet;
    }

    // ֻ��Ϊtrue�Ż᷵��ID�����򲻷���
    if (strcmp((const char*)strStatus, "true") == 0)
    {
        sqlite3_finalize(pStmt);

        strSqlCmd = (char *)"select StatusValue from KeywordStatusTable where StatusName='id'";

        // �·�SQL���
        iRet = sqlite3_prepare(pDb, strSqlCmd, strlen(strSqlCmd), &pStmt, (const char**)&strError);
        if (iRet != SQLITE_OK)
        {
            printf("getKeyWord, sqlite3_prepare failed %s\n", sqlite3_errmsg(pDb));
            sqlite3_close(pDb);
            return iRet;
        }

        // ��ȡID����������
        iRet = sqlite3_step(pStmt);
        const unsigned char *strTempKeywordId = sqlite3_column_text(pStmt, 0);
        strKeywordId = (const char*)strTempKeywordId;
    }

    sqlite3_finalize(pStmt);
    sqlite3_close(pDb);

    return iRet;
}
/**
*<FUNC+>********************************************************************
* �������ƣ� insertKeyWordId
* ���������� �洢�ؼ���ID
* ��������� strKeyWordId���ؼ���ID
* ��������� ��
* �� ��	ֵ�� �ɹ�����0�����򷵻ش�����
* ����˵���� 
* �޸ļ�¼�� 
* 		�޸�����        �汾��     �޸���	      �޸�����
* --------------------------------------------------------------------------
* 	[2011/11/14]	    V1.0	   ������		  ��������
*<FUNC+>********************************************************************
*/
int KeyWordsMgrC::insertKeyWordId(const char *strKeyWordId)
{
    if (strKeyWordId == NULL)
    {
        printf("insertKeyWordId, strKeyWordId is null\n");
        return -1;
    }

    int iRet = 0;
    sqlite3 *pDb = NULL;
    sqlite3_stmt *pStmt = NULL;
    char *strError = NULL;
    char strSqlCmd[2048] = {0};

    sprintf(strSqlCmd, "update KeywordStatusTable SET StatusValue='%s' where StatusName='id'", strKeyWordId);
    printf("����ؼ���ID��SQL=%s\n", strSqlCmd);

    // �����ݿ�
    iRet = sqlite3_open(INCAR_COMMON_MICRO_DATABASE_NAME_WITH_DIRECTORY, &pDb);

    if (iRet != SQLITE_OK)
    {
        printf("insertKeyWordId, openDB failed %s\n", sqlite3_errmsg(pDb));
        sqlite3_close(pDb);
        return iRet;
    }

    // �·�SQL���
    iRet = sqlite3_exec(pDb, strSqlCmd, 0, 0, &strError);
    printf("�洢�ؼ���ID����%d\n", iRet);

    // �޸�״̬
    memset(strSqlCmd, 0, sizeof(strSqlCmd));
    strcpy(strSqlCmd, "update KeywordStatusTable SET StatusValue='true' where StatusName='upload'");
    printf("�޸�״̬��SQL=%s\n", strSqlCmd);

    iRet = sqlite3_exec(pDb, strSqlCmd, 0, 0, &strError);
    printf("�޸�״̬���%d\n", iRet);

    sqlite3_finalize(pStmt);
    sqlite3_close(pDb);

    return iRet;
}

/**
*<FUNC+>********************************************************************
* �������ƣ� _keyWordExist
* ���������� ���ؼ������ݿ��Ƿ����
* ��������� ��
* ��������� ��
* �� ��	ֵ�� ���ڷ���true�����򷵻�false
* ����˵���� 
* �޸ļ�¼�� 
* 		�޸�����        �汾��     �޸���	      �޸�����
* --------------------------------------------------------------------------
* 	[2011/11/14]	    V1.0	   ������		  ��������
*<FUNC+>********************************************************************
*/
bool KeyWordsMgrC::_keyWordExist()
{
    int iRet = 0;
    sqlite3 *pDb = NULL;
    sqlite3_stmt *pStmt = NULL;
    char *strError = NULL;
    char *strSelectTable = (char*)"SELECT name FROM sqlite_master";

    // �����ݿ�
    iRet = sqlite3_open(INCAR_COMMON_MICRO_DATABASE_NAME_WITH_DIRECTORY, &pDb);
    if (iRet != SQLITE_OK)
    {
        printf("_keyWordExist, openDB failed %s\n", sqlite3_errmsg(pDb));
        sqlite3_close(pDb);
        return false;
    }

    // �·�SQL���
    iRet = sqlite3_prepare(pDb, strSelectTable, strlen(strSelectTable), &pStmt, (const char**)&strError);
    if (iRet != SQLITE_OK)
    {
        printf("_keyWordExist, sqlite3_prepare failed %s\n", sqlite3_errmsg(pDb));
        sqlite3_close(pDb);
        return false;
    }

    // ��ȡ����
    while (1)
    {
        iRet = sqlite3_step(pStmt);

        if (iRet != SQLITE_ROW)
        {
            break;
        }

        const unsigned char *strTableName = sqlite3_column_text(pStmt, 0);
        printf("Table name is %s\n", strTableName);

        if (strcmp("KeywordStatusTable", (const char*)strTableName) == 0
            || strcmp("KeywordTable", (const char*)strTableName) == 0)
        {
            sqlite3_finalize(pStmt);
            sqlite3_close(pDb);
            return true;
        }
    }

    sqlite3_finalize(pStmt);
    sqlite3_close(pDb);

    return false;
}

/**
*<FUNC+>********************************************************************
* �������ƣ� _createDatabase
* ���������� �������ݿ��
* ��������� ��
* ��������� ��
* �� ��	ֵ�� �ɹ�����0�����򷵻ش�����
* ����˵���� 
* �޸ļ�¼�� 
* 		�޸�����        �汾��     �޸���	      �޸�����
* --------------------------------------------------------------------------
* 	[2011/11/14]	    V1.0	   ������		  ��������
*<FUNC+>********************************************************************
*/
int KeyWordsMgrC::_createDatabase()
{
    printf("������ʼ\n");
    int iRet = 0;
    sqlite3 *pDb = NULL;
    sqlite3_stmt *pStmt = NULL;
    char *strError = NULL;
    char *strCreateTable = (char*)"create table KeywordTable(Keyword text primary key)";

    // �����ݿ�
    iRet = sqlite3_open(INCAR_COMMON_MICRO_DATABASE_NAME_WITH_DIRECTORY, &pDb);
    if (iRet != SQLITE_OK)
    {
        printf("_createDatabase, openDB failed %s\n", sqlite3_errmsg(pDb));
        sqlite3_close(pDb);
        return iRet;
    }

    // �·�SQL���
    sqlite3_exec(pDb, strCreateTable, 0, 0, &strError);

    // ����keyWordStatusTable��
    strCreateTable = (char *)"create table KeywordStatusTable(StatusName text, StatusValue text)";
    sqlite3_exec(pDb, strCreateTable, 0, 0, &strError);

    sqlite3_close(pDb);

    return iRet;
}

/**
*<FUNC+>********************************************************************
* �������ƣ� _uploadKeyword
* ���������� �ϴ��ؼ��ʵ���������
* ��������� ��
* ��������� ��
* �� ��	ֵ�� �ɹ�����0�����򷵻ش�����
* ����˵���� 
* �޸ļ�¼�� 
* 		�޸�����        �汾��     �޸���	      �޸�����
* --------------------------------------------------------------------------
* 	[2011/11/22]	    V1.0	   ������		  ��������
*<FUNC+>********************************************************************
*/
int KeyWordsMgrC::_uploadKeyword()
{
    int iRet = 0;
    KeyWordList keywordList;
    QString strKeyword;
    const char *strSessionId = NULL;

    // �Ȼ�ȡ���ݿ��еĹؼ���
    iRet = getKeyWord(keywordList);

    if (keywordList.empty())
    {
        printf("_uploadKeyword, keywordList is empty\n");
        return -1;
    }

    // ��ƴ���ַ���Ϊ���¸�ʽ����1,��2,��3��
    KeyWordList::iterator keywordIter = keywordList.begin();
    for (; keywordIter != keywordList.end(); ++keywordIter)
    {
        strKeyword.append(QString::fromUtf8(keywordIter->toUtf8().data()));
        strKeyword.append(",");
    }

    // ȥ�����һ������
    strKeyword.remove(strKeyword.length() - 1, 1);

    // ��ʼ�ϴ��ʿ� ��ʼ��
    iRet = QISRInit("");
    if(iRet != 0)
    {
        printf("QISRInit with errorCode: %d \n", iRet);
        return iRet;
    }

    // �����Ự
    strSessionId = QISRSessionBegin(NULL, "ssm=1,sub=asr", &iRet);
    if (iRet != 0)
    {
        printf("QISRSessionBegin with errorCode: %d \n", iRet);

        QISRFini();
        return iRet;
    }

    // �ϴ�����������
    printf("upload keyword are %s\n", strKeyword.toUtf8().data());
    const char *strUploadId = QISRUploadData(strSessionId, "contact", 
        strKeyword.toUtf8().data(), strKeyword.length(), "dtt=keylist", &iRet);

    printf("upload id is %s\n", strUploadId);
    if (iRet != 0)
    {
        printf("QISRUploadData with errorCode: %d \n", iRet);

        QISRSessionEnd(strSessionId, "normal");
        QISRFini();

        return iRet;
    }

    // �ϴ��ɹ����������ݿ���
    insertKeyWordId(strUploadId);

    QISRSessionEnd(strSessionId, "normal");
    QISRFini();

    return iRet;
}
