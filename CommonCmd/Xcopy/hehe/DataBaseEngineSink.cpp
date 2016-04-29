#include "StdAfx.h"
#include "ServiceUnits.h"
#include "DataBaseEngineSink.h"

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CDataBaseEngineSink::CDataBaseEngineSink()
{
	//��Ա����
	m_uiRandCodeCnt=0;

	//�������
	m_pInitParameter=NULL;
	m_pIDataBaseEngineEvent=NULL;

	return;
}

//��������
CDataBaseEngineSink::~CDataBaseEngineSink()
{
}

//�ӿڲ�ѯ
VOID * CDataBaseEngineSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IDataBaseEngineSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IDataBaseEngineSink,Guid,dwQueryVer);
	return NULL;
}

//�����¼�
bool CDataBaseEngineSink::OnDataBaseEngineStart(IUnknownEx * pIUnknownEx)
{
	//��������
	if ((m_AccountsDBModule.GetInterface()==NULL)&&(m_AccountsDBModule.CreateInstance()==false))
	{
		ASSERT(FALSE);
		return false;
	}

	//��������
	if ((m_TreasureDBModule.GetInterface()==NULL)&&(m_TreasureDBModule.CreateInstance()==false))
	{
		ASSERT(FALSE);
		return false;
	}

	//��������
	if ((m_PlatformDBModule.GetInterface()==NULL)&&(m_PlatformDBModule.CreateInstance()==false))
	{
		ASSERT(FALSE);
		return false;
	}

	try
	{
		//������Ϣ
		tagDataBaseParameter * pAccountsDBParameter=&m_pInitParameter->m_AccountsDBParameter;
		tagDataBaseParameter * pTreasureDBParameter=&m_pInitParameter->m_TreasureDBParameter;
		tagDataBaseParameter * pPlatformDBParameter=&m_pInitParameter->m_PlatformDBParameter;

		//��������
		m_AccountsDBModule->SetConnectionInfo(pAccountsDBParameter->szDataBaseAddr,pAccountsDBParameter->wDataBasePort,
			pAccountsDBParameter->szDataBaseName,pAccountsDBParameter->szDataBaseUser,pAccountsDBParameter->szDataBasePass,
			"��¼������","AccountsDBModule");
		m_TreasureDBModule->SetConnectionInfo(pTreasureDBParameter->szDataBaseAddr,pTreasureDBParameter->wDataBasePort,
			pTreasureDBParameter->szDataBaseName,pTreasureDBParameter->szDataBaseUser,pTreasureDBParameter->szDataBasePass,
			"��¼������","TreasureDBModule");
		m_PlatformDBModule->SetConnectionInfo(pPlatformDBParameter->szDataBaseAddr,pPlatformDBParameter->wDataBasePort,
			pPlatformDBParameter->szDataBaseName,pPlatformDBParameter->szDataBaseUser,pPlatformDBParameter->szDataBasePass,
			"��¼������","PlatformDBModule");

		//��������
		m_AccountsDBModule->OpenConnection();
		m_AccountsDBAide.SetDataBase(m_AccountsDBModule.GetInterface());

		//��������
		m_TreasureDBModule->OpenConnection();
		m_TreasureDBAide.SetDataBase(m_TreasureDBModule.GetInterface());

		//��������
		m_PlatformDBModule->OpenConnection();
		m_PlatformDBAide.SetDataBase(m_PlatformDBModule.GetInterface());

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);

		return false;
	}

	return true;
}

//ֹͣ�¼�
bool CDataBaseEngineSink::OnDataBaseEngineConclude(IUnknownEx * pIUnknownEx)
{
	//���ö���
	m_AccountsDBAide.SetDataBase(NULL);
	m_TreasureDBAide.SetDataBase(NULL);
	m_PlatformDBAide.SetDataBase(NULL);

	//�ر�����
	if (m_AccountsDBModule.GetInterface()!=NULL)
	{
		m_AccountsDBModule->CloseConnection();
		m_AccountsDBModule.CloseInstance();
	}

	//�ر�����
	if (m_TreasureDBModule.GetInterface()!=NULL)
	{
		m_TreasureDBModule->CloseConnection();
		m_TreasureDBModule.CloseInstance();
	}

	//�ر�����
	if (m_PlatformDBModule.GetInterface()!=NULL)
	{
		m_PlatformDBModule->CloseConnection();
		m_PlatformDBModule.CloseInstance();
	}

	//�������
	m_pInitParameter=NULL;
	m_pIDataBaseEngineEvent=NULL;

	return true;
}

//ʱ���¼�
bool CDataBaseEngineSink::OnDataBaseEngineTimer(DWORD dwTimerID, WPARAM dwBindParameter)
{
	return false;
}

//�����¼�
bool CDataBaseEngineSink::OnDataBaseEngineControl(WORD wControlID, VOID * pData, WORD wDataSize)
{
	return false;
}

//�����¼�
bool CDataBaseEngineSink::OnDataBaseEngineRequest(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	switch (wRequestID)
	{
	case DBR_GP_LOGON_GAMEID:			//��ʶ��¼
		{
			return OnRequestLogonGameID(dwContextID,pData,wDataSize);
		}
	case DBR_GP_LOGON_ACCOUNTS:			//�ʺŵ�¼
		{
			return OnRequestLogonAccounts(dwContextID,pData,wDataSize);
		}
	case DBR_GP_REGISTER_ACCOUNTS:		//ע���ʺ�
		{
			return OnRequestRegisterAccounts(dwContextID,pData,wDataSize);
		}
	case DBR_GP_GUEST_REGISTER:			//�ο�ע��
		{
			return OnRequestGuestRegister(dwContextID,pData,wDataSize);
		}
	case DBR_GP_MODIFY_MACHINE:			//�޸Ļ���
		{
			return OnRequestModifyMachine(dwContextID,pData,wDataSize);
		}
	case DBR_GP_MODIFY_LOGON_PASS:		//�޸�����
		{
			return OnRequestModifyLogonPass(dwContextID,pData,wDataSize);
		}
	case DBR_GP_MODIFY_INSURE_PASS:		//�޸�����
		{
			return OnRequestModifyInsurePass(dwContextID,pData,wDataSize);
		}
	case DBR_GP_MODIFY_UNDER_WRITE:		//�޸�ǩ��
		{
			return OnRequestModifyUnderWrite(dwContextID,pData,wDataSize);
		}
	case DBR_GP_MODIFY_SYSTEM_FACE:		//�޸�ͷ��
		{
			return OnRequestModifySystemFace(dwContextID,pData,wDataSize);
		}
	case DBR_GP_MODIFY_CUSTOM_FACE:		//�޸�ͷ��
		{
			return OnRequestModifyCustomFace(dwContextID,pData,wDataSize);
		}
	case DBR_GP_MODIFY_INDIVIDUAL:		//�޸�����
		{
			return OnRequestModifyIndividual(dwContextID,pData,wDataSize);
		}
	case DBR_GP_USER_SAVE_SCORE:		//������Ϸ��
		{
			return OnRequestUserSaveScore(dwContextID,pData,wDataSize);
		}
	case DBR_GP_USER_TAKE_SCORE:		//��ȡ��Ϸ��
		{
			return OnRequestUserTakeScore(dwContextID,pData,wDataSize);
		}
	case DBR_GP_USER_TRANSFER_SCORE:	//ת����Ϸ��
		{
			return OnRequestUserTransferScore(dwContextID,pData,wDataSize);
		}
	case DBR_GP_QUERY_INDIVIDUAL:		//��ѯ����
		{
			return OnRequestQueryIndividual(dwContextID,pData,wDataSize);
		}
	case DBR_GP_QUERY_INSURE_INFO:		//��ѯ����
		{
			return OnRequestQueryInsureInfo(dwContextID,pData,wDataSize);
		}
	case DBR_GP_USER_CHECK_PASSWORD:
		{
			return OnRequestUserCheckPassword(dwContextID,pData,wDataSize);
		}
	case DBR_GP_QUERY_USER_INFO:		//��ѯ�û�
		{
			return OnRequestQueryTransferUserInfo(dwContextID,pData,wDataSize);
		}
	case DBR_GP_LOAD_GAME_LIST:			//�����б�
		{
			return OnRequestLoadGameList(dwContextID,pData,wDataSize);
		}
	case DBR_GP_ONLINE_COUNT_INFO:		//������Ϣ
		{
			return OnRequestOnLineCountInfo(dwContextID,pData,wDataSize);
		}
	case DBR_GP_QUERY_SIGIN_INFO: //ǩ����ѯ 
		{
			return OnRequestQuerySignInInfo(dwContextID,pData,wDataSize);
		}
	case DBR_GP_USER_SIGIN: //ǩ������
		{
			return OnRequestUserSignIn(dwContextID,pData,wDataSize);
		}
	case DBR_GP_QUERY_LOTTERY_INFO:
		{
			return OnRequestQueryLotteryInfo(dwContextID,pData,wDataSize);
		}
	case DBR_GP_LOTTERY_REQ:
		{
			return OnRequestUserLtry(dwContextID,pData,wDataSize);
		}
	case DBR_GP_RECHARGE_INFO:
		{
			return OnRequestQryRechargeInfo(dwContextID,pData,wDataSize);
		}
	case DBR_MB_LOGON_GAMEID:			//��ʶ��¼
		{
			return OnMobileLogonGameID(dwContextID,pData,wDataSize);
		}
	case DBR_MB_LOGON_ACCOUNTS:			//�ʺŵ�¼
		{
			return OnMobileLogonAccounts(dwContextID,pData,wDataSize);
		}
	case DBR_MB_REGISTER_ACCOUNTS:		//ע���ʺ�
		{
			return OnMobileRegisterAccounts(dwContextID,pData,wDataSize);
		}
	}

	return false;
}

//I D ��¼
bool CDataBaseEngineSink::OnRequestLogonGameID(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_LogonGameID));
		if (wDataSize!=sizeof(DBR_GP_LogonGameID)) return false;

		//ִ�в�ѯ
		DBR_GP_LogonGameID * pLogonGameID=(DBR_GP_LogonGameID *)pData;

		//��������
		tagBindParameter * pBindParameter=(tagBindParameter *)pLogonGameID->pBindParameter;
		if (pBindParameter->dwSocketID!=dwContextID) return true;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pLogonGameID->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwGameID"),pLogonGameID->dwGameID);
		m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pLogonGameID->szPassword);
		m_AccountsDBAide.AddParameter(TEXT("@strPassPortID"),pLogonGameID->szPassPortID);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pLogonGameID->szMachineID);
		m_AccountsDBAide.AddParameter(TEXT("@nNeeValidateMBCard"),pLogonGameID->cbNeeValidateMBCard);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_EfficacyGameID"),true);

		//�������
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnLogonDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);
	
		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//������
		OnLogonDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����Ի�ѡ����һ��������¼��"),false);

		return false;
	}

	return true;
}

