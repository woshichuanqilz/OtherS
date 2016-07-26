--set statistics profile on 
Declare @strErrorDescribe	NVARCHAR(127) exec GSP_GR_UserTakeScore 2005, 1, 'C8A00F1550DC55D654159BB671E27D09', 250, 10086, '127.0.0.1', '123456', @strErrorDescribe out  print @strErrorDescribe

--select top 1 IsAndroid from QPAccountsDB.dbo.AccountsInfo

SELECT CASE WHEN  (2005 not in (SELECT UserID FROM QPAccountsDB.dbo.AccountsInfo WHERE IsAndroid=1))
            THEN 'true'
            ELSE 'false'
       END AS Ans
-------------------O(∩_∩)O~----------------------
-- 执行效率对比
Declare @UserID int
SET @UserID = 2005
IF ((@UserID not in (SELECT UserID FROM QPAccountsDB.dbo.AccountsInfo WHERE IsAndroid=1)))
begin
 print 'hello'
end
---
Declare @UserID1 int
SET @UserID1 = 2005
begin
 print 'hello'
end

IF ((2 - 1)<>0 or (1 + 1<>0)) and (2005 not in (SELECT UserID FROM QPAccountsDB.dbo.AccountsInfo WHERE IsAndroid=1 ))
BEGIN
	print 'hello world'
END
-------------------O(∩_∩)O~----------------------
select * from QPAccountsDB.dbo.AccountsInfo

-- 建立数据库环境
Use QPAccountsDB
go

-- 表复制
SELECT *
INTO QPAccountsDB.dbo.aTest
FROM QPAccountsDB.dbo.a

--测试表
--删除表
IF OBJECT_ID('dbo.aTest', 'U') IS NOT NULL
  DROP TABLE dbo.aTest; 
SELECT TOP 1000 [UserID]
      ,[GameID]
      ,[ProtectID]
      ,[PasswordID]
      ,[SpreaderID]
      ,[Accounts]
	  INTO QPAccountsDB.dbo.aTest
	FROM QPAccountsDB.dbo.AccountsInfo
Select * from QPAccountsDB.dbo.aTest

-- 测试 select * 的效率 (很低, 尽量避免)
select * from QPAccountsDB.dbo.AccountsInfo  
select * from QPAccountsDB.dbo.AccountsInfoTest

-- 强制不使用索引
select a.ID from QPAccountsDB.dbo.a where a.ID = 109
select a.ID from QPAccountsDB.dbo.a with (index(0)) where a.ID = 109
select * from QPAccountsDB.dbo.aTest

-- Get primary key
SELECT COLUMN_NAME
FROM INFORMATION_SCHEMA.KEY_COLUMN_USAGE
WHERE OBJECTPROPERTY(OBJECT_ID(CONSTRAINT_SCHEMA + '.' + CONSTRAINT_NAME), 'IsPrimaryKey') = 1
AND TABLE_NAME = 'TableName' AND TABLE_SCHEMA = 'Schema'
-------------------O(_)O~----------------------

-- Disable Trigger Test
alter table QPTreasureDB.dbo.GameScoreInfo
disable trigger truGameScoreInfo 
-- 模拟Trigger关键句子 Begin
Declare @dwUserID int
set @dwUserID = 2005
IF EXISTS ((SELECT UserID FROM QPAccountsDB.dbo.AccountsInfo WHERE UserID=@dwUserID And IsAndroid=0))
Begin
	print 'hello'
End
-- 模拟Trigger关键句子 End

UPDATE QPTreasureDB.dbo.GameScoreInfo SET Score=Score+1, InsureScore=InsureScore+1, Revenue=Revenue+1 WHERE UserID=2005

print 'Disable Over'
GO

--- 分割线 --- Disable/Enable Trigger

-- Enable Trigger Test
alter table QPTreasureDB.dbo.GameScoreInfo
enable trigger truGameScoreInfo 
UPDATE QPTreasureDB.dbo.GameScoreInfo SET Score=Score+1, InsureScore=InsureScore+1, Revenue=Revenue+1 WHERE UserID=2005
print 'Enable Over'
go


