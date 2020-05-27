//
//  main.cpp
//  无符号数识别
//
//  Created by 贾浩男 on 2019/3/30.
//  Copyright © 2019年 贾浩男. All rights reserved.
//

#include <iostream>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

int w=0;//整数累加器（初值为0）
int p=0;//指数累加器（初值为0）
int n=0;//十进小数累加器（初值为0）
int e=1;//十进指数的符号（初值为1， 遇负号为-1）

//获取下一个状态及中间量计算结果
int next_state(int currentstate,char temp){
    int next=-1;
    
    switch (currentstate) {
        case 0:{
            if(isdigit(temp)){
                n=0;
                p=0;
                e=1;
                w=(temp-'0');
                next=1;
            }
            else if(temp=='.'){
                n=0;
                p=0;
                e=1;
                w=0;
                next=3;
            }
            else
                next=-1;
            break;
        }
        
        case 1:{
            if(isdigit(temp)){
                w=w*10+(temp-'0');
                next=1;
            }
            else if(temp=='.'){
                next=2;
            }
            else if(temp=='E'||temp=='e'){
                next=4;
            }
            else
                next=-1;
            break;
        }
            
        case 2:{
            if(isdigit(temp)){
                w=w*10+(temp-'0');
                n=n+1;
                next=2;
            }
            else if(temp=='E'||temp=='e'){
                next=4;
            }
            else
                next=-1;
            break;
        }
            
        case 3:{
            if(isdigit(temp)){
                n=n+1;
                w=w*10+(temp-'0');
                next=2;
            }
            else
                next=-1;
            break;
        }
            
        case 4:{
            if(isdigit(temp)){
                p=p*10+(temp-'0');
                next=6;
            }
            else if(temp=='+'){
                next=5;
            }
            else if(temp=='-'){
                e=-1;
                next=5;
            }
            else
                next=-1;
            break;
        }
            
        case 5:{
            if(isdigit(temp)){
                p=p*10+(temp-'0');
                next=6;
            }
            else
                next=-1;
            break;
        }
        
        case 6:{
            if(isdigit(temp)){
                p=p*10+(temp-'0');
                next=2;
            }
            else
                next=-1;
            break;
        }
    }
    
    return next;
}


//判断是否为无符号数
bool isnum(string num){
    
    int state = 0;
    int i=0;
    for(;i<num.size();i++){
        char temp = num[i];
        state = next_state(state, temp);
        if(state==-1)
            return false;
    }
    //没有到达终态1、2、6
    if(state==3||state==4||state==5)
        return false;
    return true;
}


int main(int argc, const char * argv[]) {
    while(1){
        string num;
        cout<<"input:";
        cin>>num;
    
        if(isnum(num)){
            cout<<"it is an unsigned number～\n";
            if(e*p-n!=0&&w!=0)
                cout<<"the number is:"<<w<<"E"<<e*p-n<<endl<<endl;
            else
                cout<<"the number is:"<<w<<endl<<endl;
        }
        else{
            cout<<"it isn't an unsigned number!"<<endl<<endl;
        }
    }
    
    return 0;
}
