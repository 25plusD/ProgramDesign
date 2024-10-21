//求解Hanidoku有关的函数组
#include "Global.h"

#define CORRECT 0
#define WRONG -1
static int T = 0;
int cols[] = { 5,6,7,8,9,8,7,6,5 };
int maxdis = 9;
extern int flag;
int holes;

//初始化将坐标分组
int groupx[27][9] = {
    {1,1,1,1,1},
    {2,2,2,2,2,2},
    {3,3,3,3,3,3,3},
    {4,4,4,4,4,4,4,4},
    {5,5,5,5,5,5,5,5,5},
    {6,6,6,6,6,6,6,6},
    {7,7,7,7,7,7,7},
    {8,8,8,8,8,8},
    {9,9,9,9,9},
    {1,2,3,4,5},
    {1,2,3,4,5,6},
    {1,2,3,4,5,6,7},
    {1,2,3,4,5,6,7,8},
    {1,2,3,4,5,6,7,8,9},
    {2,3,4,5,6,7,8,9},
    {3,4,5,6,7,8,9},
    {4,5,6,7,8,9},
    {5,6,7,8,9},
    {5,6,7,8,9},
    {4,5,6,7,8,9},
    {3,4,5,6,7,8,9},
    {2,3,4,5,6,7,8,9},
    {1,2,3,4,5,6,7,8,9},
    {1,2,3,4,5,6,7,8},
    {1,2,3,4,5,6,7},
    {1,2,3,4,5,6},
    {1,2,3,4,5}
};
int groupy[27][9] = {
    {1,2,3,4,5},
    {1,2,3,4,5,6},
    {1,2,3,4,5,6,7},
    {1,2,3,4,5,6,7,8},
    {1,2,3,4,5,6,7,8,9},
    {1,2,3,4,5,6,7,8},
    {1,2,3,4,5,6,7},
    {1,2,3,4,5,6},
    {1,2,3,4,5},
    {1,1,1,1,1},
    {2,2,2,2,2,1},
    {3,3,3,3,3,2,1},
    {4,4,4,4,4,3,2,1},
    {5,5,5,5,5,4,3,2,1},
    {6,6,6,6,5,4,3,2},
    {7,7,7,6,5,4,3},
    {8,8,7,6,5,4},
    {9,8,7,6,5},
    {1,1,1,1,1},
    {1,2,2,2,2,2},
    {1,2,3,3,3,3,3},
    {1,2,3,4,4,4,4,4},
    {1,2,3,4,5,5,5,5,5},
    {2,3,4,5,6,6,6,6},
    {3,4,5,6,7,7,7},
    {4,5,6,7,8,8},
    {5,6,7,8,9}
};

int impossiblevalue[9];

//获取数组长度
int getarraynumlength(int a[9])
{
    int length = 0;
    for (int i = 0;i < 9;i++) {
        if (a[i] != 0) length++;
    }
    return length;
}
//获得数组中最小的正整数
int getminposi(int a[9])
{
    int minposi = 10;
    for (int i = 0;i < 9; i++) {
        if (a[i] != 0 && a[i] < minposi) {
            minposi = a[i];
        }
    }
    return minposi;
}
//获得数组中最大的正整数
int getmaxposi(int a[9])
{
    int maxposi = 0;
    for (int i = 0;i < 9; i++) {
        if (a[i] > maxposi) {
            maxposi = a[i];
        }
    }
    return maxposi;
}

