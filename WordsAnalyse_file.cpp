#pragma once
#include"WordsAnalyse_file.h"
#include<string>
void WordsAnalyse_file::analyze_file(string file_index){

	char temp;
	//int column;
	row = 0;
	int result=0;
	char a[10];
	string str;
//	int num;
	//"C:/Users\Administrator.PC-201701141453/Documents/Visual Studio 2013/Projects/compile_5_2/compile_5_2/main_WordsAnalysis.cpp";
	//cout << file_index << endl;
	keyfile.load_file(KeyWords_file, signa_file);//将关键词文本读入缓冲区
	ifile.open(file_index);
	if (!ifile.is_open()){
		open_status = false;
		cout << "fail to open file which compiled!" << endl;
		//return false;
	}
	//每次检查一行
	cout << "====================================================" << endl << endl;
	
	cout << "Start excutiving analysis for file... " << endl;
	cout << "--------------------" << endl;
	while (read_line()){
		row++;
		column = 0;
		result =Unknow;
		while (GetCharacter(temp, column))
		{
			//cout <<"column"<< column << endl;
			//cout << "temp:" << temp << endl;
			if (temp == ' ' || temp == '\n' || temp == '\r' || temp == '\t')//简单过滤特殊符
			{
				
				result = Token();//一行内都没有关键字，那么直接检测整句。
				switch (result)
				{
				case Number:content += "n"; break;
				case Keyword:content += "k"; break;
				case Identifier:content += "d"; break;
				default: break;
				}
				result = Unknow;
				column++;
				//break;
			}
			else
			{
				if (isAnnotation(temp))//检查是否为注释行
				{
					content += '/';
					break;
				}	
				if (isString(temp))//检查是否为字符串
				{
					content += 's';
					continue;
				}
					
				if (-1==serch_signs(temp))//检查该字符是否为关键字符
				{
					//cout << "非关键字" << endl;
					Concatebation(temp);
					column++;
				}
				else{
					content += temp;//是关键字，记录分析结果
				}
			}
		}
		//cout << "token:" << token << endl;
		if (!token.empty())
			result = Token();//一行内都没有关键字，那么直接检测整句。
		switch (result)
		{
		case Number:content += "n"; break;
		case Keyword:content += "k"; break;
		case Identifier:content += "d"; break;
		default: break;
		}
		if (!content.empty()){
			if (content[content.length() - 1] == ';')
			result=GraAnalyse.Anlayse_content(content.substr(0, content.length() - 1));//语法分析
			else result=GraAnalyse.Anlayse_content(content);//语法分析
		}
		if (result == 1){//是一条计算表达式
			sprintf(a, "%d", row);
			str = "finding an arithmetic expression in ";
			str += a;
			str+=" row";
			analysis_result_content.push_back(str);
		}
		//cout << "content:" << content << endl;
		content.clear();
		
	}
	//display_analyse_result(num_value);//显示词法分析结果
	display_analyse_content(analysis_result_content);//显示全部分析结果
	analysis_result_content.clear();
	ifile.close();
}
bool WordsAnalyse_file::GetCharacter(char &ch, int i)//从行缓冲区中获取一个字符
{
	//sizeof(compiled_line) / sizeof(compiled_line[0])
	if (compiled_line.length()> i)
	{
		ch=compiled_line[i];
		return true;
	}
	else return false;
}

