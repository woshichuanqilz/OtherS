

UPDATE QPTreasureDB.dbo.gamescoreinfo SET score = 1000000 WHERE userid in 
(SELECT UserID FROM QPAccountsDB.dbo.accountsinfo where gameid = 2392744)


SELECT * FROM QPPlatformDB.dbo.GameRoomInfo
SELECT * FROM qpplatformdb.dbo.fishstock WHERE roomid = 315

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
update QPAccountsDB.dbo.accountsinfo SET PassPortID = 12345
WHERE userid in 
(
SELECT UserID
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