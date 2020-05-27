//
//  main.cpp
//  文件命名识别
//
//  Created by 贾浩男 on 2019/3/30.
//  Copyright © 2019年 贾浩男. All rights reserved.
//

#include <iostream>
#include <ctype.h>
#include <string>

using namespace std;

int next_state(int current_state,char temp){
    int next=-1;
    switch (current_state) {
        case 1:{
            if(isalpha(temp)){
                next=2;
            }
            else{
                next=-1;
            }
            break;
        }
        case 2:{
            if(isalpha(temp)){
                next=2;
            }
            else if(temp=='.'){
                next=5;
            }
            else if(temp==':'){
                next=3;
            }
            else{
                next=-1;
            }
            break;
        }
        case 3:{
            if(isalpha(temp)){
                next=4;
            }
            else{
                next=-1;
            }
            break;
        }
        case 4:{
            if(isalpha(temp)){
                next=4;
            }
            else if(temp=='.'){
                next=5;
            }
            else{
                next=-1;
            }
            break;
        }
        case 5:{
            if(isalpha(temp)){
                next=6;
            }
            else{
                next=-1;
            }
            break;
        }
        case 6:{
            if(isalpha(temp)){
                next=6;
            }
            else{
                next=-1;
            }
            break;
        }
    }
    return next;
}

bool isfile(string filename){
    int state = 1;
    int i;
    for (i=0; i<filename.length(); i++) {
        state = next_state(state, filename[i]);
        if (state==-1) {
            return false;
        }
    }
    //终态为2、4、6
    if(state==1||state==3||state==5)
        return false;
    return true;
}

int main(int argc, const char * argv[]) {

    while (1) {
        string str;
        cout<<"input: ";
        cin>>str;
        
        if (isfile(str)) {
            cout<<"legal file name~"<<endl<<endl;
        }
        else{
            cout<<"illegal file name!"<<endl<<endl;
        }
        
    }
    return 0;
}