//通过挖洞法生成蜂窝数独函数组
//数独之判断填入的数字是否正确
status SuDoKu_Judge(int arr[][9], int row, int col, int n)
{
    int mark_diag1[9], mark_diag2[9], mark_row[9];
    memset(mark_diag1, 0, sizeof(mark_diag1));
    memset(mark_diag2, 0, sizeof(mark_diag2));
    memset(mark_row, 0, sizeof(mark_row));
    int i, j, maxnum = 0, minnum = 0, distan = 0;
    int linelength = cols[row], diag1length = 0, diag2length = 0;

    for (i = 0; i < cols[row]; i++) {// 判断行重复
        mark_row[i] = arr[row][i];
        if (arr[row][i] == n) {
            return FALSE;
        }
    }

    distan = getmaxposi(mark_row) - getminposi(mark_row);
    if (distan + 1 > cols[row]) return FALSE;//判断行连续

    // 判断主,副对角线重复
    if (row <= 4) {
        for (j = 0; j < 9; j++) {
            if (j <= 4 && j >= 0) {

                mark_diag1[j] = arr[j][col];diag1length++;
                if (col + j - row >= 0) mark_diag2[j] = arr[j][col + j - row];diag2length++;

                if (arr[j][col] == n) return FALSE;//副
                if (col + j - row >= 0) {
                    if (arr[j][col + j - row] == n) return FALSE;//主
                }
            }
            if (j > 4) {

                if (col - j + 4 >= 0) mark_diag1[j] = arr[j][col - j + 4];diag1length++;
                mark_diag2[j] = arr[j][col + 4 - row];diag2length++;


                if (col - j + 4 >= 0) {
                    if (arr[j][col - j + 4] == n) return FALSE;//副
                }
                if (arr[j][col + 4 - row] == n) return FALSE;//主
            }
        }
    }
    else {
        for (j = 0;j < 9;j++) {
            if (j <= 4 && j >= 0) {

                mark_diag1[j] = arr[j][col + row - 4];diag1length++;
                if (col + j - 4 >= 0) mark_diag2[j] = arr[j][col - 4 + j];diag2length++;


                if (col + j - 4 >= 0) {
                    if (arr[j][col + j - 4] == n) return FALSE;//主
                }
                if (arr[j][col - 4 + row] == n) return FALSE;//副
            }
            if (j > 4) {

                if (col - j + row >= 0) mark_diag1[j] = arr[j][col - j + row];diag1length++;
                mark_diag2[j] = arr[j][col];diag2length++;


                if (arr[j][col] == n) return FALSE;//主
                if (col - j + row >= 0) {
                    if (arr[j][col - j + row] == n) return FALSE;//副
                }
            }
        }
    }

    // 判断对角线连续
    distan = getmaxposi(mark_diag1) - getminposi(mark_diag1);
    if (distan + 1 > diag1length) return FALSE;

    distan = getmaxposi(mark_diag2) - getminposi(mark_diag2);
    if (distan + 1 > diag2length) return FALSE;


    return TRUE;
}
//随机生成第一行数
void randomFirstRow(int a[], int n) {
    srand((int)time(0));
    for (int i = 0; i < n; ++i) {
        a[i] = rand() % 9 + 1;
        int j = 0;
        while (j < i) {
            if (a[i] == a[j] || a[j] - a[i] >= n || a[j] - a[i] <= -n) {
                a[i] = rand() % 9 + 1; //generate nine random number
                j = 0;
            }
            else { j++; }
        }
    }
}
// 使用递归和回溯生成数独棋盘
bool generateSudokuBoard(int board[9][9]) {
    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < cols[row]; col++) {
            if (board[row][col] == 0) {
                for (int num = 1; num <= 9; num++) {
                    if (SuDoKu_Judge(board, row, col, num) == TRUE) {
                        board[row][col] = num;

                        if (generateSudokuBoard(board)) {
                            return true;
                        }

                        board[row][col] = 0; // 回溯
                    }
                }
                return false; // 所有数字都尝试过，无解
            }
        }
    }
    return true; // 所有格子都已填满，生成成功
}
//生成数独
void createSudoku(int a[][COL]) { //生成数独
    randomFirstRow(a[4], 9);//随机生成最长的一行
    generateSudokuBoard(a);//递归生成后i行
}
//枚举法求解数独
bool solveSudoku(int board[9][9], int* solutionCount) {
    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < cols[row]; col++) {
            if (board[row][col] == 0) {
                for (int num = 1; num <= 9; num++) {
                    if (SuDoKu_Judge(board, row, col, num)) {
                        board[row][col] = num;

                        if (solveSudoku(board, solutionCount)) {
                            if (*solutionCount > 1) {
                                return true; // 多个解，退出
                            }
                        }

                        board[row][col] = 0; // 回溯
                    }
                }
                return false; // 无解
            }
        }
    }
    (*solutionCount)++;
    return true; // 解决成功
}
//挖洞生成数独
void generateSudokuPuzzle(int board[9][9], int holes) {
    srand((int)time(NULL));
    while (holes > 0) {
        int row = rand() % 9;
        int col = rand() % cols[row];
        if (board[row][col] != 0) {
            int temp = board[row][col];
            board[row][col] = 0;

            int copyBoard[9][9];
            for (int i = 0; i < 9; i++) {
                for (int j = 0; j < cols[i]; j++) {
                    copyBoard[i][j] = board[i][j];
                }
            }

            int solutions = 0;
            if (!generateSudokuBoard(copyBoard)) {
                board[row][col] = temp; // 恢复原数字
            }
            else {
                solutions = 0;
                if (solveSudoku(copyBoard, &solutions)) {
                    if (solutions == 1) {
                        holes--;
                    }
                    else {
                        board[row][col] = temp; // 恢复原数字
                    }
                }
            }
        }
    }
}