-- 多个update的对比
UPDATE QPTreasureDB.dbo.GameScoreInfo SET Score=1120957  WHERE UserID=2005
UPDATE QPAccountsDB.dbo.AccountsInfo SET GameID =1120957 WHERE UserID=2005
UPDATE QPTreasureDB.dbo.SignLog SET Score=500 WHERE ID=1894178
UPDATE QPTreasureDB.dbo.SignDay SET OneDay=0 WHERE ID=1
--select top 1 * from  QPTreasureDB.dbo.SignDay 
GO

-------------------O(_)O~----------------------
Declare @dwUserID int
set @dwUserID = 2005
if @dwUserID not in (SELECT UserID FROM QPAccountsDB.dbo.AccountsInfo WHERE IsAndroid=1)

-------------------O(∩_∩)O~----------------------
Declare @dwUserID int
set @dwUserID = 2005
IF EXISTS (SELECT UserID FROM QPAccountsDB.dbo.AccountsInfo WHERE UserID=@dwUserID And IsAndroid=0)
begin
	print 'yes'
end

IF (@dwUserID not in (select userid FROM QPAccountsDB.dbo.AccountsInfo WHERE IsAndroid=1))
begin
	print 'yes'
end
GO

-------------------O(_)O~----------------------
-- 建表效率
-- 方式一
--set IDENTITY_INSERT Userid on
SELECT userid INTO #temp_MSIMZ FROM QPAccountsDB.dbo.AccountsInfo
 
--方式二
CREATE TABLE #temp_MSIMZ1
(userid int)
 
INSERT INTO #temp_MSIMZ1
        (userid)
SELECT userid FROM QPAccountsDB.dbo.AccountsInfo
drop table #temp_MSIMZ
drop table #temp_MSIMZ1

-------------------O(∩_∩)O~----------------------
-- Temporay handle the result
with t
as(select 1 as userid)
select userid, nickname from QPAccountsDB.dbo.AccountsInfo where userid = 2005
-------------------O(∩_∩)O~----------------------
set statistics profile on 
set statistics profile off

set showplan_all on
set showplan_all off
--SET SHOWPLAN_XML on --{ON |  OFF}
SET SHOWPLAN_XML off --{ON |  OFF}
select userid, nickname from QPAccountsDB.dbo.AccountsInfo where userid = 2005
GO

-------------------O(_)O~----------------------
select * from QPAccountsDB.dbo.AccountsInfo where nickname = 'dddee1'

SELECT isnull(SUM(Score),0),GETDATE()-1
from QPTreasureDB.dbo.SignLog(nolock)
where SignDate between CONVERT(NVARCHAR(10),GETDATE()-1,120) And CONVERT(NVARCHAR(10),GETDATE(),120)

-------------------O(∩_∩)O~----------------------
-- stored procedure Search
SELECT DISTINCT 
       o.name AS Object_Name,
       o.type_desc
  FROM sys.sql_modules m 
       INNER JOIN 
       sys.objects o 
         ON m.object_id = o.object_id
 WHERE m.definition Like '%MemberOrder%';

select * from sys.sql_modules
select * from sys.objects

-------------------O(∩_∩)O~----------------------
Use tempdb
Go
exec sp_spaceused
Go

-------------------O(∩_∩)O~----------------------
Use QPAccountsDB
Go

select * from AccountsInfo where UserID < 5000
select * from AccountsInfo where GameID < 1067497

select userid from QPAccountsDB.dbo.AccountsInfo where NickName='鲍安国'

select IsBlack from QPAccountsDB.dbo.AccountsInfo where IsBlack=0

use QPAccountsDB
go
drop index IX_AccountsInfo_IsBlack on AccountsInfo
Go

select IsBlack from QPAccountsDB.dbo.AccountsInfo where IsBlack=1
CREATE INDEX IX_AccountsInfo_IsBlack ON AccountsInfo (isblack)
select IsBlack from QPAccountsDB.dbo.AccountsInfo where IsBlack=1
drop index IX_AccountsInfo_IsBlack on AccountsInfo

