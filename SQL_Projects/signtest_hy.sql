--Modify the last signday
	-- Normal Sign
	
	--- 2388317 me
	--- 2387298 hy
	--- 2391748 ws
	--- 2391760 testserever ws
	--- 2393697
use qptreasuredb
go	
declare @lastsigndaynum int
declare @lastsignday datetime
declare @nextsignday int
declare @delayday int
declare @IsVIP int

set @delayday = -1 -- -1 is the right sign , over -1 is the wrong sign
set @lastsignday = DATEADD(Day,@delayday,GETDATE())
set @lastsigndaynum = 1
set @IsVIP = 0

update qptreasuredb.dbo.SignLog Set LastSignDay=@lastsignday, LastSignDayNum=@lastsigndaynum, ExtraBonusSignedDay = 2
, IsSignInInterrupt = 0, RewardType = 1
 where Userid in (select userid from qpaccountsdb.dbo.AccountsInfo where GameID = 2391788)
 
select * from qptreasuredb.dbo.SignLog where UserID in
(
select userid from qpaccountsdb.dbo.AccountsInfo where GameID = 2391788  
)


-- Exec the storage procedure
declare @stroutput nvarchar(127)
exec qptreasuredb.dbo.GSP_GP_QuerySignInInfo 1428656, @stroutput output

-- DELETE signlog
delete qptreasuredb.dbo.SignLog where UserID in
(
select userid from qpaccountsdb.dbo.AccountsInfo where GameID = 2396276  
)

-- DELETE cannon config 
select userid, item4 FROM qpaccountsdb.dbo.Backpack WHERE UserID in
(
select userid from qpaccountsdb.dbo.AccountsInfo where GameID = 2391786  
)
update qpaccountsdb.dbo.Backpack set item4 = 0 where userid in 
(
select userid from qpaccountsdb.dbo.AccountsInfo where GameID = 2391786  
)

update SignLog SET BounsSignedDay  = 3 WHERE userid in 
(
select userid from qpaccountsdb.dbo.AccountsInfo where GameID = 2391760  
)
-- Exec the storage procedure
declare @stroutput nvarchar(127)
exec qptreasuredb.dbo.GSP_GP_QuerySignInInfo 1419819, @stroutput output

declare @stroutput nvarchar(127)
exec qptreasuredb.dbo.GSP_GP_UserSignIn 1419819, 1,@stroutput output

 insert into qpaccountsdb.dbo.VipInfo(UserID, VipExp, VipLevel, EverydayGift, ChargeBack, Nullity)
 values(1428656,0,1,0,0,0)
 SELECT * FROM qpaccountsdb.dbo.VipInfo WHERE UserID = 1424170
 
 SELECT * FROM qptreasuredb.dbo.SignRewardConfig
 
 -------
  DECLARE @BounsSignDay INT
  DECLARE @SignDayNum INT 
  DECLARE @LastSignDay INT
  DECLARE @RewardCounttemp INT 
  DECLARE @RewardTypetemp INT 
  DECLARE @RewardValueTable table(daynum INT , rewardcount INT , rewardtype int)
  
  SELECT  @BounsSignDay = BounsSignedDay  ,@lastsignday = LastSignDayNum FROM qptreasuredb.dbo.SignLog WHERE UserID in 
(
select userid from qpaccountsdb.dbo.AccountsInfo where GameID = 2387298  
)  

DECLARE @LoopCount int 
SET @LoopCount = 1
while(@LoopCount <= 7)
 BEGIN 
	IF(@LoopCount <= @BounsSignDay)
	BEGIN
		SELECT @RewardCounttemp = RewardCount, @RewardTypetemp = RewardType FROM qptreasuredb.dbo.SignRewardConfig 
		where DayNum = @LoopCount AND IsExtraBouns = 1
	END
	else
	BEGIN 
		SELECT @RewardCounttemp = RewardCount, @RewardTypetemp = RewardType FROM qptreasuredb.dbo.SignRewardConfig 
		where DayNum = @LoopCount AND IsExtraBouns = 0
	END 
	INSERT INTO @RewardValueTable(daynum, rewardcount, rewardtype) VALUES (@LoopCount, @RewardCounttemp, @RewardTypetemp); 
	SET @LoopCount = @LoopCount + 1
 END  
 
 SELECT * FROM @RewardValueTable
 
 UPDATE SignRewardConfig SET RewardCount = RewardCount + 1 WHERE isextrabouns = 1
  SELECT * FROM SignRewardConfig 
  
 
 
----
Use qptreasuredb
GO 
 SELECT * FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME = 'SignRewardConfig'
 GO
 

----
SELECT * FROM qptreasuredb.dbo.SignLog