//画出数独棋盘
void print(int arr[][9]) {
    int i, j, a, b;
    //31-1-代入grid 
    a = 0;
    b = 0;
    for (i = 0;i < 19;i++) {
        //边界 
        if (i % 2 == 0) {
            if (i == 0 || i == 18) {
                printf("         ");
                if (i == 0) {
                    for (int j = 0;j < 5;j++) {
                        printf("/ \\ ");
                        if (j == 4) printf("\n");
                    }
                }
                else if (i == 18) {
                    for (int j = 0;j < 5;j++) {
                        printf("\\ / ");
                    }
                }
            }
            else if (i == 2 || i == 16) {
                printf("       ");
                if (i == 2) {
                    for (int j = 0;j < 6;j++) {
                        printf("/ \\ ");
                        if (j == 5) printf("\n");
                    }
                }
                else if (i == 16) {
                    for (int j = 0;j < 6;j++) {
                        printf("\\ / ");
                        if (j == 5) printf("\n");
                    }
                }

            }
            else if (i == 4 || i == 14) {
                printf("     ");
                if (i == 4) {
                    for (int j = 0;j < 7;j++) {
                        printf("/ \\ ");
                        if (j == 6) printf("\n");
                    }
                }
                else if (i == 14) {
                    for (int j = 0;j < 7;j++) {
                        printf("\\ / ");
                        if (j == 6) printf("\n");
                    }
                }

            }
            else if (i == 6 || i == 12) {
                printf("   ");
                if (i == 6) {
                    for (int j = 0;j < 8;j++) {
                        printf("/ \\ ");
                        if (j == 7) printf("\n");
                    }
                }
                else if (i == 12) {
                    for (int j = 0;j < 8;j++) {
                        printf("\\ / ");
                        if (j == 7) printf("\n");
                    }
                }
            }
            else if (i == 8 || i == 10) {
                printf(" ");
                if (i == 8) {
                    for (int j = 0;j < 9;j++) {
                        printf("/ \\ ");
                        if (j == 8) printf("\n");
                    }
                }
                else if (i == 10) {
                    for (int j = 0;j < 9;j++) {
                        printf("\\ / ");
                        if (j == 8) printf("\n");
                    }
                }
            }
        }
        //空穴 
        else if (i % 2 != 0) {
            if (i == 1 || i == 17) {
                b = 0;
                printf("        ");
                for (j = 0;j < 11;j++) {
                    if (j % 2 == 0) {
                        printf("| ");
                    }
                    else {
                        if (arr[a][b] != 0) printf("%d ", arr[a][b]);
                        else printf("· ");
                        b++;
                    }
                    if (j == 10) printf("\n");
                }
                a++;
            }
            else if (i == 3 || i == 15) {
                b = 0;
                printf("      ");
                for (j = 0;j < 13;j++) {
                    if (j % 2 == 0) {
                        printf("| ");
                    }
                    else {
                        if (arr[a][b] != 0) printf("%d ", arr[a][b]);
                        else printf("· ");
                        b++;
                    }
                    if (j == 12) printf("\n");
                }
                a++;
            }
            else if (i == 5 || i == 13) {
                b = 0;
                printf("    ");
                for (j = 0;j < 15;j++) {
                    if (j % 2 == 0) {
                        printf("| ");
                    }
                    else {
                        if (arr[a][b] != 0) printf("%d ", arr[a][b]);
                        else printf("· ");
                        b++;
                    }
                    if (j == 14) printf("\n");
                }
                a++;
            }
            else if (i == 7 || i == 11) {
                b = 0;
                printf("  ");
                for (j = 0;j < 17;j++) {
                    if (j % 2 == 0) {
                        printf("| ");
                    }
                    else {
                        if (arr[a][b] != 0) printf("%d ", arr[a][b]);
                        else printf("· ");
                        b++;
                    }
                    if (j == 16) printf("\n");
                }
                a++;
            }
            else if (i == 9) {
                b = 0;
                for (j = 0;j < 19;j++) {
                    if (j % 2 == 0) {
                        printf("| ");
                    }
                    else {
                        if (arr[a][b] != 0) printf("%d ", arr[a][b]);
                        else printf("· ");
                        b++;
                    }
                    if (j == 18) printf("\n");
                }
                a++;
            }
        }
    }
}

