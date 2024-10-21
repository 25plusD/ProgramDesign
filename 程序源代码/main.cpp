//基于SAT_DPLL的数独求解程序
#include "Global.h"

int flag;
conse SudoResult[1000000];//记录最终的真假值(Sudoku问题)
consequence result[MaxVarNum];//记录最终的真假值(SAT问题)

int main() {
    int t0=0,t1=0;
    Start();
    int choice = 0;
    cin >> choice;
    while (choice) {
        if (choice == 1) {
            cout << "请选择是1.导入数独文件  2.自动生成数独" << endl;
            cin >> flag;
            string filename = createSudokuToFile();
            int VARNUM;
            HeadNode* LIST = CreateClause(VARNUM, filename);
            clock_t StartTime, EndTime;
            StartTime = clock();
            int value = SudoDPLL(LIST, SudoResult, VARNUM);
            EndTime = clock();
            cout << "T " << (double)(EndTime - StartTime) / CLOCKS_PER_SEC * 1000.0 << " ms\n";
            if (value == 1)
                SudokuShow(SudoResult, VARNUM);
            else
                cout << "该数独无解!";
        }

        else if (choice == 2) {//SAT
            int VARNUM;
            string filename0;
            string filename;
            cout << "请输入想要检查的文件名(无需带.cnf后缀):" << endl;
            cin >> filename0;
            filename = filename0 + ".cnf";
            HeadNode* LIST = CreateClause(VARNUM, filename);
            //尝试采取对每个变量随机赋值
            srand((int)time(0));
            for (int i = 0;i < VARNUM;i++) {
                result[i].value = rand() % 2;
            }
            clock_t StartTime, EndTime;
            
            cout << "请选择是否使用优化的DPLL算法:1.优化(DLIS) 2.未优化 3.优化(VSIDS) 4.优化(MOMS) 5.优化(CDCL)" << endl;
            cin >> flag;
            cout << "DPLL正在求解中......" << endl;
            int value = 0;
            StartTime = clock();
            if (flag == 5) CDCL2solve(filename0);
            else value = DPLL(LIST, result);
            EndTime = clock();
            t1 = t0;
            t0 = EndTime - StartTime;
            cout<<"本次用时"<< (double)(EndTime - StartTime) / CLOCKS_PER_SEC * 1000.0 << " ms\n";
            if (flag == 5) goto end;
            if (t1 != 0 && t0 != 0)cout << "优化率为" << (double)(t1 - t0) / t0 * 100 << "%" << endl;
                //写到文件
            if (flag != 5) {
                string suffix = filename0 + ".res";
                string name = R"(E:\检查文件\)";
                string Outputfile = name + suffix;
                ofstream fos(Outputfile);
                if (!fos.is_open()) {
                    cout << "无法打开新文件。\n";
                    exit(0);
                }
                //结果
                if (value) {
                    fos << "S " << TRUE << endl;
                    fos << "V ";
                    for (int i = 0; i < VARNUM; i++) {
                        if (result[i].value == TRUE)
                            fos << i + 1 << " ";
                        else if (result[i].value == FALSE)
                            fos << -(i + 1) << " ";
                        else
                            fos << (i + 1) << " ";//剩下一堆可true可false，就索性输出true
                    }
                    fos << endl;
                }
                else {
                    fos << "S " << NoAnwser << endl;
                    fos << "V ";
                    fos << endl;
                }
                //时间
                fos << "T " << (double)(EndTime - StartTime) / CLOCKS_PER_SEC * 1000.0 << " ms\n";
                fos.close();
            }
            end:
            cout << "求解完成，已经存放到相应.res文件中" << endl;
        }
        else { //其他值不理睬
            cout << "请输入正确的数字! " << endl;
            exit(0);
        }
        getchar();getchar();
        system("cls");
        Start();
        cin>>choice;
    }
}