
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
 FROM QPAccountsDB.dbo.accountsinfo where gameid in (2387014, 2392994, 2393012, 2393011, 2393062))

 delete QPTreasureDB.dbo.GameScoreLocker -- where UserID in 
 (SELECT
 --*,  
UserID
 FROM QPAccountsDB.dbo.accountsinfo where gameid in (2387014, 2392994))

 -- 5 accelaroter 6 san she 

 UPDATE QPAccountsDB.dbo.backpack SET item5 = 1000, item6 =1000 
 (SELECT
 --*,  
UserID
 FROM QPAccountsDB.dbo.accountsinfo where gameid = 2387014)

----------------------------------------

 UPDATE QPTreasureDB.[dbo].[UserItem] SET count = 0 WHERE type in (122,123,124,125) AND userid = 2387014
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