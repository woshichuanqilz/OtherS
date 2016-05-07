#ifndef DATA_BASE_PACKET_HEAD_FILE
#define DATA_BASE_PACKET_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////
//�������ݰ�

//��¼����
#define	DBR_GP_LOGON_GAMEID			1									//I D ��¼
#define	DBR_GP_LOGON_ACCOUNTS		2									//�ʺŵ�¼
#define DBR_GP_REGISTER_ACCOUNTS	3									//ע���ʺ�
#define DBR_GP_GUEST_REGISTER		4									//�ο͵�¼

//�˺ŷ���
#define DBR_GP_MODIFY_MACHINE		10									//�޸Ļ���
#define DBR_GP_MODIFY_LOGON_PASS	11									//�޸�����
#define DBR_GP_MODIFY_INSURE_PASS	12									//�޸�����
#define DBR_GP_MODIFY_UNDER_WRITE	13									//�޸�ǩ��
#define DBR_GP_MODIFY_INDIVIDUAL	14									//�޸�����

//ͷ������
#define DBR_GP_MODIFY_SYSTEM_FACE	20									//�޸�ͷ��
#define DBR_GP_MODIFY_CUSTOM_FACE	21									//�޸�ͷ��

//��������
#define DBR_GP_USER_SAVE_SCORE		30									//������Ϸ��
#define DBR_GP_USER_TAKE_SCORE		31									//��ȡ��Ϸ��
#define DBR_GP_USER_TRANSFER_SCORE	32									//ת����Ϸ��
#define DBR_GP_USER_CHECK_PASSWORD	33

//��ѯ����
#define DBR_GP_QUERY_INDIVIDUAL		40									//��ѯ����
#define DBR_GP_QUERY_INSURE_INFO	41									//��ѯ����
#define DBR_GP_QUERY_USER_INFO	    42									//��ѯ�û�

//ϵͳ����
#define DBR_GP_LOAD_GAME_LIST		50									//�����б�
#define DBR_GP_ONLINE_COUNT_INFO	51									//������Ϣ

//ǩ������
#define DBR_GP_QUERY_SIGIN_INFO		61									//��ѯǩ��
#define DBR_GP_USER_SIGIN			62									//���ǩ��

//�齱����
#define DBR_GP_QUERY_LOTTERY_INFO	63									//�齱��ѯ
#define DBR_GP_LOTTERY_REQ			64									//�齱����

//��ֵ��Ϣ
#define DBR_GP_RECHARGE_INFO		65									//��ֵ��Ϣ

//////////////////////////////////////////////////////////////////////////////////

//ID ��¼
struct DBR_GP_LogonGameID
{
	//��¼��Ϣ
	DWORD							dwGameID;							//�û� I D
	TCHAR							szPassword[LEN_MD5];				//��¼����
	TCHAR							szPassPortID[LEN_PASS_PORT_ID];		//֤������
	BYTE							cbNeeValidateMBCard;				//�ܱ�У��

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������

	//������Ϣ
	LPVOID							pBindParameter;						//�󶨲���
};

//�ʺŵ�¼
struct DBR_GP_LogonAccounts
{
	//��¼��Ϣ
	TCHAR							szPassword[LEN_MD5];				//��¼����
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	TCHAR							szPassPortID[LEN_PASS_PORT_ID];		//֤������
	BYTE							cbNeeValidateMBCard;				//�ܱ�У��

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������

	//������Ϣ
	LPVOID							pBindParameter;						//�󶨲���
};

//�ʺ�ע��
struct DBR_GP_RegisterAccounts
{
	//ע����Ϣ
	WORD							wFaceID;							//ͷ���ʶ
	BYTE							cbGender;							//�û��Ա�
	DWORD							dwAgentID;							//�Ƽ���ID
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	TCHAR							szNickName[LEN_ACCOUNTS];			//�û��ǳ�
	TCHAR							szSpreader[LEN_ACCOUNTS];			//�Ƽ��ʺ�
	TCHAR							szPassPortID[LEN_PASS_PORT_ID];		//֤������
	TCHAR							szCompellation[LEN_COMPELLATION];	//��ʵ����
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//��ȫ�ֻ�

	//�������
	TCHAR							szLogonPass[LEN_MD5];				//��¼����(MD5����)
	TCHAR							szInsurePass[LEN_MD5];				//��������
	TCHAR							szGuestPass[LEN_PASSWORD];			//��¼����(δ����)

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������

