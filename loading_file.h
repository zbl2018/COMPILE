#pragma once
#include<iostream>
#include<fstream>
#include <string>
#include<vector>
#include<algorithm>
#include<iomanip>
#define KeyWords_file "./KeyWords.txt"
#define signa_file "./sign.txt"
#define grammar_file "./grammar.txt"
using namespace std;
struct gra_expansion{//�洢�ع��ķ��ṹ
	vector<string> grammar;
	vector<int> father;//��¼���ĸ���״̬ת������
	vector<char> type;//��¼���ĸ�����ת������
};
struct AnalyseList{//������ṹ
	vector<char> ACTION;
	vector<char> GOTO;
	vector<vector<string>> status;
};
struct Set{//���Ͻṹ
	char sign;
	vector<char> val;
};
struct follow_complement{//������¼����followʱ������A->aB,follow(A)����follow(B)�����
	vector<char> val_start;
	vector<char> val_end;
};
class loading_file
{
public:
	//===================���عؼ����ļ�=============
	bool load_file(string KeyWords_fs, string signa_fs);
	//bool ~load_file();
	vector<string> KeyWords_value;
	vector<int> KeyWords_num;
	string read_buf;
	vector<char> sign_value;
	//===================�����﷨�ļ�================
	AnalyseList List;//������
	vector<string> grammar_exp;//�洢�ع��ķ�(���ݣ�.�Ѿ�������)
	bool load_grammar_file(string grammar_fs);
private:
	ifstream ifile;
	vector<gra_expansion> grammar_dfa;//�洢DFA
	vector<string> grammar;
	vector<char>Vn;//һ����Ŀ���ķ��ս��
	vector<Set> First;
	vector<Set> Follow;

	int CreatGrammarList();
	bool CreatDFA();
	void Mend_grammar();//�޸�Ϊ�ع��ķ�
	vector<char> search_sign(vector<string> gra);//����->�Ҳ����ַ�����
	bool GetItem(vector<string> grammar, char ch, int father/*���ĸ����ڵ㴫��*/);//���һ���淶��
	bool check_item(gra_expansion val, int father, char type);//������ɵ���Ŀ���Ƿ��������ɹ���,�����ɣ���ô�㽫���ڵ�Ľڵ�����ת���ַ����
	vector<string>check_sign(char ch);//�����й淶���ַ��Ƿ����׸��淶��->����߳���
	int search_FatherAndType(int number, char type);
	int search_location_retrun(gra_expansion val, char ch);//������Ŀ���Ƿ��й�Լ��,���Ҹ��ַ��ڹ�Լ���ս����follow���У������򷵻ع�Լλ�ã���֮����-1
	int search_Vt_in_follow(char Vn, char Vt); //�����ַ��Ƿ��ڶ�Ӧ�ķ��ս��follow��
	int GetItemLocation(string val);//��ȡ��ǰһ��������ԭ���ķ��е�λ��
	int DisplayDFA();
	void DisplayGrammarList();

	vector<Set> CreateFirst(vector<string> item);//��ȡ������Ŀ�����з��ս����FIRST��
	vector<Set> CreateFollow(vector<string> item);//��ȡ������Ŀ�����з��ս����Follow��
	Set search_first_sign(vector<string> item, vector<char>val, char ch);//��ȡһ�����ս����first��
	bool find_sign(vector<char> val, char ch);//�жϸ��ַ����Ǵ��ڸ�������
	vector<char> search_Vn(vector<string> gra);//��ȡ��ǰ��Ŀ������->�󲿲�ͬ�ķ��ս��
	Set search_child_set(vector<Set> inter_Item, char ch);//���Ҷ�Ӧ�ַ�����ϸ����
	Set loading_file::search_follow_sign(vector<string> item, char ch, follow_complement &comp);//����->�ұ߷��ս����follow��
	vector<char> search_sign_in_firstSet(char ch);//���Ҹ��ַ��Ƿ��Ƿ��ս����������first��
	int loading_file::search_FollowSet(vector<Set>follow, char Vn);//����ĳ�����ս����follow��,������λ��
	bool loading_file::eliminating_duplicate(vector<Set> &set_vector);//������������������ظ�Ԫ��
	
};
string UTF8ToGB(const char* str);