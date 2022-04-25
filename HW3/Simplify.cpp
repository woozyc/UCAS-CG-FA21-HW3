#include "HW3.h"

vector<VertexPair> costheap;

int costbound;

bool readObj()
{
	cout << "Reading mesh......" << endl;
	if (!IO::read_mesh(mesh, objpath))
	{
		cerr << "Error: cannot open mesh file " << objpath << endl;
		return 0;
	}
	return 1;
}

bool saveObj(float ratio)
{
	cout << "Saving mesh......" << endl;
	string main_name = "dragon";
	string ext_name = ".obj";
	string r_name;
	string outfile = outpath;
	ostringstream ratio_name;
	ratio_name.precision(3);
	ratio_name << ratio;

	r_name = ratio_name.str();
	outfile.append(main_name);
	outfile.append(1, '_');
	outfile.append(r_name);
	outfile.append(1, '_');
	outfile.append(ext_name);
	if (!IO::write_mesh(mesh, outfile.c_str()))
	{
		cerr << "Error: cannot save mesh to " << outpath << endl;
		return 0;
	}
	return 1;
}

//compute the Q matrix of a vertex
Matrix4f ComputeQ(VertexHandle p)
{
	Matrix4f Q_p = MatrixXf::Zero(4, 4);
	Matrix4f K_p;
	Mesh::VertexFaceIter vf_it, vf_itend(mesh.vf_end(p));
	for (vf_it = mesh.vf_iter(p); vf_it != vf_itend; ++vf_it)
	{
		Vector4f plane;
		if (mesh.status(*vf_it).deleted())
			continue;
		plane(0, 0) = mesh.normal(*vf_it).data()[0];
		plane(1, 0) = mesh.normal(*vf_it).data()[1];
		plane(2, 0) = mesh.normal(*vf_it).data()[2];
		plane(3, 0) = -(((Vector3f)mesh.point(p).data()).transpose() * (Vector3f)(mesh.normal(*vf_it).data()))[0];
		K_p = plane * plane.transpose();
		Q_p += K_p;
	}
	mesh.data(p).set_Q(Q_p);
	return Q_p;
}

//compute cost of a vertex pair
VertexPair GetCost(VertexHandle p1, VertexHandle p2, HalfedgeHandle he)
{
	Matrix4f M;
	Vector4f b;
	Vector4f opt_V;
	VertexPair PairResult;

	//M * opt_V = b
	M = mesh.data(p1).get_Q() + mesh.data(p2).get_Q();
	M(3, 0) = 0, M(3, 1) = 0, M(3, 2) = 0, M(3, 3) = 1;
	b(0, 0) = 0, b(1, 0) = 0, b(2, 0) = 0, b(3, 0) = 1;

	double d = M.determinant();
	if (d)//M is invertable
	{
		opt_V = M.inverse() * b;
	}
	else//M is not invertable
	{
		opt_V.block(0,0,3,1) = ((Vector3f)mesh.point(p1).data() + (Vector3f)mesh.point(p1).data()) / 2;
		opt_V(3,0) = 1;
	}

	double cost = opt_V.transpose() * M * opt_V;
	/*cout << "p1:    " << mesh.point(p1).data()[0] << "," << mesh.point(p1).data()[1] << "," << mesh.point(p1).data()[2] << endl;
	cout << "p2:    " << mesh.point(p2).data()[0] << "," << mesh.point(p2).data()[1] << "," << mesh.point(p2).data()[2] << endl;
	cout << "p_best:" << opt_V(0, 0) << "," << opt_V(1, 0) << "," << opt_V(2, 0) << endl;*/

	PairResult.cost = cost;
	PairResult.p1 = p1;
	PairResult.p2 = p2;
	PairResult.he = he;
	PairResult.p_best[0] = opt_V(0, 0);
	PairResult.p_best[1] = opt_V(1, 0);
	PairResult.p_best[2] = opt_V(2, 0);
	return PairResult;
}

//used to sort the VertexPair heap
bool CompareVertexPair(const VertexPair& a, const VertexPair& b)
{
	return a.cost > b.cost;
}

//build a min VertexPair heap based on the cost
void MakeHeap()
{
	Mesh::EdgeIter e_it, e_itend(mesh.edges_end());
	HalfedgeHandle he;
	VertexPair vp;

	mesh.update_normals();
	costheap.clear();

	Mesh::VertexIter v_it, v_itend(mesh.vertices_end());
	for (v_it = mesh.vertices_begin(); v_it != v_itend; v_it++)
		ComputeQ(*v_it);
	for (e_it = mesh.edges_begin(); e_it != e_itend; e_it++)
	{
		he = mesh.halfedge_handle(*e_it, 0);
		vp = GetCost(mesh.to_vertex_handle(he), mesh.from_vertex_handle(he), he);
		costheap.push_back(vp);
	}
	//make_heap(costheap.begin(), costheap.end(), CompareVertexPair);
	sort(costheap.begin(), costheap.end(), CompareVertexPair);
	costbound = 0.0 * costheap.size();
}
/*
void MakeHeap()
{
	Mesh::VertexIter v_it, v_itend(mesh.vertices_end());
	Mesh::VertexVertexIter vv_it, vv_itend;
	VertexPair vp;

	for (v_it = mesh.vertices_begin(); v_it != v_itend; v_it++)
	{
		vv_itend = mesh.vv_end(*v_it);
		for (vv_it = mesh.vv_begin(*v_it); vv_it != vv_itend; vv_it++)
		{
			if (*v_it == *vv_it)
				continue;
			vp = GetCost(*v_it, *vv_it);
			costheap.push_back(vp);
		}
	}
	make_heap(costheap.begin(), costheap.end(), CompareVertexPair);
}
*/