	//������Ϣ
	LPVOID							pBindParameter;						//�󶨲���
};

//�޸Ļ���
struct DBR_GP_ModifyMachine
{
	BYTE							cbBind;								//�󶨱�־
	DWORD							dwUserID;							//�û���ʶ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szPassword[LEN_PASSWORD];			//�û�����
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//�޸�����
struct DBR_GP_ModifyLogonPass
{
	DWORD							dwUserID;							//�û� I D
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szDesPassword[LEN_PASSWORD];		//�û�����
	TCHAR							szScrPassword[LEN_PASSWORD];		//�û�����
};

//�޸�����
struct DBR_GP_ModifyInsurePass
{
	DWORD							dwUserID;							//�û� I D
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szDesPassword[LEN_PASSWORD];		//�û�����
	TCHAR							szScrPassword[LEN_PASSWORD];		//�û�����
};

//�޸�ǩ��
struct DBR_GP_ModifyUnderWrite
{
	DWORD							dwUserID;							//�û� I D
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szPassword[LEN_PASSWORD];			//�û�����
	TCHAR							szUnderWrite[LEN_UNDER_WRITE];		//����ǩ��
};

//�޸�ͷ��
struct DBR_GP_ModifySystemFace
{
	//�û���Ϣ
	WORD							wFaceID;							//ͷ���ʶ
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_MD5];				//��¼����

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//�޸�ͷ��
struct DBR_GP_ModifyCustomFace
{
	//�û���Ϣ
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_MD5];				//��¼����
	DWORD							dwCustomFace[FACE_CX*FACE_CY];		//ͼƬ��Ϣ

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//�޸�����
struct DBR_GP_ModifyIndividual
{
	//��֤����
	DWORD							dwUserID;							//�û� I D
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szPassword[LEN_PASSWORD];			//�û�����

	//�ʺ�����
	BYTE							cbGender;							//�û��Ա�
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�
	TCHAR							szUnderWrite[LEN_UNDER_WRITE];		//����ǩ��

	//�û���Ϣ
	TCHAR							szUserNote[LEN_USER_NOTE];			//�û�˵��
	TCHAR							szCompellation[LEN_COMPELLATION];	//��ʵ����

	//�绰����
	TCHAR							szSeatPhone[LEN_SEAT_PHONE];		//�̶��绰
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�ƶ��绰