---
UPDATE TB SET IsBlack=1
FROM (
select *,ROW_NUMBER()over(order by userid) nb
from QPAccountsDB.dbo.AccountsInfo)TB
WHERE nb%2=0 --修改第一行

-------------------O(∩_∩)O~----------------------
-- 复合索引测试, where的顺序会不会影响查询的结果。
USE QPAccountsDB
GO
create index ix_test on accountsinfo (Gameid, isblack)
drop index IX_AccountsInfo_GameID on AccountsInfo
drop index IX_AccountsInfo_IsBlack on AccountsInfo
Go
select * from AccountsInfo where GameID=1056225 and IsBlack=1
select * from AccountsInfo where IsBlack=1 and GameID=1056225
GO

CREATE index IX_AccountsInfo_GameID on AccountsInfo(gameid)
CREATE index IX_AccountsInfo_IsBlack on AccountsInfo(isblack)
-------------------O(∩_∩)O~----------------------

-------------------O(∩_∩)O~----------------------
-- 取消测试结果
begin tran
Use QPAccountsDB
Go
select * from AccountsInfo where NickName='鲍安国'
update AccountsInfo set ProtectID=1989 where NickName='鲍安国'
select * from AccountsInfo where NickName='鲍安国'
rollback tran

-------------------O(∩_∩)O~----------------------
-- RID Lookup , Non-cluster Lookup
set statistics profile on 
select * from AccountsInfo where GameID='1057497'
select * from AccountsInfo where NickName='鲍安国'
select * from AccountsInfo where userid=3

select GameID from AccountsInfo where GameID='1057497'
select UserID, GameID from AccountsInfo where GameID='1057497'
select UserID, GameID, Gender from AccountsInfo where GameID='1057497'
select UserID, GameID, Gender from AccountsInfo where Userid=164

--如果不考虑索引的跳转两个查找的性能比较
select GameID from AccountsInfo where GameID=1057497
select UserID from AccountsInfo where Userid=3

-------------------O(∩_∩)O~----------------------
select count(distinct gender) from QPAccountsDB.dbo.AccountsInfo -- ans is 2


--书签查找
select GameID, Isblack from QPAccountsDB.dbo.AccountsInfo where GameID=1057497
select GameID from QPAccountsDB.dbo.AccountsInfo where GameID=1057497
-------------------O(∩_∩)O~----------------------
-- 聚集索引, 非聚集索引对比
Use QPAccountsDB
Go
IF EXISTS (SELECT name FROM sysindexes WHERE name = 'IX_TestTable_TestCol1') 
DROP INDEX test.IX_TestTable_TestCol1

CREATE NONCLUSTERED INDEX IX_TestTable_TestCol1 ON QPAccountsDB.dbo.Test (UserID); 
select UserID from QPAccountsDB.dbo.Test Where UserID < 5000
DROP INDEX IX_TestTable_TestCol1 ON QPAccountsDB.dbo.Test;
CREATE CLUSTERED INDEX IX_TestTable_TestCol1 ON QPAccountsDB.dbo.Test (UserID); 
select UserID from QPAccountsDB.dbo.Test Where UserID < 5000
DROP INDEX IX_TestTable_TestCol1 ON QPAccountsDB.dbo.Test;


Use QPAccountsDB
Go
IF EXISTS (SELECT name FROM sysindexes WHERE name = 'IX_TestTable_TestCol1') 
DROP INDEX test.IX_TestTable_TestCol1

CREATE NONCLUSTERED INDEX IX_TestTable_TestCol1 ON QPAccountsDB.dbo.Test (UserID); 
select UserID from QPAccountsDB.dbo.Test Where UserID = 2
DROP INDEX IX_TestTable_TestCol1 ON QPAccountsDB.dbo.Test;
CREATE CLUSTERED INDEX IX_TestTable_TestCol1 ON QPAccountsDB.dbo.Test (UserID); 
select UserID from QPAccountsDB.dbo.Test Where UserID = 2
DROP INDEX IX_TestTable_TestCol1 ON QPAccountsDB.dbo.Test;

-------------------O(∩_∩)O~----------------------


