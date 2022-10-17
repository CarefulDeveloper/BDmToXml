#include <fstream>
#include <iostream>
#include <string>
#include <Windows.h>
// 根据protobuf文件解析生成的一个头文件
#include "danmaku.pb.h"
// 自己写的一个String处理的常用工具函数头文件
#include "stringutil.h"
// 对可选参数处理的一个头文件
#include "paramlist.h"

using namespace std;
using namespace bilibili::community::service::dm::v1;

DmSegMobileReply PauseProtobufFile(const string &fileName)
{
	DmSegMobileReply danmaku_seg;
	{
		//读取文件
		fstream input(fileName, ios::in | ios::binary);
		if (!input.is_open())
		{
			cerr << "文件" << fileName << "读取失败！" << endl;
			exit(-1);
		}
		if (!danmaku_seg.ParseFromIstream(&input))
		{
			cerr << fileName << "数据解析失败！" << endl;
			exit(-1);
		}
		return danmaku_seg;
	}
}

void ConvertToXML(const DmSegMobileReply &danmaku_seg, char *&fileName, const Params &params)
{
	fstream output(fileName, ios::out | ios::trunc | ios::binary);
	if (!output.is_open())
	{
		cerr << "文件" << fileName << "写入失败！" << endl;
		exit(-1);
	}
	output << "<?xml version=\"1.0\" encoding=\"UTF - 8\"?><i><chatserver>chat.bilibili.com</chatserver><chatid>" << params.chatID << "</chatid><mission>0</mission><maxlimit></maxlimit><state>0</state><real_name>0</real_name><source>k-v</source>";

	for (int i = 0; i < danmaku_seg.elems_size(); i++)
	{
		const DanmakuElem &elem = danmaku_seg.elems(i);

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
		int32_t progress = elem.progress() + params.offset;
		if(progress < 0) progress = 0;
		output << "<d p=\"" << progress / 1000.0 << ","
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

/**
 * argc说明：
 * 传入参数的个数：
 * 其中arg[0]就是BDmToXML.exe
 * */
int main(int argc, char *argv[])
{
	// 根据官方的建议，写的这一句话，管你看不看得懂，用就完了。
	GOOGLE_PROTOBUF_VERIFY_VERSION;

	string chatId = "";

	// 非法参数个数时
	if (argc != 3 && argc != 4)
	{
		cerr << "参数非法！" << endl;
		cout << ">>格式：" << endl
			 << "    " << argv[0] << " <输入文件路径名[|输入文件路径名]{n}> <输出文件路径名> [可选参数key=可选参数value[|可选参数key=可选参数value]{n}]" << endl;

		cout << ">>>>可选参数key=value说明：" << endl;
		cout << "    "
			 << "chatID=string[\"\"]:指定string值的数字作为弹幕的chatid" << endl;
		cout << "    "
			 << "offset=int[0]:设置弹幕的发送时间偏移量" << endl;
		cout << ">>示例：" << endl
			 << "    " << argv[0] << " ./input.so|./input2.so output.xml chat_id=1008611|offset=12" << endl;
		cout << endl
			 << "version:1.5" << endl;
		return -1;
	}

	Params params;
	// 如果参数个数为4，即存在可选参数。
	if (argc == 4)
	{
		// 全部未对错误进行处理，默认忽略错误值。
		Check_Parm(argv[3]).dataOrDefault(params);
	}

	// 合并所有输入的文件
	DmSegMobileReply danmaku_seg;
	vector<string> inputFileList = String_Split(string(argv[1]), "|");
	for (int index; index < inputFileList.size(); index++)
	{
		danmaku_seg.MergeFrom(PauseProtobufFile(inputFileList[index]));
	}

	ConvertToXML(danmaku_seg, argv[2], params);

	//删除所有由libprotobuf分配的全局对象
	google::protobuf::ShutdownProtobufLibrary();

	return 0;
}