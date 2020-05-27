//
//  LL1.h
//  LL(1)语法分析
//
//  Created by 贾浩男 on 2019/4/21.
//  Copyright © 2019年 贾浩男. All rights reserved.
//

#ifndef LL1_h
#define LL1_h

#include <iostream>
#include <cstdio>
#include <cctype>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <stack>

using namespace std;

// 记录产生式规则 string->string|string...
map<string,vector<string>> rule;

// 记录各个符号对应的first和follow集
map<char,set<char>> first;
map<char,set<char>> follow;

// 记录Vt、Vn
set<char> Vt;
set<char> Vn;

//文法开始符号
char start;

/*LL1分析表
  table[char char]=string
  Vn 纵轴
  Vt 横轴
 */
typedef struct Point{
    char vn;
    char vt;
    bool operator <(const Point &x)const
    {
        return vt<x.vt||vn<x.vn;
    }
}point;
map<point,string>table;

//文件二元组结构体
typedef struct {
    int code;//编码
    char symbol;//符号
}file_tuple;
//二元组内容存储在file_text
vector<file_tuple> file_text;

#endif /* LL1_h */