//�ʺŵ�¼
bool CDataBaseEngineSink::OnRequestLogonAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_LogonAccounts));
		if (wDataSize!=sizeof(DBR_GP_LogonAccounts)) return false;

		//������
		DBR_GP_LogonAccounts * pLogonAccounts=(DBR_GP_LogonAccounts *)pData;

		//ִ���ж�
		tagBindParameter * pBindParameter=(tagBindParameter *)pLogonAccounts->pBindParameter;
		if (pBindParameter->dwSocketID!=dwContextID) return true;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pLogonAccounts->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@strAccounts"),pLogonAccounts->szAccounts);
		m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pLogonAccounts->szPassword);
		m_AccountsDBAide.AddParameter(TEXT("@strPassPortID"),pLogonAccounts->szPassPortID);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pLogonAccounts->szMachineID);
		m_AccountsDBAide.AddParameter(TEXT("@nNeeValidateMBCard"),pLogonAccounts->cbNeeValidateMBCard);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_EfficacyAccounts"),true);

		//�������
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnLogonDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//������
		OnLogonDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����Ի�ѡ����һ��������¼��"),false);

		return false;
	}

	return true;
}

//ע�ᴦ��
bool CDataBaseEngineSink::OnRequestRegisterAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_RegisterAccounts));
		if (wDataSize!=sizeof(DBR_GP_RegisterAccounts)) return false;

		//������
		DBR_GP_RegisterAccounts * pRegisterAccounts=(DBR_GP_RegisterAccounts *)pData;

		//ִ���ж�
		tagBindParameter * pBindParameter=(tagBindParameter *)pRegisterAccounts->pBindParameter;
		if (pBindParameter->dwSocketID!=dwContextID) return true;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pRegisterAccounts->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@strAccounts"),pRegisterAccounts->szAccounts);
		m_AccountsDBAide.AddParameter(TEXT("@strNickName"),pRegisterAccounts->szNickName);
		m_AccountsDBAide.AddParameter(TEXT("@strSpreader"),pRegisterAccounts->szSpreader);
		m_AccountsDBAide.AddParameter(TEXT("@strLogonPass"),pRegisterAccounts->szLogonPass);
		m_AccountsDBAide.AddParameter(TEXT("@strInsurePass"),pRegisterAccounts->szInsurePass);
		m_AccountsDBAide.AddParameter(TEXT("@wFaceID"),pRegisterAccounts->wFaceID);
		m_AccountsDBAide.AddParameter(TEXT("@cbGender"),pRegisterAccounts->cbGender);
		m_AccountsDBAide.AddParameter(TEXT("@strPassPortID"),pRegisterAccounts->szPassPortID);
		m_AccountsDBAide.AddParameter(TEXT("@strCompellation"),pRegisterAccounts->szCompellation);
		m_AccountsDBAide.AddParameter(TEXT("@strMobilePhone"),pRegisterAccounts->szMobilePhone);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pRegisterAccounts->szMachineID);
		m_AccountsDBAide.AddParameter(TEXT("@bIsGuestReg"),0);
		m_AccountsDBAide.AddParameter(TEXT("@AgentID"),pRegisterAccounts->dwAgentID);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_RegisterAccounts"),true);

		//�������
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnLogonDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//������
		OnLogonDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����Ի�ѡ����һ��������¼��"),false);

		return false;
	}

	return true;
}

//I D ��¼
bool CDataBaseEngineSink::OnMobileLogonGameID(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_MB_LogonGameID));
		if (wDataSize!=sizeof(DBR_MB_LogonGameID)) return false;

		//ִ�в�ѯ
		DBR_MB_LogonGameID * pLogonGameID=(DBR_MB_LogonGameID *)pData;

		//��������
		tagBindParameter * pBindParameter=(tagBindParameter *)pLogonGameID->pBindParameter;
		if (pBindParameter->dwSocketID!=dwContextID) return true;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pLogonGameID->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwGameID"),pLogonGameID->dwGameID);
		m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pLogonGameID->szPassword);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pLogonGameID->szMachineID);
		m_AccountsDBAide.AddParameter(TEXT("@strMobilePhone"),pLogonGameID->szMobilePhone);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_MB_EfficacyGameID"),true);

		//�������
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnLogonDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),true);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//������
		OnLogonDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����Ի�ѡ����һ��������¼��"),true);

		return false;
	}

	return true;
}

//�ʺŵ�¼
bool CDataBaseEngineSink::OnMobileLogonAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_MB_LogonAccounts));
		if (wDataSize!=sizeof(DBR_MB_LogonAccounts)) return false;

		//������
		DBR_MB_LogonAccounts * pLogonAccounts=(DBR_MB_LogonAccounts *)pData;

		//ִ���ж�
		tagBindParameter * pBindParameter=(tagBindParameter *)pLogonAccounts->pBindParameter;
		if (pBindParameter->dwSocketID!=dwContextID) return true;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pLogonAccounts->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@strAccounts"),pLogonAccounts->szAccounts);
		m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pLogonAccounts->szPassword);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pLogonAccounts->szMachineID);
		m_AccountsDBAide.AddParameter(TEXT("@strMobilePhone"),pLogonAccounts->szMobilePhone);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_MB_EfficacyAccounts"),true);

		//�������
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnLogonDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),true);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//������
		OnLogonDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����Ի�ѡ����һ��������¼��"),true);

		return false;
	}

	return true;
}

//�����ο���Ϣ
LONG CDataBaseEngineSink::InsGuestRegInfo(DBR_GP_RegisterAccounts* pGuestRegInfo, CDBVarValue * pDBVarValue)
{
	//ת����ַ
	LONG lResultCode;
	TCHAR szEncryCode[LEN_MD5];
	TCHAR szClientAddr[16]=TEXT("");
	BYTE * pClientAddr=(BYTE *)&pGuestRegInfo->dwClientAddr;
	_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);
	CWHEncrypt::MD5Encrypt(pGuestRegInfo->szGuestPass,szEncryCode);

	//�������
	m_AccountsDBAide.ResetParameter();
	m_AccountsDBAide.AddParameter(TEXT("@strAccounts"),pGuestRegInfo->szAccounts);
	m_AccountsDBAide.AddParameter(TEXT("@strNickName"),pGuestRegInfo->szNickName);
	m_AccountsDBAide.AddParameter(TEXT("@strSpreader"),pGuestRegInfo->szSpreader);
	m_AccountsDBAide.AddParameter(TEXT("@strLogonPass"),szEncryCode);
	m_AccountsDBAide.AddParameter(TEXT("@strInsurePass"),szEncryCode);
	m_AccountsDBAide.AddParameter(TEXT("@wFaceID"),pGuestRegInfo->wFaceID);
	m_AccountsDBAide.AddParameter(TEXT("@cbGender"),pGuestRegInfo->cbGender);
	m_AccountsDBAide.AddParameter(TEXT("@strPassPortID"),pGuestRegInfo->szPassPortID);
	m_AccountsDBAide.AddParameter(TEXT("@strCompellation"),pGuestRegInfo->szCompellation);
	m_AccountsDBAide.AddParameter(TEXT("@strMobilePhone"),pGuestRegInfo->szMobilePhone);
	m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
	m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pGuestRegInfo->szMachineID);
	m_AccountsDBAide.AddParameter(TEXT("@bIsGuestReg"),1);
	m_AccountsDBAide.AddParameter(TEXT("@AgentID"),0);

	//�������
	TCHAR szDescribeString[128]=TEXT("");
	m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

	//ִ�в�ѯ
	lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_RegisterAccounts"),true);

	//�������
	CDBVarValue DBVarValue;
	m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"), DBVarValue);
	memcpy(pDBVarValue, &DBVarValue, sizeof(CDBVarValue));

	_bstr_t bsErrString = DBVarValue.bstrVal;
	LPCTSTR lpErrString = bsErrString;

	return lResultCode;
}

//��ȡ�ο���Ϣ
LONG CDataBaseEngineSink::GetGuestRegInfo(DBR_GP_RegisterAccounts* pGuestRegInfo, CDBVarValue * pDBVarValue)
{
	//ת����ַ
	LONG lResultCode;
	TCHAR szClientAddr[16]=TEXT("");
	BYTE * pClientAddr=(BYTE *)&pGuestRegInfo->dwClientAddr;
	_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

	//�������
	m_AccountsDBAide.ResetParameter();
	m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
	m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pGuestRegInfo->szMachineID);
	m_AccountsDBAide.AddParameter(TEXT("@strPassPortID"),pGuestRegInfo->szPassPortID);

	//�������
	TCHAR szDescribeString[128]=TEXT("");
	m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

	//ִ�в�ѯ
	lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_GuestRegister"),true);

	//�������
	CDBVarValue DBVarValue;
	m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"), DBVarValue);
	memcpy(pDBVarValue, &DBVarValue, sizeof(CDBVarValue));

	_bstr_t bsErrString = DBVarValue.bstrVal;
	LPCTSTR lpErrString = bsErrString;

	return lResultCode;
}

//��������˺�
VOID CDataBaseEngineSink::GenRandAccount(TCHAR szRandAccount[])
{
	TCHAR szCharBase[100];
	TCHAR szRandString[10];
	INT iCharNum=0;

	//�������
	m_uiRandCodeCnt++;
	if (m_uiRandCodeCnt>=1000)
	{
		m_uiRandCodeCnt=0;
	}

	lstrcpyn(szCharBase,TEXT("abcdefghijklmnopqrstuvwxyz0123456789"),CountArray(szCharBase));

	for(;iCharNum<9;iCharNum++)
	{
		szRandString[iCharNum]=szCharBase[rand()%36];
	}
	szRandString[iCharNum]='\0';

	_sntprintf(szRandAccount,LEN_ACCOUNTS,TEXT("TY_%s%03u"),szRandString,m_uiRandCodeCnt);

	return;
}

//��������ַ���
VOID CDataBaseEngineSink::GenRandString(TCHAR szRandStr[], UINT uLength)
{
	if (uLength >= 100)
	{
		return;
	}

	TCHAR szCharBase[100];
	TCHAR szRandString[100];
	INT iCharNum=0;

	lstrcpyn(szCharBase,TEXT("abcdefghijklmnopqrstuvwxyz0123456789"),CountArray(szCharBase));

	for(;iCharNum<uLength;iCharNum++)
	{
		szRandString[iCharNum]=szCharBase[rand()%36];
	}
	szRandString[iCharNum]='\0';

	lstrcpyn(szRandStr,szRandString,CountArray(szRandString));

	return;
}

