#include "mesh.h"

//glm::mat4 Zero;

//glm::mat4 Objects::ComputeQ(int p)
//{
//    //Q[p] = Matrix::Zero;
//    glm::mat4 Kp;
//    for(int i = 0; i < linkfaces[p].size(); i++) {
//        glm::vec4 plane;
//        plane.x = normals[linkfaces[p][i]][0];
//        plane.y = normals[linkfaces[p][i]][1];
//        plane.z = normals[linkfaces[p][i]][2];
//        plane.p = -points[p] | normals[linkfaces[p][i]];
//        Kp = glm::outerProduct(plane,plane);
//        Q[p] += Kp;
//    }
//    return Q[p];
//}
//
//double Det3x3(double *Q)
//{
//    double det;
//    return det;
//}
//
//Item Objects::GetCost(int pti, int ptj)
//{
//    glm::mat4 m;
//    m = Q[pti] + Q[ptj];
//
//    glm::mat4 m1;
//    for (int k = 0; k < 3; k++) {
//        for (int j = 0; j < 4; j++) {
//            m1[k][j] = m[k][j];
//        }
//    }
//    m1[3][0] = 0;
//    m1[3][1] = 0;
//    m1[3][2] = 0;
//    m1[3][3] = 1;
//    glm::vec4 a={0,0,0,1};
//    glm::vec4 optimalv4,pi,pj;
//    pi[0] = points[pti][0];
//    pi[1] = points[pti][1];
//    pi[2] = points[pti][2];
//    pi[3] = 1;
//    pj[0] = points[ptj][0];
//    pj[1] = points[ptj][1];
//    pj[2] = points[ptj][2];
//    pj[3] = 1;
//    if (glm::determinant(m1) == 0) {
//        double maxk=0, maxcost=0;
//        double cost;
//        double k=0,step=0.1;
//        while(k <= 1) {
//            glm::vec4 k1 = {k,k,k,k};
//            glm::vec4 k2 = {1-k,1-k,1-k,1-k};
//            optimalv4 = k2 * pi + k1 * pj;
//            cost= (glm::transpose(optimalv4) * m * optimalv4)[0];
//            if (cost < maxcost) {
//                maxk = k;
//            }
//            k+=step;
//        }
//        glm::vec4 k1 = {maxk,maxk,maxk,maxk};
//        glm::vec4 k2 = {1-maxk,1-maxk,1-maxk,1-maxk};
//        optimalv4 = k2 * pi + k1 * pj;
//    } else{
//        optimalv4 = glm::inverse(m1) * a;
//    }
//
//    /*
//    double Qsolve[4][10];
//    memset(Qsolve, 0 ,sizeof(Qsolve));
//    for (int k = 0; k < 4; k++) {
//        for (int i = 0; i < 3; i++) {
//            for (int j = 0; j < 3; j++) {
//                Qsolve[k][i * 3 + j] = m[i][j];
//            }
//        }
//    }
//    for (int k = 1; k < 4; k++) {
//        for (int i = 0; i < 3; i++) {
//            Qsolve[k][i * 3 + k - 1] = m[i][3];
//        }
//    }
//    glm::vec4 optimalv4;
//    double temp3x3 = Det3x3(Qsolve[0]);
//    if(temp3x3 != 0) {
//        optimalv4.x = -Det3x3(Qsolve[1]) / Det3x3(Qsolve[0]);
//        optimalv4.y = -Det3x3(Qsolve[2]) / Det3x3(Qsolve[0]);
//        optimalv4.z = -Det3x3(Qsolve[3]) / Det3x3(Qsolve[0]);
//    } else {
//        glm::vec4 pi,pj;
//        pi[0] = points[pti][0];
//        pi[1] = points[pti][1];
//        pi[2] = points[pti][2];
//        pi[3] = 1;
//        pj[0] = points[ptj][0];
//        pj[1] = points[ptj][1];
//        pj[2] = points[ptj][2];
//        pj[3] = 1;
//        double maxk=0, maxcost=0;
//        double cost;
//        double k = 0.0,step = 0.1;
//        while (k <= 1) {
//            optimalv4 = (1 - k) * pj + k * pj;
//            cost= (glm::transpose(optimalv4) * m * optimalv4)(0);
//            if (cost < maxcost) {
//                maxk = k;
//                maxcost = cost;
//            }
//            k += step;
//        } //find the smallest
//        optimalv4 = (1 - maxk)* pi + maxk * pj;
//    }
//    optimalv4.p = 1;
//     */
//    double cost;
//    cost = glm::transpose(optimalv4) * m * optimalv4;
//    Item best;
////    if ((cost >= 2 || cost <= 2) == false) {
////        cost == 0.0;
////    }
//    best.cost = fabs(cost);
//    best.optimalv3[0] = optimalv4.x;
//    best.optimalv3[1] = optimalv4.y;
//    best.optimalv3[2] = optimalv4.z;
//    best.validpair.pt1 = pti;
//    best.validpair.pt2 = ptj;
//    return best;
//}
//
//void Objects::MakeHeap() {
//    cout << "MakeHeap..." << endl;
//    link null;
//    for (int i = 0; i < points.size(); i++) {
//        linkcostheap.push_back(null);
//    }
//
//    for (int i = 0; i < points.size(); i++) {
//        for (int j = 0; j < linkpoints[i].size(); j++) {
//            if (i < linkpoints[i][j]) {
//                Item temp = GetCost(i, linkpoints[i][j]);
//                costheap.push_back(temp);
//                push_heap(costheap.begin(),costheap.end(),MinHeapCmp());
//            }
//        }
//    }
//}
//
//void Objects::DeleteVertex()
//{
//    Item vbest = costheap[0];//PopHeap
//
//    points.push_back(vbest.optimalv3);
//    validpoints.push_back(true);
//    sizeofvalidpoints++;
//
//    int todelete1 = vbest.validpair.pt1;
//    int todelete2 = vbest.validpair.pt2;
//    validpoints[todelete1] = false;
//    validpoints[todelete2] = false;
//    sizeofvalidpoints --;
//    sizeofvalidpoints --;
//
//    for (int i = 0; i < linkfaces[todelete1].size(); i++) {
//        for (int j = 0; j < linkfaces[todelete2].size(); j++) {
//            if (linkfaces[todelete1][i] == linkfaces[todelete2][j]) {
//                validfaces[linkfaces[todelete1][i]] = false;
//                int c = faces[linkfaces[todelete1][i]].pt1 ^ faces[linkfaces[todelete1][i]].pt2
//                        ^ faces[linkfaces[todelete1][i]].pt3 ^ todelete1 ^ todelete2;
//                for (int k = 0; k < linkfaces[c].size(); k) {
//                    if (linkfaces[c][k] == linkfaces[todelete1][i]) {
//                        linkfaces[c].erase(linkfaces[c].begin() + k);
//                    }
//                }
//                sizeofvalidfaces --;
//            }
//        }
//    }
//    link nulllinkface;
//    linkfaces.push_back(nulllinkface);
//    updateLink(todelete1);
//    updateLink(todelete2);
//
//    //Calculate new Q
//    Q.push_back(Zero);
//    ComputeQ(Q.size() - 1);
//
//    // set new pair
//    // change cost and update
//    link nullpair;
//    linkcostheap.push_back(nullpair);
//    vector<int> ano;
//    addLink(todelete1,&ano);
//    addLink(todelete2,&ano);
//    sort(ano.begin(), ano.end());
//    int p = 1;
//    for (int i = 1; i < ano.size(); i++) {
//        if (ano[i] != ano[i-1]) {
//            ano[p++] = ano[i];
//        }
//    }
//    ano.erase(ano.begin() + p, ano.end());
//
//    linkpoints.push_back(ano);
//    int tempsize = linkcostheap[todelete1].size();
//    for(int i = 0; i < tempsize; i++) {
//        Heappop(linkcostheap[todelete1][0]);
//    }
//    tempsize = linkcostheap[todelete2].size();
//    for(int i = 0; i < tempsize; i++) {
//        Heappop(linkcostheap[todelete2][0]);
//    }
//    for (int i = 0; i < ano.size(); i++) {
//        Item temp = GetCost(points.size() - 1, ano[i]);
//        costheap.push_back(temp);
//        push_heap(costheap.begin(),costheap.end(),MinHeapCmp());
//    }
//}
//
//void Objects::updateLink(int todelete) {
//    for (int i = 0; i < linkfaces[todelete].size(); i++) {
//        if (validfaces[linkfaces[todelete][i]]) {
//            linkfaces.rbegin()->push_back(linkfaces[todelete][i]);
//            if(faces[linkfaces[todelete][i]].pt1 == todelete)
//            {
//                faces[linkfaces[todelete][i]].pt1 = linkfaces.size() - 1;
//            } else if (faces[linkfaces[todelete][i]].pt2 == todelete)
//            {
//                faces[linkfaces[todelete][i]].pt2 = linkfaces.size() - 1;
//            } else if (faces[linkfaces[todelete][i]].pt3 == todelete)
//            {
//                faces[linkfaces[todelete][i]].pt3 = linkfaces.size() - 1;
//            }
//            normals[linkfaces[todelete][i]] = Normalize(linkfaces[todelete][i]);
//        }
//    }
//}
//
//void Objects::addLink(int todelete, vector<int> *ano) {
//    for (int i = 0; i < linkcostheap[todelete].size(); i++) {
//        int anopoint = 0;
//        if (costheap[linkcostheap[todelete][i]].validpair.pt1 == todelete) {
//            anopoint = costheap[linkcostheap[todelete][i]].validpair.pt2;
//        } else {
//            anopoint = costheap[linkcostheap[todelete][i]].validpair.pt1;
//        }
//        if (validpoints[anopoint]) {
//            ano->push_back(anopoint);
//        }
//    }
//}