select * from sys.indexes
where object_id = (select object_id from sys.objects where name = 'test')
Select * from sys.objects ;
select * from sys.objects where name = 'test'

--父子关系的objectid
Select * from sys.objects where name = 'IX_ID';
select * from sys.indexes where name = 'IX_ID';

-------------------O(_)O~----------------------
-- 索引丢失 - 没有测试出来想要的结果。
begin tran
declare @UD int 
set @UD = 20
select * from [QPAccountsDB].[dbo].[Test] where UserID < @UD
select * from [QPAccountsDB].[dbo].[Test] where UserID < 20
commit tran

-------------------O(∩_∩)O~----------------------
--测试in的效率
CREATE CLUSTERED INDEX IX_TestTable_TestCol1 ON QPAccountsDB.dbo.Test (UserID); 
Select userid from [QPAccountsDB].[dbo].[Test] where userid in (2,3,4)
Select userid from [QPAccountsDB].[dbo].[Test] where userid in (2,3,4)
--Select userid from [QPAccountsDB].[dbo].[Test] where userid between 2 and 4;


-------------------O(∩_∩)O~----------------------
--内部碎片检测。
EXEC sp_configure 'show advanced options', 1
GO
RECONFIGURE WITH OVERRIDE
GO
DECLARE @DefaultFillFactor INT 
DECLARE @Fillfactor TABLE
    (
      Name VARCHAR(100) ,
      Minimum INT ,
      Maximum INT ,
      config_value INT ,
      run_value INT
    )
INSERT  INTO @Fillfactor
        EXEC sp_configure 'fill factor (%)'     
SELECT  @DefaultFillFactor = CASE WHEN run_value = 0 THEN 100
                                  ELSE run_value
                             END
FROM    @Fillfactor 

SELECT  DB_NAME() AS DBname ,
        QUOTENAME(s.name) AS CchemaName ,
        QUOTENAME(o.name) AS TableName ,
        i.name AS IndexName ,
        stats.Index_type_desc AS IndexType ,
        stats.page_count AS [PageCount] ,
        stats.partition_number AS PartitionNumber ,
        CASE WHEN i.fill_factor > 0 THEN i.fill_factor
             ELSE @DefaultFillFactor
        END AS [Fill Factor] ,
        stats.avg_page_space_used_in_percent ,
        CASE WHEN stats.index_level = 0 THEN 'Leaf Level'
             ELSE 'Nonleaf Level'
        END AS IndexLevel
FROM    sys.dm_db_index_physical_stats(DB_ID(), NULL, NULL, NULL, 'DETAILED')
        AS stats ,
        sys.objects AS o ,
        sys.schemas AS s ,
        sys.indexes AS i
WHERE   o.OBJECT_ID = stats.OBJECT_ID
        AND s.schema_id = o.schema_id
        AND i.OBJECT_ID = stats.OBJECT_ID
        AND i.index_id = stats.index_id
        AND stats.avg_page_space_used_in_percent <= 85
        AND stats.page_count >= 10
        AND stats.index_id > 0
ORDER BY stats.avg_page_space_used_in_percent ASC ,
        stats.page_count DESC

-------------------O(∩_∩)O~----------------------
-- 表 索引的数据统计
select  db_name(US.database_id)
	, object_name(US.object_id)
	, I.name as IndexName
	, OS.leaf_allocation_count
	, OS.nonleaf_allocation_count
	, OS.leaf_page_merge_count
	, OS.leaf_insert_count
	, OS.leaf_delete_count
	, OS.leaf_update_count
	, *
from sys.dm_db_index_usage_stats US
	join sys.indexes I 
		on I.object_id = US.object_id 
		and I.index_id = US.index_id
	join sys.dm_db_index_operational_stats(db_id(), null, null, null) OS
		on OS.object_id = I.object_id and OS.index_id = I.Index_id
where	I.type <> 0  -- not heap
	and object_name(US.object_id) not like 'sys%'
order by	OS.leaf_allocation_count desc, 
		OS.nonleaf_allocation_count desc, 
		OS.leaf_page_merge_count desc,
		US.User_updates desc, 
		US.User_Seeks desc, 
		US.User_Scans desc, 
		US.User_Lookups desc