	//��ϵ����
	TCHAR							szQQ[LEN_QQ];						//Q Q ����
	TCHAR							szEMail[LEN_EMAIL];					//�����ʼ�
	TCHAR							szDwellingPlace[LEN_DWELLING_PLACE];//��ϵ��ַ
};

//��ѯ����
struct DBR_GP_QueryIndividual
{
	DWORD							dwUserID;							//�û� I D
	DWORD							dwClientAddr;						//���ӵ�ַ
};

//������Ϸ��
struct DBR_GP_UserSaveScore
{
	DWORD							dwUserID;							//�û� I D
	SCORE							lSaveScore;							//������Ϸ��
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//ȡ����Ϸ��
struct DBR_GP_UserTakeScore
{
	DWORD							dwUserID;							//�û� I D
	SCORE							lTakeScore;							//��ȡ��Ϸ��
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szPassword[LEN_PASSWORD];			//��������
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//ת����Ϸ��
struct DBR_GP_UserTransferScore
{
	DWORD							dwUserID;							//�û� I D
	DWORD							dwClientAddr;						//���ӵ�ַ
	BYTE                            cbByNickName;                       //�ǳ�����
	SCORE							lTransferScore;						//ת����Ϸ��
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�
	TCHAR							szPassword[LEN_PASSWORD];			//��������
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//��ѯ����
struct DBR_GP_QueryInsureInfo
{
	DWORD							dwUserID;							//�û� I D
	DWORD							dwClientAddr;						//���ӵ�ַ
};

//��ѯ�û�
struct DBR_GP_QueryInsureUserInfo
{
	BYTE                            cbByNickName;                       //�ǳ�����
	TCHAR							szNickName[LEN_NICKNAME];			//Ŀ���û�
};

//�û�����
struct DBO_GP_UserTransferUserInfo
{
	DWORD							dwGameID;							//�û� I D
	TCHAR							szNickName[LEN_NICKNAME];			//�ʺ��ǳ�
};

//������Ϣ
struct DBR_GP_OnLineCountInfo
{
	WORD							wKindCount;							//������Ŀ
	DWORD							dwOnLineCountSum;					//��������
	tagOnLineInfoKind				OnLineCountKind[MAX_KIND];			//��������
};

//ǩ����Ϣ��ѯ
struct DBR_GP_QuerySignInInfo
{
	DWORD dwUserID;
};

//���ǩ��
struct DBR_GP_UserSignIn
{
	DWORD dwUserID;
	WORD  wBQDay;
};

//�齱��Ϣ��ѯ
struct DBR_GP_QryLtryInfo
{
	DWORD dwUserID;
};

//�û��齱����
struct DBR_GP_UserLtry
{
	DWORD dwUserID;
};

//��ֵ��Ϣ
struct DBR_GP_RechargeInfo
{
	DWORD dwUserID;
};

//////////////////////////////////////////////////////////////////////////////////
//�����Ϣ

//��¼���
#define DBO_GP_LOGON_SUCCESS		100									//��¼�ɹ�
#define DBO_GP_LOGON_FAILURE		101									//��¼ʧ��
#define DBO_GP_VALIDATE_MBCARD		102									//��¼ʧ��
#define DBO_GP_VALIDATE_PASSPOSTID	103									//��¼ʧ��
#define DBO_GP_GUEST_LOGON_SUCCESS	104									//�ο͵�¼

//�������
#define DBO_GP_USER_FACE_INFO		110									//�û�ͷ��
#define DBO_GP_USER_INDIVIDUAL		111									//�û�����

//��������
#define DBO_GP_USER_INSURE_INFO		120									//��������
#define DBO_GP_USER_INSURE_SUCCESS	121									//���гɹ�
#define DBO_GP_USER_INSURE_FAILURE	122									//����ʧ��
#define DBO_GP_USER_INSURE_USER_INFO   123								//�û�����
#define DBO_GP_USER_INSURE_TRANSFER_INFO	124
#define DBO_GP_USER_CHECK_PASSWORD	125

//�б����
#define DBO_GP_GAME_TYPE_ITEM			130									//������Ϣ
#define DBO_GP_GAME_KIND_ITEM			131									//������Ϣ
#define DBO_GP_GAME_NODE_ITEM			132									//�ڵ���Ϣ
#define DBO_GP_GAME_PAGE_ITEM			133									//������Ϣ
#define DBO_GP_GAME_LIST_RESULT			134									//���ؽ��

//ǩ�����
#define DBO_GP_SIGNIN_QUERY_SUCCESS		201									//��ѯ�ɹ�
#define DBO_GP_SIGNIN_QUERY_FAILURE		202									//��ѯʧ��
#define DBO_GP_USER_SIGNIN_SUCCESS		203									//��ѯ�ɹ�
#define DBO_GP_USER_SIGNIN_FAILURE		204									//��ѯʧ��

//�齱���
#define DBO_GP_LOTTERY_QUERY_RESULT		205									//�齱�ɹ�
#define DBO_GP_USER_LOTTERY_RESULT		206									//�齱�ɹ�

//��ֵ��Ϣ
#define DBO_GP_RECHARGE_RESULT			207									//��ֵ��Ϣ

//������
#define DBO_GP_OPERATE_SUCCESS			500									//�����ɹ�
#define DBO_GP_OPERATE_FAILURE			501									//����ʧ��

//��¼�ɹ�
struct DBO_GP_LogonSuccess
{
	//��������
	WORD							wFaceID;							//ͷ���ʶ
	DWORD							dwUserID;							//�û���ʶ
	DWORD							dwGameID;							//��Ϸ��ʶ
	DWORD							dwGroupID;							//���ű�ʶ
	DWORD							dwCustomID;							//�Զ�����
	DWORD							dwUserMedal;						//�û�����
	DWORD							dwExperience;						//������ֵ
	DWORD							dwLoveLiness;						//�û�����
	TCHAR							szPassword[LEN_MD5];				//��¼����
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�
	TCHAR							szGroupName[LEN_GROUP_NAME];		//��������

	//�û��ɼ�
	SCORE							lUserScore;							//�û���Ϸ��
	SCORE							lUserInsure;						//�û�����

	//�û�����
	BYTE							cbGender;							//�û��Ա�
	BYTE							cbMoorMachine;						//��������
	TCHAR							szUnderWrite[LEN_UNDER_WRITE];		//����ǩ��

