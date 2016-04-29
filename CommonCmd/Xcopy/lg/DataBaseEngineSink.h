#ifndef DATABASE_ENGINE_SINK_HEAD_FILE
#define DATABASE_ENGINE_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "InitParameter.h"
#include "DataBasePacket.h"

//////////////////////////////////////////////////////////////////////////////////

#define DB_NO_AVAILABLE_ACCOUNT	1									//没有可用账户
#define DB_ACCINFO_ALREADYEXIST		7									//账户已经存在
#define DB_GUEST_REG_SUCCESS		21									//游客注册成功

#define DB_QUERY_SIGNIN_SUCCESS     22									//签到查询成功
#define DB_QUERY_SIGNIN_FAILURE		23									//签到查询失败
#define DB_USER_SIGNIN_SUCCESS		24									//点击签到成功
#define DB_USER_SIGNIN_FAILURE		25									//点击签到失败
#define DB_USER_SIGNIN_ALREADY		26									//用户已签到
#define DB_USER_BQ_SUCCESS			27									//补签成功

#define DB_QUERY_LOTTERY_SUCCESS    30									//抽奖查询成功
#define DB_QUERY_LOTTERY_FAILURE    31									//抽奖查询失败
#define DB_USER_LOTTERY_SUCCESS     32									//抽奖操作成功
#define DB_USER_LOTTERY_FAILURE     33									//抽奖操作失败
#define DB_GET_PRIZEITEM_SUCCESS    34									//中奖等级获取成功
#define DB_GET_PRIZEITEM_FAILURE    35									//中奖等级获取失败
#define DB_GET_RECHARGE_SUCCESS     36									//充值信息获取成功
#define DB_GET_RECHARGE_FAILURE     37									//充值信息获取失败

//数据库类
class CDataBaseEngineSink : public IDataBaseEngineSink
{
	//友元定义
	friend class CServiceUnits;

	//成员变量
private:
	UINT m_uiRandCodeCnt;												//密码计数

	//用户数据库
protected:
	CDataBaseAide					m_AccountsDBAide;					//用户数据库
	CDataBaseHelper					m_AccountsDBModule;					//用户数据库

	//游戏币数据库
protected:
	CDataBaseAide					m_TreasureDBAide;					//游戏币数据库
	CDataBaseHelper					m_TreasureDBModule;					//游戏币数据库

	//平台数据库
protected:
	CDataBaseAide					m_PlatformDBAide;					//平台数据库
	CDataBaseHelper					m_PlatformDBModule;					//平台数据库

	//组件变量
protected:
	CInitParameter *				m_pInitParameter;					//配置参数
	IDataBaseEngineEvent *			m_pIDataBaseEngineEvent;			//数据事件

	//函数定义
public:
	//构造函数
	CDataBaseEngineSink();
	//析构函数
	virtual ~CDataBaseEngineSink();

	//基础接口
public:
	//释放对象
	virtual VOID Release() { return; }
	//接口查询
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//系统事件
public:
	//启动事件
	virtual bool OnDataBaseEngineStart(IUnknownEx * pIUnknownEx);
	//停止事件
	virtual bool OnDataBaseEngineConclude(IUnknownEx * pIUnknownEx);

	//内核事件
public:
	//时间事件
	virtual bool OnDataBaseEngineTimer(DWORD dwTimerID, WPARAM dwBindParameter);
	//控制事件
	virtual bool OnDataBaseEngineControl(WORD wControlID, VOID * pData, WORD wDataSize);
	//请求事件
	virtual bool OnDataBaseEngineRequest(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);

	//登录服务
protected:
	//I D 登录
	bool OnRequestLogonGameID(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//帐号登录
	bool OnRequestLogonAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//帐号注册
	bool OnRequestRegisterAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//获取游客信息
	LONG InsGuestRegInfo(DBR_GP_RegisterAccounts* pGuestRegInfo, CDBVarValue * pDBVarValue);
	//插入游客信息
	LONG GetGuestRegInfo(DBR_GP_RegisterAccounts* pGuestRegInfo, CDBVarValue * pDBVarValue);
	//游客登录
	bool OnRequestGuestRegister(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//生成随机账户
	VOID GenRandAccount(TCHAR szRandAccount[]);
	//生成随机字符串
	VOID GenRandString(TCHAR szRandStr[], UINT uLength);

	//手机登录
protected:
	//I D 登录
	bool OnMobileLogonGameID(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//帐号登录
	bool OnMobileLogonAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//帐号注册
	bool OnMobileRegisterAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//账号服务
protected:
	//修改机器
	bool OnRequestModifyMachine(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//修改密码
	bool OnRequestModifyLogonPass(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//修改密码
	bool OnRequestModifyInsurePass(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//修改签名
	bool OnRequestModifyUnderWrite(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//修改头像
	bool OnRequestModifySystemFace(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//修改头像
	bool OnRequestModifyCustomFace(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//修改资料
	bool OnRequestModifyIndividual(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//签到服务
protected:
	bool OnRequestUserSignIn(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//银行服务
protected:
	//存入游戏币
	bool OnRequestUserSaveScore(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//提取游戏币
	bool OnRequestUserTakeScore(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//转账游戏币
	bool OnRequestUserTransferScore(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//信息查询
protected:
	//查询资料
	bool OnRequestQueryIndividual(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//查询银行
	bool OnRequestQueryInsureInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//密码验证
	bool OnRequestUserCheckPassword(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//查询用户
	bool OnRequestQueryTransferUserInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//查询签到
	bool OnRequestQuerySignInInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//抽奖查询
	bool OnRequestQueryLotteryInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//抽奖操作
	bool OnRequestUserLtry(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//充值查询
	bool OnRequestQryRechargeInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//系统功能
protected:
	//加载列表
	bool OnRequestLoadGameList(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//在线信息
	bool OnRequestOnLineCountInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//结果处理
protected:
	//登录结果
	VOID OnLogonDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient);
	//银行结果
	VOID OnInsureDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient);
	//操作结果
	VOID OnOperateDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient);
	//赠送信息
	VOID OnInsureTransferInfo(DWORD dwContextID, DWORD dwErrorCode);
	//签到结果
	VOID OnSignInDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient);
	//抽奖结果
	VOID OnLotteryDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient);
	//充值结果
	VOID OnRechargeDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient);
};

//////////////////////////////////////////////////////////////////////////////////

#endif