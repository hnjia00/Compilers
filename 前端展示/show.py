import os
from tkinter import *
from tkinter import messagebox
import SLR

root = Tk()
root.title("前端展示")
root.geometry('400x600')

title = Label(root,text="编译前端",font=("手扎体-简",30))
title.place(x=120,y=30)


tip = Label(root,text="你可以在此输入代码",font=("手扎体-简",18))
tip.place(x=100,y=110)

code = Entry(root,width=18)
code.place(x=100,y=135)

def compile_confirm(event):
    if(len(code.get())==0):
        a = messagebox.showinfo("","您没有输入任何语句")
        return

    f = open("in.txt", 'w')
    var = code.get()
    f.write(code.get())
    f.write('\n')

word_show = Text(root,height=6,width=30)
word_show.place(x=75,y=235)
def print_confirm1(event):
    word_ans = ""
    os.system("word_analyse")
    with open("out.txt",'r') as f:
        word_ans += f.read()

    word_show.delete('1.0', 'end')
    word_show.insert(INSERT,word_ans)

langu_show = Text(root,height=10,width=30)
langu_show.place(x=75,y=365)
def print_confirm2(event):
    langu_ans = ""
    SLR.test()
    with open("out1.txt",'r') as f:
        langu_ans += f.read()

    langu_show.delete('1.0', 'end')
    langu_show.insert(INSERT,langu_ans)


btn_compile = Button(root, text=" 词法分析 ")
btn_compile.place(x=140,y=180)
btn_compile.bind("<Button-1>",compile_confirm)

btn_print1 = Button(root, text=" 打印结果 ")
btn_print1.place(x=140,y=205)
btn_print1.bind("<Button-1>",print_confirm1)

btn_compile1 = Button(root, text=" 语法分析 ")
btn_compile1.place(x=140,y=335)
btn_compile1.bind("<Button-1>",print_confirm2)


root.mainloop()