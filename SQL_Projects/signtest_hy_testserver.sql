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

set @delayday = 0 -- -1 is the right sign , over -1 is the wrong sign
set @lastsignday = DATEADD(Day,@delayday,GETDATE())
set @lastsigndaynum = 3
set @IsVIP = 0

update qptreasuredb.dbo.SignLog Set LastSignDay=@lastsignday, LastSignDayNum=@lastsigndaynum, ExtraBonusSignedDay = 2
, IsSignInInterrupt = 0, RewardType = 1
 where Userid in (select userid from qpaccountsdb.dbo.AccountsInfo where GameID = 2387298)
 
select * from qptreasuredb.dbo.SignLog where UserID in
(
select userid from qpaccountsdb.dbo.AccountsInfo where GameID = 2387298  
)

-- Exec the storage procedure
declare @stroutput nvarchar(127)
exec qptreasuredb.dbo.GSP_GP_QuerySignInInfo 1419819, @stroutput outpuGO

 DECLARE @a INT
 SET @a = -1
SELECT ABS(@a)