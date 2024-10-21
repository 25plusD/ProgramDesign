//DPLL求解器
//分别采用了DLIS VSIDS MOMS 和暴力求解四种策略
//数据通过临接链表存储

#include "Global.h"

extern int VarNum;
extern int ClauseNum;
extern int flag;
struct VariableInfo variable_info[MaxVarNum]; // 存储变量信息的数组

//判断LIST中是否有空子句
status IsEmptyClause(HeadNode* LIST) {
    HeadNode* PHead = LIST;
    while (PHead != nullptr) {
        if (PHead->Num == 0)
            return TRUE;
        PHead = PHead->down;
    }
    return FALSE;
}
//寻找单子句,返回单子句的指针
HeadNode* IsSingleClause(HeadNode* Pfind) {
    while (Pfind != nullptr) {
        if (Pfind->Num == 1)
            return Pfind;
        Pfind = Pfind->down;
    }
    return nullptr;
}
//复制整个LIST
HeadNode* Duplication(HeadNode* LIST) { //此处检验传参正常，开始检查复制有无逻辑错误
    HeadNode* SrcHead = LIST;
    HeadNode* ReHead = new HeadNode;//新链表的头节点
    ReHead->Num = SrcHead->Num;//复制第一个头节点
    HeadNode* Phead = ReHead;//Phead创建头节点
    DataNode* ReData = new DataNode;//新链表的数据节点
    DataNode* FirstSrcData = SrcHead->right;//用于创建第一行的第一个数据节点
    ReData->data = FirstSrcData->data;//新链表的第一个数据节点的数值
    Phead->right = ReData;
    for (FirstSrcData = FirstSrcData->next;FirstSrcData != nullptr; FirstSrcData = FirstSrcData->next) {//第一行链表复制完成
        DataNode* NewDataNode = new DataNode;
        NewDataNode->data = FirstSrcData->data;
        ReData->next = NewDataNode;
        ReData = ReData->next;
    }
    //此下行数节点的复制 >=2th
    for (SrcHead = SrcHead->down; SrcHead != nullptr; SrcHead = SrcHead->down) {
        HeadNode* NewHead = new HeadNode;
        DataNode* NewData = new DataNode;
        NewHead->Num = SrcHead->Num;
        Phead->down = NewHead;
        Phead = Phead->down;
        DataNode* SrcData = SrcHead->right;
        NewData->data = SrcData->data;
        Phead->right = NewData;//第一个数据节点
        for (SrcData = SrcData->next;SrcData != nullptr; SrcData = SrcData->next) {//此行剩下的数据节点
            DataNode* node = new DataNode;
            node->data = SrcData->data;
            NewData->next = node;
            NewData = NewData->next;
        }
        NewData->next = nullptr;
    }
    Phead->down = nullptr;

    return ReHead;
}
//增加单子句
HeadNode* ADDSingleClause(HeadNode* LIST, int Var) { //所加的单子句位于链表的头
    HeadNode* AddHead = new HeadNode;
    DataNode* AddData = new DataNode;
    AddData->data = Var;
    AddData->next = nullptr;
    AddHead->right = AddData;
    AddHead->Num = 1;
    AddHead->down = LIST;
    LIST = AddHead;
    return LIST;
}
//删除数据结点
void DeleteDataNode(int temp, HeadNode*& LIST) {
    DataNode* p;
    HeadNode* pHeadNode0=LIST;
    for (HeadNode* pHeadNode = LIST; pHeadNode != nullptr; pHeadNode = pHeadNode0) {
        pHeadNode0 = pHeadNode->down;
        DataNode* rear = pHeadNode->right;
        while (rear != nullptr) {
            
            if (rear->data == temp) {//相等删除整行
                DeleteHeadNode(pHeadNode, LIST);
                break;
            }
            else if (abs(rear->data) == abs(temp)) { //仅仅是绝对值相等铲除该节点
                if (rear == pHeadNode->right) { //头节点删除
                    pHeadNode->right = rear->next;
                    pHeadNode->Num--;

                    p = rear;
                    rear = rear->next;
                    delete p;
                    continue;
                }
                else { //删除普通节点
                    for (DataNode* front = pHeadNode->right; front != nullptr; front = front->next)
                        if (front->next == rear) {
                            front->next = rear->next;
                            pHeadNode->Num--;

                            p = rear;
                            rear = rear->next;
                            delete p;
                            continue;
                        }
                }
            }
            if(rear!=nullptr) rear = rear->next;
        }
    }
}
//删除头结点(即删除一行的数据节点)
void DeleteHeadNode(HeadNode* Clause, HeadNode*& LIST) {
    DataNode* p, * q;
    if (!Clause) return;
    if (Clause == LIST) {

        p = Clause->right;
        while (p != nullptr) {
            q = p;
            p = p->next;
            delete q;
        }

        LIST = Clause->down;

        delete Clause;
    }
    else {
        for (HeadNode* front = LIST; front != nullptr; front = front->down) {
            if (front->down == Clause) {

                p = Clause->right;
                while (p != nullptr) {
                    q = p;
                    p = p->next;
                    delete q;
                }


                front->down = Clause->down;

                delete Clause;
            }
        }
    }
}
//输出相应结果到屏幕上
void show(struct consequence* result) {
    cout << "V ";
    for (int i = 0; i < VarNum; i++) {
        if (result[i].value == TRUE)
            cout << i + 1 << " ";
        else if (result[i].value == FALSE)
            cout << -(i + 1) << " ";
        else
            cout << (i + 1) << " ";//输出相应结果到.res文件中
    }
    cout << endl;
}


