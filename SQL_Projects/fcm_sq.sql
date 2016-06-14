-- test last day
DECLARE @LastLogonDate DATETIME
select @LastLogonDate = dateadd(d, -1, GETDATE())
update AccountsInfo set PlayingTime=130, TimeMark_ForGame=1, LastLogonDate=@LastLogonDate  where NickName='livetest2'

select TimeMark_ForGame, IsOnline, PlayingTime,LastLogonDate, AntiAddictMark 
from AccountsInfo where NickName in ('livetest', 'livetest2', 'livetest3', 'livetest4')  -- 15091
update AccountsInfo set PlayingTime=130, TimeMark_ForGame=1, LastLogonDate=GETDATE() where NickName='livetest'
update AccountsInfo set PlayingTime=130, TimeMark_ForGame=1 where NickName='livetest2'
update AccountsInfo set PlayingTime=130, TimeMark_ForGame=1 where NickName='livetest3'
update AccountsInfo set PlayingTime=130, TimeMark_ForGame=1 where NickName='livetest4'

	SELECT AccountsInfo.UserID, AccountsInfo.TimeMark_ForGame FROM QPAccountsDBLINK.QPAccountsDB.dbo.AccountsInfo WITH(updlock, HOLDLOCK) 
	WHERE AntiAddictMark=1 AND IsOnline=1 AND ((((PlayingTime + DATEDIFF(MINUTE, LastLogonDate, GETDATE())) >= 60 AND (PlayingTime + DATEDIFF(MINUTE, LastLogonDate, GETDATE()) < 2 * 60 )) AND TimeMark_ForGame=0)
	OR
	(((PlayingTime + DATEDIFF(MINUTE, LastLogonDate, GETDATE())) >= 2 * 60 AND ((PlayingTime + DATEDIFF(MINUTE, LastLogonDate, GETDATE()) < 3 * 60 ))) AND TimeMark_ForGame=1)
	OR
	((PlayingTime + DATEDIFF(MINUTE, LastLogonDate, GETDATE()) >= 3 * 60)) AND TimeMark_ForGame=2)
	
	
Declare	@strErrorDescribe NVARCHAR(127)		-- 输出信息
exec GSP_GP_RegisterAccounts 'livetest11', 
'livetest11', '','8DAE5EFA6A9F2B4393BD367212CE5764', '8DAE5EFA6A9F2B4393BD367212CE5764', 0, 1, 
'', '', '', '127.0.0.1', 'E088BE49EE1D85235F3B82000EB0776C', 1, 1, @strErrorDescribe output

delete from AccountsInfo where NickName='livetest10'


Use QPTreasureDB
select * from dbo.AndroidManager order by UserID
delete AndroidManager where UserID > 37
--youkuss03
use QPAccountsDB
go

UPDATE accountsinfo SET LastLogoutDateForAntiAddiction = DATEADD(Day,-1,GETDATE())
select Nickname, TimeMark_ForGame, IsOnline, PlayingTime,LastLogonDate, AntiAddictMark, LastLogoutDateForAntiAddiction from AccountsInfo 
where  Nickname like 'livetest%' and AntiAddictMark = 1 and IsOnline = 1 


-- point
update AccountsInfo set IsOnline=0 where IsOnline is Null 

-- new reg user
Use qpaccountsdb
GO
Declare @NickName NVARCHAR(31) ; set @NickName = 'livetest40' 
select Nickname, TimeMark_ForGame, IsOnline, PlayingTime,LastLogonDate, AntiAddictMark, LastLogoutDateForAntiAddiction 
from AccountsInfo where NickName = @NickName
update AccountsInfo set PlayingTime=59, TimeMark_ForGame=0 where NickName=@NickName
print '1'
update AccountsInfo set PlayingTime=130, TimeMark_ForGame=1 where NickName=@NickName
print '2'
update AccountsInfo set PlayingTime=190, TimeMark_ForGame=2  where NickName=@NickName
print '3'


-----------check the miss point
-- get the info
Use qpaccountsdb
GO
Declare @NickName NVARCHAR(31) ; set @NickName = 'youkuss01' 
select Nickname, TimeMark_ForGame, IsOnline, PlayingTime,LastLogonDate, AntiAddictMark, LastLogoutDateForAntiAddiction 
from AccountsInfo where NickName = @NickName
update AccountsInfo set PlayingTime=65, TimeMark_ForGame=0, LastLogonDate=GETDATE()  where NickName=@NickName
update AccountsInfo set PlayingTime=119, TimeMark_ForGame=1, LastLogonDate=GETDATE() where NickName=@NickName
update AccountsInfo set PlayingTime=179, TimeMark_ForGame=2, LastLogonDate=GETDATE() where NickName=@NickName
update AccountsInfo set PlayingTime=190, TimeMark_ForGame=2, LastLogonDate=GETDATE() where NickName=@NickName

--- check the multi use test
Use qpaccountsdb
GO
Declare @NickName NVARCHAR(31) ; set @NickName = 'youkuss02' --'leleTestA001' 
update AccountsInfo set PlayingTime=59, TimeMark_ForGame=0, LastLogonDate=GETDATE()  
where NickName=@NickName or NickName='youkuss03'
update AccountsInfo set PlayingTime=65, TimeMark_ForGame=0, LastLogonDate=GETDATE()  
where NickName=@NickName or NickName='youkuss03'
update AccountsInfo set PlayingTime=130, TimeMark_ForGame=1, LastLogonDate=GETDATE()  
where NickName=@NickName or NickName='youkuss03'
update AccountsInfo set PlayingTime=190, TimeMark_ForGame=2, LastLogonDate=GETDATE()
where NickName=@NickName or NickName='youkuss03'


-- 模拟
Use QPAccountsDB
GO
Declare @NickName NVARCHAR(31) ; set @NickName = 'leleTestA001' 
update AccountsInfo set PlayingTime=65, TimeMark_ForGame=0, LastLogonDate=GETDATE(), IsOnline = 1  where NickName=@NickName
	exec GSP_GP_USER_PLAYINGTIME
declare @i int
select @i=1
while(@i<=50)
begin
	waitfor DELAY '00:00:02';
	exec GSP_GP_USER_PLAYINGTIME
end 

-----------modify the lastlogout time
-- get the info
Use qpaccountsdb
GO
Declare @NickName NVARCHAR(31) ; set @NickName = 'youkuss02' 
-- Break the limit
update AccountsInfo set LastLogoutDateForAntiAddiction = DATEADD(hour, -5,GETDATE()) where NickName = @NickName

-- add the limit
update AccountsInfo set LastLogoutDateForAntiAddiction = DATEADD(hour, -3,GETDATE()), TimeMark_ForGame=3 where NickName = @NickName

select Nickname, TimeMark_ForGame, IsOnline, PlayingTime,LastLogonDate, AntiAddictMark, LastLogoutDateForAntiAddiction 
from AccountsInfo where NickName = @NickName