#include"GrammarAnalyse.h"
#include<string>
GrammarAnalyse::GrammarAnalyse(){
	grammar_open.load_grammar_file("./grammar.txt");
	cout << "--------------------" << endl;
	cout << "The grammar is loaded successfully! " << endl;
}
int GrammarAnalyse::Anlayse_content(string content){
	string result;
	int loct;
	string temp;
	stack<char>input_content;
	stack<int> status_stack;//状态栈
	string char_stack;//符号栈
	content += '#';//输入串尾部添加#号
	char_stack += '#';
	status_stack.push(0);//栈底为0

	for (int i = content.length() - 1; i >= 0; i--)//将输入的句子放入栈中
	{
		input_content.push(content[i]);
		//cout << "kk:" << content[i] << endl;
	}

	//=============开始分析输入的句子是否符合文法==============
	cout << "The result of analysis is :";
	while (input_content.size() >=1)
	{
		temp.clear();
		result = get_status_Actionfrom_List(status_stack.top(), input_content.top());
		//cout << result[0] << endl;
		//cout << input_content.size() << endl;
		switch (result[0])
		{
		case 'e':
		{
					if (result == "error")
					{
						cout << result << endl;
						//cout << "该句子不符合当前文法！查询Action表状态时出错！" << endl;
						return 0;
					}
					else{
						//cout << "分析表有误，状态项不止r,s,error这三种" << endl;
						return -1;
					}
		}break;
		case 's':
		{
				cout << result << "-";
				char_stack += input_content.top();
				input_content.pop();
				status_stack.push(atoi(result.substr(1).data()));//
				//cout << "s_end" << endl;
		}break;
		case 'r':
		{		
				cout <<result << "-";
			//	cout << "temp:" << atoi(result.substr(1).data()) << endl;
				temp = get_returnConten_from_grammar_exp(atoi(result.substr(1).data()));//获得对应规约串（）
				//cout << "temp：" << temp.substr(1) << ":" << char_stack << endl;
				loct = char_stack.find(temp.substr(1));
			   // cout << "loct:"<<loct << endl;
				char_stack.erase(loct, temp.substr(1).length());//将规约串删除
				char_stack.insert(loct, temp.substr(0,1));
				//char_stack += temp[0];//加入规约后的非终结符
				for (int i = 0; i < temp.substr(1).length();i++)
					status_stack.pop();//弹出当前规约串长度个状态
				//cout << "r1" << endl;
				result = get_status_Gotofrom_List(status_stack.top(), temp[0]);//获取Goto表对应状态
				//cout << "goto:" << atoi(result.data()) << endl;
				if (result != "error")
				{
					//cout << "goto:" << result << endl;
					status_stack.push(atoi(result.data()));
				}
				else{
					cout << result << endl;
					//cout << "该句子不符合当前文法！查询GOTO表状态时出错！" << endl;
					return 0;//查表出错
				}
				
					
		}break;
		case 'a':
		{
				if (result == "acc")
				{
					cout << result << endl;
					//cout << "该句子符合当前文法！" << endl;
					return 1;//成功接受
				}
				else{
					//cout << "分析表有误，状态项不止r,s,error这三种" << endl;
					return -1;
				}
		}break;
		default:return -2;//未知错误
		}
	}
	
	return 1;
}
string GrammarAnalyse::get_status_Actionfrom_List(int line, char ch){
	vector<char>::iterator action_it;
	//vector<char>::iterator goto_it;
	vector<vector<string>>::iterator status_it=grammar_open.List.status.begin()+line;
	for (action_it = grammar_open.List.ACTION.begin(); action_it != grammar_open.List.ACTION.end(); action_it++)
	{
		if (*action_it == ch)
		{
			return *(status_it->begin() + (action_it - grammar_open.List.ACTION.begin()));
		}
	}
	return "error";
	//grammar_open.List;
}
string GrammarAnalyse::get_status_Gotofrom_List(int line, char ch){
	vector<char>::iterator action_it;
	vector<char>::iterator goto_it;
	vector<vector<string>>::iterator status_it = grammar_open.List.status.begin() + line;
	for (goto_it = grammar_open.List.GOTO.begin(); goto_it != grammar_open.List.GOTO.end(); goto_it++)
	{
		if (*goto_it == ch)
		{
			return *(status_it->begin() + grammar_open.List.ACTION.size() + (goto_it - grammar_open.List.GOTO.begin()));
		}
	}
	return "error";
	//grammar_open.List;
}
//返回字符串的第一个字符是规约的非终结符，从第二个字符开始，都是->右边部分
string GrammarAnalyse::get_returnConten_from_grammar_exp(int line){
	string temp;
	int loct;
	//cout << line << endl;
	//cout << *(grammar_open.grammar_exp.begin() )<< endl;
	loct=(grammar_open.grammar_exp.begin() + line)->find("->");
	//cout << "->" << loct << endl;
	temp = (grammar_open.grammar_exp.begin() + line)->substr(0, loct) + (grammar_open.grammar_exp.begin() + line)->substr(loct+2);
	return temp;
}
GrammarAnalyse::~GrammarAnalyse(){
	ifile.close();
}
