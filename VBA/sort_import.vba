Sub sorttest()

Range("A1:A4").sort key1:=Range("A1:A4"), _
order1:=1, Header:=xlNo

End Sub


  Sub ImportTextFile()

       
       Dim DestBook As Workbook, SourceBook As Workbook
       Dim DestCell As Range
       Dim RetVal As Boolean

       ' Turn off screen updating.
       Application.ScreenUpdating = False

       ' Set object variables for the active book and active cell.
       Set DestBook = ActiveWorkbook
       Set DestCell = ActiveCell

       ' Show the Open dialog box.
       RetVal = Application.Dialogs(xlDialogOpen).Show("D:\win7ÎÒµÄÎÄµµ-×ÀÃæ-ÊÕ²Ø¼Ð\Desktop\*")

       ' If Retval is false (Open dialog canceled), exit the procedure.
       If RetVal = False Then Exit Sub

       ' Set an object variable for the workbook containing the text file.
       Set SourceBook = ActiveWorkbook

       ' Copy the contents of the entire sheet containing the text file.
       Range(Range("A1"), Range("A1").SpecialCells(xlLastCell)).Copy

       ' Activate the destination workbook and paste special the values
       ' from the text file.
       DestBook.Activate
       DestCell.PasteSpecial Paste:=xlValues

       ' Close the book containing the text file.
       SourceBook.Close False

   End Sub

Sub ImportCSV()

Dim vPath As Variant
Dim wb As Excel.Workbook
Dim ws As Excel.Worksheet

Set wb = Excel.ActiveWorkbook
Set ws = Excel.ActiveSheet

vPath = Application.GetOpenFilename("CSV (Comma Delimited) (*.csv),*.csv" _
, 1, "Select a file", , False)
''//Show the file open dialog to allow user to select a CSV file

If vPath = False Then Exit Sub
''//Exit macro if no file selected

Workbooks.OpenText Filename:=vPath, Origin:=xlMSDOS, StartRow:=1 _
    , DataType:=xlDelimited, TextQualifier:=xlDoubleQuote, Comma:=True _
    , FieldInfo:=Array(Array(1, xlTextFormat), Array(2, xlTextFormat), _
    Array(3, xlTextFormat))
''//The fieldinfo array needs to be extended to match your number of columns

Columns.EntireColumn.AutoFit
''//Resize the columns

Sheets(1).Move Before:=wb.Sheets(1)
''//Move the data into the Workbook

End Sub


