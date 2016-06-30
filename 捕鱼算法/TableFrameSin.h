#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE
#pragma once
//�ӵ���Ϣ
struct tagBulletInfo
{
	DWORD dwID;//�ӵ�ID
	int score;//�ӵ�����
	bool bHave;//�ӵ�����
	bool bTry;//�����ӵ�
	bool bHitMore;
	char cbHitTime;
	__int64 lTick;
};

//����Ϣ
struct tagFishInfo
{
	int nSpriteID;
	int nHit;
	int nMultiple;
	int nSameDieID;
	int nProperty;
	int nPlotID;
	CString sName;
	tagFishInfo()
	{
		nSameDieID=0;
		nHit=0;
		nMultiple=0;
		nSameDieID=0;
		nProperty=0;
		nPlotID=0;
		sName=TEXT("");
	}
};

//����
struct tagNotice
{
	int nID;
	CString sNotice;
	tagNotice()
	{
		nID=0;
		sNotice=TEXT("");
	}
};

//��������
struct tagPlotInfo
{
	int nID;
	int nType;
	int nFunc;
	int nParam0;
	int nParam1;
	int nParam2;
	int nParam3;
	int nParam4;
	int nParam5;
	int nParam6;
	int nPercent;
	int nExecuteCount;
	int nExecuteRange;
	int nFishIDBegin;
	int nFishIDRange;
	int nPathIndexBegin;
	int nPathIndexRange;
	int nDelayTime;
	int nSpaceTime;
	bool bGroupPos;
	bool bNotNormal;
	float fElapse;
	unsigned int dwLastTick;
	float fRotation[PATH_POINT_COUNT];
	int nMoveTime[PATH_POINT_COUNT];
	int nSpeed[PATH_POINT_COUNT];
};

//������Ϣ
struct tagConfig
{
	int nCellScore;		//��Ԫ����
	int nMinScore;		//��С����
	int nMaxScore;		//������
	int nAddScore;		//ÿ���Ϸ�
	int nExchange;		//�һ�����
	int nMaxBullet;		//����ӵ�
	//@��ϰ����
	int nExercise;

	int nAndroidLock;	//���������㼸��
	float m_fAutoStockDiffMaxFactor;//��̬����Ѷȵ�����ϵ��
	float m_fAutoStockDiffMaxValue;//��̬����Ѷȵ������ֵ
};

//������
struct tagFishCount
{
	int nID;
	int nCount;
};

struct tagManEatingFish
{
	bool bIsDead[3];
	int nFishID[3];
	int nEatScore[3];
	int nSpriteID[3];
};

#define POINTS 15
#define PIVALUE 3.1415926536f
struct tagBoxPrizeInfo
{
	WORD wBoxPrizeID;
	int  nAddScore;
	LONG lUserScore;
};

//�����Ϣ
struct tagPlayerInfo
{
public:
	std::vector<tagMatchInfo> vMatchInfo;
	std::vector<tagFishCount> vFishCount;

	LONG lUserCellScore;
	LONG lUserAllScore;
	LONG lUserTryScore;
	LONG lUserEnterScore;
	BYTE cbCannonType;

	//��¼
	int nSitTime;
	int nSendTime;
	int nGameTime;
	int nOutTime;
	int nShootCount;
	int nFishCount;//����
	int nBuyTimes;//�Ϸִ���
	int nMultipleTimes;//�л���̨����

	bool bBonus;
	bool bSuperCannon;
	bool bTryCannon;
	bool bSkillGave;
	bool bScoreFull;

	tagBulletInfo BulletInfo[200];

