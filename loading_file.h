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
struct gra_expansion{//存储拓广文法结构
	vector<string> grammar;
	vector<int> father;//记录由哪个父状态转换而来
	vector<char> type;//记录由哪个操作转换而来
};
struct AnalyseList{//分析表结构
	vector<char> ACTION;
	vector<char> GOTO;
	vector<vector<string>> status;
};
struct Set{//集合结构
	char sign;
	vector<char> val;
};
struct follow_complement{//用来记录创建follow时产生的A->aB,follow(A)属于follow(B)的情况
	vector<char> val_start;
	vector<char> val_end;
};
class loading_file
{
public:
	//===================加载关键字文件=============
	bool load_file(string KeyWords_fs, string signa_fs);
	//bool ~load_file();
	vector<string> KeyWords_value;
	vector<int> KeyWords_num;
	string read_buf;
	vector<char> sign_value;
	//===================加载语法文件================
	AnalyseList List;//分析表
	vector<string> grammar_exp;//存储拓广文法(备份，.已经被擦除)
	bool load_grammar_file(string grammar_fs);
private:
	ifstream ifile;
	vector<gra_expansion> grammar_dfa;//存储DFA
	vector<string> grammar;
	vector<char>Vn;//一个项目集的非终结符
	vector<Set> First;
	vector<Set> Follow;

	int CreatGrammarList();
	bool CreatDFA();
	void Mend_grammar();//修改为拓广文法
	vector<char> search_sign(vector<string> gra);//查找->右部的字符种类
	bool GetItem(vector<string> grammar, char ch, int father/*由哪个父节点传来*/);//获得一个规范族
	bool check_item(gra_expansion val, int father, char type);//检查生成的项目集是否是已生成过的,已生成，那么便将父节点的节点数与转换字符标记
	vector<string>check_sign(char ch);//检查进行规范的字符是否在首个规范集->的左边出现
	int search_FatherAndType(int number, char type);
	int search_location_retrun(gra_expansion val, char ch);//检查该项目集是否有规约项,并且该字符在规约非终结符的follow集中，若是则返回规约位置，反之返回-1
	int search_Vt_in_follow(char Vn, char Vt); //检查该字符是否在对应的非终结符follow中
	int GetItemLocation(string val);//获取当前一条句子在原先文法中的位置
	int DisplayDFA();
	void DisplayGrammarList();

	vector<Set> CreateFirst(vector<string> item);//获取整个项目集所有非终结符的FIRST集
	vector<Set> CreateFollow(vector<string> item);//获取整个项目集所有非终结符的Follow集
	Set search_first_sign(vector<string> item, vector<char>val, char ch);//获取一个非终结符的first集
	bool find_sign(vector<char> val, char ch);//判断该字符否是存在该容器中
	vector<char> search_Vn(vector<string> gra);//获取当前项目集所有->左部不同的非终结符
	Set search_child_set(vector<Set> inter_Item, char ch);//查找对应字符的详细集合
	Set loading_file::search_follow_sign(vector<string> item, char ch, follow_complement &comp);//查找->右边非终结符的follow集
	vector<char> search_sign_in_firstSet(char ch);//查找该字符是否是非终结符，并且有first集
	int loading_file::search_FollowSet(vector<Set>follow, char Vn);//查找某个非终结符的follow集,并返回位置
	bool loading_file::eliminating_duplicate(vector<Set> &set_vector);//消除集合里面的所有重复元素
	
};
string UTF8ToGB(const char* str);