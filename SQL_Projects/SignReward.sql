INSERT INTO SignRewardConfig (DayNum, RewardType, RewardCount, IsVIP)
VALUES (1, 1, 200, 0); 
INSERT INTO SignRewardConfig (DayNum, RewardType, RewardCount, IsVIP)
VALUES (2, 1, 1200, 0); 
INSERT INTO SignRewardConfig (DayNum, RewardType, RewardCount, IsVIP)
VALUES (3, 1, 1300, 0); 
INSERT INTO SignRewardConfig (DayNum, RewardType, RewardCount, IsVIP)
VALUES (4, 1, 2000, 0); 
INSERT INTO SignRewardConfig (DayNum, RewardType, RewardCount, IsVIP)
VALUES (5, 1, 3000, 0); 
INSERT INTO SignRewardConfig (DayNum, RewardType, RewardCount, IsVIP)
VALUES (6, 1, 5000, 0); 
INSERT INTO SignRewardConfig (DayNum, RewardType, RewardCount, IsVIP)
VALUES (7, 1, 10000, 0); 

-- Add VIP Reward
INSERT INTO SignRewardConfig (DayNum, RewardType, RewardCount, IsVIP)
VALUES (1, 1, 600, 1); 
INSERT INTO SignRewardConfig (DayNum, RewardType, RewardCount, IsVIP)
VALUES (2, 1, 2400, 1); 
INSERT INTO SignRewardConfig (DayNum, RewardType, RewardCount, IsVIP)
VALUES (3, 1, 2600, 1); 
INSERT INTO SignRewardConfig (DayNum, RewardType, RewardCount, IsVIP)
VALUES (4, 1, 4000, 1); 
INSERT INTO SignRewardConfig (DayNum, RewardType, RewardCount, IsVIP)
VALUES (5, 1, 6000, 1); 
INSERT INTO SignRewardConfig (DayNum, RewardType, RewardCount, IsVIP)
VALUES (6, 1, 10000, 1); 
INSERT INTO SignRewardConfig (DayNum, RewardType, RewardCount, IsVIP)
VALUES (7, 3, 10, 1); 


-- Add VIP 

INSERT INTO SignRewardConfig (DayNum, RewardType, RewardCount, IsVIP) VALUES (8, 1, 201, 0); 
INSERT INTO SignRewardConfig (DayNum, RewardType, RewardCount, IsVIP) VALUES (9, 1, 1201, 0); 
INSERT INTO SignRewardConfig (DayNum, RewardType, RewardCount, IsVIP) VALUES (10, 1, 1301, 0); 
INSERT INTO SignRewardConfig (DayNum, RewardType, RewardCount, IsVIP) VALUES (11, 1, 2001, 0); 
INSERT INTO SignRewardConfig (DayNum, RewardType, RewardCount, IsVIP) VALUES (12, 1, 3001, 0); 
INSERT INTO SignRewardConfig (DayNum, RewardType, RewardCount, IsVIP) VALUES (13, 1, 5001, 0); 
INSERT INTO SignRewardConfig (DayNum, RewardType, RewardCount, IsVIP) VALUES (14, 1, 10001, 0); 

-- Add VIP Reward
INSERT INTO SignRewardConfig (DayNum, RewardType, RewardCount, IsVIP) VALUES (8, 1, 601, 1); 
INSERT INTO SignRewardConfig (DayNum, RewardType, RewardCount, IsVIP) VALUES (9, 1, 2401, 1); 
INSERT INTO SignRewardConfig (DayNum, RewardType, RewardCount, IsVIP) VALUES (10, 1, 2601, 1); 
INSERT INTO SignRewardConfig (DayNum, RewardType, RewardCount, IsVIP) VALUES (11, 1, 4001, 1); 
INSERT INTO SignRewardConfig (DayNum, RewardType, RewardCount, IsVIP) VALUES (12, 1, 6001, 1); 
INSERT INTO SignRewardConfig (DayNum, RewardType, RewardCount, IsVIP) VALUES (13, 1, 10001, 1); 
INSERT INTO SignRewardConfig (DayNum, RewardType, RewardCount, IsVIP) VALUES (14, 3, 11, 1); 




Select * from SignRewardConfig

delete SignRewardConfig Where IsVIP = 1