	int nElapseTime;//�����Ϸʱ��
	int nTotalWinScore;//�����Ӯ��
	int nLotteryGeted;//�ѻ�õĽ�ȯ
	int nLotteryGift;//���ͽ�ȯ
	WORD wMobileMoneyWill;//�����õ��Ļ���
	WORD wMobileMoneyGeted;//�ѻ�õĻ���
	WORD wMobileMoneyGift;//���ͻ���
	WORD wShareCancelCount;//ȡ������Ĵ���
	unsigned int nMobileMoneyTime;
	WORD wNewPlayerMobileMoney;
	int nThisTimeWinScore;
	__int64 nPassTime;
	__int64 nNextLotteryTime;

	bool bJumpBomb;
	int nJumpBombMultiple;
	int nJumpCount;

	float fUserLucky;
	unsigned int dwLuckyChangeTime;
	unsigned int dwLuckyRandTime;

	bool bGetedDragonPiece;
	int nEquipmentDragonCount;
	std::queue<int> qBoxPrize;

	float fUpperLimit;
	float fLowerLimit;
	float fCurve[POINTS];
	float fYields;
	int nPointCount;
	int nFireCount;
	__int64 lGains;
	__int64 lConsume;

	void initCurve();
	void fire(int nFireScore);
	int calcFishPercent(int nFishPercent, int nFishMultiple);
	void hit(int nHitScore, int nSpriteID);

	tagPlayerInfo()
	{
		Clear();
	}

	void ClearBullet()
	{
		for (int i=0; i<CountArray(BulletInfo); i++)
		{
			BulletInfo[i].bHave=false;
			BulletInfo[i].dwID=0;
			BulletInfo[i].score=0;
			BulletInfo[i].bTry=false;
			BulletInfo[i].bHitMore=false;
			BulletInfo[i].cbHitTime=0;
			BulletInfo[i].lTick=0;
		}
	}

	void Clear()
	{
		vMatchInfo.clear();
		vFishCount.clear();
		lUserCellScore=0;
		lUserAllScore=0;
		lUserTryScore=0;
		lUserEnterScore=0;
		cbCannonType=0;
		nSitTime=0;
		nSendTime=0;
		nGameTime=0;
		nOutTime=0;
		nShootCount=0;
		bBonus=false;
		bSuperCannon=false;
		bTryCannon=false;
		bSkillGave=false;
		bScoreFull=false;
		ClearBullet();
		nElapseTime=0;
		nThisTimeWinScore=0;
		nTotalWinScore=0;
		nLotteryGeted=0;
		nLotteryGift=0;
		wMobileMoneyWill=0;
		wMobileMoneyGeted=0;
		wMobileMoneyGift=0;
		wShareCancelCount=0;
		nMobileMoneyTime=0;
		wNewPlayerMobileMoney=0;
		nPassTime=0;
		nNextLotteryTime=0;
		bJumpBomb=false;
		nJumpBombMultiple=0;
		nJumpCount=0;
		nFishCount=0;
		nBuyTimes=0;
		nMultipleTimes=0;
		fUserLucky = 0.0f;
		dwLuckyChangeTime = 0;
		dwLuckyRandTime = 0;
		bGetedDragonPiece = false;
		nEquipmentDragonCount = 0;

		// clear the queue
		std::queue<int>().swap(qBoxPrize);
		fUpperLimit = 1.1f;
		fLowerLimit = 0.5f;
		fYields = 0.0f;
		nPointCount = -1;
		nFireCount = 0;
		lGains = 0;
		lConsume = 0;
	}

	void addBullet(DWORD id, int score, bool bTry, bool bHitMore, __int64 lTick)
	{
		for (int i=0; i<CountArray(BulletInfo); i++)
		{
			if (!BulletInfo[i].bHave)
			{
				BulletInfo[i].bHave=true;
				BulletInfo[i].dwID=id;
				BulletInfo[i].score=score;
				BulletInfo[i].bTry=bTry;
				BulletInfo[i].bHitMore = bHitMore;
				BulletInfo[i].cbHitTime = 0;
				BulletInfo[i].lTick = lTick;
				break;
			}
		}
	}

