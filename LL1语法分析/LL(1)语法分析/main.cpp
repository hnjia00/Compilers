//
//  main.cpp
//  LL(1)语法分析
//
//  Created by 贾浩男 on 2019/4/21.
//  Copyright © 2019年 贾浩男. All rights reserved.
//

#include "LL1.h"


void init(){
    start='S';
    // Vt: '=':18 '+':19 '-':21 '*':23 '/':24 'i':10 '(':27 ')':28
    Vt.insert('=');
    Vt.insert('+');
    Vt.insert('-');
    Vt.insert('*');
    Vt.insert('/');
    Vt.insert('i');
    Vt.insert(')');
    Vt.insert('(');
    
    //Vn: S E E'(X) T T'(Y) F A M V
    Vn.insert('S');
    Vn.insert('E');
    Vn.insert('X');
    Vn.insert('T');
    Vn.insert('Y');
    Vn.insert('F');
    Vn.insert('A');
    Vn.insert('M');
    Vn.insert('V');
    
    //初始化rule产生式
    FILE *fp1=fopen("/Users/jiahaonan/Desktop/compile/in3.0-rule.txt", "r");
    while (!feof(fp1)) {
        char left[10]="",right[10]="";
        fscanf(fp1, "%s %s",left,right);
        //将ε置换为' '
        if(right[0]=='\xa6'&&right[1]=='\xc5'){
            right[0]=' ';
            right[1]='\0';
        }
        //没有left
        if(rule.count(left)==0){
            vector<string> temp;
            temp.push_back(right);
            rule.insert(make_pair(left, temp));
        }
        //有left
        else{
            //插入left对应的新的right
            rule.at(left).push_back(right);
        }
    }
    rule.erase(rule.begin());//删除首部空规则
    fclose(fp1);
}

void make_first(){
    //初始化first集
    for(set<char>::iterator it = Vn.begin();it!=Vn.end();it++){
        set<char>t;
        first.insert(make_pair(*it,t));
    }
    //遍历规则rule
    //2.若X∈Vn,有X→aα,(a∈Vt)或/和X→ε 则a或/和ε∈FIRST(x)
    for(map<string,vector<string>>::iterator it=rule.begin();it!=rule.end();it++){
        //用tempp记录it->first对应的first集
        set<char> tempp;
        //left -> right
        vector<string> right=it->second;
        string left=it->first;
        for(int i=0;i<right.size();i++){
            // X->aα (a∈Vt) a即right[i][0]
            if(Vt.count(right[i][0])!=0){
                tempp.insert(right[i][0]);
            }
            // X→ε 则ε∈FIRST(x)
            if(right[i]==" "){
                tempp.insert(' ');
            }
        }
        if(first.at(*left.c_str()).size()==0){
            first.at(*left.c_str())=tempp;
        }
        else
            first.at(*left.c_str()).insert(tempp.begin(),tempp.end());
    }
    
    //循环结束条件： 直到每一个FIRST(X)不再增大为止
    //3.对X→Y0Y1…….Yk(且Y0 ∈Vn), 反复使用以下直到每一个FIRST(X)不再增大为止
    set<char> temp[9];
    int k=0,first_size=0;
    for(map<string,vector<string>>::iterator it=rule.begin();;it++,k++){
        //用temp[k]记录it->first对应的first集
        //set<char> temp;
        //left -> right
        vector<string> right=it->second;
        string left=it->first;
        //3.对X→Y0Y1…….Yk(且Y0 ∈Vn), 反复使用以下直到每一个FIRST(X)不再增大为止
        bool flag=true;//检测连续含{ε}的Yj
        for(int i=0;i<right.size();i++){
            for(int j=0;j<right[i].size()&&(Vn.count(right[i][j])>0)&&flag;j++){
                //FIRST(Yj)
                set<char> temp1=first.at(right[i][j]);
                //i 若Y0∈Vn  则把FIRST(Y0)\{ε}元素加入FIRST(X)中
                if(j==0){
                    //\{ε}
                    if(temp1.count(' ')>0){
                        temp1.erase(' ');
                    }
                    //Y0不含{ε}
                    else{
                        flag=false;
                    }
                    //FIRST(Y0)\{ε}
                    temp[k].insert(temp1.begin(),temp1.end());
                }
                /*
                 ii 若Y1、Y2、……Y i-1 ∈Vn(2≤i ≤k) 且ε ∈FIRST(Y j) (1≤j ≤i-1)
                    则把FIRST(Yi )\{ε}元素加入FIRST(x)中
                 */
                else if(j<right[i].size()-1){
                    //Y0...Yj-1含{ε}
                    if(temp1.count(' ')>0){
                        temp1.erase(' ');
                    }
                    //Yj不含{ε}
                    else{
                        flag=false;
                    }
                    //FIRST(Yj)\{ε}
                    temp[k].insert(temp1.begin(),temp1.end());
                }
                /*
                 iii 若Y0、Y1、……Yk ∈Vn且ε ∈FIRST(Yj)(0≤j ≤k)
                    则把ε元素加入FIRST(x)中
                 */
                else{
                    temp[k].insert(' ');
                }
            }
        }
        if(first.at(*left.c_str()).size()==0){
            first.at(*left.c_str())=temp[k];
        }
        else
            first.at(*left.c_str()).insert(temp[k].begin(),temp[k].end());
        
        //跳出条件
        if(k==first.size()-1){
            int cnt=0;
            //求和，观察first集有无增大
            for(int kk=0;kk<=k;kk++){
                cnt+=temp[kk].size();
            }
            //和上一次大小一样，循环终止
            if(cnt==first_size)
                break;
            //重新循环
            else{
                k=0;
                it=rule.begin();
                first_size=cnt;
            }
        }
    }
}

