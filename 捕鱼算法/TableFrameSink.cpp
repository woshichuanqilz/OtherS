#include "StdAfx.h"

enum enServerTimerID
{
	enServerTimerID_Begin=0,
	TimerClearTrace,//销毁轨迹
	TimerChangeScene,//切换场景
	TimerRegularFish,//规则鱼群
	TimerCreateFish,
	TimerCheckUser,
	TimerAndroid,//机器人
	TimerCheckMatch,
	TimerWatch,
	TimerManEatingFish,
	TimerSysMessage0=20,
	TimerSysMessage1,
	TimerSysMessage2,
	TimerSysMessage3,
	TimerSysMessage4,
	TimerSysMessage5,
	TimerSysMessage10,
	TimerSysMessage11,
	TimerSysMessage12,
	TimerSysMessage13,
	TimerSysMessage14,
	TimerSysMessage15,
	enServerTimerID_End
};

#define TIME_CLEAR_TRACE			3									//销毁轨迹

#define FISH_ALIVE_TIME				80*1000								//存活时间

#define TIMER_REPEAT_TIMER			DWORD(-1)							//重复次数

#define TIME_LOTTERY				(300 + 10 * (rand() % 8 + 1))

char szBuffer1[4096];
WCHAR szWBuffer1[4096];

static FILE * fcalcnew = NULL;

//Unicode转换为ANSI
bool unicodeToAnsi(LPCWSTR str1, std::string & str2)
{
	//获取需要的缓冲区长度
	int len=WideCharToMultiByte(CP_ACP,NULL,str1,-1,NULL,0,NULL,FALSE);
	if (0==len)
		return false;
	//LPSTR buffer=new char[len];
	LPSTR buffer=szBuffer1;
	WideCharToMultiByte(CP_ACP,NULL,str1,-1,buffer,len,NULL,FALSE);
	str2=buffer;
	//delete []buffer;
	return true;
}

//UTF8转换为Unicode
bool utf8ToUnicode(LPCSTR str1, std::wstring & str2)
{
	//获取需要的缓冲区长度
	int len=MultiByteToWideChar(CP_UTF8,0,str1,-1,NULL,0);
	if (0==len) return false;
	//LPWSTR buffer=new WCHAR[len];
	LPWSTR buffer=szWBuffer1;
	MultiByteToWideChar(CP_UTF8,0,str1,-1,buffer,len);
	str2=buffer;
	//delete []buffer;
	return true;
}

unsigned __int64 _GetTickCount64()
{
	static unsigned __int64 llLastTick = 0;

	unsigned int dwCurrentTick = GetTickCount();
	unsigned int dwHighBit = (unsigned int)(llLastTick >> 32);
	unsigned int dwLowBit = (unsigned int)llLastTick;

	if (0 == llLastTick)
	{
		llLastTick = dwCurrentTick;
		return llLastTick;
	}

	if (dwCurrentTick < dwLowBit)
	{
		dwHighBit += 1;
	}

	dwLowBit = dwCurrentTick;
	llLastTick = (((unsigned __int64)dwHighBit) << 32) + dwLowBit;

	return llLastTick;
}

float logfab(float a, float b)
{
	return logf(b) / logf(a);
}

//构造函数
CTableFrameSink::CTableFrameSink()
{
	m_pITableFrame=NULL;
	m_pGameServiceOption=NULL;
	m_pGameServiceAttrib=NULL;
	m_Config.nExchange=1;
	m_dwFishID=0;
	m_dwBulletID=0;
	m_bFishingOffSeason=false;
	m_bNoFish=false;
	m_bManEatingFish=false;
	m_Config.nAddScore=0;
	m_nLittleRevenue=0;
	m_nScoreOut=0;
	m_nSceneIndex=0;
	m_nTimeChangeScene=300;
	m_nPlotNumber=0;
	m_nChangeSceneElapse=0;
	m_nBossScore=3;
	m_nBossComingCount=0;
	m_nManEatingFishTime=0;
	m_nRoomType=0;

	ZeroMemory(&m_Config,sizeof(m_Config));
	ZeroMemory(&m_nPlotTime,sizeof(m_nPlotTime));
	ZeroMemory(&m_ManEatingFish,sizeof(m_ManEatingFish));
	ZeroMemory(&m_FishPackBoss,sizeof(m_FishPackBoss));

	m_vFishInfo.clear();
	m_vPlotInfo.clear();
AllocConsole();
freopen("CONOUT$","w+t",stdout);

	return;
}

//析构函数
CTableFrameSink::~CTableFrameSink()
{
	for (unsigned int i=0; i<m_vFishInfo.size(); i++)
	{
		delete m_vFishInfo[i];
	}
	m_vFishInfo.clear();
	for (unsigned int i=0; i<m_vPlotInfo.size(); i++)
	{
		delete m_vPlotInfo[i];
	}
	m_vPlotInfo.clear();
	for (unsigned int i=0; i<m_vNotice.size(); i++)
	{
		delete m_vNotice[i];
	}
	m_vNotice.clear();
}

//接口查询
VOID * CTableFrameSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITableFrameSink,Guid,dwQueryVer);
	QUERYINTERFACE(ITableUserAction,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITableFrameSink,Guid,dwQueryVer);
	return NULL;
}

//配置桌子
bool CTableFrameSink::Initialization(IUnknownEx * pIUnknownEx)
{
	//查询接口
	m_pITableFrame=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrame);

	//错误判断
	if (m_pITableFrame==NULL)
	{
		ERROR_LOG;
		return false;
	}

	//开始模式
	m_pITableFrame->SetStartMode(START_MODE_TIME_CONTROL);

	//游戏配置
	m_pGameServiceAttrib=m_pITableFrame->GetGameServiceAttrib();
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();

	//设置文件名
	std::string str;
	FILE * f=NULL;
	TCHAR szPath[MAX_PATH]={0};
	_sntprintf(szPath,sizeof(szPath),TEXT("FishConfig/%s.xml"),m_pGameServiceOption->szServerName);
//#ifdef _UNICODE
//	unicodeToAnsi(szPath,str);
//#else
	str=szPath;
//#endif;
	f=fopen(str.c_str(),"r");
	fclose(f);
	if (NULL==f)
	{
		AfxMessageBox(szPath);
		return false;
	}
	readXML(str.c_str());

	for (int i=0;i<GAME_PLAYER;i++)
	{
		m_PlayerInfo[i].Clear();
	}

	if (m_Config.nMinScore == 10)
	{
		m_nRoomType = 0;
	}
	else if (m_Config.nMinScore == 100)
	{
		m_nRoomType = 1;
	}
	else if (m_Config.nMinScore == 1000)
	{
		m_nRoomType = 2;
	}
	else if (m_Config.nMinScore == 10000)
	{
		m_nRoomType = 3;
	}

	if (m_pITableFrame->GetTableID() == 0)
	{
		char szFileName[64];
		sprintf(szFileName, "calcnew%d.log", m_pGameServiceOption->wServerID);
		fcalcnew = fopen(szFileName, "w+t");
	}

	m_pITableFrame->SetGameTimer(TimerCheckMatch,1000,TIMER_REPEAT_TIMER,0L);

	return true;
}

//游戏结束
bool CTableFrameSink::OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	//响应解散游戏，将玩家分数保存，防止掉分
	if (GER_DISMISS==cbReason)
	{
		IServerUserItem * pItemDismiss=NULL;
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			pItemDismiss=m_pITableFrame->GetTableUserItem(i);
			if (NULL!=pItemDismiss)
			{
				m_pITableFrame->PerformStandUpAction(pItemDismiss);
			}
		}
		return true;
	}

	return true;
}

//发送场景
bool CTableFrameSink::OnEventSendGameScene(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)
{
	switch (cbGameStatus)
	{
	case GAME_SCENE_FREE://空闲状态
	case GAME_SCENE_PLAY://游戏状态
		{
			//发送场景
			CMD_S_GameScene GameScene={0};
			GameScene.nSceneIndex=m_nSceneIndex;
			GameScene.nMaxBullet=m_Config.nMaxBullet;
			GameScene.lCellScore=m_Config.nCellScore;
			GameScene.lMaxMultiple=m_Config.nMaxScore;
			GameScene.nExercise=m_Config.nExercise;
			GameScene.bNoFish=m_bNoFish;

			//玩家数目
			WORD wUserCount=0;
			for (WORD wIndex=0;wIndex<GAME_PLAYER;++wIndex)
			{
				if (NULL!=m_pITableFrame->GetTableUserItem(wIndex))
				{
					if (!m_pITableFrame->GetTableUserItem(wIndex)->IsAndroidUser())
					{
						wUserCount++;
					}
				}
			}

			if (wUserCount<=1)
			{
				for (WORD wIndex=0;wIndex<GAME_PLAYER;++wIndex)
				{
					if (NULL!=m_pITableFrame->GetTableUserItem(wIndex))
					{
						if (m_pITableFrame->GetTableUserItem(wIndex)->IsAndroidUser())
						{
							m_PlayerInfo[wIndex].ClearBullet();
						}
					}
				}
			}

			for (int i=0; i<GAME_PLAYER; i++)
			{
				if (m_PlayerInfo[i].lUserCellScore<=0)
				{
					m_PlayerInfo[i].lUserCellScore=m_Config.nCellScore;
				}
				GameScene.lUserAllScore[i]=m_PlayerInfo[i].lUserAllScore;
				GameScene.lUserCellScore[i]=m_PlayerInfo[i].lUserCellScore;
				GameScene.cbCannonType[i]=m_PlayerInfo[i].cbCannonType;
			}

			tagPlayerInfo & playerInfo=m_PlayerInfo[wChairID];

			if (m_Config.nExercise==0)
			{
				CMD_S_EquipmentDragonCount EquipmentDragonCount={0};
				for (int i = 0; i < GAME_PLAYER; i++)
				{
					EquipmentDragonCount.nEquipmentDragonCount[i] = m_PlayerInfo[i].nEquipmentDragonCount;
				}
				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_EQUIPMENT_DRAGON_COUNT,&EquipmentDragonCount,sizeof(EquipmentDragonCount));
			}

			CMD_S_VipLevel VipLevel = {};
			for (int i = 0; i < GAME_PLAYER; i++)
			{
				IServerUserItem * pTemp = m_pITableFrame->GetTableUserItem(i);
				if (pTemp != NULL)
				{
					VipLevel.cbVipLevel[i] = pTemp->GetVipLevel();
				}
			}
			m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_VIP_LEVEL, &VipLevel, sizeof(VipLevel));

			m_pITableFrame->SendGameScene(pIServerUserItem,&GameScene,sizeof(GameScene));

			//修正剧情时和过海浪时出散鱼的问题
			if (!m_bNoFish && !m_bFishingOffSeason && !m_bManEatingFish)
			{
				plotNormal(pIServerUserItem->GetChairID());
			}

			//@@@
			if (m_bManEatingFish)
			{
				m_pITableFrame->SendTableData(pIServerUserItem->GetChairID(),SUB_S_TRACE_POINT,&m_FishPackBoss,sizeof(tagFishPack));
				SendManEatingFish();
			}

			return true;
		}
	}

	ERROR_LOG;
	return false;
}

//时间事件
bool CTableFrameSink::OnTimerMessage(DWORD wTimerID, WPARAM wBindParam)
{
	switch (wTimerID)
	{
	case TimerCheckUser:
		{
			for (int i=0;i<GAME_PLAYER;i++)
			{
				IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(i);
				tagPlayerInfo & PlayerInfo=m_PlayerInfo[i];
				if (pIServerUserItem!=NULL && pIServerUserItem->GetMemberOrder() != 5)
				{
					PlayerInfo.nOutTime++;
					if (PlayerInfo.nOutTime>=660)
					{
						OnActionUserOffLine(pIServerUserItem->GetChairID(),pIServerUserItem);
						//@debug
						//pIServerUserItem->SetUserStatus(US_FREE,pIServerUserItem->GetTableID(),pIServerUserItem->GetChairID());
						m_pITableFrame->PerformStandUpAction(pIServerUserItem);
					}
				}
			}
			return true;
		}
	case TimerSysMessage0:
		{
			WORD wChairID=(WORD)wBindParam;
			CMD_S_CaptureFish CaptureFish={0};
			CaptureFish.wChairID=wChairID;
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_CAPTURE_FISH,&CaptureFish,sizeof(CaptureFish));
			m_PlayerInfo[wChairID].bSuperCannon=false;
			return true;
		}
	case TimerSysMessage1:
		{
			WORD wChairID=(WORD)wBindParam;
			CMD_S_CaptureFish CaptureFish={0};
			CaptureFish.wChairID=wChairID;
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_CAPTURE_FISH,&CaptureFish,sizeof(CaptureFish));
			m_PlayerInfo[wChairID].bSuperCannon=false;
			return true;
		}
	case TimerSysMessage2:
		{
			WORD wChairID=(WORD)wBindParam;
			CMD_S_CaptureFish CaptureFish={0};
			CaptureFish.wChairID=wChairID;
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_CAPTURE_FISH,&CaptureFish,sizeof(CaptureFish));
			m_PlayerInfo[wChairID].bSuperCannon=false;
			return true;
		}
	case TimerSysMessage3:
		{
			WORD wChairID=(WORD)wBindParam;
			CMD_S_CaptureFish CaptureFish={0};
			CaptureFish.wChairID=wChairID;
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_CAPTURE_FISH,&CaptureFish,sizeof(CaptureFish));
			m_PlayerInfo[wChairID].bSuperCannon=false;
			return true;
		}
	case TimerSysMessage4:
		{
			WORD wChairID=(WORD)wBindParam;
			CMD_S_CaptureFish CaptureFish={0};
			CaptureFish.wChairID=wChairID;
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_CAPTURE_FISH,&CaptureFish,sizeof(CaptureFish));
			m_PlayerInfo[wChairID].bSuperCannon=false;
			return true;
		}
	case TimerSysMessage5:
		{
			WORD wChairID=(WORD)wBindParam;
			CMD_S_CaptureFish CaptureFish={0};
			CaptureFish.wChairID=wChairID;
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_CAPTURE_FISH,&CaptureFish,sizeof(CaptureFish));
			m_PlayerInfo[wChairID].bSuperCannon=false;
			return true;
		} 
	case TimerSysMessage10:
		{
			WORD wChairID=(WORD)wBindParam;
			m_PlayerInfo[wChairID].bBonus=false;
			return true;
		}
	case TimerSysMessage11:
		{
			WORD wChairID=(WORD)wBindParam;
			m_PlayerInfo[wChairID].bBonus=false;
			return true;
		}
	case TimerSysMessage12:
		{
			WORD wChairID=(WORD)wBindParam;
			m_PlayerInfo[wChairID].bBonus=false;
			return true;
		}
	case TimerSysMessage13:
		{
			WORD wChairID=(WORD)wBindParam;
			m_PlayerInfo[wChairID].bBonus=false;
			return true;
		}
	case TimerSysMessage14:
		{
			WORD wChairID=(WORD)wBindParam;
			m_PlayerInfo[wChairID].bBonus=false;
			return true;
		}
	case TimerSysMessage15:
		{
			WORD wChairID=(WORD)wBindParam;
			m_PlayerInfo[wChairID].bBonus=false;
			return true;
		}
	case TimerCreateFish:
		{
			if (!m_bFishingOffSeason && !m_bManEatingFish)
			{
				plotNormal();
				m_bNoFish=false;
			}
			//重置时间
			if (0!=wBindParam)
			{
				m_pITableFrame->SetGameTimer(TimerCreateFish,200,TIMER_REPEAT_TIMER,0L);
				m_nChangeSceneElapse=0;
				m_nBossComingCount=0;
			}
//printf("m_nBossComingCount is %d\n", m_nBossComingCount);
#ifndef KIND_7
			m_nBossComingCount++;
			//2分半，定时器每200毫秒触发一次，750次就是2分半
			//if (m_nBossComingCount == 750)
			//if (m_nBossComingCount == 100)
			if (m_nBossComingCount == 1800)
			{
//printf("SUB_S_BOSS_COMING\n");
				m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_BOSS_COMING, NULL, 0);
				
				int nPathIndex = 134 + rand() % 4;
				tagFishPack * pFishPack=fillFishPack(18,nPathIndex,500);
				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_TRACE_POINT,pFishPack,sizeof(tagFishPack));
				CopyMemory(&m_FishPackBoss, pFishPack, sizeof(tagFishPack));
				m_FishPackBoss.nCreateDelayTime = 0;

				//@@@
				m_bManEatingFish = true;
				m_nManEatingFishTime = 0;
				ZeroMemory(&m_ManEatingFish,sizeof(m_ManEatingFish));
				m_pITableFrame->SetGameTimer(TimerManEatingFish,1000,TIMER_REPEAT_TIMER,0L);
			}