	int getBulletCount()
	{
		int nBulletCount=0;
		for (int i=0; i<CountArray(BulletInfo); i++)
		{
			if (BulletInfo[i].bHave)
				nBulletCount++;
		}
		return nBulletCount;
	}

	void addFishCount(DWORD id)
	{
		bool bHave=false;
		for (unsigned int i=0; i<vFishCount.size(); i++)
		{
			if (id==vFishCount[i].nID)
			{
				vFishCount[i].nCount++;
				bHave=true;
				break;
			}
		}
		if (!bHave)
		{
			tagFishCount FishCount={0};
			FishCount.nID=id;
			FishCount.nCount=1;
			vFishCount.push_back(FishCount);
		}
	}
};

//��Ϸ����
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
protected:
	int								m_nScoreOut;//�ͳ���
	int								m_nLittleRevenue;//����1�ֵĳ�ˮ(ǧ�ֱ�)
	int								m_nSceneIndex;//��������
	int								m_nTimeChangeScene;
	int								m_nPlotNumber;
	int								m_nPlotTime[10];
	int								m_nChangeSceneElapse;
	int								m_nBossScore;
	unsigned int					m_dwFishID;//���ʶ
	unsigned int					m_dwBulletID;//�ӵ���ʶ
	bool							m_bFishingOffSeason;
	bool							m_bNoFish;//���������
	bool							m_bManEatingFish;
	int								m_nManEatingFishTime;
	tagFishPack						m_FishPackBoss;
	int								m_nRoomType;

	int								m_nBossComingCount;

	tagPlayerInfo					m_PlayerInfo[GAME_PLAYER];
	tagManEatingFish				m_ManEatingFish[GAME_PLAYER];

	tagConfig						m_Config;
	MatchManager					m_MatchManager;
	std::vector<tagFishInfo *>		m_vFishInfo;
	std::vector<tagPlotInfo *>		m_vPlotInfo;
	std::vector<tagNotice *>		m_vNotice;
	CFactoryTemplate<tagFishPack>	m_FishFactory;//��Ⱥ����
	bool							m_bMatchPlaying;
	//����ӿ�
protected:
	ITableFrame *					m_pITableFrame;//��ܽӿ�
	tagGameServiceOption *			m_pGameServiceOption;//��Ϸ����
	tagGameServiceAttrib *			m_pGameServiceAttrib;//��Ϸ����

	//��������
public:
	//���캯��
	CTableFrameSink();
	//��������
	virtual ~CTableFrameSink();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID Release() { delete this; }
	//�ӿڲ�ѯ
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//����ӿ�
public:
	//��λ����
	virtual VOID RepositionSink() {}
	//��������
	virtual bool Initialization(IUnknownEx * pIUnknownEx);

	//��ѯ�ӿ�
public:
	//��ѯ�޶�
	virtual SCORE QueryConsumeQuota(IServerUserItem * pIServerUserItem) { return 0L; }
	//���ٻ���
	virtual SCORE QueryLessEnterScore(WORD wChairID, IServerUserItem * pIServerUserItem) { return 0L; }
	//��ѯ�Ƿ�۷����
	virtual bool QueryBuckleServiceCharge(WORD wChairID) {return true;}

	//��Ϸ�¼�
public:
	//��Ϸ��ʼ
	virtual bool OnEventGameStart() { return true; }
	//��Ϸ����
	virtual bool OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);
	//���ͳ���
	virtual bool OnEventSendGameScene(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret);

	//�¼��ӿ�
public:
	//ʱ���¼�
	virtual bool OnTimerMessage(DWORD wTimerID, WPARAM wBindParam);
	//�����¼�
	virtual bool OnDataBaseMessage(WORD wRequestID, VOID * pData, WORD wDataSize) { return false; }
	//�����¼�
	virtual bool OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason) { return false; }
	//������������
	bool OnNewGuidGift(IServerUserItem * pIServerUserItem, bool bSuccess, int nCannonStatus, int nLotteryCount);
	//Ӯ������
	bool OnWinPearl(IServerUserItem * pIServerUserItem, int nPearlType, int nPearlCount);
	//��ȡ�û�����
	bool OnGetUserItemCount(IServerUserItem * pIServerUserItem, int nType, SCORE lCount);
	//�����������
	void OnMatchRank(IServerUserItem * pIServerUserItem, SCORE lMatchScore, int nCurrentRank, int nChangeRank);

	//����ӿ�
