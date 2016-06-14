-- WuSong accounts
select * from SignRewardConfig
select UserID from qpaccountsdb.dbo.accountsinfo where gameid=2391593

select * from qptreasuredb.dbo.SignLog 
where UserID in (select UserID from qpaccountsdb.dbo.AccountsInfo where GameID=2391593)

delete from qptreasuredb.dbo.SignLog where UserID=1423990

-- Lizhe Accounts
declare @Userid int
set @Userid = 1420818

select * from qptreasuredb.dbo.SignRewardConfig
select * from qptreasuredb.dbo.UserItem where userid = @Userid
select UserID from qpaccountsdb.dbo.accountsinfo where gameid=2388317



--Modify the last signday
	-- Normal Sign
declare @lastsigndaynum int
declare @lastsignday datetime
declare @nextsignday int
declare @IsVIP int

set @lastsignday = DATEADD(Day,-1,GETDATE())
set @lastsigndaynum = 3
set @IsVIP = 1

update qptreasuredb.dbo.SignLog Set LastSignDay=@lastsignday, LastSignDayNum=@lastsigndaynum 
 where UserID = 1420818 AND IsVIP = @IsVIP
 
	-- Back to day 1
declare @lastsigndaynum int
declare @lastsignday datetime
declare @nextsignday int
declare @Userid int

set @lastsignday = DATEADD(Day,-2,GETDATE())
set @lastsigndaynum = 3
set @Userid = 1420818

update qptreasuredb.dbo.SignLog Set LastSignDay=@lastsignday, LastSignDayNum=@lastsigndaynum  
where UserID = @Userid


-------------------------------------- modify the signrewardconfig 
declare @rewardtype int
declare @IsVIP int
declare @daynum int
set @rewardtype = 3
set @daynum = 1

--init the ENV delete the log in the signLog
delete from qptreasuredb.dbo.SignLog where UserID=1420818

update qptreasuredb.dbo.SignRewardConfig Set RewardType = @rewardtype where DayNum = 1 And IsVIP = 1
update qptreasuredb.dbo.SignRewardConfig Set RewardType = @rewardtype where DayNum = 1 And IsVIP = 0

select * from qptreasuredb.dbo.UserItem where userid = 1420818 AND ([type] = 105 or [type] = 113)
-------EXEC the stored procedure 
Declare @strOutPut NVARCHAR(127) 
exec qptreasuredb.dbo.GSP_GP_UserSignIn 1420818, 0, @strOutPut output
select * from qptreasuredb.dbo.UserItem where userid = 1420818 AND ([type] = 105 or [type] = 113)

-- delete the UserItem 
delete qptreasuredb.dbo.UserItem where userid = 1420818
-- delete the log in the signLog
delete from qptreasuredb.dbo.SignLog where UserID=1420818
-- check the value in the SignLog
select * from qptreasuredb.dbo.SignLog where UserID = 1420818
--------------------------------------------------------------------

select * from qpaccountsdb.dbo.Backpack where UserID = 1420818
select * from qptreasuredb.dbo.UserItem where userid = 1422605 AND ([type] = 105 or [type] = 113)


INSERT UserItem ([type], [count], [maxcount], [userid], [guidtag1], [guidtag2], [guidtag3], [guidtag4], source, subsource, isgamepreload, flag)
						VALUES (105, 11, 22, 1420818, 0, 0, 0, 0, 0, 0, 1, 11)
						
						
-------------------------------------- Modify the BackPack
declare @rewardtype int
declare @IsVIP int
declare @daynum int
set @rewardtype = 10
set @daynum = 1

--init the ENV delete the log in the signLog
delete from qptreasuredb.dbo.SignLog where UserID=1420818

update qptreasuredb.dbo.SignRewardConfig Set RewardType = @rewardtype where DayNum = 1 And IsVIP = 1
update qptreasuredb.dbo.SignRewardConfig Set RewardType = @rewardtype where DayNum = 1 And IsVIP = 0

select item0, item1, item2, item5, item6, item7, item8 from qpaccountsdb.dbo.Backpack Where UserID = 1420818
-------EXEC the stored procedure 
Declare @strOutPut NVARCHAR(127) 
exec qptreasuredb.dbo.GSP_GP_UserSignIn 1420818, 0, @strOutPut output
select item0, item1, item2, item5, item6, item7, item8 from qpaccountsdb.dbo.Backpack Where UserID = 1420818


select * from qptreasuredb.dbo.SignRewardConfig
update qptreasuredb.dbo.SignRewardConfig set RewardType = 1 where DayNum = 7 and IsVIP = 0
update qptreasuredb.dbo.SignRewardConfig set RewardType = 1 where DayNum = 1 and IsVIP = 1
select * from qptreasuredb.dbo.SignRewardConfig


--- test with HY
select * from qpaccountsdb.dbo.AccountsInfo where GameID = 2387298

delete qptreasuredb.dbo.SignLog where UserID = 1419819
select * from qptreasuredb.dbo.SignLog where Userid = 1419819

--Modify the last signday
	-- Normal Sign
declare @lastsigndaynum int
declare @lastsignday datetime
declare @nextsignday int
declare @IsVIP int
declare @delayday int

set @delayday = -2 -- -1 is the right sign , over -1 is the wrong sign
set @lastsignday = DATEADD(Day,@delayday,GETDATE())
set @lastsigndaynum = 1
set @IsVIP = 0

