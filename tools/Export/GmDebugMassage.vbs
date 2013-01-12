if WScript.Arguments.Count = 0 then
    WScript.Echo "Missing GM debug file path parameter!"
end if

Set objFS = CreateObject("Scripting.FileSystemObject")
strFile = WScript.Arguments(0)
Set objFile = objFS.OpenTextFile(strFile)

Do Until objFile.AtEndOfStream
    strLine = objFile.ReadLine
    If InStr(strLine,"global g_debug = true;")> 0 Then
    	strLine = Replace(strLine,"global g_debug = true;","global g_debug = false;")
    End If 
    WScript.Echo strLine
Loop