//�ο͵�¼
bool CDataBaseEngineSink::OnRequestGuestRegister( DWORD dwContextID, VOID * pData, WORD wDataSize )
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_RegisterAccounts));
		if (wDataSize!=sizeof(DBR_GP_RegisterAccounts)) return false;

		//������
		DBR_GP_RegisterAccounts * pGuestRegInfo=(DBR_GP_RegisterAccounts *)pData;

		//ִ���ж�
		tagBindParameter * pBindParameter=(tagBindParameter *)pGuestRegInfo->pBindParameter;
		if (pBindParameter->dwSocketID!=dwContextID) return true;

		//�������
		CDBVarValue DBVarValue;
		LONG lResultCode=DB_ERROR;
		lResultCode=GetGuestRegInfo(pGuestRegInfo, &DBVarValue);

		if (lResultCode == DB_NO_AVAILABLE_ACCOUNT)
		{
			do
			{
				GenRandAccount(pGuestRegInfo->szAccounts);
				lstrcpyn(pGuestRegInfo->szNickName,pGuestRegInfo->szAccounts,CountArray(pGuestRegInfo->szAccounts));
				GenRandString(pGuestRegInfo->szGuestPass, 6);
				lResultCode=InsGuestRegInfo(pGuestRegInfo, &DBVarValue);
			}	while(lResultCode==DB_ACCINFO_ALREADYEXIST);
		}

		if (lResultCode == DB_GUEST_REG_SUCCESS)
		{
			OnLogonDisposeResult(dwContextID,lResultCode,pGuestRegInfo->szGuestPass,false);
		}
		else
		{
			OnLogonDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);
		}
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//������
		OnLogonDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����Ի�ѡ����һ��������¼��"),false);

		return false;
	}

	return true;
}

//�ʺ�ע��
bool CDataBaseEngineSink::OnMobileRegisterAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_MB_RegisterAccounts));
		if (wDataSize!=sizeof(DBR_MB_RegisterAccounts)) return false;

		//������
		DBR_MB_RegisterAccounts * pRegisterAccounts=(DBR_MB_RegisterAccounts *)pData;

		//ִ���ж�
		tagBindParameter * pBindParameter=(tagBindParameter *)pRegisterAccounts->pBindParameter;
		if (pBindParameter->dwSocketID!=dwContextID) return true;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pRegisterAccounts->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@strAccounts"),pRegisterAccounts->szAccounts);
		m_AccountsDBAide.AddParameter(TEXT("@strNickName"),pRegisterAccounts->szNickName);
		m_AccountsDBAide.AddParameter(TEXT("@strLogonPass"),pRegisterAccounts->szLogonPass);
		m_AccountsDBAide.AddParameter(TEXT("@strInsurePass"),pRegisterAccounts->szInsurePass);
		m_AccountsDBAide.AddParameter(TEXT("@wFaceID"),pRegisterAccounts->wFaceID);
		m_AccountsDBAide.AddParameter(TEXT("@cbGender"),pRegisterAccounts->cbGender);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pRegisterAccounts->szMachineID);
		m_AccountsDBAide.AddParameter(TEXT("@strMobilePhone"),pRegisterAccounts->szMobilePhone);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_MB_RegisterAccounts"),true);

		//�������
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnLogonDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),true);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//������
		OnLogonDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����Ի�ѡ����һ��������¼��"),true);

		return false;
	}

	return true;
}

//�޸Ļ���
bool CDataBaseEngineSink::OnRequestModifyMachine(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_ModifyMachine));
		if (wDataSize!=sizeof(DBR_GP_ModifyMachine)) return false;

		//������
		DBR_GP_ModifyMachine * pModifyMachine=(DBR_GP_ModifyMachine *)pData;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pModifyMachine->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pModifyMachine->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pModifyMachine->szPassword);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pModifyMachine->szMachineID);

		//�������
		WCHAR szDescribe[128]=L"";
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribe,sizeof(szDescribe),adParamOutput);

		//�󶨲���
		if (pModifyMachine->cbBind==TRUE)
		{
			m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_MoorMachine"),false);
		}
		else
		{
			m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_UnMoorMachine"),false);
		}

		//�������
		if (m_AccountsDBAide.GetReturnValue()==DB_SUCCESS)
		{
			//��������
			DBO_GP_OperateSuccess OperateSuccess;
			ZeroMemory(&OperateSuccess,sizeof(OperateSuccess));

			//��ȡ��Ϣ
			CDBVarValue DBVarValue;
			m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

			//�������
			OperateSuccess.lResultCode=m_AccountsDBModule->GetReturnValue();
			lstrcpyn(OperateSuccess.szDescribeString,CW2CT(DBVarValue.bstrVal),CountArray(OperateSuccess.szDescribeString));

			//���ͽ��
			WORD wStringSize=CountStringBuffer(OperateSuccess.szDescribeString);
			WORD wSendSize=sizeof(OperateSuccess)-sizeof(OperateSuccess.szDescribeString)+wStringSize;
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_OPERATE_SUCCESS,dwContextID,&OperateSuccess,wSendSize);
		}
		else
		{
			//��������
			DBO_GP_OperateFailure OperateFailure;
			ZeroMemory(&OperateFailure,sizeof(OperateFailure));

			//��ȡ��Ϣ
			CDBVarValue DBVarValue;
			m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

			//�������
			OperateFailure.lResultCode=m_AccountsDBModule->GetReturnValue();
			lstrcpyn(OperateFailure.szDescribeString,CW2CT(DBVarValue.bstrVal),CountArray(OperateFailure.szDescribeString));

			//���ͽ��
			WORD wStringSize=CountStringBuffer(OperateFailure.szDescribeString);
			WORD wSendSize=sizeof(OperateFailure)-sizeof(OperateFailure.szDescribeString)+wStringSize;
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_OPERATE_FAILURE,dwContextID,&OperateFailure,wSendSize);
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//�������
		OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

		return false;
	}

	return true;
}

//�޸�ͷ��
bool CDataBaseEngineSink::OnRequestModifySystemFace(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_ModifySystemFace));
		if (wDataSize!=sizeof(DBR_GP_ModifySystemFace)) return false;

		//������
		DBR_GP_ModifySystemFace * pModifySystemFace=(DBR_GP_ModifySystemFace *)pData;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pModifySystemFace->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pModifySystemFace->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pModifySystemFace->szPassword);
		m_AccountsDBAide.AddParameter(TEXT("@wFaceID"),pModifySystemFace->wFaceID);

		//������Ϣ
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pModifySystemFace->szMachineID);

		//�������
		WCHAR szDescribe[128]=L"";
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribe,sizeof(szDescribe),adParamOutput);

		//�������
		if (m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_SystemFaceInsert"),true)==DB_SUCCESS)
		{
			//��������
			DBO_GP_UserFaceInfo UserFaceInfo;
			ZeroMemory(&UserFaceInfo,sizeof(UserFaceInfo));

			//��ȡ��Ϣ
			UserFaceInfo.wFaceID=m_AccountsDBAide.GetValue_WORD(TEXT("FaceID"));

			//���ͽ��
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_USER_FACE_INFO,dwContextID,&UserFaceInfo,sizeof(UserFaceInfo));
		}
		else
		{
			//��ȡ����
			CDBVarValue DBVarValue;
			m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

			//�������
			OnOperateDisposeResult(dwContextID,m_AccountsDBAide.GetReturnValue(),CW2CT(DBVarValue.bstrVal),false);
		}
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//�������
		OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

		return false;
	}

	return true;
}

//�޸�ͷ��
bool CDataBaseEngineSink::OnRequestModifyCustomFace(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_ModifyCustomFace));
		if (wDataSize!=sizeof(DBR_GP_ModifyCustomFace)) return false;

		//������
		DBR_GP_ModifyCustomFace * pModifyCustomFace=(DBR_GP_ModifyCustomFace *)pData;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pModifyCustomFace->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//��������
		SAFEARRAYBOUND SafeArrayBound;
		ZeroMemory(&SafeArrayBound,sizeof(SafeArrayBound));

		//���ñ���
		SafeArrayBound.lLbound=0L;
		SafeArrayBound.cElements=sizeof(pModifyCustomFace->dwCustomFace);

		//��������
		BYTE * pcbCustomFace=(BYTE *)pModifyCustomFace->dwCustomFace;
		SAFEARRAY * pSafeArray=SafeArrayCreate(VT_UI1,1,&SafeArrayBound);

		//��������
		for (LONG nIndex=0;nIndex<sizeof(pModifyCustomFace->dwCustomFace);nIndex++)
		{
			SafeArrayPutElement(pSafeArray,&nIndex,pcbCustomFace+nIndex);
		}

		//���ñ���
		VARIANT VarChunk;
		VariantClear(&VarChunk);
		VarChunk.parray=pSafeArray;
		VarChunk.vt=VT_ARRAY|VT_UI1;

		//��ȡ����
		ASSERT(m_AccountsDBModule.GetInterface()!=NULL);
		IDataBase * pIDataBase=m_AccountsDBModule.GetInterface();

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pModifyCustomFace->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pModifyCustomFace->szPassword);

		//ͷ����Ϣ
		pIDataBase->AddParameter(TEXT("@dwCustomFace"),adLongVarBinary,adParamInput,sizeof(pModifyCustomFace->dwCustomFace),CDBVarValue(VarChunk));

		//������Ϣ
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pModifyCustomFace->szMachineID);

		//�������
		WCHAR szDescribe[128]=L"";
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribe,sizeof(szDescribe),adParamOutput);

		//�ͷ���Դ
		SafeArrayDestroy(pSafeArray);

		//�������
		if (m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_CustomFaceInsert"),true)==DB_SUCCESS)
		{
			//��������
			DBO_GP_UserFaceInfo UserFaceInfo;
			ZeroMemory(&UserFaceInfo,sizeof(UserFaceInfo));

			//��ȡ��Ϣ
			UserFaceInfo.wFaceID=INVALID_WORD;
			UserFaceInfo.dwCustomID=m_AccountsDBAide.GetValue_DWORD(TEXT("CustomID"));

			//���ͽ��
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_USER_FACE_INFO,dwContextID,&UserFaceInfo,sizeof(UserFaceInfo));
		}
		else
		{
			//��ȡ����
			CDBVarValue DBVarValue;
			m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

			//�������
			OnOperateDisposeResult(dwContextID,m_AccountsDBAide.GetReturnValue(),CW2CT(DBVarValue.bstrVal),false);
		}
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//�������
		OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

		return false;
	}

	return true;
}

//�޸�����
bool CDataBaseEngineSink::OnRequestModifyLogonPass(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_ModifyLogonPass));
		if (wDataSize!=sizeof(DBR_GP_ModifyLogonPass)) return false;

		//������
		DBR_GP_ModifyLogonPass * pModifyLogonPass=(DBR_GP_ModifyLogonPass *)pData;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pModifyLogonPass->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pModifyLogonPass->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@strScrPassword"),pModifyLogonPass->szScrPassword);
		m_AccountsDBAide.AddParameter(TEXT("@strDesPassword"),pModifyLogonPass->szDesPassword);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);

		//�������
		WCHAR szDescribe[128]=L"";
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribe,sizeof(szDescribe),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_ModifyLogonPassword"),false);

		//�������
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnOperateDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//�������
		OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

		return false;
	}

	return true;
}

//�޸�����
bool CDataBaseEngineSink::OnRequestModifyInsurePass(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_ModifyInsurePass));
		if (wDataSize!=sizeof(DBR_GP_ModifyInsurePass)) return false;

		//������
		DBR_GP_ModifyInsurePass * pModifyInsurePass=(DBR_GP_ModifyInsurePass *)pData;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pModifyInsurePass->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pModifyInsurePass->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@strScrPassword"),pModifyInsurePass->szScrPassword);
		m_AccountsDBAide.AddParameter(TEXT("@strDesPassword"),pModifyInsurePass->szDesPassword);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);

		//�������
		WCHAR szDescribe[128]=L"";
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribe,sizeof(szDescribe),adParamOutput);

		//�������
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_ModifyInsurePassword"),false);

		//�������
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnOperateDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//�������
		OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

		return false;
	}

	return true;
}