update qptreasuredb.dbo.SignLog Set LastSignDay=@lastsignday, LastSignDayNum=@lastsigndaynum 
 where UserID = 1419819 --AND IsVIP = @IsVIP
select * from qptreasuredb.dbo.SignLog where UserID = 1419819

delete SignLog where UserID = 1419819
select * from GameScoreInfo where userid = 1419819
---
Declare @strOutPut NVARCHAR(127) 
exec GSP_GP_QuerySignInInfo 1419819, @strOutPut


-- ”√ªß«©µΩ
Declare @strOutPut NVARCHAR(127) 
exec GSP_GP_UserSignIn 1419819, 1,@strOutPut output
Declare @strOutPut NVARCHAR(127) 
exec GSP_GP_UserSignIn 1419819, 0,@strOutPut output
print @strOutPut

SELECT RewardCount, RewardType FROM SignRewardConfig WHERE DayNum = 1 AND IsVIP = 0

Select  LastSignDay,  LastSignDayNum FROM SignLog WHERE UserID = 1419819 AND IsVIP = 0

-------------------------------------- modify the signrewardconfig 
declare @rewardtype int
declare @IsVIP int
declare @daynum int
set @rewardtype = 3
set @daynum = 1

--init the ENV delete the log in the signLog
delete from qptreasuredb.dbo.SignLog where UserID=1420818
update qptreasuredb.dbo.SignRewardConfig Set RewardType = 1 where DayNum = 1 And IsVIP = 1
update qptreasuredb.dbo.SignRewardConfig Set RewardType = 4 where DayNum = 1 And IsVIP = 0

update qptreasuredb.dbo.SignRewardConfig Set RewardType = 3 where DayNum = 2 And IsVIP = 1
update qptreasuredb.dbo.SignRewardConfig Set RewardType = 2 where DayNum = 2 And IsVIP = 0

update qptreasuredb.dbo.SignRewardConfig Set RewardType = 3 where DayNum = 3 And IsVIP = 1
update qptreasuredb.dbo.SignRewardConfig Set RewardType = 5 where DayNum = 3 And IsVIP = 0

update qptreasuredb.dbo.SignRewardConfig Set RewardType = 9 where DayNum = 4 And IsVIP = 1
update qptreasuredb.dbo.SignRewardConfig Set RewardType = 10 where DayNum = 4 And IsVIP = 0

update qptreasuredb.dbo.SignRewardConfig Set RewardType = 7 where DayNum = 5 And IsVIP = 1
update qptreasuredb.dbo.SignRewardConfig Set RewardType = 6 where DayNum = 5 And IsVIP = 0

update qptreasuredb.dbo.SignRewardConfig Set RewardType = 9 where DayNum = 6 And IsVIP = 1
update qptreasuredb.dbo.SignRewardConfig Set RewardType = 10 where DayNum = 6 And IsVIP = 0

update qptreasuredb.dbo.SignRewardConfig Set RewardType = 2 where DayNum = 7 And IsVIP = 1
update qptreasuredb.dbo.SignRewardConfig Set RewardType = 9 where DayNum = 7 And IsVIP = 0



select * from SignRewardConfig 
delete SignRewardConfig 
insert into SignRewardConfig (DayNum, RewardType, RewardCount, IsVIP) values (1, 1, 200, 0)
insert into SignRewardConfig (DayNum, RewardType, RewardCount, IsVIP) values (2, 1, 1200, 0)
insert into SignRewardConfig (DayNum, RewardType, RewardCount, IsVIP) values (3, 1, 1300, 0)
insert into SignRewardConfig (DayNum, RewardType, RewardCount, IsVIP) values (4, 1, 2000, 0)
insert into SignRewardConfig (DayNum, RewardType, RewardCount, IsVIP) values (5, 1, 3000, 0)
insert into SignRewardConfig (DayNum, RewardType, RewardCount, IsVIP) values (6, 1, 5000, 0)
insert into SignRewardConfig (DayNum, RewardType, RewardCount, IsVIP) values (7, 1, 10000, 0)


insert into SignRewardConfig (DayNum, RewardType, RewardCount, IsVIP) values (1, 1, 600, 1)
insert into SignRewardConfig (DayNum, RewardType, RewardCount, IsVIP) values (2, 1, 2400, 1)
insert into SignRewardConfig (DayNum, RewardType, RewardCount, IsVIP) values (3, 1, 1300, 1)
insert into SignRewardConfig (DayNum, RewardType, RewardCount, IsVIP) values (4, 1, 2000, 1)
insert into SignRewardConfig (DayNum, RewardType, RewardCount, IsVIP) values (5, 1, 3000, 1)
insert into SignRewardConfig (DayNum, RewardType, RewardCount, IsVIP) values (6, 1, 5000, 1)
insert into SignRewardConfig (DayNum, RewardType, RewardCount, IsVIP) values (7, 10, 2, 1)
 
 
 DECLARE @a int
 select @a = (1 & POWER(2, 0))
  DECLARE @b int
  set @b =1
  print convert(varchar(5), @a) + ',' + convert(varchar(5), @b)
 if(@a = 1) AND (@b = 1)
 begin
  print 'yes'
 end
 else
 BEGIN
  print 'no'
 end
 
 

 