#endif

			return true;
		}
	case TimerManEatingFish://@@@
		{
			if (!m_bManEatingFish)
			{
				m_pITableFrame->KillGameTimer(TimerManEatingFish);
				return true;
			}

			m_nManEatingFishTime++;

			CMD_S_ManEaingFishSpeak ManEatingFishSpeak = {};
			ManEatingFishSpeak.wElapse = 5;
			switch (m_nManEatingFishTime)
			{
			case 7+1:
				strcpy(ManEatingFishSpeak.szContent, "小的们！人鱼女王在召唤！开始抵御人类、至死方休！");
				m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_MAN_EATING_FISH_SPEAK, &ManEatingFishSpeak, sizeof(ManEatingFishSpeak));
				break;
			case 7+10:
				strcpy(ManEatingFishSpeak.szContent, "小的们！啃食他们的炮台，夺取金币让你们更强大！");
				m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_MAN_EATING_FISH_SPEAK, &ManEatingFishSpeak, sizeof(ManEatingFishSpeak));
				break;
			case 7+30:
				strcpy(ManEatingFishSpeak.szContent, "小的们！你们如果死了，人类会夺回他们的金币！");
				m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_MAN_EATING_FISH_SPEAK, &ManEatingFishSpeak, sizeof(ManEatingFishSpeak));
				break;
			case 7+40:
				strcpy(ManEatingFishSpeak.szContent, "小的们! 如果我死了，你们谁也活不了！");
				m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_MAN_EATING_FISH_SPEAK, &ManEatingFishSpeak, sizeof(ManEatingFishSpeak));
				break;
			case 7+50:
				strcpy(ManEatingFishSpeak.szContent, "你们这些人类，不用技能，休想打到我！");
				m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_MAN_EATING_FISH_SPEAK, &ManEatingFishSpeak, sizeof(ManEatingFishSpeak));
				break;
			case 7+60:
				strcpy(ManEatingFishSpeak.szContent, "女王巡游即将结束！人类此次必将空手而归！");
				ManEatingFishSpeak.wElapse = 15;
				m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_MAN_EATING_FISH_SPEAK, &ManEatingFishSpeak, sizeof(ManEatingFishSpeak));
				break;
			}

			if (m_nManEatingFishTime > 90)
			{
				m_pITableFrame->KillGameTimer(TimerManEatingFish);
				m_bManEatingFish = false;
				m_nManEatingFishTime = 0;

				m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_MAN_EATING_FISH_FINISH, NULL, 0);
				return true;
			}

			if (m_nManEatingFishTime == 7)
			{
				SendManEatingFish();
			}

			if (m_nManEatingFishTime < 9)
			{
				return true;
			}

			if (rand() % 10000 < 5000 && m_nManEatingFishTime % 8 == 0)
			{
				int nMultiple[] = {10, 20, 40};

				IServerUserItem * pIServerUserItem = m_pITableFrame->GetTableUserItem(0);
				if (pIServerUserItem != NULL)
				{
					WORD wChairID = 0;
					WORD wIndex = rand() % 3;
					int nEatScore = 100;

					int nEatScoreArray[4][3] =
					{
						{600, 1200, 2400},
						{1200, 2400, 4800},
						{6000, 12000, 24000},
						{12000, 24000, 48000},
					};

					int nOldSpriteID = m_ManEatingFish[wChairID].nSpriteID[wIndex];
					if (!m_ManEatingFish[wChairID].bIsDead[wIndex] && nOldSpriteID < 1029 && nOldSpriteID >= 1027)
					{
						nEatScore = nEatScoreArray[m_nRoomType][nOldSpriteID - 1027];
						if (m_PlayerInfo[wChairID].lUserAllScore < nEatScore)
						{
							nEatScore = m_PlayerInfo[wChairID].lUserAllScore;
						}
						m_PlayerInfo[wChairID].lUserAllScore -= nEatScore;

						int nNewSpriteID = nOldSpriteID + 1;
						GenManEatingFish(wChairID, wIndex, nNewSpriteID);
						m_ManEatingFish[wChairID].nEatScore[wIndex] += nEatScore;

						CMD_S_EatScore EatScore = {};
						EatScore.wChairID = wChairID;
						EatScore.wIndex = wIndex;
						EatScore.nEatScore = nEatScore;
						EatScore.nFishID = m_ManEatingFish[wChairID].nFishID[wIndex];
						EatScore.nSpriteID = m_ManEatingFish[wChairID].nSpriteID[wIndex];
						EatScore.nCurrentScore = m_PlayerInfo[wChairID].lUserAllScore;
						EatScore.nMultiple = nMultiple[nNewSpriteID - 1027];
						m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_EAT_SCORE, &EatScore, sizeof(EatScore));
					}
					if (!m_ManEatingFish[wChairID].bIsDead[wIndex] && nOldSpriteID == 1029)
					{
						nEatScore = nEatScoreArray[m_nRoomType][nOldSpriteID - 1027];
						if (m_PlayerInfo[wChairID].lUserAllScore < nEatScore)
						{
							nEatScore = m_PlayerInfo[wChairID].lUserAllScore;
						}
						m_PlayerInfo[wChairID].lUserAllScore -= nEatScore;
						m_ManEatingFish[wChairID].nEatScore[wIndex] += nEatScore;
						m_ManEatingFish[wChairID].bIsDead[wIndex] = true;

						CMD_S_EatScore EatScore = {};
						EatScore.wChairID = wChairID;
						EatScore.wIndex = wIndex;
						EatScore.nEatScore = nEatScore;
						EatScore.nFishID = m_ManEatingFish[wChairID].nFishID[wIndex];
						EatScore.nSpriteID = m_ManEatingFish[wChairID].nSpriteID[wIndex];
						EatScore.nCurrentScore = m_PlayerInfo[wChairID].lUserAllScore;
						m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_EAT_SCORE, &EatScore, sizeof(EatScore));

						GenManEatingFish(wChairID, wIndex, 1027);
						CMD_S_ManEatingFishNewFish ManEatingFishNewFish = {};
						ManEatingFishNewFish.wChairID = wChairID;
						ManEatingFishNewFish.nFishID = m_ManEatingFish[wChairID].nFishID[wIndex];
						ManEatingFishNewFish.nSpriteID = 1027;
						ManEatingFishNewFish.wIndex = wIndex;
						ManEatingFishNewFish.nCurrentScore = m_PlayerInfo[wChairID].lUserAllScore;
						ManEatingFishNewFish.nMultiple = 10;
						m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_EAT_3RD, &ManEatingFishNewFish, sizeof(ManEatingFishNewFish));

						m_ManEatingFish[wChairID].nFishID[wIndex] = 0;
						m_ManEatingFish[wChairID].nSpriteID[wIndex] = 0;
						m_ManEatingFish[wChairID].bIsDead[wIndex] = true;
					}
				}
				pIServerUserItem = m_pITableFrame->GetTableUserItem(1);
				if (pIServerUserItem != NULL)
				{
					WORD wChairID = 1;
					WORD wIndex = rand() % 3;
					int nEatScore = 100;

					int nEatScoreArray[4][3] =
					{
						{600, 1200, 2400},
						{1200, 2400, 4800},
						{6000, 12000, 24000},
						{12000, 24000, 48000},
					};

					int nOldSpriteID = m_ManEatingFish[wChairID].nSpriteID[wIndex];
					if (!m_ManEatingFish[wChairID].bIsDead[wIndex] && nOldSpriteID < 1029 && nOldSpriteID >= 1027)
					{
						nEatScore = nEatScoreArray[m_nRoomType][nOldSpriteID - 1027];
						if (m_PlayerInfo[wChairID].lUserAllScore < nEatScore)
						{
							nEatScore = m_PlayerInfo[wChairID].lUserAllScore;
						}
						m_PlayerInfo[wChairID].lUserAllScore -= nEatScore;

						int nNewSpriteID = nOldSpriteID + 1;
						GenManEatingFish(wChairID, wIndex, nNewSpriteID);
						m_ManEatingFish[wChairID].nEatScore[wIndex] += nEatScore;

						CMD_S_EatScore EatScore = {};
						EatScore.wChairID = wChairID;
						EatScore.wIndex = wIndex;
						EatScore.nEatScore = nEatScore;
						EatScore.nFishID = m_ManEatingFish[wChairID].nFishID[wIndex];
						EatScore.nSpriteID = m_ManEatingFish[wChairID].nSpriteID[wIndex];
						EatScore.nCurrentScore = m_PlayerInfo[wChairID].lUserAllScore;
						EatScore.nMultiple = nMultiple[nNewSpriteID - 1027];
						m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_EAT_SCORE, &EatScore, sizeof(EatScore));
					}
					if (!m_ManEatingFish[wChairID].bIsDead[wIndex] && nOldSpriteID == 1029)
					{
						nEatScore = nEatScoreArray[m_nRoomType][nOldSpriteID - 1027];
						if (m_PlayerInfo[wChairID].lUserAllScore < nEatScore)
						{
							nEatScore = m_PlayerInfo[wChairID].lUserAllScore;
						}
						m_PlayerInfo[wChairID].lUserAllScore -= nEatScore;
						m_ManEatingFish[wChairID].nEatScore[wIndex] += nEatScore;
						m_ManEatingFish[wChairID].bIsDead[wIndex] = true;

						CMD_S_EatScore EatScore = {};
						EatScore.wChairID = wChairID;
						EatScore.wIndex = wIndex;
						EatScore.nEatScore = nEatScore;
						EatScore.nFishID = m_ManEatingFish[wChairID].nFishID[wIndex];
						EatScore.nSpriteID = m_ManEatingFish[wChairID].nSpriteID[wIndex];
						EatScore.nCurrentScore = m_PlayerInfo[wChairID].lUserAllScore;
						m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_EAT_SCORE, &EatScore, sizeof(EatScore));

						GenManEatingFish(wChairID, wIndex, 1027);
						CMD_S_ManEatingFishNewFish ManEatingFishNewFish = {};
						ManEatingFishNewFish.wChairID = wChairID;
						ManEatingFishNewFish.nFishID = m_ManEatingFish[wChairID].nFishID[wIndex];
						ManEatingFishNewFish.nSpriteID = 1027;
						ManEatingFishNewFish.wIndex = wIndex;
						ManEatingFishNewFish.nCurrentScore = m_PlayerInfo[wChairID].lUserAllScore;
						ManEatingFishNewFish.nMultiple = 10;
						m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_EAT_3RD, &ManEatingFishNewFish, sizeof(ManEatingFishNewFish));
					}
				}
			}

			return true;
		}
	case TimerClearTrace://销毁轨迹
		{
			tagFishPack * pFishPack=NULL;
			INT_PTR nEnumIndex=0;
			do
			{
				pFishPack=m_FishFactory.EnumActiveObject(nEnumIndex++);
				//枚举鱼群
				if (NULL==pFishPack) break;
				DWORD dwNowTime=GetTickCount();
				if ((pFishPack->nCreateTime+FISH_ALIVE_TIME+pFishPack->nCreateDelayTime)<=dwNowTime)
				{
					m_FishFactory.FreeItem(pFishPack);
					continue;
				}
			}while(true);
			return true;
		}
	case TimerChangeScene://切换场景
		{
			//printf("TimerChangeScene\n");
			m_nChangeSceneElapse=0;
			m_nBossComingCount = 0;

			m_pITableFrame->KillGameTimer(TimerCreateFish);
			m_pITableFrame->SetGameTimer(TimerRegularFish,10000L,1,0L);

			CMD_S_ChangeScene ChangeScene={0};
			ChangeScene.SceneIndex=m_nSceneIndex;
			//if (m_nSceneIndex == 1 || m_nSceneIndex == 4 || m_nSceneIndex == 7)
			//{
			//	ChangeScene.RmoveID=1;
			//}
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_CHANGE_SCENE,&ChangeScene,sizeof(CMD_S_ChangeScene));

			m_bFishingOffSeason=true;

			return true;
		}
	case TimerRegularFish:
		{
			//printf("TimerRegularFish\n");
			m_nChangeSceneElapse=0;

			m_pITableFrame->KillGameTimer(TimerRegularFish);
			plotSpecial(m_nSceneIndex);

			m_nSceneIndex++;
			if (m_nSceneIndex>=m_nPlotNumber) m_nSceneIndex=0;

			return true;
		}
	case TimerCheckMatch:
		{
			//更新BOSS分数
			{
				CMD_S_UpdateBossScore UpdateBossScore={0};
				UpdateBossScore.nBossScore=m_nBossScore;
				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_UPDATE_BOSS_SCORE,&UpdateBossScore,sizeof(CMD_S_UpdateBossScore));
			}

			for (int i = 0; i < GAME_PLAYER; i++)
			{
				tagPlayerInfo & PlayerInfo=m_PlayerInfo[i];
				for (int j=0; j < CountArray(PlayerInfo.BulletInfo); j++)
				{
					if (PlayerInfo.BulletInfo[j].bHitMore)
					{
						if (_GetTickCount64() - PlayerInfo.BulletInfo[j].lTick > 1000)
						{
							//printf("clear bullet\n");
							PlayerInfo.BulletInfo[j].bHave=false;
							PlayerInfo.BulletInfo[j].bHitMore=false;
						}
					}
				}
			}

			//printf("m_nChangeSceneElapse=%d\n",m_nChangeSceneElapse);
			m_nChangeSceneElapse++;
#ifndef KIND_7
			m_MatchManager.CheckMatchConfig();

			//存比赛得分
			for (int i=0; i<GAME_PLAYER; i++)
			{
				tagPlayerInfo & PlayerInfo = m_PlayerInfo[i];
				if (GetTickCount() - PlayerInfo.nSitTime < 60 * 1000) continue;//不足1分钟
				PlayerInfo.nSitTime = GetTickCount();

				IServerUserItem * pIServerUserItem = m_pITableFrame->GetTableUserItem(i);
				if (pIServerUserItem == NULL) continue;

				for (int j = 0; j < (int)PlayerInfo.vMatchInfo.size(); j++)
				{
					int nType = PlayerInfo.vMatchInfo[j].nFlag;
					SCORE lCount = PlayerInfo.vMatchInfo[j].lWinScore;

					//m_pITableFrame->SendMatchScore(pIServerUserItem,PlayerInfo.vMatchInfo[j].lWinScore);

					if (0 == lCount) continue;

					SaveUserItem(pIServerUserItem,
						nType,
						eUserItemFlag_Heap | eUserItemFlag_GamePreLoad | eUserItemFlag_OverWrite | eUserItemFlag_Match,
						lCount, true);
				}
			}

			__time64_t ct;  
			__int64 nCurrentTime = _time64(&ct);

			//发送比赛公告
			for (int i = 0; i < (int)m_MatchManager.m_vMatchConfig.size() && m_pITableFrame->GetTableID() == 0; i++)
			{
				if (0 == m_MatchManager.m_vMatchConfig[i].m_nSendMatchInfoElapse)
				{
					continue;
				}

				m_MatchManager.CheckMatchConfig();

				if (!m_MatchManager.m_vMatchConfig[i].bInMatch)
				{
					continue;
				}

				if (nCurrentTime < m_MatchManager.m_vMatchConfig[i].m_nLastSendMatchInfoTime + m_MatchManager.m_vMatchConfig[i].m_nSendMatchInfoElapse)
				{
					continue;
				}

				if (0 == m_MatchManager.m_vMatchConfig[i].m_sSendMatchInfo.length())
				{
					continue;
				}

				LPCTSTR strFilePath=NULL;
//#ifdef _UNICODE
//				std::wstring str;
//				utf8ToUnicode(m_MatchManager.m_vMatchConfig[i].m_sSendMatchInfo.c_str(),str);
//				strFilePath=str.c_str();
//#else
				strFilePath=m_MatchManager.m_vMatchConfig[i].m_sSendMatchInfo.c_str();
//#endif
				m_pITableFrame->SendRoomMessage(strFilePath,SMT_CHAT|SMT_CATCH_FISH);
				m_MatchManager.m_vMatchConfig[i].m_nLastSendMatchInfoTime = nCurrentTime;
			}
#endif
			return true;
		}
	}
	return false;
}

//游戏消息
bool CTableFrameSink::OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	switch (wSubCmdID)
	{
	case SUB_C_BUY_BULLET://购买子弹
		{
			if (wDataSize!=sizeof(CMD_C_BuyBullet))
			{
				ERROR_LOG;
				return false;
			}
			CMD_C_BuyBullet * pBuyBullet=(CMD_C_BuyBullet *)pData;

			return OnSubBuyBullet(pIServerUserItem,pBuyBullet->bAdd,pBuyBullet->lScore);
		}
	case SUB_C_USER_SHOOT:
		{
			if (wDataSize!=sizeof(CMD_C_UserShoot))
			{
				ERROR_LOG;
				return false;
			}
			CMD_C_UserShoot * pUserShoot=(CMD_C_UserShoot *)pData;

			return OnSubFire(pIServerUserItem,pUserShoot->fAngle,pUserShoot->dwBulletID,(int)pUserShoot->dwCount,pUserShoot->cbAttrib);
		}
	case SUB_C_HIT_FISH:
		{
			return OnSubHitFish(pIServerUserItem,pData,wDataSize);
		}
	case SUB_C_BUY_BULLETSPEED:
		{
			if (wDataSize!=sizeof(CMD_C_CoinCount))
			{
				ERROR_LOG;
				return false;
			}
			CMD_C_CoinCount * pHitFish=(CMD_C_CoinCount *)pData;

			CMD_S_CoinCount UserShoot={0};
			UserShoot.wChairID=pIServerUserItem->GetChairID();
			UserShoot.lCoinCount=pHitFish->lCoinCount;
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_COIN_COUNT,&UserShoot,sizeof(UserShoot));

			return true;
		}
	case SUB_C_CANNON_MULTIPLE://设置炮的倍率
		{
			if (wDataSize!=sizeof(CMD_C_CannonMultiple))
			{
				ERROR_LOG;
				return false;
			}
			CMD_C_CannonMultiple * pCannonMultiple=(CMD_C_CannonMultiple *)pData;

			return OnSubCannonMultiple(pIServerUserItem,pCannonMultiple->cbAddMultiple,pCannonMultiple->cbCannonType);
		}
	case SUB_C_LOCKFISH://锁鱼
		{
			if (wDataSize<sizeof(CMD_C_LockFishId)-MAX_PATH+1)
			{
				ERROR_LOG;
				return false;
			}
			CMD_C_LockFishId * pLockFishId=(CMD_C_LockFishId *)pData;

			if (pLockFishId->wChairID!=pIServerUserItem->GetChairID())
			{
				if (m_pITableFrame->GetTableUserItem(pLockFishId->wChairID)!=NULL)
				{
					if (!m_pITableFrame->GetTableUserItem(pLockFishId->wChairID)->IsAndroidUser())
						return true;
				}
				else
				{
					return true;
				}
				if (rand()%10000>=m_Config.nAndroidLock) return true;
			}

			CMD_S_LockFishId lockFish={0};
			lockFish.iChairID=pLockFishId->wChairID;
			lockFish.lFishID=pLockFishId->lFishID;
			return m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_LOCKFISH,&lockFish,sizeof(lockFish));
		}
	case SUB_C_SHARE_INFO:
		{
			if (pIServerUserItem==NULL || pIServerUserItem->GetChairID()>=GAME_PLAYER)
			{
				ERROR_LOG;
				return true;
			}

			if (wDataSize != sizeof(CMD_C_ShareInfo))
			{
				ERROR_LOG;
				return true;
			}

			CMD_C_ShareInfo * pShareInfo = (CMD_C_ShareInfo *)pData;

			if (pShareInfo->nShareReason != eShareMobileMoney) return true;

			WORD wChairID = pIServerUserItem->GetChairID();
			tagPlayerInfo & PlayerInfo = m_PlayerInfo[wChairID];

			switch (pShareInfo->nShareAction)
			{
			case eShareClick:
				{
					//printf("eShareClick\n");
					break;
				}
			case eShareSuccess:
				{
					//printf("eShareSuccess\n");

					if (PlayerInfo.wMobileMoneyWill <= 0) return true;

					//对于老版本的玩家，分享走这里，话费在这里增加
					PlayerInfo.wMobileMoneyGeted += PlayerInfo.wMobileMoneyWill;
					m_pITableFrame->SaveUserItemCount(pIServerUserItem, eUserItem_MobileMoney, (SCORE)PlayerInfo.wMobileMoneyGeted);

					//赠送的话费清0
					PlayerInfo.wMobileMoneyGift = 0;
					SaveUserItem(pIServerUserItem,
						eUserItem_MobileGift,
						eUserItemFlag_Heap|eUserItemFlag_GamePreLoad|eUserItemFlag_OverWrite,
						PlayerInfo.wMobileMoneyGift,false);

					//获奖记录
					TCHAR szRecord[1024]={0};
					sprintf(szRecord, "{\"MobileMoney\":%hu}", PlayerInfo.wMobileMoneyWill);
					m_pITableFrame->SaveGameJson(pIServerUserItem,szRecord,0);

					m_pITableFrame->SendTableData(wChairID, SUB_S_MOBILE_MONEY_SUCCESS);

					//发送公告
					CString szMessage;
					//szMessage.Format("玩家[%s]捕鱼技艺高超，轻松打到[%d]元话费，大家加油啊。", pIServerUserItem->GetNickName(), PlayerInfo.wMobileMoneyWill);
					szMessage.Format("玩家[%s]捕鱼技艺高超，轻松打到手机话费，大家加油啊。", pIServerUserItem->GetNickName());
					m_pITableFrame->SendRoomMessage(szMessage,SMT_CHAT|SMT_CATCH_FISH);

					PlayerInfo.wMobileMoneyWill = 0;

					break;
				}
			case eShareFail:
				{
					//printf("eShareFail\n");
					break;
				}
			case eShareCancel:
				{
					//赠送的话费清0
					PlayerInfo.wMobileMoneyGift = 0;
					SaveUserItem(pIServerUserItem,
						eUserItem_MobileGift,
						eUserItemFlag_Heap|eUserItemFlag_GamePreLoad|eUserItemFlag_OverWrite,
						PlayerInfo.wMobileMoneyGift,false);

					PlayerInfo.wShareCancelCount++;
					SaveUserItem(pIServerUserItem,
						eUserItem_ShareCancel,
						eUserItemFlag_Heap|eUserItemFlag_GamePreLoad|eUserItemFlag_OverWrite,
						PlayerInfo.wShareCancelCount,false);

					//printf("eShareCancel\n");
					break;
				}
			}

			return true;
		}
	case SUB_C_GET_MOBILE_MONEY:
		{
			if (pIServerUserItem==NULL || pIServerUserItem->GetChairID()>=GAME_PLAYER)
			{
				ERROR_LOG;
				return true;
			}

			m_pITableFrame->GetUserItemCount(pIServerUserItem, eUserItem_MobileMoney);

			return true;
		}
	case SUB_C_MOBILE_CHARGE:
		{
			if (pIServerUserItem==NULL || pIServerUserItem->GetChairID()>=GAME_PLAYER)
			{
				ERROR_LOG;
				return true;
			}

			if (wDataSize != sizeof(CMD_C_MobileCharge))
			{
				ERROR_LOG;
				return true;
			}

			CMD_C_MobileCharge * pMobileCharge = (CMD_C_MobileCharge *)pData;

			WORD wChairID = pIServerUserItem->GetChairID();
			tagPlayerInfo & PlayerInfo = m_PlayerInfo[wChairID];

			CMD_S_MobileCharge MobileCharge = {0};
			if (PlayerInfo.wMobileMoneyGeted >= pMobileCharge->wMoneyCount && pMobileCharge->wMoneyCount != 0)
			{
				PlayerInfo.wMobileMoneyGeted -= pMobileCharge->wMoneyCount;
				m_pITableFrame->SaveUserItemCount(pIServerUserItem, eUserItem_MobileMoney, (SCORE)PlayerInfo.wMobileMoneyGeted);
				//保存要充值的数量
				SaveUserItem(pIServerUserItem,
					eUserItem_MobileCharge,
					eUserItemFlag_Heap,
					pMobileCharge->wMoneyCount,false);
				MobileCharge.bSuccess = true;
				MobileCharge.wMoneyCurrent = PlayerInfo.wMobileMoneyGeted;

				CString str;
				str.Format("#ffffff恭喜【#ff0000%s#ffffff】成功的兑换了#ff0000%d元话费#ffffff！",pIServerUserItem->GetNickName(),pMobileCharge->wMoneyCount);
				m_pITableFrame->SendPlatformMessage(str, 20);
			}
			else
			{
				MobileCharge.bSuccess = false;
				MobileCharge.wMoneyCurrent = PlayerInfo.wMobileMoneyGeted;
			}

			m_pITableFrame->SendTableData(wChairID, SUB_S_MOBILE_CHARGE, &MobileCharge, sizeof(MobileCharge));

			return true;
		}
	case SUB_C_EXCHANGE_PEARL:
		{
			if (pIServerUserItem==NULL || pIServerUserItem->GetChairID()>=GAME_PLAYER)
			{
				ERROR_LOG;
				return true;
			}

			if (wDataSize != sizeof(CMD_C_ExchangePearl))
			{
				ERROR_LOG;
				return true;
			}

			CMD_C_ExchangePearl * pExchangePearl = (CMD_C_ExchangePearl *)pData;

			SCORE lNeedScore = 0;
			switch (pExchangePearl->nPearlType)
			{
			case 0:
				{
					lNeedScore = (SCORE)pExchangePearl->nPearlCount * 100000;
					break;
				}
			case 1:
				{
					lNeedScore = (SCORE)pExchangePearl->nPearlCount * 200000;
					break;
				}
			case 2:
				{
					lNeedScore = (SCORE)pExchangePearl->nPearlCount * 500000;
					break;
				}
			default:
				{
					ERROR_LOG;
					return true;
				}
			}

			if (pIServerUserItem->GetUserScore() < lNeedScore)
			{
				CMD_S_ExchangePearl ExchangePearl = {};
				ExchangePearl.cbResult = 0;
				sprintf(ExchangePearl.szResult, "金币不足，兑换失败。");

				m_pITableFrame->SendTableData(pIServerUserItem->GetChairID(), SUB_S_EXCHANGE_PEARL, &ExchangePearl, sizeof(ExchangePearl));

				return true;
			}

			//身上必须保留5万
			if (pIServerUserItem->GetUserScore() - lNeedScore < 50000)
			{
				return true;
			}

			pIServerUserItem->WriteUserScore(-lNeedScore,0,0,0,SCORE_TYPE_LOSE,0);
			m_pITableFrame->WinPearl(pIServerUserItem, pExchangePearl->nPearlType, pExchangePearl->nPearlCount);

			return true;
		}
	case 216:
		{
			//@debug
			//pIServerUserItem->SetUserStatus(US_FREE,pIServerUserItem->GetTableID(),pIServerUserItem->GetChairID());
			m_pITableFrame->PerformStandUpAction(pIServerUserItem);
			return true;
		}
	case 217://退出游戏
		{
			//玩家数目
			WORD wUserCount=0;
			for (WORD wIndex=0;wIndex<GAME_PLAYER;++wIndex)
			{
				if (NULL!=m_pITableFrame->GetTableUserItem(wIndex))
				{
					if (!m_pITableFrame->GetTableUserItem(wIndex)->IsAndroidUser())
					{
						wUserCount++;
					}
				}
			}

			if (wUserCount<=1)
			{
				for (WORD wIndex=0;wIndex<GAME_PLAYER;++wIndex)
				{
					if (NULL!=m_pITableFrame->GetTableUserItem(wIndex))
					{
						if (m_pITableFrame->GetTableUserItem(wIndex)->IsAndroidUser())
						{
							m_PlayerInfo[wIndex].ClearBullet();
						}
					}
				}
			}
			return true;
		}
	case SUB_C_TRY_CANNON:
		{
#ifndef KIND_7
			WORD wChairID = pIServerUserItem->GetChairID();
			tagPlayerInfo & playerInfo = m_PlayerInfo[wChairID];
			if (playerInfo.lUserTryScore <= 1)
			{
				playerInfo.bTryCannon = true;
				playerInfo.lUserTryScore = 0;
				m_pITableFrame->SendTableData(wChairID, SUB_S_TRY_CANNON, NULL, 0);
			}
#endif
			return true;
		}
	case SUB_C_NEW_GUID_GIFT:
		{
			m_pITableFrame->NewGuidGift(pIServerUserItem);
			return true;
		}
	case SUB_C_TRY_CANNON_CHECK:
		{
#ifndef KIND_7
			WORD wChairID = pIServerUserItem->GetChairID();
			tagPlayerInfo & playerInfo = m_PlayerInfo[wChairID];
			CMD_S_TryCannonCheck TryCannonCheck = {0};
			if (playerInfo.lUserTryScore == 0 && !IsSignUpMatch(pIServerUserItem))
			{
				playerInfo.lUserTryScore = 1;

				SaveUserItem(pIServerUserItem,
					eUserItemTryScore,
					eUserItemFlag_Heap|eUserItemFlag_GamePreLoad|eUserItemFlag_OverWrite,
					playerInfo.lUserTryScore,false);

				TryCannonCheck.bCan = true;
			}
			else
			{
				TryCannonCheck.bCan = false;
			}

			m_pITableFrame->SendTableData(wChairID, SUB_S_TRY_CANNON_CHECK, &TryCannonCheck, sizeof(TryCannonCheck));
#endif
			return true;
		}
	case SUB_C_ADD_BOX_PRIZE:
		{
			if (wDataSize != 0)
			{
				return true;
			}

			//判断椅子号
			WORD wChairID=pIServerUserItem->GetChairID();
			if (wChairID>=GAME_PLAYER)
			{
				ERROR_LOG;
				return true;
			}

			//玩家信息
			tagPlayerInfo & PlayerInfo=m_PlayerInfo[wChairID];
			int nAddScore = 0;
			if (PlayerInfo.qBoxPrize.empty())
			{
				nAddScore = 0;	
			}
			else
			{
				nAddScore = PlayerInfo.qBoxPrize.front();
				PlayerInfo.qBoxPrize.pop();
			}

			PlayerInfo.lUserAllScore += nAddScore;

			// pack Send Data
			CMD_S_AddBoxRes stAddBoxRes = {0};
			stAddBoxRes.lUserAllScore = PlayerInfo.lUserAllScore;

			m_pITableFrame->SendTableData(wChairID,SUB_S_ADD_BOX_RES,&stAddBoxRes,sizeof(CMD_S_AddBoxRes));

			return true;
		}
	}

	return false;
}

