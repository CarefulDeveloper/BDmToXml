Set-Location (Split-Path $MyInvocation.MyCommand.Definition)
$codeList = Get-ChildItem | Where-Object {$_.Extension -eq ".cc" -or $_.Extension -eq ".c" -or $_.Extension -eq ".cpp" -and $_.BaseName -notmatch "test.*"}

"正在编译以下文件："+ ($codeList -join ' ')
g++.exe -fexec-charset=GBK -o BDmToXml.exe $codeList -I. -L. -lprotobuf -Os -s
if($LASTEXITCODE -eq 0) {
    .\BDmToXml.exe .\seg.so .\res.txt
}