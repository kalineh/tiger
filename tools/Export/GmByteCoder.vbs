Wscript.Echo
Wscript.Echo "Generating to GM byte code..." 

if WScript.Arguments.Count = 0 then
    WScript.Echo "Missing directory parameters!"
end if

' All files in "common\gm\" folder
objStartFolder=WScript.Arguments(0)
Wscript.Echo "Converting GM folder: " + objStartFolder
ConvertFolder objStartFolder

Sub ConvertFolder(Folder)
	Set objFSO = CreateObject("Scripting.FileSystemObject")
	Set objFolder = objFSO.GetFolder(Folder)
	Set colFiles = objFolder.Files
	For Each objFile in colFiles
		MakeByteCode objFolder.Path + "\" + objFile.Name
	Next
	ConvertSubFolders objFSO.GetFolder(Folder)
End Sub

' Converts subfolder items
Sub ConvertSubFolders(Folder)
	Set objFSO = CreateObject("Scripting.FileSystemObject")
    For Each Subfolder in Folder.SubFolders
        Set objFolder = objFSO.GetFolder(Subfolder.Path)
        Set colFiles = objFolder.Files
        For Each objFile in colFiles
			MakeByteCode Subfolder.Path + "\" + objFile.Name
        Next
        ConvertSubFolders Subfolder
    Next
End Sub

' Overwrite a file by its bytecoded version
Sub MakeByteCode(file)
	Set wshShell = WScript.CreateObject ("WSCript.shell")
	Dim outputFile, execString
	outputFile = file + "o"
	execString = "tools\Export\gmByteCoder.exe " + file + " " + file
	wshshell.run execString, 6 , true
	Wscript.Echo "-> Converted: " + file
End Sub