//delete top VertexPair in heap
bool DeleteVertex()
{
	//1.get delete info from heap
	//pop_heap(costheap.begin(), costheap.end());
	//VertexPair toDelete = costheap.back();
	if (costheap.size() <= costbound)
	{
		cout << "\r(Rebuilding heap...)";
		MakeHeap();
	}
	VertexPair toDelete = costheap.back();
	costheap.pop_back();
	if (toDelete.p1 == toDelete.p2)
		return 0;

	//2.collapse the edge between two points to p1,
	//then set p1 position to v_best
	if (!mesh.is_collapse_ok(toDelete.he))
		return 0;
	mesh.collapse(toDelete.he);
	/*
	Mesh::VertexIHalfedgeIter vih_it, vih_itend(mesh.vih_end(toDelete.p1));
	for (vih_it = mesh.vih_begin(toDelete.p1); vih_it != vih_itend; vih_it++)
	{//get the halfedge from p2 to p1
		if (mesh.from_vertex_handle(*vih_it) == toDelete.p2)
			break;
	}
	if (vih_it == vih_itend || !vih_it.is_valid() ||
		mesh.from_vertex_handle(*vih_it) != toDelete.p2 || mesh.to_vertex_handle(*vih_it) == toDelete.p1)
		return 0;
	mesh.collapse(*vih_it);//p2 and the edge have been deleted*/
	mesh.set_point(toDelete.p1, toDelete.p_best);
	//mesh.update_face_normals();
	//mesh.update_normals();

	//3.update the costheap
	//delete all vertexpairs related to p1 and p2 in costheap
	vector<VertexPair>::iterator VP_it;
	for (VP_it = costheap.begin(); VP_it != costheap.end(); )
	{
		if (VP_it->p1 == toDelete.p1 || VP_it->p1 == toDelete.p2
			|| VP_it->p2 == toDelete.p1 || VP_it->p2 == toDelete.p2)
		{
			VP_it = costheap.erase(VP_it);
		}
		else
		{
			VP_it++;
		}
	}
	//make_heap(costheap.begin(), costheap.end(), CompareVertexPair);

	//update Q value
/*	ComputeQ(toDelete.p1);
	Mesh::VertexVertexIter vv_it, vv_itend(mesh.vv_end(toDelete.p1));
	for (vv_it = mesh.vv_begin(toDelete.p1); vv_it != vv_itend; vv_it++)
		ComputeQ(*vv_it);

	//add new vertexpairs to costheap

	//Mesh::VertexVertexIter vv_it, vv_itend(mesh.vv_end(toDelete.p1));
	VertexPair VP;
	Mesh::VertexIHalfedgeIter vih_it, vih_itend(mesh.vih_end(toDelete.p1));
	for (vih_it = mesh.vih_begin(toDelete.p1); vih_it != vih_itend; vih_it++)
	{
		if (mesh.status(mesh.from_vertex_handle(*vih_it)).deleted())
			continue;
		ComputeQ(mesh.from_vertex_handle(*vih_it));
		VP = GetCost(toDelete.p1, mesh.from_vertex_handle(*vih_it), *vih_it);
		costheap.push_back(VP);
		//push_heap(costheap.begin(), costheap.end());
	}
	
	for (vv_it = mesh.vv_begin(toDelete.p1); vv_it != vv_itend; vv_it++)
	{
		if (toDelete.p1 == *vv_it)
			continue;
		VP = GetCost(toDelete.p1, *vv_it, );
		costheap.push_back(VP);
	}
	sort(costheap.begin(), costheap.end(), CompareVertexPair);*/

	return 1;
}

bool SimplifyMesh(float ratio)
{
	cout << "===============================================" << endl;
	cout << "          Mesh simplification started" << endl;
	cout << "===============================================" << endl;
	clock_t a, b;
	a = time(NULL);
	if (ratio <= 0.2f)
		cout << "Warning: Ratio is too low!\n    May cause loss of mesh geometry or simplification error!" << endl;

	mesh.request_vertex_normals();
	mesh.request_face_normals();
	mesh.update_normals();
	mesh.request_vertex_status();
	mesh.request_edge_status();
	mesh.request_face_status();
	/*cout << "Computing vertices' Q values......" << endl;
	Mesh::VertexIter v_it, v_itend(mesh.vertices_end());
	for (v_it = mesh.vertices_begin(); v_it != v_itend; v_it++)
		ComputeQ(*v_it);*/

	cout << "Making Heap......" << endl;
	MakeHeap();

	cout << "Deleting......" << endl;
	int opt_faces = mesh.n_faces() * ratio;
	int n = mesh.n_faces();
	printf("Target face number: %6d\n", opt_faces);
	while (n > opt_faces)
	{	
		printf("\rCurrent face number:%6d", n);
		if(DeleteVertex())
			n -= 2;
	}
	mesh.garbage_collection();
	printf("\rFinal face number:  %6d\n", mesh.n_faces());

	mesh.release_vertex_normals();
	mesh.release_face_normals();
	mesh.release_vertex_status();
	mesh.release_edge_status();
	mesh.release_face_status();

	b = time(NULL);
	float cputime = (float)(b - a);
	cout << "===============================================" << endl;
	cout << "         Mesh simplification finished!" << endl;
	cout << "===============================================" << endl;
	cout << "Cputime = " << cputime << endl;
	return true;
}