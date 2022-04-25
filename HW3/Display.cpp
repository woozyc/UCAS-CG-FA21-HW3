#include "HW3.h"
using namespace std;

class PIC {
public:
    vector<POINT3> V;
    vector<Texture> VT;
    vector<Normal> VN;
    vector<Face> F;
};

PIC m_pic = PIC();
static ColorRGB myBackground = { 0.85f, 0.85f, 0.85f };
#ifdef MAT_BRASS
//Brass material
static GLfloat MatAmbient_Brass[] = { 0.33, 0.22, 0.03, 1.0 };
static GLfloat MatDiffuse_Brass[] = { 0.78, 0.57, 0.11, 1.0 };
static GLfloat MatSpecular_Brass[] = { 0.99, 0.91, 0.81, 1.0 };
static GLfloat MatEmission_Brass[] = { 0.0, 0.0, 0.0, 1.0 };
static GLfloat fShininess_Brass = 27.8;
GLfloat *glfMatAmbient = MatAmbient_Brass;
GLfloat *glfMatDiffuse = MatDiffuse_Brass;
GLfloat *glfMatSpecular = MatSpecular_Brass;
GLfloat *glfMatEmission = MatEmission_Brass;
GLfloat fShininess = fShininess_Brass;
#endif
#ifdef MAT_EMERALD
//EmeraldGreen material
static GLfloat MatAmbient_EmeraldGreen[] = { 0.135f,0.2225f,0.1575f,0.95f };
static GLfloat MatDiffuse_EmeraldGreen[] = { 0.54f,0.89f,0.63f,0.95f };
static GLfloat MatSpecular_EmeraldGreen[] = { 0.316228f,0.316228f,0.316228f,0.95f };
static GLfloat MatEmission_EmeraldGreen[] = { 0.000f, 0.000f, 0.000f, 1.0f };
static GLfloat fShininess_EmeraldGreen = 12.800f;
GLfloat* glfMatAmbient = MatAmbient_EmeraldGreen;
GLfloat* glfMatDiffuse = MatDiffuse_EmeraldGreen;
GLfloat* glfMatSpecular = MatSpecular_EmeraldGreen;
GLfloat* glfMatEmission = MatEmission_EmeraldGreen;
GLfloat fShininess = fShininess_EmeraldGreen;
#endif
float distance_z = 0;
float distance_y = 0;
float distance_x = 0;
float x_rotate, y_rotate, z_rotate;
int xold = -1, yold = -1;
float dangle, angle;
float rotatefrac = 8.0f;
float diszfrac = 0.05f;
int leftbuttondown;
int WindowSize[] = { 600, 600 };//x, y
int WindowPos[] = { 500, 200 };//x, y

#ifndef USE_MESH_INSTEAD
void ReadPIC() {
    ifstream ifs(objpath);
    string s;
    Face* f;
    POINT3* v;
    Normal* vn;
    Texture* vt;
    while (getline(ifs, s)) {
        if (s.length() < 2)
            continue;
        if (s[0] == 'v') {
            if (s[1] == 't') {
                istringstream in(s);
                vt = new Texture();
                string head;
                in >> head >> vt->TU >> vt->TV;
                m_pic.VT.push_back(*vt);
            }
            else if (s[1] == 'n') {
                istringstream in(s);
                vn = new Normal();
                string head;
                in >> head >> vn->NX >> vn->NY >> vn->NZ;
                m_pic.VN.push_back(*vn);
            }
            else {
                istringstream in(s);
                v = new POINT3();
                string head;
                in >> head >> v->x >> v->y >> v->z;
                m_pic.V.push_back(*v);
            }
        }
        else if (s[0] == 'f') {
            for (long long k = s.size() - 1; k >= 0; k--) {
                if (s[k] == '/')
                    s[k] = ' ';
            }
            istringstream in(s);
            f = new Face();
            string head;
            in >> head;
            int i = 0;
            while (i < 3) {
                if (m_pic.V.size() != 0) {
                    in >> f->V[i];
                    f->V[i] -= 1;
                }
                if (m_pic.VT.size() != 0) {
                    in >> f->T[i];
                    f->T[i] -= 1;
                }
                if (m_pic.VN.size() != 0) {
                    in >> f->N[i];
                    f->N[i] -= 1;
                }
                i++;
            }
            m_pic.F.push_back(*f);
        }
    }
}
#endif


