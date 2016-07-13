DECLARE @GameID INT 
SET @GameID = 2392764

SELECT UserID, accounts, nickname,  TimeMark_ForGame, AntiAddictMark, IsOnline, PlayingTime, LastLogoutDateForAntiAddiction  
FROM [qpaccountsdb].dbo.accountsinfo WHERE gameid = @GameID

--- SET the time mark
UPDATE QPAccountsDB.dbo.accountsinfo SET AntiAddictMark = 1,PlayingTime = 161, lastlogondate = getdate(), TimeMark_ForGame = 1 WHERE gameid = @GameID
--UPDATE QPAccountsDB.dbo.accountsinfo SET LastLogoutDateForAntiAddiction = DATEADD(Hour,-6,GETDATE()) WHERE gameid = @GameID
--UPDATE accountsinfo SET PlayingTime = 119, lastlogondate = getdate(), TimeMark_ForGame = 1 WHERE gameid = @GameID
--UPDATE accountsinfo SET PlayingTime = 190, lastlogondate = getdate(), TimeMark_ForGame = 2 WHERE gameid = @GameID

--- check fcm time
	SELECT AccountsInfo.UserID, AccountsInfo.TimeMark_ForGame FROM QPAccountsDB.dbo.AccountsInfo --WITH(updlock, HOLDLOCK) 
	WHERE AntiAddictMark=1 AND IsOnline=1 AND ((((PlayingTime + DATEDIFF(MINUTE, LastLogonDate, GETDATE())) >= 60 AND (PlayingTime + DATEDIFF(MINUTE, LastLogonDate, GETDATE()) < 2 * 60 )) AND TimeMark_ForGame=0)
	OR
	(((PlayingTime + DATEDIFF(MINUTE, LastLogonDate, GETDATE())) >= 2 * 60 AND ((PlayingTime + DATEDIFF(MINUTE, LastLogonDate, GETDATE()) < 3 * 60 ))) AND TimeMark_ForGame=1)
	OR
	((PlayingTime + DATEDIFF(MINUTE, LastLogonDate, GETDATE()) >= 3 * 60)) AND TimeMark_ForGame=2)

---SELECT 	DATEDIFF(Hour, '2016-07-12 05:04:09.913', GETDATE()) 
-- environment set
--UPDATE accountsinfo SET AntiAddictMark = 1 WHERE gameid = 2392695 
--UPDATE accountsinfo SET TimeMark_ForGame = 0 

--UPDATE accountsinfo SET PlayingTime = 0 --DATEADD(day, -1, getdate())
--SELECT DATEADD(day, -1, getdate());
--exec GSP_GP_UserLogout 1425030


-- ÕÊºÅµÇÂ¼

--DECLARE @string nvarchar(100)
--exec QPAccountsDB.dbo.[GSP_GP_EfficacyAccounts_III] 'Jhsjxjjp', '83878c91171338902e0fe0fb97a8c47a', '', '192.168.0.1', 'xxxxxxxx', 1, '11', @string output
--print @string

--SELECT * FROM QPAccountsDB.dbo.accountsinfo where Accounts = 'Jhsjxjjp'


