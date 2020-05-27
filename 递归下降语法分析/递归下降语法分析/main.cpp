//
//  main.cpp
//  递归下降语法分析
//
//  Created by 贾浩男 on 2019/4/21.
//  Copyright © 2019年 贾浩男. All rights reserved.
//

#include <iostream>
#include <cstdio>
#include <cctype>
#include <vector>
using namespace std;

bool match(int num);
bool S();
bool E();
bool E_1();
bool T();
bool T_1();
bool F();
bool A();
bool M();
bool V();
/*
S→V=E
E→TE′
E′→ATE′|ε
T→FT′
T′→MFT′|ε
F→ (E)|i
A→+|-
M→*|/
V→i
*/

//记录词法分析器生成的二元组
typedef struct {
    int num;
    string token;
}two;
vector<two> text;
int item=0;//全局下标

bool match(int num){
    if(text[item].num==num){
        item++;
        return true;
    }
    return false;
}


bool S(){
    /*
    S->V=E
     */
    if(V()){
        //18:'='
        if(match(18)){
            if(E()){
                return true;
            }
        }
    }
    return false;
}

bool E(){
    /*
     E→TE′
     */
    if(T()){
        if(E_1()){
            return true;
        }
    }
    return false;
}

bool E_1(){
    /*
     E′→ATE′|ε
     */
    if(A()){
        if(T()){
            if(E_1()){
                return true;
            }
        }
    }
    // follow集
    // 匹配follow需要回滚
    // 28:')' -1:'#'
    else if(match(28)||match(-1)){
        item--;
        return true;
    }
    return false;
}

bool T(){
    /*
     T→FT′
     */
    if(F()){
        if(T_1()){
            return true;
        }
    }
    return false;
}

bool T_1(){
    /*
    T′→MFT′|ε
     */
    if(M()){
        if(F()){
            if(T_1()){
                return true;
            }
        }
    }
    // follow集
    // 匹配follow需要回滚
    // 19:'+' 21:'-' 28:')' -1:'#'
    else if(match(19)||match(21)||match(28)||match(-1)){
        item--;
        return true;
    }
    return false;
}

bool F(){
    /*
     F→ (E)|i
     */
    //27:'('
    if(match(27)){
        if(E()){
            //28:')'
            if(match(28)){
                return true;
            }
        }
    }
    // 10:'id' 用户定义标识符
    else if(match(10)){
        return true;
    }
    return false;
}

bool A(){
    /*
     A→+|-
     */
    //19:'+' 21:'-'
    if(match(19)||match(21)){
        return true;
    }
    return false;
}

bool M(){
    /*
     M→*|/
     */
    //23:'*' 24:'/'
    if(match(23)||match(24)){
        return true;
    }
    return false;
}

bool V(){
    /*
     V→i
     */
    // 10:'id' 用户定义标识符
    if(match(10)){
        return true;
    }
    return false;
}

int main(int argc, const char * argv[]) {
   
    FILE *fp=fopen("/Users/jiahaonan/Desktop/compile/in2.0.txt", "r");
    while(!feof(fp)){
        int num;
        char tok[32]=" ";
        fscanf(fp, "%d %s",&num,tok);
        two temp;
        temp.num=num;
        temp.token=tok;
        text.push_back(temp);
    }
    //去掉由feof(fp)引起的结尾重复
    text.pop_back();
    //结束标志
    two temp;
    temp.num=-1;
    temp.token="#";
    text.push_back(temp);
    //输出
    cout<<"*********************"<<endl;
    for(int i=0;i<text.size();i++){
        cout<<text[i].num<<" "<<text[i].token<<endl;
    }
    cout<<"*********************"<<endl;
    //编译结果
    if(S()){
        cout<<"compile successful!"<<endl;
    }
    else{
        cout<<"compile wrong!"<<endl;
    }

    return 0;
}
