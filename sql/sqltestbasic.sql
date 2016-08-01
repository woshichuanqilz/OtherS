-- get the current ip address 
SELECT  
   CONNECTIONPROPERTY('net_transport') AS net_transport,
   CONNECTIONPROPERTY('protocol_type') AS protocol_type,
   CONNECTIONPROPERTY('auth_scheme') AS auth_scheme,
   CONNECTIONPROPERTY('local_net_address') AS local_net_address,
   CONNECTIONPROPERTY('local_tcp_port') AS local_tcp_port,
   CONNECTIONPROPERTY('client_net_address') AS client_net_address 
----------------------------------------
	SELECT *
	from QPTreasureDB.dbo.NumConfig(nolock) as uk WHERE numtype = 6


select UserCount from qpaccountsdb.dbo.AccountsInfo(nolock) where UserID = 1425459
UPDATE qpaccountsdb.dbo.AccountsInfo SET UserCount = 1 where UserID = 1425459
exec QPTreasureDB.dbo.GSP_GR_GetDftAlgoData 1425459
----
-- restrict duplicated accounts
SELECT * FROM QPAccountsDB.dbo.accountsinfo where gameid = 2393152
SELECT * FROM  QPTreasureDB.dbo.UserItem 
	WHERE UserID = 1457098 AND [type] = 126

SELECT * FROM QPAccountsDB.dbo.localtestmachineinfo
--121.24.27.50
-----------------------algorithm--------
UPDATE QPTreasureDB.dbo.gamescoreinfo SET score = 100000 WHERE userid in 
(SELECT
 --*,  
UserID
 FROM QPAccountsDB.dbo.accountsinfo where gameid in (2387014, 2392994, 2393012, 2393011, 2393062, 2393152))

 UPDATE QPTreasureDB.[dbo].[UserItem] SET count = 0 WHERE type in (122,123,124,125) AND userid in 
 (
SELECT Userid FROM QPAccountsDB.dbo.accountsinfo where gameid in (2387014, 2392994, 2393012, 2393011, 2393062, 2393152)
 )



UPDATE QPAccountsDB.dbo.Backpack SET item6 = 1000 WHERE UserID in
(SELECT
 --*,  
UserID
 FROM QPAccountsDB.dbo.accountsinfo where gameid in (2387014, 2392994, 2393012, 2393011, 2393062, 2393152))


 
-- unlock duplicated accounts
--SELECT * FROM QPAccountsDB.dbo.accountsinfo where gameid = 2447207
--SELECT count(*) FROM qpaccountsdb.dbo.accountsinfo WHERE registerip = '121.24.31.3'
--SELECT count(*) FROM qpaccountsdb.dbo.accountsinfo WHERE RegisterMachine = 'B85AA4BD9CFE49FE8C44253573994066'

-- 散射
UPDATE QPAccountsDB.dbo.Backpack SET item6 = 1000 WHERE UserID = @dwUserID 

---------------
if exists (SELECT * FROM QPAccountsDB.dbo.localtestmachineinfo WHERE LocalTestMachineIP = 'B85AA4BD9CFE49FE8C44253573994066')
BEGIN
	print 'xixi'
END 

else
BEGIN
	print 'hehe'
END 
--------------------------------------------- unlock duplicated accounts
SELECT * FROM QPAccountsDB.dbo.accountsinfo where gameid = 2393162
SELECT count(*) FROM qpaccountsdb.dbo.accountsinfo WHERE registerip = '121.24.31.3'
SELECT count(*) FROM qpaccountsdb.dbo.accountsinfo WHERE RegisterMachine = 'B85AA4BD9CFE49FE8C44253573994066'


UPDATE QPTreasureDB.dbo.UserItem 
	SET [guidtag1] = 1, [guidtag2] = 1, [guidtag3] = 10, isgamepreload = 1 
	WHERE UserID = 1478222 AND [type] = 126



