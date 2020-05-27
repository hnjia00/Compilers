class OPG:
    def __init__(self,start,Vn,Vt,rule):
        # 初始化类元素
        self.start=start
        self.Vt=Vt
        self.Vn=Vn
        self.rule=rule
        # 拓广文法
        self.rule['S']="#"+start+"#"
        self.Vn.append('S')

        # 构造firstvt和get_lastVt
        self.get_firstVt()
        self.get_lastVt()

        # 构造优先关系矩阵
        self.get_matrix()

        # 构造辅助规则集: 将非终结符全替换为N
        self.get_assistrule()

        self.Vt.append('#')

    def get_firstVt(self):
        # 初始化firstVt
        self.firstVt = {Vn: set() for Vn in self.Vn}
        # 初始化辅助栈
        stack = []
        # 初始化辅助布尔判断函数
        F = dict()

        # 根据原则①：若有规则U→b…, 或U→Vb…,则b∈FIRSTVT(U)
        # first -> right
        for left in self.firstVt:
            for right in self.rule[left]:
                # U→b…
                if(right[0] in self.Vt):
                    self.firstVt[left].add(right[0])
                    stack.append((left, right[0]))
                    F[(left,right[0])] = True
                # U→Vb…
                elif(len(right)>1 and right[0] in self.Vn and right[1] in self.Vt):
                    self.firstVt[left].add(right[1])
                    stack.append((left, right[1]))
                    F[(left, right[1])] = True

        # 根据原则②：若有规则U→V…,且b ∈FIRSTVT(V)，则b ∈FIRSTVT(U)
        while(len(stack)>0):
            # 弹出栈顶元素,记(V,b)
            V,b = stack.pop()

            for left in self.Vn:
                for right in self.rule[left]:
                    # 对每一个形如U→V…的规则
                    # U即left
                    if(right[0]==V):
                        # 若F(U,b) 为假,变为真,  进STACK栈
                        if((left,b) not in F.keys()):
                            F[(left, b)]=True
                            stack.append((left,b))
                        # 若F(U,b)为真,再循环
                        if(F[(left,b)]==True):
                            continue

        # FIRSTVT(U)={b∣F(U,b)=TRUE}
        for key in F:
            self.firstVt[key[0]].add(key[1])

    def get_lastVt(self):
        # 初始化lastVt
        self.lastVt = {Vn: set() for Vn in self.Vn}
        # 初始化辅助栈
        stack = []
        # 初始化辅助布尔判断函数
        F1 = dict()

        # 根据原则①：若有规则U→…b, 或U→…bV,则b∈LASTVT(U)
        # first -> right
        for left in self.firstVt:
            for right in self.rule[left]:
                # U→…b
                if(right[-1] in self.Vt):
                    self.lastVt[left].add(right[-1])
                    stack.append((left, right[-1]))
                    F1[(left,right[-1])] = True
                # U→…Vb
                elif(len(right)>1 and right[-1] in self.Vn and right[-2] in self.Vt):
                    self.lastVt[left].add(right[-2])
                    stack.append((left, right[-2]))
                    F1[(left, right[-2])] = True

        # 根据原则②：若有规则U→…V,且b ∈ LASTVT(V)，则b ∈ LASTVT(U)
        while(len(stack)>0):
            # 弹出栈顶元素,记(V,b)
            V,b = stack.pop()

            for left in self.Vn:
                for right in self.rule[left]:
                    # 对每一个形如U→…V的规则
                    # U即left
                    if(right[-1]==V):
                        # 若F(U,b) 为假,变为真,  进STACK栈
                        if((left,b) not in F1.keys()):
                            F1[(left, b)]=True
                            stack.append((left,b))
                        # 若F(U,b)为真,再循环
                        if(F1[(left,b)]==True):
                            continue

        # FIRSTVT(U)={b∣F(U,b)=TRUE}
        for key in F1:
            self.lastVt[key[0]].add(key[1])

    def get_matrix(self):
        # 用字典映射来记录算符优先关系（横，纵）-> '>、<、='
        self.matrix = dict()
        # left -> right
        for left in self.Vn:
            for right in self.rule[left]:
                for i in range(len(right)-1):
                    # ...ab... : Xi = Xi+1
                    if(right[ i ] in self.Vt and right[i + 1] in self.Vt):
                        self.matrix[(right[i], right[i + 1])] = '='

                    # ...aVb... : Xi = Xi+2
                    if(i<len(right)-2 and right[i] in self.Vt and right[i+2] in self.Vt):
                        self.matrix[(right[i], right[i + 2])] = '='

                    # ...aU... : a < firstvt(U)
                    if(right[i] in self.Vt and right[i+1] in self.Vn):
                        for b in self.firstVt[right[i+1]]:
                            self.matrix[(right[i], b)] = '<'

                    # ...Ub... : lastvt(U) > b
                    if(right[i] in self.Vn and right[i+1] in self.Vt):
                        for a in self.lastVt[right[i]]:
                            self.matrix[(a, right[i+1])] = '>'

        # 处理：'#'
        # '#' < firstvt(S)
        # '#' < lastvt(S)
        # '(#,#)'='='

        for item in self.firstVt[self.start]:
            self.matrix[('#',item)]='<'
        for item in self.lastVt[self.start]:
            self.matrix[(item,'#')]='>'
        self.matrix[('#','#')]='='

    # 构造辅助规则集，将非终结符全替换为N
    def get_assistrule(self):
        self.rule1 = {key : list() for key in self.rule.keys()}
        for key in self.rule.keys():
            for right in self.rule[key]:
                right1=""
                for c in range(len(right)):
                    if (right[c] in self.Vn):
                        right1+='N'
                    else:
                        right1+=right[c]
                self.rule1[key].append(right1)

    # 判断最左素短语能否规约
    def can_statute(self,test):
        for key in self.rule1:
            for right in self.rule1[key]:
                if(test==right):
                    return True
        return False

    def compile(self,test):
        # 分析栈
        analyse_stack = ['#']

        i = 0
        j = 0 # j+1到i记录最左素短语
        k = -1 # i记录输入串下标

        while(True):
            k = k+1
            # 数组越界则为出错
            if(k>len(test) or i>len(analyse_stack) or j>len(analyse_stack)):
                print("[error] 输入串格式不满足算符文法格式")
                return False
            R = test[k] # R记录输入串下一个要输入的字符

            while(True):
                # 寻找非终结符
                if(analyse_stack[i] in self.Vt):
                    j = i
                else:
                    j = i-1

                # S(j),R 无优先关系
                if((analyse_stack[j],R) not in self.matrix.keys()):
                    print("[error]  \'" + analyse_stack[j] + "\',\'" + R + "\' 无优先关系")
                    return False

                # S(j)<R 或 S(j)=R
                if(self.matrix[(analyse_stack[j],R)]!='>'):
                    i = i+1
                    if(i>=len(analyse_stack)):
                        analyse_stack.append(R)
                    else:
                        #analyse_stack.append(R)
                        analyse_stack[i]=R
                    break

                # S(j)>R
                while(True):
                    # Q=S(j)
                    Q = analyse_stack[j]
                    j = j-1
                    if(analyse_stack[j] not in self.Vt):
                        j = j-1

                    # S(j),Q 无优先关系
                    if ((analyse_stack[j], Q) not in self.matrix.keys()):
                        print("[error]  \'" + analyse_stack[j] + "\',\'" + Q + "\' 无优先关系")
                        return False
                    # S(j)< Q >R
                    temp=self.matrix[(analyse_stack[j],Q)]
                    if(temp=='<'):
                        break

                # 检验最左素短语是否能规约
                item = j+1
                the_left=""
                while(item<=i):
                    the_left += analyse_stack[item]
                    item+=1
                if(self.can_statute(the_left)):
                    for c in range(j+2,i+1):
                        analyse_stack.pop()
                    i = j+1
                    analyse_stack[i] = 'N'
                else:
                    print("无法规约："+the_left)
                    return False

                # 成功
                if(i==1 and R=='#'):
                    return True

# 终结符号集/非终结符号集
Vn = ['E','T','F']
Vt = ['+','-','*','/','(',')','i']

# 规则集合
rule = {
    'E':["E+T","E-T","T"],
    'T':["T*F","T/F","F"],
    'F':["(E)","i"]
}

# test = ""
# f = open("in2.0.txt",'r')
# while(True):
#     t = f.readline().strip('\n')
#     if(t == ''): break
#     test += t[-1]

test = "i+ii-i"
my_OPG = OPG(start='E',Vt=Vt,Vn=Vn,rule=rule)

print("*"*9+"OPG"+"*"*8+'\n'+"input: "+test)
if(my_OPG.compile(test+'#')==True):
    print("compile successful!")
else:
    print("compile failed")
print("*"*20)