-------------------O(∩_∩)O~----------------------
select  db_name(US.database_id)
	, object_name(US.object_id)
	, *
from sys.dm_db_index_usage_stats US
	join sys.indexes I 
		on I.object_id = US.object_id 
		and I.index_id = US.index_id
	join sys.dm_db_index_operational_stats(db_id(), null, null, null) OS
		on OS.object_id = I.object_id and OS.index_id = I.Index_id
where	I.type <> 0  -- not heap
	and object_name(US.object_id) not like 'sys%'
order by	OS.leaf_allocation_count desc, 
		OS.nonleaf_allocation_count desc, 
		OS.leaf_page_merge_count desc,
		US.User_updates desc, 
		US.User_Seeks desc, 
		US.User_Scans desc, 
		US.User_Lookups desc


Select * from sys.indexes si
join sys.dm_db_index_usage_stats us
on si.object_id = us.object_id;


-------------------O(∩_∩)O~----------------------
-- Get all the index from a table
SELECT i.name AS index_name
    ,i.type_desc
    ,is_unique
    --,ds.type_desc AS filegroup_or_partition_scheme
    --,ds.name AS filegroup_or_partition_scheme_name
    ,ignore_dup_key
    ,is_primary_key
    ,is_unique_constraint
    ,fill_factor
    ,is_padded
    ,is_disabled
    ,allow_row_locks
    ,allow_page_locks
FROM sys.indexes AS i
INNER JOIN sys.data_spaces AS ds ON i.data_space_id = ds.data_space_id
WHERE is_hypothetical = 0 AND i.index_id <> 0 
AND i.object_id = OBJECT_ID('QPAccountsDB.dbo.AccountsInfo');
GO

-------------------O(∩_∩)O~----------------------
-- how to use Object ID
USE QPAccountsDB;
GO
SELECT OBJECT_ID(N'QPAccountsDB.dbo.AccountsInfo') AS 'Object ID';
GO

-------------------O(∩_∩)O~----------------------

-- Get the read time and the write time of the table
DECLARE @dbid int
SELECT @dbid = db_id('QPAccountsDB')

SELECT TableName = object_name(s.object_id),
       Reads = SUM(user_seeks + user_scans + user_lookups), Writes =  SUM(user_updates)
FROM sys.dm_db_index_usage_stats AS s
INNER JOIN sys.indexes AS i
ON i.object_id = s.object_id
AND i.index_id = s.index_id
WHERE objectproperty(s.object_id,'IsUserTable') = 1
AND s.database_id = @dbid
GROUP BY object_name(s.object_id)
ORDER BY writes DESC


-------------------O(∩_∩)O~----------------------
--Create View
CREATE VIEW myView AS
SELECT * FROM QPAccountsDB.dbo.AccountsFace

Select * from myView;

------

use Test
Go
IF OBJECT_ID('t1', 'U') IS NOT NULL
  DROP TABLE dbo.t1; 
IF OBJECT_ID('t2', 'U') IS NOT NULL
  DROP TABLE dbo.t2; 
  
--- Create table
CREATE TABLE t1
(
id INT NOT NULL IDENTITY(1,1) PRIMARY KEY
)

CREATE TABLE t2
(
id INT NOT NULL IDENTITY(1,1) PRIMARY KEY
)

set IDENTITY_INSERT t1 on
set IDENTITY_INSERT t2 on
declare @i int
select @i=1
while(@i<=100)
begin
  INSERT INTO t1 (id) VALUES(@i)
  select @i=@i+1
end 

select @i=1
while(@i<=200)
begin
  insert into t2 values(@i)
  select @i=@i+1
end 

CREATE INDEX IX_t1 ON t1 (id)
CREATE INDEX IX_t2 ON t1 (id)
--create index here
DECLARE @deltest varchar(20) = 'deltest';
Begin tran @deltest
delete from t1
where exists 
(select ID from t2 where t1.id=t2.id)
rollback tran @deltest