//用户坐下
bool CTableFrameSink::OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	tagPlayerInfo & PlayerInfo=m_PlayerInfo[wChairID];

	//@debug
	//pIServerUserItem->SetUserStatus(US_PLAYING,pIServerUserItem->GetTableID(),pIServerUserItem->GetChairID());

	if (false==bLookonUser && m_pITableFrame->GetGameStatus()==GAME_SCENE_FREE)
	{
		m_pITableFrame->SetGameTimer(TimerClearTrace,TIME_CLEAR_TRACE*3000,TIMER_REPEAT_TIMER,0L);
		m_pITableFrame->SetGameTimer(TimerChangeScene,m_nTimeChangeScene*1000,TIMER_REPEAT_TIMER,1L);
		m_pITableFrame->SetGameTimer(TimerCreateFish,200,TIMER_REPEAT_TIMER,0L);
		//m_pITableFrame->SetGameTimer(TimerCheckUser,1000,TIMER_REPEAT_TIMER,0L);

		m_pITableFrame->SetGameStatus(GAME_SCENE_PLAY);
	}

	if (false==bLookonUser)
	{
		PlayerInfo.Clear();
		PlayerInfo.nSitTime=PlayerInfo.nSendTime=PlayerInfo.nGameTime=GetTickCount();
		PlayerInfo.lUserCellScore=m_Config.nCellScore;

		//读入游戏时长和总赢分
		int nUserItemCount=pIServerUserItem->GetUserItemCount();
		tagUserItem * pUserItem=pIServerUserItem->GetUserItems();
		for (int i=0; i<nUserItemCount; i++)
		{
			tagUserItem * pTmpUserItem=pUserItem+i;

			if (pTmpUserItem->nType==eUserItem_ElapseTime)
			{
				PlayerInfo.nElapseTime=(int)pTmpUserItem->lCount;
				break;
			}
		}
		for (int i=0; i<nUserItemCount; i++)
		{
			tagUserItem * pTmpUserItem=pUserItem+i;

			if (pTmpUserItem->nType==eUserItem_TotalWinScore)
			{
				PlayerInfo.nTotalWinScore=(int)pTmpUserItem->lCount;
				break;
			}
		}
		for (int i=0; i<nUserItemCount; i++)
		{
			tagUserItem * pTmpUserItem=pUserItem+i;

			if (pTmpUserItem->nType==eUserItem_LotteryInfo)
			{
				PlayerInfo.nLotteryGeted=(int)pTmpUserItem->lCount;
				break;
			}
		}
		for (int i=0; i<nUserItemCount; i++)
		{
			tagUserItem * pTmpUserItem=pUserItem+i;

			if (pTmpUserItem->nType==eUserItem_LotteryGift)
			{
				PlayerInfo.nLotteryGift=(int)pTmpUserItem->lCount;
				break;
			}
		}
		for (int i=0; i<nUserItemCount; i++)
		{
			tagUserItem * pTmpUserItem=pUserItem+i;

			if (pTmpUserItem->nType==eUserItem_MobileMoney)
			{
				PlayerInfo.wMobileMoneyGeted=(int)pTmpUserItem->lCount;
				break;
			}
		}
		for (int i=0; i<nUserItemCount; i++)
		{
			tagUserItem * pTmpUserItem=pUserItem+i;

			if (pTmpUserItem->nType==eUserItem_MobileGift)
			{
				PlayerInfo.wMobileMoneyGift=(int)pTmpUserItem->lCount;
				printf("PlayerInfo.wMobileMoneyGift=%hu\n",PlayerInfo.wMobileMoneyGift);
				break;
			}
		}
		for (int i=0; i<nUserItemCount; i++)
		{
			tagUserItem * pTmpUserItem=pUserItem+i;

			if (pTmpUserItem->nType==eUserItem_MobileMoneyTime)
			{
				PlayerInfo.nMobileMoneyTime=(int)pTmpUserItem->lCount;
				break;
			}
		}
		for (int i=0; i<nUserItemCount; i++)
		{
			tagUserItem * pTmpUserItem=pUserItem+i;

			if (pTmpUserItem->nType==eUserItem_NewPlayerMobileMoney)
			{
				PlayerInfo.wNewPlayerMobileMoney=(WORD)pTmpUserItem->lCount;
				break;
			}
		}
		for (int i=0; i<nUserItemCount; i++)
		{
			tagUserItem * pTmpUserItem=pUserItem+i;

			if (pTmpUserItem->nType==eUserItem_ShareCancel)
			{
				PlayerInfo.wShareCancelCount=(int)pTmpUserItem->lCount;
				break;
			}
		}
		for (int i=0; i<nUserItemCount; i++)
		{
			tagUserItem * pTmpUserItem=pUserItem+i;

			if (pTmpUserItem->nType==eUserItem_EquipmentDragonCount)
			{
				PlayerInfo.nEquipmentDragonCount=(int)pTmpUserItem->lCount;
				break;
			}
		}
		for (int i=0; i<nUserItemCount; i++)
		{
			tagUserItem * pTmpUserItem=pUserItem+i;

			if (pTmpUserItem->nType==eUserItemTryScore)
			{
				PlayerInfo.lUserTryScore=(int)pTmpUserItem->lCount;
				break;
			}
		}
		for (int i=0; i<nUserItemCount; i++)
		{
			tagUserItem * pTmpUserItem=pUserItem+i;
			if (pTmpUserItem->nType==eUserItemLastCannon)
			{
				PlayerInfo.cbCannonType=(int)pTmpUserItem->lCount;
				break;
			}
		}
		for (int i=0; i<nUserItemCount; i++)
		{
			tagUserItem * pTmpUserItem=pUserItem+i;
			if (pTmpUserItem->nType==eUserItem_nPointCount)
			{
				PlayerInfo.nPointCount=(int)pTmpUserItem->lCount;
				break;
			}
		}
		for (int i=0; i<nUserItemCount; i++)
		{
			tagUserItem * pTmpUserItem=pUserItem+i;
			if (pTmpUserItem->nType==eUserItem_nFireCount)
			{
				PlayerInfo.nFireCount=(int)pTmpUserItem->lCount;
				break;
			}
		}
		for (int i=0; i<nUserItemCount; i++)
		{
			tagUserItem * pTmpUserItem=pUserItem+i;
			if (pTmpUserItem->nType==eUserItem_lGains)
			{
				PlayerInfo.lGains=pTmpUserItem->lCount;
				break;
			}
		}
		for (int i=0; i<nUserItemCount; i++)
		{
			tagUserItem * pTmpUserItem=pUserItem+i;
			if (pTmpUserItem->nType==eUserItem_lConsume)
			{
				PlayerInfo.lConsume=pTmpUserItem->lCount;
				break;
			}
		}

		PlayerInfo.initCurve();

printf("读入 %d %d %lld %lld\n", PlayerInfo.nPointCount, PlayerInfo.nFireCount, PlayerInfo.lGains, PlayerInfo.lConsume);
		//比赛数据
		tagMatchInfo MatchInfo = {0};
		SYSTEMTIME st;
		GetLocalTime(&st);
		int nToday = st.wMonth * 100 + st.wDay;
		for (int i=0; i<nUserItemCount; i++)
		{
			tagUserItem * pTmpUserItem=pUserItem+i;
			//类型大于100000000，格式是316040899
			if (pTmpUserItem->nType>100000000)
			{
				int nFlagTemp = pTmpUserItem->nType % 1000000;//只留下040899的数据
				nFlagTemp /= 100;//只留下0408的数据
				if (nToday == nFlagTemp)
				{
					MatchInfo.nFlag = pTmpUserItem->nType;
					MatchInfo.lWinScore = pTmpUserItem->lCount;
				}
			}
		}
		if (MatchInfo.nFlag != 0)
		{
//printf("read match data %d %lld\n", MatchInfo.nFlag, MatchInfo.lWinScore);
			PlayerInfo.vMatchInfo.push_back(MatchInfo);
		}

		if (PlayerInfo.nPassTime==0)
		{
			PlayerInfo.nPassTime=_time64(NULL);
			PlayerInfo.nNextLotteryTime=_time64(NULL) + TIME_LOTTERY;
		}

		if (PlayerInfo.nMobileMoneyTime == 0)
		{
			PlayerInfo.nMobileMoneyTime = time(NULL);
			SaveUserItem(pIServerUserItem,
				eUserItem_MobileMoneyTime,
				eUserItemFlag_Heap|eUserItemFlag_GamePreLoad|eUserItemFlag_OverWrite,
				PlayerInfo.nMobileMoneyTime,false);
		}

		//@练习房间，上分
		if (m_Config.nExercise>0)
		{
			CMD_S_BulletCount BulletCount={0};
			PlayerInfo.lUserAllScore=m_Config.nExercise;
			BulletCount.bAdd=true;
			BulletCount.lScore=PlayerInfo.lUserAllScore;
			BulletCount.wChairID=wChairID;
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_BULLET_COUNT,&BulletCount,sizeof(BulletCount));
		}

		if (m_Config.nExercise==0)
		{
			CMD_S_EquipmentDragonCount EquipmentDragonCount={0};
			for (int i = 0; i < GAME_PLAYER; i++)
			{
				EquipmentDragonCount.nEquipmentDragonCount[i] = m_PlayerInfo[i].nEquipmentDragonCount;
			}
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_EQUIPMENT_DRAGON_COUNT,&EquipmentDragonCount,sizeof(EquipmentDragonCount));
		}
	}

	if (m_pITableFrame->IsGameStarted()==false)
	{
		m_pITableFrame->StartGame();
	}

	saveGameJson(pIServerUserItem,PlayerInfo,false);

	CMD_S_CannonMultiple CannonMultiple={0};
	CannonMultiple.wChairID=wChairID;
	CannonMultiple.nCannonMultiple=PlayerInfo.lUserCellScore;
	CannonMultiple.cbCannonType=PlayerInfo.cbCannonType;
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_BONUS_INFO,&CannonMultiple,sizeof(CannonMultiple));

	return true;
}

//用户起立
bool CTableFrameSink::OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	calc(wChairID,pIServerUserItem,bLookonUser);

	return true;
}

//特殊剧情
void CTableFrameSink::plotSpecial(int number)
{
	m_bFishingOffSeason=false;
	BYTE cbBuffer[2048]={0};
	WORD wSendSize=0;
	tagFishPack * pFishTrace=(tagFishPack *)cbBuffer;

	for (unsigned int i=0; i<m_vPlotInfo.size(); i++)
	{
		tagPlotInfo * pPlotInfo=m_vPlotInfo[i];

		if (pPlotInfo->bNotNormal) continue;

		//鱼种类
		int nSpriteID=pPlotInfo->nFishIDBegin+rand()%pPlotInfo->nFishIDRange;
		//路线索引
		int nPathIndex=pPlotInfo->nPathIndexBegin+rand()%pPlotInfo->nPathIndexRange;

		if (number==pPlotInfo->nType)
		{
			int nX=pPlotInfo->nParam0;
			int nY=pPlotInfo->nParam1;
			float fRotation=pPlotInfo->fRotation[0];
			int nMoveTime=pPlotInfo->nMoveTime[0];
			int nSpeed=pPlotInfo->nSpeed[0];
			int nCount=pPlotInfo->nParam2;
			int nR=pPlotInfo->nParam3;
			int nStartAngle=pPlotInfo->nParam4;
			int nTotalAngle=pPlotInfo->nParam5;
			for (int j=0; j<nCount; j++)
			{
				int nX2=nX;
				int nY2=nY;

				if (100==pPlotInfo->nFunc)
				{
					nX2+=(int)(nR*sin(D3DXToRadian(nStartAngle+nTotalAngle*j/nCount)));
					nY2+=(int)(-nR*cos(D3DXToRadian(nStartAngle+nTotalAngle*j/nCount)));
					if (pPlotInfo->nParam6>0)
						fRotation=(float)(nStartAngle+nTotalAngle*j/nCount);
				}

				if (101==pPlotInfo->nFunc && nCount>1)
				{
					nX2=nX-(pPlotInfo->nParam3)/2+pPlotInfo->nParam3*j/nCount;
					if (pPlotInfo->nParam4>0)
						nY2+=rand()%pPlotInfo->nParam4;
				}

				if (102==pPlotInfo->nFunc)
				{
					fRotation=360.0f*j/nCount;
				}

				tagFishPack * pFishPack=fillFishPack(nSpriteID,nPathIndex,pPlotInfo->nDelayTime+j*pPlotInfo->nSpaceTime);
				if (NULL!=pFishPack)
				{
					for (int k=0; k<PATH_POINT_COUNT; k++)
					{
						pFishPack->fRotation[k]=pPlotInfo->fRotation[k];
						pFishPack->nMoveTime[k]=pPlotInfo->nMoveTime[k];
						pFishPack->nSpeed[k]=pPlotInfo->nSpeed[k];
					}
					pFishPack->nXPos=nX2;
					pFishPack->nYPos=nY2;
					pFishPack->fRotation[0]=fRotation;
					pFishPack->nMoveTime[0]=nMoveTime;
					pFishPack->nSpeed[0]=nSpeed;
					CopyMemory(pFishTrace,pFishPack,sizeof(tagFishPack));
					wSendSize+=sizeof(tagFishPack);
					++pFishTrace;
					if (wSendSize+sizeof(tagFishPack)>sizeof(cbBuffer))
					{
						m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_TRACE_POINT,cbBuffer,wSendSize);
						pFishTrace=(tagFishPack *)cbBuffer;
						wSendSize=0;
					}
				}
			}
		}
	}

	if (wSendSize>0)
	{
		if (wSendSize>sizeof(cbBuffer))
			ERROR_LOG;
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_TRACE_POINT,cbBuffer,wSendSize);
	}
	m_bNoFish=true;

	if (number<CountArray(m_nPlotTime))
	{
		m_pITableFrame->SetGameTimer(TimerCreateFish,m_nPlotTime[number]*1000,TIMER_REPEAT_TIMER,1L);
		m_pITableFrame->SetGameTimer(TimerChangeScene,m_nPlotTime[number]*1000+m_nTimeChangeScene*1000,TIMER_REPEAT_TIMER,1L);
	}
}

//普通剧情
void CTableFrameSink::plotNormal(WORD wChairID)
{
	BYTE cbBuffer[2048]={0};
	WORD wSendSize=0;
	tagFishPack * pFishTrace=(tagFishPack *)cbBuffer;

	int nFastPlotCount = 0;

	bool bFast = (wChairID != INVALID_CHAIR);

	//小鱼群坐标偏移
	//CPoint ptGroup[]={CPoint(0,0),CPoint(10,50),CPoint(20,80),CPoint(-20,120),CPoint(-30,60),CPoint(-10,100),CPoint(-1,140)};
	CPoint ptGroup[]={CPoint(0,0),CPoint(20,100),CPoint(40,160),CPoint(-40,240),CPoint(-60,120),CPoint(-20,200),CPoint(-2,280)};

	for (unsigned int i=0; i<m_vPlotInfo.size(); i++)
	{
		tagPlotInfo * pPlotInfo=m_vPlotInfo[i];

		if (pPlotInfo->bNotNormal) continue;

		//鱼种类
		int nSpriteID=pPlotInfo->nFishIDBegin+rand()%pPlotInfo->nFishIDRange;
		//路线索引
		int nPathIndex=pPlotInfo->nPathIndexBegin+rand()%pPlotInfo->nPathIndexRange;

		//普通剧情
		if (-1==pPlotInfo->nType)
		{
			//时间间隔
			unsigned int dwNeedTick=(unsigned int)(pPlotInfo->fElapse*1000);
			unsigned int dwCurrentTick=GetTickCount();
//printf("dwNeedTick=%d dwCurrentTick=%d dwLastTick=%d result=%d\n",dwNeedTick,dwCurrentTick,pPlotInfo->dwLastTick,dwCurrentTick-pPlotInfo->dwLastTick);

			if (!bFast)
			{
				if (dwCurrentTick-pPlotInfo->dwLastTick<dwNeedTick)
				{
					continue;
				}
				else
				{
					pPlotInfo->dwLastTick=dwCurrentTick;
				}
			}

			if (bFast)
			{
				if (pPlotInfo->bGroupPos || nFastPlotCount>9)
				{
					continue;
				}
				nFastPlotCount++;
			}

			//生成概率
			int nPercent=rand()%100;
			//判断几率
			if (pPlotInfo->nPercent>0 && nPercent<pPlotInfo->nPercent)
			{
				int nCount=pPlotInfo->nExecuteCount;
				if (pPlotInfo->nExecuteRange>0)
					nCount+=rand()%pPlotInfo->nExecuteRange;
				for (int j=0; j<nCount; j++)
				{
					tagFishPack * pFishPack=fillFishPack(nSpriteID,nPathIndex,j+pPlotInfo->nDelayTime+j*pPlotInfo->nSpaceTime);
					if (NULL!=pFishPack)
					{
						if (pPlotInfo->bGroupPos)
						{
							pFishPack->nXPos=ptGroup[j%CountArray(ptGroup)].x;
							pFishPack->nYPos=ptGroup[j%CountArray(ptGroup)].y;
						}
						CopyMemory(pFishTrace,pFishPack,sizeof(tagFishPack));
						wSendSize+=sizeof(tagFishPack);
						++pFishTrace;
						//若下次大小增加会超出缓存，那么发送数据，并重置变量
						if (wSendSize+sizeof(tagFishPack)>sizeof(cbBuffer))
						{
							m_pITableFrame->SendTableData(wChairID,SUB_S_TRACE_POINT,cbBuffer,wSendSize);
							pFishTrace=(tagFishPack *)cbBuffer;
							wSendSize=0;
						}
					}
				}
			}
		}
		if (10==pPlotInfo->nType)
		{
			plotCircle(pPlotInfo->nID,rand()%pPlotInfo->nParam2,rand()%pPlotInfo->nParam3);
		}
	}

	//发送数据
	if (wSendSize>0)
	{
		if (wSendSize>sizeof(cbBuffer))
			ERROR_LOG;
		m_pITableFrame->SendTableData(wChairID,SUB_S_TRACE_POINT,cbBuffer,wSendSize);
	}
}

//用户断线
bool CTableFrameSink::OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem)
{
	calc(wChairID,pIServerUserItem,false);
	return true;
}

//鱼标识
unsigned int CTableFrameSink::GetNewFishID()
{
	m_dwFishID++;
	if (0==m_dwFishID) m_dwFishID=1;
	return m_dwFishID;
}

//子弹标识
unsigned int CTableFrameSink::GetNewBulletID()
{
	m_dwBulletID++;
	if (0==m_dwBulletID) m_dwBulletID=1;
	return m_dwBulletID;
}

