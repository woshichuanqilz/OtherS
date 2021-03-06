USE [qptreasuredb]
GO
/****** Object:  StoredProcedure [dbo].[GSP_GP_UserSignIn]    Script Date: 05/13/2016 15:36:03 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
----------------------------------------------------------------------------------------------------

-- 用户签到
ALter PROC [dbo].[GSP_GP_UserSignIn]
	@dwUserID INT,
	@cbIsVIP BIT,
	@strErrorDescribe NVARCHAR(127) OUTPUT		-- 输出信息
AS

-- 属性设置
SET NOCOUNT ON

--变量声明
DECLARE @RealRewardCount INT
DECLARE @RealRewardType INT
DECLARE @LastSignDayNum INT
DECLARE @RealRewardSignDay INT
DECLARE @LastSignDay DateTime

-- Reward type and count
DECLARE @RewardType INT
DECLARE @RewardCount INT

DECLARE @ScoreRes BIGINT
DECLARE @LotteryRes BIGINT
SET @ScoreRes = 0
SET @LotteryRes = 0

-- 执行逻辑
BEGIN
	-- VIP 账户才能领取VIP签到奖励
	IF @cbIsVIP = 1
	BEGIN
        IF (NOT EXISTS (SELECT UserID FROM QPAccountsDB.dbo.VipInfo WHERE UserID=@dwUserID))
            BEGIN
                SET @strErrorDescribe = '对不起, 只有VIP用户才能领取' 
                RETURN 25
            END
	END

	

    -- 如果用户之前没有
    IF (NOT EXISTS (SELECT UserID FROM SignLog WHERE UserID = @dwUserID AND IsVIP = @cbIsVIP))
        BEGIN
			SELECT @RewardCount=RewardCount, @RewardType=RewardType FROM SignRewardConfig WHERE DayNum = 1 AND IsVIP = @cbIsVIP
            INSERT INTO SignLog (UserID, LastSignDay, LastSignDayNum, RewardType, RewardCount ,IsVIP) VALUES (@dwUserID, GETDATE(), 1, @RewardType, @RewardCount, @cbIsVIP)
            SET @RealRewardSignDay = 1
        END
    ELSE 
    BEGIN
		Select @LastSignDay = LastSignDay, @LastSignDayNum = LastSignDayNum FROM SignLog WHERE UserID = @dwUserID AND IsVIP = @cbIsVIP
        IF(DATEDIFF(DAY, @LastSignDay, GETDATE()) = 0) 
            BEGIN
                SET @strErrorDescribe = '今天已经签到过了, 请不要重复签到' 
                RETURN 25
            END
        ELSE IF (DATEDIFF(DAY, @LastSignDay, GETDATE()) = 1) -- 昨天已经签到, 今天正常签到 
            BEGIN
                SET @RealRewardSignDay = @LastSignDayNum + 1
                UPDATE SignLog SET LastSignDay = GETDATE(), LastSignDayNum = @RealRewardSignDay WHERE  UserID = @dwUserID 
            END
        ELSE IF (DATEDIFF(DAY, @LastSignDay, GETDATE()) > 1) -- 之前的签到作废, 从第一天开始签到
            BEGIN
                SET @RealRewardSignDay = 1
                UPDATE SignLog SET LastSignDay = GETDATE(), LastSignDayNum = @RealRewardSignDay WHERE UserID = @dwUserID 
            END
        ELSE 
            BEGIN
                SET @strErrorDescribe = '签到日期错误' 
                RETURN 25
            END
    END

    IF EXISTS (SELECT RewardCount, RewardType FROM SignRewardConfig WHERE DayNum = @RealRewardSignDay AND IsVIP = @cbIsVIP)
    BEGIN
	    SELECT @RealRewardCount = RewardCount, @RealRewardType = RewardType FROM SignRewardConfig WHERE DayNum = @RealRewardSignDay AND IsVIP = @cbIsVIP

		DECLARE @strDescriptionTmp NVARCHAR(100)
		DECLARE @ResCount BIGINT

	    IF @RealRewardType = 1  -- 金币
	        BEGIN
	            UPDATE GameScoreInfo SET Score = Score + @RealRewardCount WHERE UserID = @dwUserID
	            select @ScoreRes = Score from GameScoreInfo where UserID = @dwUserID
	            set @strDescriptionTmp = '金币'
	        END
	    ELSE IF @RealRewardType = 2  -- 奖券 注意不要和奖项的type弄混
	        BEGIN
				IF EXISTS (select Userid from qptreasuredb.dbo.UserItem where userid = @dwUserID and [Type]=105)
					BEGIN
			           UPDATE UserItem SET [Count] = [Count] + @RealRewardCount, [maxcount] = [maxcount] + @RealRewardCount  WHERE UserID = @dwUserID AND [Type]=105
			        END
		        ELSE
					BEGIN 
					   	INSERT UserItem ([type], [count], [maxcount], [userid], [guidtag1], [guidtag2], [guidtag3], [guidtag4], source, subsource, isgamepreload, flag)
						VALUES (105, @RealRewardCount, @RealRewardCount, @dwUserID, 0, 0, 0, 0, 0, 0, 1, 11)
			        END
			        
	           select @LotteryRes = [Count] from UserItem where userid = @dwUserID And [Type] = 105
	           SET @strDescriptionTmp = '奖券'
	        END
		ELSE IF @RealRewardType = 3 -- 话费
	        BEGIN
				IF EXISTS (select Userid from qptreasuredb.dbo.UserItem where userid = @dwUserID and [Type]=113)
					BEGIN
			           UPDATE UserItem SET [Count] = [Count] + @RealRewardCount, [maxcount] = [maxcount] + @RealRewardCount WHERE UserID = @dwUserID AND [Type]=113
			        END
		        ELSE
					BEGIN 
					   	INSERT UserItem ([type], [count], [maxcount], [userid], [guidtag1], [guidtag2], [guidtag3], [guidtag4], source, subsource, isgamepreload, flag)
						VALUES (113, @RealRewardCount, @RealRewardCount, @dwUserID, 0, 0, 0, 0, 0, 0, 1, 11)
			        END
			        
			   select @LotteryRes = [Count] from UserItem where userid = @dwUserID And [Type] = 105
	           SET @strDescriptionTmp = '元话费'
	        END

	    ELSE IF @RealRewardType = 4  -- 初级珍珠
	        BEGIN
	           UPDATE QPAccountsDB.dbo.Backpack SET item0 = item0 + @RealRewardCount WHERE UserID = @dwUserID 
               SET @strDescriptionTmp = '颗初级珍珠'
	        END
	    ELSE IF @RealRewardType = 5  -- 中级珍珠
	        BEGIN
	           UPDATE QPAccountsDB.dbo.Backpack SET item1 = item1 + @RealRewardCount WHERE UserID = @dwUserID 
               SET @strDescriptionTmp = '颗中级珍珠'
	        END
	    ELSE IF @RealRewardType = 6  -- 高级珍珠
	        BEGIN
	           UPDATE QPAccountsDB.dbo.Backpack SET item2 = item2 + @RealRewardCount WHERE UserID = @dwUserID 
               SET @strDescriptionTmp = '颗高级珍珠'
	        END
	    ELSE IF @RealRewardType = 7  -- 加速
	        BEGIN
	           UPDATE QPAccountsDB.dbo.Backpack SET item5 = item5 + @RealRewardCount WHERE UserID = @dwUserID 
               SET @strDescriptionTmp = '次加速'
	        END
	    ELSE IF @RealRewardType = 8  -- 散射
	        BEGIN
	           UPDATE QPAccountsDB.dbo.Backpack SET item6 = item6 + @RealRewardCount WHERE UserID = @dwUserID 
               SET @strDescriptionTmp = '次散射'
	        END
	    ELSE IF @RealRewardType = 9  -- 暴击
	        BEGIN
	           UPDATE QPAccountsDB.dbo.Backpack SET item7 = item7 + @RealRewardCount WHERE UserID = @dwUserID 
               SET @strDescriptionTmp = '次暴击'
	        END
	    ELSE IF @RealRewardType = 10  -- 小丑免费抽奖次数
	        BEGIN
	           UPDATE QPAccountsDB.dbo.Backpack SET item8 = item8 + @RealRewardCount WHERE UserID = @dwUserID 
               SET @strDescriptionTmp = '张藏宝图'
	        END
    ELSE
        BEGIN
            SET @strErrorDescribe = '奖励类型获取失败' 
           RETURN 25
        END
    SET @strErrorDescribe = '签到成功获得' + CONVERT(VARCHAR(5), @RealRewardCount) + @strDescriptionTmp 
    END
    ELSE
	    BEGIN
		    SET @strErrorDescribe = '签到信息错误无法找到对应的奖励信息' 
	    END 
END

SELECT @ScoreRes AS ScoreCount, @LotteryRes AS LotteryCount, @cbIsVIP AS IsVIP 

RETURN 24