Mesh mesh;
int InitializeAll()
{
    
    //normal = mesh.normal(*v_it);
    //mesh.update_normals();
    /*if (!mesh.has_vertex_normals()) {
        std::cerr << "ERROR: Vertex property Normals not available!\n";
    }
    for (v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); ++v_it) {
        auto point = mesh.point(*v_it);
        points.push_back(point);
        //normal = mesh.normal(*v_it);
        //normals.push_back(normal);
        link lps, lfs;
        for (vv_it = mesh.vv_iter(*v_it); vv_it.is_valid(); ++vv_it) {
            auto lp = mesh.point(*vv_it);
            lps.push_back(lp.data()[0]);
            lps.push_back(lp.data()[0]);
            lps.push_back(lp.data()[0]);
        }
        //        for(auto vf_it=mesh.vf_iter(*v_it); vf_it.is_valid(); vf_it++) {
        //            auto lf = mesh.face(*vf_it);
        //            lfs.push_back(lf.data()[0]);
        //            lfs.push_back(lf.data()[0]);
        //            lfs.push_back(lf.data()[0]);
        //        }
        linkpoints.push_back(lps);
        //        Q.push_back(Zero);
        //        ComputeQ(Q.size() - 1);
    }
    for (auto f_it = mesh.faces_begin(); f_it != mesh.faces_end(); f_it++) {
        auto face = mesh.face(*f_it);
        faces.push_back(face);
    }*/

    //MakeHeap();
    return 1;
}
/*int Objects::Output(char* output_name)
{
    Mesh mesh;
    Mesh::VertexHandle vhandle[sizeofvalidpoints];
    Mesh::FaceHandle fhandle[sizeofvalidfaces];
    for (int i = 0; i < points.size(); ++i) {
        if (validpoints[i]) {
            vhandle[i] = mesh.add_vertex(points[i]);
        }
    }
    //    for (int i = 0; i < faces.size(); ++i) {
    //        if(validfaces[i]){
    //            for(auto it = mesh.fv_begin(faces[i]); it != mesh.fv_end(faces[i]); ++it){
    //                fhandle[i](it.handle());
    //            }
    //            mesh.add_face(fhandle);
    //        }
    //    }
    bool result = OpenMesh::IO::write_mesh(mesh, output_name);
    if (!result) {
        cerr << "Error: fail to write into " << output_name << endl;
        return 0;
    }
    return 1;
} */

