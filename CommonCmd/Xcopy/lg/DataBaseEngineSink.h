#ifndef DATABASE_ENGINE_SINK_HEAD_FILE
#define DATABASE_ENGINE_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "InitParameter.h"
#include "DataBasePacket.h"

//////////////////////////////////////////////////////////////////////////////////

#define DB_NO_AVAILABLE_ACCOUNT	1									//û�п����˻�
#define DB_ACCINFO_ALREADYEXIST		7									//�˻��Ѿ�����
#define DB_GUEST_REG_SUCCESS		21									//�ο�ע��ɹ�

#define DB_QUERY_SIGNIN_SUCCESS     22									//ǩ����ѯ�ɹ�
#define DB_QUERY_SIGNIN_FAILURE		23									//ǩ����ѯʧ��
#define DB_USER_SIGNIN_SUCCESS		24									//���ǩ���ɹ�
#define DB_USER_SIGNIN_FAILURE		25									//���ǩ��ʧ��
#define DB_USER_SIGNIN_ALREADY		26									//�û���ǩ��
#define DB_USER_BQ_SUCCESS			27									//��ǩ�ɹ�

#define DB_QUERY_LOTTERY_SUCCESS    30									//�齱��ѯ�ɹ�
#define DB_QUERY_LOTTERY_FAILURE    31									//�齱��ѯʧ��
#define DB_USER_LOTTERY_SUCCESS     32									//�齱�����ɹ�
#define DB_USER_LOTTERY_FAILURE     33									//�齱����ʧ��
#define DB_GET_PRIZEITEM_SUCCESS    34									//�н��ȼ���ȡ�ɹ�
#define DB_GET_PRIZEITEM_FAILURE    35									//�н��ȼ���ȡʧ��
#define DB_GET_RECHARGE_SUCCESS     36									//��ֵ��Ϣ��ȡ�ɹ�
#define DB_GET_RECHARGE_FAILURE     37									//��ֵ��Ϣ��ȡʧ��

//���ݿ���
class CDataBaseEngineSink : public IDataBaseEngineSink
{
	//��Ԫ����
	friend class CServiceUnits;

	//��Ա����
private:
	UINT m_uiRandCodeCnt;												//�������

	//�û����ݿ�
protected:
	CDataBaseAide					m_AccountsDBAide;					//�û����ݿ�
	CDataBaseHelper					m_AccountsDBModule;					//�û����ݿ�

	//��Ϸ�����ݿ�
protected:
	CDataBaseAide					m_TreasureDBAide;					//��Ϸ�����ݿ�
	CDataBaseHelper					m_TreasureDBModule;					//��Ϸ�����ݿ�

	//ƽ̨���ݿ�
protected:
	CDataBaseAide					m_PlatformDBAide;					//ƽ̨���ݿ�
	CDataBaseHelper					m_PlatformDBModule;					//ƽ̨���ݿ�

	//�������
protected:
	CInitParameter *				m_pInitParameter;					//���ò���
	IDataBaseEngineEvent *			m_pIDataBaseEngineEvent;			//�����¼�

	//��������
public:
	//���캯��
	CDataBaseEngineSink();
	//��������
	virtual ~CDataBaseEngineSink();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID Release() { return; }
	//�ӿڲ�ѯ
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//ϵͳ�¼�
public:
	//�����¼�
	virtual bool OnDataBaseEngineStart(IUnknownEx * pIUnknownEx);
	//ֹͣ�¼�
	virtual bool OnDataBaseEngineConclude(IUnknownEx * pIUnknownEx);

	//�ں��¼�
public:
	//ʱ���¼�
	virtual bool OnDataBaseEngineTimer(DWORD dwTimerID, WPARAM dwBindParameter);
	//�����¼�
	virtual bool OnDataBaseEngineControl(WORD wControlID, VOID * pData, WORD wDataSize);
	//�����¼�
	virtual bool OnDataBaseEngineRequest(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);

	//��¼����
protected:
	//I D ��¼
	bool OnRequestLogonGameID(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�ʺŵ�¼
	bool OnRequestLogonAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�ʺ�ע��
	bool OnRequestRegisterAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ȡ�ο���Ϣ
	LONG InsGuestRegInfo(DBR_GP_RegisterAccounts* pGuestRegInfo, CDBVarValue * pDBVarValue);
	//�����ο���Ϣ
	LONG GetGuestRegInfo(DBR_GP_RegisterAccounts* pGuestRegInfo, CDBVarValue * pDBVarValue);
	//�ο͵�¼
	bool OnRequestGuestRegister(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��������˻�
	VOID GenRandAccount(TCHAR szRandAccount[]);
	//��������ַ���
	VOID GenRandString(TCHAR szRandStr[], UINT uLength);

	//�ֻ���¼
protected:
	//I D ��¼
	bool OnMobileLogonGameID(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�ʺŵ�¼
	bool OnMobileLogonAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�ʺ�ע��
	bool OnMobileRegisterAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//�˺ŷ���
protected:
	//�޸Ļ���
	bool OnRequestModifyMachine(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�޸�����
	bool OnRequestModifyLogonPass(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�޸�����
	bool OnRequestModifyInsurePass(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�޸�ǩ��
	bool OnRequestModifyUnderWrite(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�޸�ͷ��
	bool OnRequestModifySystemFace(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�޸�ͷ��
	bool OnRequestModifyCustomFace(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�޸�����
	bool OnRequestModifyIndividual(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//ǩ������
protected:
	bool OnRequestUserSignIn(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//���з���
protected:
	//������Ϸ��
	bool OnRequestUserSaveScore(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ȡ��Ϸ��
	bool OnRequestUserTakeScore(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//ת����Ϸ��
	bool OnRequestUserTransferScore(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//��Ϣ��ѯ
protected:
	//��ѯ����
	bool OnRequestQueryIndividual(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ѯ����
	bool OnRequestQueryInsureInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//������֤
	bool OnRequestUserCheckPassword(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ѯ�û�
	bool OnRequestQueryTransferUserInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ѯǩ��
	bool OnRequestQuerySignInInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�齱��ѯ
	bool OnRequestQueryLotteryInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�齱����
	bool OnRequestUserLtry(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ֵ��ѯ
	bool OnRequestQryRechargeInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//ϵͳ����
protected:
	//�����б�
	bool OnRequestLoadGameList(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//������Ϣ
	bool OnRequestOnLineCountInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//�������
protected:
	//��¼���
	VOID OnLogonDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient);
	//���н��
	VOID OnInsureDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient);
	//�������
	VOID OnOperateDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient);
	//������Ϣ
	VOID OnInsureTransferInfo(DWORD dwContextID, DWORD dwErrorCode);
	//ǩ�����
	VOID OnSignInDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient);
	//�齱���
	VOID OnLotteryDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient);
	//��ֵ���
	VOID OnRechargeDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient);
};

//////////////////////////////////////////////////////////////////////////////////

#endif