//�޸�ǩ��
bool CDataBaseEngineSink::OnRequestModifyUnderWrite(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_ModifyUnderWrite));
		if (wDataSize!=sizeof(DBR_GP_ModifyUnderWrite)) return false;

		//������
		DBR_GP_ModifyUnderWrite * pModifyUnderWrite=(DBR_GP_ModifyUnderWrite *)pData;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pModifyUnderWrite->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pModifyUnderWrite->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pModifyUnderWrite->szPassword);
		m_AccountsDBAide.AddParameter(TEXT("@strUnderWrite"),pModifyUnderWrite->szUnderWrite);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);

		//�������
		WCHAR szDescribe[128]=L"";
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribe,sizeof(szDescribe),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_ModifyUnderWrite"),false);

		//�������
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnOperateDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//�������
		OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

		return false;
	}

	return true;
}

//�޸�����
bool CDataBaseEngineSink::OnRequestModifyIndividual(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_ModifyIndividual));
		if (wDataSize!=sizeof(DBR_GP_ModifyIndividual)) return false;

		//������
		DBR_GP_ModifyIndividual * pModifyIndividual=(DBR_GP_ModifyIndividual *)pData;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pModifyIndividual->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pModifyIndividual->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pModifyIndividual->szPassword);
		m_AccountsDBAide.AddParameter(TEXT("@cbGender"),pModifyIndividual->cbGender);
		m_AccountsDBAide.AddParameter(TEXT("@strNickName"),pModifyIndividual->szNickName);
		m_AccountsDBAide.AddParameter(TEXT("@strUnderWrite"),pModifyIndividual->szUnderWrite);
		m_AccountsDBAide.AddParameter(TEXT("@strCompellation"),pModifyIndividual->szCompellation);
		m_AccountsDBAide.AddParameter(TEXT("@strQQ"),pModifyIndividual->szQQ);
		m_AccountsDBAide.AddParameter(TEXT("@strEMail"),pModifyIndividual->szEMail);
		m_AccountsDBAide.AddParameter(TEXT("@strSeatPhone"),pModifyIndividual->szSeatPhone);
		m_AccountsDBAide.AddParameter(TEXT("@strMobilePhone"),pModifyIndividual->szMobilePhone);
		m_AccountsDBAide.AddParameter(TEXT("@strDwellingPlace"),pModifyIndividual->szDwellingPlace);
		m_AccountsDBAide.AddParameter(TEXT("@strUserNote"),pModifyIndividual->szUserNote);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);

		//�������
		WCHAR szDescribe[128]=L"";
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribe,sizeof(szDescribe),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_ModifyUserIndividual"),false);

		//�������
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnOperateDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//�������
		OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

		return false;
	}

	return true;
}

//������Ϸ��
bool CDataBaseEngineSink::OnRequestUserSaveScore(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_UserSaveScore));
		if (wDataSize!=sizeof(DBR_GP_UserSaveScore)) return false;

		//������
		DBR_GP_UserSaveScore * pUserSaveScore=(DBR_GP_UserSaveScore *)pData;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pUserSaveScore->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@dwUserID"),pUserSaveScore->dwUserID);
		m_TreasureDBAide.AddParameter(TEXT("@lSaveScore"),pUserSaveScore->lSaveScore);
		m_TreasureDBAide.AddParameter(TEXT("@wKindID"),0L);
		m_TreasureDBAide.AddParameter(TEXT("@wServerID"),0L);
		m_TreasureDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_TreasureDBAide.AddParameter(TEXT("@strMachineID"),pUserSaveScore->szMachineID);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_TreasureDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode=m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GR_UserSaveScore"),true);

		//�������
		CDBVarValue DBVarValue;
		m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnInsureDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);
	
		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//������
		OnInsureDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

		return false;
	}

	return true;
}

//��ȡ��Ϸ��
bool CDataBaseEngineSink::OnRequestUserTakeScore(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_UserTakeScore));
		if (wDataSize!=sizeof(DBR_GP_UserTakeScore)) return false;

		//������
		DBR_GP_UserTakeScore * pUserTakeScore=(DBR_GP_UserTakeScore *)pData;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pUserTakeScore->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@dwUserID"),pUserTakeScore->dwUserID);
		m_TreasureDBAide.AddParameter(TEXT("@lTakeScore"),pUserTakeScore->lTakeScore);
		m_TreasureDBAide.AddParameter(TEXT("@strPassword"),pUserTakeScore->szPassword);
		m_TreasureDBAide.AddParameter(TEXT("@wKindID"),0L);
		m_TreasureDBAide.AddParameter(TEXT("@wServerID"),0L);
		m_TreasureDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_TreasureDBAide.AddParameter(TEXT("@strMachineID"),pUserTakeScore->szMachineID);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_TreasureDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode=m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GR_UserTakeScore"),true);

		//�������
		CDBVarValue DBVarValue;
		m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnInsureDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);
	
		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//������
		OnInsureDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

		return false;
	}

	return true;
}

//ת����Ϸ��
bool CDataBaseEngineSink::OnRequestUserTransferScore(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_UserTransferScore));
		if (wDataSize!=sizeof(DBR_GP_UserTransferScore)) return false;

		//������
		DBR_GP_UserTransferScore * pUserTransferScore=(DBR_GP_UserTransferScore *)pData;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pUserTransferScore->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@dwUserID"),pUserTransferScore->dwUserID);
		m_TreasureDBAide.AddParameter(TEXT("@cbByNickName"),pUserTransferScore->cbByNickName);
		m_TreasureDBAide.AddParameter(TEXT("@lTransferScore"),pUserTransferScore->lTransferScore);
		m_TreasureDBAide.AddParameter(TEXT("@strPassword"),pUserTransferScore->szPassword);
		m_TreasureDBAide.AddParameter(TEXT("@strNickName"),pUserTransferScore->szNickName);
		m_TreasureDBAide.AddParameter(TEXT("@wKindID"),0L);
		m_TreasureDBAide.AddParameter(TEXT("@wServerID"),0L);
		m_TreasureDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_TreasureDBAide.AddParameter(TEXT("@strMachineID"),pUserTransferScore->szMachineID);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_TreasureDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode=m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GR_UserTransferScore"),true);

		//�������
		CDBVarValue DBVarValue;
		m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		//OnInsureDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);
		OnInsureTransferInfo(dwContextID,lResultCode);
		OnInsureDisposeResult(dwContextID,lResultCode,TEXT(""),false);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//������
		OnInsureDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

		return false;
	}

	return true;
}

//��ѯ����
bool CDataBaseEngineSink::OnRequestQueryIndividual(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_QueryIndividual));
		if (wDataSize!=sizeof(DBR_GP_QueryIndividual)) return false;

		//������
		DBR_GP_QueryIndividual * pQueryIndividual=(DBR_GP_QueryIndividual *)pData;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pQueryIndividual->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pQueryIndividual->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);

		//�������
		if (m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_QueryUserIndividual"),true)==DB_SUCCESS)
		{
			//��������
			DBO_GP_UserIndividual UserIndividual;
			ZeroMemory(&UserIndividual,sizeof(UserIndividual));

			//�û���Ϣ
			UserIndividual.dwUserID=m_AccountsDBAide.GetValue_DWORD(TEXT("UserID"));
			m_AccountsDBAide.GetValue_String(TEXT("UserNote"),UserIndividual.szUserNote,CountArray(UserIndividual.szUserNote));
			m_AccountsDBAide.GetValue_String(TEXT("Compellation"),UserIndividual.szCompellation,CountArray(UserIndividual.szCompellation));

			//�绰����
			m_AccountsDBAide.GetValue_String(TEXT("SeatPhone"),UserIndividual.szSeatPhone,CountArray(UserIndividual.szSeatPhone));
			m_AccountsDBAide.GetValue_String(TEXT("MobilePhone"),UserIndividual.szMobilePhone,CountArray(UserIndividual.szMobilePhone));

			//��ϵ����
			m_AccountsDBAide.GetValue_String(TEXT("QQ"),UserIndividual.szQQ,CountArray(UserIndividual.szQQ));
			m_AccountsDBAide.GetValue_String(TEXT("EMail"),UserIndividual.szEMail,CountArray(UserIndividual.szEMail));
			m_AccountsDBAide.GetValue_String(TEXT("DwellingPlace"),UserIndividual.szDwellingPlace,CountArray(UserIndividual.szDwellingPlace));

			//���ͽ��
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_USER_INDIVIDUAL,dwContextID,&UserIndividual,sizeof(UserIndividual));
		}
		else
		{
			//��ȡ����
			CDBVarValue DBVarValue;
			m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

			//������
			OnOperateDisposeResult(dwContextID,m_AccountsDBAide.GetReturnValue(),CW2CT(DBVarValue.bstrVal),false);
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//�������
		OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

		return false;
	}

	return true;
}

//��ѯ����
bool CDataBaseEngineSink::OnRequestQueryInsureInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_QueryInsureInfo));
		if (wDataSize!=sizeof(DBR_GP_QueryInsureInfo)) return false;

		//������
		DBR_GP_QueryInsureInfo * pQueryInsureInfo=(DBR_GP_QueryInsureInfo *)pData;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pQueryInsureInfo->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@dwUserID"),pQueryInsureInfo->dwUserID);
		m_TreasureDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_TreasureDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//�������
		if (m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GR_QueryUserInsureInfo"),true)==DB_SUCCESS)
		{
			//��������
			DBO_GP_UserInsureInfo UserInsureInfo;
			ZeroMemory(&UserInsureInfo,sizeof(UserInsureInfo));

			//������Ϣ
			UserInsureInfo.wRevenueTake=m_TreasureDBAide.GetValue_WORD(TEXT("RevenueTake"));
			UserInsureInfo.wRevenueTransfer=m_TreasureDBAide.GetValue_WORD(TEXT("RevenueTransfer"));
			UserInsureInfo.wServerID=m_TreasureDBAide.GetValue_WORD(TEXT("ServerID"));
			UserInsureInfo.lUserScore=m_TreasureDBAide.GetValue_LONGLONG(TEXT("Score"));
			UserInsureInfo.lUserInsure=m_TreasureDBAide.GetValue_LONGLONG(TEXT("Insure"));
			UserInsureInfo.lTransferPrerequisite=m_TreasureDBAide.GetValue_LONGLONG(TEXT("TransferPrerequisite"));

			//���ͽ��
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_USER_INSURE_INFO,dwContextID,&UserInsureInfo,sizeof(UserInsureInfo));
		}
		else
		{
			//��ȡ����
			CDBVarValue DBVarValue;
			m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

			//������
			OnOperateDisposeResult(dwContextID,m_TreasureDBAide.GetReturnValue(),CW2CT(DBVarValue.bstrVal),false);
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//�������
		OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

		return false;
	}

	return true;
}

