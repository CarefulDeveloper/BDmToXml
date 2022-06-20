#include <fstream>
#include <iostream>
#include <string.h>
#include <Windows.h>
#include "danmaku.pb.h"
#include "stringutil.h"
using namespace std;
using namespace bilibili::community::service::dm::v1;

DmSegMobileReply PauseProtobufFile(char*& fileName)
{
	DmSegMobileReply danmaku_seg;
	{
		//读取文件
		fstream input(fileName, ios::in | ios::binary);
		if (!input.is_open())
		{
			cerr << "文件" << fileName << "读取失败！"<<endl;
			exit(-1);
		}
		if (!danmaku_seg.ParseFromIstream(&input)) {
			cerr << fileName << "数据解析失败！" << endl;
			exit(-1);
		}
		return danmaku_seg;
	}
}

void ConvertToXML(const DmSegMobileReply& danmaku_seg, char*& fileName, string& chatId) 
{
	fstream output(fileName, ios::out | ios::trunc | ios::binary);
	if (!output.is_open())
	{
		cerr << "文件" << fileName << "写入失败！" <<endl;
		exit(-1);
	}
	output << "<?xml version=\"1.0\" encoding=\"UTF - 8\"?><i><chatserver>chat.bilibili.com</chatserver><chatid>"<<chatId<<"</chatid><mission>0</mission><maxlimit></maxlimit><state>0</state><real_name>0</real_name><source>k-v</source>";

	for (int i = 0; i < danmaku_seg.elems_size(); i++) {
		const DanmakuElem& elem = danmaku_seg.elems(i);
		
		string content = UTF8_To_String(elem.content());
		String_Replace_All(content, "\n", "");
		String_Replace_All(content, "\r", "");
		String_Replace_All(content, "&", "&amp;");
		String_Replace_All(content, "<", "&lt;");
		String_Replace_All(content, ">", "&gt;");
/*
		output << "    <d p=\"" << elem.progress()/1000.0 << ","
			<< elem.mode() << ","
			<< elem.fontsize() << ","
			<< elem.color() << ","
			<< elem.ctime() << ","
			<< elem.pool() << ","
			<< elem.midhash() << ","
			<< elem.idstr() << ","
			<< elem.weight() << "\">"
			<< String_TO_UTF8(content) << "</d>" << endl;
*/
		output << "<d p=\"" << elem.progress()/1000.0 << ","
			<< elem.mode() << ","
			<< elem.fontsize() << ","
			<< elem.color() << ","
			<< elem.ctime() << ","
			<< elem.pool() << ","
			<< elem.midhash() << ","
			<< elem.idstr() << ","
			<< elem.weight() << "\">"
			<< String_TO_UTF8(content) << "</d>";

		/*
		cout << "========"<< endl;
		cout << "弹幕ID: " << elem.id() << endl;
		cout << "  弹幕出现位置: " << elem.progress() << endl;
		cout << "  弹幕类型: " << elem.mode() << endl;
		cout << "  弹幕字号: " << elem.fontsize() << endl;
		cout << "  弹幕颜色: " << elem.color() << endl;
		cout << "  弹幕发送人mid（Hash值）: " << elem.midhash() << endl;
		cout << "  弹幕正文: " << elem.content() << endl;
		cout << "  发送时间: " << elem.ctime() << endl;
		cout << "  弹幕屏蔽权重: " << elem.weight() << endl;
		cout << "  弹幕动作: " << elem.action() << endl;
		cout << "  弹幕池: " << elem.pool() << endl;
		cout << "  弹幕mid: " << elem.idstr() << endl;
		cout << "  弹幕属性位: " << elem.attr() << endl;
		*/
	}
	output << "</i>";
	output.close();
}

int main(int argc, char *argv[])
{
	//根据官方的建议，写的这一句话，管你看不看得懂，用就完了。
	GOOGLE_PROTOBUF_VERIFY_VERSION;

	string chatId = "";

	if (argc == 3) {
		
	} 
	else if(argc == 4) {
		chatId = argv[3];
	} 
	else {
		cerr << "参数非法！" <<endl;
		cout << ">>格式："<< endl << "    " << argv[0] << " <输入文件路径名[|输入文件路径名]{n}> <输出文件路径名> [弹幕cid]" << endl;
		cout << ">>示例："<< endl << "    " << argv[0] << " ./input.so|./input2.so output.xml 1008611" << endl;
		return -1;
	}

	char* context = NULL;
	char* fileName = strtok_s(argv[1], "|", &context);
	
	DmSegMobileReply danmaku_seg;
	while (fileName) {
		danmaku_seg.MergeFrom(PauseProtobufFile(fileName));
		fileName = strtok_s(NULL, "|", &context);
	}

	ConvertToXML(danmaku_seg, argv[2], chatId);

	//删除所有由libprotobuf分配的全局对象
	google::protobuf::ShutdownProtobufLibrary();

	return 0;
}