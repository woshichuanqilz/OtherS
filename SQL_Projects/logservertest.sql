USE StatisticsDB
GO

select * from ClownOperation where UserID = 1424258
delete from ClownOperation 


Use StatisticsDB
GO
exec LOG_GSP_GR_INSERT_CLOWNOPERATION 1424258,1,1,10443,100,"2016-05-11 18:10:38",0,0 ;