	//��Ա����
	BYTE							cbMemberOrder;						//��Ա�ȼ�
	SYSTEMTIME						MemberOverDate;						//����ʱ��

	//��ֵ��Ϣ
	BYTE							cbIsRecharge;						//�Ƿ��ֵ

	//������Ϣ
	TCHAR							szDescribeString[128];				//������Ϣ
};

//��¼ʧ��
struct DBO_GP_LogonFailure
{
	LONG							lResultCode;						//�������
	TCHAR							szDescribeString[128];				//������Ϣ
};

//��¼ʧ��
struct DBR_GP_ValidateMBCard
{
	UINT							uMBCardID;						//��������
};

//ͷ����Ϣ
struct DBO_GP_UserFaceInfo
{
	WORD							wFaceID;							//ͷ���ʶ
	DWORD							dwCustomID;							//�Զ�����
};

//��������
struct DBO_GP_UserIndividual
{
	//�û���Ϣ
	DWORD							dwUserID;							//�û� I D
	TCHAR							szUserNote[LEN_USER_NOTE];			//�û�˵��
	TCHAR							szCompellation[LEN_COMPELLATION];	//��ʵ����

	//�绰����
	TCHAR							szSeatPhone[LEN_SEAT_PHONE];		//�̶��绰
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�ƶ��绰

	//��ϵ����
	TCHAR							szQQ[LEN_QQ];						//Q Q ����
	TCHAR							szEMail[LEN_EMAIL];					//�����ʼ�
	TCHAR							szDwellingPlace[LEN_DWELLING_PLACE];//��ϵ��ַ
};

//��������
struct DBO_GP_UserInsureInfo
{
	WORD							wRevenueTake;						//˰�ձ���
	WORD							wRevenueTransfer;					//˰�ձ���
	WORD							wServerID;							//�����ʶ
	SCORE							lUserScore;							//�û���Ϸ��
	SCORE							lUserInsure;						//������Ϸ��
	SCORE							lTransferPrerequisite;				//ת������
};

//���гɹ�
struct DBO_GP_UserInsureSuccess
{
	DWORD							dwUserID;							//�û� I D
	SCORE							lSourceScore;						//ԭ����Ϸ��
	SCORE							lSourceInsure;						//ԭ����Ϸ��
	SCORE							lInsureRevenue;						//����˰��
	SCORE							lVariationScore;					//��Ϸ�ұ仯
	SCORE							lVariationInsure;					//���б仯
	TCHAR							szDescribeString[128];				//������Ϣ
};

//����ʧ��
struct  DBO_GP_UserInsureFailure
{
	LONG							lResultCode;						//��������
	TCHAR							szDescribeString[128];				//������Ϣ
};

//����ʧ��
struct DBO_GP_OperateFailure
{
	LONG							lResultCode;						//��������
	TCHAR							szDescribeString[128];				//������Ϣ
};

//�����ɹ�
struct DBO_GP_OperateSuccess
{
	LONG							lResultCode;						//��������
	TCHAR							szDescribeString[128];				//�ɹ���Ϣ
};

//////////////////////////////////////////////////////////////////////////////////

//��Ϸ����
struct DBO_GP_GameType
{
	WORD							wJoinID;							//�ҽ�����
	WORD							wSortID;							//��������
	WORD							wTypeID;							//��������
	TCHAR							szTypeName[LEN_TYPE];				//��������
};

//��Ϸ����
struct DBO_GP_GameKind
{
	WORD							wTypeID;							//��������
	WORD							wJoinID;							//�ҽ�����
	WORD							wSortID;							//��������
	WORD							wKindID;							//��������
	WORD							wGameID;							//ģ������
	TCHAR							szKindName[LEN_KIND];				//��Ϸ����
	TCHAR							szProcessName[LEN_PROCESS];			//��������
};

//��Ϸ�ڵ�
struct DBO_GP_GameNode
{
	WORD							wKindID;							//��������
	WORD							wJoinID;							//�ҽ�����
	WORD							wSortID;							//��������
	WORD							wNodeID;							//�ڵ�����
	TCHAR							szNodeName[LEN_NODE];				//�ڵ�����
};

//��������
struct DBO_GP_GamePage
{
	WORD							wKindID;							//��������
	WORD							wNodeID;							//�ڵ�����
	WORD							wSortID;							//��������
	WORD							wPageID;							//��������
	WORD							wOperateType;						//��������
	TCHAR							szDisplayName[LEN_PAGE];			//��ʾ����
};

//���ؽ��
struct DBO_GP_GameListResult
{
	BYTE							cbSuccess;							//�ɹ���־
};
//////////////////////////////////////////////////////////////////////////
//ǩ����Ϣ
struct DBO_GP_UserSignInInfo
{
	WORD                            wSignInDay;                         //�õڼ���ǩ����1-7��
	SCORE							llExtraPrize;						//���⽱��
	SCORE							llBQCost;							//��ǩ����
	tagSignInDayInfo                SignInDay[7];						//ÿ����Ϣ

