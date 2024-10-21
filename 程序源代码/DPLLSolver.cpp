//DPLL�����
//�ֱ������DLIS VSIDS MOMS �ͱ���������ֲ���
//����ͨ���ٽ�����洢

#include "Global.h"

extern int VarNum;
extern int ClauseNum;
extern int flag;
struct VariableInfo variable_info[MaxVarNum]; // �洢������Ϣ������

//�ж�LIST���Ƿ��п��Ӿ�
status IsEmptyClause(HeadNode* LIST) {
    HeadNode* PHead = LIST;
    while (PHead != nullptr) {
        if (PHead->Num == 0)
            return TRUE;
        PHead = PHead->down;
    }
    return FALSE;
}
//Ѱ�ҵ��Ӿ�,���ص��Ӿ��ָ��
HeadNode* IsSingleClause(HeadNode* Pfind) {
    while (Pfind != nullptr) {
        if (Pfind->Num == 1)
            return Pfind;
        Pfind = Pfind->down;
    }
    return nullptr;
}
//��������LIST
HeadNode* Duplication(HeadNode* LIST) { //�˴����鴫����������ʼ��鸴�������߼�����
    HeadNode* SrcHead = LIST;
    HeadNode* ReHead = new HeadNode;//�������ͷ�ڵ�
    ReHead->Num = SrcHead->Num;//���Ƶ�һ��ͷ�ڵ�
    HeadNode* Phead = ReHead;//Phead����ͷ�ڵ�
    DataNode* ReData = new DataNode;//����������ݽڵ�
    DataNode* FirstSrcData = SrcHead->right;//���ڴ�����һ�еĵ�һ�����ݽڵ�
    ReData->data = FirstSrcData->data;//������ĵ�һ�����ݽڵ����ֵ
    Phead->right = ReData;
    for (FirstSrcData = FirstSrcData->next;FirstSrcData != nullptr; FirstSrcData = FirstSrcData->next) {//��һ�����������
        DataNode* NewDataNode = new DataNode;
        NewDataNode->data = FirstSrcData->data;
        ReData->next = NewDataNode;
        ReData = ReData->next;
    }
    //���������ڵ�ĸ��� >=2th
    for (SrcHead = SrcHead->down; SrcHead != nullptr; SrcHead = SrcHead->down) {
        HeadNode* NewHead = new HeadNode;
        DataNode* NewData = new DataNode;
        NewHead->Num = SrcHead->Num;
        Phead->down = NewHead;
        Phead = Phead->down;
        DataNode* SrcData = SrcHead->right;
        NewData->data = SrcData->data;
        Phead->right = NewData;//��һ�����ݽڵ�
        for (SrcData = SrcData->next;SrcData != nullptr; SrcData = SrcData->next) {//����ʣ�µ����ݽڵ�
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
//���ӵ��Ӿ�
HeadNode* ADDSingleClause(HeadNode* LIST, int Var) { //���ӵĵ��Ӿ�λ�������ͷ
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
//ɾ�����ݽ��
void DeleteDataNode(int temp, HeadNode*& LIST) {
    DataNode* p;
    HeadNode* pHeadNode0=LIST;
    for (HeadNode* pHeadNode = LIST; pHeadNode != nullptr; pHeadNode = pHeadNode0) {
        pHeadNode0 = pHeadNode->down;
        DataNode* rear = pHeadNode->right;
        while (rear != nullptr) {
            
            if (rear->data == temp) {//���ɾ������
                DeleteHeadNode(pHeadNode, LIST);
                break;
            }
            else if (abs(rear->data) == abs(temp)) { //�����Ǿ���ֵ��Ȳ����ýڵ�
                if (rear == pHeadNode->right) { //ͷ�ڵ�ɾ��
                    pHeadNode->right = rear->next;
                    pHeadNode->Num--;

                    p = rear;
                    rear = rear->next;
                    delete p;
                    continue;
                }
                else { //ɾ����ͨ�ڵ�
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
//ɾ��ͷ���(��ɾ��һ�е����ݽڵ�)
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
//�����Ӧ�������Ļ��
void show(struct consequence* result) {
    cout << "V ";
    for (int i = 0; i < VarNum; i++) {
        if (result[i].value == TRUE)
            cout << i + 1 << " ";
        else if (result[i].value == FALSE)
            cout << -(i + 1) << " ";
        else
            cout << (i + 1) << " ";//�����Ӧ�����.res�ļ���
    }
    cout << endl;
}


//DLIS����
//����DLIS�ķ�ֵ
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
//����DLIS����ѡ�����
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


//VSIDS����
//����Ӿ��Ƿ�����
int isClauseSatisfied(HeadNode* Clause, consequence* result) {
    for (DataNode* front = Clause->right; front != nullptr; front = front->next) {
        int literal = front->data;
        int var = abs(literal);

        // �����Ԫ�����䲢�������Ӿ�����������1
        if (result[var-1].value == (literal > 0 ? 1 : 0)) {
            return 1;
        }
    }

    // ����Ӿ���û�б�Ԫ�����㣬����0
    return 0;
}
//��������
int calculateParticipation(HeadNode* LIST, int var, consequence* result) {
    int participation = 0;

    for (HeadNode* pHeadNode = LIST; pHeadNode != nullptr; pHeadNode = pHeadNode->down) {
        if (isClauseSatisfied(pHeadNode, result)) {
            // �Ӿ䱻���㣬���ӱ�Ԫ�Ĳ�������
            for (DataNode* front = pHeadNode->right; front != nullptr; front = front->next) {
                if (abs(front->data) == var) {
                    participation++;
                }
            }
        }
    }

    return participation;
}
// �����������ӱ�Ԫ�Ļ�Ծ��
void increaseVariableScore(int variable) {
    variable_info[variable].score++;
}
// �������ڼ�С��Ԫ�Ļ�Ծ��
void decreaseVariableScore(int variable) {
    variable_info[variable].score--;
}
// �������ڸ��±�Ԫ���ϴη���ʱ��
void updateLastAssignedTime(int variable) {
    variable_info[variable].lastAssignedTime = (int)time(NULL);
}
// �������ڼ����Ԫ�ĵ÷֣�����������ʷ����Ծ�Ⱥ��ϴη���ʱ���Ӱ��
int calculateVariableScore(int variable) {
    int baseScore = variable_info[variable].score;

    // �����ϴη���ʱ�䣬����һ���Ľ�����ͷ�
    time_t currentTime = time(NULL);
    int timeDifference = (int)difftime(currentTime, variable_info[variable].lastAssignedTime);
    int timeFactor = 1; // ������Ҫ����������ͷ��ĳ̶�
    int timeScore = timeDifference * timeFactor;

    // �ۺϻ���������ʱ�����
    int totalScore = baseScore + timeScore;

    return totalScore;
}
// �����Ԫ��VSIDS��ֵ
int calculateVSIDSScore(HeadNode* LIST, int var, consequence* result) {
    // ��Ԫ�Ļ�Ծ�ȷ�ֵ
    // 1. ��Ԫ�ĳ��ִ����������ñ�Ԫ���ֵľ���ֵ������ʾ
    int frequency = 0;
    for (HeadNode* pHeadNode = LIST; pHeadNode != nullptr; pHeadNode = pHeadNode->down) {
        for (DataNode* front = pHeadNode->right; front != nullptr; front = front->next) {
            if (front->data == var || front->data == -var) {
                frequency++;
            }
        }
    }

    // 2. ��Ԫ�Ĳ����Ĵ�����ͨ����ָ��Ԫ���Ӿ��б�����Ĵ���
    int participation = calculateParticipation(LIST, var, result);

    // 3. �������أ��������ʷ��
        // ��ʼ����Ԫ����Ϣ����

    int historyscore = calculateVariableScore(var);

    // 4. �ۺ��������ؼ����Ծ�ȷ�ֵ
    int score = (frequency + participation) * historyscore/10;

    return score;
}
//����VSIDS����ѡ���Ԫ
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


//MOMS����
int chooseMOMSVariable(HeadNode* LIST) {
    int min_unassigned_count = 99999; // ��ʼ��Ϊһ����ֵ
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


//DPLL��⺯��
status DPLL(HeadNode* LIST, consequence* result) {
    //���Ӿ�����
    HeadNode* Pfind = LIST;
    HeadNode* SingleClause = IsSingleClause(Pfind);
    while (SingleClause != nullptr) {
        SingleClause->right->data > 0 ? result[abs(SingleClause->right->data) - 1].value = TRUE : result[abs(SingleClause->right->data) - 1].value = FALSE;
        int temp = SingleClause->right->data;
        DeleteHeadNode(SingleClause, LIST);//ɾ�����Ӿ���һ��
        DeleteDataNode(temp, LIST);//ɾ����Ȼ��෴���Ľڵ�
        if (!LIST) return TRUE;
        else if (IsEmptyClause(LIST)) return FALSE;
        Pfind = LIST;
        SingleClause = IsSingleClause(Pfind);//�ص�ͷ�ڵ�������м���Ƿ��е��Ӿ�
    }
    //���Ѳ���
    int Var = 0;
    if (flag == 1) Var = chooseDLISVariable(LIST);//ѡȡ��Ԫ:��ȡDLIS���ԣ���headnode�е�num����ò����е�weight
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
    HeadNode* replica = Duplication(LIST);//���LIST�ĸ���replica
    HeadNode* temp1 = ADDSingleClause(LIST, Var);//װ�ر�Ԫ��Ϊ���Ӿ�
    if (DPLL(temp1, result)) return TRUE;
    else {
        HeadNode* temp2 = ADDSingleClause(replica, -Var);
        return DPLL(temp2, result);
    }
}