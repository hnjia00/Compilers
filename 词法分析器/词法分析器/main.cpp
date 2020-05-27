//
//  main.cpp
//  词法分析器
//
//  Created by 贾浩男 on 2019/4/3.
//  Copyright © 2019年 贾浩男. All rights reserved.
//

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctype.h>
#include <map>

#define ID 10    //identofier：字母打头的字母数字串
#define INT 11   //整数
#define LT 12    // <
#define LE 13    // <=
#define EQ 14   // ==
#define NE 15   // != <>
#define GT 16   // >
#define GE 17   // >=
#define ASSIGN 18 // =
#define ADD 19  // +
#define ADD1 20  // ++
#define SUB 21  // -
#define SUB1 22  // --
#define MUL 23  // *
#define DIV 24  // /
#define COMMENT 25  // /*多行注释+//单行注释
#define SEMIC 26    //  ;
#define LB 27   // (
#define RB 28   //  )
#define NOT 29   //  !
#define AND 30   //  &&
#define OR 31   //  ||
#define COMMA 32 //  ,
#define LCB 33   // {
#define RCB 34   //  }
#define ADDE 35   //  +=
#define SUBE 36 //  -=
#define MULE 37   // *=
#define DIVE 38   //  /=
#define MULE 37   // *=
#define DIVE 38   //  /=
#define MR 39   // >>
#define ML 40   //  <<

using namespace std;
const int maxn = 256+10;

void test();
void init_map();
int lookup(char *tok);
void out(int c,string tok);
void report_error(int line,string tips);
void scanner(FILE *fp);


char token[maxn];  //存放单词词文中的字符

map<int, string> reserved;  //保留字映射
map<int, string> word;  //助记符映射
void init_map(){
    reserved[1]="void";
    reserved[2]="if";
    reserved[3]="else";
    reserved[4]="for";
    reserved[5]="while";
    reserved[6]="int";
    reserved[7]="float";
    reserved[8]="double";
    reserved[9]="return";
    
    word[10]="ID";
    word[11]="INT";
    word[12]="LT";
    word[13]="LE";
    word[14]="EQ";
    word[15]="NE";
    word[16]="GT";
    word[17]="GE";
    word[18]="ASSIGN";
    word[19]="ADD";
    word[20]="ADD1";
    word[21]="SUB1";
    word[22]="SUB1";
    word[23]="MUL";
    word[24]="DIV";
    word[25]="COMMENT";
    word[26]="SEMIC";
    word[27]="LB";
    word[28]="RB";
    word[29]="NOT";
    word[30]="AND";
    word[31]="OR";
    word[32]="COMMA";
    word[33]="LCB";
    word[34]="RCB";
    word[35]="ADDE";
    word[36]="SUBE";
    word[37]="MULE";
    word[38]="DIVE";
    word[39]="MR";
    word[40]="ML";
    
}

//以token中的字符串查找保留字表，查到则返回类别码给c，否则c=0
int lookup(char *tok){
    for(map<int,string>::iterator iter=reserved.begin();iter!=reserved.end();iter++){
        if(iter->second==tok)
            return iter->first;
    }
    return 0;
        
}

void out(int c,string tok){
    // 保留字只输出编码
    if(tok == "reserved")
        cout<<c<<" "<<reserved.at(c)<<endl;
    //ID和int输出
    else if(c==10||c==11){
        cout<<"i"<<" "<<tok<<endl;
    }
    //注释(不)输出
    else if(c==25){
        //return;
        cout<<"COMMENT "<<tok<<endl;
    }
    else{
        cout<<tok<<endl;
    }
}

void report_error(int line,string tips){
    cout<<"[Error] at line "<<line<<": "<<tips<<endl;
}

