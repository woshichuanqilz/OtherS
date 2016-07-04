#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE
#pragma once
//子弹信息
struct tagBulletInfo
{
	DWORD dwID;//子弹ID
	int score;//子弹倍率
	bool bHave;//子弹存在
	bool bTry;//试用子弹
	bool bHitMore;
	char cbHitTime;
	__int64 lTick;
};

//鱼信息
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

//公告
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

//剧情配置
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

//配置信息
struct tagConfig
{
	int nCellScore;		//单元积分
	int nMinScore;		//最小积分
	int nMaxScore;		//最大积分
	int nAddScore;		//每次上分
	int nExchange;		//兑换比例
	int nMaxBullet;		//最大子弹
	//@练习房间
	int nExercise;

	int nAndroidLock;	//机器人锁鱼几率
	float m_fAutoStockDiffMaxFactor;//动态库存难度调整正系数
	float m_fAutoStockDiffMaxValue;//动态库存难度调整最大值
};

//鱼数量
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

//玩家信息
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

	//记录
	int nSitTime;
	int nSendTime;
	int nGameTime;
	int nOutTime;
	int nShootCount;
	int nFishCount;//鱼数
	int nBuyTimes;//上分次数
	int nMultipleTimes;//切换炮台次数

	bool bBonus;
	bool bSuperCannon;
	bool bTryCannon;
	bool bSkillGave;
	bool bScoreFull;

	tagBulletInfo BulletInfo[200];

	int nElapseTime;//玩家游戏时长
	int nTotalWinScore;//玩家总赢分
	int nLotteryGeted;//已获得的奖券
	int nLotteryGift;//赠送奖券
	WORD wMobileMoneyWill;//即将得到的话费
	WORD wMobileMoneyGeted;//已获得的话费
	WORD wMobileMoneyGift;//赠送话费
	WORD wShareCancelCount;//取消分享的次数
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

//游戏桌子
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
protected:
	int								m_nScoreOut;//送出分
	int								m_nLittleRevenue;//不足1分的抽水(千分比)
	int								m_nSceneIndex;//背景索引
	int								m_nTimeChangeScene;
	int								m_nPlotNumber;
	int								m_nPlotTime[10];
	int								m_nChangeSceneElapse;
	int								m_nBossScore;
	unsigned int					m_dwFishID;//鱼标识
	unsigned int					m_dwBulletID;//子弹标识
	bool							m_bFishingOffSeason;
	bool							m_bNoFish;//渔阵进行中
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
	CFactoryTemplate<tagFishPack>	m_FishFactory;//鱼群工厂
	bool							m_bMatchPlaying;
	//组件接口
protected:
	ITableFrame *					m_pITableFrame;//框架接口
	tagGameServiceOption *			m_pGameServiceOption;//游戏配置
	tagGameServiceAttrib *			m_pGameServiceAttrib;//游戏属性

	//函数定义
public:
	//构造函数
	CTableFrameSink();
	//析构函数
	virtual ~CTableFrameSink();

	//基础接口
public:
	//释放对象
	virtual VOID Release() { delete this; }
	//接口查询
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//管理接口
public:
	//复位桌子
	virtual VOID RepositionSink() {}
	//配置桌子
	virtual bool Initialization(IUnknownEx * pIUnknownEx);

	//查询接口
public:
	//查询限额
	virtual SCORE QueryConsumeQuota(IServerUserItem * pIServerUserItem) { return 0L; }
	//最少积分
	virtual SCORE QueryLessEnterScore(WORD wChairID, IServerUserItem * pIServerUserItem) { return 0L; }
	//查询是否扣服务费
	virtual bool QueryBuckleServiceCharge(WORD wChairID) {return true;}

	//游戏事件
public:
	//游戏开始
	virtual bool OnEventGameStart() { return true; }
	//游戏结束
	virtual bool OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);
	//发送场景
	virtual bool OnEventSendGameScene(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret);

	//事件接口
