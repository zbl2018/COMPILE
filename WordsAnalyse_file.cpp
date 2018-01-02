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
	keyfile.load_file(KeyWords_file, signa_file);//���ؼ����ı����뻺����
	ifile.open(file_index);
	if (!ifile.is_open()){
		open_status = false;
		cout << "fail to open file which compiled!" << endl;
		//return false;
	}
	//ÿ�μ��һ��
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
			if (temp == ' ' || temp == '\n' || temp == '\r' || temp == '\t')//�򵥹��������
			{
				
				result = Token();//һ���ڶ�û�йؼ��֣���ôֱ�Ӽ�����䡣
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
				if (isAnnotation(temp))//����Ƿ�Ϊע����
				{
					content += '/';
					break;
				}	
				if (isString(temp))//����Ƿ�Ϊ�ַ���
				{
					content += 's';
					continue;
				}
					
				if (-1==serch_signs(temp))//�����ַ��Ƿ�Ϊ�ؼ��ַ�
				{
					//cout << "�ǹؼ���" << endl;
					Concatebation(temp);
					column++;
				}
				else{
					content += temp;//�ǹؼ��֣���¼�������
				}
			}
		}
		//cout << "token:" << token << endl;
		if (!token.empty())
			result = Token();//һ���ڶ�û�йؼ��֣���ôֱ�Ӽ�����䡣
		switch (result)
		{
		case Number:content += "n"; break;
		case Keyword:content += "k"; break;
		case Identifier:content += "d"; break;
		default: break;
		}
		if (!content.empty()){
			if (content[content.length() - 1] == ';')
			result=GraAnalyse.Anlayse_content(content.substr(0, content.length() - 1));//�﷨����
			else result=GraAnalyse.Anlayse_content(content);//�﷨����
		}
		if (result == 1){//��һ��������ʽ
			sprintf(a, "%d", row);
			str = "finding an arithmetic expression in ";
			str += a;
			str+=" row";
			analysis_result_content.push_back(str);
		}
		//cout << "content:" << content << endl;
		content.clear();
		
	}
	//display_analyse_result(num_value);//��ʾ�ʷ��������
	display_analyse_content(analysis_result_content);//��ʾȫ���������
	analysis_result_content.clear();
	ifile.close();
}
bool WordsAnalyse_file::GetCharacter(char &ch, int i)//���л������л�ȡһ���ַ�
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
		if (*it== ch)//�ؼ���Ϊһ��
		{
			if (Token() == Number)//����֮ǰ���ַ���
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
//����ǰ�����ķǹؼ��ʵ����ַ���֮ǰ��ƴ������
bool WordsAnalyse_file::Concatebation(char ch){
	token+=ch;
	return true;
}
int WordsAnalyse_file::Token()//������ɵ��ַ������ж���ֵ
{
	//cout << "token��" <<token<< endl;
	vector<string>::iterator it;
	//string temp;
	if (token.empty())
		return false;
	for (it = keyfile.KeyWords_value.begin(); it != keyfile.KeyWords_value.end(); it++)
	{
		if (token.compare(*it)==0)//����ַ����Ƿ�Ϊ�ؼ���
		{
			v_n.column = column + 1;
			v_n.row = row;
			v_n.number = keyfile.KeyWords_num[it - keyfile.KeyWords_value.begin()];
			//cout << "v_n.numbe  " << v_n.number << endl;
			v_n.value = token;
			v_n.symbol = 'k';
			num_value.push_back(v_n);
			token.erase();//��ջ�������
			return Keyword;
		}
		}

	if (!token.empty())//�������ڲ�Ϊ��ʱ
	{
		if (isNumber())//����Ƿ�Ϊ���֣�
		{
			v_n.column = column + 1;
			v_n.row = row;
			v_n.number = Number;//����
			v_n.value = token;
			v_n.symbol = 'n';
			num_value.push_back(v_n);
			token.erase();//��ջ�������
			return Number;
		}
		if (isIdentifier())
		{
			v_n.column = column + 1;
			v_n.row = row;
			v_n.number = Identifier;//��ʶ��
			v_n.value = token;
			v_n.symbol = 'd';//�����ڴ��жϱ�ʶ�������ͣ������ͣ�STRING���͵ȵȣ���
			num_value.push_back(v_n);
			token.erase();//��ջ�������
			return Identifier;
		}
		else{
			//���ַ����ǹؼ���
			//if (token[0] == '_' || token = '_')
			v_n.column = column + 1;
			v_n.row = row;
			v_n.number = Unknow;//δ֪���Ŵ�
			v_n.value = token;
			v_n.symbol = 'u';
			num_value.push_back(v_n);
		}
	
		token.erase();//��ջ�������
	}
	
	return Unknow;
}
bool WordsAnalyse_file::read_line(){
	if (getline(ifile, compiled_line))//ÿ�ζ�ȡ�ļ�һ��
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
bool  WordsAnalyse_file::zimu(char c)//�ж��Ƿ�Ϊ��ĸ
{
	if (c >= 'a'&&c <= 'z' || c >= 'A'&&c <= 'Z')
		return true;
	else
		return false;
}
bool  WordsAnalyse_file::shuzi(char c)//�ж��Ƿ�Ϊ����
{
	if (c >= '0'&&c <= '9')
		return true;
	else
		return false;
}
bool  WordsAnalyse_file::isAnnotation(char ch){//�ж��Ƿ�Ϊע����
	if (ch == '/')//�ж��Ƿ���ע����
	{
		GetCharacter(ch, column + 1);
		if (ch == '/')//��ע����
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
bool  WordsAnalyse_file::isString(char ch){//�ж��Ƿ�Ϊ�ַ���
	//string str;
	int temp;
	if (ch == '"')//�ж��Ƿ����ַ�
	{
		//cout << "1" << endl;
		//str = compiled_line.substr(column+1);
		temp = compiled_line.find('"', column + 1);//�ӵ�ǰλ��������"�ַ�
		
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
			column = temp + 1;//��ȡ�б�Ϊ�ַ���֮��
			return true;
		}	
	}
	return false;
}
bool WordsAnalyse_file::isNumber()//�ж��Ƿ�Ϊ����
{
	int fag=0;//��¼С�������
	if (shuzi(token[0]) && token[token.length()] != '.')//��һλ�����֣��������һλ��Ϊ��.����С���㣩
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
bool WordsAnalyse_file::isIdentifier()//�ж�token�Ƿ��ϱ�ʶ��
{
	if ((zimu(token[0]) || token[0]=='_')&&token.length()<=32)//��һλ����ĸ����_���»��ߣ�,����Ϊ32����
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
	cout << "=============================����Ϊ�������===========================================" << endl;
	cout << "����  ---   �����ֱ����  --------------   ���ʱ�ʾ����   ---------------   ���������ֵ" << endl;
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