	//������Ϣ
	TCHAR							szDescribeString[128];				//������Ϣ
};

//���ǩ�����
struct DBO_GP_UserSignInResult
{
	BOOL                            bIsSuccess;                         //ǩ���Ƿ�ɹ�
	BOOL                            bIsAllDaySigned;                     //�����Ƿ�ǩ��

	//������Ϣ
	TCHAR							szDescribeString[128];				//������Ϣ
};

//��ѯ�齱���
struct DBO_GP_Ltry_Qry_Res
{
	WORD							wFreeCount;
	TCHAR							szDescribeString[128];				//������Ϣ
};

//�û��齱���
struct DBO_GP_User_Ltry_Res
{
	WORD							wIsLtrySuc;							//�齱�Ƿ�ɹ�
	TCHAR							szNickName[LEN_NICKNAME];			//�û�����
	WORD							wPrizeID;							//����ID
	WORD							wFreeCount;							//��Ѵ���
	SCORE                           lUserScore;                        //��ҵ�ǰʣ����
	TCHAR							szDescribeString[128];				//������Ϣ
};

//��ֵ���
struct DBO_GP_Rcg_Qry_Res
{
	BYTE							cbIsRecharge;
	TCHAR							szDescribeString[128];				//������Ϣ
};
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//�ֻ����ݰ�

//��¼����
#define	DBR_MB_LOGON_GAMEID			500									//I D ��¼
#define	DBR_MB_LOGON_ACCOUNTS		501									//�ʺŵ�¼
#define DBR_MB_REGISTER_ACCOUNTS	502									//ע���ʺ�

//��¼���
#define DBO_MB_LOGON_SUCCESS		600									//��¼�ɹ�
#define DBO_MB_LOGON_FAILURE		601									//��¼ʧ��

//////////////////////////////////////////////////////////////////////////////////

//ID ��¼
struct DBR_MB_LogonGameID
{
	//��¼��Ϣ
	DWORD							dwGameID;							//�û� I D
	TCHAR							szPassword[LEN_MD5];				//��¼����

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�绰����

	//������Ϣ
	LPVOID							pBindParameter;						//�󶨲���
};

//�ʺŵ�¼
struct DBR_MB_LogonAccounts
{
	//��¼��Ϣ
	TCHAR							szPassword[LEN_MD5];				//��¼����
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�绰����

	//������Ϣ
	LPVOID							pBindParameter;						//�󶨲���
};

//�ʺ�ע��
struct DBR_MB_RegisterAccounts
{
	//ע����Ϣ
	WORD							wFaceID;							//ͷ���ʶ
	BYTE							cbGender;							//�û��Ա�
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	TCHAR							szNickName[LEN_ACCOUNTS];			//�û��ǳ�

	//�������
	TCHAR							szLogonPass[LEN_MD5];				//��¼����
	TCHAR							szInsurePass[LEN_MD5];				//��������

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�绰����

	//������Ϣ
	LPVOID							pBindParameter;						//�󶨲���
};

//��¼�ɹ�
struct DBO_MB_LogonSuccess
{
	//�û�����
	WORD							wFaceID;							//ͷ���ʶ
	BYTE							cbGender;							//�û��Ա�
	DWORD							dwUserID;							//�û� I D
	DWORD							dwGameID;							//��Ϸ I D
	DWORD							dwExperience;						//������ֵ
	DWORD							dwLoveLiness;						//�û�����
	TCHAR							szNickName[LEN_ACCOUNTS];			//�û��ǳ�

	//������Ϣ
	TCHAR							szDescribeString[128];				//������Ϣ
};


//��¼ʧ��
struct DBO_MB_LogonFailure
{
	LONG							lResultCode;						//�������
	TCHAR							szDescribeString[128];				//������Ϣ
};

//////////////////////////////////////////////////////////////////////////////////

#endif