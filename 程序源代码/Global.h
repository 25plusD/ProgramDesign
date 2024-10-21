//定义与引入

#ifndef WORKSHOP_GLOBAL_H
#define WORKSHOP_GLOBAL_H

#include <iostream>
#include <fstream>
#include <ctime>
#include <algorithm>
#include <cmath>

using namespace std;
#define TRUE 1
#define FALSE 0
#define ROW 9
#define COL 9
#define NoAnwser -1
#define MaxVarNum 50000
typedef int status;

typedef struct DataNode {
    int data = 0;
    DataNode* next{};
}DataNode;

typedef struct HeadNode {
    int Num = 0;
    DataNode* right{};
    HeadNode* down{};
}HeadNode;

struct consequence {
    int value = -1;//存真值 真时为true=1，假时为false=0,未标记时为-1
};

struct conse {
    int num = 0;
    int value = -1;//存真值 真时为true-1，假时为false-0
};

struct VariableInfo {
    int variable;
    int score;
    int lastAssignedTime;
};//结构体表示变量的信息分数

//start
void Start();
//cnfparser
HeadNode* CreateClause(int&, string&);
//DPLLSolver
status DPLL(HeadNode*, consequence*);
HeadNode* IsSingleClause(HeadNode*);
status IsEmptyClause(HeadNode*);
HeadNode* ADDSingleClause(HeadNode*, int);
HeadNode* Duplication(HeadNode*);
void DeleteHeadNode(HeadNode*, HeadNode*&);
void DeleteDataNode(int, HeadNode*&);
void show(struct consequence*);
int chooseDLISVariable(HeadNode* );
int calculateDLISScore(HeadNode*, int);
int isClauseSatisfied(HeadNode*, consequence*);
int calculateParticipation(HeadNode*, int, consequence*);
void increaseVariableScore(int);
void decreaseVariableScore(int);
void updateLastAssignedTime(int);
int calculateVariableScore(int);
int calculateVSIDSScore(HeadNode*, int, consequence*);
int chooseVSIDSVariable(HeadNode*, consequence*);
int chooseMOMSVariable(HeadNode*);
//Hanidoku
void randomFirstRow(int a0[], int n);
void createSudoku(int a[][COL]);
void print(int arr[][9]);
string ToCnf(int a[][COL], int holes);
string createSudokuToFile();
status SudoDPLL(HeadNode* LIST, conse* result, int VARNUM);
void SudokuShow(conse* result, int VARNUM);
int getarraynumlength(int a[9]);
int getminposi(int a[9]);
int getmaxposi(int a[9]);
status SuDoKu_Judge(int arr[][9], int row, int col, int n);
bool generateSudokuBoard(int board[9][9]);
bool solveSudoku(int board[9][9], int* solutionCount);
void generateSudokuPuzzle(int board[9][9], int holes);
int getimpossiblevalue(int x, int len);
void SudokuParser(string filename, int a[][9]);
//CDCL
int CDCL2solve(string);


#endif //WORKSHOP_GLOBAL_H
