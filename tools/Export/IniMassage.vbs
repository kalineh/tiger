if WScript.Arguments.Count = 0 then
    WScript.Echo "Missing INI file path parameter!"
end if

Set objFS = CreateObject("Scripting.FileSystemObject")
strFile = WScript.Arguments(0)
Set objFile = objFS.OpenTextFile(strFile)

Do Until objFile.AtEndOfStream
    strLine = objFile.ReadLine
    If InStr(strLine,"ShowConsole = 1")> 0 Then
    	strLine = Replace(strLine,"ShowConsole = 1","ShowConsole = 0")
	ElseIf InStr(strLine,"ShowAnalytics = 1")> 0 Then
    	strLine = Replace(strLine,"ShowAnalytics = 1","ShowAnalytics = 0")
	ElseIf InStr(strLine,"GLCheckError = 1")> 0 Then
    	strLine = Replace(strLine,"GLCheckError = 1","GLCheckError = 0")
    End If 
    WScript.Echo strLine
Loop