DECLARE @deltest varchar(20) = 'deltest';
Begin tran @deltest
delete from t1
where ID in(select ID from t2)
rollback tran @deltest

DECLARE @deltest varchar(20) = 'deltest';
Begin tran @deltest
Merge t1 
Using t2 on t1.id = t2.id
WHen Matched then
DELETE;
select * from t1
rollback tran @deltest

select COUNT(*) from t1

--- test the insert for the xxx
Use Test
GO
Begin Tran
--set IDENTITY_INSERT [QPTreasureDBFull].[dbo].[RecordUserInout] ON;
MERGE t1 AS T
USING Test2.dbo.t1 AS S ON T.id = S.id
WHEN NOT MATCHED
THEN INSERT VALUES
(
S.[id]
);
select COUNT(*) from t1
rollback tran

use Test2
Go
IF OBJECT_ID('t1', 'U') IS NOT NULL
  DROP TABLE dbo.t1; 
IF OBJECT_ID('t2', 'U') IS NOT NULL
  DROP TABLE dbo.t2; 
  
CREATE TABLE t1
(
id int NOT NULL,
PRIMARY KEY (id)
)

CREATE TABLE t2
(
id int NOT NULL,
PRIMARY KEY (id)
)

declare @i int
select @i=1
while(@i<=500)
begin
  insert into t1 values(@i)
  select @i=@i+1
end 
-------------------O(∩_∩)O~---------------------- 

set statistics profile on 

Use qpaccountsdb
go
Declare	@strErrorDescribe NVARCHAR(127)		-- 输出信息
	
exec GSP_GP_EfficacyAccounts "emigwlib6a0d", "2FE985C99F0D30CED706301F685CEBA1", "371321199011067811", "127.0.0.1", "machineID", 0, @strErrorDescribe output


Declare	@strErrorDescribe NVARCHAR(127)		-- 输出信息
exec GSP_GP_RegisterAccounts 'livetest11', 'livetest11', '','8DAE5EFA6A9F2B4393BD367212CE5764', '8DAE5EFA6A9F2B4393BD367212CE5764', 0, 1, '', '', '', '', '127.0.0.1', 'E088BE49EE1D85235F3B82000EB0776C', 1, 1, @strErrorDescribe output


drop IX_ShareDetailInfo_PayAmount on AccountsInfo
declare @totalCharge int
select @totalCharge = SUM(PayAmount) from QPTreasureDB.dbo.ShareDetailInfo(nolock) where UserID = 2527


CREATE NONCLUSTERED INDEX IX_ShareDetailInfo_PayAmount ON QPTreasureDB.dbo.ShareDetailInfo (PayAmount) with (drop_existing = ON); 
declare @totalCharge int
select @totalCharge = SUM(PayAmount) from QPTreasureDB.dbo.ShareDetailInfo(nolock) where UserID = 2527
drop Index IX_ShareDetailInfo_PayAmount on QPTreasureDB.dbo.ShareDetailInfo
--declare @totalCharge int
select @totalCharge = SUM(PayAmount) from QPTreasureDB.dbo.ShareDetailInfo(nolock) where UserID = 2527

--



	
DECLARE @strPassPortID NVARCHAR(18)				-- 身份证号
DECLARE @strClientIP NVARCHAR(15)
DECLARE	@strMachineID NVARCHAR(32)					-- 机器标识
DECLARE @UserID INT
DECLARE @LastLogonDate DATETIME

UPDATE AccountsInfo SET GameLogonTimes=GameLogonTimes+1,LastLogonDate=GETDATE(), LastLogonIP=@strClientIP,
LastLogonMachine=@strMachineID,PassPortID=@strPassPortID WHERE UserID=@UserID

UPDATE AccountsInfo SET 
--GameLogonTimes=GameLogonTimes+1,
LastLogonDate=GETDATE(), 
LastLogonIP=@strClientIP,
LastLogonMachine=@strMachineID,
PassPortID=@strPassPortID 
WHERE UserID=@UserID

client ip 5%

-- grand right
Use master
Go
GRANT ALTER TRACE TO lele2_game;
GRANT VIEW SERVER STATE TO lele2_game; 