//DLIS策略
//计算DLIS的分值
int calculateDLISScore(HeadNode* LIST, int var) {
    int score = 0;
    for (HeadNode* pHeadNode = LIST; pHeadNode != nullptr; pHeadNode = pHeadNode->down) {
        for (DataNode* front = pHeadNode->right; front != nullptr; front = front->next) {
            if (front->data == var) {
                score += pHeadNode->Num;
            }
        }
    }
    return score;
}
//依据DLIS策略选择变量
int chooseDLISVariable(HeadNode* LIST) {
    int maxScore = -1;
    int chosenVar = -1;

    for (int var = -VarNum; var <= VarNum; var++) {
        int score = calculateDLISScore(LIST, var);
        if (score > maxScore) {
            maxScore = score;
            chosenVar = var;
        }
    }

    return chosenVar;
}


//VSIDS策略
//检查子句是否被满足
int isClauseSatisfied(HeadNode* Clause, consequence* result) {
    for (DataNode* front = Clause->right; front != nullptr; front = front->next) {
        int literal = front->data;
        int var = abs(literal);

        // 如果变元被分配并且满足子句条件，返回1
        if (result[var-1].value == (literal > 0 ? 1 : 0)) {
            return 1;
        }
    }

    // 如果子句中没有变元被满足，返回0
    return 0;
}
//计算参与度
int calculateParticipation(HeadNode* LIST, int var, consequence* result) {
    int participation = 0;

    for (HeadNode* pHeadNode = LIST; pHeadNode != nullptr; pHeadNode = pHeadNode->down) {
        if (isClauseSatisfied(pHeadNode, result)) {
            // 子句被满足，增加变元的参与解次数
            for (DataNode* front = pHeadNode->right; front != nullptr; front = front->next) {
                if (abs(front->data) == var) {
                    participation++;
                }
            }
        }
    }

    return participation;
}
// 函数用于增加变元的活跃度
void increaseVariableScore(int variable) {
    variable_info[variable].score++;
}
// 函数用于减小变元的活跃度
void decreaseVariableScore(int variable) {
    variable_info[variable].score--;
}
// 函数用于更新变元的上次分配时间
void updateLastAssignedTime(int variable) {
    variable_info[variable].lastAssignedTime = (int)time(NULL);
}
// 函数用于计算变元的得分，包括决策历史，活跃度和上次分配时间的影响
int calculateVariableScore(int variable) {
    int baseScore = variable_info[variable].score;

    // 根据上次分配时间，给予一定的奖励或惩罚
    time_t currentTime = time(NULL);
    int timeDifference = (int)difftime(currentTime, variable_info[variable].lastAssignedTime);
    int timeFactor = 1; // 根据需要调整奖励或惩罚的程度
    int timeScore = timeDifference * timeFactor;

    // 综合基础分数和时间分数
    int totalScore = baseScore + timeScore;

    return totalScore;
}
// 计算变元的VSIDS分值
int calculateVSIDSScore(HeadNode* LIST, int var, consequence* result) {
    // 变元的活跃度分值
    // 1. 变元的出现次数，可以用变元出现的绝对值次数表示
    int frequency = 0;
    for (HeadNode* pHeadNode = LIST; pHeadNode != nullptr; pHeadNode = pHeadNode->down) {
        for (DataNode* front = pHeadNode->right; front != nullptr; front = front->next) {
            if (front->data == var || front->data == -var) {
                frequency++;
            }
        }
    }

    // 2. 变元的参与解的次数，通常是指变元在子句中被满足的次数
    int participation = calculateParticipation(LIST, var, result);

    // 3. 其他因素，如决策历史等
        // 初始化变元的信息数组

    int historyscore = calculateVariableScore(var);

    // 4. 综合以上因素计算活跃度分值
    int score = (frequency + participation) * historyscore/10;

    return score;
}
//依据VSIDS策略选择变元
int chooseVSIDSVariable(HeadNode* LIST, consequence* result) {
    int maxScore = -1;
    int chosenVar = -1;

    for (int i = 1; i <= VarNum; i++) {
        int score = calculateVSIDSScore(LIST, i, result);
        if (score > maxScore) {
            maxScore = score;
            chosenVar = i;
        }
    }
    increaseVariableScore(chosenVar);
    updateLastAssignedTime(chosenVar);
    return chosenVar;
}