//������֤
bool CDataBaseEngineSink::OnRequestUserCheckPassword(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(CMD_GP_CheckPassword));
		if (wDataSize!=sizeof(CMD_GP_CheckPassword)) return false;

		//������
		CMD_GP_CheckPassword * pCheckPassword=(CMD_GP_CheckPassword *)pData;

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pCheckPassword->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@strInsurePassword"),pCheckPassword->szPassword);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//�������
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_UserCheckPassword"),true);

		//��������
		CMD_GP_CheckPasswordReturn CheckPasswordReturn={0};
		if (lResultCode==0)
		{
			CheckPasswordReturn.bPasswordRight=true;
		}
		else
		{
			CheckPasswordReturn.bPasswordRight=false;
		}

		//���ͽ��
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_USER_CHECK_PASSWORD,dwContextID,&CheckPasswordReturn,sizeof(CheckPasswordReturn));

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//�������
		OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

		return false;
	}

	return true;
}

//��ѯ�û�
bool CDataBaseEngineSink::OnRequestQueryTransferUserInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(DBR_GP_QueryInsureUserInfo));
	if (wDataSize!=sizeof(DBR_GP_QueryInsureUserInfo)) return false;

	//������
	DBR_GP_QueryInsureUserInfo * pQueryTransferUserInfo=(DBR_GP_QueryInsureUserInfo *)pData;

	try
	{
		//�������
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@cbByNickName"),pQueryTransferUserInfo->cbByNickName);
		m_TreasureDBAide.AddParameter(TEXT("@strNickName"),pQueryTransferUserInfo->szNickName);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_TreasureDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//�������
		if (m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GR_QueryTransferUserInfo"),true)==DB_SUCCESS)
		{
			//��������
			DBO_GP_UserTransferUserInfo TransferUserInfo;
			ZeroMemory(&TransferUserInfo,sizeof(TransferUserInfo));

			//������Ϣ
			TransferUserInfo.dwGameID=m_TreasureDBAide.GetValue_DWORD(TEXT("GameID"));
			m_TreasureDBAide.GetValue_String(TEXT("NickName"), TransferUserInfo.szNickName, CountArray(TransferUserInfo.szNickName));

			//���ͽ��
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_USER_INSURE_USER_INFO,dwContextID,&TransferUserInfo,sizeof(TransferUserInfo));
		}
		else
		{
			//��ȡ����
			CDBVarValue DBVarValue;
			m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

			//������
			OnInsureDisposeResult(dwContextID,m_TreasureDBAide.GetReturnValue(),CW2CT(DBVarValue.bstrVal),false);
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//�������
		OnInsureDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

		return false;
	}
}

//��ѯǩ��
bool CDataBaseEngineSink::OnRequestQuerySignInInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(DBR_GP_QuerySignInInfo));
	if (wDataSize!=sizeof(DBR_GP_QuerySignInInfo)) return false;

	//������
	DBR_GP_QuerySignInInfo * pQuerySignInInfo=(DBR_GP_QuerySignInInfo *)pData;

	try
	{
		//�������
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@UserID"),pQuerySignInInfo->dwUserID);
		m_TreasureDBAide.AddParameter(TEXT("@BqNum"),0);
		m_TreasureDBAide.AddParameter(TEXT("@Flag"),0);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_TreasureDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode=m_TreasureDBAide.ExecuteProcess(TEXT("NET_PW_Sign"),true);

		//�������
		CDBVarValue DBVarValue;
		m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnSignInDisposeResult(dwContextID,m_TreasureDBAide.GetReturnValue(),CW2CT(DBVarValue.bstrVal),false);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//�������
		OnSignInDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

		return false;
	}
}

//ǩ������
bool CDataBaseEngineSink::OnRequestUserSignIn( DWORD dwContextID, VOID * pData, WORD wDataSize )
{
	//Ч�����
	ASSERT(wDataSize==sizeof(DBR_GP_UserSignIn));
	if (wDataSize!=sizeof(DBR_GP_UserSignIn)) return false;

	//������
	DBR_GP_UserSignIn * pUserSignIn=(DBR_GP_UserSignIn *)pData;

	try
	{
		//�������
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@UserID"),pUserSignIn->dwUserID);
		m_TreasureDBAide.AddParameter(TEXT("@BqNum"),pUserSignIn->wBQDay);
		m_TreasureDBAide.AddParameter(TEXT("@Flag"),1);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_TreasureDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode=m_TreasureDBAide.ExecuteProcess(TEXT("NET_PW_Sign"),true);

		//�������
		CDBVarValue DBVarValue;
		m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnSignInDisposeResult(dwContextID,m_TreasureDBAide.GetReturnValue(),CW2CT(DBVarValue.bstrVal),false);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//�������
		OnSignInDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

		return false;
	}
}

//�齱��ѯ
bool CDataBaseEngineSink::OnRequestQueryLotteryInfo( DWORD dwContextID, VOID * pData, WORD wDataSize )
{
	//Ч�����
	ASSERT(wDataSize==sizeof(DBR_GP_QryLtryInfo));
	if (wDataSize!=sizeof(DBR_GP_QryLtryInfo)) return false;

	//������
	DBR_GP_QryLtryInfo * pQryLtryInfo=(DBR_GP_QryLtryInfo *)pData;

	try
	{
		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@UserID"),pQryLtryInfo->dwUserID);

		//ִ�в�ѯ
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("PROC_GET_PRIZE_UserInfo"),true);

		//�������
		OnLotteryDisposeResult(dwContextID,m_AccountsDBAide.GetReturnValue(),TEXT("�齱��ѯ�ɹ�"),false);
	}
	catch (IDataBaseException* pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//�������
		OnLotteryDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);
	}

	return true;
}

//�齱����
bool CDataBaseEngineSink::OnRequestUserLtry( DWORD dwContextID, VOID * pData, WORD wDataSize )
{
	//Ч�����
	ASSERT(wDataSize==sizeof(DBR_GP_UserLtry));
	if (wDataSize!=sizeof(DBR_GP_UserLtry)) return false;

	//������
	DBR_GP_UserLtry * pUserLtry=(DBR_GP_UserLtry *)pData;

	//�����н�����
	INT iPrizeItem;

	try
	{
		//���������
		UINT uRandNum=rand()%10000;

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@UserID"),pUserLtry->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@UserID"),uRandNum);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//��ȡ�н�����
		printf(TEXT("GSP_GP_GetPrizeItemNum\n"));
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_GetPrizeItemNum"),true);

		if (lResultCode == DB_GET_PRIZEITEM_SUCCESS)
		{
			iPrizeItem=m_AccountsDBAide.GetValue_DWORD(TEXT("PrizeItemID"));

			printf(TEXT("%d"), iPrizeItem);
			//��ȡ��ǰʱ��
			SYSTEMTIME systime; 
			GetLocalTime(&systime);
			TCHAR szTimeNow[100];
			_sntprintf(szTimeNow, 100, TEXT("%4d-%02d-%02d %02d:%02d:%02d.%03d"),systime.wYear,systime.wMonth,systime.wDay,systime.wHour,systime.wMinute, systime.wSecond,systime.wMilliseconds);

			//�н���Ϣ����
			m_AccountsDBAide.ResetParameter();
			m_AccountsDBAide.AddParameter(TEXT("@UserID"),pUserLtry->dwUserID);
			m_AccountsDBAide.AddParameter(TEXT("@PrizeItemID"),iPrizeItem);
			m_AccountsDBAide.AddParameter(TEXT("@PrizeTime"),szTimeNow);

			printf(TEXT("PROC_SAVE_PrizeLog\n"));
			lResultCode = m_AccountsDBAide.ExecuteProcess(TEXT("PROC_SAVE_PrizeLog"),true);

			//�������
			m_AccountsDBAide.GetValue_String(TEXT("ErrorText"),szDescribeString,CountArray(szDescribeString));
		}
		else 
		{
			//�������
			CDBVarValue DBVarValue;
			m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
			lstrcpyn(szDescribeString,CW2CT(DBVarValue.bstrVal),CountArray(szDescribeString));
			lResultCode = DB_GET_PRIZEITEM_FAILURE;
		}

		OnLotteryDisposeResult(dwContextID,lResultCode,szDescribeString,false);
	}
	catch (IDataBaseException* pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//�������
		OnLotteryDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);
	}

	return true;
}