void make_follow(){
    //初始化follow集
    for(set<char>::iterator it = Vn.begin();it!=Vn.end();it++){
        set<char>t;
        //令#∈FOLLOW(S) S为文法开始符号
        if(*it==start)
            t.insert('#');
        follow.insert(make_pair(*it,t));
    }
    
    //遍历规则rule
    //2.对A→ αBβ,且β ≠ε, 则将 FIRST(β)\{ε}加入FOLLOW(B)中
    for(map<string,vector<string>>::iterator it=rule.begin();it!=rule.end();it++){
        //left -> right
        vector<string> right=it->second;
        string left=it->first;
        //对每个left遍历
        for(int i=0;i<right.size();i++){
            //对left的每一个规则遍历
            for(int j=0;j<right[i].size();j++){
                //找到B所在：right[i][j]
                if(Vn.count(right[i][j])!=0){
                    set<char> tt;
                    //β属于Vt
                    if(Vt.count(right[i][j+1])!=0){
                        tt.insert(right[i][j+1]);
                        follow.at(right[i][j]).insert(tt.begin(),tt.end());
                    }
                    //β属于Vn,FIRST(β)\{ε}加入FOLLOW(B)中
                    if(Vn.count(right[i][j+1])!=0){
                        tt=first.at(right[i][j+1]);
                        tt.erase(' ');
                        follow.at(right[i][j]).insert(tt.begin(),tt.end());
                    }
                    
                }//找到B所在：right[i][j]
            }//对left的每一个规则遍历
        }//对每个left遍历
    }//遍历规则rule
    
    // 3.反复, 直至每一个FOLLOW(A)不再增大
    //   对A→ αB或A→ αBβ(且ε ∈ FIRST(β))
    //   则FOLLOW(A)中的全部元素加入FOLLOW(B)
    int k=0;
    int follow_size=0;//记录follow集的大小
    for(map<string,vector<string>>::iterator it=rule.begin();;it++,k++){
        //left -> right
        vector<string> right=it->second;
        string left=it->first;
        //对每个left遍历(一个left对应多个规则)
        for(int i=0;i<right.size();i++){
            //对left的每一个规则遍历（一个规则对应多个字符）
            for(int j=0;j<right[i].size();j++){
                //找到B所在：right[i][j]
                if(Vn.count(right[i][j])!=0){
                    set<char> tt;
                    //到规则的最后一个字符
                    if(j==right[i].size()-1){
                        //最后一个字符是Vn，即A→αB，则FOLLOW(A)中的全部元素加入FOLLOW(B)
                        tt=follow.at(*left.c_str());
                        follow.at(right[i][j]).insert(tt.begin(),tt.end());
                    }
                    //没到最后一个,即A→ αBβ(且ε∈FIRST(β)) 这种情况
                    else{
                        bool flag=true; 
                        //检索ε∈FIRST(β)是否成立
                        for(int jj=j+1;jj<right[i].size()&&flag;jj++){
                            //right[i][jj]是Vn且包含ε
                            if(Vn.count(right[i][jj])!=0&&first.at(right[i][jj]).count(' ')!=0){
                                continue;
                            }
                            //right[i][jj]不是Vn或不包含ε
                            else{
                                flag=false;
                            }
                        }
                        //A→ αBβ(且ε∈FIRST(β))情况满足
                        if(flag){
                            tt=follow.at(*left.c_str());
                            follow.at(right[i][j]).insert(tt.begin(),tt.end());
                        }
                    }
                }//找到B所在：right[i][j]
            }//对left的每一个规则遍历
        }//对每个left遍历
        
        //跳出条件
        if(k==follow.size()-1){
            int cnt=0;
            for(map<char,set<char>>::iterator it1=follow.begin();it1!=follow.end();it1++){
                cnt+=it1->second.size();
            }
            //和上一次大小一样，循环终止
            if(cnt==follow_size)
                break;
            //重新循环
            else{
                k=0;
                it=rule.begin();
                follow_size=cnt;
            }
        }//跳出条件
    }
}

