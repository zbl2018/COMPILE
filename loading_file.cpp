#include"loading_file.h"
#include <Windows.h>
bool loading_file::load_file(string KeyWords_fs, string signa_fs)
{
	//int line;
	ifile.open(KeyWords_fs);//�򿪹ؼ����ı�
	if (!ifile.is_open())
		cout << "Fail to open KeyWords file.Please  check your index." << endl;
	cout << endl << "============================================" << endl;
	cout << "��keywords�ɹ���" << endl;
	if(getline(ifile, read_buf))//��һ�в���
	while (getline(ifile, read_buf)){//��ȡ�ļ��ڵ����ݣ�ÿ�ζ�ȡһ�У��ӵڶ��ж���
		int temp = read_buf.find(",");
		if (std::string::npos == temp)
		{
			cout << "�ؼ����ļ���" << KeyWords_value.size() + 2 << "�У�ֵ�����ķָ�������','(Ӣ�Ķ���)" << endl;
		}
		else { 
			
			KeyWords_num.push_back(atoi(read_buf.substr(0, temp).data()));//ת��������
			KeyWords_value.push_back(read_buf.substr(temp + 1));
			//cout << KeyWords_num[0] <<"  "<< KeyWords_value[0]<< endl;
		}
		
		//if (ifile.eof())//�ļ�����ʱ,������ȡ����
			//break;
	}
	ifile.close();
	cout << "�Ѷ�ȡȫ���ؼ���..." << endl;

	ifile.open(signa_fs);
	if (!ifile.is_open())
		cout << "Fail to open index file.Please  check your index." << endl;
	//string value;
	char interval;
	if (getline(ifile, read_buf))//��һ�в���
	while (ifile >> interval)//��ȡ�ַ���
		sign_value.push_back(interval);
	//vector<char>::iterator it;
	//string temp;
	//for (it = sign_value.begin(); it != sign_value.end(); it++)
	//{
	//	cout << *it << endl;
	//}
	ifile.close();
	return true;
}
bool loading_file::load_grammar_file(string grammar_fs){
	//int a + b;
	8 + 1;
	//dasda + dasjhd;
	//==========================��ȡ�﷨�ļ�=====================
	ifile.open(grammar_fs);
	if (!ifile.is_open())
		cout << "Fail to open index file.Please check your index." << endl;
	//string value;
	cout << "loading grammar..." << endl;
	char interval;
	string temp;
	if (getline(ifile, read_buf))//��һ�в���
	while (ifile >> interval)//��ȡ�﷨�ļ�
	{
		//cout << interval << endl;
		if (interval != ';')
			temp += interval;
		else {
			grammar.push_back(temp);
			temp.erase();
		}
	}
	ifile.close();
	//============================����LR�﷨������==================
	//�޸��ع��ķ�
	Mend_grammar();
	//����DFA
	CreatDFA();
	//DisplayDFA();
	CreatGrammarList();
	DisplayGrammarList();
	return true;
}
//��DFA�в��ҵ�ǰת���ַ���Ӧ�ĸ��ڵ�λ��
int loading_file::search_FatherAndType(int number, char type){
	vector<gra_expansion>::iterator it;
	vector<string>::iterator g;
	vector<int>::iterator i;
	vector<char>::iterator c;
	//string temp;
	for (it = grammar_dfa.begin() + 1/*�ӵڶ�����ʼ����*/; it != grammar_dfa.end(); it++)
	{
		for (i = it->father.begin(), c = it->type.begin(); c != it->type.end() && i != it->father.end(); i++, c++)
		{
			if (*i == number&&*c == type)
			{
				return (it - grammar_dfa.begin());
			}
		}
	}
	return -1;
}
//�����﷨������
int loading_file::CreatGrammarList(){
	vector<char>::iterator cp;
	vector<char> character;//��ȡ�ķ��е��ַ�����
	character=search_sign(grammar_dfa.begin()->grammar);
	//===============����ACTION��GOTO���ͷ==================
	for (cp = character.begin(); cp != character.end(); cp++)
	{
		if (check_sign(*cp).empty())
		{//����ACTION
			List.ACTION.push_back(*cp);
		}
		else{//����GOTO��

			List.GOTO.push_back(*cp);
		}

	}
	List.ACTION.push_back('#');//ACTION��ͷ���һ��#
	//=================����FIRST����FOLLOW==============================
	//vector<string>::iterator it_exp;//�ع��ķ�����
	First=CreateFirst(grammar_exp);
	Follow = CreateFollow(grammar_exp);
	//for (it_exp=grammar_exp..begin())
	//=================����״̬��==============================
		vector<vector<string>>::iterator it;
		vector<char>::iterator action_it;
		vector<char>::iterator goto_it;
		vector<string>temp;
		int number= DisplayDFA();//DFA����Ŀ��
		int loct=-1;
		bool interval=false;//1�����ƽ���0�����ƽ��������й�Լ
		for (int j=0;j<number; j++)
		{
			//���ACTION
			for (action_it = List.ACTION.begin(); action_it != List.ACTION.end(); action_it++)
			{

				if (*action_it != '#'&&-1 != search_FatherAndType(j, *action_it))//����ƽ�
				{
					interval = true;
					temp.push_back("s" + to_string(search_FatherAndType(j, *action_it)));
				}//����Լ
				//dfa_it = grammar_dfa.begin() + j;
				else{
					//cout << "ooooo:" << *(grammar_dfa.begin() + j)->grammar.begin() << endl;
					//if()
					loct = search_location_retrun(*(grammar_dfa.begin() + j), *action_it);
					if (loct >= 0)
					{
						if (loct == 0)
						{
							//cout << "11111111111:" << *action_it << endl;
							temp.push_back("acc");
						}
							
						else temp.push_back("r" + to_string(loct));
					}
					else {
						temp.push_back("error");
					}
				}
			}
			//���GOTO
			//cout << "000" << endl;
			for (goto_it = List.GOTO.begin(); goto_it != List.GOTO.end(); goto_it++)
			{
				//cout << "2" << endl;
				//cout << search_FatherAndType(j, *goto_it) << endl;
				if (-1 != search_FatherAndType(j, *goto_it))
				{
					interval = true;
					temp.push_back(to_string(search_FatherAndType(j, *goto_it)));
				}
				else {
					temp.push_back("error");
				}
			}
//			if (!interval)//���ƽ�,�ǹ�Լ��
//			{
//				//cout << "9" << endl;
//				for (vector<string>::iterator it = (grammar_dfa.begin() + j)->grammar.begin(); it != (grammar_dfa.begin() + j)->grammar.end(); it++)
//				{
//					//cout <<"333��"<< it->substr(0, it->length() - 1) << endl;
//					loct = GetItemLocation(it->substr(0, it->length() - 1));//��ȡ�������Ӷ�Ӧ��Լԭ�ķ��еĵڼ���
//					//cout << "sss:" << loct << endl;
//					if ( loct!= -1)//�������꣬��Ϊ-1������������acc��
//					{
//						//cout << "inequal" << endl;
//						temp.assign(temp.size()-(List.GOTO.end()-List.GOTO.begin()), "r" + to_string(loct));
//						//temp.clear();
//						for (vector<char>::iterator g = List.GOTO.begin(); g != List.GOTO.end();g++)
//						temp.push_back("error");//��Լ��
//						break;
//					}	
//					//cout << "222" << endl;
//				}
//				//����Ƿ��ѱ����ķ�����
//				if (loct==-1)
//					temp[List.ACTION.end() - List.ACTION.begin() - 1] = "acc";//������
//				else {
//					loct = -1;
//				}
//			}
//			
//			/*cout << "==========================================================================" << endl;
//			cout << j << endl;
//			for (vector<string>::iterator it = temp.begin(); it != temp.end(); it++)
//				cout << *it << endl;
//*/



			List.status.push_back(temp);//�������һ��
			temp.clear();
			interval = false;
			//for (it = List.status.begin() + 1/*�ӵڶ�����ʼ����*/; it != List.status.end(); it++)
			//{
			//}
		}
	return 0;
}
//���ķ��޸�Ϊ�ع��ķ�
void loading_file::Mend_grammar(){
	vector<string>::iterator it;
	gra_expansion gra;
	int location = 3;
	string temp;
	//�޸�Ϊ�ع��ķ�
	for (it = grammar.begin(); it != grammar.end(); it++)
	{
		//cout << (*it)[0] << endl;
		temp = (*it).substr(0, 1) + "'->." + (*it).substr(0, 1);
		gra.grammar.push_back(temp);//����ع��ķ����������
		//cout << *gra.grammar.begin() << endl;
		for (int i = 3; i < (*it).length(); i++)//ȥ��->�����ַ�:����Ϊ3
		{
			if ((*it)[i] == '|')//�ָ���|
			{
				temp = (*it).substr(0, 3) + "." + (*it).substr(location, i - location);
				gra.grammar.push_back(temp);
				location = i + 1;
			}
		}
		temp = (*it).substr(0, 3) + "." + (*it).substr(location, (*it).length() - location);
		gra.grammar.push_back(temp);//���һ��ʽ�ӣ�E->n������û�и�'|'��

		gra.father.push_back(it - grammar.begin() - 1);
		gra.type.push_back('#');

		grammar_dfa.push_back(gra);//DFA���׸���Ŀ��
		grammar_exp=gra.grammar;//�ع��ķ�

		//�����ݵ��ع��ķ��еġ�.��ȫ������
		for (vector<string>::iterator g = grammar_exp.begin(); g != grammar_exp.end(); g++)
		{
			//cout << *g << endl;
			g->erase(g->find('.', 0), 1);//����'.'

		}
	}
}
//������Ŀ�������С�->���Ҳ಻ͬ���ַ����洢���������
vector<char> loading_file::search_sign(vector<string> gra){
	vector<char> capability;
	std::size_t loct;
	string temp;
	vector<string>::iterator g;
	for (g = gra.begin(); g != gra.end(); g++)
	{
		loct = (*g).find('.', 0);
		if (loct!= string::npos)
			temp = (*g).substr(loct+1);
		else{
			cout << "the expension of grammar is error!" << endl;
		}
		//��ʼ���ҵ�ǰ�ķ���->�Ҳ಻ͬ���ַ��洢�ڱ���capability
		for (int i = 0; i < temp.length(); i++)
		{
			if (capability.empty())
				capability.push_back(temp[i]);
			else{
				if (find(capability.begin(), capability.end(), temp[i]) == capability.end()){
					//û�ҵ�
					capability.push_back(temp[i]);
				}
			}
		}
	}
	return capability;
}
//������ɵ���Ŀ���Ƿ��������ɹ��ģ������ɷ���TRUE,���򷵻�FALSE
bool loading_file::check_item(gra_expansion val,int father,char type)
{
	vector<gra_expansion>::iterator g;
	for (g = grammar_dfa.begin(); g != grammar_dfa.end(); g++)
	{
		if (g->grammar == val.grammar)
		{
			g->father.push_back(father);
			g->type.push_back(type);
			return true;
		}
	}
	return false;
}
//������Ŀ���Ƿ��й�Լ��,���Ҹ��ַ��ڹ�Լ���ս����follow���У������򷵻ع�Լλ��(>=0)����֮����-1,
int loading_file::search_location_retrun(gra_expansion val, char ch)
{
	vector<char>::iterator type_it;
	vector<int>::iterator father_it;
	for (father_it = val.father.begin(), type_it = val.type.begin(); father_it != val.father.end()&&type_it != val.type.end(); father_it++,type_it++)
	{
		if (ch == '#')
		{
			if (*father_it == 0 && *type_it == '#')//acc��
				return 0;
			else if (*type_it == '#'&&*father_it != -1 && 1 == search_Vt_in_follow((*(grammar_exp.begin() + *father_it))[0], ch))//�����ַ��Ƿ��ڸ÷��ս����follow����
				return *father_it;
		}
		else if (*type_it == '#'&&*father_it != -1 && *father_it!=0)//�ǹ�Լ��(�ų��ع��ķ���һ����Լ�ľ���)
		{		
			if (1==search_Vt_in_follow((*(grammar_exp.begin() + *father_it))[0], ch))//�����ַ��Ƿ��ڸ÷��ս����follow����
				return *father_it;
		}
	}
	return -1;
}
/*�����ַ��Ƿ��ڶ�Ӧ�ķ��ս��follow�У����ַ��ڸ÷��ս����follow���У�1���������ڸ÷��ս����follow���򷵻�-1��
�����ڸ�follow�򣬵����ַ����ڸ�follow���򷵻�  0������������� - 2*/
int loading_file::search_Vt_in_follow(char Vn, char Vt){
	vector<Set>::iterator it;
	for (it = Follow.begin(); it != Follow.end(); it++)
	{
		if (it->sign == Vn);//���ڸ�follow����
		{
			if (find_sign(it->val, Vt))
			{
				return 1;
			}
			else return 0;
		}
		//return -2;
	}
		return -1;
}
//��ȡ��Լ������ԭ���ķ��е�λ��
int loading_file::GetItemLocation(string val)//��ȡ��ǰһ��������ԭ���ķ��е�λ��
{
	vector<string>::iterator it;
	string temp;
	for (it = grammar_exp.begin(); it != grammar_exp.end(); it++)
	{
		//cout << *it << endl;
		if (*it== val)
		{
			return it - grammar_exp.begin();//�������꣬��Ϊ-1������������acc��
		}
		temp.clear();
	}
	return -1;
}
//�����й淶���ַ��Ƿ��ǵ�һ����Ŀ��->��ߵĵ�һ���ַ�
vector<string>loading_file::check_sign(char ch)
{
	vector<string>temp;
	vector<string>::iterator g ;
	for (g = grammar_dfa.begin()->grammar.begin(); g != grammar_dfa.begin()->grammar.end(); g++)
	{
		if ((*g)[0] == ch && (*g)[1]!='\'')
			temp.push_back(*g);
	}
	return temp;
}
//���һ���淶��
bool loading_file::GetItem(vector<string> grammar, char ch,int father/*���ĸ����ڵ㴫��*/)
{
	//cout << "11" << endl;
	gra_expansion exp_gra;
	vector<string>temp;
	vector<string>::iterator g;
	std::size_t loct;
	//char temp;
	for (g = grammar.begin(); g != grammar.end(); g++)
	{
		loct = (*g).find('.', 0);
		if ( loct!= string::npos &&loct<g->length()-1&&(*g)[loct + 1] == ch)
		{
			(*g)[loct + 1] = '.';
			(*g)[loct] = ch;
			exp_gra.grammar.push_back(*g);
			//cout << *g << endl;
			temp = check_sign((*g)[loct + 2]);//�����ַ��Ƿ���->����߳���
			if (!temp.empty())
			{
				//cout << *temp.begin() << endl;
				exp_gra.grammar.insert(exp_gra.grammar.end(), temp.begin(), temp.end()); // ��v2�ӵ�v1ĩβ
			}
			if (loct+1 == g->length() - 1)
			{
				//cout << "222222:" << *g << endl;
				//cout << "111111:" << GetItemLocation(g->substr(0, g->length() - 1)) << endl;
				exp_gra.father.push_back(GetItemLocation(g->substr(0,g->length()-1)));
				exp_gra.type.push_back('#');
			}	
		}
		
		temp.clear();//�������
	}
	exp_gra.father.push_back(father);
	exp_gra.type.push_back(ch);
	if (!check_item(exp_gra, father, ch))//������ɵ���Ŀ���Ƿ��������ɹ���(δ����)
	{
		if (!exp_gra.grammar.empty())
		grammar_dfa.push_back(exp_gra);
		/*vector<string>::iterator g;
		for (g = exp_gra.grammar.begin(); g != exp_gra.grammar.end(); g++)
			{
				cout << *g << endl;
			}*/
		return true;
	}
	else {//�����ɹ�
		/*cout << "1" << endl;
		for (g = exp_gra.grammar.begin(); g != exp_gra.grammar.end(); g++)
		{
			cout << *g << endl;
		}*/
		return false;
	}
}
//����DFA
bool loading_file::CreatDFA(){
	vector<gra_expansion>::iterator it;
	vector<string>::iterator g;
	vector<char>::iterator c;
	vector<char>cup;
	int number=0;//�淶�ļ�����
	for (it = grammar_dfa.begin(); it != grammar_dfa.end(); number++,it++)
	{
		cup= search_sign(it->grammar);
		for (c = cup.begin(); c != cup.end(); c++)
		{
			//cout << *c << endl;
			GetItem(it->grammar, *c, number/*���ĸ����ڵ㴫��*/);
			it = grammar_dfa.begin()+number;//GetItem�������grammar_dfa������չ����������ʧЧ����Ҫ��ԭ
		}
	}
	return true;
}
//��ʾ���õ�DFA����������Ŀ��������
int loading_file::DisplayDFA(){
	vector<gra_expansion>::iterator it;
	vector<string>::iterator g;
	vector<int>::iterator i;
	vector<char>::iterator c;
	int j = 0;
	for (it = grammar_dfa.begin(); it != grammar_dfa.end(); it++,j++)
	{
		cout << j << ':' << endl;
		//��ʾ�淶��
		for (g = it->grammar.begin(); g != it->grammar.end(); g++)
		{
			cout << *g << endl;
		}
		//��ʾ������ţ���ʾת�������ַ�
		for (i = it->father.begin(), c = it->type.begin(); c != it->type.end() && i != it->father.end(); i++,c++)
		{
			cout << "('" << *i << "'" << ",'" << *c << "')" << endl;
		}
		
		
	}
	return j;//���ع淶������
}
//��ʾ������
void loading_file::DisplayGrammarList(){
	vector<char>::iterator it;
	vector<vector<string>>::iterator p1;//��һ��
	vector<string>::iterator p2;//�ڶ���ѭ��
	cout << std::left << setw(10) << " ";
	//��ʾACTION��ͷ
	for (it = List.ACTION.begin(); it != List.ACTION.end(); it++)
	{
		 cout<< std::left << setw(10) << *it;
	}
	//��ʾGOTO��ͷ
	for (it = List.GOTO.begin(); it != List.GOTO.end(); it++)
	{
		cout << std::left << setw(10) << *it;
	}
	cout << endl;

	for (p1 = List.status.begin(); p1 != List.status.end(); p1++)
	{
		cout << std::left << setw(10) << p1 - List.status.begin();
		for (p2 =p1->begin(); p2 != p1->end(); p2++)
			cout << std::left << setw(10) << *p2;
		cout << endl;
	}
}