//打鱼判断
bool CTableFrameSink::OnSubHitFish(IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize)
{
	if (NULL==pIServerUserItem)
	{
		ERROR_LOG;
		return true;
	}

	int nMinSize=sizeof(CMD_C_HitFish)-sizeof(int)*MAX_HITFISH;
	if (wDataSize<nMinSize)
	{
		ERROR_LOG;
		return false;
	}

	CMD_C_HitFish * pHitFish=(CMD_C_HitFish *)pData;
	if (pHitFish->nOtherCount>MAX_HITFISH)
	{
		ERROR_LOG;
		return false;
	}

	if (nMinSize+sizeof(int)*pHitFish->nOtherCount!=wDataSize)
	{
		ERROR_LOG;
		return false;
	}

	int nFishID=pHitFish->nFishID;
	DWORD dwBulletID=pHitFish->dwBulletID;
	int nHitUser=pHitFish->nHitUser;
	bool bAndroid=pHitFish->bAndroid;

	//判断椅子号
	WORD wChairID=-1;
	if (bAndroid)
		wChairID=nHitUser;
	else
		wChairID=pIServerUserItem->GetChairID();
	if (wChairID>=GAME_PLAYER)
	{
		ERROR_LOG;
		return true;
	}

	//玩家信息
	tagPlayerInfo & PlayerInfo=m_PlayerInfo[wChairID];

	if (PlayerInfo.bScoreFull)
	{
		return true;
	}

	//查找子弹
	int nCurrentBulletID=-1;
	for (int i=0;i<CountArray(PlayerInfo.BulletInfo);i++)
	{
		if (PlayerInfo.BulletInfo[i].dwID==dwBulletID&&PlayerInfo.BulletInfo[i].bHave)
		{
			nCurrentBulletID=i;
			if (PlayerInfo.BulletInfo[i].bHitMore)
			{
				PlayerInfo.BulletInfo[i].cbHitTime++;
			}
			if (!PlayerInfo.BulletInfo[i].bHitMore || PlayerInfo.BulletInfo[i].cbHitTime>=8)
			{
				PlayerInfo.BulletInfo[i].bHave=false;
			}
			break;
		}
	}
	if (nCurrentBulletID==-1)
	{
		if (!(PlayerInfo.bJumpBomb && dwBulletID==0))
		{
			ERROR_LOG;
			return true;
		}
	}

	//如果是机器人标志，判定是否存在，是否是机器人
	if (bAndroid)
	{
		IServerUserItem * pAndroidItem=m_pITableFrame->GetTableUserItem(nHitUser);
		if (pAndroidItem==NULL)
		{
			ERROR_LOG;
			return true;
		}
		if (!pAndroidItem->IsAndroidUser())
		{
			ERROR_LOG;
			return true;
		}
		if (nCurrentBulletID>=0)
		{
			PlayerInfo.BulletInfo[nCurrentBulletID].score=PlayerInfo.lUserCellScore;
		}
	}

	//当前子弹倍率
	int nCurrentBulletScore=0;
	//试用子弹
	bool bTryBullet=false;

	if (nCurrentBulletID>=0)
	{
		nCurrentBulletScore=PlayerInfo.BulletInfo[nCurrentBulletID].score;
		bTryBullet=PlayerInfo.BulletInfo[nCurrentBulletID].bTry;
		if (!PlayerInfo.BulletInfo[nCurrentBulletID].bHitMore || PlayerInfo.BulletInfo[nCurrentBulletID].cbHitTime>=8)
		{
			PlayerInfo.BulletInfo[nCurrentBulletID].score=0;
			PlayerInfo.BulletInfo[nCurrentBulletID].bTry=false;
		}
	}
	else if (PlayerInfo.bJumpBomb && dwBulletID==0)
	{
		bTryBullet=PlayerInfo.bTryCannon;
		nCurrentBulletScore=PlayerInfo.nJumpBombMultiple;
		PlayerInfo.nJumpCount++;
	}
	else
	{
		ERROR_LOG;
		return true;
	}

	//查找鱼
	bool bHaveFish=false;
	tagFishPack * pFish=NULL;
	INT_PTR nEnumIndex=0;
	do
	{
		pFish=m_FishFactory.EnumActiveObject(nEnumIndex++);
		if (NULL==pFish)
			break;

		if (pFish->nFishID==nFishID)
		{
			bHaveFish=true;
			break;
		}
	} while(true);
	if (!bHaveFish)
	{
		ERROR_LOG;
		return true;
	}

	//鱼信息
	tagFishInfo * pFishInfo=getFishInfo(pFish->nSpriteID);

	//玩家幸运值
	float fUserLucky=m_pITableFrame->GetUserFactor(pIServerUserItem->GetUserID());

	__int64 nPassTime=0;
	if (0!=PlayerInfo.nPassTime)
	{
		nPassTime=_time64(NULL)-PlayerInfo.nPassTime;
	}

	//库存信息
	const tagStockInfo * pStockInfo=NULL;
	pStockInfo=m_pITableFrame->GetStockInfo(0,m_pITableFrame->GetTableID(),bAndroid?eStockType_Robot:eStockType_Player);

	//@奖券
	bool bLottery=false;
	tagLotteryInfo * pLotteryInfo = m_pITableFrame->GetLotteryInfo();
	if (pLotteryInfo == NULL)
	{
		ERROR_LOG;
		return true;
	}

	BYTE cbVipLevel = pIServerUserItem->GetVipLevel();
	if (cbVipLevel < 2)
	{
		pLotteryInfo->nMaxGet = 29000;
	}

	//赠送奖券
	bool bLotteryGift = false;
	bool bBigLottery = false;
#ifndef KIND_7
	if (!pLotteryInfo->bCouldGet && PlayerInfo.nLotteryGift > 0)
	{
		tagLotteryInfo LotteryInfo = {0};
		LotteryInfo.nMaxGet = 10000000;
		LotteryInfo.nMinusStorage = 0;
		LotteryInfo.nPercent = 100;

		if (m_Config.nMinScore == 10 && PlayerInfo.nLotteryGift >= 10)
		{
			LotteryInfo.bCouldGet = true;
			bLotteryGift = true;
			int nPrice = rand() % 5 + 1;//1~5，step=1
			LotteryInfo.nPrice = min(nPrice, PlayerInfo.nLotteryGift);
		}
		else if (m_Config.nMinScore == 100 && PlayerInfo.nLotteryGift >= 10)
		{
			LotteryInfo.bCouldGet = true;
			bLotteryGift = true;
			int nPrice = 10 * (rand() % 5 + 1);//10~50，step=10
			LotteryInfo.nPrice = min(nPrice, PlayerInfo.nLotteryGift);
		}
		else if (m_Config.nMinScore == 1000 && PlayerInfo.nLotteryGift >= 50)
		{
			LotteryInfo.bCouldGet = true;
			bLotteryGift = true;
			int nPrice = 50 + 10 * (rand() % 6);//50~100，step=10
			LotteryInfo.nPrice = min(nPrice, PlayerInfo.nLotteryGift);
		}
		else if (m_Config.nMinScore == 10000 && PlayerInfo.nLotteryGift >= 100)
		{
			LotteryInfo.bCouldGet = true;
			bLotteryGift = true;
			int nPrice = 100 + 10 * (rand() % 41);//100~500，step=10
			LotteryInfo.nPrice = min(nPrice, PlayerInfo.nLotteryGift);
		}

		pLotteryInfo = &LotteryInfo;
	}
#endif

	//大奖券动画
	if (m_Config.nMinScore == 10 && pLotteryInfo->nPrice > 5 / 2)
	{
		bBigLottery = true;
	}
	else if (m_Config.nMinScore == 100 && pLotteryInfo->nPrice >= 50 / 2)
	{
		bBigLottery = true;
	}
	else if (m_Config.nMinScore == 1000 && pLotteryInfo->nPrice >= 100 / 2)
	{
		bBigLottery = true;
	}
	else if (m_Config.nMinScore == 10000 && pLotteryInfo->nPrice >= 500 / 2)
	{
		bBigLottery = true;
	}

	//@话费
	bool bMobileMoney=false;
	tagMobileMoney * pMobileMoney = m_pITableFrame->GetMobileMoney();
	if (pMobileMoney == NULL)
	{
		ERROR_LOG;
		return true;
	}

#ifndef KIND_7
	tagMobileMoney MobileMoney = {};
	//赠送话费
	//printf("PlayerInfo.wMobileMoneyGift=%hu,PlayerInfo.nShootCount=%d\n",PlayerInfo.wMobileMoneyGift,PlayerInfo.nShootCount);
	if (!pMobileMoney->bCouldGet && PlayerInfo.wMobileMoneyGift > 0 && PlayerInfo.nShootCount >= 10)
	{
		MobileMoney.bCouldGet = true;
		MobileMoney.nLessTime = 30 + rand() % 30;//30~60秒//120 + rand() % 60;//2到3分钟
		MobileMoney.wMaxGet = 10000;
		MobileMoney.nMinusStorage = 0;
		MobileMoney.nPercent = 100;
		MobileMoney.wPrice = PlayerInfo.wMobileMoneyGift;
		pMobileMoney = &MobileMoney;
		char szMsg[100];
		sprintf( szMsg, "pMobileMoney->bCouldGet is false,PlayerInfo.wMobileMoneyGift=%hu,用户id=%ld,奖励话费=%hu\n",PlayerInfo.wMobileMoneyGift,pIServerUserItem->GetUserID(),pMobileMoney->wPrice);
		OnRecord(szMsg);
	}
	//新手话费
	bool bNewPlayerMobileMoney = false;
	if (!pMobileMoney->bCouldGet)
	{
		if (PlayerInfo.wNewPlayerMobileMoney < 9)
		{
			time_t tMobileMoney = (time_t)PlayerInfo.nMobileMoneyTime;
			unsigned int nResult = time(NULL) - tMobileMoney;

			struct tm tm1, tm2;

			time_t tNow = time(NULL);
			memcpy(&tm1, localtime(&tMobileMoney), sizeof(struct tm));
			memcpy(&tm2, localtime(&tNow), sizeof(struct tm));
			struct tm * ptm1 = &tm1;
			struct tm * ptm2 = &tm2;
			//第2、3、4、5、6、7天，能得到的话费
			int nMax1[6] =
			{
				1, 3, 5, 7, 8, 9,
			};
			int nMax2[6] =
			{
				2, 4, 6, 7, 8, 9,
			};
			if (ptm1 != NULL && ptm2 != NULL)
			{
//printf("tm %d %d\n", ptm1->tm_mday, ptm2->tm_mday);
				int nDiff = ptm2->tm_mday - ptm2->tm_mday;
				if (nDiff >= 1 && nDiff <=6)
				{
//printf("nResult is %u nDiff is %d wNewPlayerMobileMoney %hu nMax1 %d\n", nResult, nDiff, PlayerInfo.wNewPlayerMobileMoney, nMax1[nDiff - 1]);
					if (nResult >= 1800 && PlayerInfo.wNewPlayerMobileMoney < nMax1[nDiff - 1])
					{
						bNewPlayerMobileMoney = true;
					}
					else if (nResult >= 3600 && PlayerInfo.wNewPlayerMobileMoney < nMax2[nDiff - 1])
					{
						bNewPlayerMobileMoney = true;
					}
				}
			}
			else
			{
//printf("tm null\n");
			}
		}
	}
	if (bNewPlayerMobileMoney)
	{
		MobileMoney.bCouldGet = true;
		MobileMoney.nLessTime = 0;
		MobileMoney.wMaxGet = 10000;
		MobileMoney.nMinusStorage = 0;
		MobileMoney.nPercent = 100;
		MobileMoney.wPrice = 1;

		pMobileMoney = &MobileMoney;
		char szMsg[100];
		sprintf( szMsg, "bNewPlayerMobileMoney is true,用户id=%ld,奖励话费=%hu\n",pIServerUserItem->GetUserID(),pMobileMoney->wPrice);
		OnRecord(szMsg);
	}
#endif
	//当前实际库存
	int nStorage=pStockInfo->mStockScore;

	//随机概率
	int nRandomPercent=int(random(0,10000));

	//命中率
	int nFishDiePercent=0;
	if (NULL!=pFishInfo)
		nFishDiePercent=pFishInfo->nHit;

	//倍率
	int nFishMultiple=0;
	if (NULL!=pFishInfo)
		nFishMultiple=pFishInfo->nMultiple;

	//int nCurrentPercent=nFishDiePercent*0.7f;
	int nCurrentPercent=nFishDiePercent;

	//同类炸弹、连线鱼
	if (pFish->nProperty&enSameDie || pFish->nProperty&enLine)
	{
		//@修正连线鱼概率不合理的问题
		if (pHitFish->nOtherCount>0)
		{
			int nValue=pFishInfo->nMultiple*pHitFish->nOtherCount;
			if (nValue>0)
			{
				nCurrentPercent=(6000/nValue)*0.7f;
			}
		}
	}

	//机器人不能打死博乐弹
	if (pFish->nProperty&enJumpBomb && bAndroid)
	{
		return true;
	}

	//如果是双倍炮，概率降低
	if (PlayerInfo.bSuperCannon)
		nCurrentPercent/=3;

	//@修改幸运值默认值
	if (fUserLucky<0)
	{
		bool bChangeLucky = (GetTickCount() - PlayerInfo.dwLuckyChangeTime > PlayerInfo.dwLuckyRandTime);//随机幸运值改为每5~15秒随机一次
		if (bChangeLucky)
		{
			fUserLucky = changeLucky(PlayerInfo);
		}
		else
		{
			fUserLucky = PlayerInfo.fUserLucky;
		}
	}
	if (m_pGameServiceOption->wServerID == 309 || m_pGameServiceOption->wServerID == 356)
	{
		__int64 nPassTime=0;

		if (0!=PlayerInfo.nPassTime)
		{
			nPassTime=_time64(NULL)-PlayerInfo.nPassTime;
		}

//printf("nPassTime is %lld\n", nPassTime);
		if (nPassTime < 600)
		{
			float f = (float)PlayerInfo.lUserAllScore / 150000.f;
			float f2 = 1.0f - f;

			fUserLucky = 0.6f + 1.2f * f2;
			if (pFishInfo->nSpriteID > 1016 || pFish->nProperty&enSameDie || pFish->nProperty&enLine)
			{
				fUserLucky = 0.1f;
			}
//printf("fUserLucky = %.2f\n", fUserLucky);
		}
	}
	//printf("fUserLucky is %.2f\n", fUserLucky);

	//动态库存难度调整正系数，机器人不计算库存几率影响
	float fAutoStockDiffFactor = 1.f;
	if (m_Config.m_fAutoStockDiffMaxFactor > 0.001f && m_Config.m_fAutoStockDiffMaxValue > 0.001f && !bAndroid)
	{
		if (nStorage > m_Config.m_fAutoStockDiffMaxValue)
		{
			fAutoStockDiffFactor = 1.f + m_Config.m_fAutoStockDiffMaxFactor;
		}
		else
		{
			fAutoStockDiffFactor = 1.f + (float)nStorage * m_Config.m_fAutoStockDiffMaxFactor / m_Config.m_fAutoStockDiffMaxValue;
		}
	}
//printf("fAutoStockDiffFactor=%.2f\n",fAutoStockDiffFactor);

	////玩家幸运值调整
	//if (fUserLucky>=0&&!bAndroid)
	//	nCurrentPercent=(int)(nCurrentPercent*fUserLucky);

	//nCurrentPercent=(int)(nCurrentPercent*fAutoStockDiffFactor);

	//if (pFish->nProperty == 0)
	{
		nCurrentPercent=PlayerInfo.calcFishPercent(nCurrentPercent, nFishMultiple);
	}

	if (PlayerInfo.BulletInfo[nCurrentBulletID].bHitMore)
	{
		float fFactor = 1.5f + (float)PlayerInfo.BulletInfo[nCurrentBulletID].cbHitTime * 0.1f;
		nCurrentPercent /= fFactor;
	}

	int nSendCount=0;
	CMD_S_CaptureFish vFishHit={0};

	int nTotalScore=0;
	int nTotalMultiple=0;
	int nSpriteID=pFish->nSpriteID;
	int nProperty=pFish->nProperty;

	//更新BOSS分数
	if (nRandomPercent>=nCurrentPercent && pFish->nProperty&enBoss)
	{
		m_nBossScore+=3;
		if (m_nBossScore>330)
			m_nBossScore=330;
	}

	//博乐弹必中
	if (PlayerInfo.bJumpBomb && dwBulletID==0)
	{
		nCurrentPercent=10000;
	}

	//试用概率
	if (bTryBullet)
	{
		if (PlayerInfo.bTryCannon)
		{
			nCurrentPercent=8000;
		}
		else
		{
			nCurrentPercent=0;
		}
	}

	bool bSendJumpBomb=false;

	//@打中的鱼
	int nElementCount=0;
	int nHitCount[MAX_HITFISH]={0};

	bool bBomb=false;
	bool bSameDie=false;
	bool bLineFish=false;
	bool bRangeBomb=false;
	//判断是否打中鱼
	if (nRandomPercent<nCurrentPercent)
	{
		//炸弹
		if (pFish->nProperty&enBomb)
			bBomb=true;
		//同类鱼
		if (pFish->nProperty&enSameDie)
			bSameDie=true;
		//连线鱼
		if (pFish->nProperty&enLine)
			bLineFish=true;
		//范围炸弹
		if (pFish->nProperty&enRangeBomb)
			bRangeBomb=true;

		if (pFish->nProperty&enBoss)
		{
			nTotalScore=nCurrentBulletScore*m_nBossScore;
			vFishHit.nFishMultiple[nSendCount]=m_nBossScore;
			nTotalMultiple=m_nBossScore;
			m_nBossScore=3;
		}
		else
		{
			nTotalScore=nCurrentBulletScore*nFishMultiple;
			vFishHit.nFishMultiple[nSendCount]=nFishMultiple;
			nTotalMultiple=nFishMultiple;
		}

		vFishHit.nFishID[nSendCount]=nFishID;
		vFishHit.nFishScore[nSendCount]=nTotalScore;
		nSendCount++;
		nHitCount[nElementCount++]=pFishInfo->nSpriteID;

		for (int i=0; i<pHitFish->nOtherCount; i++)
		{
			tagFishInfo * pFishInfo2=NULL;
			tagFishPack * pFish2=NULL;
			nEnumIndex=0;
			do
			{
				pFish2=m_FishFactory.EnumActiveObject(nEnumIndex++);
				if (NULL==pFish2) break;

				if (pFish->nFishID==pFish2->nFishID) continue;

				if (pFish2->nFishID==pHitFish->nOtherFishID[i])
				{
					pFishInfo2=getFishInfo(pFish2->nSpriteID);
					break;
				}
			} while (true);

			if (NULL==pFish2)
			{
				continue;
			}

			int nFishMultiple2=0;
			if (NULL!=pFishInfo2)
			{
				nFishMultiple2=pFishInfo2->nMultiple;
			}
			else
			{
				ERROR_LOG;
				continue;
			}

			bool bHit=false;
			//炸弹
			if (bBomb)
			{
				bHit=true;
			}
			//同类鱼
			if (bSameDie && pFishInfo->nSameDieID-1000==pFish2->nSpriteID)
			{
				bHit=true;
			}
			//连线鱼
			if (bLineFish && pFishInfo->nSpriteID-1000==pFish2->nSpriteID)
			{
				bHit=true;
			}
			//范围炸弹
			if (bRangeBomb)
			{
				bHit=true;
			}

			//判断数量
			if (nSendCount>=MAX_HITFISH)
			{
				ERROR_LOG;
				break;
			}

			if (bHit || (PlayerInfo.bJumpBomb && dwBulletID==0))
			{
				nFishMultiple+=nFishMultiple2;
				if (pFish2->nProperty&enBoss)
				{
					nTotalScore+=nCurrentBulletScore*m_nBossScore;
					vFishHit.nFishMultiple[nSendCount]=m_nBossScore;
					nTotalMultiple+=m_nBossScore;
					m_nBossScore=3;
				}
				else
				{
					nTotalScore+=nCurrentBulletScore*nFishMultiple2;
					vFishHit.nFishMultiple[nSendCount]=nFishMultiple2;
					nTotalMultiple+=nFishMultiple2;
				}

				vFishHit.nFishID[nSendCount]=pFish2->nFishID;
				vFishHit.nFishScore[nSendCount]=nCurrentBulletScore*nFishMultiple2;
				nSendCount++;
				nHitCount[nElementCount++]=pFishInfo2->nSpriteID;
			}
		}

		//双倍炮
		if (PlayerInfo.bSuperCannon)
		{
			nTotalScore*=2;
			nTotalMultiple*=2;
		}
#ifndef KIND_7
		//@奖券
		//////////
		if (pLotteryInfo->bCouldGet && !bAndroid)//本房间有获得奖券的资格
		{
			int nPercent=(int)(random(0,100));
			if (nPercent <= pLotteryInfo->nPercent && _time64(NULL) > PlayerInfo.nNextLotteryTime)//概率是否足够、时间是否足够
			{
				if (PlayerInfo.nLotteryGeted < pLotteryInfo->nMaxGet)//最大获取量限制
				{
					bLottery = true;
				}
			}
		}
		//////////

		//@话费
		//////////
		if (pMobileMoney->bCouldGet && !bAndroid)//本房间有获得话费的资格
		{
			int nPercent=int(random(0,100));
			//printf("%d %d %lld %d\n", nPercent, pMobileMoney->nPercent, nPassTime, pMobileMoney->nLessTime);
			if (nPercent <= pMobileMoney->nPercent && nPassTime >= pMobileMoney->nLessTime)//概率是否足够、时间是否足够
			{
				printf("%d %d\n", PlayerInfo.wMobileMoneyGeted, pMobileMoney->wMaxGet);
				if (PlayerInfo.wMobileMoneyGeted < pMobileMoney->wMaxGet)//最大获取量限制
				{
					bMobileMoney = true;
				}
			}
		}
		//////////
#endif
		//试用
		if (!bTryBullet)
		{
			////库存检测
			//if (nStorage-nTotalScore-pLotteryInfo->nMinusStorage<0)
			//{
			//	//printf("storage not enough, nSendCount=%d\n",nSendCount);
			//	nTotalMultiple=0;
			//	return true;
			//}
		}
		else
		{
			if (PlayerInfo.lUserTryScore + nTotalScore >= 185000)
			{
				return true;
			}
		}

		////如果是抽奖18号鱼
		//if (pFish->nSpriteID==18)
		//{
		//	int tempint[20]={0,0,3,3,3,7,7,7,6,0,3,3,3,7,7,7,2,6,7,0};
		//	int tempscore[8]={0,10,500,40,0,20,100,25};
		//	nFishMultiple=tempint[rand()%20];
		//	nFishScore=tempscore[nFishMultiple]*m_UserShoot[wChairID][nCurrentBulletID].score;
		//	if (nFishMultiple==3||nFishMultiple==7)
		//	{
		//		PlayerInfo.bBonus=true;
		//		PlayerInfo.bSuperCannon=true;
		//		switch(wChairID)
		//		{
		//		case 0:{m_pITableFrame->SetGameTimer(TimerSysMessage0,20000L,1,wChairID);m_pITableFrame->SetGameTimer(TimerSysMessage10,5000L,1,wChairID);break;}
		//		case 1:{m_pITableFrame->SetGameTimer(TimerSysMessage1,20000L,1,wChairID);m_pITableFrame->SetGameTimer(TimerSysMessage11,5000L,1,wChairID);break;}
		//		case 2:{m_pITableFrame->SetGameTimer(TimerSysMessage2,20000L,1,wChairID);m_pITableFrame->SetGameTimer(TimerSysMessage12,5000L,1,wChairID);break;}
		//		case 3:{m_pITableFrame->SetGameTimer(TimerSysMessage3,20000L,1,wChairID);m_pITableFrame->SetGameTimer(TimerSysMessage13,5000L,1,wChairID);break;}
		//		case 4:{m_pITableFrame->SetGameTimer(TimerSysMessage4,20000L,1,wChairID);m_pITableFrame->SetGameTimer(TimerSysMessage14,5000L,1,wChairID);break;}
		//		case 5:{m_pITableFrame->SetGameTimer(TimerSysMessage5,20000L,1,wChairID);m_pITableFrame->SetGameTimer(TimerSysMessage15,5000L,1,wChairID);break;}
		//		}
		//	}
		//}
		//CMD_S_CaptureFish CaptureFish={0};
		//if ((nRandomPercent%10==0&&PlayerInfo.bSuperCannon==false)&&(pFish->nSpriteID>=15)&&(pFish->nSpriteID!=18))
		//{
		//	switch(wChairID)
		//	{
		//	case 0:{m_pITableFrame->SetGameTimer(TimerSysMessage0,15000L,1,wChairID);break;}
		//	case 1:{m_pITableFrame->SetGameTimer(TimerSysMessage1,15000L,1,wChairID);break;}
		//	case 2:{m_pITableFrame->SetGameTimer(TimerSysMessage2,15000L,1,wChairID);break;}
		//	case 3:{m_pITableFrame->SetGameTimer(TimerSysMessage3,15000L,1,wChairID);break;}
		//	case 4:{m_pITableFrame->SetGameTimer(TimerSysMessage4,15000L,1,wChairID);break;}
		//	case 5:{m_pITableFrame->SetGameTimer(TimerSysMessage5,15000L,1,wChairID);break;}
		//	}
		//	CaptureFish.bSuperCannon=true;
		//	PlayerInfo.bSuperCannon=true;
		//}

		//博乐弹
		if (pFish->nProperty&enJumpBomb)
		{
			bSendJumpBomb=true;
		}

		//删除这条鱼
		m_FishFactory.FreeItem(pFish);

		//圆圈剧情
		tagPlotInfo * pPlotInfo=getPlotInfo(pFishInfo->nPlotID);
		if (NULL!=pPlotInfo && !m_bFishingOffSeason && !m_bManEatingFish)
		{
			plotCircle(pPlotInfo->nID,pHitFish->nXPos,pHitFish->nYPos);
		}
	}

#ifndef KIND_7
	//公告
	if (nTotalScore < 2000000)
#endif
	{
		if (nTotalMultiple>=300)
		{
			IServerUserItem * pUserItem=m_pITableFrame->GetTableUserItem(wChairID);
			CString str;
			unsigned int nCount=m_vNotice.size();
			if (nCount>0)
			{
				int nIndex=rand()%nCount;
				if (bLineFish)
				{
					str.Format(m_vNotice[nIndex]->sNotice,pUserItem->GetNickName(),TEXT("红鱼"),nTotalScore);
				}
				else
				{
					str.Format(m_vNotice[nIndex]->sNotice,pUserItem->GetNickName(),pFishInfo->sName,nTotalScore);
				}
				//m_pITableFrame->SendGameMessage(str,0);
				m_pITableFrame->SendRoomMessage(str,SMT_CHAT|SMT_CATCH_FISH);
			}
		}
	}
#ifndef KIND_7
	else
	{
		IServerUserItem * pUserItem=m_pITableFrame->GetTableUserItem(wChairID);
		CString str;
		if (bLineFish)
		{
			str.Format("#ffffff恭喜【#ff0000%s#ffffff】在#ff0000%s#ffffff击中了#ff0000%s#ffffff获得了#ff0000%d万#ffffff金币。",pUserItem->GetNickName(),m_pGameServiceOption->szServerName,TEXT("红鱼"),nTotalScore/10000);
		}
		else
		{
			str.Format("#ffffff恭喜【#ff0000%s#ffffff】在#ff0000%s#ffffff击中了#ff0000%s#ffffff获得了#ff0000%d万#ffffff金币。",pUserItem->GetNickName(),m_pGameServiceOption->szServerName,pFishInfo->sName,nTotalScore/10000);
		}
		m_pITableFrame->SendPlatformMessage(str, 45);
	}
#endif

	//试用
	if (!bTryBullet)
	{
		if (nTotalScore!=0)
		{
			m_pITableFrame->ChangeStock(bAndroid?eStockType_Robot:eStockType_Player,wChairID,-nTotalScore,0,0);
		}

		if (!bAndroid)
			m_nScoreOut+=nTotalScore;

		//结算分数
		PlayerInfo.lUserAllScore+=nTotalScore;
		char szMsg[100];
		sprintf( szMsg, "玩家id = %d,分数=%ld，增加分数nTotalScore=%d\n",pIServerUserItem->GetUserID(),PlayerInfo.lUserAllScore,nTotalScore);
		//OnRecord(szMsg);
		PlayerInfo.nTotalWinScore+=nTotalScore;
		PlayerInfo.nThisTimeWinScore+=nTotalScore;

		PlayerInfo.hit(nTotalScore, pFishInfo->nSpriteID);
		if (fcalcnew != NULL)
		{
			fprintf(fcalcnew, "hit UserID %u Gains = %lld Consume = %lld Yields = %f Crr = %f\n", pIServerUserItem->GetUserID(), PlayerInfo.lGains, PlayerInfo.lConsume, PlayerInfo.fYields, PlayerInfo.fCurve[PlayerInfo.nPointCount]);
			fflush(fcalcnew);
		}

#ifndef KIND_7
		//系统赠送技能
		if (!PlayerInfo.bSkillGave && PlayerInfo.lUserEnterScore > 0)
		{
			__int64 nPassTime=0;
			if (0!=PlayerInfo.nPassTime)
			{
				nPassTime=_time64(NULL)-PlayerInfo.nPassTime;
			}

			float fResult = (float)PlayerInfo.lUserAllScore / (float)PlayerInfo.lUserEnterScore;
			//当前分是进入时分数的1.3倍以上
			if (fResult >= 1.3f || fResult <= 0.9f || nPassTime >= 600)
			{
				PlayerInfo.bSkillGave = true;
				m_pITableFrame->SkillGive(pIServerUserItem);
			}
		}
#endif
	}
	else
	{
		PlayerInfo.lUserTryScore+=nTotalScore;
	}

	if (nSendCount>0)
	{
		//printf("nSendCount=%d total is %d\n",nSendCount,m_FishFactory.GetActiveCount());
		BYTE cbBuffer[sizeof(CMD_S_CaptureFish)]={0};
		WORD wSendSize=0;
		CMD_S_CaptureFish * pCaptureFish=(CMD_S_CaptureFish *)cbBuffer;
		vFishHit.nCaptureCount=nSendCount;
		vFishHit.wChairID=wChairID;
		vFishHit.nUserScore=PlayerInfo.lUserAllScore;
		vFishHit.nTryScore=PlayerInfo.bTryCannon?PlayerInfo.lUserTryScore:0;
		vFishHit.nProperty=nProperty;
		vFishHit.nTotalScore=nTotalScore;
		vFishHit.nTotalMultiple=nTotalMultiple;
		if (m_bMatchPlaying&&!bTryBullet)
		{
			UpdateUserIntegrate(pIServerUserItem,nTotalScore);
			char szMsg[100];
			sprintf( szMsg, "用户id=%ld,增加积分=%hu\n",pIServerUserItem->GetUserID(),nTotalScore);
			OnRecord(szMsg);
		}
		
		//@奖券
		if (bLottery)
		{
			//printf("LotterySend\n");
			//已被领走，设置为不能获得
			pLotteryInfo->bCouldGet = false;
			int nAddTime = TIME_LOTTERY;
			PlayerInfo.nNextLotteryTime = _time64(NULL) + nAddTime;
			vFishHit.nLotteryCount = pLotteryInfo->nPrice;
			vFishHit.bBigLottery = bBigLottery;
			//printf("vFishHit.nLotteryCount is %d\n", vFishHit.nLotteryCount);

			PlayerInfo.nLotteryGeted += vFishHit.nLotteryCount;
			SaveUserItem(pIServerUserItem,
				eUserItem_LotteryInfo,
				eUserItemFlag_Heap|eUserItemFlag_GamePreLoad|eUserItemFlag_OverWrite,
				PlayerInfo.nLotteryGeted,false);

			//获奖记录
			TCHAR szRecord[1024]={0};
			sprintf(szRecord, "{\"Lottery\":%d}", vFishHit.nLotteryCount);
			m_pITableFrame->SaveGameJson(pIServerUserItem,szRecord,0);

			if (bLotteryGift)
			{
				//赠送的奖券
				PlayerInfo.nLotteryGift -= pLotteryInfo->nPrice;
				SaveUserItem(pIServerUserItem,
					eUserItem_LotteryGift,
					eUserItemFlag_Heap|eUserItemFlag_GamePreLoad|eUserItemFlag_OverWrite,
					PlayerInfo.nLotteryGift,false);
			}
		}
		CopyMemory(pCaptureFish,&vFishHit,sizeof(CMD_S_CaptureFish));
		wSendSize=sizeof(CMD_S_CaptureFish);
		bool bHavePearlFish = false;
		bool bHaveBoxFish = false;
		for (int i=0; i<nSendCount; i++)
		{
			//删除命中的鱼
			tagFishPack * pFishPack=NULL;
			nEnumIndex=0;
			do
			{
				pFishPack=m_FishFactory.EnumActiveObject(nEnumIndex++);
				if (NULL==pFishPack)
					break;
				if (vFishHit.nFishID[i]==pFishPack->nFishID)
				{
					if (pFishPack->nSpriteID == 18)
					{
						bHavePearlFish = true;
					}
					m_FishFactory.FreeItem(pFishPack);
				}
			} while(true);
		}
		if (pFishInfo->nSpriteID == 1018)
		{
			bHavePearlFish = true;
		}
		if (pFishInfo->nSpriteID == 1026)
		{
			bHaveBoxFish = true;
		}
		//食金币鱼
		if ((pFishInfo->nSpriteID == 1027 || pFishInfo->nSpriteID == 1028 || pFishInfo->nSpriteID == 1029) && m_bManEatingFish)
		{
			bool bHaveManEatingFish = false;
			for (int i = 0; i < GAME_PLAYER; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					if (m_ManEatingFish[i].nFishID[j] == nFishID)
					{
printf("bHaveManEatingFish\n");
printf("nFishID=%d\n", nFishID);
						bHaveManEatingFish = true;
						PlayerInfo.lUserAllScore += m_ManEatingFish[i].nEatScore[j];
						vFishHit.nUserScore = PlayerInfo.lUserAllScore;
						m_ManEatingFish[i].nSpriteID[j] = 0;
						m_ManEatingFish[i].bIsDead[j] = true;
//						GenManEatingFish(i, j, 1027);
//						CMD_S_ManEatingFishNewFish ManEatingFishNewFish = {};
//						ManEatingFishNewFish.wChairID = i;
//						ManEatingFishNewFish.nFishID = m_ManEatingFish[i].nFishID[j];
//printf("j %d nFishID %d\n", j, m_ManEatingFish[i].nFishID[j]);
//						ManEatingFishNewFish.nSpriteID = 1027;
//						ManEatingFishNewFish.wIndex = j;
//						ManEatingFishNewFish.nCurrentScore = PlayerInfo.lUserAllScore;
//						ManEatingFishNewFish.nMultiple = 10;
//printf("score %d %ld\n", m_ManEatingFish[i].nEatScore[j], PlayerInfo.lUserAllScore);
//						ManEatingFishNewFish.nAteScore = m_ManEatingFish[i].nEatScore[j];
//						m_ManEatingFish[i].nEatScore[j] = 0;
//						m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_MAN_EATING_FISH_NEW_FISH, &ManEatingFishNewFish, sizeof(ManEatingFishNewFish));
						break;
					}
				}
				if (bHaveManEatingFish)
				{
					break;
				}
			}
		}
#ifdef KIND_7
		if (bHavePearlFish)
		{
			CMD_S_SendPearlInfo SendPearlInfo = {};
			SendPearlInfo.nPearl1Price = 100000;
			SendPearlInfo.nPearl2Price = 200000;
			SendPearlInfo.nPearl3Price = 500000;
			m_pITableFrame->SendTableData(wChairID, SUB_S_SEND_PEARL_INFO, &SendPearlInfo, sizeof(SendPearlInfo));
		}
#endif
		if (bHaveBoxFish)
		{
			if (bTryBullet)
			{
				PlayerInfo.lUserTryScore = 180000;
			}
			else
			{
				BoxChange(pIServerUserItem, PlayerInfo, nFishID);
			}
		}
		if (wSendSize>0)
		{
//printf("SUB_S_CAPTURE_FISH\n");
			if (wSendSize>sizeof(cbBuffer))
				ERROR_LOG;
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_CAPTURE_FISH,cbBuffer,wSendSize);
		}
		//@@@
		if (pFishInfo->nSpriteID == 1018 && m_bManEatingFish)
		{
			m_bManEatingFish = false;
			m_nManEatingFishTime = 0;
			m_pITableFrame->KillGameTimer(TimerManEatingFish);
			CMD_S_EatingFishBossDie EatingFishBossDie = {};
			EatingFishBossDie.wChairID = wChairID;
			int nMultiple[] = {10, 20, 40};
			for (int i = 0; i < GAME_PLAYER; i++)
			{
				tagManEatingFish & ManEatingFish = m_ManEatingFish[i];
				for (int j = 0; j < 3; j++)
				{
					EatingFishBossDie.nFishID[i][j] = ManEatingFish.nFishID[j];
					EatingFishBossDie.nFishScore[i][j] = ManEatingFish.nEatScore[j] + nCurrentBulletScore * nMultiple[ManEatingFish.nSpriteID[j] - 1027];

					PlayerInfo.lUserAllScore += ManEatingFish.nEatScore[j];

					ManEatingFish.bIsDead[j] = true;
					ManEatingFish.nFishID[j] = 0;
					ManEatingFish.nEatScore[j] = 0;
					ManEatingFish.nSpriteID[j] = 0;
				}
			}
			EatingFishBossDie.nCurrentScore = PlayerInfo.lUserAllScore;
			m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_MAN_EATING_BOSS_DIE, &EatingFishBossDie, sizeof(EatingFishBossDie));
		}

		if (pIServerUserItem->GetPearl() >= 0)
		{
			m_pITableFrame->WinPearl(pIServerUserItem, pIServerUserItem->GetPearl(), 1);
			pIServerUserItem->SetPearl(-1);
		}

		//试用结束
		if (PlayerInfo.bTryCannon && PlayerInfo.lUserTryScore >= 175000)
		{
			PlayerInfo.bTryCannon = false;

			SaveUserItem(pIServerUserItem,
				eUserItemTryScore,
				eUserItemFlag_Heap|eUserItemFlag_GamePreLoad|eUserItemFlag_OverWrite,
				PlayerInfo.lUserTryScore,false);

			CMD_S_TryCannonEnd TryCannonEnd = {0};
			TryCannonEnd.nTryScore = PlayerInfo.lUserTryScore;

			m_pITableFrame->SendTableData(wChairID, SUB_S_TRY_CANNON_END, &TryCannonEnd, sizeof(TryCannonEnd));
		}

		PlayerInfo.nFishCount+=nSendCount;
	}

	if (PlayerInfo.bJumpBomb && dwBulletID==0)
	{
		if (PlayerInfo.nJumpCount>=3)
		{
			PlayerInfo.bJumpBomb=false;
			PlayerInfo.nJumpCount=0;
		}
	}

	if (!PlayerInfo.bScoreFull && PlayerInfo.lUserAllScore >= m_pGameServiceOption->lMaxEnterScore && (m_pGameServiceOption->wServerID == 309 || m_pGameServiceOption->wServerID == 356))
	{
//printf("standup\n");
		PlayerInfo.bScoreFull = true;
		m_pITableFrame->SendTableData(pIServerUserItem->GetChairID(), SUB_S_SCORE_FULL, NULL, 0);
		return true;
	}

	if (bSendJumpBomb)
	{
		CMD_S_JumpBomb JumpBomb={0};
		JumpBomb.wChairID=wChairID;
		JumpBomb.nSpriteID=nSpriteID;
		JumpBomb.ptStart=CPoint(pHitFish->nXPos,pHitFish->nYPos);
		JumpBomb.ptJump[0]=JumpBomb.calcPoint(JumpBomb.ptStart);
		JumpBomb.ptJump[1]=JumpBomb.calcPoint(JumpBomb.ptJump[0]);
		JumpBomb.ptJump[2]=JumpBomb.calcPoint(JumpBomb.ptJump[1]);
		PlayerInfo.bJumpBomb=true;
		PlayerInfo.nJumpBombMultiple=nCurrentBulletScore;
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_JUMP_BOMB,&JumpBomb,sizeof(JumpBomb));
	}
