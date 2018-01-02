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
	stack<int> status_stack;//״̬ջ
	string char_stack;//����ջ
	content += '#';//���봮β�����#��
	char_stack += '#';
	status_stack.push(0);//ջ��Ϊ0

	for (int i = content.length() - 1; i >= 0; i--)//������ľ��ӷ���ջ��
	{
		input_content.push(content[i]);
		//cout << "kk:" << content[i] << endl;
	}

	//=============��ʼ��������ľ����Ƿ�����ķ�==============
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
						//cout << "�þ��Ӳ����ϵ�ǰ�ķ�����ѯAction��״̬ʱ����" << endl;
						return 0;
					}
					else{
						//cout << "����������״̬�ֹr,s,error������" << endl;
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
				temp = get_returnConten_from_grammar_exp(atoi(result.substr(1).data()));//��ö�Ӧ��Լ������
				//cout << "temp��" << temp.substr(1) << ":" << char_stack << endl;
				loct = char_stack.find(temp.substr(1));
			   // cout << "loct:"<<loct << endl;
				char_stack.erase(loct, temp.substr(1).length());//����Լ��ɾ��
				char_stack.insert(loct, temp.substr(0,1));
				//char_stack += temp[0];//�����Լ��ķ��ս��
				for (int i = 0; i < temp.substr(1).length();i++)
					status_stack.pop();//������ǰ��Լ�����ȸ�״̬
				//cout << "r1" << endl;
				result = get_status_Gotofrom_List(status_stack.top(), temp[0]);//��ȡGoto���Ӧ״̬
				//cout << "goto:" << atoi(result.data()) << endl;
				if (result != "error")
				{
					//cout << "goto:" << result << endl;
					status_stack.push(atoi(result.data()));
				}
				else{
					cout << result << endl;
					//cout << "�þ��Ӳ����ϵ�ǰ�ķ�����ѯGOTO��״̬ʱ����" << endl;
					return 0;//������
				}
				
					
		}break;
		case 'a':
		{
				if (result == "acc")
				{
					cout << result << endl;
					//cout << "�þ��ӷ��ϵ�ǰ�ķ���" << endl;
					return 1;//�ɹ�����
				}
				else{
					//cout << "����������״̬�ֹr,s,error������" << endl;
					return -1;
				}
		}break;
		default:return -2;//δ֪����
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
//�����ַ����ĵ�һ���ַ��ǹ�Լ�ķ��ս�����ӵڶ����ַ���ʼ������->�ұ߲���
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