//�����б�
bool CDataBaseEngineSink::OnRequestLoadGameList(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//��������
		WORD wPacketSize=0;
		BYTE cbBuffer[MAX_ASYNCHRONISM_DATA];

		//��������
		m_PlatformDBAide.ResetParameter();
		m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GP_LoadGameTypeItem"),true);

		//��������
		wPacketSize=0;
		DBO_GP_GameType * pGameType=NULL;
		while (m_PlatformDBModule->IsRecordsetEnd()==false)
		{
			//������Ϣ
			if ((wPacketSize+sizeof(DBO_GP_GameType))>sizeof(cbBuffer))
			{
				m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_TYPE_ITEM,dwContextID,cbBuffer,wPacketSize);
				wPacketSize=0;
			}

			//��ȡ��Ϣ
			pGameType=(DBO_GP_GameType *)(cbBuffer+wPacketSize);
			pGameType->wSortID=m_PlatformDBAide.GetValue_WORD(TEXT("SortID"));
			pGameType->wJoinID=m_PlatformDBAide.GetValue_WORD(TEXT("JoinID"));
			pGameType->wTypeID=m_PlatformDBAide.GetValue_WORD(TEXT("TypeID"));
			m_PlatformDBAide.GetValue_String(TEXT("TypeName"),pGameType->szTypeName,CountArray(pGameType->szTypeName));

			//����λ��
			wPacketSize+=sizeof(DBO_GP_GameType);

			//�ƶ���¼
			m_PlatformDBModule->MoveToNext();
		}
		if (wPacketSize>0) m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_TYPE_ITEM,dwContextID,cbBuffer,wPacketSize);

		//��ȡ����
		m_PlatformDBAide.ResetParameter();
		m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GP_LoadGameKindItem"),true);

		//��������
		wPacketSize=0;
		DBO_GP_GameKind * pGameKind=NULL;
		while (m_PlatformDBModule->IsRecordsetEnd()==false)
		{
			//������Ϣ
			if ((wPacketSize+sizeof(DBO_GP_GameKind))>sizeof(cbBuffer))
			{
				m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_KIND_ITEM,dwContextID,cbBuffer,wPacketSize);
				wPacketSize=0;
			}

			//��ȡ��Ϣ
			pGameKind=(DBO_GP_GameKind *)(cbBuffer+wPacketSize);
			pGameKind->wSortID=m_PlatformDBAide.GetValue_WORD(TEXT("SortID"));
			pGameKind->wTypeID=m_PlatformDBAide.GetValue_WORD(TEXT("TypeID"));
			pGameKind->wJoinID=m_PlatformDBAide.GetValue_WORD(TEXT("JoinID"));
			pGameKind->wKindID=m_PlatformDBAide.GetValue_WORD(TEXT("KindID"));
			pGameKind->wGameID=m_PlatformDBAide.GetValue_WORD(TEXT("GameID"));
			m_PlatformDBAide.GetValue_String(TEXT("KindName"),pGameKind->szKindName,CountArray(pGameKind->szKindName));
			m_PlatformDBAide.GetValue_String(TEXT("ProcessName"),pGameKind->szProcessName,CountArray(pGameKind->szProcessName));

			//����λ��
			wPacketSize+=sizeof(DBO_GP_GameKind);

			//�ƶ���¼
			m_PlatformDBModule->MoveToNext();
		}
		if (wPacketSize>0) m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_KIND_ITEM,dwContextID,cbBuffer,wPacketSize);

		//��ȡ�ڵ�
		m_PlatformDBAide.ResetParameter();
		m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GP_LoadGameNodeItem"),true);

		//���ͽڵ�
		wPacketSize=0;
		DBO_GP_GameNode * pGameNode=NULL;
		while (m_PlatformDBModule->IsRecordsetEnd()==false)
		{
			//������Ϣ
			if ((wPacketSize+sizeof(DBO_GP_GameNode))>sizeof(cbBuffer))
			{
				m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_NODE_ITEM,dwContextID,cbBuffer,wPacketSize);
				wPacketSize=0;
			}

			//��ȡ��Ϣ
			pGameNode=(DBO_GP_GameNode *)(cbBuffer+wPacketSize);
			pGameNode->wSortID=m_PlatformDBAide.GetValue_WORD(TEXT("SortID"));
			pGameNode->wKindID=m_PlatformDBAide.GetValue_WORD(TEXT("KindID"));
			pGameNode->wJoinID=m_PlatformDBAide.GetValue_WORD(TEXT("JoinID"));
			pGameNode->wNodeID=m_PlatformDBAide.GetValue_WORD(TEXT("NodeID"));
			m_PlatformDBAide.GetValue_String(TEXT("NodeName"),pGameNode->szNodeName,CountArray(pGameNode->szNodeName));
			
			//����λ��
			wPacketSize+=sizeof(DBO_GP_GameNode);

			//�ƶ���¼
			m_PlatformDBModule->MoveToNext();
		}
		if (wPacketSize>0) m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_NODE_ITEM,dwContextID,cbBuffer,wPacketSize);

		//��ȡ����
		m_PlatformDBAide.ResetParameter();
		m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GP_LoadGamePageItem"),true);

		//���Ͷ���
		wPacketSize=0;
		DBO_GP_GamePage * pGamePage=NULL;
		while (m_PlatformDBModule->IsRecordsetEnd()==false)
		{
			//������Ϣ
			if ((wPacketSize+sizeof(DBO_GP_GamePage))>sizeof(cbBuffer))
			{
				m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_PAGE_ITEM,dwContextID,cbBuffer,wPacketSize);
				wPacketSize=0;
			}

			//��ȡ��Ϣ
			pGamePage=(DBO_GP_GamePage *)(cbBuffer+wPacketSize);
			pGamePage->wKindID=m_PlatformDBAide.GetValue_WORD(TEXT("KindID"));
			pGamePage->wNodeID=m_PlatformDBAide.GetValue_WORD(TEXT("NodeID"));
			pGamePage->wSortID=m_PlatformDBAide.GetValue_WORD(TEXT("SortID"));
			pGamePage->wPageID=m_PlatformDBAide.GetValue_WORD(TEXT("PageID"));
			pGamePage->wOperateType=m_PlatformDBAide.GetValue_WORD(TEXT("OperateType"));
			m_PlatformDBAide.GetValue_String(TEXT("DisplayName"),pGamePage->szDisplayName,CountArray(pGamePage->szDisplayName));
			
			//����λ��
			wPacketSize+=sizeof(DBO_GP_GamePage);

			//�ƶ���¼
			m_PlatformDBModule->MoveToNext();
		}
		if (wPacketSize>0) m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_PAGE_ITEM,dwContextID,cbBuffer,wPacketSize);

		//��������
		DBO_GP_GameListResult GameListResult;
		ZeroMemory(&GameListResult,sizeof(GameListResult));

		//���ñ���
		GameListResult.cbSuccess=TRUE;

		//������Ϣ
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_LIST_RESULT,dwContextID,&GameListResult,sizeof(GameListResult));

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);

		//��������
		DBO_GP_GameListResult GameListResult;
		ZeroMemory(&GameListResult,sizeof(GameListResult));

		//���ñ���
		GameListResult.cbSuccess=FALSE;

		//������Ϣ
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_LIST_RESULT,dwContextID,&GameListResult,sizeof(GameListResult));

		return false;
	}

	return true;
}

//������Ϣ
bool CDataBaseEngineSink::OnRequestOnLineCountInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//��������
		DBR_GP_OnLineCountInfo * pOnLineCountInfo=(DBR_GP_OnLineCountInfo *)pData;
		WORD wHeadSize=(sizeof(DBR_GP_OnLineCountInfo)-sizeof(pOnLineCountInfo->OnLineCountKind));

		//Ч������
		ASSERT((wDataSize>=wHeadSize)&&(wDataSize==(wHeadSize+pOnLineCountInfo->wKindCount*sizeof(tagOnLineInfoKind))));
		if ((wDataSize<wHeadSize)||(wDataSize!=(wHeadSize+pOnLineCountInfo->wKindCount*sizeof(tagOnLineInfoKind)))) return false;

		//������ʶ
		TCHAR szMachineID[LEN_MACHINE_ID];
		CWHService::GetMachineID(szMachineID);

		//������Ϣ
		TCHAR szOnLineCountKind[2048]=TEXT("");
		for (WORD i=0;i<pOnLineCountInfo->wKindCount;i++)
		{
			INT nLength=lstrlen(szOnLineCountKind);
			_sntprintf(&szOnLineCountKind[nLength],CountArray(szOnLineCountKind)-nLength,TEXT("%d:%ld;"),pOnLineCountInfo->OnLineCountKind[i].wKindID,
				pOnLineCountInfo->OnLineCountKind[i].dwOnLineCount);
		}

		//�������
		m_PlatformDBAide.ResetParameter();
		m_PlatformDBAide.AddParameter(TEXT("@strMachineID"),szMachineID);
		m_PlatformDBAide.AddParameter(TEXT("@strMachineServer"),m_pInitParameter->m_szServerName);
		m_PlatformDBAide.AddParameter(TEXT("@dwOnLineCountSum"),pOnLineCountInfo->dwOnLineCountSum);
		m_PlatformDBAide.AddParameter(TEXT("@strOnLineCountKind"),szOnLineCountKind);

		//ִ������
		m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GP_OnLineCountInfo"),false);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		return false;
	}

	return true;
}