public:
	//时间事件
	virtual bool OnTimerMessage(DWORD wTimerID, WPARAM wBindParam);
	//数据事件
	virtual bool OnDataBaseMessage(WORD wRequestID, VOID * pData, WORD wDataSize) { return false; }
	//积分事件
	virtual bool OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason) { return false; }
	//新手引导奖励
	bool OnNewGuidGift(IServerUserItem * pIServerUserItem, bool bSuccess, int nCannonStatus, int nLotteryCount);
	//赢得珍珠
	bool OnWinPearl(IServerUserItem * pIServerUserItem, int nPearlType, int nPearlCount);
	//获取用户道具
	bool OnGetUserItemCount(IServerUserItem * pIServerUserItem, int nType, SCORE lCount);
	//处理比赛排名
	void OnMatchRank(IServerUserItem * pIServerUserItem, SCORE lMatchScore, int nCurrentRank, int nChangeRank);

	//网络接口
public:
	//游戏消息
	virtual bool OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//框架消息
	virtual bool OnFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem) { return false; }

	//比赛接口
public:
	//设置基数
	virtual void SetGameBaseScore(LONG lBaseScore) {}
	//获取游戏内部分数
	virtual	LONG GetInsideScore(WORD wChairID);

	//用户事件
public:
	//用户断线
	virtual bool OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem);
	//用户重入
	virtual bool OnActionUserConnect(WORD wChairID, IServerUserItem * pIServerUserItem) { return true; }
	//用户坐下
	virtual bool OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//用户起立
	virtual bool OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//用户同意
	virtual bool OnActionUserOnReady(WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize) { return true; }

	//游戏事件
protected:
	//特殊剧情
	void plotSpecial(int number);
	//普通剧情
	void plotNormal(WORD wChairID = INVALID_CHAIR);
	//
	unsigned int GetNewFishID();
	//
	unsigned int GetNewBulletID();
	//发送消息
	void SendDataExcludeSourceUser(IServerUserItem * pIServerUserItemSource, WORD wSubCmdID, void * pData, WORD wDataSize);
	//
	bool OnSubHitFish(IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize);
	//开炮处理
	bool OnSubFire(IServerUserItem * pIServerUserItem, float fAngle, DWORD dwBulletID, int nCount, BYTE cbAttrib);
	//炮台倍率处理
	bool OnSubCannonMultiple(IServerUserItem * pIServerUserItem, BYTE cbAddMultiple, BYTE cbCannonType, int nMultiple=0);
	//购买子弹
	bool OnSubBuyBullet(IServerUserItem * pIServerUserItem, bool bAdd, int nScore);
	//
	double random(double start, double end);
	//获取配置
	tagFishInfo * getFishInfo(int nSpriteID);
	//获取剧情配置
	tagPlotInfo * getPlotInfo(int nPlotID);
	//读取配置
	void readXML(const char * filename);
	//填充鱼数据
	tagFishPack * fillFishPack(int nSpriteID, int nPathIndex1, int nCreateDelayTime);
	//存储用户物品
	void SaveUserItem(IServerUserItem * pIServerUserItem, int nType, int nFlag, SCORE lCount, bool bSaveRobot);
	//圆圈剧情
	void plotCircle(int nPlotID, int nXPos, int nYPos);
	//游戏记录
	void saveGameJson(IServerUserItem * pIServerUserItem, tagPlayerInfo & PlayerInfo, bool bStandUp);
	//结算
	void calc(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);

	float changeLucky(tagPlayerInfo & PlayerInfo);
	//更新玩家积分
	virtual void UpdateUserIntegrate(IServerUserItem * pIServerUserItem,int nAddScore);

	virtual bool IsSignUpMatch(IServerUserItem * pIServerUserItem);

	//比赛内报名
	virtual bool OnChangeMatchPlayerScore(IServerUserItem * pIServerUserItem, int wSignUpFee, WORD wChairID, SCORE & llUserScore);

	virtual void SetMatchStatus(bool bPlaying);

	void BoxChange(IServerUserItem * pIServerUserItem, tagPlayerInfo & PlayerInfo, int nFishID);

	void GenManEatingFish(WORD wChairID, WORD wIndex, int nSpriteID);

	void SendManEatingFish();

	void OnRecord(char* strMsg);
};

#endif