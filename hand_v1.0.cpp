#include "stdafx.h"
#include "SCServo/SCServo.h"
#include <iostream>
using namespace std;

SMSBL sm;


struct Gesture
{
    string gname;
    int gnum;
    int gpos[6];
};

Gesture chushi = { "��ʼ", 0, {2047, 2047, 2047, 2047, 2047, 2047} };
Gesture shitou = { "ʯͷ", 1, {1000, 10, 4090, 4090, 4090, 4090} };
Gesture jiandao = { "����", 2, {1000, 10, 2047, 2047, 4090, 4090} };
Gesture bu = { "��", 3, {2047, 2047, 2047, 2047, 2047, 2047} };

Gesture zhua1 = { "ץ1", 11, {1000, 1536, 2558, 2558, 2558, 2558} };
Gesture zhua2 = { "ץ2", 12, {1000, 10, 4090, 4090, 4090, 4090} };

Gesture hello = { "�ʺ�", 17, {1000, 10, 4090, 2047, 4090, 4090} };

void hand_write(int a[6])
{
    for (int i = 0; i < 6; i++)
    {
        sm.WritePosEx(40 + i, a[i], 1500, 150);
    }
}

int main(int argc, char** argv)
{
    if (argc < 2) {                  //�����Ƿ����ö˿�
        cout << "argc error!" << endl;
        getchar();
        return 0;
    }
    cout << "serial:" << argv[1] << endl;
    if (!sm.begin(115200, argv[1])) {                  //���鲨���ʼ��˿��Ƿ���ȷ
        cout << "Failed to init smb motor!" << endl;
        getchar();
        return 0;
    }
    //================================����Ϊ��ʼ��================================

    int sr;
    bool quit = false;

    while (1)
    {
        cout << "ʯͷ��1�� ������2�� ����3��" << endl;
        cin >> sr;

        switch (sr)
        {
        case 0:
            hand_write(chushi.gpos);
            break;
        case 1:
            hand_write(shitou.gpos);
            break;
        case 2:
            hand_write(jiandao.gpos);
            break;
        case 3:
            hand_write(bu.gpos);
            break;
        case 11:
            hand_write(zhua1.gpos);
            break;
        case 12:
            hand_write(zhua2.gpos);
            break;
        case 17:
            hand_write(hello.gpos);
            break;
        case 99:
            quit = true;
            break;
        default:
            cout << "��tmϹ��" << endl;
            break;
        }

        if (quit) break;

    }

    return 0;
}