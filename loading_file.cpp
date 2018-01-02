#include"loading_file.h"
#include <Windows.h>
bool loading_file::load_file(string KeyWords_fs, string signa_fs)
{
	//int line;
	ifile.open(KeyWords_fs);//打开关键词文本
	if (!ifile.is_open())
		cout << "Fail to open KeyWords file.Please  check your index." << endl;
	cout << endl << "============================================" << endl;
	cout << "打开keywords成功！" << endl;
	if(getline(ifile, read_buf))//第一行不读
	while (getline(ifile, read_buf)){//读取文件内的数据，每次读取一行（从第二行读起）
		int temp = read_buf.find(",");
		if (std::string::npos == temp)
		{
			cout << "关键词文件第" << KeyWords_value.size() + 2 << "行，值与编码的分隔符不是','(英文逗号)" << endl;
		}
		else { 
			
			KeyWords_num.push_back(atoi(read_buf.substr(0, temp).data()));//转换成整型
			KeyWords_value.push_back(read_buf.substr(temp + 1));
			//cout << KeyWords_num[0] <<"  "<< KeyWords_value[0]<< endl;
		}
		
		//if (ifile.eof())//文件结束时,结束读取操作
			//break;
	}
	ifile.close();
	cout << "已读取全部关键词..." << endl;

	ifile.open(signa_fs);
	if (!ifile.is_open())
		cout << "Fail to open index file.Please  check your index." << endl;
	//string value;
	char interval;
	if (getline(ifile, read_buf))//第一行不读
	while (ifile >> interval)//读取字符表
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
	//==========================读取语法文件=====================
	ifile.open(grammar_fs);
	if (!ifile.is_open())
		cout << "Fail to open index file.Please check your index." << endl;
	//string value;
	cout << "loading grammar..." << endl;
	char interval;
	string temp;
	if (getline(ifile, read_buf))//第一行不读
	while (ifile >> interval)//读取语法文件
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
	//============================构建LR语法分析表==================
	//修改拓广文法
	Mend_grammar();
	//构建DFA
	CreatDFA();
	//DisplayDFA();
	CreatGrammarList();
	DisplayGrammarList();
	return true;
}
//在DFA中查找当前转换字符对应的父节点位置
int loading_file::search_FatherAndType(int number, char type){
	vector<gra_expansion>::iterator it;
	vector<string>::iterator g;
	vector<int>::iterator i;
	vector<char>::iterator c;
	//string temp;
	for (it = grammar_dfa.begin() + 1/*从第二个开始查找*/; it != grammar_dfa.end(); it++)
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
//创建语法分析表
int loading_file::CreatGrammarList(){
	vector<char>::iterator cp;
	vector<char> character;//获取文法中的字符种类
	character=search_sign(grammar_dfa.begin()->grammar);
	//===============构建ACTION与GOTO表表头==================
	for (cp = character.begin(); cp != character.end(); cp++)
	{
		if (check_sign(*cp).empty())
		{//属于ACTION
			List.ACTION.push_back(*cp);
		}
		else{//属于GOTO表

			List.GOTO.push_back(*cp);
		}

	}
	List.ACTION.push_back('#');//ACTION表头添加一个#
	//=================构建FIRST集与FOLLOW==============================
	//vector<string>::iterator it_exp;//拓广文法备份
	First=CreateFirst(grammar_exp);
	Follow = CreateFollow(grammar_exp);
	//for (it_exp=grammar_exp..begin())
	//=================构建状态集==============================
		vector<vector<string>>::iterator it;
		vector<char>::iterator action_it;
		vector<char>::iterator goto_it;
		vector<string>temp;
		int number= DisplayDFA();//DFA的项目数
		int loct=-1;
		bool interval=false;//1：有移进，0：无移进，可能有规约
		for (int j=0;j<number; j++)
		{
			//检查ACTION
			for (action_it = List.ACTION.begin(); action_it != List.ACTION.end(); action_it++)
			{

				if (*action_it != '#'&&-1 != search_FatherAndType(j, *action_it))//检查移进
				{
					interval = true;
					temp.push_back("s" + to_string(search_FatherAndType(j, *action_it)));
				}//检查规约
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
			//检查GOTO
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
//			if (!interval)//无移进,是规约项
//			{
//				//cout << "9" << endl;
//				for (vector<string>::iterator it = (grammar_dfa.begin() + j)->grammar.begin(); it != (grammar_dfa.begin() + j)->grammar.end(); it++)
//				{
//					//cout <<"333："<< it->substr(0, it->length() - 1) << endl;
//					loct = GetItemLocation(it->substr(0, it->length() - 1));//获取该条句子对应规约原文法中的第几句
//					//cout << "sss:" << loct << endl;
//					if ( loct!= -1)//返回坐标，若为-1，则是首条（acc）
//					{
//						//cout << "inequal" << endl;
//						temp.assign(temp.size()-(List.GOTO.end()-List.GOTO.begin()), "r" + to_string(loct));
//						//temp.clear();
//						for (vector<char>::iterator g = List.GOTO.begin(); g != List.GOTO.end();g++)
//						temp.push_back("error");//规约项
//						break;
//					}	
//					//cout << "222" << endl;
//				}
//				//检查是否已被该文法接受
//				if (loct==-1)
//					temp[List.ACTION.end() - List.ACTION.begin() - 1] = "acc";//接受项
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



			List.status.push_back(temp);//分析表的一行
			temp.clear();
			interval = false;
			//for (it = List.status.begin() + 1/*从第二个开始查找*/; it != List.status.end(); it++)
			//{
			//}
		}
	return 0;
}
//将文法修改为拓广文法
void loading_file::Mend_grammar(){
	vector<string>::iterator it;
	gra_expansion gra;
	int location = 3;
	string temp;
	//修改为拓广文法
	for (it = grammar.begin(); it != grammar.end(); it++)
	{
		//cout << (*it)[0] << endl;
		temp = (*it).substr(0, 1) + "'->." + (*it).substr(0, 1);
		gra.grammar.push_back(temp);//添加拓广文法的首条语句
		//cout << *gra.grammar.begin() << endl;
		for (int i = 3; i < (*it).length(); i++)//去掉->与首字符:长度为3
		{
			if ((*it)[i] == '|')//分隔符|
			{
				temp = (*it).substr(0, 3) + "." + (*it).substr(location, i - location);
				gra.grammar.push_back(temp);
				location = i + 1;
			}
		}
		temp = (*it).substr(0, 3) + "." + (*it).substr(location, (*it).length() - location);
		gra.grammar.push_back(temp);//最后一个式子：E->n（后面没有跟'|'）

		gra.father.push_back(it - grammar.begin() - 1);
		gra.type.push_back('#');

		grammar_dfa.push_back(gra);//DFA的首个项目集
		grammar_exp=gra.grammar;//拓广文法

		//将备份的拓广文法中的‘.’全部擦除
		for (vector<string>::iterator g = grammar_exp.begin(); g != grammar_exp.end(); g++)
		{
			//cout << *g << endl;
			g->erase(g->find('.', 0), 1);//擦除'.'

		}
	}
}
//查找项目集中所有“->”右侧不同的字符并存储，结果返回
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
		//开始查找当前文法中->右侧不同的字符存储在变量capability
		for (int i = 0; i < temp.length(); i++)
		{
			if (capability.empty())
				capability.push_back(temp[i]);
			else{
				if (find(capability.begin(), capability.end(), temp[i]) == capability.end()){
					//没找到
					capability.push_back(temp[i]);
				}
			}
		}
	}
	return capability;
}
//检查生成的项目集是否是已生成过的，已生成返回TRUE,否则返回FALSE
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
//检查该项目集是否有规约项,并且该字符在规约非终结符的follow集中，若是则返回规约位置(>=0)，反之返回-1,
int loading_file::search_location_retrun(gra_expansion val, char ch)
{
	vector<char>::iterator type_it;
	vector<int>::iterator father_it;
	for (father_it = val.father.begin(), type_it = val.type.begin(); father_it != val.father.end()&&type_it != val.type.end(); father_it++,type_it++)
	{
		if (ch == '#')
		{
			if (*father_it == 0 && *type_it == '#')//acc项
				return 0;
			else if (*type_it == '#'&&*father_it != -1 && 1 == search_Vt_in_follow((*(grammar_exp.begin() + *father_it))[0], ch))//检查该字符是否在该非终结符的follow集中
				return *father_it;
		}
		else if (*type_it == '#'&&*father_it != -1 && *father_it!=0)//是规约项(排除拓广文法第一个规约的句子)
		{		
			if (1==search_Vt_in_follow((*(grammar_exp.begin() + *father_it))[0], ch))//检查该字符是否在该非终结符的follow集中
				return *father_it;
		}
	}
	return -1;
}
/*检查该字符是否在对应的非终结符follow中，该字符在该非终结符的follow集中：1，若不存在该非终结符的follow集则返回-1，
若存在该follow则，但该字符不在该follow中则返回  0，其他情况返回 - 2*/
int loading_file::search_Vt_in_follow(char Vn, char Vt){
	vector<Set>::iterator it;
	for (it = Follow.begin(); it != Follow.end(); it++)
	{
		if (it->sign == Vn);//存在该follow集合
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
//获取规约句子在原先文法中的位置
int loading_file::GetItemLocation(string val)//获取当前一条句子在原先文法中的位置
{
	vector<string>::iterator it;
	string temp;
	for (it = grammar_exp.begin(); it != grammar_exp.end(); it++)
	{
		//cout << *it << endl;
		if (*it== val)
		{
			return it - grammar_exp.begin();//返回坐标，若为-1，则是首条（acc）
		}
		temp.clear();
	}
	return -1;
}
//检查进行规范的字符是否是第一个项目集->左边的第一个字符
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
//获得一个规范族
bool loading_file::GetItem(vector<string> grammar, char ch,int father/*由哪个父节点传来*/)
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
			temp = check_sign((*g)[loct + 2]);//检查该字符是否在->的左边出现
			if (!temp.empty())
			{
				//cout << *temp.begin() << endl;
				exp_gra.grammar.insert(exp_gra.grammar.end(), temp.begin(), temp.end()); // 把v2加到v1末尾
			}
			if (loct+1 == g->length() - 1)
			{
				//cout << "222222:" << *g << endl;
				//cout << "111111:" << GetItemLocation(g->substr(0, g->length() - 1)) << endl;
				exp_gra.father.push_back(GetItemLocation(g->substr(0,g->length()-1)));
				exp_gra.type.push_back('#');
			}	
		}
		
		temp.clear();//清空容器
	}
	exp_gra.father.push_back(father);
	exp_gra.type.push_back(ch);
	if (!check_item(exp_gra, father, ch))//检查生成的项目集是否是已生成过的(未生成)
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
	else {//已生成过
		/*cout << "1" << endl;
		for (g = exp_gra.grammar.begin(); g != exp_gra.grammar.end(); g++)
		{
			cout << *g << endl;
		}*/
		return false;
	}
}
//创建DFA
bool loading_file::CreatDFA(){
	vector<gra_expansion>::iterator it;
	vector<string>::iterator g;
	vector<char>::iterator c;
	vector<char>cup;
	int number=0;//规范的集合数
	for (it = grammar_dfa.begin(); it != grammar_dfa.end(); number++,it++)
	{
		cup= search_sign(it->grammar);
		for (c = cup.begin(); c != cup.end(); c++)
		{
			//cout << *c << endl;
			GetItem(it->grammar, *c, number/*由哪个父节点传来*/);
			it = grammar_dfa.begin()+number;//GetItem函数会对grammar_dfa进行扩展，迭代器会失效，需要还原
		}
	}
	return true;
}
//显示建好的DFA，并返回项目集的总数
int loading_file::DisplayDFA(){
	vector<gra_expansion>::iterator it;
	vector<string>::iterator g;
	vector<int>::iterator i;
	vector<char>::iterator c;
	int j = 0;
	for (it = grammar_dfa.begin(); it != grammar_dfa.end(); it++,j++)
	{
		cout << j << ':' << endl;
		//显示规范集
		for (g = it->grammar.begin(); g != it->grammar.end(); g++)
		{
			cout << *g << endl;
		}
		//显示符结点标号，显示转换来的字符
		for (i = it->father.begin(), c = it->type.begin(); c != it->type.end() && i != it->father.end(); i++,c++)
		{
			cout << "('" << *i << "'" << ",'" << *c << "')" << endl;
		}
		
		
	}
	return j;//返回规范集个数
}
//显示分析表
void loading_file::DisplayGrammarList(){
	vector<char>::iterator it;
	vector<vector<string>>::iterator p1;//第一层
	vector<string>::iterator p2;//第二层循环
	cout << std::left << setw(10) << " ";
	//显示ACTION表头
	for (it = List.ACTION.begin(); it != List.ACTION.end(); it++)
	{
		 cout<< std::left << setw(10) << *it;
	}
	//显示GOTO表头
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

//获取整个项目集所有非终结符的FIRST集,并返回
vector<Set>loading_file::CreateFirst(vector<string> item){
	//vector<char> Vn;
	vector<Set> first_item;
	vector<Set> null_item;
	vector<Set> inter_item;
	Set temp;
	vector<char>::iterator c;
	vector<Set>::iterator it;//第一层循环
	Vn=search_Vn(item);//获取当前项目集中->左边的非终结符
	for (c = Vn.begin(); c != Vn.end(); c++)
	{
		//cout << "left:" << *c<<endl;
		temp = search_first_sign(item, Vn, *c);//获取非终结的->后的第一个字符
		if (!temp.val.empty())
		{
			first_item.push_back(temp);
		}
		else cout << "该文法有误，构建项目集的first集时出错1！" << endl;
	}
	//vector<char>::iterator it;
	//===============1将集合中的非终结符替换其成对应的first（全部是终结符）==================
	for (it = first_item.begin(); it != first_item.end(); it++)
	{
		for (c = it->val.begin(); c != it->val.end(); c++)
		{
			if (!find_sign(Vn, *c))//只要含有一个非终结符，那么便不存储
				break;
		}
		inter_item.push_back(*it);//将已经确定的first集合保存
	}
	//===============将集合中未确定的非终结符替换成已确定的first集合==========================
	for (it = first_item.begin(); it != first_item.end(); it++)
	{
		for (c = it->val.begin(); c != it->val.end(); c++)
		{
			if (find_sign(Vn, *c))//找到非终结符位置
			{
				//cout <<"111:"<<*c << endl;
				temp=search_child_set(inter_item, *c);//获取该非终结符对应的first集
				c = it->val.erase(c);//删除该非终结符
				//cout << temp.val[0] << endl;
				if (temp.sign!=NULL)//该非终结符存在确定的first集
				{
					sort(temp.val.begin(), temp.val.end());//排序
					temp.val.erase(unique(temp.val.begin(), temp.val.end()), temp.val.end());//删除相邻重复的元素
					it->val.insert(it->val.end(), temp.val.begin(), temp.val.end());//将该子集加入
				}
				else { 
					cout << "该文法错误！有不确定的非终结符，无法生成first集！" << endl; 
					return null_item;//返回一个空的集合
				}
				//it = first_item.begin() + i;
				//c = it->val.begin()+j;//还原迭代器
			}
		}
		inter_item.push_back(*it);//将已经确定的first集合保存
	}
	//===============显示创建的first集========================
	cout << "=============以下为first集合===========" << endl;
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
//获取整个项目集所有非终结符的Follow集,并返回
vector<Set>loading_file::CreateFollow(vector<string> item)
{
	vector<char>::iterator c;
	vector<char>::iterator c2;
	Set set;
	vector<Set> inter_follow;
	vector<Set>::iterator it;
	follow_complement comp;//记录A->aB时的情形A-B对应记录
	
	//第一个非终结符肯定时开始符号
	//set.val.push_back('#');
	for (c = Vn.begin(); c != Vn.end(); c++)
	{
		set=search_follow_sign(item, *c,comp);
		if (!set.val.empty())
			inter_follow.push_back(set);
		set.val.clear();//清空
	}
	//====================解决A->aB的问题============
	for (c = comp.val_start.begin(),c2=comp.val_end.begin(); c != comp.val_start.end(),c2!=comp.val_end.end(); c++,c2++)
	{
		if (-1 != search_FollowSet(inter_follow,*c))//存在该集合
		{
			//cout << "1111" << endl;
			//如A->aB形式，则将follow(A)加入到follow(B)中
			(inter_follow.begin() + search_FollowSet(inter_follow, *c2))->val.insert((inter_follow.begin() + search_FollowSet(inter_follow, *c2))->val.end(), (inter_follow.begin() + search_FollowSet(inter_follow, *c))->val.begin(), (inter_follow.begin() + search_FollowSet(inter_follow, *c))->val.end());
		}
	}
	//===================消除集合元素重复的问题========
	eliminating_duplicate(inter_follow);
	//===================显示follow集=================
	cout << "=============以下为follow集合===========" << endl;
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
//获取一个非终结符的first
Set loading_file::search_first_sign(vector<string> item, vector<char> val,char ch)
{
	Set one_first_set;
	vector<string>temp;
	vector<string>::iterator g;
	one_first_set.sign = ch;//first集对应的字符
	for (g = item.begin(); g != item.end(); g++)
	{
		if ((*g)[0] == ch)//找到对应的句子
		{
			if (g->find("->") != string::npos)
			{
				//cout << "查找->的位置" << (*g)[g->find("->")+2] << endl;
				//if (!find_sign(val, (*g)[g->find("->") + 2]))//不是非终结符
				//{
					//cout << "222" << endl;
					one_first_set.val.push_back((*g)[g->find("->") + 2]);//可能是非终结符
				//}
				//else{//是非终结符
				//	cout << "333" << endl;
				//	if (!search_first_sign(item, val, (*g)[g->find("->") + 2]).val.empty()/*递归返回其first*/)
				//	{					
				//		//利用递归将该非终结符的first集加入所求字符的first中
				//		one_first_set.val.insert(one_first_set.val.end(), search_first_sign(item, val, (*g)[g->find("->") + 2]).val.begin(), search_first_sign(item, val, (*g)[g->find("->") + 2]).val.end());
				//	}
				//	else cout << "该文法不符合规则，获取构建项目的first集时出错2！" << endl;
				//}
			}
		}

	}
	while (1){
		if (find(one_first_set.val.begin(), one_first_set.val.end(), one_first_set.sign) != one_first_set.val.end())
			one_first_set.val.erase(find(one_first_set.val.begin(), one_first_set.val.end(), one_first_set.sign));//删除集合中与所求字符相同的元素
		else break;
	}
	return one_first_set;
}
//查找当前字符是否已经存在vecter中
bool loading_file::find_sign(vector<char> val, char ch){
	vector<char>::iterator c;
	for (c = val.begin(); c != val.end(); c++)
	{
		if (*c == ch)
			return true;
	}
	return false;
}
//获取当前项目集所有->左部不同的非终结符
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
			if(!find_sign(capability, (*g)[0]))//还不存在
				capability.push_back((*g)[0]);
		}
		//cout << "222:" << (*g)[0] << endl;
	}
	return capability;
}
//查找对应字符的详细集合
Set loading_file::search_child_set(vector<Set> inter_Item,char ch){
	//vector<char>::iterator c;
	Set set;
	vector<Set>::iterator it;//第一层循环
	for (it = inter_Item.begin(); it != inter_Item.end(); it++)
	{
		if (it->sign == ch)//找到非终结符位置
			set=*it;
	}
	return set;
}
//查找->右边非终结符的下一个
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
			//此处还应检测一句中含有两个及以上相同的非终结符该如何处理
			if ((loct = it->find(ch, loct+2)) != string::npos&&loct + 1 < it->length())//存在follow字符
			{
				//cout << "ppp:" << (*it)[loct] << ":" << (*it)[loct + 1] << endl;
				temp = search_sign_in_firstSet((*it)[loct+1]);
				if (temp.empty())//是终结符
					set.val.push_back((*it)[loct + 1]);
				else set.val.insert(set.val.end(), temp.begin(), temp.end());//加入非终结符的first集
			}
			else{
				if (loct != string::npos&&loct ==it->length() - 1&& (*it)[0] != (*it)[loct]&&(*it)[1] !='\'')//该非终结符是句子的最后一个字符且->左边的非终结符不等于他本身
				{
					//cout << "loct" << (*it)[loct] << endl;
					comp.val_start.push_back((*it)[0]);
					comp.val_end.push_back((*it)[loct]);
				}
			}
				
		}
		else {
			cout << "该文法有误！创建follow集时找不到'->'字符串。" << endl;
			return set;
		}
		loct = string::npos;
	}
	return set;
}
//查找该字符是否是非终结符，并且有first集
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
//查找某个非终结符的follow集,返回该FOLLOW集在VECTOR中的位置，找不到则返回-1
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
//消除集合里面的所有重复元素
bool loading_file::eliminating_duplicate(vector<Set> &set_vector)
{
	vector<Set>::iterator it;
	for (it = set_vector.begin(); it != set_vector.end(); it++)
	{
		sort(it->val.begin(), it->val.end());//排序
		//sort(temp.val.begin(), temp.val.end());//排序
		it->val.erase(unique(it->val.begin(), it->val.end()), it->val.end());//删除相邻重复的元素
	}
	return true;
}