void scanner(FILE *fp){
    char ch;
    int i,c;
    int line=1; //记录行数
    
    while (!feof(fp)) {
    ch = fgetc(fp);
    //UTF-8编码结束标志
    if(ch=='\377')
        return;
    
    // 字母打头，则为标识符
    if(isalpha(ch)){
        token[0]=ch;
        ch=fgetc(fp);
        i=1;

        while (isalnum(ch)||ch=='_') { //ch是数字或是字母
            token[i++]=ch;
            ch=fgetc(fp);
        }
        //用户定义标识符不能超过32位
        if(i>=32){
            string tips="Too long identifier! ";
            report_error(line,tips);
            
            fseek(fp, -1, 1); // ch既不是数字也不是字母，回退一步
            continue;
        }
        
        token[i]='\0';
        fseek(fp, -1, 1); // ch既不是数字也不是字母，回退一步
        c=lookup(token);
        if(c==0)
            out(ID, token); // 用户定义
        else
            out(c, "reserved"); // 保留字
    }
    
    // 数字打头，无符号数
    else if(isdigit(ch)){
        token[0]=ch;
        ch=fgetc(fp);
        i=1;
        while (isdigit(ch)) {
            token[i++]=ch;
            ch=fgetc(fp);
        }
        token[i]='\0';
        fseek(fp, -1, 1); // 数字截取完，回退一步
        out(INT, token);    // 整数
    }
    
    //空格、tab不做处理
    else if(ch == ' '||ch=='\t')
        continue;
    
    //统计行数
    else if(ch == '\n')
        line++;
    
    // 判断运算符和分隔符等的组合情况
    else{
        switch (ch) {
            case '<':{
                ch = fgetc(fp);
                if(ch=='=')
                    out(LE, "<=");
                else if(ch=='>')
                    out(NE, "<>");
                else if(ch == '<')
                    out(ML, "<<");
                else{
                    fseek(fp, -1, 1);   // 其仅代表<，后面没有更多
                    out(LT,"<");
                }
                break;
            }
                
            case '=': {
                ch = fgetc(fp);
                if(ch=='=')
                    out(EQ, "==");  // 相等
                else{
                    fseek(fp, -1, 1);
                    out(ASSIGN, "="); //赋值
                }
                break;
            }
                
            case '>':{
                ch = fgetc(fp);
                if(ch=='=')
                    out(GE, ">=");
                else if(ch=='>'){
                    out(MR, ">>");
                }
                else{
                    fseek(fp, -1, 1);   // 其仅代表>，后面没有更多
                    out(GT,">");
                }
                break;
            }
            
            case '+':{
                ch = fgetc(fp);
                if(ch=='+')
                    out(ADD1, "++");     // ++
                else if(ch=='=')
                    out(ADDE,"+=");     // +=
                else{
                    fseek(fp, -1, 1);   // +
                    out(ADD,"+");
                }
                break;
            }
                
            case '-':{
                ch = fgetc(fp);
                if(ch=='-')
                    out(SUB1, "--");     // --
                else if(ch=='=')
                    out(SUBE,"-=");     // -=
                else{
                    fseek(fp, -1, 1);   // -
                    out(SUB,"-");
                }
                break;
            }
            
            case '*':{
                ch = fgetc(fp);
                if(ch=='+')
                    out(MULE, "*=");    // *=
                else{
                    out(MUL, "*");  // *
                    fseek(fp, -1, 1);
                }
                break;
            }
            
            case '/':{
                ch = fgetc(fp);
                // /* */多行注释
                if(ch=='*'){
                    char ch1,ch2;
                    bool flag=false;
                    ch1 = fgetc(fp);
                    ch2 = fgetc(fp);
                    string w="";
                    while (!feof(fp)) {
                        if(ch1=='*'&&ch2=='/'){
                            flag=true;
                            break;
                        }
                        else{
                            w=w+ch1;
                            ch1=ch2;
                            ch2=getc(fp);
                            
                        }
                    }
                    if(flag)out(COMMENT, w);
                    else report_error(line,"Unlimited comment of /*");
                
                }
                // //单行注释
                else if(ch=='/'){
                    char ch1;
                    ch1 = fgetc(fp);
                    string w="";
                    while (!feof(fp)) {
                        if(ch1=='\n'){
                            line++;
                            break;
                        }
                        else{
                            w=w+ch1;
                            ch1=getc(fp);
                        }
                    }
                    out(COMMENT, w);
                }
                // *=
                else if(ch=='='){
                    out(DIVE, "/=");
                }
                // / 除
                else{
                    fseek(fp, -1, 1);
                    out(DIV,"/");
                }
                break;
            }
            
            case '!':{
                ch = fgetc(fp);
                // != 不等于
                if(ch=='='){
                    out(NE, "!=");
                    
                }
                // ！ 非
                else{
                    fseek(fp, -1, 1);
                    out(NOT,"!");
                }
                break;
            }
            
            // 逗号
            case ',':{
                out(COMMA, ",");
                break;
            }
            // 分号
            case ';':{
                out(SEMIC, ";");
                break;
            }
            
            //花括号
            case '{':{
                out(LCB, "{");
                break;
            }
            //圆括号
            case '(':{
                out(LB, "(");
                break;
            }
            
            case '}':{
                out(RCB, "}");
                break;
            }
            case ')':{
                out(RB, ")");
                break;
            }
            
            //逻辑运算符
            case '&':{
                ch = fgetc(fp);
                // &&
                if(ch=='&'){
                    out(AND, "&&");
                }
                else{
                    fseek(fp, -1, 1);
                    string tips="Unknown charactor: ";
                    tips = tips+"'"+ch+"'";
                    report_error(line,tips);
                }
                break;
            }
            
            case '|':{
                ch = fgetc(fp);
                // ||
                if(ch=='|'){
                    out(OR, "||");
                }
                else{
                    fseek(fp, -1, 1);
                    string tips="Unknown charactor: ";
                    tips = tips+"'"+ch+"'";
                    report_error(line,tips);

                }
                break;
            }
            
            default:{
                string tips="Unknown charactor: ";
                tips = tips+"'"+ch+"'";
                report_error(line,tips);
                break;
            }
        }
    }
    }
    
    return ;
}


void test(){
    init_map();
    FILE *fp=fopen("/Users/jiahaonan/Desktop/大三下/编译原理/前端展示/in.txt", "r");
    FILE *fp1 = freopen("/Users/jiahaonan/Desktop/大三下/编译原理/前端展示/out.txt", "w+", stdout);
    scanner(fp);

}

int main(int argc, const char * argv[]) {
    test();
    
    return 0;
}