//MOMS策略
int chooseMOMSVariable(HeadNode* LIST) {
    int min_unassigned_count = 99999; // 初始化为一个大值
    int chosen_var = -1;

    for (HeadNode* pHeadNode = LIST; pHeadNode != nullptr; pHeadNode = pHeadNode->down) {
        int unassigned_count = pHeadNode->Num;

        if (unassigned_count < min_unassigned_count && unassigned_count > 0) {
            min_unassigned_count = unassigned_count;
            chosen_var = abs(pHeadNode->right->data);
        }
    }

    return chosen_var;
}


//DPLL求解函数
status DPLL(HeadNode* LIST, consequence* result) {
    //单子句规则简化
    HeadNode* Pfind = LIST;
    HeadNode* SingleClause = IsSingleClause(Pfind);
    while (SingleClause != nullptr) {
        SingleClause->right->data > 0 ? result[abs(SingleClause->right->data) - 1].value = TRUE : result[abs(SingleClause->right->data) - 1].value = FALSE;
        int temp = SingleClause->right->data;
        DeleteHeadNode(SingleClause, LIST);//删除单子句这一行
        DeleteDataNode(temp, LIST);//删除相等或相反数的节点
        if (!LIST) return TRUE;
        else if (IsEmptyClause(LIST)) return FALSE;
        Pfind = LIST;
        SingleClause = IsSingleClause(Pfind);//回到头节点继续进行检测是否有单子句
    }
    //分裂策略
    int Var = 0;
    if (flag == 1) Var = chooseDLISVariable(LIST);//选取变元:采取DLIS策略，用headnode中的num代替该策略中的weight
    if (flag == 2) Var = LIST->right->data;
    if (flag == 3) {
        for (int i = 1; i <= VarNum; i++) {
            variable_info[i].variable = i;
            variable_info[i].score = 0;
            variable_info[i].lastAssignedTime = (int)time(NULL)-10;
        }
        Var = chooseVSIDSVariable(LIST, result);
    }
    if (flag == 4) Var = chooseMOMSVariable(LIST);
    HeadNode* replica = Duplication(LIST);//存放LIST的副本replica
    HeadNode* temp1 = ADDSingleClause(LIST, Var);//装载变元成为单子句
    if (DPLL(temp1, result)) return TRUE;
    else {
        HeadNode* temp2 = ADDSingleClause(replica, -Var);
        return DPLL(temp2, result);
    }
}