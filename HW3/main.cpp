#include "HW3.h"

char bunnypath[] = "E:\\Desktop\\�γ�\\�����ͼ��ѧ����\\ģ��_ʵ���-��άģ������\\bunny.obj";
char dragonpath[] = "E:\\Desktop\\�γ�\\�����ͼ��ѧ����\\ģ��_drangon\\dragon.obj";
char resultpath[] = "E:\\Desktop\\�γ�\\�����ͼ��ѧ����\\ģ��_drangon\\";
char* objpath = dragonpath;
char* outpath = resultpath;

using namespace std;

Mesh mesh;

int main(int argc, char* argv[]) {
    float Smplf_ratio;
    if(!readObj())
        exit(1);

    cout << "Please input mesh simplification ratio: ";
    cin >> Smplf_ratio;
    if (cin.fail() || Smplf_ratio >= 1 || Smplf_ratio <= 0)
    {
        cerr << "Error: simplification ratio must be a float number between 0.0 and 1.0" << endl;
        exit(1);
    }
    SimplifyMesh(Smplf_ratio);
    if(!saveObj(Smplf_ratio))
        exit(1);
    
    init(&argc, argv);
    glutMainLoop();

    return 0;
}