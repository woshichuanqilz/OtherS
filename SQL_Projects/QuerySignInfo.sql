----------------------------------------------------------------------------------------------------

-- 用户签到
CREATE PROC [dbo].[GSP_GP_QuerySignInInfo]
	@dwUserID INT,
	@strErrorDescribe NVARCHAR(127) OUTPUT		-- 输出信息
AS

-- 属性设置
SET NOCOUNT ON

--变量声明
-- Sign Day 
DECLARE @SignDay INT
DECLARE @LastSignDay DATETIME
DECLARE @SignDayNum INT

--Sign info for Everyday
DECLARE @Day1RewardType INT
DECLARE @Day1RewardCount INT
DECLARE @Day2RewardType INT
DECLARE @Day2RewardCount INT
DECLARE @Day3RewardType INT
DECLARE @Day3RewardCount INT
DECLARE @Day4RewardType INT
DECLARE @Day4RewardCount INT
DECLARE @Day5RewardType INT
DECLARE @Day5RewardCount INT
DECLARE @Day6RewardType INT
DECLARE @Day6RewardCount INT
DECLARE @Day7RewardType INT
DECLARE @Day7RewardCount INT

DECLARE @Day1RewardTypeVIP INT
DECLARE @Day1RewardCountVIP INT
DECLARE @Day2RewardTypeVIP INT
DECLARE @Day2RewardCountVIP INT
DECLARE @Day3RewardTypeVIP INT
DECLARE @Day3RewardCountVIP INT
DECLARE @Day4RewardTypeVIP INT
DECLARE @Day4RewardCountVIP INT
DECLARE @Day5RewardTypeVIP INT
DECLARE @Day5RewardCountVIP INT
DECLARE @Day6RewardTypeVIP INT
DECLARE @Day6RewardCountVIP INT
DECLARE @Day7RewardTypeVIP INT
DECLARE @Day7RewardCountVIP INT
-- 执行逻辑
BEGIN
    SELECT @LastSignDay= LastSignDay, @SignDayNum = LastSignDayNum from SignLog Where UserID = @dwUserID

    IF(DATEDIFF(DAY, @LastSignDay, GETDATE()) = 0) 
        BEGIN
            SET @SignDay = -1  -- 没有可以签到的日子
        END
    ELSE IF(DATEDIFF(DAY, @LastSignDay, GETDATE()) = 1)
        BEGIN
            SET @SignDay = @SignDayNum + 1
        END
    ELSE IF(DATEDIFF(DAY, @LastSignDay, GETDATE()) > 1)
        BEGIN
            SET @SignDay = 1
        END

    -- Get the value of the Normal Use
    SELECT @Day1RewardType = RewardType, @Day1RewardCount = RewardCount From SignRewardConfig Where DayNum = 1 AND IsVIP = 0
    SELECT @Day2RewardType = RewardType, @Day2RewardCount = RewardCount From SignRewardConfig Where DayNum = 2 AND IsVIP = 0
    SELECT @Day3RewardType = RewardType, @Day3RewardCount = RewardCount From SignRewardConfig Where DayNum = 3 AND IsVIP = 0
    SELECT @Day4RewardType = RewardType, @Day4RewardCount = RewardCount From SignRewardConfig Where DayNum = 4 AND IsVIP = 0
    SELECT @Day5RewardType = RewardType, @Day5RewardCount = RewardCount From SignRewardConfig Where DayNum = 5 AND IsVIP = 0
    SELECT @Day6RewardType = RewardType, @Day6RewardCount = RewardCount From SignRewardConfig Where DayNum = 6 AND IsVIP = 0
    SELECT @Day7RewardType = RewardType, @Day7RewardCount = RewardCount From SignRewardConfig Where DayNum = 7 AND IsVIP = 0

    -- Get the value of the VIP User
    SELECT @Day1RewardTypeVIP = RewardType, @Day1RewardCountVIP = RewardCount From SignRewardConfig Where DayNum = 1 AND IsVIP = 1
    SELECT @Day2RewardTypeVIP = RewardType, @Day2RewardCountVIP = RewardCount From SignRewardConfig Where DayNum = 2 AND IsVIP = 1
    SELECT @Day3RewardTypeVIP = RewardType, @Day3RewardCountVIP = RewardCount From SignRewardConfig Where DayNum = 3 AND IsVIP = 1
    SELECT @Day4RewardTypeVIP = RewardType, @Day4RewardCountVIP = RewardCount From SignRewardConfig Where DayNum = 4 AND IsVIP = 1
    SELECT @Day5RewardTypeVIP = RewardType, @Day5RewardCountVIP = RewardCount From SignRewardConfig Where DayNum = 5 AND IsVIP = 1
    SELECT @Day6RewardTypeVIP = RewardType, @Day6RewardCountVIP = RewardCount From SignRewardConfig Where DayNum = 6 AND IsVIP = 1
    SELECT @Day7RewardTypeVIP = RewardType, @Day7RewardCountVIP = RewardCount From SignRewardConfig Where DayNum = 7 AND IsVIP = 1


    -- Return the value of the 
    SELECT @SignDay AS SignDay, @Day1RewardType, 
            @Day1RewardTypeVIP AS Day1RewardTypeVIP,
            @Day1RewardCount AS Day1RewardCount,
            @Day2RewardType AS Day2RewardType,
            @Day2RewardCount AS Day2RewardCount,
            @Day3RewardType AS Day3RewardType,
            @Day3RewardCount AS Day3RewardCount,
            @Day4RewardType AS Day4RewardType,
            @Day4RewardCount AS Day4RewardCount,
            @Day5RewardType AS Day5RewardType,
            @Day5RewardCount AS Day5RewardCount,
            @Day6RewardType AS Day6RewardType,
            @Day6RewardCount AS Day6RewardCount,
            @Day7RewardType AS Day7RewardType,
            @Day7RewardCount AS Day7RewardCount,
            @Day1RewardTypeVIP AS Day1RewardTypeVIP,
            @Day1RewardCountVIP AS Day1RewardCountVIP,
            @Day2RewardTypeVIP AS Day2RewardTypeVIP,
            @Day2RewardCountVIP AS Day2RewardCountVIP,
            @Day3RewardTypeVIP AS Day3RewardTypeVIP,
            @Day3RewardCountVIP AS Day3RewardCountVIP,
            @Day4RewardTypeVIP AS Day4RewardTypeVIP,
            @Day4RewardCountVIP AS Day4RewardCountVIP,
            @Day5RewardTypeVIP AS Day5RewardTypeVIP,
            @Day5RewardCountVIP AS Day5RewardCountVIP,
            @Day6RewardTypeVIP AS Day6RewardTypeVIP,
            @Day6RewardCountVIP AS Day6RewardCountVIP,
            @Day7RewardTypeVIP AS Day7RewardTypeVIP,
            @Day7RewardCountVIP AS Day7RewardCountVIP
END

RETURN 0



