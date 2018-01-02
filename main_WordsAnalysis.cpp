#include<iostream>
#include<fstream>
#include"loading_file.h"
#include"WordsAnalyse_file.h"
#include"GrammarAnalyse.h"
using namespace std;
int main(){
//	int line;//读取到的被编译文件的行数
	int a = 999;
	double b = 0.11;
	//1+1;
	//int + double
	1 - 1 * 6 - 9 + 100 / 10000 * 12;
	//string compile_content;
	string index;
	//GrammarAnalyse GraAnalyse;
	//cout << "===============================简单的语法分析器===================================" << endl;
	//cout << "please input your content to analyse：";
	//while (cin >> index)
	//{
	//	GraAnalyse.Anlayse_content(index);//语法分析
	//	cout << "please input your content to analyse：";
	//}
	WordsAnalyse_file analyse;
	1 - 1 * 6 - 9 + 100 / 10000 * 12;
	cout << "===================================THE ANALYZER===================================" << endl;
	cout << "Please input the index of file,like: d:\\compiler.cpp!" << endl << endl;;

	cout << "Please input the index of file  to analyse :" << endl<< "->";
	while (getline(cin, index)){
		analyse.analyze_file(index);
		//analyse.a(index);

		cout << "--------------------" << endl;
		cout << "Please again input the index of file  to analyse :" << endl << "->";
	}
	1 - 1 * 6 - 9 + 100 / 10000 * 12;
	//compiled_file subfile(index);	
	//fstream complied_file(index);
	//int =1 - 1 * 6 - 9 + 100 / 10000 * 12;
	//subfile.open_file(index);
	8 + 1 / 9 + 1 - 0 - 0 - 0 - 0 - 0 - 0 - 0 + 1 * 1 / 6;
}