--SELECT nickname,IsAndroid FROM QPAccountsDB.dbo.accountsinfo where RegisterMachine = '' -- like '%---%'
SELECT  NickName, RegisterMachine, RegisterIP  FROM QPAccountsDB.dbo.accountsinfo where nickname like '%livetest%'
SELECT * FROM QPAccountsDB.dbo.accountsinfo where nickname = 'livetest4'
update QPAccountsDB.dbo.accountsinfo SET RegisterIP = '192.168.1.4' where userid in 
(SELECT top 8 UserID  FROM QPAccountsDB.dbo.accountsinfo where nickname like '%livetest%')

-----
SELECT count(*)  FROM QPAccountsDB.dbo.accountsinfo where RegisterIP = '192.168.1.4' 
begin tran mytran

-- remain 8 row info register ip
DELETE QPAccountsDB.dbo.accountsinfo where userid in 
(SELECT UserID FROM QPAccountsDB.dbo.accountsinfo
WHERE UserID not in  
(SELECT top 8 UserID FROM QPAccountsDB.dbo.accountsinfo where RegisterIP = '192.168.1.4' order by UseriD)
AND RegisterIP = '192.168.1.4')

-- remain 8 row info machine id 
DELETE QPAccountsDB.dbo.accountsinfo WHERE UserID in
(SELECT UserID FROM QPAccountsDB.dbo.accountsinfo
WHERE UserID not in  
(SELECT top 8 UserID FROM QPAccountsDB.dbo.accountsinfo where RegisterMachine = '6C9B51B76E854B8F915B77D7816A8BD9' order by UseriD)
AND RegisterMachine = '6C9B51B76E854B8F915B77D7816A8BD9')

WAITFOR DELAY '00:02';
SELECT count(*)  FROM QPAccountsDB.dbo.accountsinfo where RegisterIP = '192.168.1.4' 
rollback tran mytran

SELECT count(*)  FROM QPAccountsDB.dbo.accountsinfo where RegisterIP = '192.168.1.4' 

--SELECT * FROM QPAccountsDB.dbo.accountsinfo where nickname like '%livetest6%'

	DECLARE @SameIPCnt INT
	DECLARE @SameMachineIDCnt INT
	SELECT @SameIPCnt = count(*) FROM QPAccountsDB.dbo.accountsinfo where  RegisterIP = @strClientIP AND IsAndroid = 0
	SELECT @SameMachineIDCnt = count(*) FROM QPAccountsDB.dbo.accountsinfo where  RegisterIP = @strClientIP AND IsAndroid = 0

	-- guid1 = bIsDuplicatedReg , 
	-- guid2 = bIsTestAccount , 
	-- guid3 = Percent Divisor, 
	-- final hit fish probility will be Normal Percent divied / guid3;
	if (@SameIPCnt >= 10) or (@SameMachineIDCnt >= 10)
	BEGIN
		IF NOT EXISTS (SELECT * FROM QPTreasureDB.dbo.UserItem(NOLOCK) WHERE UserID = @UserID AND [type] = 126)
		BEGIN
			INSERT QPTreasureDB.dbo.UserItem ([type], [count], [maxcount], [userid], [guidtag1], [guidtag2], [guidtag3], [guidtag4], source, subsource, isgamepreload, flag)
			VALUES (126, 0, 0, @UserID, 1, 0, 10, 0, 0, 0, 1, 0)
		END
		else
		BEGIN
			UPDATE QPTreasureDB.dbo.UserItem 
				SET [guidtag1] = 1, [guidtag2] = 0, [guidtag3] = 10, isgamepreload = 1 
				WHERE UserID = @UserID AND [type] = 126
		END
	END

----------------
SELECT RegisterMachine FROM QPAccountsDB.dbo.accountsinfo 
SELECT top 1 * FROM QPAccountsDB.dbo.accountsinfo 
-- duplicated line test
WITH cte AS (
  SELECT name , age, gender,  
     row_number() OVER(PARTITION BY name, age ORDER BY name) AS [rn]
  FROM name
)

DELETE cte WHERE [rn] > 1


-- duplicate register
DECLARE @countx INT
SELECT @countx=count(*) FROM QPAccountsDB.dbo.accountsinfo where RegisterIP = '60.5.62.2'
print 'hello world here'
print convert(varchar(20), @countx)