#ifndef KIND_7
	//@话费
	if (bMobileMoney)
	{
		printf("MobileMoneySend\n");
		//已被领走，设置为不能获得
		pMobileMoney->bCouldGet = false;
		if (PlayerInfo.wMobileMoneyGeted == 0 && pMobileMoney->wPrice == 5)
		{
			PlayerInfo.wMobileMoneyGeted += pMobileMoney->wPrice;
			m_pITableFrame->SaveUserItemCount(pIServerUserItem, eUserItem_MobileMoney, (SCORE)PlayerInfo.wMobileMoneyGeted);

			//获奖记录
			TCHAR szRecord[1024]={0};
			sprintf(szRecord, "{\"MobileMoney\":%hu}", pMobileMoney->wPrice);
			m_pITableFrame->SaveGameJson(pIServerUserItem,szRecord,0);

			CMD_S_MobileMoney MobileMoney = {0};
			MobileMoney.wMoneyCount = pMobileMoney->wPrice;
			_snprintf(MobileMoney.szURL, sizeof(MobileMoney.szURL), "%s", "http://lb.66y.com");
			_snprintf(MobileMoney.szContent, sizeof(MobileMoney.szContent), "我在捕鱼大咖中赢得了%d元话费。", pMobileMoney->wPrice);
			m_pITableFrame->SendTableData(wChairID,SUB_S_FIRST_MOBILE_MONEY,&MobileMoney,sizeof(MobileMoney));
		}
		else
		{
			if (bNewPlayerMobileMoney)
			{
				PlayerInfo.wNewPlayerMobileMoney += pMobileMoney->wPrice;
//printf("PlayerInfo.wNewPlayerMobileMoney is %hu\n", PlayerInfo.wNewPlayerMobileMoney);
				m_pITableFrame->SaveUserItemCount(pIServerUserItem, eUserItem_NewPlayerMobileMoney, (SCORE)PlayerInfo.wNewPlayerMobileMoney);
			}
			//对于新版本，分享通过网站，话费由网站更新
			PlayerInfo.wMobileMoneyWill = pMobileMoney->wPrice;
			//printf("PlayerInfo.wMobileMoneyWill is %hu\n", PlayerInfo.wMobileMoneyWill);
			m_pITableFrame->SaveUserItemCount(pIServerUserItem, eUserItem_MobileShare, (SCORE)pMobileMoney->wPrice);
			//发送得奖消息
			CMD_S_MobileMoney MobileMoney = {0};
			MobileMoney.wMoneyCount = pMobileMoney->wPrice;
			_snprintf(MobileMoney.szURL, sizeof(MobileMoney.szURL), "%s", "http://lb.66y.com");
			_snprintf(MobileMoney.szContent, sizeof(MobileMoney.szContent), "我在捕鱼大咖中赢得了%d元话费。", pMobileMoney->wPrice);
			m_pITableFrame->SendTableData(wChairID,SUB_S_MOBILE_MONEY,&MobileMoney,sizeof(MobileMoney));
			char szMsg[100];
			sprintf( szMsg, "用户id=%ld,奖励话费=%hu\n",pIServerUserItem->GetUserID(),pMobileMoney->wPrice);
			OnRecord(szMsg);
		}

		//赠送的话费清0
		PlayerInfo.wMobileMoneyGift = 0;
		SaveUserItem(pIServerUserItem,
			eUserItem_MobileGift,
			eUserItemFlag_Heap|eUserItemFlag_GamePreLoad|eUserItemFlag_OverWrite,
			PlayerInfo.wMobileMoneyGift,false);
	}
#endif
//printf("nFishScore=%d\n",nFishScore);
#ifndef KIND_7
	//比赛得分
	if (0 != m_MatchManager.m_nMatchCheckElapse && !bTryBullet)
	{
		if (0 != m_MatchManager.CheckMatchConfig())
		{
			for (int i = 0; i < (int)m_MatchManager.m_vMatchConfig.size(); i++)
			{
				//处理比赛结束写分
				if (false == m_MatchManager.m_vMatchConfig[i].bInMatch)
				{
					for (int j = 0; j < (int)PlayerInfo.vMatchInfo.size(); j++)
					{
						if (PlayerInfo.vMatchInfo[j].nFlag == m_MatchManager.m_vMatchConfig[i].nGenTag)
						{
							int nType = PlayerInfo.vMatchInfo[j].nFlag;
							SCORE lCount = PlayerInfo.vMatchInfo[j].lWinScore;
//printf("比赛结束得分%d\n",PlayerInfo.vMatchInfo[j].nWinScore);
							if (lCount>0)
							{
								SaveUserItem(pIServerUserItem,
									nType,
									eUserItemFlag_Heap | eUserItemFlag_GamePreLoad | eUserItemFlag_OverWrite | eUserItemFlag_Match,
									lCount, true);
							}
							break;
						}
					}
					continue;
				}

				bool bHaveTag = false;
				for (int j = 0; j < (int)PlayerInfo.vMatchInfo.size(); j++)
				{
					if (PlayerInfo.vMatchInfo[j].nFlag == m_MatchManager.m_vMatchConfig[i].nGenTag)
					{
						PlayerInfo.vMatchInfo[j].lWinScore += nTotalScore;
//printf("比赛得分%d\n",PlayerInfo.vMatchInfo[j].nWinScore);
						bHaveTag = true;
						break;
					}
					else
					{
					}
				}

				if (!bHaveTag)
				{
					int nMatchIndex = (int)PlayerInfo.vMatchInfo.size();
					PlayerInfo.vMatchInfo.push_back(tagMatchInfo());
					PlayerInfo.vMatchInfo[nMatchIndex].nFlag = m_MatchManager.m_vMatchConfig[i].nGenTag;
					PlayerInfo.vMatchInfo[nMatchIndex].lWinScore = nTotalScore;
				}
			}
		}
	}
#endif

	return true;
}

//发送消息
void CTableFrameSink::SendDataExcludeSourceUser(IServerUserItem * pIServerUserItemSource, WORD wSubCmdID, void * pData, WORD wDataSize)
{
	if (NULL==pIServerUserItemSource)
	{
		ERROR_LOG;
		return;
	}

	for (int i=0;i<GAME_PLAYER;i++)
	{
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(i);
		if (pIServerUserItem==pIServerUserItemSource) continue;
		if (pIServerUserItem!=NULL && pIServerUserItem->IsAndroidUser()==false)
		{
			m_pITableFrame->SendTableData(i,wSubCmdID,pData,wDataSize);
		}
	}
}

double CTableFrameSink::random(double start, double end)
{
	return start+(end-start)*rand()/(RAND_MAX+1.0);
}

//获取鱼配置
tagFishInfo * CTableFrameSink::getFishInfo(int nSpriteID)
{
	
	tagFishInfo * pFishInfo=NULL;
	for (unsigned int i=0; i<m_vFishInfo.size(); i++)
	{
		if (nSpriteID==m_vFishInfo[i]->nSpriteID-1000)
		{
			pFishInfo=m_vFishInfo[i];
			break;
		}
	}
	if (NULL==pFishInfo)
	{
		printf("nSprinteID=%d\n",nSpriteID);
		ERROR_LOG;
	}
		
	return pFishInfo;
}