void make_table(){
    
    string action;
    //纵轴遍历Vn
    for(set<char>::iterator it1=Vn.begin();it1!=Vn.end();it1++){
        //横轴遍历Vt
        for(set<char>::iterator it2=Vt.begin();it2!=Vt.end();it2++){
            //结构体t（vn，vt）来记录坐标
            point t;
            t.vn=*it1;
            t.vt=*it2;
            action="";
            //如果it2是it1的first集元素:it2∈first(it1)
            if(first.at(t.vn).count(t.vt)!=0){
                //本应有一步动作：查询it2是it1哪一个规则的first集元素
                string v= &t.vn;//将it1转化为string类型
                vector<string> Trule;
                Trule=rule.at(v.substr(0,1));
                
                //剔除空产生式
                int T=0;
                for(vector<string>::iterator it=Trule.begin();T<Trule.size();it++,T++){
                    if(*it==" ")
                        Trule.erase(it);
                }
                
                if(Trule.size()>1){
                    for(int ii=0;ii<Trule.size();ii++){
                        //空动作不管
                        if(Trule[ii]==" ")
                            continue;
                        //产生式的第一个即为vt，则必定为其first集
                        if(Trule[ii].find(t.vt)!=string::npos){
                            action=Trule[ii];
                            break;
                        }

                    }
                }
                else action = rule.at(v.substr(0,1))[0];//直接拿第一个规则，因为第二个规则即末尾规则肯定为it1->ε
                table.insert(make_pair(t, action));
                
            }
            //如果it2是it1的follow集元素:it2∈follow(it1),且若ε ∈ FIRST(it1)
            else if(first.at(t.vn).count(' ')!=0&&follow.at(t.vn).count(t.vt)!=0){
                action="pop";
                table.insert(make_pair(t, action));
            }
            //对应分析表中的空白，即出错
            else{
                action="-1";
                table.insert(make_pair(t, action));
            }
        }//横轴遍历Vt
        
        //纵轴添加一个‘#’
        point t1;
        t1.vn=*it1;
        t1.vt='#';
        //添加条件：Vn->ε 且 ‘#’ ∈ follow(Vn)
        if(first.at(t1.vn).count(' ')!=0&&follow.at(t1.vn).count(t1.vt)!=0){
            action="pop";
            table.insert(make_pair(t1, action));
        }
        //对应分析表中的空白，即出错
        else{
            action="-1";
            table.insert(make_pair(t1, action));
        }
    }//纵轴遍历Vn
}

bool scanner(){
    FILE *fp=fopen("/Users/jiahaonan/Desktop/compile/in2.0.txt", "r");
    //读二元组
    while(!feof(fp)){
        file_tuple t;
        fscanf(fp, "%d %c",&t.code,&t.symbol);
        file_text.push_back(t);
    }
    file_text.pop_back();
    //结尾加一个#
    file_tuple t;
    t.symbol='#';
    t.code=-1;
    file_text.push_back(t);
    //分析栈
    stack<char> analyse;
    analyse.push('#');
    analyse.push(start);//加入文法开始符号
    
    //输出
    cout<<"*******LL(1)*********"<<endl;
    for(int i=0;i<file_text.size();i++){
        cout<<file_text[i].code<<" "<<file_text[i].symbol<<endl;
    }
    cout<<"*********************"<<endl;
    
    //开始分析
    int k=0;//记录输入串下标
    char stack_top;//记录栈顶元素
    char str_ele;//记录输入串最左元素
    while(!analyse.empty()){
        stack_top=analyse.top();
        str_ele=file_text[k].symbol;
        
        //i==i
        if(Vt.count(stack_top)!=0||stack_top=='#'){
            if(stack_top==str_ele){
                analyse.pop();
                k++;
                continue;
            }
            else{
                return false;
            }
        }
        
        //去规则表table中提取对应规则
        point axis;
        axis.vn=stack_top;
        axis.vt=str_ele;
        string current_rule=table.at(axis);
        
        //错误
        if(current_rule=="-1"){
            return false;
        }
        //Vn->ε
        else if(current_rule=="pop"){
            analyse.pop();
        }
        //规则反向入栈
        else{
            analyse.pop();
            char* temp=(char*)current_rule.c_str();
            int len=(int)current_rule.size();
            for(int j=len-1;j>=0;j--){
                analyse.push(temp[j]);
            }
        }
    }
    return true;
}


int main(int argc, const char * argv[]) {
    init();
    make_first();
    make_follow();
    make_table();

    if(scanner()){
        printf("successful!\n");
    }
    else{
        printf("fail!\n");
    }

    return 0;
}