//通过导入文件生成数独函数组
//得到每组不可能的取值
int getimpossiblevalue(int x, int len) {
    for (int i = 0;i < 9;i++) {
        impossiblevalue[i] = 0;//初始化impossiblevalue数组
    }
    int j = 0;
    for (int i = 1;i <= x - len;i++) {
        impossiblevalue[j] = i;
        j++;
    }
    for (int i = x + len;i <= 9;i++) {
        impossiblevalue[j] = i;
        j++;
    }
    impossiblevalue[j] = x;
    j++;//返回的j为无法取到的数字的个数
    return j;
}
//将数独转化为cnf文件
string ToCnf(int a[][COL], int holes) {
    ofstream in("E:\\检查文件\\sudoku.cnf");//定义输入文件
    if (!in.is_open())
        cout << "文件无法打开!\n";

    //尝试计算有多少条Clause
    int Clausenum = 0;
    for (int x = 0; x < ROW; ++x) {
        for (int y = 0; y < cols[x]; ++y)
            if (a[x][y] != 0)
                Clausenum++;
    }
    for (int x = 1; x <= 9; ++x) {
        for (int y = 1; y <= cols[x - 1]; ++y) {
            for (int z = 1; z <= 9; ++z)
                Clausenum++;
        }
    }
    for (int i = 1;i <= 9;i++) {
        for (int j = 1;j <= cols[i - 1];j++) {
            for (int x = 1;x <= 8;x++) {
                for (int y = x + 1;y <= 9;y++) {
                    Clausenum++;
                }
            }
        }
    }
    for (int l = 0;l < 27;l++) {
        int len = getarraynumlength(groupx[l]);
        for (int temp1 = 0;temp1 < len;temp1++) {
            for (int temp2 = 0;temp2 < len;temp2++) {
                if (temp1 != temp2) {
                    for (int x = 1;x <= 9;x++) {
                        int num = getimpossiblevalue(x, len);
                        for (int y = 0;y < num;y++) {
                            Clausenum++;
                        }
                    }
                }
            }
        }
    }


    in << "p" << " " << "cnf" << " " << 549 << " " << Clausenum << " " << endl;
    //一个映射将不同的坐标映射到61个空(111~959->1~549)
    //single clause,即初盘已经存在的数 61-holes
    for (int x = 0; x < ROW; ++x) {
        for (int y = 0; y < cols[x]; ++y)
            if (a[x][y] != 0)
                in << (x + 1) * 100 + (y + 1) * 10 + a[x][y] << " " << 0 << endl;
    }
    //entry,每个格子必须要从1~9中填一个数 61
    for (int x = 1; x <= 9; ++x) {
        for (int y = 1; y <= cols[x-1]; ++y) {
            for (int z = 1; z <= 9; ++z)
                in << x * 100 + y * 10 + z << " ";
            in << 0;
            in << endl;
        }
    }
    //每个格子只能从1-9中填一个数 61*(8+7+6+5+...+1)=61*36
    for (int i = 1;i <= 9;i++) {
        for (int j = 1;j <= cols[i - 1];j++) {
            for (int x = 1;x <= 8;x++) {
                for (int y = x + 1;y <= 9;y++) {
                        in << 0 - (i * 100 + j * 10 + x) << " "
                        << 0 - (i * 100 + j * 10 + y) << " " << 0 << endl;
                }
            }
        }
    }
    //行,主副对角线的元素不能重复且同一组中的数需要连续 
    for (int l = 0;l < 27;l++) {
        int len = getarraynumlength(groupx[l]);
        for (int temp1 = 0;temp1 < len;temp1++) {
            for (int temp2 = 0;temp2 < len;temp2++) {
                if (temp1 != temp2) {
                    for (int x = 1;x <= 9;x++) {
                        int num = getimpossiblevalue(x, len);
                        for (int y = 0;y < num;y++) {
                            in << 0 - (groupx[l][temp1] * 100 + groupy[l][temp1] * 10 + x) << " "
                                << 0 - (groupx[l][temp2] * 100 + groupy[l][temp2] * 10 + impossiblevalue[y]) << " " << 0 << endl;
                        }
                    }
                }
            }
        }
    }

    
    in.close();
    return string("sudoku.cnf");
}
//导入本地的数独文件
void SudokuParser(string filename, int a[][9]) {
    int i, j;
    filename = R"(E:\蜂窝数独游戏格局文件（设计要求的应用任务）\easy_hanidoku.txt)";
    ifstream fis(filename);
    if (!fis) {
        cout << "文件无法打开.";
        exit(0);
    }
    int line;
    cout << "请输入想要求解的蜂窝数独所在的行数" << endl;
    cin >> line;
    char buf[100] = { '0' };
    for (i = 1;i <= line;i++) {
        fis.getline(buf, 100);
    }
    for (i = 5, j = 0 ; j < 5 ; i++, j++) {
        a[0][j] = buf[i] - 48;
        if (a[0][j] == 0) holes++;
    }
    for (i , j = 0; j < 6; i++, j++) {
        a[1][j] = buf[i] - 48;
        if (a[1][j] == 0) holes++;
    }
    for (i, j = 0; j < 7; i++, j++) {
        a[2][j] = buf[i] - 48;
        if (a[2][j] == 0) holes++;
    }
    for (i, j = 0; j < 8; i++, j++) {
        a[3][j] = buf[i] - 48;
        if (a[3][j] == 0) holes++;
    }
    for (i, j = 0; j < 9; i++, j++) {
        a[4][j] = buf[i] - 48;
        if (a[4][j] == 0) holes++;
    }
    for (i, j = 0; j < 8; i++, j++) {
        a[5][j] = buf[i] - 48;
        if (a[5][j] == 0) holes++;
    }
    for (i, j = 0; j < 7; i++, j++) {
        a[6][j] = buf[i] - 48;
        if (a[6][j] == 0) holes++;
    }
    for (i, j = 0; j < 6; i++, j++) {
        a[7][j] = buf[i] - 48;
        if (a[7][j] == 0) holes++;
    }
    for (i, j = 0; j < 5; i++, j++) {
        a[8][j] = buf[i] - 48;
        if (a[8][j] == 0) holes++;
    }

}


