#pragma once
#include<iostream>
#include<fstream>
#include <string>
#include<vector>
#include<iomanip>
#include"loading_file.h"
#include"GrammarAnalyse.h"

//#define alphabet -1;
//#define figure -2;
//#define alphabet -3;
using namespace std;
struct word_value{
	int number;
	string value;
	char symbol;//表示符号
	int row;
	int column;
};

class WordsAnalyse_file
{
public:
	void analyze_file(string file_index);
	//~WordsAnalyse_file();
	enum Type {
		Keyword=105, //关键词
		Number=103,  //数字（纯数字）
		Identifier=104, //标识符
		Sign=100, //符号
		Annotation=102, //注释
		String=101, //字符串

		Unknow=999, //未知的
	};
	bool read_line();//每次只读一行
	bool GetCharacter(char &ch,int i);//从行缓冲区中获取一个字符
	bool Concatebation(char ch);//将当前读到的非关键词单个字符与之前的拼接起来
	int Token();//处理组成的字符串，判断其值，若为关键字，return true，否则return false
	int  serch_signs(char ch);
	void display_analyse_result(vector<word_value> num_value);
	void WordsAnalyse_file::display_analyse_content(vector<string> analysis_result_content);

	bool zimu(char c);//判断是否为字母
	bool shuzi(char c);//判断是否为数字
	bool isAnnotation(char ch);//判断是否为注释行
	bool isString(char ch);//判断是否为普通字符串
	bool isNumber();//判断token是否为数字
	bool isIdentifier();//判断token是符合标识符
	//bool open_file(string file_index);
private:
	loading_file keyfile;
	string compiled_line;
	ifstream ifile;
	//ofstream *ofile;
	string token;
	int row;//当前读到的行数
	int column;//当前读取的列数
	bool open_status=true;//文件正常打开
	vector<word_value> num_value;
	word_value v_n;
	string content;//记录句子分析的结果（没一句）；
	GrammarAnalyse GraAnalyse;
	vector<string> analysis_result_content;
};
