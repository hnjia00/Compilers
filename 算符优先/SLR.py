
class SLR:
    def __init__(self, start, Vn, Vt, rule):
        # 初始化类元素
        self.start = start
        self.Vt = Vt
        self.Vt.append('#')
        self.Vn = Vn
        self.rule = rule
        self.point = '.'
        self.V = Vn + Vt

        # 拓广文法
        self.rule['S'] = start + "#"
        self.Vn.append('S')
        self.start = 'S'

        # 获得first集
        self.first = {vn:set() for vn in self.Vn}
        self.get_first()

        # 获得follow集
        self.follow = {vn: set() for vn in self.Vn}
        self.get_follow()

        # 生成有效项目集
        #self.C = {k : dict() for k in range(12)}
        self.C = {}
        self.tC = [] # 终态项目集
        self.get_C()

        # 生成action表
        self.r = {}  # 记录规约序号，辅助生成action表中的r项
        self.get_r()
        self.action = dict()
        self.get_action()

        # 生成Goto表
        self.goto = dict()
        self.get_goto()

    def get_first(self):
        rule1 = self.rule.copy()
        item = rule1[self.start]
        rule1[self.start] = [item]
        '''
            若X∈Vn 有X→aα , (a ∈Vt )或/和X→ε
            则 a或 /和ε ∈ FIRST(x)
        '''
        for left in rule1.keys():
            for right in rule1[left]:
                if(right[0] in self.Vt):
                    self.first[left].add(right[0])

        '''
           对X→Y1Y2.......Yk(且Y1 ∈Vn), 反复使用以下 直到每一个FIRST(X)不再增大为止.
            i   若Y1 ∈Vn
                则把FIRST(Y1 )\{ε}元素加入FIRST(X)中
            ii  若Y1、Y2、......Y i-1 ∈Vn (2≤i ≤k) 且ε ∈FIRST(Y j) (1≤j ≤i-1)
                则把FIRST(Yi )\{ε}元素加入FIRST(x)中 
            iii 若Y1、Y2、......Yk ∈Vn
                且ε ∈FIRST(Y j) (1≤j ≤k) 则把ε元素加入FIRST(x)中
        '''
        size1 = 0
        size2 = 0
        for key in self.first:
            for item in self.first[key]:
                size1 += len(item)
        while(True):
            for left in rule1.keys():
                for right in rule1[left]:
                    Y1 = right[0]
                    if(Y1 in self.Vn):
                        set1 = self.first[Y1]
                        for item in set1:
                            self.first[left].add(item)

            for key in self.first:
                for item in self.first[key]:
                    size2 += len(item)

            if(size1 != size2):
                size1=size2
                size2=0
                continue
            else:
                break

    def get_follow(self):
        rule1 = self.rule.copy()
        item = rule1[self.start]
        rule1[self.start] = [item]
        # 1. 令# ∈FOLLOW(S) S为文法开始符号
        self.follow[self.start].add('#')

        # 2. 对A→ αBβ, 且β ≠ ε 则将 FIRST(β)\{ε}加入FOLLOW(B)中
        for left in rule1.keys():
            for right in rule1[left]:
                for k in range(len(right)-1):
                    # B:right[k]
                    if(right[k] in self.Vn):
                        # B下一个字符是终结符，则直接加入B的follow集
                        if(right[k+1] in self.Vt):
                            self.follow[right[k]].add(right[k+1])
                        # B下一个字符不是终结符，则将FIRST(β)加入B的follow集
                        elif(right[k+1] in self.Vn):
                            set1 = self.first[right[k+1]]
                            for item in set1:
                                self.follow[right[k]].add(item)

         # 3.反复, 直至每一个FOLLOW(A)不再增大
         # 对A→ αB或A→ αBβ(且ε ∈ FIRST(β))
         # 则FOLLOW(A)中的全部元素加入FOLLOW(B)
        size1=0
        size2=0
        for key in self.follow:
            for item in self.follow[key]:
                size1 += len(item)
        while(True):
            for left in rule1.keys():
                for right in rule1[left]:
                    # B:right[-1] 对A→ αB
                    if(right[-1] in self.Vn):
                        set1 = self.follow[left]
                        for item in set1:
                            self.follow[right[-1]].add(item)

            for key in self.follow:
                for item in self.follow[key]:
                    size2 += len(item)

            if (size1 != size2):
                size1=size2
                size2=0
                continue
            else:
                break

    '''
        开始操作:S为开始符号, S→δ               
        则  S→·δ ∈ C0
    '''
    def op_start(self):
        self.C[0]= {'S':[self.point+self.rule['S']]}

    '''
        闭包操作:closure(Ci)       Ci的闭包        
            ① Ci 的任何项目均属于closure(Ci)        
            ② 若A → α·X β且X∈Vn属于closure(Ci)            
               则X → ·λ 属于closure(Ci)   
               重复,直至 closure(Ci)不再增大.        
            ③ Ci = closure(Ci)
    '''
    def op_closure(self,c_j):
        #closure_i = self.C[i] #dict类型
        closure_i = c_j
        while(True):
            closure_i1=closure_i.copy()
            key_list = list(closure_i.keys())
            for A in key_list:
                for aXb in closure_i[A]:
                    for j in range(len(aXb)-1):
                        if(aXb[j]==self.point and aXb[j+1] in self.Vn):
                            # 如何没有则新建一个key
                            if(aXb[j+1] not in closure_i.keys()):
                                closure_i.update({aXb[j + 1]:[]})
                                key_list.append(aXb[j + 1])
                            for l in rule[aXb[j+1]]:
                                temp = self.point+l
                                if(temp not in closure_i[aXb[j+1]]):
                                    closure_i[aXb[j+1]].append(temp)
            # 重复,直至 closure(Ci)不再增大.
            if(closure_i != closure_i1):
                closure_i1 = closure_i.copy()
            else:
                return closure_i

    '''
        读操作: Go(Ci ,x )     x∈V           
        Go(Ci ,x )=Cj 
        其中: Cj={A→αx·β∣ A→α·x β∈Ci} 
    '''
    def op_go(self,i,x):
        c_j = dict()
        dict_j = self.C[i]

        for A in dict_j.keys():
            for aXb in dict_j[A]:
                for j in range(len(aXb) - 1):
                    if(aXb[j] == self.point and aXb[j+1] == x):
                        if(A not  in c_j.keys()):
                            c_j.update({A:[]})
                        c_j[A].append(aXb[:j]+aXb[j+1]+self.point+aXb[j+2:])
        return c_j

    # 判断C[i]是否为终态
    def judge(self,i):
        dict_i = self.C[i]
        for key in dict_i.keys():
            for value in dict_i[key]:
                if(value[-1]!=self.point):
                    return False
        return True

    def check_in_C(self,closure_j):
        for i in self.C.keys():
            if closure_j == self.C[i]:
                return True
        return False


    def get_C(self):
        # 生成C0={S→ ·δ}
        self.op_start()
        # ∪ {S→ ·δ的闭包操作}
        self.C[0]=self.op_closure(self.C[0])


        '''
        重复以下过程,直至C不再增大为止.          
            Ci读操作,生成Cj1,Cj2,…….Cjn               
            Cj1,Cj2,…….Cjn闭包操作 
            (若其中某项目集已经存在就略去)   
        '''
        i = 0
        j = 1
        while(True):
            # 判断C[i]是否为终态
            if (i == j):
                break
            if(self.judge(i)==True):
                self.tC.append(i)
                i=i+1
                continue
            for v in self.V:
                c_j = self.op_go(i,v)
                if(len(c_j)!=0):
                    #self.C[j] = c_j
                    closure_j = self.op_closure(c_j)
                    flag = True
                    # for key in self.C.keys():
                    #     if(closure_j == self.C[key]):
                    #         flag = False
                    if self.check_in_C(closure_j):
                        flag=False

                    if flag:
                        self.C[j]=closure_j
                        j = j + 1
            if (i<j):
                i=i+1

            else:
                break

    # 用于建分析表的Go函数返回状态集的编号
    def GO(self,i,x):
        c_j = dict()
        dict_j = self.C[i]

        for A in dict_j.keys():
            for aXb in dict_j[A]:
                for j in range(len(aXb) - 1):
                    if (aXb[j] == self.point and aXb[j + 1] == x):
                        if (A not in c_j.keys()):
                            c_j.update({A: []})
                        c_j[A].append(aXb[:j] + aXb[j + 1] + self.point + aXb[j + 2:])
        # 别忘了闭包
        c_j = self.op_closure(c_j)
        for key in self.C.keys():
            if self.C[key]==c_j:
                return key

        return -1

    # 生成辅助规约映射self.r
    def get_r(self):
        rule1 = self.rule.copy()
        item = rule1[self.start]
        rule1[self.start]=[item]
        i = 0
        for left in rule1.keys():
            for right in list(rule1[left]):
                self.r[i] = {left: right}
                i = i+1

    # 构造分析表的action部分
    def get_action(self):
        # 若GO(Ci, a)=Cj  a∈Vt, 置ACTION(i,a)=Sj
        for i in self.C.keys():
            for a in self.Vt:
                j = self.GO(i,a)
                if(j!=-1):
                    self.action[(i,a)]='S'+str(j)

        # 若A→α·∈Ci , 且 a∈FOLLOW(A)
        #  a∈Vt 置ACTION(i,a)=rj (A → α为第 j个产生式)
        for i in self.C:
            for A in self.C[i].keys():
                for item in self.C[i][A]:
                    # 项目最后一个为点 '·'
                    if(item[-1] == self.point):
                        for a in self.follow[A]:
                            # 找到对应的规约产生式
                            for j in self.r.keys():
                                temp = {A:item[:-1]}
                                if(self.r[j] == temp):
                                    self.action[(i,a)]='r'+str(j)


        # 若S→δ·∈ Ck ,(S为拓广文法开始符号）
        # 置ACTION(k,#)=acc
        omiga = rule[self.start]+self.point
        for k in self.C:
            for key in self.C[k].keys():
                if(key==self.start and omiga in self.C[k][key]):
                    self.action[(k,'#')]="acc"


    # 构造分析表的goto部分
    def get_goto(self):
        # 若GO(Ci, A)=Cj  A∈Vn, 置GOTO(i,A)=j
        for i in self.C.keys():
            for A in self.Vn:
                j = self.GO(i, A)
                if (j != -1):
                    self.goto[(i, A)] = j

    # SLR（1）法分析
    # symbol: 符号语句 word: 真实语句
    def compile(self, symbol, word, log):
        # 预定义.PLACE变量
        if log == True:
            E_place = []
            F_place = []
            V_place = []
            T_place = []
            t = 0 #temp计数器
            kk = 1#log计数器

        # 补充#
        symbol+="##"

        # 状态栈
        state = ['0']
        # 符号栈
        charater = ['#']

        #输入串下标
        k=0

        while(True):
            print("[状态栈]:",end="")
            print(state)
            print("[符号栈]:",end="")
            print(charater)
            state_now = eval(state[len(state)-1])

            input_ch = symbol[k]

            if ((state_now,input_ch) in self.action.keys()):
                action = self.action[(state_now, input_ch)]
            else:
                action='-1'

            if(action[0]=='S'):
                #if(input_ch != '#'):
                k = k+1
                charater.append(input_ch)
                state.append(action[1:])
                continue

            elif(action[0]=='r'):
                # 规约动作r
                r = {}
                r = self.r[eval(action[1])]
                # 规约长度
                lenth = 0
                # 规约终结符
                substitude = ''
                r_rule=''
                for left in r.keys():
                    substitude = left
                    for right in r[left]:
                        r_rule+=right


                for i in range(len(r_rule)):
                    charater.pop()
                    state.pop()

                # 根据规约规则r_rule决定执行动作, log为分析开关
                if log==True:
                    print("["+str(kk)+"]: "+substitude+" -> "+r_rule)
                    kk = kk+1
                    # ② A →V=E
                    if (substitude == 'A' and r_rule == "V=E"):
                        print("( =, "+E_place.pop()+", ,"+V_place.pop()+" )")

                    # ③ E(1)→E(2)+T
                    elif (substitude == 'E' and r_rule == "E+T"):
                        E1_place = "temp"+str(t)
                        t=t+1
                        print("( +, "+E_place.pop()+", "+T_place.pop()+", "+E1_place+" )")
                        E_place.append(E1_place)

                    # ④ E → T
                    elif (substitude == 'E' and r_rule == "T"):
                        E_place.append(T_place.pop())

                    # ⑤ T(1)→ T(2)*F
                    elif (substitude == 'T' and r_rule == "T*F"):
                        T1_place = "temp"+str(t)
                        t=t+1
                        print("( *, "+T_place.pop()+", "+F_place.pop()+", "+T1_place+" )")
                        T_place.append(T1_place)

                    # ⑥ T → F
                    elif (substitude == 'T' and r_rule == "F"):
                        T_place.append(F_place.pop())

                    # ⑦ F →(E)
                    elif (substitude == 'F' and r_rule == "(E)"):
                        F_place.append(E_place.pop())

                    # ⑧ F → i
                    elif (substitude == 'F' and r_rule == "i"):
                        F_place.append(word[k-1])

                    # ⑨ V →i
                    elif (substitude == 'V' and r_rule == "i"):
                        V_place.append(word[k-1])


                # 新规约终结符进符号栈
                charater.append(substitude)
                new_state = self.goto[(eval(state[len(state)-1]),substitude)]
                state.append(str(new_state))

            elif(action=='acc'):
                return True

            else:
                return False


if __name__ == "__main__":
    # 终结符号集/非终结符号集
    Vn = ['A', 'V', 'E', 'T', 'F']
    Vt = ['+', '-', '*', '/', '(', ')', 'i' , '=']

    # 规则集合
    rule = {
        'A': ["V=E"],
        'E': ["E+T","E-T","T"],
        'T': ["T*F", "T/F", "F"],
        'F': ["(E)","i"],
        'V': ["i"]
    }

    slr = SLR(start='A',Vt=Vt,Vn=Vn,rule=rule)

    test_words = ""
    test_symbol = ""
    f = open("in2.0.txt",'r')
    while(True):
        t = f.readline().strip('\n')
        if(t == ''): break
        test_symbol += t[0]
        test_words += t[-1]

    print(slr.compile(symbol=test_symbol,word=test_words,log=True))

