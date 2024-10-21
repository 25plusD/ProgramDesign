//cnf�ļ���ȡ��
#include "Global.h"

int VarNum;
int ClauseNum;

//��cnf�ļ��е����ݵ��벢�洢���ٽ�����ṹ��
HeadNode* CreateClause(int& VARNUM, string& filename) {
    string HFilePath = R"(E:\����ļ�\)";
    string path = HFilePath + filename;
    ifstream fis(path);
    if (!fis) {
        cout << "�ļ��޷���.";
        exit(0);
    }
    char ch;
    char buf[100];
    fis >> ch;
    while (ch != 'p') {
        fis.getline(buf, 100);
        fis >> ch;
    }
    string cnf;

    fis >> cnf >> VarNum >> ClauseNum;
    fis.get();

    //д���ٽ���������ݽṹ��
    HeadNode* HEAD = new HeadNode;
    HeadNode* headRear = HEAD;
    HeadNode* END = new HeadNode;
    for (int i = 0; i < ClauseNum; i++) {
        //����DATA LIST
        int temp;
        fis >> temp;
        //�����һ��DATANODE
        DataNode* front = new DataNode;
        front->data = temp;
        headRear->right = front;
        headRear->Num++;
        //���������DATANODE
        fis >> temp;
        while (temp != 0) {
            DataNode* rear = new DataNode;
            front->next = rear;
            rear->data = temp;
            front = front->next;
            headRear->Num++;
            fis >> temp;
        }
        front->next = nullptr;
        fis.get();//���з�
        HeadNode* tp = new HeadNode;
        headRear->down = tp;
        END = headRear;
        headRear = headRear->down;
    }
    END->down = nullptr;

    //output link lists
    HeadNode* Phead = HEAD;
    DataNode* front;
    cout << "\n";
    while (Phead != nullptr) {
        front = Phead->right;
        while (front != nullptr) {
            //cout << front->data << " ";
            front = front->next;
        }
        //cout << endl;
        Phead = Phead->down;
    }

    VARNUM = VarNum;
    return HEAD;
}