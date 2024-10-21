//cnf文件读取器
#include "Global.h"

int VarNum;
int ClauseNum;

//将cnf文件中的数据导入并存储到临接链表结构中
HeadNode* CreateClause(int& VARNUM, string& filename) {
    string HFilePath = R"(E:\检查文件\)";
    string path = HFilePath + filename;
    ifstream fis(path);
    if (!fis) {
        cout << "文件无法打开.";
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

    //写入临接链表的数据结构中
    HeadNode* HEAD = new HeadNode;
    HeadNode* headRear = HEAD;
    HeadNode* END = new HeadNode;
    for (int i = 0; i < ClauseNum; i++) {
        //导入DATA LIST
        int temp;
        fis >> temp;
        //导入第一个DATANODE
        DataNode* front = new DataNode;
        front->data = temp;
        headRear->right = front;
        headRear->Num++;
        //导入后续的DATANODE
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
        fis.get();//换行符
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