#pragma once
#include<iostream>
#include<fstream>
#include <string>
#include<stack>
#include<vector>
#include"loading_file.h"
#define KeyWords_fs "./KeyWords.txt"
#define signa_fs "./sign.txt"
using namespace std;
class GrammarAnalyse
{
public:
	GrammarAnalyse();
	int Anlayse_content(string content);
	string get_status_Actionfrom_List(int line,char ch);
	string GrammarAnalyse::get_status_Gotofrom_List(int line, char ch);
	string GrammarAnalyse::get_returnConten_from_grammar_exp(int line);//返回字符串的第一个字符是规约的非终结符，从第二个字符开始，都是->右边部分
	~GrammarAnalyse();
private:
	loading_file grammar_open;//加载预定义的关键字表文件
	ifstream ifile;
//	enum Type {
//		Keyword, //关键词
//		Number = 103,  //数字（纯数字）
//		Identifier = 104, //标识符
//		Sign = 100, //符号
//		Annotation = 102, //注释
//		String = 101, //字符串
//
//		Unknow = 999, //未知的
//	};
//	bool read_line();//每次只读一行
//	bool GetCharacter(char &ch, int i);//从行缓冲区中获取一个字符
//	bool Concatebation(char ch);//将当前读到的非关键词单个字符与之前的拼接起来
//	int Token();//处理组成的字符串，判断其值，若为关键字，return true，否则return false
//	int  serch_signs(char ch);
//	void display_analyse_result(vector<word_value> num_value);
//
//	bool zimu(char c);//判断是否为字母
//	bool shuzi(char c);//判断是否为数字
//	bool isAnnotation(char ch);//判断是否为注释行
//	bool isString(char ch);//判断是否为普通字符串
//	bool isNumber();//判断token是否为数字
//	bool isIdentifier();//判断token是符合标识符
//	//bool open_file(string file_index);
//private:
//	loading_file keyfile;
//	string compiled_line;
//	ifstream ifile;
//	//ofstream *ofile;
//	string token;
//	int row;//当前读到的行数
//	int column;//当前读取的列数
//	bool open_status = true;//文件正常打开
//	vector<word_value> num_value;
//	word_value v_n;
};