SELECT top 1 * FROM QPAccountsDB.dbo.accountsinfo 
--------------------
SELECT * FROM QPTreasureDB.dbo.NumConfig
SELECT * FROM QPAccountsDB.dbo.accountsinfo where nickname like '%livetest29%'


-----------------------algorithm--------
UPDATE QPTreasureDB.dbo.gamescoreinfo SET score = 100000 WHERE userid in 
(SELECT
 --*,  
UserID
 FROM QPAccountsDB.dbo.accountsinfo where gameid in (2387014, 2392994, 2393012, 2393011, 2393062, 2393152))

 delete QPTreasureDB.dbo.GameScoreLocker  where UserID in 
 (SELECT
 --*,  
UserID
 FROM QPAccountsDB.dbo.accountsinfo where gameid in (2387014, 2392994,2393152))

 -- 5 accelaroter 6 san she 

 UPDATE QPAccountsDB.dbo.backpack SET item5 = 1000, item6 =1000 
 (SELECT
 --*,  
UserID
 FROM QPAccountsDB.dbo.accountsinfo where gameid = 2387014)

----------------------------------------

SELECT * FROM QPTreasureDB.[dbo].[UserItem]  WHERE type in (122,123,124,125) AND userid in 
 (
SELECT Userid FROM QPAccountsDB.dbo.accountsinfo where gameid = 2387014 
 )

 SELECT userid , count  FROM QPTreasureDB.[dbo].[UserItem]  WHERE type in (122) 


 UPDATE QPTreasureDB.[dbo].[UserItem] SET count = 0 WHERE type in (122,123,124,125) AND userid in 
 (
SELECT Userid FROM QPAccountsDB.dbo.accountsinfo where gameid = 2393152 
 )

--SELECT * FROM QPPlatformDB.dbo.GameRoomInfo
--SELECT * FROM qpplatformdb.dbo.fishstock WHERE roomid = 315

--DELETE qpplatformdb.dbo.GameRoomInfo WHERE serverid = 359
-- phone fee
--SELECT * FROM QPTreasureDB.dbo.UserItem WHERE userid = 1425075 AND [type] = 115
--UPDATE QPTreasureDB.dbo.UserItem SET count = 26 WHERE userid = 1425075 AND [type] = 115

-- database info 

--DataBaseInfo 
--UPDATE [QPTreasureDB].[dbo].[GameScoreInfo] SET [Bankruptcy] = 0 WHERE userid = 1425083

--SELECT [Bankruptcy] FROM [QPTreasureDB].[dbo].[GameScoreInfo] WHERE userid = 1425083


---
---string length
--declare @str char(20)
--set @str = '我是中国人'
--select datalength(@str) as datalength长度, len( @str ) as len长度

--SELECT UserID, PassPortID, len(RTRIM(PassPortID)) AS PassportIDLen FROM QPAccountsDB.dbo.accountsinfo WHERE len(RTRIM(PassPortID)) < 18 or PassPortID = null
update QPAccountsDB.dbo.accountsinfo SET PassPortID = '12345'
WHERE userid in 
(
SELECT UserID --, PassPortID
FROM QPAccountsDB.dbo.accountsinfo WHERE  PassPortID is null or PassPortID = ''
)


--- register
Declare	@strErrorDescribe NVARCHAR(127)		-- 输出信息
exec QPAccountsDB.dbo.GSP_GP_RegisterAccounts_III 'Qqqqqqq123345', 'Qqqqqqq123345', '','8DAE5EFA6A9F2B4393BD367212CE5764', '8DAE5EFA6A9F2B4393BD367212CE5764', 0, 1, '', '', '', '127.0.0.1', 'E088BE49EE1D85235F3B82000EB0776C', 1, 0,'','xx',1,1, @strErrorDescribe output
print @strErrorDescribe


-- null
DECLARE @strPassPortID varchar(20)
SET @strPassPortID = null
	if @strPassPortID <> '' or @strPassPortID is null
	BEGIN
		print @strPassPortID + 'xixi'
	END 