public:
	//��Ϸ��Ϣ
	virtual bool OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//�����Ϣ
	virtual bool OnFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem) { return false; }

	//�����ӿ�
public:
	//���û���
	virtual void SetGameBaseScore(LONG lBaseScore) {}
	//��ȡ��Ϸ�ڲ�����
	virtual	LONG GetInsideScore(WORD wChairID);

	//�û��¼�
public:
	//�û�����
	virtual bool OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem);
	//�û�����
	virtual bool OnActionUserConnect(WORD wChairID, IServerUserItem * pIServerUserItem) { return true; }
	//�û�����
	virtual bool OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�����
	virtual bool OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�ͬ��
	virtual bool OnActionUserOnReady(WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize) { return true; }

	//��Ϸ�¼�
protected:
	//�������
	void plotSpecial(int number);
	//��ͨ����
	void plotNormal(WORD wChairID = INVALID_CHAIR);
	//
	unsigned int GetNewFishID();
	//
	unsigned int GetNewBulletID();
	//������Ϣ
	void SendDataExcludeSourceUser(IServerUserItem * pIServerUserItemSource, WORD wSubCmdID, void * pData, WORD wDataSize);
	//
	bool OnSubHitFish(IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize);
	//���ڴ���
	bool OnSubFire(IServerUserItem * pIServerUserItem, float fAngle, DWORD dwBulletID, int nCount, BYTE cbAttrib);
	//��̨���ʴ���
	bool OnSubCannonMultiple(IServerUserItem * pIServerUserItem, BYTE cbAddMultiple, BYTE cbCannonType, int nMultiple=0);
	//�����ӵ�
	bool OnSubBuyBullet(IServerUserItem * pIServerUserItem, bool bAdd, int nScore);
	//
	double random(double start, double end);
	//��ȡ����
	tagFishInfo * getFishInfo(int nSpriteID);
	//��ȡ��������
	tagPlotInfo * getPlotInfo(int nPlotID);
	//��ȡ����
	void readXML(const char * filename);
	//���������
	tagFishPack * fillFishPack(int nSpriteID, int nPathIndex1, int nCreateDelayTime);
	//�洢�û���Ʒ
	void SaveUserItem(IServerUserItem * pIServerUserItem, int nType, int nFlag, SCORE lCount, bool bSaveRobot);
	//ԲȦ����
	void plotCircle(int nPlotID, int nXPos, int nYPos);
	//��Ϸ��¼
	void saveGameJson(IServerUserItem * pIServerUserItem, tagPlayerInfo & PlayerInfo, bool bStandUp);
	//����
	void calc(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);

	float changeLucky(tagPlayerInfo & PlayerInfo);
	//������һ���
	virtual void UpdateUserIntegrate(IServerUserItem * pIServerUserItem,int nAddScore);

	virtual bool IsSignUpMatch(IServerUserItem * pIServerUserItem);

	//�����ڱ���
	virtual bool OnChangeMatchPlayerScore(IServerUserItem * pIServerUserItem, int wSignUpFee, WORD wChairID, SCORE & llUserScore);

	virtual void SetMatchStatus(bool bPlaying);

	void BoxChange(IServerUserItem * pIServerUserItem, tagPlayerInfo & PlayerInfo, int nFishID);

	void GenManEatingFish(WORD wChairID, WORD wIndex, int nSpriteID);

	void SendManEatingFish();

	void OnRecord(char* strMsg);
};

#endif