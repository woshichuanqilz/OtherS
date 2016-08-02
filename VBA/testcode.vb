Sub Hello()
    Worksheets("Sheet1").Activate
    Range("A1:H8").Formula = "=Rand()"    'Range is on the active sheet
End Sub



Private Sub Worksheet_Change(ByVal Target As Range)

If Target.Row = 1 And Target.Column = 1 Then
   
   Application.EnableEvents = False
   
   Range("B1") = Range("A1") + Range("A1") + 1
   
   Application.EnableEvents = True

End If
End Sub