//��¼�ɹ�
VOID CDataBaseEngineSink::OnLogonDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient)
{
	if (bMobileClient==false)
	{
		if (dwErrorCode==DB_SUCCESS)
		{
			//��������
			DBO_GP_LogonSuccess LogonSuccess;
			ZeroMemory(&LogonSuccess,sizeof(LogonSuccess));

			//��������
			LogonSuccess.wFaceID=m_AccountsDBAide.GetValue_WORD(TEXT("FaceID"));
			LogonSuccess.dwUserID=m_AccountsDBAide.GetValue_DWORD(TEXT("UserID"));
			LogonSuccess.dwGameID=m_AccountsDBAide.GetValue_DWORD(TEXT("GameID"));
			LogonSuccess.dwCustomID=m_AccountsDBAide.GetValue_DWORD(TEXT("CustomID"));
			LogonSuccess.dwUserMedal=m_AccountsDBAide.GetValue_DWORD(TEXT("UserMedal"));
			LogonSuccess.dwExperience=m_AccountsDBAide.GetValue_DWORD(TEXT("Experience"));
			LogonSuccess.dwLoveLiness=m_AccountsDBAide.GetValue_DWORD(TEXT("LoveLiness"));
			m_AccountsDBAide.GetValue_String(TEXT("Accounts"),LogonSuccess.szAccounts,CountArray(LogonSuccess.szAccounts));
			m_AccountsDBAide.GetValue_String(TEXT("NickName"),LogonSuccess.szNickName,CountArray(LogonSuccess.szNickName));

			//�û��ɼ�
			LogonSuccess.lUserScore=m_AccountsDBAide.GetValue_LONGLONG(TEXT("Score"));
			LogonSuccess.lUserInsure=m_AccountsDBAide.GetValue_LONGLONG(TEXT("Insure"));

			//�û�����
			LogonSuccess.cbGender=m_AccountsDBAide.GetValue_BYTE(TEXT("Gender"));
			LogonSuccess.cbMoorMachine=m_AccountsDBAide.GetValue_BYTE(TEXT("MoorMachine"));
			m_AccountsDBAide.GetValue_String(TEXT("UnderWrite"),LogonSuccess.szUnderWrite,CountArray(LogonSuccess.szUnderWrite));

			//��Ա����
			LogonSuccess.cbMemberOrder=m_AccountsDBAide.GetValue_BYTE(TEXT("MemberOrder"));
			m_AccountsDBAide.GetValue_SystemTime(TEXT("MemberOverDate"),LogonSuccess.MemberOverDate);

			//��ֵ��Ϣ
			LogonSuccess.cbIsRecharge=m_AccountsDBAide.GetValue_BYTE("cbIsRecharge");

			//��ȡ��Ϣ
			lstrcpyn(LogonSuccess.szDescribeString,pszErrorString,CountArray(LogonSuccess.szDescribeString));

			//���ͽ��
			WORD wDataSize=CountStringBuffer(LogonSuccess.szDescribeString);
			WORD wHeadSize=sizeof(LogonSuccess)-sizeof(LogonSuccess.szDescribeString);
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_LOGON_SUCCESS,dwContextID,&LogonSuccess,wHeadSize+wDataSize);
		}
		else if(dwErrorCode == DB_GUEST_REG_SUCCESS)
		{
			//��������
			DBO_GP_LogonSuccess LogonSuccess;
			ZeroMemory(&LogonSuccess,sizeof(LogonSuccess));

			//��������
			LogonSuccess.wFaceID=m_AccountsDBAide.GetValue_WORD(TEXT("FaceID"));
			LogonSuccess.dwUserID=m_AccountsDBAide.GetValue_DWORD(TEXT("UserID"));
			LogonSuccess.dwGameID=m_AccountsDBAide.GetValue_DWORD(TEXT("GameID"));
			LogonSuccess.dwCustomID=m_AccountsDBAide.GetValue_DWORD(TEXT("CustomID"));
			LogonSuccess.dwUserMedal=m_AccountsDBAide.GetValue_DWORD(TEXT("UserMedal"));
			LogonSuccess.dwExperience=m_AccountsDBAide.GetValue_DWORD(TEXT("Experience"));
			LogonSuccess.dwLoveLiness=m_AccountsDBAide.GetValue_DWORD(TEXT("LoveLiness"));
			m_AccountsDBAide.GetValue_String(TEXT("Accounts"),LogonSuccess.szAccounts,CountArray(LogonSuccess.szAccounts));
			m_AccountsDBAide.GetValue_String(TEXT("NickName"),LogonSuccess.szNickName,CountArray(LogonSuccess.szNickName));
			lstrcpyn(LogonSuccess.szPassword, pszErrorString, CountArray(LogonSuccess.szPassword));

			//�û��ɼ�
			LogonSuccess.lUserScore=m_AccountsDBAide.GetValue_LONGLONG(TEXT("Score"));
			LogonSuccess.lUserInsure=m_AccountsDBAide.GetValue_LONGLONG(TEXT("Insure"));

			//�û�����
			LogonSuccess.cbGender=m_AccountsDBAide.GetValue_BYTE(TEXT("Gender"));
			LogonSuccess.cbMoorMachine=m_AccountsDBAide.GetValue_BYTE(TEXT("MoorMachine"));
			m_AccountsDBAide.GetValue_String(TEXT("UnderWrite"),LogonSuccess.szUnderWrite,CountArray(LogonSuccess.szUnderWrite));

			//��Ա����
			LogonSuccess.cbMemberOrder=m_AccountsDBAide.GetValue_BYTE(TEXT("MemberOrder"));
			m_AccountsDBAide.GetValue_SystemTime(TEXT("MemberOverDate"),LogonSuccess.MemberOverDate);

			//��ȡ��Ϣ
			lstrcpyn(LogonSuccess.szDescribeString,"",CountArray(LogonSuccess.szDescribeString));

			//���ͽ��
			WORD wDataSize=CountStringBuffer(LogonSuccess.szDescribeString);
			WORD wHeadSize=sizeof(LogonSuccess)-sizeof(LogonSuccess.szDescribeString);
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GUEST_LOGON_SUCCESS,dwContextID,&LogonSuccess,wHeadSize+wDataSize);
		}
		else if(dwErrorCode == DB_NEEDMB)
		{
			//��¼�ɹ�
			DBR_GP_ValidateMBCard ValidateMBCard;
			ZeroMemory(&ValidateMBCard,sizeof(ValidateMBCard));

			//��ȡ����
			ValidateMBCard.uMBCardID=m_AccountsDBAide.GetValue_UINT(TEXT("PasswordID"));

			//Ͷ�ݽ��
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_VALIDATE_MBCARD,dwContextID,&ValidateMBCard,sizeof(ValidateMBCard));
		}
		else if(dwErrorCode == DB_VALIDATE_PASSPOSTID)
		{
			//Ͷ�ݽ��
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_VALIDATE_PASSPOSTID,dwContextID,NULL,0);
		}
		else
		{
			//��������
			DBO_GP_LogonFailure LogonFailure;
			ZeroMemory(&LogonFailure,sizeof(LogonFailure));

			//��������
			LogonFailure.lResultCode=dwErrorCode;
			lstrcpyn(LogonFailure.szDescribeString,pszErrorString,CountArray(LogonFailure.szDescribeString));

			//���ͽ��
			WORD wDataSize=CountStringBuffer(LogonFailure.szDescribeString);
			WORD wHeadSize=sizeof(LogonFailure)-sizeof(LogonFailure.szDescribeString);
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_LOGON_FAILURE,dwContextID,&LogonFailure,wHeadSize+wDataSize);
		}
	}
	else
	{
		if (dwErrorCode==DB_SUCCESS)
		{
			//��������
			DBO_MB_LogonSuccess LogonSuccess;
			ZeroMemory(&LogonSuccess,sizeof(LogonSuccess));

			//��������
			LogonSuccess.wFaceID=m_AccountsDBAide.GetValue_WORD(TEXT("FaceID"));
			LogonSuccess.cbGender=m_AccountsDBAide.GetValue_BYTE(TEXT("Gender"));
			LogonSuccess.dwUserID=m_AccountsDBAide.GetValue_DWORD(TEXT("UserID"));
			LogonSuccess.dwGameID=m_AccountsDBAide.GetValue_DWORD(TEXT("GameID"));
			LogonSuccess.dwExperience=m_AccountsDBAide.GetValue_DWORD(TEXT("Experience"));
			LogonSuccess.dwLoveLiness=m_AccountsDBAide.GetValue_DWORD(TEXT("LoveLiness"));
			m_AccountsDBAide.GetValue_String(TEXT("NickName"),LogonSuccess.szNickName,CountArray(LogonSuccess.szNickName));

			//��ȡ��Ϣ
			lstrcpyn(LogonSuccess.szDescribeString,pszErrorString,CountArray(LogonSuccess.szDescribeString));

			//���ͽ��
			WORD wDataSize=CountStringBuffer(LogonSuccess.szDescribeString);
			WORD wHeadSize=sizeof(LogonSuccess)-sizeof(LogonSuccess.szDescribeString);
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_MB_LOGON_SUCCESS,dwContextID,&LogonSuccess,wHeadSize+wDataSize);
		}
		else
		{
			//��������
			DBO_MB_LogonFailure LogonFailure;
			ZeroMemory(&LogonFailure,sizeof(LogonFailure));

			//��������
			LogonFailure.lResultCode=dwErrorCode;
			lstrcpyn(LogonFailure.szDescribeString,pszErrorString,CountArray(LogonFailure.szDescribeString));

			//���ͽ��
			WORD wDataSize=CountStringBuffer(LogonFailure.szDescribeString);
			WORD wHeadSize=sizeof(LogonFailure)-sizeof(LogonFailure.szDescribeString);
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_MB_LOGON_FAILURE,dwContextID,&LogonFailure,wHeadSize+wDataSize);
		}
	}

	return;
}

//������Ϣ
VOID CDataBaseEngineSink::OnInsureTransferInfo(DWORD dwContextID, DWORD dwErrorCode)
{
	if (dwErrorCode==DB_SUCCESS)
	{
		DBO_GP_UserInsureTransferInfo UserInsureTransferInfo={0};
		UserInsureTransferInfo.dwRecordID=m_TreasureDBAide.GetValue_DWORD(TEXT("RecordID"));
		UserInsureTransferInfo.dwSourceID=m_TreasureDBAide.GetValue_DWORD(TEXT("SourceGameID"));
		UserInsureTransferInfo.dwTargetID=m_TreasureDBAide.GetValue_DWORD(TEXT("TargetGameID"));
		UserInsureTransferInfo.lSwapScore=m_TreasureDBAide.GetValue_LONGLONG(TEXT("TransferScore"));
		m_TreasureDBAide.GetValue_String(TEXT("SourceNickName"),UserInsureTransferInfo.szSourceName,CountArray(UserInsureTransferInfo.szSourceName));
		m_TreasureDBAide.GetValue_String(TEXT("TargetNickName"),UserInsureTransferInfo.szTargetName,CountArray(UserInsureTransferInfo.szTargetName));
		m_TreasureDBAide.GetValue_SystemTime(TEXT("TransferTime"),UserInsureTransferInfo.timeTransfer);

		WORD wDataSize=sizeof(DBO_GP_UserInsureTransferInfo);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_USER_INSURE_TRANSFER_INFO,dwContextID,&UserInsureTransferInfo,wDataSize);
	}
}

//���н��
VOID CDataBaseEngineSink::OnInsureDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient)
{
	if (dwErrorCode==DB_SUCCESS)
	{
		//��������
		DBO_GP_UserInsureSuccess UserInsureSuccess;
		ZeroMemory(&UserInsureSuccess,sizeof(UserInsureSuccess));

		//�������
		UserInsureSuccess.dwUserID=m_TreasureDBAide.GetValue_DWORD(TEXT("UserID"));
		UserInsureSuccess.lSourceScore=m_TreasureDBAide.GetValue_LONGLONG(TEXT("SourceScore"));
		UserInsureSuccess.lSourceInsure=m_TreasureDBAide.GetValue_LONGLONG(TEXT("SourceInsure"));
		UserInsureSuccess.lInsureRevenue=m_TreasureDBAide.GetValue_LONGLONG(TEXT("InsureRevenue"));
		UserInsureSuccess.lVariationScore=m_TreasureDBAide.GetValue_LONGLONG(TEXT("VariationScore"));
		UserInsureSuccess.lVariationInsure=m_TreasureDBAide.GetValue_LONGLONG(TEXT("VariationInsure"));
		lstrcpyn(UserInsureSuccess.szDescribeString,pszErrorString,CountArray(UserInsureSuccess.szDescribeString));

		//���ͽ��
		WORD wDataSize=CountStringBuffer(UserInsureSuccess.szDescribeString);
		WORD wHeadSize=sizeof(UserInsureSuccess)-sizeof(UserInsureSuccess.szDescribeString);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_USER_INSURE_SUCCESS,dwContextID,&UserInsureSuccess,wHeadSize+wDataSize);
	}
	else
	{
		//��������
		DBO_GP_UserInsureFailure UserInsureFailure;
		ZeroMemory(&UserInsureFailure,sizeof(UserInsureFailure));

		//�������
		UserInsureFailure.lResultCode=dwErrorCode;
		lstrcpyn(UserInsureFailure.szDescribeString,pszErrorString,CountArray(UserInsureFailure.szDescribeString));

		//���ͽ��
		WORD wDataSize=CountStringBuffer(UserInsureFailure.szDescribeString);
		WORD wHeadSize=sizeof(UserInsureFailure)-sizeof(UserInsureFailure.szDescribeString);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_USER_INSURE_FAILURE,dwContextID,&UserInsureFailure,wHeadSize+wDataSize);
	}

	return;
}

//�������
VOID CDataBaseEngineSink::OnOperateDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient)
{
	if (dwErrorCode==DB_SUCCESS)
	{
		//��������
		DBO_GP_OperateSuccess OperateSuccess;
		ZeroMemory(&OperateSuccess,sizeof(OperateSuccess));

		//�������
		OperateSuccess.lResultCode=dwErrorCode;
		lstrcpyn(OperateSuccess.szDescribeString,pszErrorString,CountArray(OperateSuccess.szDescribeString));

		//���ͽ��
		WORD wDataSize=CountStringBuffer(OperateSuccess.szDescribeString);
		WORD wHeadSize=sizeof(OperateSuccess)-sizeof(OperateSuccess.szDescribeString);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_OPERATE_SUCCESS,dwContextID,&OperateSuccess,wHeadSize+wDataSize);
	}
	else
	{
		//��������
		DBO_GP_OperateFailure OperateFailure;
		ZeroMemory(&OperateFailure,sizeof(OperateFailure));

		//�������
		OperateFailure.lResultCode=dwErrorCode;
		lstrcpyn(OperateFailure.szDescribeString,pszErrorString,CountArray(OperateFailure.szDescribeString));

		//���ͽ��
		WORD wDataSize=CountStringBuffer(OperateFailure.szDescribeString);
		WORD wHeadSize=sizeof(OperateFailure)-sizeof(OperateFailure.szDescribeString);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_OPERATE_FAILURE,dwContextID,&OperateFailure,wHeadSize+wDataSize);
	}

	return;
}