//获取剧情配置
tagPlotInfo * CTableFrameSink::getPlotInfo(int nPlotID)
{
	tagPlotInfo * pPlotInfo=NULL;
	for (unsigned int i=0; i<m_vPlotInfo.size(); i++)
	{
		if (nPlotID==m_vPlotInfo[i]->nID)
		{
			pPlotInfo=m_vPlotInfo[i];
			break;
		}
	}
	if (NULL==pPlotInfo)
		ERROR_LOG;
	return pPlotInfo;
}

//读取配置
void CTableFrameSink::readXML(const char * filename)
{
	//读取xml文档
#ifndef _DEBUG
	TiXmlDocument * xmldoc=new TiXmlDocument;
	if (!xmldoc->LoadFile(filename))
	{
		ERROR_LOG;
		return;
	}
#else
	//Decrypt DecryptObj;
	//int nSize=0;
	//unsigned char * cbBuffer=DecryptObj.decryptFile(filename,nSize);z
	//TiXmlDocument * xmldoc=new TiXmlDocument;
	//if (!xmldoc->LoadFile(cbBuffer,nSize))
	//{
	//	ERROR_LOG;
	//	return;
	//}
	//free(cbBuffer);

	TiXmlDocument * xmldoc=new TiXmlDocument;
	if (!xmldoc->LoadFile(filename))
	{
		ERROR_LOG;
		return;
	}
#endif
	//检查根节点
	TiXmlElement * pElement=xmldoc->RootElement();
	std::string nodename=pElement->Value();
	if (0!=nodename.compare("server"))
	{
		ERROR_LOG;
		return;
	}

	//读取配置
	TiXmlElement * pChildElement=pElement->FirstChildElement();
	while (pChildElement!=NULL)
	{
		TiXmlElement * pCurrentElement=pChildElement;
		const char * pValue=NULL;
		pChildElement=pChildElement->NextSiblingElement();
		nodename=pCurrentElement->Value();
		if (0==nodename.compare("fish"))
		{
			tagFishInfo * pFishInfo=NULL;

			pFishInfo=new tagFishInfo;

			m_vFishInfo.push_back(pFishInfo);

			pValue=pCurrentElement->Attribute("id");
			if (NULL!=pValue)
			{
				pFishInfo->nSpriteID=atoi(pValue);
			}
			pValue=pCurrentElement->Attribute("hit");
			if (NULL!=pValue)
			{
				pFishInfo->nHit=atoi(pValue);
			}
			pValue=pCurrentElement->Attribute("multiple");
			if (NULL!=pValue)
			{
				pFishInfo->nMultiple=atoi(pValue);
			}
			pValue=pCurrentElement->Attribute("freeze");
			if (NULL!=pValue)
			{
				if (atoi(pValue)>0)
					pFishInfo->nProperty|=enFreeze;
			}
			pValue=pCurrentElement->Attribute("bomb");
			if (NULL!=pValue)
			{
				if (atoi(pValue)>0)
					pFishInfo->nProperty|=enBomb;
			}
			pValue=pCurrentElement->Attribute("boss");
			if (NULL!=pValue)
			{
				if (atoi(pValue)>0)
					pFishInfo->nProperty|=enBoss;
			}
			pValue=pCurrentElement->Attribute("samedie");
			if (NULL!=pValue)
			{
				if (atoi(pValue)>0)
					pFishInfo->nProperty|=enSameDie;
			}
			pValue=pCurrentElement->Attribute("samedieid");
			if (NULL!=pValue)
			{
				pFishInfo->nSameDieID=atoi(pValue);
			}
			else
			{
				pFishInfo->nSameDieID=-1;
			}
			pValue=pCurrentElement->Attribute("rangebomb");
			if (NULL!=pValue)
			{
				if (atoi(pValue)>0)
					pFishInfo->nProperty|=enRangeBomb;
			}
			pValue=pCurrentElement->Attribute("jumpbomb");
			if (NULL!=pValue)
			{
				if (atoi(pValue)>0)
					pFishInfo->nProperty|=enJumpBomb;
			}
			pValue=pCurrentElement->Attribute("plot");
			if (NULL!=pValue)
			{
				pFishInfo->nPlotID=atoi(pValue);
			}
			pValue=pCurrentElement->Attribute("name");
			if (NULL!=pValue)
			{
				std::wstring str1;
				utf8ToUnicode(pValue,str1);
//#ifdef _UNICODE
//				pFishInfo->sName.Format(TEXT("%s"),str1.c_str());
//#else
				std::string str2;
				unicodeToAnsi(str1.c_str(),str2);
				pFishInfo->sName.Format(TEXT("%s"),str2.c_str());
//#endif
			}
		}
		else if (0==nodename.compare("notice"))
		{
			tagNotice * pNotice=NULL;
			pNotice=new tagNotice;
			m_vNotice.push_back(pNotice);

			pValue=pCurrentElement->Attribute("notice_id");
			if (NULL!=pValue)
			{
				pNotice->nID=atoi(pValue);
			}
			pValue=pCurrentElement->Attribute("info");
			if (NULL!=pValue)
			{
				std::wstring str1;
				utf8ToUnicode(pValue,str1);
//#ifdef _UNICODE
//				pNotice->sNotice.Format(TEXT("%s"),str1.c_str());
//#else
				std::string str2;
				unicodeToAnsi(str1.c_str(),str2);
				pNotice->sNotice.Format(TEXT("%s"),str2.c_str());
//#endif
			}
		}
		else if (0==nodename.compare("plot"))
		{
			tagPlotInfo * pPlotInfo=NULL;
			pPlotInfo=new tagPlotInfo;
			ZeroMemory(pPlotInfo,sizeof(tagPlotInfo));

			pValue=pCurrentElement->Attribute("id");
			if (NULL!=pValue)
			{
				pPlotInfo->nID=atoi(pValue);
			}

			pValue=pCurrentElement->Attribute("type");
			if (NULL!=pValue)
			{
				pPlotInfo->nType=atoi(pValue);
			}

			pValue=pCurrentElement->Attribute("not_normal");
			if (NULL!=pValue)
			{
				pPlotInfo->bNotNormal=(atoi(pValue)>0);
			}

			pValue=pCurrentElement->Attribute("percent");
			if (NULL!=pValue)
			{
				pPlotInfo->nPercent=atoi(pValue);
			}

			pValue=pCurrentElement->Attribute("elapse");
			if (NULL!=pValue)
			{
				pPlotInfo->fElapse=(float)atof(pValue);
			}

			pValue=pCurrentElement->Attribute("execute_count");
			if (NULL!=pValue)
			{
				pPlotInfo->nExecuteCount=atoi(pValue);
			}

			pValue=pCurrentElement->Attribute("execute_range");
			if (NULL!=pValue)
			{
				pPlotInfo->nExecuteRange=atoi(pValue);
			}

			pValue=pCurrentElement->Attribute("fish_id_begin");
			if (NULL!=pValue)
			{
				pPlotInfo->nFishIDBegin=atoi(pValue);
			}

			pValue=pCurrentElement->Attribute("fish_id_range");
			if (NULL!=pValue)
			{
				pPlotInfo->nFishIDRange=atoi(pValue);
				if (pPlotInfo->nFishIDRange<=0)
					ERROR_LOG;
			}

			pValue=pCurrentElement->Attribute("path_index_begin");
			if (NULL!=pValue)
			{
				pPlotInfo->nPathIndexBegin=atoi(pValue);
			}

			pValue=pCurrentElement->Attribute("path_index_range");
			if (NULL!=pValue)
			{
				pPlotInfo->nPathIndexRange=atoi(pValue);
				if (pPlotInfo->nPathIndexRange<=0)
					ERROR_LOG;
			}

			pValue=pCurrentElement->Attribute("delay_time");
			if (NULL!=pValue)
			{
				pPlotInfo->nDelayTime=atoi(pValue);
			}
//printf("%d %d %d %d %d %d %d\n",pPlotInfo->nType,pPlotInfo->nFishIDBegin,pPlotInfo->nFishIDRange,pPlotInfo->nPathIndexBegin,pPlotInfo->nPathIndexRange,pPlotInfo->nExecuteCount,pPlotInfo->nDelayTime);
			pValue=pCurrentElement->Attribute("space_time");
			if (NULL!=pValue)
			{
				pPlotInfo->nSpaceTime=atoi(pValue);
			}

			pValue=pCurrentElement->Attribute("func");
			if (NULL!=pValue)
			{
				pPlotInfo->nFunc=atoi(pValue);
			}

			pValue=pCurrentElement->Attribute("param0");
			if (NULL!=pValue)
			{
				pPlotInfo->nParam0=atoi(pValue);
			}

			pValue=pCurrentElement->Attribute("param1");
			if (NULL!=pValue)
			{
				pPlotInfo->nParam1=atoi(pValue);
			}

			pValue=pCurrentElement->Attribute("param2");
			if (NULL!=pValue)
			{
				pPlotInfo->nParam2=atoi(pValue);
			}

			pValue=pCurrentElement->Attribute("param3");
			if (NULL!=pValue)
			{
				pPlotInfo->nParam3=atoi(pValue);
			}

			pValue=pCurrentElement->Attribute("param4");
			if (NULL!=pValue)
			{
				pPlotInfo->nParam4=atoi(pValue);
			}

			pValue=pCurrentElement->Attribute("param5");
			if (NULL!=pValue)
			{
				pPlotInfo->nParam5=atoi(pValue);
			}

			pValue=pCurrentElement->Attribute("param6");
			if (NULL!=pValue)
			{
				pPlotInfo->nParam6=atoi(pValue);
			}

			pValue=pCurrentElement->Attribute("group_pos");
			if (NULL!=pValue)
			{
				pPlotInfo->bGroupPos=(atoi(pValue)>0);
			}

			TiXmlElement * pChildElement2=pCurrentElement->FirstChildElement();
			while (pChildElement2!=NULL)
			{
				TiXmlElement * pCurrentElement2=pChildElement2;
				const char * pValue=NULL;
				pChildElement2=pChildElement2->NextSiblingElement();
				nodename=pCurrentElement2->Value();
				if (0==nodename.compare("point"))
				{
					int index=-1;
					float fRotation=0.0f;
					int nMoveTime=0;
					int nSpeed=0;
					pValue=pCurrentElement2->Attribute("index");
					if (NULL!=pValue)
					{
						index=atoi(pValue);
					}
					pValue=pCurrentElement2->Attribute("rotation");
					if (NULL!=pValue)
					{
						fRotation=(float)atof(pValue);
					}
					pValue=pCurrentElement2->Attribute("movetime");
					if (NULL!=pValue)
					{
						nMoveTime=atoi(pValue);
					}
					pValue=pCurrentElement2->Attribute("speed");
					if (NULL!=pValue)
					{
						nSpeed=atoi(pValue);
					}
					if (-1!=index&&index<PATH_POINT_COUNT)
					{
						pPlotInfo->fRotation[index]=fRotation;
						pPlotInfo->nMoveTime[index]=nMoveTime;
						pPlotInfo->nSpeed[index]=nSpeed;
					}
				}
			}

			m_vPlotInfo.push_back(pPlotInfo);
		}
		else if (0==nodename.compare("config"))
		{
			int nTag=0;
			pValue=pCurrentElement->Attribute("tag");
			if (NULL!=pValue)
			{
				nTag=atoi(pValue);
			}
			pValue=pCurrentElement->Attribute("cell_score");
			if (NULL!=pValue)
			{
				m_Config.nCellScore=atoi(pValue);
			}
			pValue=pCurrentElement->Attribute("min_score");
			if (NULL!=pValue)
			{
				m_Config.nMinScore=atoi(pValue);
			}
			pValue=pCurrentElement->Attribute("max_score");
			if (NULL!=pValue)
			{
				m_Config.nMaxScore=atoi(pValue);
			}
			pValue=pCurrentElement->Attribute("add_score");
			if (NULL!=pValue)
			{
				m_Config.nAddScore=atoi(pValue);
			}
			pValue=pCurrentElement->Attribute("exchange");
			if (NULL!=pValue)
			{
				m_Config.nExchange=atoi(pValue);
				if (m_Config.nExchange<=0)
				{
					ERROR_LOG;
					m_Config.nExchange=1;
				}
			}
			pValue=pCurrentElement->Attribute("max_bullet");
			if (NULL!=pValue)
			{
				m_Config.nMaxBullet=atoi(pValue);
			}
			pValue=pCurrentElement->Attribute("exercise");
			if (NULL!=pValue)
			{
				m_Config.nExercise=atoi(pValue);
			}
			pValue=pCurrentElement->Attribute("android_lock");
			if (NULL!=pValue)
			{
				m_Config.nAndroidLock=atoi(pValue);
			}
			pValue=pCurrentElement->Attribute("autostockdiffmaxfactor");
			if (NULL!=pValue)
			{
				m_Config.m_fAutoStockDiffMaxFactor=(float)atof(pValue);
			}
			pValue=pCurrentElement->Attribute("autostockdiffmaxvalue");
			if (NULL!=pValue)
			{
				m_Config.m_fAutoStockDiffMaxValue=(float)atof(pValue);
			}
			pValue=pCurrentElement->Attribute("timechangescene");
			if (NULL!=pValue)
			{
				m_nTimeChangeScene=atoi(pValue);
			}
			pValue=pCurrentElement->Attribute("plotnumber");
			if (NULL!=pValue)
			{
				m_nPlotNumber=atoi(pValue);
			}
			for (int i=0; i<CountArray(m_nPlotTime); i++)
			{
				char szAttribute[32];
				sprintf(szAttribute,"timeplot%d",i);
				pValue=pCurrentElement->Attribute(szAttribute);
				if (NULL!=pValue)
				{
					m_nPlotTime[i]=atoi(pValue);
				}
			}
			//比赛
			pValue=pCurrentElement->Attribute("checkmatchelapse");
			if (NULL!=pValue)
			{
				m_MatchManager.m_nMatchCheckElapse=atoi(pValue);
			}
			//配置开始日期
			pValue=pCurrentElement->Attribute("matchtimestart");
			if (NULL!=pValue)
			{
				do
				{
					if (0 == nTag)
					{
						continue;
					}

					unsigned int nMatchStart = atoi(pValue);
					int nCharCount = (int)strlen(pValue);
					tagMatchTimeDate matchdata;
					bool bError = false;

					switch(nCharCount)
					{
					case 2://26，每小时26分
						{
							matchdata.nMin = nMatchStart % 100;
						}
						break;
					case 4://1126，每天11点26分
						{
							matchdata.nMin  = nMatchStart % 100;
							matchdata.nHour = (nMatchStart / 100) % 100;
						}
						break;
					case 5://61126，每周六11点26分
						{
							matchdata.nMin  = nMatchStart % 100;
							matchdata.nHour = (nMatchStart / 100) % 100;
							matchdata.nWeek = (nMatchStart / 10000) % 100;
						}
						break;
					case 6://271126，每月27号11点26分
						{
							matchdata.nMin  = nMatchStart % 100;
							matchdata.nHour = (nMatchStart / 100) % 100;
							matchdata.nDay  = (nMatchStart / 10000) % 100;
						}
						break;
					case 9://1307271126，2013年07月27日11点26分
						{
							matchdata.nMin    = nMatchStart % 100;
							matchdata.nHour   = (nMatchStart / 100) % 100;
							matchdata.nDay    = (nMatchStart / 10000) % 100;
							matchdata.nMonth  = (nMatchStart / 1000000) % 100;   
							matchdata.nYear   = (nMatchStart / 100000000) % 100 + 2000; 
						}
						break;
					default:
						{
							ERROR_LOG;
							bError = true;
						}
						break;
					}

					if (bError)
					{
						continue;
					}

					tagMatchConfig * pMatchCfg = NULL;
					//查找
					for (int i = 0; i < (int)m_MatchManager.m_vMatchConfig.size(); i++)
					{
						if (nTag == m_MatchManager.m_vMatchConfig[i].nCfgID)
						{
							pMatchCfg = &m_MatchManager.m_vMatchConfig[i];
							break;
						}
					}
					//新建
					if (NULL == pMatchCfg)
					{
						m_MatchManager.m_vMatchConfig.push_back(tagMatchConfig());
						pMatchCfg = &m_MatchManager.m_vMatchConfig[m_MatchManager.m_vMatchConfig.size() - 1];
						pMatchCfg->nCfgID = nTag;
					}

					pMatchCfg->CfgStartDate = matchdata;

				} while (0);
			}
			//配置结束日期
			pValue=pCurrentElement->Attribute("matchtimeend");
			if (NULL!=pValue)
			{
				do
				{
					if (0 == nTag)
					{
						continue;
					}

					unsigned int nMatchStart = atoi(pValue);
					int nCharCount = (int)strlen(pValue);
					tagMatchTimeDate matchdata;
					bool bError = false;

					switch(nCharCount)
					{
					case 2://26，每小时26分
						{
							matchdata.nMin = nMatchStart % 100;
						}
						break;
					case 4://1126，每天11点26分
						{
							matchdata.nMin  = nMatchStart % 100;
							matchdata.nHour = (nMatchStart / 100) % 100;
						}
						break;
					case 5://61126，每周六11点26分
						{
							matchdata.nMin  = nMatchStart % 100;
							matchdata.nHour = (nMatchStart / 100) % 100;
							matchdata.nWeek = (nMatchStart / 10000) % 100;
						}
						break;
					case 6://271126，每月27号11点26分
						{
							matchdata.nMin  = nMatchStart % 100;
							matchdata.nHour = (nMatchStart / 100) % 100;
							matchdata.nDay  = (nMatchStart / 10000) % 100;
						}
						break;
					case 9://1307271126，2013年07月27日11点26分
						{
							matchdata.nMin    = nMatchStart % 100;
							matchdata.nHour   = (nMatchStart / 100) % 100;
							matchdata.nDay    = (nMatchStart / 10000) % 100;
							matchdata.nMonth  = (nMatchStart / 1000000) % 100;   
							matchdata.nYear   = (nMatchStart / 100000000) % 100 + 2000; 
						}
						break;
					default:
						{
							ERROR_LOG;
							bError = true;
						}
						break;
					}

					if (bError)
					{
						continue;
					}

					tagMatchConfig * pMatchCfg = NULL;
					//查找
					for (int i = 0; i < (int)m_MatchManager.m_vMatchConfig.size(); i++)
					{
						if (nTag == m_MatchManager.m_vMatchConfig[i].nCfgID)
						{
							pMatchCfg = &m_MatchManager.m_vMatchConfig[i];
							break;
						}
					}
					//新建
					if (NULL == pMatchCfg)
					{
						m_MatchManager.m_vMatchConfig.push_back(tagMatchConfig());
						pMatchCfg = &m_MatchManager.m_vMatchConfig[m_MatchManager.m_vMatchConfig.size() - 1];
						pMatchCfg->nCfgID = nTag;
					}

					pMatchCfg->CfgEndDate = matchdata;

				} while (0);
			}
			//比赛积分重置类型
			pValue=pCurrentElement->Attribute("matchresettype");
			if (NULL!=pValue)
			{
				do
				{
					if (0 == nTag)
					{
						continue;
					}

					tagMatchConfig * pMatchCfg = NULL;
					//查找
					for (int i = 0; i < (int)m_MatchManager.m_vMatchConfig.size(); i++)
					{
						if (nTag == m_MatchManager.m_vMatchConfig[i].nCfgID)
						{
							pMatchCfg = &m_MatchManager.m_vMatchConfig[i];
							break;
						}
					}

					//新建
					if (NULL == pMatchCfg)
					{
						m_MatchManager.m_vMatchConfig.push_back(tagMatchConfig());
						pMatchCfg = &m_MatchManager.m_vMatchConfig[m_MatchManager.m_vMatchConfig.size() - 1];
						pMatchCfg->nCfgID = nTag;
					}

					pMatchCfg->nCfgResetType = atoi(pValue);

				} while (0);
			}
			//比赛标识
			pValue=pCurrentElement->Attribute("matchflag");
			if (NULL!=pValue)
			{
				do
				{
					if (0 == nTag)
					{
						continue;
					}

					tagMatchConfig * pMatchCfg = NULL;
					//查找
					for (int i = 0; i < (int)m_MatchManager.m_vMatchConfig.size(); i++)
					{
						if (nTag == m_MatchManager.m_vMatchConfig[i].nCfgID)
						{
							pMatchCfg = &m_MatchManager.m_vMatchConfig[i];
							break;
						}
					}

					//新建
					if (NULL == pMatchCfg)
					{
						m_MatchManager.m_vMatchConfig.push_back(tagMatchConfig());
						pMatchCfg = &m_MatchManager.m_vMatchConfig[m_MatchManager.m_vMatchConfig.size() - 1];
						pMatchCfg->nCfgID = nTag;
					}

					pMatchCfg->nCfgFlag = atoi(pValue);

				} while (0);
			}
			//比赛发送信息
			pValue=pCurrentElement->Attribute("sendmatchinfo");
			if (NULL!=pValue)
			{
				do
				{
					if (0 == nTag)
					{
						continue;
					}

					tagMatchConfig * pMatchCfg = NULL;
					//查找
					for (int i = 0; i < (int)m_MatchManager.m_vMatchConfig.size(); i++)
					{
						if (nTag == m_MatchManager.m_vMatchConfig[i].nCfgID)
						{
							pMatchCfg = &m_MatchManager.m_vMatchConfig[i];
							break;
						}
					}

					//新建
					if (NULL == pMatchCfg)
					{
						m_MatchManager.m_vMatchConfig.push_back(tagMatchConfig());
						pMatchCfg = &m_MatchManager.m_vMatchConfig[m_MatchManager.m_vMatchConfig.size() - 1];
						pMatchCfg->nCfgID = nTag;
					}

					std::wstring str1;
					utf8ToUnicode(pValue,str1);
					std::string str2;
					unicodeToAnsi(str1.c_str(),str2);
					pMatchCfg->m_sSendMatchInfo = str2;

				} while (0);
			}
			//比赛发送信息间隔
			pValue=pCurrentElement->Attribute("sendmatchinfoelapse");
			if (NULL!=pValue)
			{
				do
				{
					if (0 == nTag)
					{
						continue;
					}

					tagMatchConfig * pMatchCfg = NULL;
					//查找
					for (int i = 0; i < (int)m_MatchManager.m_vMatchConfig.size(); i++)
					{
						if (nTag == m_MatchManager.m_vMatchConfig[i].nCfgID)
						{
							pMatchCfg = &m_MatchManager.m_vMatchConfig[i];
							break;
						}
					}

					//新建
					if (NULL == pMatchCfg)
					{
						m_MatchManager.m_vMatchConfig.push_back(tagMatchConfig());
						pMatchCfg = &m_MatchManager.m_vMatchConfig[m_MatchManager.m_vMatchConfig.size() - 1];
						pMatchCfg->nCfgID = nTag;
					}

					pMatchCfg->m_nSendMatchInfoElapse = atoi(pValue);

				} while (0);
			}
		}
		else
		{
			ERROR_LOG;
			continue;
		}
	}
	delete xmldoc;
}

