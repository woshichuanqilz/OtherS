Dim Gains As Long '收益，公式计算用
Dim Consume As Long '消耗，公式计算用
Dim Weight As Integer '新权重
Dim OriginalWeight As Integer '原始权重，鱼的原始权重
Dim Multiple As Integer '倍数，鱼的倍数
Dim WeightCoefficient As Double '权重系数，公式计算用
Dim Upperlimit As Double '波峰，需填写
Dim Lowerlimit As Double '波谷，需填写
Dim ExpectYields As Double '期望收益比，公式计算用
Dim UL As Integer '上升或下降，公式计算用
Dim Points As Integer '半周期点个数，需填写
Dim Yields As Double '实际收益比，公式计算用
Dim ShootTimes As Integer '开炮N次变动一次概率，需填写
Dim ShootPrice As Integer '开炮金币数
Dim Crr() '期望收益比数组
Points = 100 '暂定半周期100个点
ReDim Crr(1 To Points)
ShootTimes = 50 '暂定50炮变动一次概率
Upperlimit = 1.1 '暂定波峰为1.1
Lowerlimit = 0.6 '暂定波谷为0.6
'*****************************以下终身循环*****************************
If Yields > (Upperlimit + Lowerlimit) / 2 Then '根据当前收益率来判断下次波峰/波谷
    UL = 1 '波谷
Else
    UL = 0 '波峰
End If
For i = 1 To Points '期望收益比曲线
        Crr(i) = Sin(Application.Pi() * (i * (1 / Points) + UL)) * ((Upperlimit - Lowerlimit) / 2) + Upperlimit - (Upperlimit - Lowerlimit) / 2
Next
For i = 1 To Points '半周期开始
    For m = 1 To ShootTimes '开N炮变动概率
        WeightCoefficient = Crr(i) / (OriginalWeight * Multiple / 10000) '根据收益率得到新权重系数
        Weight = Int(OriginalWeight * WeightCoefficient) '新权重
        rand = Int(10000 * Rnd + 1) '是否打中
        If rand <= Weight Then
            Gains = Gains + Price * Multiple '打中，收益上升
        End If
        Consume = Consume + Price '开出一炮扣费，消耗加大
        Yields = Gains / Consume '结算收益率
        Next
    Next
'*****************************半周期结束后，回到循环开始判定下次半周期波峰/波谷*****************************
