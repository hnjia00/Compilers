//
//  main.cpp
//  语法分析4-3
//
//  Created by 贾浩男 on 2019/4/20.
//  Copyright © 2019年 贾浩男. All rights reserved.
//

#include <iostream>
#include <cstdio>
using namespace std;

bool program();     //程序
bool statement();   //语句
bool assign();      //赋值语句
bool condition();   //条件语句
bool expression();  //表达式
bool expression_1();//表达式1
bool variable();    //变量

string test;        //记录语句字符串
int item=0;         //记录下标定位

bool match(string token){
    int item1=item;
    string tk;//辅助判断
    while(test[item]!=' '&&test[item]!='\n'){
        tk+=test[item];
        item++;
    }
    if(token==tk)
        return true;
    else{
        item=item1; //匹配失败则回滚
        return false;
    }
}

void nextsym(){
    while(test[item]==' '||test[item]=='\n')
        item++;
}

bool program(){
    /*
     <program> -> begin <statement> end
     */
    if(match("begin")){
        nextsym();
        if(statement()){
            if(match("end")){
                return true;
            }
        }
    }
    return false;
}

bool statement(){
    /*
     <statement> -> <assign> | <condition>
     */
    if(assign()){
        return true;
    }
    else if(condition()){
        return true;
    }
    return false;
}

bool assign(){
    /*
     <assign> -> <variable>:=<statement>
     */
    if(variable()){
        if(match(":=")){
            nextsym();
            if(expression()){
                return true;
            }
        }
    }
    return false;
}

bool condition(){
    /*
     <condition> -> if <expression> then <statement>
     */
    if(match("if")){
        nextsym();
        if(expression()){
            if(match("then")){
                nextsym();
                if(statement()){
                    return true;
                }
            }
        }
    }
    return false;
}

bool expression(){
    /*
     <expression> -> <variable><expression_1>
     */
    if(variable()){
        if(expression_1()){
            return true;
        }
    }
    return false;
}

bool expression_1(){
    /*
     <expression_1> -> +<variable><expression_1> | ε
     */
    int item1=item;
    if(match("+")){
        nextsym();
        if(variable()){
            if(expression_1()){
                return true;
            }
        }
    }
    //match follow集
    if(match("then")||match("end")){
        item=item1;//回滚
        return true;
    }
    return false;
}

bool variable(){
    /*
     <variable> -> i
    */
    if(match("i")){
        nextsym();
        return true;
    }
    return false;
    
}

int main(int argc, const char * argv[]) {
    FILE *fp=fopen("/Users/jiahaonan/Desktop/in.txt", "r");
    while(!feof(fp)){
        test+=fgetc(fp);
    }
    test[test.size()-1]='\0';
    //输出源程序
    cout<<"*********************"<<endl;
    cout<<test;
    cout<<"*********************"<<endl;
    //编译结果
    if(program()){
        cout<<"compile successful!"<<endl;
    }
    else{
        cout<<"compile wrong!"<<endl;
    }
    return 0;
}
