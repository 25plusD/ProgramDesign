//����SAT_DPLL������������
#include "Global.h"

int flag;
conse SudoResult[1000000];//��¼���յ����ֵ(Sudoku����)
consequence result[MaxVarNum];//��¼���յ����ֵ(SAT����)

int main() {
    int t0=0,t1=0;
    Start();
    int choice = 0;
    cin >> choice;
    while (choice) {
        if (choice == 1) {
            cout << "��ѡ����1.���������ļ�  2.�Զ���������" << endl;
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
                cout << "�������޽�!";
        }

        else if (choice == 2) {//SAT
            int VARNUM;
            string filename0;
            string filename;
            cout << "��������Ҫ�����ļ���(�����.cnf��׺):" << endl;
            cin >> filename0;
            filename = filename0 + ".cnf";
            HeadNode* LIST = CreateClause(VARNUM, filename);
            //���Բ�ȡ��ÿ�����������ֵ
            srand((int)time(0));
            for (int i = 0;i < VARNUM;i++) {
                result[i].value = rand() % 2;
            }
            clock_t StartTime, EndTime;
            
            cout << "��ѡ���Ƿ�ʹ���Ż���DPLL�㷨:1.�Ż�(DLIS) 2.δ�Ż� 3.�Ż�(VSIDS) 4.�Ż�(MOMS) 5.�Ż�(CDCL)" << endl;
            cin >> flag;
            cout << "DPLL���������......" << endl;
            int value = 0;
            StartTime = clock();
            if (flag == 5) CDCL2solve(filename0);
            else value = DPLL(LIST, result);
            EndTime = clock();
            t1 = t0;
            t0 = EndTime - StartTime;
            cout<<"������ʱ"<< (double)(EndTime - StartTime) / CLOCKS_PER_SEC * 1000.0 << " ms\n";
            if (flag == 5) goto end;
            if (t1 != 0 && t0 != 0)cout << "�Ż���Ϊ" << (double)(t1 - t0) / t0 * 100 << "%" << endl;
                //д���ļ�
            if (flag != 5) {
                string suffix = filename0 + ".res";
                string name = R"(E:\����ļ�\)";
                string Outputfile = name + suffix;
                ofstream fos(Outputfile);
                if (!fos.is_open()) {
                    cout << "�޷������ļ���\n";
                    exit(0);
                }
                //���
                if (value) {
                    fos << "S " << TRUE << endl;
                    fos << "V ";
                    for (int i = 0; i < VARNUM; i++) {
                        if (result[i].value == TRUE)
                            fos << i + 1 << " ";
                        else if (result[i].value == FALSE)
                            fos << -(i + 1) << " ";
                        else
                            fos << (i + 1) << " ";//ʣ��һ�ѿ�true��false�����������true
                    }
                    fos << endl;
                }
                else {
                    fos << "S " << NoAnwser << endl;
                    fos << "V ";
                    fos << endl;
                }
                //ʱ��
                fos << "T " << (double)(EndTime - StartTime) / CLOCKS_PER_SEC * 1000.0 << " ms\n";
                fos.close();
            }
            end:
            cout << "�����ɣ��Ѿ���ŵ���Ӧ.res�ļ���" << endl;
        }
        else { //����ֵ�����
            cout << "��������ȷ������! " << endl;
            exit(0);
        }
        getchar();getchar();
        system("cls");
        Start();
        cin>>choice;
    }
}