//填充鱼数据
tagFishPack * CTableFrameSink::fillFishPack(int nSpriteID, int nPathIndex1, int nCreateDelayTime)
{
	tagFishPack * pFishPack=m_FishFactory.ActiveItem();
	if (NULL==pFishPack)
		ERROR_LOG;

	//鱼信息
	tagFishInfo * pFishInfo=getFishInfo(nSpriteID);

	if (NULL!=pFishPack)
	{
		ZeroMemory(pFishPack,sizeof(tagFishPack));
		pFishPack->nCreateTime=GetTickCount();
		pFishPack->nFishID=GetNewFishID();
		pFishPack->nSpriteID=nSpriteID;
		pFishPack->nPathIndex1=nPathIndex1;
		pFishPack->nCreateDelayTime=nCreateDelayTime;
		if (NULL!=pFishInfo)
		{
			pFishPack->nMultiple=pFishInfo->nMultiple;
			pFishPack->nProperty=pFishInfo->nProperty;
		}
	}
	return pFishPack;
}

//开炮处理
bool CTableFrameSink::OnSubFire(IServerUserItem * pIServerUserItem, float fAngle, DWORD dwBulletID, int nCount, BYTE cbAttrib)
{
	if (NULL==pIServerUserItem)
	{
		ERROR_LOG;
		return true;
	}
	if (nCount<1)
	{
		ERROR_LOG;
		return true;
	}
	WORD wChairID=pIServerUserItem->GetChairID();
	tagPlayerInfo & PlayerInfo=m_PlayerInfo[wChairID];
	if (!PlayerInfo.bTryCannon)
	{
		//验证分数
		if (PlayerInfo.lUserAllScore<=0) return true;
		if (PlayerInfo.lUserAllScore-PlayerInfo.lUserCellScore*nCount<0) return true;
		if (PlayerInfo.lUserCellScore<m_Config.nCellScore) return false;
	}
	//当前子弹数
	int nCurrentShootCount=PlayerInfo.getBulletCount();
	//验证子弹数
	if (nCurrentShootCount>=m_Config.nMaxBullet) return true;

	PlayerInfo.nOutTime=0;
	bool bRealUser=false;
	for (int i=0;i<GAME_PLAYER;i++)
	{
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(i);
		if (pIServerUserItem!=NULL)
		{
			if (!pIServerUserItem->IsAndroidUser())
			{
				bRealUser=true;
				break;
			}
		}
	}
	if (!bRealUser) return true;

	////开炮太快则忽略
	//if (GetTickCount()-PlayerInfo.nSendTime<30)
	//{
	//	PlayerInfo.nSendTime=GetTickCount();
	//	//PlayerInfo.nShootCount=0;
	//	//PlayerInfo.ClearBullet();
	//	return true;
	//}
	//PlayerInfo.nSendTime=GetTickCount();

	//试用状态不扣分
	if (!PlayerInfo.bTryCannon)
	{
		//玩家减去开炮分数
		PlayerInfo.lUserAllScore-=PlayerInfo.lUserCellScore*nCount;
		//char szMsg[100];
		//sprintf( szMsg, "玩家id = %d,分数=%ld，增加分数lUserCellScore*nCount=%d,子弹id=%d\n",pIServerUserItem->GetUserID(),PlayerInfo.lUserAllScore,-PlayerInfo.lUserCellScore*nCount,dwBulletID);
		//OnRecord(TEXT("before 2\n"));
		//OnRecord(szMsg);
		PlayerInfo.nTotalWinScore-=PlayerInfo.lUserCellScore*nCount;
		PlayerInfo.nThisTimeWinScore-=PlayerInfo.lUserCellScore*nCount;
	}

	CMD_S_UserShoot UserShoot={0};
	UserShoot.wChairID=wChairID;
	UserShoot.fAngle=fAngle;
	UserShoot.bAndroid=pIServerUserItem->IsAndroidUser();
	UserShoot.cbAttrib=cbAttrib;

	for (DWORD i=0; i<nCount; i++)
	{
		UserShoot.dwBulletID=dwBulletID+i;
	}
	UserShoot.dwCount=nCount;
	UserShoot.nUserScore=PlayerInfo.lUserAllScore + (PlayerInfo.bTryCannon?PlayerInfo.lUserTryScore:0);

	//练习房间和副本房间，不消耗装备的龙珠幸运点
	if (PlayerInfo.nEquipmentDragonCount > 0 && m_Config.nExercise == 0)
	{
		PlayerInfo.nEquipmentDragonCount -= nCount;
		if (PlayerInfo.nEquipmentDragonCount <= 0)
		{
			PlayerInfo.nEquipmentDragonCount = 0;
			CMD_S_EquipmentDragonCount EquipmentDragonCount={0};
			for (int i = 0; i < GAME_PLAYER; i++)
			{
				EquipmentDragonCount.nEquipmentDragonCount[i] = m_PlayerInfo[i].nEquipmentDragonCount;
			}
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_EQUIPMENT_DRAGON_COUNT,&EquipmentDragonCount,sizeof(EquipmentDragonCount));
		}
	}

	SendDataExcludeSourceUser(pIServerUserItem,SUB_S_USER_SHOOT,&UserShoot,sizeof(UserShoot));
	UserShoot.nUserScore=PlayerInfo.lUserAllScore;
	m_pITableFrame->SendTableData(pIServerUserItem->GetChairID(),SUB_S_USER_SHOOT,&UserShoot,sizeof(UserShoot));

	int nChangeScore=0;
	int nBulletScore=0;

	//试用状态不改变库存
	if (!PlayerInfo.bTryCannon)
	{
		nChangeScore=PlayerInfo.lUserCellScore*nCount;
		nBulletScore=PlayerInfo.lUserCellScore;
		//int nRevenue=0;
		//int nOptRevenue=m_pGameServiceOption->wRevenueRatio;

		////大于0抽水
		//if (nOptRevenue>0 && nChangeScore>0)
		//{
		//	//抽水是千分比
		//	nRevenue=nChangeScore*nOptRevenue;
		//	//储存小抽水值
		//	nRevenue+=m_nLittleRevenue;
		//	m_nLittleRevenue=nRevenue%1000;
		//	nRevenue=nRevenue/1000;
		//	if (nRevenue>nChangeScore)
		//	{
		//		nRevenue=nChangeScore;
		//	}
		//}

		////printf("nOptRevenue=%d,m_nLittleRevenue=%d,nRevenue=%d,nChangeScore=%d\n",nOptRevenue,m_nLittleRevenue,nRevenue,nChangeScore-nRevenue);
		//m_pITableFrame->ChangeStock(pIServerUserItem->IsAndroidUser()?eStockType_Robot:eStockType_Player,pIServerUserItem->GetChairID(),nChangeScore-nRevenue,nRevenue,0);
		PlayerInfo.fire(nChangeScore);
		if (fcalcnew != NULL)
		{
			fprintf(fcalcnew, "fire UserID %u Gains = %lld Consume = %lld Yields = %f Crr = %f\n", pIServerUserItem->GetUserID(), PlayerInfo.lGains, PlayerInfo.lConsume, PlayerInfo.fYields, PlayerInfo.fCurve[PlayerInfo.nPointCount]);
			fflush(fcalcnew);
		}
	}
	else
	{
		//试用状态固定倍率
		nBulletScore=100;
	}

	//子弹计数
	PlayerInfo.nShootCount++;

	bool bHitMore = false;
	if (PlayerInfo.cbCannonType == 2 || PlayerInfo.cbCannonType == 4 || PlayerInfo.cbCannonType == 7 || PlayerInfo.cbCannonType == 3 || PlayerInfo.cbCannonType == 5 || PlayerInfo.cbCannonType == 8 || PlayerInfo.cbCannonType == 10 || PlayerInfo.cbCannonType == 9)
	{
		bHitMore = true;
	}

	//将子弹加入数组
	PlayerInfo.addBullet(dwBulletID,nBulletScore,PlayerInfo.bTryCannon,bHitMore,_GetTickCount64());

	return true;
}

//炮台倍率处理
bool CTableFrameSink::OnSubCannonMultiple(IServerUserItem * pIServerUserItem, BYTE cbAddMultiple, BYTE cbCannonType, int nMultiple)
{
	WORD wChairID=pIServerUserItem->GetChairID();
	tagPlayerInfo & PlayerInfo=m_PlayerInfo[wChairID];

	if (nMultiple==0)
	{
		if (cbAddMultiple == 1)
		{
			if (m_Config.nMaxScore==99)
			{
				if (PlayerInfo.lUserCellScore==90)
				{
					PlayerInfo.lUserCellScore=99;
				}
				else if (PlayerInfo.lUserCellScore==99)
				{
					PlayerInfo.lUserCellScore=10;
				}
				else
				{
					PlayerInfo.lUserCellScore+=m_Config.nCellScore;
				}
			}
			else
			{
				PlayerInfo.lUserCellScore+=m_Config.nCellScore;
			}
		}
		else if (cbAddMultiple == 2)
		{
			if (m_Config.nMaxScore==99)
			{
				if (PlayerInfo.lUserCellScore==99)
				{
					PlayerInfo.lUserCellScore=90;
				}
				else if (PlayerInfo.lUserCellScore==10)
				{
					PlayerInfo.lUserCellScore=99;
				}
				else
				{
					PlayerInfo.lUserCellScore-=m_Config.nCellScore;
				}
			}
			else
			{
				PlayerInfo.lUserCellScore-=m_Config.nCellScore;
			}
		}
	}
	else
	{
		PlayerInfo.lUserCellScore=m_Config.nCellScore*nMultiple;
	}

	if (cbCannonType > 12)
	{
		cbCannonType = 0;
	}

	if (PlayerInfo.cbCannonType != cbCannonType)
	{
		PlayerInfo.cbCannonType = cbCannonType;

		int nCannonStatus = pIServerUserItem->GetCannonStatus();
		if (nCannonStatus & (1 << cbCannonType))
		{
			SaveUserItem(pIServerUserItem,
				eUserItemLastCannon,
				eUserItemFlag_Heap|eUserItemFlag_GamePreLoad|eUserItemFlag_OverWrite,
				PlayerInfo.cbCannonType,false);
		}
	}

	if (PlayerInfo.lUserCellScore>m_Config.nMaxScore)
		PlayerInfo.lUserCellScore=m_Config.nMinScore;
	if (PlayerInfo.lUserCellScore<m_Config.nMinScore)
		PlayerInfo.lUserCellScore=m_Config.nMaxScore;

	CMD_S_CannonMultiple CannonMultiple={0};
	CannonMultiple.wChairID=wChairID;
	CannonMultiple.nCannonMultiple=PlayerInfo.lUserCellScore;
	CannonMultiple.cbCannonType=PlayerInfo.cbCannonType;
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_BONUS_INFO,&CannonMultiple,sizeof(CannonMultiple));

	PlayerInfo.nMultipleTimes++;

	changeLucky(PlayerInfo);

	return true;
}

//购买子弹
bool CTableFrameSink::OnSubBuyBullet(IServerUserItem * pIServerUserItem, bool bAdd, int nScore)
{
	WORD wChairID=pIServerUserItem->GetChairID();
	tagPlayerInfo & PlayerInfo=m_PlayerInfo[wChairID];

	tagScoreInfo ScoreInfoArray;
	ZeroMemory(&ScoreInfoArray,sizeof(ScoreInfoArray));
	CMD_S_BulletCount BulletCount={0};

	//@练习房间，下分
	if (m_Config.nExercise>0)
	{
		if (!bAdd)
		{
			BulletCount.bAdd=bAdd;
			BulletCount.lScore=PlayerInfo.lUserAllScore;
			BulletCount.wChairID=wChairID;
			PlayerInfo.lUserAllScore=0;
			PlayerInfo.ClearBullet();
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_BULLET_COUNT,&BulletCount,sizeof(BulletCount));
		}
		return true;
	}

	//上分
	if (bAdd)
	{
		if (nScore<0)
		{
			ERROR_LOG;
			return true;
		}
		if (pIServerUserItem->GetUserScore()<=0)
		{
			return true;
		}
		int lScore=m_Config.nAddScore;
		if (nScore>0)
		{
			lScore=nScore;
		}
		if (pIServerUserItem->GetUserScore()<lScore)
		{
			lScore=pIServerUserItem->GetUserScore();
		}
		//int lScore=pIServerUserItem->GetUserScore()/m_Config.nExchange;

		//if (0!=PlayerInfo.lUserAllScore)
		//{
		//	ERROR_LOG;
		//	return true;
		//}

		PlayerInfo.lUserAllScore+=lScore;
		//OnRecord(TEXT("before 3\n"));
		//char szMsg[100];
		//sprintf( szMsg, "玩家id = %d,分数=%ld，增加分数lScore=%d\n",pIServerUserItem->GetUserID(),PlayerInfo.lUserAllScore,lScore);
		//OnRecord(szMsg);

		BulletCount.bAdd=bAdd;
		BulletCount.lScore=PlayerInfo.lUserAllScore;
		BulletCount.wChairID=wChairID;

		ScoreInfoArray.lScore=-lScore*m_Config.nExchange;

		PlayerInfo.nBuyTimes++;

		PlayerInfo.lUserEnterScore = PlayerInfo.lUserAllScore;
	}
	else
	{
		BulletCount.bAdd=bAdd;
		BulletCount.lScore=PlayerInfo.lUserAllScore;
		BulletCount.wChairID=wChairID;

		ScoreInfoArray.lScore=PlayerInfo.lUserAllScore*m_Config.nExchange;

		PlayerInfo.lUserAllScore=0;
		PlayerInfo.ClearBullet();
	}
	ScoreInfoArray.lRevenue=0;
	if (ScoreInfoArray.lScore>0) ScoreInfoArray.cbType=SCORE_TYPE_WIN;
	else if (ScoreInfoArray.lScore<0) ScoreInfoArray.cbType=SCORE_TYPE_LOSE;
	else ScoreInfoArray.cbType=SCORE_TYPE_DRAW;

	if (m_pITableFrame->GetTableUserItem(wChairID)!=NULL)
		m_pITableFrame->WriteUserScore(wChairID,ScoreInfoArray);
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_BULLET_COUNT,&BulletCount,sizeof(BulletCount));

	return true;
}

//存储用户物品
void CTableFrameSink::SaveUserItem(IServerUserItem * pIServerUserItem, int nType, int nFlag, SCORE lCount, bool bSaveRobot)
{
	if (NULL == pIServerUserItem)
	{
		ERROR_LOG;
		return;
	}

	//选择是否记录机器人
	if (!bSaveRobot && pIServerUserItem->IsAndroidUser())
	{
		return;
	}

	int nUserItemCount = pIServerUserItem->GetUserItemCount();
	tagUserItem * pUserItem = pIServerUserItem->GetUserItems();
	tagUserItem * pSelectUserItem = NULL;
	int nIndex = -1;

	for (int i = 0; i < nUserItemCount; i++)
	{
		tagUserItem * pTmpUserItem = pUserItem + i;

		if (pTmpUserItem->nType == nType)
		{
			pSelectUserItem = pTmpUserItem;
			nIndex = i;
			break;
		}
	}

	if (NULL == pSelectUserItem)
	{
		tagUserItem useritem;
		memset(&useritem, 0, sizeof(useritem));
		useritem.lCount = lCount;
		useritem.nUserID = pIServerUserItem->GetUserID();
		useritem.nType = nType;
		useritem.nFlag = nFlag;
		useritem.nSourceID = m_pGameServiceOption->wServerID;
		useritem.nSubSourceID = m_pITableFrame->GetTableID();

		if (pIServerUserItem->AddUserItem(&useritem))
		{
			m_pITableFrame->SaveUserItem(pIServerUserItem, nIndex);
		}
		else
		{
			ERROR_LOG;
		}
	}
	else
	{
		if (0 != (eUserItemFlag_OverWrite & nFlag))
		{
			pSelectUserItem->lCount = lCount;
		}
		else
		{
			pSelectUserItem->lCount += lCount;
		}

		m_pITableFrame->SaveUserItem(pIServerUserItem, nIndex);
	}
}

//圆圈剧情
void CTableFrameSink::plotCircle(int nPlotID, int nXPos, int nYPos)
{
	BYTE cbBuffer[2048]={0};
	WORD wSendSize=0;
	tagFishPack * pFishTrace=(tagFishPack *)cbBuffer;

	tagPlotInfo * pPlotInfo=getPlotInfo(nPlotID);
	if (NULL==pPlotInfo)
	{
		ERROR_LOG;
		return;
	}

	//鱼种类
	int nSpriteID=pPlotInfo->nFishIDBegin+rand()%pPlotInfo->nFishIDRange;
	//路线索引
	int nPathIndex=pPlotInfo->nPathIndexBegin+rand()%pPlotInfo->nPathIndexRange;

	if (10==pPlotInfo->nType)
	{
		//时间间隔
		unsigned int dwNeedTick=(unsigned int)(pPlotInfo->fElapse*1000);
		unsigned int dwCurrentTick=GetTickCount();
		if (dwCurrentTick-pPlotInfo->dwLastTick<dwNeedTick)
			return;
		else
			pPlotInfo->dwLastTick=dwCurrentTick;

		//生成概率
		int nPercent=rand()%100;
		//判断几率
		if (pPlotInfo->nPercent>0 && nPercent<pPlotInfo->nPercent)
		{
			//圈数
			int nCircleCount=pPlotInfo->nParam0;
			//每圈的鱼数
			int nFishCount=pPlotInfo->nParam1;

			if (0==nFishCount)
			{
				ERROR_LOG;
				return;
			}
			//随机红鱼位置
			int nTempCircle=rand()%nCircleCount;
			int nTempFish=rand()%nFishCount;
			for (int j=0; j<nCircleCount; j++)
			{
				for (int k=0; k<nFishCount; k++)
				{
					tagFishPack * pFishPack=fillFishPack(nSpriteID,-1,pPlotInfo->nDelayTime+j*pPlotInfo->nSpaceTime);
					if (NULL!=pFishPack)
					{
						pFishPack->nXPos=nXPos;
						pFishPack->nYPos=nYPos;
						pFishPack->fRotation[0]=360.0f*k/nFishCount;
						pFishPack->nMoveTime[0]=1000000;
						pFishPack->nSpeed[0]=4;
						if (j==nTempCircle && k==nTempFish)
						{
							pFishPack->nProperty|=enLine;
						}
						CopyMemory(pFishTrace,pFishPack,sizeof(tagFishPack));
						wSendSize+=sizeof(tagFishPack);
						++pFishTrace;
						if (wSendSize+sizeof(tagFishPack)>sizeof(cbBuffer))
						{
							m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_TRACE_POINT,cbBuffer,wSendSize);
							pFishTrace=(tagFishPack *)cbBuffer;
							wSendSize=0;
						}
					}
				}
			}
		}
	}
	if (wSendSize>0)
	{
		if (wSendSize>sizeof(cbBuffer))
			ERROR_LOG;
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_TRACE_POINT,cbBuffer,wSendSize);
	}
}

//游戏记录
void CTableFrameSink::saveGameJson(IServerUserItem * pIServerUserItem, tagPlayerInfo & PlayerInfo, bool bStandUp)
{
	if (NULL==pIServerUserItem)
	{
		return;
	}
	//机器人不记录
	if (pIServerUserItem->IsAndroidUser())
	{
		return;
	}
	//@练习房间不记录
	if (m_Config.nExercise>0)
	{
		return;
	}

	TCHAR szRecord[1024]={0};

	if (bStandUp)
	{
		//未开炮不记录
		if (PlayerInfo.nShootCount<=0)
		{
			return;
		}

		CString sRecord;
		CString sFishItem;

		for (unsigned int i=0; i<PlayerInfo.vFishCount.size(); i++)
		{
			if (i != 0)
			{
				sFishItem += ",";
			}
			char szFishItem[64];
			sprintf(szFishItem, "{\"C\":%d,\"ID\":%d}", PlayerInfo.vFishCount[i].nCount, PlayerInfo.vFishCount[i].nID);
			sFishItem += szFishItem;
		}

		sRecord.Format("{\"BuyC\":%d,\"ChangeC\":%d,\"FishC\":[%s],\"FishTotalC\":%d,\"InsureScore\":%d,\"PassT\":%d,\"Score\":%d,\"ShootC\":%d,\"WinScore\":%d}",
			PlayerInfo.nBuyTimes, PlayerInfo.nMultipleTimes, sFishItem, PlayerInfo.nFishCount, (int)(pIServerUserItem->GetUserInsure()),
			(int)(_time64(NULL)-PlayerInfo.nPassTime), (int)(pIServerUserItem->GetUserScore()), PlayerInfo.nShootCount, PlayerInfo.nThisTimeWinScore);

		strncpy(szRecord, sRecord, sizeof(szRecord) - 1);
	}
	else
	{
		//坐下记录分数，防止玩家掉分
		sprintf(szRecord, "{\"InsureScore\":%d,\"Score\":%d}", (int)(pIServerUserItem->GetUserInsure()), (int)(pIServerUserItem->GetUserScore()));
	}

	m_pITableFrame->SaveGameJson(pIServerUserItem,szRecord,PlayerInfo.nThisTimeWinScore);
}

//获取游戏内部分数
LONG CTableFrameSink::GetInsideScore(WORD wChairID)
{
	//@练习房间返回0
	if (m_Config.nExercise>0)
	{
		return 0;
	}
	if (wChairID>=GAME_PLAYER) return 0;
	tagPlayerInfo & PlayerInfo=m_PlayerInfo[wChairID];
	return PlayerInfo.lUserAllScore;
}

