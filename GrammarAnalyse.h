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
	string GrammarAnalyse::get_returnConten_from_grammar_exp(int line);//�����ַ����ĵ�һ���ַ��ǹ�Լ�ķ��ս�����ӵڶ����ַ���ʼ������->�ұ߲���
	~GrammarAnalyse();
private:
	loading_file grammar_open;//����Ԥ����Ĺؼ��ֱ��ļ�
	ifstream ifile;
//	enum Type {
//		Keyword, //�ؼ���
//		Number = 103,  //���֣������֣�
//		Identifier = 104, //��ʶ��
//		Sign = 100, //����
//		Annotation = 102, //ע��
//		String = 101, //�ַ���
//
//		Unknow = 999, //δ֪��
//	};
//	bool read_line();//ÿ��ֻ��һ��
//	bool GetCharacter(char &ch, int i);//���л������л�ȡһ���ַ�
//	bool Concatebation(char ch);//����ǰ�����ķǹؼ��ʵ����ַ���֮ǰ��ƴ������
//	int Token();//������ɵ��ַ������ж���ֵ����Ϊ�ؼ��֣�return true������return false
//	int  serch_signs(char ch);
//	void display_analyse_result(vector<word_value> num_value);
//
//	bool zimu(char c);//�ж��Ƿ�Ϊ��ĸ
//	bool shuzi(char c);//�ж��Ƿ�Ϊ����
//	bool isAnnotation(char ch);//�ж��Ƿ�Ϊע����
//	bool isString(char ch);//�ж��Ƿ�Ϊ��ͨ�ַ���
//	bool isNumber();//�ж�token�Ƿ�Ϊ����
//	bool isIdentifier();//�ж�token�Ƿ��ϱ�ʶ��
//	//bool open_file(string file_index);
//private:
//	loading_file keyfile;
//	string compiled_line;
//	ifstream ifile;
//	//ofstream *ofile;
//	string token;
//	int row;//��ǰ����������
//	int column;//��ǰ��ȡ������
//	bool open_status = true;//�ļ�������
//	vector<word_value> num_value;
//	word_value v_n;
};
