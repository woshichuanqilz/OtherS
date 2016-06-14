select * from Backpack where UserID in ( select UserID from AccountsInfo where GameID = 2387278)


select UserID from AccountsInfo where Accounts = 'TY_yyo6ghspe001'
select * from Backpack where UserID = 1419811

exec GSP_GP_RegisterAccounts 'livetest11', 'livetest11', '','8DAE5EFA6A9F2B4393BD367212CE5764', '8DAE5EFA6A9F2B4393BD367212CE5764', 0, 1, '', '', '', '', '127.0.0.1', 'E088BE49EE1D85235F3B82000EB0776C', 1, 1, @strErrorDescribe output
