# BDmToXml
将bilibili的protobuf序列化文件转换为B站的XML弹幕文件的windows端命令行工具
# 程序说明
1. 使用方法可以通过命令行输入`BDmToXml.exe`后查看。
2. 程序发生错误会有提示，并且程序返回值为-1，正常则无输出，返回值为0。
3. 对于获取B站protobuf序列化的文件的接口可以从[这里](https://github.com/SocialSisterYi/bilibili-API-collect/blob/master/danmaku/danmaku_proto.md)查看。
4. 本程序只实现了基本的转化功能，初衷用于Windows Bat语言或Power Shell语言调用。