//ǩ�����
VOID CDataBaseEngineSink::OnSignInDisposeResult( DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient )
{
	if (dwErrorCode==DB_QUERY_SIGNIN_SUCCESS)
	{
		//��������
		DBO_GP_UserSignInInfo SignInQrySuccess;
		ZeroMemory(&SignInQrySuccess,sizeof(SignInQrySuccess));

		//�������
		SignInQrySuccess.wSignInDay = m_TreasureDBAide.GetValue_WORD(TEXT("SignToday"));
		SignInQrySuccess.llBQCost = m_TreasureDBAide.GetValue_LONGLONG(TEXT("BqScore"));
		SignInQrySuccess.llExtraPrize = m_TreasureDBAide.GetValue_LONGLONG(TEXT("AllDay"));

		UINT uDay = 0;
		for (; uDay <= 6 ; uDay++)
		{
			TCHAR szDayCount[20];
			TCHAR szDayNum[20];
			TCHAR szDayPrize[20];

			switch(uDay)
			{
				case 0:
					{
						lstrcpyn(szDayCount,TEXT("One"),CountArray(szDayCount));
						break;
					}
				case 1:
					{
						lstrcpyn(szDayCount,TEXT("Two"),CountArray(szDayCount));
						break;
					}
				case 2:
					{
						lstrcpyn(szDayCount,TEXT("Three"),CountArray(szDayCount));
						break;
					}
				case 3:
					{
						lstrcpyn(szDayCount,TEXT("Four"),CountArray(szDayCount));
						break;
					}
				case 4:
					{
						lstrcpyn(szDayCount,TEXT("Five"),CountArray(szDayCount));
						break;
					}
				case 5:
					{
						lstrcpyn(szDayCount,TEXT("Six"),CountArray(szDayCount));
						break;
					}
				case 6:
					{
						lstrcpyn(szDayCount,TEXT("Seven"),CountArray(szDayCount));
						break;
					}
			}

			_sntprintf(szDayNum,CountArray(szDayNum),TEXT("Day%u"),uDay+1);
			_sntprintf(szDayPrize,CountArray(szDayPrize),TEXT("%sDay"),szDayCount);

			SignInQrySuccess.SignInDay[uDay].wDayNum = m_TreasureDBAide.GetValue_UINT(szDayNum);
			SignInQrySuccess.SignInDay[uDay].llPrize = m_TreasureDBAide.GetValue_LONGLONG(szDayPrize);
		}
		
		//��ȡ��Ϣ
		lstrcpyn(SignInQrySuccess.szDescribeString,pszErrorString,CountArray(SignInQrySuccess.szDescribeString));

		//���ͽ��
		WORD wDataSize=CountStringBuffer(SignInQrySuccess.szDescribeString);
		WORD wHeadSize=sizeof(SignInQrySuccess)-sizeof(SignInQrySuccess.szDescribeString);

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_SIGNIN_QUERY_SUCCESS,dwContextID,&SignInQrySuccess,wHeadSize+wDataSize);
	}

	else if (dwErrorCode==DB_QUERY_SIGNIN_FAILURE)
	{
		//m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_SIGNIN_QUERY_FAILURE,dwContextID,&SignInFailure,wHeadSize+wDataSize);
	}

	//���ǩ���ɹ�
	else if (dwErrorCode==DB_USER_SIGNIN_SUCCESS)
	{
		//��������
		DBO_GP_UserSignInResult SignInSuccess;
		ZeroMemory(&SignInSuccess,sizeof(SignInSuccess));

		//�������
		SignInSuccess.bIsSuccess = true;
		SignInSuccess.bIsAllDaySigned = m_TreasureDBAide.GetValue_DWORD(TEXT("IsAllDaySigned"));

		//��ȡ��Ϣ
		lstrcpyn(SignInSuccess.szDescribeString,pszErrorString,CountArray(SignInSuccess.szDescribeString));

		//���ͽ��
		WORD wDataSize=CountStringBuffer(SignInSuccess.szDescribeString);
		WORD wHeadSize=sizeof(SignInSuccess)-sizeof(SignInSuccess.szDescribeString);

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_USER_SIGNIN_SUCCESS,dwContextID,&SignInSuccess,wHeadSize+wDataSize);
	}

	//���ǩ��ʧ��
	else if (dwErrorCode==DB_USER_SIGNIN_FAILURE)
	{
		//��������
		DBO_GP_UserSignInResult SignInFailure;
		ZeroMemory(&SignInFailure,sizeof(SignInFailure));

		//�������
		SignInFailure.bIsSuccess = false;
		SignInFailure.bIsAllDaySigned = 0;

		//��ȡ��Ϣ
		lstrcpyn(SignInFailure.szDescribeString,pszErrorString,CountArray(SignInFailure.szDescribeString));

		//���ͽ��
		WORD wDataSize=CountStringBuffer(SignInFailure.szDescribeString);
		WORD wHeadSize=sizeof(SignInFailure)-sizeof(SignInFailure.szDescribeString);

		printf("DBO_GP_USER_SIGNIN_FAILURE\n");
		printf("%s", SignInFailure.szDescribeString);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_USER_SIGNIN_FAILURE,dwContextID,&SignInFailure,wHeadSize+wDataSize);
	}

	else
	{
		ASSERT(false);
	}
	
}

//�齱���
VOID CDataBaseEngineSink::OnLotteryDisposeResult( DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient )
{
	if (dwErrorCode==DB_QUERY_LOTTERY_SUCCESS)
	{
		//��������
		DBO_GP_Ltry_Qry_Res LtryQryRes;
		ZeroMemory(&LtryQryRes,sizeof(DBO_GP_Ltry_Qry_Res));

		//�������
		LtryQryRes.wFreeCount = m_AccountsDBAide.GetValue_WORD(TEXT("ALLCOUNT"));

		//��ȡ��Ϣ
		lstrcpyn(LtryQryRes.szDescribeString,pszErrorString,CountArray(LtryQryRes.szDescribeString));

		//���ͽ��
		WORD wDataSize=CountStringBuffer(LtryQryRes.szDescribeString);
		WORD wHeadSize=sizeof(LtryQryRes)-sizeof(LtryQryRes.szDescribeString);

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_LOTTERY_QUERY_RESULT,dwContextID,&LtryQryRes,wHeadSize+wDataSize);
	}
	else if (dwErrorCode==DB_QUERY_LOTTERY_FAILURE)
	{
		return;
	}
	else if (dwErrorCode==DB_USER_LOTTERY_SUCCESS)
	{
		//��������
		DBO_GP_User_Ltry_Res UserLtryRes;
		ZeroMemory(&UserLtryRes,sizeof(DBO_GP_User_Ltry_Res));

		//��ȡ��Ϣ
		UserLtryRes.wIsLtrySuc=true;
		UserLtryRes.wPrizeID=m_AccountsDBAide.GetValue_WORD(TEXT("PrizeItemID"));
		UserLtryRes.wFreeCount=m_AccountsDBAide.GetValue_WORD(TEXT("ALLCOUNT"));
		UserLtryRes.lUserScore=m_AccountsDBAide.GetValue_LONGLONG(TEXT("Score"));
		m_AccountsDBAide.GetValue_String(TEXT("NickName"),UserLtryRes.szNickName,CountArray(UserLtryRes.szNickName,));
		lstrcpyn(UserLtryRes.szDescribeString,pszErrorString,CountArray(UserLtryRes.szDescribeString));

		//���ͽ��
		WORD wDataSize=CountStringBuffer(UserLtryRes.szDescribeString);
		WORD wHeadSize=sizeof(UserLtryRes)-sizeof(UserLtryRes.szDescribeString);

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_USER_LOTTERY_RESULT,dwContextID,&UserLtryRes,wHeadSize+wDataSize);
	}
	else if ((dwErrorCode==DB_USER_LOTTERY_FAILURE) || (dwErrorCode==DB_GET_PRIZEITEM_FAILURE))
	{
		//��������
		DBO_GP_User_Ltry_Res UserLtryRes;
		ZeroMemory(&UserLtryRes,sizeof(DBO_GP_User_Ltry_Res));

		//��ȡ��Ϣ
		UserLtryRes.wIsLtrySuc=false;
		UserLtryRes.wPrizeID=-1;
		UserLtryRes.wFreeCount=-1;
		lstrcpyn(UserLtryRes.szDescribeString, pszErrorString,CountArray(UserLtryRes.szDescribeString));

		//���ͽ��
		WORD wDataSize=CountStringBuffer(UserLtryRes.szDescribeString);
		WORD wHeadSize=sizeof(UserLtryRes)-sizeof(UserLtryRes.szDescribeString);

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_USER_LOTTERY_RESULT,dwContextID,&UserLtryRes,wHeadSize+wDataSize);
	}

	else
	{
		ASSERT(false);
	}
}

bool CDataBaseEngineSink::OnRequestQryRechargeInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(DBR_GP_RechargeInfo));
	if (wDataSize!=sizeof(DBR_GP_RechargeInfo)) return false;

	//������
	DBR_GP_RechargeInfo * pRechargeInfo=(DBR_GP_RechargeInfo *)pData;

	try
	{
		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@UserID"),pRechargeInfo->dwUserID);

		//ִ�в�ѯ
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("PROC_GET_RechargeInfo"),true);

		//�������
		OnRechargeDisposeResult(dwContextID,m_AccountsDBAide.GetReturnValue(),TEXT("�齱��ѯ�ɹ�"),false);
	}
	catch (IDataBaseException* pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//�������
		OnRechargeDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);
	}

	return true;
}

VOID CDataBaseEngineSink::OnRechargeDisposeResult( DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient )
{
	if (dwErrorCode==DB_GET_RECHARGE_SUCCESS)
	{
		//��������
		DBO_GP_Rcg_Qry_Res RcgQryRes;
		ZeroMemory(&RcgQryRes,sizeof(DBO_GP_Rcg_Qry_Res));

		//�������
		RcgQryRes.cbIsRecharge = m_AccountsDBAide.GetValue_WORD(TEXT("cbIsRecharge"));

		//��ȡ��Ϣ
		lstrcpyn(RcgQryRes.szDescribeString,pszErrorString,CountArray(RcgQryRes.szDescribeString));

		//���ͽ��
		WORD wDataSize=CountStringBuffer(RcgQryRes.szDescribeString);
		WORD wHeadSize=sizeof(RcgQryRes)-sizeof(RcgQryRes.szDescribeString);

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_RECHARGE_RESULT,dwContextID,&RcgQryRes,wHeadSize+wDataSize);
	}
	else
	{
		//��������
		DBO_GP_Rcg_Qry_Res RcgQryRes;
		ZeroMemory(&RcgQryRes,sizeof(DBO_GP_Rcg_Qry_Res));

		//��ȡ��Ϣ
		RcgQryRes.cbIsRecharge=-1;
		lstrcpyn(RcgQryRes.szDescribeString, TEXT("��ѯ��ֵ��Ϣʧ��"),CountArray(RcgQryRes.szDescribeString));

		//���ͽ��
		WORD wDataSize=CountStringBuffer(RcgQryRes.szDescribeString);
		WORD wHeadSize=sizeof(RcgQryRes)-sizeof(RcgQryRes.szDescribeString);

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_RECHARGE_RESULT,dwContextID,&RcgQryRes,wHeadSize+wDataSize);
	}
}


//////////////////////////////////////////////////////////////////////////////////