int main(int argc, char* argv[]) {
    //if (1) {
      //  strcpy_s(input_name, argv[1]);
        //strcpy_s(output_name, argv[2]);
        //sscanf_s(argv[3], "%f", &c);
        /* initialize */
    OpenMesh::IO::read_mesh(mesh, "E:\\Desktop\\课程\\计算机图形学基础\\模型_drangon\\dragon.obj");
    Mesh::VertexIter v_it = mesh.vertices_begin();
    Mesh::VertexFaceIter vv_it = mesh.vf_iter(*v_it);
    mesh.request_vertex_normals();
    mesh.request_face_normals();
    mesh.update_normals();
    cout << mesh.normal(*v_it).data()[0] << mesh.normal(*v_it).data()[1] << mesh.normal(*v_it).data()[2] << endl;
    cout << mesh.normal(*vv_it).data()[0] << mesh.normal(*vv_it).data()[1] << mesh.normal(*vv_it).data()[2] << endl;

        /* simplify mesh 
        int opt_faces = model.sizeofvalidfaces * c;
        a = time(NULL);
        cout << "Deleting..." << endl;
        while (model.sizeofvalidfaces > opt_faces) {
            //model.DeleteVertex();
        }
        b = time(NULL);
       // model.Output(output_name);
        cpu_time = (float)(b - a);
        cout << "cpu time=" << cpu_time << endl;
    }
    else {
        cerr << "Usage: ./main <input_name> <output_name> <simplify_rate>" << endl;
    }
    //    ObjPic p;
    //    p.filePath = output_name;
    //    p.read();
    //    glutInit(&argc, argv);
    //    init();
    //    glutDisplayFunc(drawScene);
    //    glutMainLoop();*/
    return 0;
}
