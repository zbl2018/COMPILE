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
	char symbol;//��ʾ����
	int row;
	int column;
};

class WordsAnalyse_file
{
public:
	void analyze_file(string file_index);
	//~WordsAnalyse_file();
	enum Type {
		Keyword=105, //�ؼ���
		Number=103,  //���֣������֣�
		Identifier=104, //��ʶ��
		Sign=100, //����
		Annotation=102, //ע��
		String=101, //�ַ���

		Unknow=999, //δ֪��
	};
	bool read_line();//ÿ��ֻ��һ��
	bool GetCharacter(char &ch,int i);//���л������л�ȡһ���ַ�
	bool Concatebation(char ch);//����ǰ�����ķǹؼ��ʵ����ַ���֮ǰ��ƴ������
	int Token();//������ɵ��ַ������ж���ֵ����Ϊ�ؼ��֣�return true������return false
	int  serch_signs(char ch);
	void display_analyse_result(vector<word_value> num_value);
	void WordsAnalyse_file::display_analyse_content(vector<string> analysis_result_content);

	bool zimu(char c);//�ж��Ƿ�Ϊ��ĸ
	bool shuzi(char c);//�ж��Ƿ�Ϊ����
	bool isAnnotation(char ch);//�ж��Ƿ�Ϊע����
	bool isString(char ch);//�ж��Ƿ�Ϊ��ͨ�ַ���
	bool isNumber();//�ж�token�Ƿ�Ϊ����
	bool isIdentifier();//�ж�token�Ƿ��ϱ�ʶ��
	//bool open_file(string file_index);
private:
	loading_file keyfile;
	string compiled_line;
	ifstream ifile;
	//ofstream *ofile;
	string token;
	int row;//��ǰ����������
	int column;//��ǰ��ȡ������
	bool open_status=true;//�ļ�������
	vector<word_value> num_value;
	word_value v_n;
	string content;//��¼���ӷ����Ľ����ûһ�䣩��
	GrammarAnalyse GraAnalyse;
	vector<string> analysis_result_content;
};