void interact(int sudoku[][9]) {
    int choice = 0;
    int x, y, z;
    printf("\n              请开始你的数独游戏！     \n");
    print(sudoku);
    printf("\n    1:填写数字      2：删除数字     3.查看答案  \n");
    cin >> choice;
    while (choice) {
        if (choice == 1) {
            printf("请输入在第几行 第几列填写何数字(中间用空格分隔)\n");
            cin >> x >> y >> z;
            if (SuDoKu_Judge(sudoku, x - 1, y - 1, z) == FALSE) {
                sudoku[x - 1][y - 1] = 0;
                printf("填错了，再想想\n");
                goto end1;
            }
            else {
                sudoku[x - 1][y - 1] = z;
            }
        }
        if (choice == 2) {
            printf("请输入在第几行 第几列删除(中间用空格分隔)\n");
            cin >> x >> y;
            sudoku[x - 1][y - 1] = 0;
        }
        if (choice == 3) {
            return;
        }
    end1:
        getchar(); getchar();
        system("cls");
        printf("\n              请开始你的数独游戏！     \n");
        print(sudoku);
        printf("\n    1:填写数字      2：删除数字     3.查看答案  \n");
        cin >> choice;
    }
}

//创建求解数独的文件
string createSudokuToFile() {

    int sudoku[9][9] = { 0 };
    int copy[9][9] = { 0 };

    if (flag == 1) {
        string filename;
        cout << "请输入想要求解的文件名" << endl;
        cin >> filename;
        SudokuParser(filename, sudoku);
    }
    
    if (flag == 2) {
        
        cout << "请输入想要挖洞的个数:" << endl;
        cin >> holes;

        //holes= 5;//挖洞个数
        createSudoku(sudoku);//生成数独终盘
        //print(sudoku);
        generateSudokuPuzzle(sudoku, holes);
        
    }
    print(sudoku);//输出初盘
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            copy[i][j] = sudoku[i][j];
        }
    }
    interact(copy);
    //转化为cnf文件
    string filename = ToCnf(sudoku, holes);
    return filename;
}
//DPLL求解数独
status SudoDPLL(HeadNode* LIST, conse* result, int VARNUM) {
    //单子句规则
    HeadNode* Pfind = LIST;
    HeadNode* SingleClause = IsSingleClause(Pfind);
    while (SingleClause != nullptr) {
        result[T].num = SingleClause->right->data;
        SingleClause->right->data > 0 ? result[T++].value = TRUE : result[T++].value = FALSE;
        int temp = SingleClause->right->data;
        DeleteHeadNode(SingleClause, LIST);//删除单子句这一行
        DeleteDataNode(temp, LIST);//删除相等或相反数的节点
        if (!LIST) return TRUE;
        else if (IsEmptyClause(LIST)) return FALSE;
        Pfind = LIST;
        SingleClause = IsSingleClause(Pfind);//回到头节点继续进行检测是否有单子句
    }
    //分裂策略
    int Var = chooseDLISVariable(LIST);
    //int Var = LIST->right->data;//选取变元
    HeadNode* replica = Duplication(LIST);//存放LIST的副本replica
    HeadNode* temp1 = ADDSingleClause(LIST, Var);//装载变元成为单子句
    if (SudoDPLL(temp1, result, VARNUM)) return TRUE;
    else {
        HeadNode* temp2 = ADDSingleClause(replica, -Var);
        return SudoDPLL(temp2, result, VARNUM);
    }
}
//输出求解完成的数独棋盘
void SudokuShow(conse* result, int VARNUM) {
    int res[9][9] = { 0 };
    for (int i = 0; result[i].value!=-1; ++i) {
        if (result[i].value == TRUE) {
            int x = (int)(abs(result[i].num) / 100) - 1;
            int y = (int)((abs(result[i].num) - (x + 1) * 100) / 10) - 1;
            res[x][y] = abs(result[i].num) - (x + 1) * 100 - (y + 1) * 10;
        }
    }
    //输出result数组
    cout << "求解后的数独如下:" << endl;
    print(res);
}