//结算
void CTableFrameSink::calc(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	if (bLookonUser) return;
	if (pIServerUserItem==NULL)
	{
		ERROR_LOG;
		return;
	}

	tagPlayerInfo & PlayerInfo=m_PlayerInfo[wChairID];
	if (PlayerInfo.lUserAllScore>0)
	{
		//@练习房间
		if (m_Config.nExercise==0)
		{
			SCORE lScore=PlayerInfo.lUserAllScore*m_Config.nExchange;
			//if (NULL!=m_pITableFrame->GetTableUserItem(wChairID))
			{
				pIServerUserItem->WriteUserScore(lScore,0,0,0,SCORE_TYPE_WIN,0);
			}
		}
	}

	//@练习房间
	if (m_Config.nExercise==0)
	{
		__int64 nPassTime=0;

		if (0!=PlayerInfo.nPassTime)
		{
			nPassTime=_time64(NULL)-PlayerInfo.nPassTime;
		}

		//存游戏时长
		if (nPassTime>0)
		{
			PlayerInfo.nElapseTime+=nPassTime;
			//数据验证
			if (PlayerInfo.nElapseTime<0)
			{
				PlayerInfo.nElapseTime=3600*24*3;//3天
			}
			else if (PlayerInfo.nElapseTime>100000000)//3600*24*1157
			{
				PlayerInfo.nElapseTime=3600*24*7;//一星期时间
			}
			SaveUserItem(pIServerUserItem,
				eUserItem_ElapseTime,
				eUserItemFlag_Heap|eUserItemFlag_GamePreLoad|eUserItemFlag_OverWrite,
				PlayerInfo.nElapseTime,false);
		}

		if (PlayerInfo.nTotalWinScore!=0)
		{
			//存游戏盈利
			SaveUserItem(pIServerUserItem,
				eUserItem_TotalWinScore,
				eUserItemFlag_Heap|eUserItemFlag_GamePreLoad|eUserItemFlag_OverWrite,
				PlayerInfo.nTotalWinScore,false);
		}

		SaveUserItem(pIServerUserItem,
			eUserItem_nPointCount,
			eUserItemFlag_Heap|eUserItemFlag_GamePreLoad|eUserItemFlag_OverWrite,
			PlayerInfo.nPointCount,false);

		SaveUserItem(pIServerUserItem,
			eUserItem_nFireCount,
			eUserItemFlag_Heap|eUserItemFlag_GamePreLoad|eUserItemFlag_OverWrite,
			PlayerInfo.nFireCount,false);

		SaveUserItem(pIServerUserItem,
			eUserItem_lGains,
			eUserItemFlag_Heap|eUserItemFlag_GamePreLoad|eUserItemFlag_OverWrite,
			PlayerInfo.lGains,false);

		SaveUserItem(pIServerUserItem,
			eUserItem_lConsume,
			eUserItemFlag_Heap|eUserItemFlag_GamePreLoad|eUserItemFlag_OverWrite,
			PlayerInfo.lConsume,false);

printf("保存 %d %d %lld %lld\n", PlayerInfo.nPointCount, PlayerInfo.nFireCount, PlayerInfo.lGains, PlayerInfo.lConsume);

#ifndef KIND_7
		//存比赛得分
		for (int i = 0; i < (int)PlayerInfo.vMatchInfo.size(); i++)
		{
			int nType = PlayerInfo.vMatchInfo[i].nFlag;
			SCORE lCount = PlayerInfo.vMatchInfo[i].lWinScore;

			if (0 == lCount)
			{
				continue;
			}

			SaveUserItem(pIServerUserItem,
				nType,
				eUserItemFlag_Heap | eUserItemFlag_GamePreLoad | eUserItemFlag_OverWrite | eUserItemFlag_Match,
				lCount, true);
		}
#endif
		saveGameJson(pIServerUserItem,PlayerInfo,true);
	}
	SaveUserItem(pIServerUserItem,
		eUserItem_EquipmentDragonCount,
		eUserItemFlag_Heap|eUserItemFlag_GamePreLoad|eUserItemFlag_OverWrite,
		PlayerInfo.nEquipmentDragonCount,false);

	PlayerInfo.Clear();
	//@@@
	for (int i = 0; i < 3; i++)
	{
		m_ManEatingFish[wChairID].nFishID[i] = 0;
		m_ManEatingFish[wChairID].nSpriteID[i] = 0;
		m_ManEatingFish[wChairID].nEatScore[i] = 0;
		m_ManEatingFish[wChairID].bIsDead[i] = false;
	}

	//玩家数目
	WORD wUserCount=0;
	for (WORD wIndex=0;wIndex<GAME_PLAYER;++wIndex)
	{
		if (NULL!=m_pITableFrame->GetTableUserItem(wIndex)) wUserCount++;
	}

	//没有玩家
	if (0==wUserCount) 
	{
		//结束游戏
		m_pITableFrame->ConcludeGame(GAME_SCENE_FREE);
		//取消时间
		m_pITableFrame->KillGameTimer(TimerClearTrace);
		m_pITableFrame->KillGameTimer(TimerChangeScene);
		m_pITableFrame->KillGameTimer(TimerCreateFish);
		m_pITableFrame->KillGameTimer(TimerCheckUser);
		m_pITableFrame->KillGameTimer(17);

		//如果桌子上没有玩家, 清空食人鱼信息
		m_bManEatingFish = false;
		m_nManEatingFishTime = 0;
		m_pITableFrame->KillGameTimer(TimerManEatingFish);
		ZeroMemory(&m_ManEatingFish,sizeof(m_ManEatingFish));

		m_FishFactory.FreeAllItem();
		//设置状态
		m_pITableFrame->SetGameStatus(GAME_SCENE_FREE);
	}
}

//新手引导奖励
bool CTableFrameSink::OnNewGuidGift(IServerUserItem * pIServerUserItem, bool bSuccess, int nCannonStatus, int nLotteryCount)
{
	if (pIServerUserItem == NULL)
	{
		return false;
	}

	WORD wChairID = pIServerUserItem->GetChairID();
	tagPlayerInfo & PlayerInfo = m_PlayerInfo[wChairID];

	int nScoreGift = 0;

	if (bSuccess)
	{
		nScoreGift = 5000;

		PlayerInfo.nLotteryGeted += nLotteryCount;
		SaveUserItem(pIServerUserItem,
			eUserItem_LotteryInfo,
			eUserItemFlag_Heap|eUserItemFlag_GamePreLoad|eUserItemFlag_OverWrite,
			PlayerInfo.nLotteryGeted,false);
	}

	PlayerInfo.lUserAllScore += nScoreGift;
	//char szMsg[100];
	//OnRecord(TEXT("before 4\n"));
	//sprintf( szMsg, "玩家id = %d,分数=%ld，增加分数nScoreGift=%d\n",pIServerUserItem->GetUserID(),PlayerInfo.lUserAllScore,nScoreGift);
	//OnRecord(szMsg);
	if (nCannonStatus & 2)
	{
		SaveUserItem(pIServerUserItem,
			eUserItemLastCannon,
			eUserItemFlag_Heap|eUserItemFlag_GamePreLoad|eUserItemFlag_OverWrite,
			PlayerInfo.cbCannonType,false);
	}

	CMD_S_NewGuidGift NewGuidGift = {0};
	NewGuidGift.bSuccess = bSuccess;
	NewGuidGift.nCannonStatus = nCannonStatus;
	NewGuidGift.nLotteryCount = nLotteryCount;
	NewGuidGift.nScoreGift = nScoreGift;
	NewGuidGift.nCurrentScore = PlayerInfo.lUserAllScore;

	m_pITableFrame->SendTableData(wChairID, SUB_S_NEW_GUID_GIFT, &NewGuidGift, sizeof(NewGuidGift));

	return true;
}

//赢得珍珠
bool CTableFrameSink::OnWinPearl(IServerUserItem * pIServerUserItem, int nPearlType, int nPearlCount)
{
//printf("%s %d\n", __FUNCTION__, __LINE__);
	if (pIServerUserItem == NULL)
	{
		return false;
	}

	WORD wChairID = pIServerUserItem->GetChairID();
	//tagPlayerInfo & PlayerInfo = m_PlayerInfo[wChairID];

	//CMD_S_WinPearl WinPearl1 = {0};
	//WinPearl1.nPearlType = nPearlType;
	//WinPearl1.nPearlCount = nPearlCount;

	LPCTSTR szPearlType[] =
	{
		"初级珍珠", "中级珍珠", "高级珍珠",
	};
	CMD_S_ExchangePearl ExchangePearl = {};
	ExchangePearl.cbResult = 1;
	sprintf(ExchangePearl.szResult, "成功兑换%d颗%s。", nPearlCount, szPearlType[nPearlType]);

	m_pITableFrame->SendTableData(wChairID, SUB_S_EXCHANGE_PEARL, &ExchangePearl, sizeof(ExchangePearl));

	return true;
}

//处理比赛排名
void CTableFrameSink::OnMatchRank(IServerUserItem * pIServerUserItem, SCORE lMatchScore, int nCurrentRank, int nChangeRank)
{
#ifndef KIND_7
//printf("userid is %u matchscore is %lld rank is %d changerank is %d\n", pIServerUserItem->GetUserID(), lMatchScore, nCurrentRank, nChangeRank);
	CMD_S_MatchRank MatchRank = {0};
	MatchRank.lMatchScore = lMatchScore;
	MatchRank.nCurrentRank = nCurrentRank;
	MatchRank.nChangeRank = nChangeRank;
	m_pITableFrame->SendTableData(pIServerUserItem->GetChairID(), SUB_S_MATCH_RANK, &MatchRank, sizeof(MatchRank));
#endif
}

float CTableFrameSink::changeLucky(tagPlayerInfo & PlayerInfo)
{
//printf("%s %d\n", __FUNCTION__, __LINE__);
	PlayerInfo.dwLuckyChangeTime = GetTickCount();
	PlayerInfo.dwLuckyRandTime = (rand() % 11 + 5) * 1000;//随机时间为5秒到15秒
	int nLuckyPercent = rand() % 100;
	float fUserLucky;
	//if (nLuckyPercent >= 96)//%4(96~99)，2.0~2.9
	//{
	//	fUserLucky=(float)random(2.0, 2.9);
	//}
	//else if (nLuckyPercent >= 86)//%10(86~95)，1.5~1.9
	//{
	//	fUserLucky=(float)random(1.5, 1.9);
	//}
	//else if (nLuckyPercent >= 61)//%25(61~84)，1.0~1.4
	//{
	//	fUserLucky=(float)random(1.0, 1.4);
	//}
	//else//%61(0~60)，0.6~0.9
	//{
	//	fUserLucky=(float)random(0.6, 0.9);
	//}
	if (nLuckyPercent >= 97)//%3(97~99)
	{
		fUserLucky=(float)random(3.5, 4.0);
	}
	else if (nLuckyPercent >= 93)//%4(93~96)
	{
		fUserLucky=(float)random(2.6, 3.4);
	}
	else if (nLuckyPercent >= 85)//%10(85~92)
	{
		fUserLucky=(float)random(1.7, 2.5);
	}
	else if (nLuckyPercent >= 25)//%60(25~84)
	{
		fUserLucky=(float)random(0.9, 1.6);
	}
	else//%25(0~24)
	{
		fUserLucky=(float)random(0.6, 0.8);
	}
	PlayerInfo.fUserLucky = fUserLucky;
	return fUserLucky;
}

//获取用户道具
bool CTableFrameSink::OnGetUserItemCount(IServerUserItem * pIServerUserItem, int nType, SCORE lCount)
{
//printf("%s %d\n", __FUNCTION__, __LINE__);
	if (pIServerUserItem == NULL)
	{
		return false;
	}

	WORD wChairID = pIServerUserItem->GetChairID();
	tagPlayerInfo & PlayerInfo = m_PlayerInfo[wChairID];

	switch (nType)
	{
	case eUserItem_MobileMoney:
		{
			PlayerInfo.wMobileMoneyGeted = (WORD)lCount;

			WORD wChairID = pIServerUserItem->GetChairID();
			tagPlayerInfo & PlayerInfo = m_PlayerInfo[wChairID];
			CMD_S_ReturnMobileMoney ReturnMobileMoney = {0};
			ReturnMobileMoney.wMoneyCount = PlayerInfo.wMobileMoneyGeted;
			m_pITableFrame->SendTableData(wChairID, SUB_S_RETURN_MOBILE_MONEY, &ReturnMobileMoney, sizeof(ReturnMobileMoney));

			break;
		}
	}

	return true;
}

void CTableFrameSink::UpdateUserIntegrate(IServerUserItem * pIServerUserItem,int nAddScore)
{
	if (pIServerUserItem == NULL)
	{
		return ;
	}
	m_pITableFrame->UpdateUserIntegrate(pIServerUserItem,nAddScore);
	return;
}

bool CTableFrameSink::IsSignUpMatch(IServerUserItem * pIServerUserItem)
{
	if (pIServerUserItem == NULL)
	{
		return false;
	}
	return m_pITableFrame->IsSignUpMatch(pIServerUserItem);
	
}

void CTableFrameSink::SetMatchStatus(bool bPlaying)
{
	m_bMatchPlaying = bPlaying;
	return;
}

bool CTableFrameSink::OnChangeMatchPlayerScore(IServerUserItem * pIServerUserItem, int wSignUpFee, WORD wChairID, SCORE &llUserScore)
{
	tagPlayerInfo & PlayerInfo=m_PlayerInfo[wChairID];
	LONG lUserScore = PlayerInfo.lUserAllScore + wSignUpFee;
	if (lUserScore < 0)
	{
		return false;
	}

	PlayerInfo.lUserAllScore += wSignUpFee;
	llUserScore = PlayerInfo.lUserAllScore;
	//char szMsg[100];
	//sprintf( szMsg, "玩家id = %d,分数=%ld，增加分数wSignUpFee=%d\n",pIServerUserItem->GetUserID(),PlayerInfo.lUserAllScore,wSignUpFee);
	//OnRecord(TEXT("before 5\n"));
	//OnRecord(szMsg);

	return true;
}

void CTableFrameSink::BoxChange(IServerUserItem * pIServerUserItem, tagPlayerInfo & PlayerInfo, int nFishID)
{
	int nRoomID = 0;
	int nCount1 = 0;//话费、藏宝图、技能的数量
	int nCount2 = 0;//奖券的数量
	int nCount3 = 0;//金币的数量
	if (m_Config.nMinScore == 10)
	{
		nRoomID = 1;
		nCount1 = rand() % 2 + 1;
		nCount2 = (rand() % 5 + 1) * 1;
		nCount3 = (rand() % 5 + 1) * 100;
	}
	else if (m_Config.nMinScore == 100)
	{
		nRoomID = 2;
		nCount1 = rand() % 2 + 3;
		nCount2 = (rand() % 5 + 1) * 10;
		nCount3 = (rand() % 5 + 1) * 1000;
	}
	else if (m_Config.nMinScore == 1000)
	{
		nRoomID = 3;
		nCount1 = rand() % 2 + 5;
		nCount2 = (rand() % 5 + 1) * 100;
		nCount3 = (rand() % 5 + 1) * 10000;
	}
	else if (m_Config.nMinScore == 10000)
	{
		nRoomID = 4;
		nCount1 = rand() % 4 + 7;
		nCount2 = (rand() % 5 + 1) * 1000;
		nCount3 = (rand() % 5 + 1) * 100000;
	}

	int nRand = rand() % 10000 + 1;
	BYTE cbType = 1;
	int nCount = 0;
	if (nRand == 1)//珍珠
	{
		cbType = rand() % 3 + 4;
		nCount = 1;
	}
	else if (nRand > 1 && nRand <= 3)//话费
	{
		cbType = 3;
		nCount = nCount1;
	}
	else if (nRand > 3 && nRand <= 503)//藏宝图
	{
		cbType = 10;
		nCount = nCount1;
	}
	else if (nRand > 503 && nRand <= 1003)//技能
	{
		cbType = rand() % 3 + 7;
		nCount = nCount1;
	}
	else if (nRand > 1003 && nRand <= 1253)//奖券
	{
		cbType = 2;
		nCount = nCount2;
	}
	else//金币
	{
		cbType = 1;
		nCount = nCount3;
	}
	switch (cbType)
	{
	case 1:
		{
			//PlayerInfo.lUserAllScore += nCount;
			PlayerInfo.qBoxPrize.push(nCount);
			char szMsg[100];
			//OnRecord(TEXT("before push\n"));
			//sprintf( szMsg, "Addqueue: pushqueue 玩家id = %d,分数=%ld，增加分数nCount=%d, queue size is %d\n",pIServerUserItem->GetUserID(),PlayerInfo.lUserAllScore,nCount, PlayerInfo.qBoxPrize.size());
			////printf(szMsg);
			//OnRecord(szMsg);
			break;
		}
	case 2:
		{
			PlayerInfo.nLotteryGeted += nCount;
			SaveUserItem(pIServerUserItem,
				eUserItem_LotteryInfo,
				eUserItemFlag_Heap|eUserItemFlag_GamePreLoad|eUserItemFlag_OverWrite,
				PlayerInfo.nLotteryGeted,false);
			break;
		}
	case 3:
		{
			PlayerInfo.wMobileMoneyGeted += nCount;
			m_pITableFrame->SaveUserItemCount(pIServerUserItem, eUserItem_MobileMoney, (SCORE)PlayerInfo.wMobileMoneyGeted);
			break;
		}
	case 4:
		{
			m_pITableFrame->ModifyBackpack(pIServerUserItem, 0, nCount);
			break;
		}
	case 5:
		{
			m_pITableFrame->ModifyBackpack(pIServerUserItem, 1, nCount);
			break;
		}
	case 6:
		{
			m_pITableFrame->ModifyBackpack(pIServerUserItem, 2, nCount);
			break;
		}
	case 7:
		{
			m_pITableFrame->ModifyBackpack(pIServerUserItem, 5, nCount);
			break;
		}
	case 8:
		{
			m_pITableFrame->ModifyBackpack(pIServerUserItem, 6, nCount);
			break;
		}
	case 9:
		{
			m_pITableFrame->ModifyBackpack(pIServerUserItem, 7, nCount);
			break;
		}
	case 10:
		{
			m_pITableFrame->ModifyBackpack(pIServerUserItem, 8, nCount);
			break;
		}
	}
	CMD_S_BoxPrize BoxPrize = {};
	BoxPrize.cbType = cbType;
	BoxPrize.nCount = nCount;
	BoxPrize.nFishID = nFishID;
	m_pITableFrame->SendTableData(pIServerUserItem->GetChairID(), SUB_S_BOX_PRIZE, &BoxPrize, sizeof(BoxPrize));
}

void CTableFrameSink::GenManEatingFish(WORD wChairID, WORD wIndex, int nSpriteID)
{
	tagFishPack * pFishPack = fillFishPack(nSpriteID - 1000, 0, 0);
	m_ManEatingFish[wChairID].nFishID[wIndex] = pFishPack->nFishID;
	//m_ManEatingFish[wChairID].nEatScore[wIndex] = 0;
	m_ManEatingFish[wChairID].nSpriteID[wIndex] = nSpriteID;
}

void CTableFrameSink::SendManEatingFish()
{
	IServerUserItem * pIServerUserItem = m_pITableFrame->GetTableUserItem(0);
	if (pIServerUserItem != NULL)
	{
		if ((m_ManEatingFish[0].nFishID[0] == 0 && m_ManEatingFish[0].nFishID[1] == 0 && m_ManEatingFish[0].nFishID[2] == 0)
			&& (!m_ManEatingFish[0].bIsDead[0] && !m_ManEatingFish[0].bIsDead[1] && !m_ManEatingFish[0].bIsDead[2]))
		{
			GenManEatingFish(0, 0, 1027);
			GenManEatingFish(0, 1, 1027);
			GenManEatingFish(0, 2, 1027);
		}

		CMD_S_ManEatingFish ManEatingFish = {};
		ManEatingFish.wChairID = 0;
		for (int i = 0; i < 3; i++)
		{
			ManEatingFish.nFishID[i] = m_ManEatingFish[0].nFishID[i];
			ManEatingFish.nSpriteID[i] = m_ManEatingFish[0].nSpriteID[i];
			ManEatingFish.nMultiple[i] = 10;
		}
		m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_MAN_EATING_FISH, &ManEatingFish, sizeof(ManEatingFish));
	}
	pIServerUserItem = m_pITableFrame->GetTableUserItem(1);
	if (pIServerUserItem != NULL)
	{
		if ((m_ManEatingFish[1].nFishID[0] == 0 && m_ManEatingFish[1].nFishID[1] == 0 && m_ManEatingFish[1].nFishID[2] == 0)
			&& (!m_ManEatingFish[1].bIsDead[0] && !m_ManEatingFish[1].bIsDead[1] && !m_ManEatingFish[1].bIsDead[2]))
		{
			GenManEatingFish(1, 0, 1027);
			GenManEatingFish(1, 1, 1027);
			GenManEatingFish(1, 2, 1027);
		}

		CMD_S_ManEatingFish ManEatingFish = {};
		ManEatingFish.wChairID = 1;
		for (int i = 0; i < 3; i++)
		{
			ManEatingFish.nFishID[i] = m_ManEatingFish[1].nFishID[i];
			ManEatingFish.nSpriteID[i] = m_ManEatingFish[1].nSpriteID[i];
			ManEatingFish.nMultiple[i] = 10;
		}
		m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_MAN_EATING_FISH, &ManEatingFish, sizeof(ManEatingFish));
	}
}

void CTableFrameSink::OnRecord(char* strMsg)
{
	SYSTEMTIME sys; 
	GetLocalTime(&sys);
	char szTime[20];
	sprintf( szTime, "::%d:%d:%d",sys.wHour,sys.wMinute,sys.wSecond);
	lstrcat(strMsg,szTime);
	FILE *FSPOINTER;
	FSPOINTER = fopen("FishLog.TXT", "a+");
	fprintf(FSPOINTER,strMsg);
	fclose(FSPOINTER);
}

void tagPlayerInfo::initCurve()
{
	float fUL;//0波峰、1波谷
	if (fYields > (fUpperLimit + fLowerLimit) / 2)
	{
		fUL = 1.0f;
printf("波谷\n");
	}
	else
	{
		fUL = 0.0f;
printf("波峰\n");
	}
	for (int i = 0; i < POINTS; i++)
	{
		fCurve[i] = sin(PIVALUE * (i * (1.0f / POINTS) + fUL)) * ((fUpperLimit - fLowerLimit) / 2) + fUpperLimit - (fUpperLimit - fLowerLimit) / 2;
	}
}

void tagPlayerInfo::fire(int nFireScore)
{
	lConsume += nFireScore;
	fYields = (float)((double)lGains / lConsume);

	nFireCount++;
	if (nFireCount % 20 == 0)
	{
		nPointCount++;
	}
printf("Gains = %lld Consume = %lld Yields = %f Crr = %f\n", lGains, lConsume, fYields, fCurve[nPointCount]);
}

int tagPlayerInfo::calcFishPercent(int nFishPercent, int nFishMultiple)
{
	if (nPointCount >= POINTS || nPointCount == -1)
	{
		initCurve();
		nPointCount = 0;
	}
	float fWeightCoefficient = fCurve[nPointCount] / (nFishPercent * nFishMultiple / 10000.0f) * 1.5f;
	return (int)(nFishPercent * fWeightCoefficient);
}

void tagPlayerInfo::hit(int nHitScore, int nSpriteID)
{
	lGains += nHitScore;
	fYields = (float)((double)lGains / lConsume);
printf("Gains = %lld Consume = %lld Yields = %f Crr = %f nSpriteID = %d\n", lGains, lConsume, fYields, fCurve[nPointCount], nSpriteID);
}