//��ȡ������Ŀ�����з��ս����FIRST��,������
vector<Set>loading_file::CreateFirst(vector<string> item){
	//vector<char> Vn;
	vector<Set> first_item;
	vector<Set> null_item;
	vector<Set> inter_item;
	Set temp;
	vector<char>::iterator c;
	vector<Set>::iterator it;//��һ��ѭ��
	Vn=search_Vn(item);//��ȡ��ǰ��Ŀ����->��ߵķ��ս��
	for (c = Vn.begin(); c != Vn.end(); c++)
	{
		//cout << "left:" << *c<<endl;
		temp = search_first_sign(item, Vn, *c);//��ȡ���ս��->��ĵ�һ���ַ�
		if (!temp.val.empty())
		{
			first_item.push_back(temp);
		}
		else cout << "���ķ����󣬹�����Ŀ����first��ʱ����1��" << endl;
	}
	//vector<char>::iterator it;
	//===============1�������еķ��ս���滻��ɶ�Ӧ��first��ȫ�����ս����==================
	for (it = first_item.begin(); it != first_item.end(); it++)
	{
		for (c = it->val.begin(); c != it->val.end(); c++)
		{
			if (!find_sign(Vn, *c))//ֻҪ����һ�����ս������ô�㲻�洢
				break;
		}
		inter_item.push_back(*it);//���Ѿ�ȷ����first���ϱ���
	}
	//===============��������δȷ���ķ��ս���滻����ȷ����first����==========================
	for (it = first_item.begin(); it != first_item.end(); it++)
	{
		for (c = it->val.begin(); c != it->val.end(); c++)
		{
			if (find_sign(Vn, *c))//�ҵ����ս��λ��
			{
				//cout <<"111:"<<*c << endl;
				temp=search_child_set(inter_item, *c);//��ȡ�÷��ս����Ӧ��first��
				c = it->val.erase(c);//ɾ���÷��ս��
				//cout << temp.val[0] << endl;
				if (temp.sign!=NULL)//�÷��ս������ȷ����first��
				{
					sort(temp.val.begin(), temp.val.end());//����
					temp.val.erase(unique(temp.val.begin(), temp.val.end()), temp.val.end());//ɾ�������ظ���Ԫ��
					it->val.insert(it->val.end(), temp.val.begin(), temp.val.end());//�����Ӽ�����
				}
				else { 
					cout << "���ķ������в�ȷ���ķ��ս�����޷�����first����" << endl; 
					return null_item;//����һ���յļ���
				}
				//it = first_item.begin() + i;
				//c = it->val.begin()+j;//��ԭ������
			}
		}
		inter_item.push_back(*it);//���Ѿ�ȷ����first���ϱ���
	}
	//===============��ʾ������first��========================
	cout << "=============����Ϊfirst����===========" << endl;
	for (it = first_item.begin(); it != first_item.end(); it++)
	{ 
		cout << it->sign << ": ";
		for (c = it->val.begin(); c != it->val.end(); c++)
		{
			cout << *c;
		}
		cout << endl;
	}
	
	return first_item;
}
//��ȡ������Ŀ�����з��ս����Follow��,������
vector<Set>loading_file::CreateFollow(vector<string> item)
{
	vector<char>::iterator c;
	vector<char>::iterator c2;
	Set set;
	vector<Set> inter_follow;
	vector<Set>::iterator it;
	follow_complement comp;//��¼A->aBʱ������A-B��Ӧ��¼
	
	//��һ�����ս���϶�ʱ��ʼ����
	//set.val.push_back('#');
	for (c = Vn.begin(); c != Vn.end(); c++)
	{
		set=search_follow_sign(item, *c,comp);
		if (!set.val.empty())
			inter_follow.push_back(set);
		set.val.clear();//���
	}
	//====================���A->aB������============
	for (c = comp.val_start.begin(),c2=comp.val_end.begin(); c != comp.val_start.end(),c2!=comp.val_end.end(); c++,c2++)
	{
		if (-1 != search_FollowSet(inter_follow,*c))//���ڸü���
		{
			//cout << "1111" << endl;
			//��A->aB��ʽ����follow(A)���뵽follow(B)��
			(inter_follow.begin() + search_FollowSet(inter_follow, *c2))->val.insert((inter_follow.begin() + search_FollowSet(inter_follow, *c2))->val.end(), (inter_follow.begin() + search_FollowSet(inter_follow, *c))->val.begin(), (inter_follow.begin() + search_FollowSet(inter_follow, *c))->val.end());
		}
	}
	//===================��������Ԫ���ظ�������========
	eliminating_duplicate(inter_follow);
	//===================��ʾfollow��=================
	cout << "=============����Ϊfollow����===========" << endl;
	for (it = inter_follow.begin(); it != inter_follow.end(); it++)
	{
		cout << it->sign << ": ";
		for (c = it->val.begin(); c != it->val.end(); c++)
		{
			cout << *c;
		}
		cout << endl;
	}
	return inter_follow;
}
//��ȡһ�����ս����first
Set loading_file::search_first_sign(vector<string> item, vector<char> val,char ch)
{
	Set one_first_set;
	vector<string>temp;
	vector<string>::iterator g;
	one_first_set.sign = ch;//first����Ӧ���ַ�
	for (g = item.begin(); g != item.end(); g++)
	{
		if ((*g)[0] == ch)//�ҵ���Ӧ�ľ���
		{
			if (g->find("->") != string::npos)
			{
				//cout << "����->��λ��" << (*g)[g->find("->")+2] << endl;
				//if (!find_sign(val, (*g)[g->find("->") + 2]))//���Ƿ��ս��
				//{
					//cout << "222" << endl;
					one_first_set.val.push_back((*g)[g->find("->") + 2]);//�����Ƿ��ս��
				//}
				//else{//�Ƿ��ս��
				//	cout << "333" << endl;
				//	if (!search_first_sign(item, val, (*g)[g->find("->") + 2]).val.empty()/*�ݹ鷵����first*/)
				//	{					
				//		//���õݹ齫�÷��ս����first�����������ַ���first��
				//		one_first_set.val.insert(one_first_set.val.end(), search_first_sign(item, val, (*g)[g->find("->") + 2]).val.begin(), search_first_sign(item, val, (*g)[g->find("->") + 2]).val.end());
				//	}
				//	else cout << "���ķ������Ϲ��򣬻�ȡ������Ŀ��first��ʱ����2��" << endl;
				//}
			}
		}

	}
	while (1){
		if (find(one_first_set.val.begin(), one_first_set.val.end(), one_first_set.sign) != one_first_set.val.end())
			one_first_set.val.erase(find(one_first_set.val.begin(), one_first_set.val.end(), one_first_set.sign));//ɾ���������������ַ���ͬ��Ԫ��
		else break;
	}
	return one_first_set;
}
//���ҵ�ǰ�ַ��Ƿ��Ѿ�����vecter��
bool loading_file::find_sign(vector<char> val, char ch){
	vector<char>::iterator c;
	for (c = val.begin(); c != val.end(); c++)
	{
		if (*c == ch)
			return true;
	}
	return false;
}
//��ȡ��ǰ��Ŀ������->�󲿲�ͬ�ķ��ս��
vector<char> loading_file::search_Vn(vector<string> gra){
	vector<char> capability;
	std::size_t loct;
	string temp;
	vector<string>::iterator g;
	for (g = gra.begin(); g != gra.end(); g++)
	{
		if (capability.empty())
			capability.push_back((*g)[0]);
		else {
			if(!find_sign(capability, (*g)[0]))//��������
				capability.push_back((*g)[0]);
		}
		//cout << "222:" << (*g)[0] << endl;
	}
	return capability;
}
//���Ҷ�Ӧ�ַ�����ϸ����
Set loading_file::search_child_set(vector<Set> inter_Item,char ch){
	//vector<char>::iterator c;
	Set set;
	vector<Set>::iterator it;//��һ��ѭ��
	for (it = inter_Item.begin(); it != inter_Item.end(); it++)
	{
		if (it->sign == ch)//�ҵ����ս��λ��
			set=*it;
	}
	return set;
}
//����->�ұ߷��ս������һ��
Set loading_file::search_follow_sign(vector<string> item,char ch,follow_complement &comp)
{
	vector<string>::iterator it;
	vector<char> temp;
	Set set;
	set.sign = ch;
	std::size_t loct;
	9 + 11111 - 12939;
	if (ch == (*grammar_exp.begin())[0])
	{
		set.val.push_back('#');
	}
	for (it = item.begin(); it != item.end(); it++)
	{
		loct =it->find("->");
		if (loct != string::npos)
		{
			//�˴���Ӧ���һ���к���������������ͬ�ķ��ս������δ���
			if ((loct = it->find(ch, loct+2)) != string::npos&&loct + 1 < it->length())//����follow�ַ�
			{
				//cout << "ppp:" << (*it)[loct] << ":" << (*it)[loct + 1] << endl;
				temp = search_sign_in_firstSet((*it)[loct+1]);
				if (temp.empty())//���ս��
					set.val.push_back((*it)[loct + 1]);
				else set.val.insert(set.val.end(), temp.begin(), temp.end());//������ս����first��
			}
			else{
				if (loct != string::npos&&loct ==it->length() - 1&& (*it)[0] != (*it)[loct]&&(*it)[1] !='\'')//�÷��ս���Ǿ��ӵ����һ���ַ���->��ߵķ��ս��������������
				{
					//cout << "loct" << (*it)[loct] << endl;
					comp.val_start.push_back((*it)[0]);
					comp.val_end.push_back((*it)[loct]);
				}
			}
				
		}
		else {
			cout << "���ķ����󣡴���follow��ʱ�Ҳ���'->'�ַ�����" << endl;
			return set;
		}
		loct = string::npos;
	}
	return set;
}
//���Ҹ��ַ��Ƿ��Ƿ��ս����������first��
vector<char> loading_file::search_sign_in_firstSet(char ch)
{
	vector<Set>::iterator it;
	vector<char> temp;
	//vector<char>::iterator c;
	for (it = First.begin(); it != First.end(); it++)
	{
		if (it->sign == ch)
			temp = it->val;
	}
	return temp;
}
//����ĳ�����ս����follow��,���ظ�FOLLOW����VECTOR�е�λ�ã��Ҳ����򷵻�-1
int loading_file::search_FollowSet(vector<Set>follow,char Vn)
{
	vector<Set>::iterator it;
	///Set set;

	for (it = follow.begin(); it != follow.end(); it++)
	{
		//cout << "222:" << it->sign << endl;
		if (it->sign == Vn)
			return it - follow.begin();
	}
	return -1;
}
//������������������ظ�Ԫ��
bool loading_file::eliminating_duplicate(vector<Set> &set_vector)
{
	vector<Set>::iterator it;
	for (it = set_vector.begin(); it != set_vector.end(); it++)
	{
		sort(it->val.begin(), it->val.end());//����
		//sort(temp.val.begin(), temp.val.end());//����
		it->val.erase(unique(it->val.begin(), it->val.end()), it->val.end());//ɾ�������ظ���Ԫ��
	}
	return true;
}