void InitScene() {
#ifndef USE_MESH_INSTEAD
    ReadPIC();
#endif
    glClearColor(myBackground.red, myBackground.green, myBackground.blue, 1.0);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_POLYGON_SMOOTH);

    glMaterialfv(GL_FRONT, GL_AMBIENT, glfMatAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, glfMatDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, glfMatSpecular);
    glMaterialfv(GL_FRONT, GL_EMISSION, glfMatEmission);
    glMaterialf(GL_FRONT, GL_SHININESS, fShininess);
}
void GLCube() {
    glBegin(GL_TRIANGLES);
#ifndef USE_MESH_INSTEAD
    int i, j;
    Face* f;
    int v, vn;
    double fn[3];
    for (i = 0; i < m_pic.F.size(); i++) {
        f = &(m_pic.F[i]);
        fn[0] = 0;
        fn[1] = 0;
        fn[2] = 0;
        for (j = 0; j < 3; j++) {
            vn = f->N[j];
            fn[0] += m_pic.VN[vn].NX / 3;
            fn[1] += m_pic.VN[vn].NY / 3;
            fn[2] += m_pic.VN[vn].NZ / 3;
        }
        glNormal3f(fn[0], fn[1], fn[2]);
        for (j = 0; j < 3; j++) {
            v = f->V[j];
            glVertex3f(m_pic.V[v].x, m_pic.V[v].y, m_pic.V[v].z);
        }
    }
#else
    //use openmesh
    float f_n[3] = { 0 };
    mesh.request_vertex_normals();
    mesh.request_face_normals();
    mesh.update_normals();
    Mesh::FaceIter f_it, f_end(mesh.faces_end());
    Mesh::FaceVertexIter fv_it;
    for (f_it = mesh.faces_begin(); f_it != f_end; f_it++)
    {//get a face
        f_n[0] = 0, f_n[1] = 0, f_n[2] = 0;
        //get average normal for the face
        for (fv_it = mesh.fv_iter(*f_it); fv_it.is_valid(); fv_it++)
        {
            f_n[0] += mesh.normal(*fv_it).data()[0] / 3;
            f_n[1] += mesh.normal(*fv_it).data()[1] / 3;
            f_n[2] += mesh.normal(*fv_it).data()[2] / 3;
        }
        //draw a triangle
        for (fv_it = mesh.fv_iter(*f_it); fv_it.is_valid(); fv_it++)
        {
            //normal is the average of 3 vertices' normals
            glNormal3fv(f_n);
            glVertex3fv(mesh.point(*fv_it).data());
        }
    }
#endif
    glEnd();
}

void DrawScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    GLfloat glfLight[] = { -4.0, 4.0, 4.0, 0.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, glfLight);
    glTranslatef(distance_x, distance_y, distance_z);
    gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    glRotatef(x_rotate, 1.0, 0.0, 0.0);
    glRotatef(y_rotate, 0.0, 1.0, 0.0);
    glRotatef(z_rotate, 0.0, 0.0, 1.0);
    GLCube();
    glFlush();
    glutSwapBuffers();
}
void ChangeSize(int w, int h) {
    if (h == 0)
        h = 1;
    float ratio = 1.0f * w / h;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, w, h);
    gluPerspective(45, ratio, 1, 50000);
    glMatrixMode(GL_MODELVIEW);
}
void mouseButton(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_UP) {
            leftbuttondown = 0;
        }
        else {//state == GLUT_DOWN
            leftbuttondown = 1;
            xold = x, yold = y;
        }
    }
}
void mouseMove(int x, int y) {
    if (!leftbuttondown)
        return;
    if (xold < 0) {
        xold = x;
    }
    if (yold < 0) {
        yold = y;
    }
    distance_z += (y - yold) * diszfrac * 0.2f;
    //y_rotate += (x - xold) * rotatefrac;
    //x_rotate += (y - yold) * rotatefrac;
    xold = x, yold = y;
}
void ProcessNormalKeys(unsigned char key, int x, int y) {
    switch (key) {
    case 27:
        exit(0); break;
    case 'w':
        x_rotate += rotatefrac; break;
    case 's':
        x_rotate -= rotatefrac; break;
    case 'a':
        y_rotate += rotatefrac; break;
    case 'd':
        y_rotate -= rotatefrac; break;
    case 'e':
        z_rotate -= rotatefrac; break;
    case 'q':
        z_rotate += rotatefrac; break;
    default:
        break;
    }
}
void ProcessSpecialKeys(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_UP: distance_y -= diszfrac; break;
    case GLUT_KEY_DOWN: distance_y += diszfrac; break;
    case GLUT_KEY_LEFT: distance_x += diszfrac; break;
    case GLUT_KEY_RIGHT: distance_x -= diszfrac; break;
    default: break;
    }
}
void init(int* argc, char* argv[]) {
    cout << "Initializing display......" << endl;
    glutInit(argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_RGBA | GLUT_DOUBLE);
    glutInitWindowPosition(WindowPos[0], WindowPos[1]);
    glutInitWindowSize(WindowSize[0], WindowSize[1]);
    glutCreateWindow("3D-Render");
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glutDisplayFunc(DrawScene);
    glutIdleFunc(DrawScene);
    glutReshapeFunc(ChangeSize);
    glutMouseFunc(mouseButton);
    glutMotionFunc(mouseMove);
    glutKeyboardFunc(ProcessNormalKeys);
    glutSpecialFunc(ProcessSpecialKeys);
    InitScene();
}