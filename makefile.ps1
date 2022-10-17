Set-Location (Split-Path $MyInvocation.MyCommand.Definition)
$codeList = Get-ChildItem | Where-Object {$_.Extension -match "(^.c$)|(^.cpp$)|(^.hpp$)|(^.cc$)" -and $_.BaseName -notmatch "^test.*"}

"正在编译以下文件："+ ($codeList -join ' ')
g++.exe -fexec-charset=GBK -o BDmToXml.exe $codeList -I./include -L./lib -lprotobuf -Os -s
if($LASTEXITCODE -eq 0) {
    "编译成功！"
}