int WordsAnalyse_file::serch_signs(char ch){
	vector<char>::iterator it;
	//string temp;
	for (it = keyfile.sign_value.begin(); it != keyfile.sign_value.end(); it++)
	{
		//cout << "1111" << (*it).length() <<" "<< (*it)[0] << endl;
		if (*it== ch)//关键字为一个
		{
			if (Token() == Number)//处理之前的字符串
			{
				content += "n";
			}
			v_n.column = column + 1;
			v_n.row = row;
			v_n.number = Sign;
			v_n.value = ch;
			v_n.symbol = 'i';
			num_value.push_back(v_n);
			column++;
			//cout << "it[0][0]:" << it[0][0] << endl;
			return Sign;
		}
	}
	return -1;
}
//将当前读到的非关键词单个字符与之前的拼接起来
bool WordsAnalyse_file::Concatebation(char ch){
	token+=ch;
	return true;
}
int WordsAnalyse_file::Token()//处理组成的字符串，判断其值
{
	//cout << "token：" <<token<< endl;
	vector<string>::iterator it;
	//string temp;
	if (token.empty())
		return false;
	for (it = keyfile.KeyWords_value.begin(); it != keyfile.KeyWords_value.end(); it++)
	{
		if (token.compare(*it)==0)//检查字符串是否为关键字
		{
			v_n.column = column + 1;
			v_n.row = row;
			v_n.number = keyfile.KeyWords_num[it - keyfile.KeyWords_value.begin()];
			//cout << "v_n.numbe  " << v_n.number << endl;
			v_n.value = token;
			v_n.symbol = 'k';
			num_value.push_back(v_n);
			token.erase();//清空缓冲区域
			return Keyword;
		}
		}

	if (!token.empty())//缓冲区内不为空时
	{
		if (isNumber())//检查是否为数字；
		{
			v_n.column = column + 1;
			v_n.row = row;
			v_n.number = Number;//数字
			v_n.value = token;
			v_n.symbol = 'n';
			num_value.push_back(v_n);
			token.erase();//清空缓冲区域
			return Number;
		}
		if (isIdentifier())
		{
			v_n.column = column + 1;
			v_n.row = row;
			v_n.number = Identifier;//标识符
			v_n.value = token;
			v_n.symbol = 'd';//可以在此判断标识符的类型，（整型，STRING类型等等）；
			num_value.push_back(v_n);
			token.erase();//清空缓冲区域
			return Identifier;
		}
		else{
			//该字符不是关键字
			//if (token[0] == '_' || token = '_')
			v_n.column = column + 1;
			v_n.row = row;
			v_n.number = Unknow;//未知符号串
			v_n.value = token;
			v_n.symbol = 'u';
			num_value.push_back(v_n);
		}
	
		token.erase();//清空缓冲区域
	}
	
	return Unknow;
}
bool WordsAnalyse_file::read_line(){
	if (getline(ifile, compiled_line))//每次读取文件一行
	{
		//cout << "compiled_line  " << compiled_line<< endl;
		return true;
	}
	else {
		cout << "--------------------" << endl;
		cout << "end up analysing..." << endl;
		return false;
	}
}
bool  WordsAnalyse_file::zimu(char c)//判断是否为字母
{
	if (c >= 'a'&&c <= 'z' || c >= 'A'&&c <= 'Z')
		return true;
	else
		return false;
}
bool  WordsAnalyse_file::shuzi(char c)//判断是否为数字
{
	if (c >= '0'&&c <= '9')
		return true;
	else
		return false;
}
bool  WordsAnalyse_file::isAnnotation(char ch){//判断是否为注释行
	if (ch == '/')//判断是否是注释行
	{
		GetCharacter(ch, column + 1);
		if (ch == '/')//是注释行
		{
			v_n.column = column + 1;
			v_n.row = row;
			v_n.number = Annotation;
			v_n.value = compiled_line.substr(column);
			v_n.symbol = '/';
			num_value.push_back(v_n);
			return true;
		}
		else return false;

	}
	return false;
}
bool  WordsAnalyse_file::isString(char ch){//判断是否为字符串
	//string str;
	int temp;
	if (ch == '"')//判断是否是字符
	{
		//cout << "1" << endl;
		//str = compiled_line.substr(column+1);
		temp = compiled_line.find('"', column + 1);//从当前位置往后找"字符
		
		if (std::string::npos == temp)
			return false;
		else {
			//cout << temp << endl;
			//str.erase();
			//cout << compiled_line.substr(column, temp) << endl;
			v_n.column = column + 1;
			v_n.row = row;
			v_n.number =String;
			v_n.value = compiled_line.substr(column, temp-column+1);
			v_n.symbol = 's';
			num_value.push_back(v_n);
			column = temp + 1;//读取行变为字符串之后
			return true;
		}	
	}
	return false;
}
bool WordsAnalyse_file::isNumber()//判断是否为数字
{
	int fag=0;//记录小数点个数
	if (shuzi(token[0]) && token[token.length()] != '.')//第一位是数字，并且最后一位不为‘.’（小数点）
	{
		//cout << "3333" << token << endl;
		for (int i = 1; i < token.length() - 1; i++)
		{
			//cout << "3333" << token[i] << endl;
			if (token[i] == '.')
				fag++;
			if (shuzi(token[i]) || token[i] == '.' || fag <= 1)
				continue;
			else return false;
		}
		return true;
	}
	else return false;
}
bool WordsAnalyse_file::isIdentifier()//判断token是符合标识符
{
	if ((zimu(token[0]) || token[0]=='_')&&token.length()<=32)//第一位是字母或者_（下划线）,长度为32以内
	{		
		for (int i = 1; i < token.length() - 1; i++)
		{
			//cout << "3333" << token[i] << endl;
			//if (token[i] == '.')
			if (zimu(token[i]) || token[i] == '_'||shuzi(token[i]))
				continue;
			else return false;
		}
		return true;
	}
	else return false;
}
void WordsAnalyse_file::display_analyse_result(vector<word_value> num_value){
	cout << "=============================以下为分析结果===========================================" << endl;
	cout << "行数  ---   单词种别编码  --------------   单词表示符号   ---------------   单词自身的值" << endl;
	vector<word_value>::iterator it;
	for (it = num_value.begin(); it != num_value.end(); it++)
	//	cout << " " << it->row << "   " << it->number <<"                         " << it->symbol << "                         " << it->value << endl;
	cout << std::left << setw(16) << it->row << std::left << setw(32) << it->number << std::left << setw(30) << it->symbol << std::left<< it->value << endl;
}
//bool compiled_file::open_file(string file_index){
//	fstream f 
//}
void WordsAnalyse_file::display_analyse_content(vector<string> analysis_result_content){
	
	cout << "--------------------" << endl;
	cout << "The result of analysis is:" << endl;
	vector<string>::iterator it;
	for (it = analysis_result_content.begin(); it != analysis_result_content.end(); it++)
		//	cout << " " << it->row << "   " << it->number <<"                         " << it->symbol << "                         " << it->value << endl;
		cout << *it<< endl;
}