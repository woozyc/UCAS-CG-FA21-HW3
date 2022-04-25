#pragma once
#ifdef _WIN32 
#include <windows.h> 
#endif 
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

#include <gl/GL.h>
#include <gl/GLU.h>
#include <gl/freeglut_std.h>
#include <GLFW/glfw3.h>

#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>

#include <eigen3/Eigen/Eigen>

#define USE_MESH_INSTEAD
//#define MAT_BRASS
#define MAT_EMERALD
/*
using namespace std;
using namespace Eigen;
using namespace OpenMesh;

struct Traits :public OpenMesh::DefaultTraits
{
    VertexTraits
    {
    private:
        Eigen::Matrix4f Q_;
    public:
        Eigen::Matrix4f get_Q() { return Q_; }
        void set_Q(Eigen::Matrix4f Q) { Q_ = Q; }
    };
};

typedef struct VertexPair
{
    OpenMesh::VertexHandle p1;
    OpenMesh::VertexHandle p2;
    OpenMesh::HalfedgeHandle he;
    OpenMesh::Vec3f p_best;
    double cost;
    //for comparation
    bool operator < (const VertexPair& b) const { return cost < b.cost; };
    bool operator > (const VertexPair& b) const { return cost > b.cost; };
} VertexPair;

typedef OpenMesh::PolyMesh_ArrayKernelT<Traits> Mesh;

extern char* objpath;
extern char* outpath;
extern Mesh mesh;

struct ColorRGB {
    float red;
    float green;
    float blue;
};
struct POINT3 {
    double x;
    double y;
    double z;
};
struct Texture {
    double TU;
    double TV;
};
struct Normal {
    double NX;
    double NY;
    double NZ;
};
struct Face {
    int V[3];
    int T[3];
    int N[3];
};

void ReadPIC(void);
void InitScene();
void GLCube();
void DrawScene();
void ChangeSize(int w, int h);

void mouseButton(int button, int state, int x, int y);
void mouseMove(int x, int y);
void ProcessNormalKeys(unsigned char key, int x, int y);
void ProcessSpecialKeys(int key, int x, int y);

void init(int* argc, char* argv[]);

bool readObj();
bool saveObj(float ratio);
bool SimplifyMesh(float ratio);
*/

using namespace std;

// Openmesh
//typedef OpenMesh::TriMesh_ArrayKernelT<>  Mesh;
/*struct DefaultTraits {
    typedef OpenMesh::Vec3f Point;
    typedef OpenMesh::Vec3f Normal;
    typedef OpenMesh::Vec2f TexCoord;
    typedef OpenMesh::Vec3uc Color;
#ifndef DOXY_IGNORE_THIS
    VertexTraits{};
    HalfedgeTraits{};
    EdgeTraits{};
    FaceTraits{};
#endif
    VertexAttributes(0);
    HalfedgeAttributes(OpenMesh::Attributes::PrevHalfedge);
    EdgeAttributes(0);
    FaceAttributes(0);
};*/
/*
struct Traits :public OpenMesh::DefaultTraits
{
    VertexTraits
    {
    private:
        Eigen::Matrix4f Q_;
    public:
        Eigen::Matrix4f get_Q() { return Q_; }
        void set_Q(Eigen::Matrix4f Q) { Q_ = Q; }
    };
};*/
typedef OpenMesh::PolyMesh_ArrayKernelT<> Mesh;
/*
struct MyTraits :public OpenMesh::DefaultTraits {
    VertexAttributes(OpenMesh::Attributes::Normal | OpenMesh::Attributes::Color);
    FaceAttributes(OpenMesh::Attributes::Normal);
};

typedef vector<int> link;

class Pair {
public:
    int pt1;
    int pt2;
};

class Item {
public:
    int cost;
    Mesh::Point optimalv3;
    Pair validpair;
};

class Objects {
public:
    vector<Mesh::Point> points;
    vector<Mesh::Face> faces;
    vector<Mesh::Normal> normals;
    vector<link> linkpoints;
    vector<link> linkfaces;
    //vector<glm::mat4> Q;
    int sizeofvalidfaces;
    int sizeofvalidpoints;
    vector<bool>validpoints;
    vector<bool>validfaces;
    vector<link> linkcostheap;
    vector<Item>costheap;

    //glm::mat4 ComputeQ(int p);
    Item GetCost(int pti, int ptj);
    void MakeHeap();
    void DeleteVertex();
    //int InitializeAll(char* input_name);
    int Output(char* output_name);
    void updateLink(int todelete);
    void addLink(int todelete, vector<int>* ano);
};

struct MinHeapCmp
{
    inline bool operator()(Item x, Item y)
    {
        return x.cost > y.cost;
    }
};

//needed in Lab2
struct Point3 {
    double X;
    double Y;
    double Z;
};
struct Texture {
    double TU;
    double TV;
};
struct Normal {
    double NX;
    double NY;
    double NZ;
};
struct Face {
    int V[3];
    int T[3];
    int N[3];
};
class ObjPic {
    vector<Point3> V;
    vector<Texture> VT;
    vector<Normal> VN;
    vector<Face> F;
public:
    string filePath;
    void read();
    void draw();
};

extern ObjPic p;
void init();
void drawScene();
*/