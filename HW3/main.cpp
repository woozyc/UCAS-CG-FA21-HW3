#include "HW3.h"

char bunnypath[] = "E:\\Desktop\\课程\\计算机图形学基础\\模型_实验二-三维模型样例\\bunny.obj";
char dragonpath[] = "E:\\Desktop\\课程\\计算机图形学基础\\模型_drangon\\dragon.obj";
char resultpath[] = "E:\\Desktop\\课程\\计算机图形学基础\\模型_drangon\\";
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