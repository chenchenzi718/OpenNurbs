
#include "..\..\opennurbs-7.x\opennurbs_public.h"
#include<fstream>
#include <iostream>

#include"trim.h"
#include"NurbsSurface.h"

using namespace std;

constexpr auto SWITCH = true;

int main()
{
	ON::Begin();
	
	int count = -1;//¼ÇÂ¼brepµÄ±àºÅ
	string filename1 = "C:\\Users\\ChenZehao\\Documents\\opennurbs\\Bezier_cv.txt";
	string filename2 = "C:\\Users\\ChenZehao\\Documents\\opennurbs\\Bezier_order.txt";
	ofstream outfile_cv, outfile_order;
	outfile_cv.open(filename1, ios::out);
	outfile_order.open(filename2, ios::out);

	ONX_Model model;
	model.Read("C:\\Users\\ChenZehao\\Documents\\opennurbs\\opennurbs-7.x\\example_files\\V7\\v7_rhino_logo_nurbs.3dm");

	ONX_Model model_cylinder;
	model_cylinder.Read("C:\\Users\\ChenZehao\\Documents\\opennurbs\\test2.3dm");

	
	const wchar_t* filename_3dm = L"C:\\Users\\ChenZehao\\Documents\\opennurbs\\test1.3dm";
	const wchar_t* filename_mynurbs_3dm = L"C:\\Users\\ChenZehao\\Documents\\opennurbs\\test2.3dm";
	const wchar_t* filename_mycylinder_3dm = L"C:\\Users\\ChenZehao\\Documents\\opennurbs\\test3.3dm";


	ONX_ModelComponentIterator it(model, ON_ModelComponent::Type::ModelGeometry);
	//ONX_ModelComponentIterator it(model_cylinder, ON_ModelComponent::Type::ModelGeometry);
	const ON_ModelComponent* model_component = nullptr;
	for (model_component = it.FirstComponent(); nullptr != model_component; model_component = it.NextComponent())
	{
		auto geo_component = ON_ModelGeometryComponent::Cast(model_component);
		if (nullptr == geo_component)
			continue;

		const ON_Geometry* geometry = geo_component->Geometry(nullptr);
		if (nullptr == geometry)
			continue;

		switch (geometry->ObjectType())
		{
		case ON::brep_object:
		{
			const ON_Brep* brep = ON_Brep::Cast(geometry);

			count++;

			// Surface
			ON_TextLog error_log;


			if (count == 1)
				GetNurbsSurfaceInfo(brep, count, filename_3dm, error_log);
			
			//GetNurbsToBezierSurfaceInfo(brep, outfile_cv, outfile_order, count);

			

			// Trim		
			//if (count == 1)
			//	GetTrimInformation(brep, count, filename_3dm, error_log);
			
			break;
		}
		default:
			break;
		}
		cout << endl << endl << endl;
	}

	outfile_cv.close();
	outfile_order.close();

	//cout << count + 1 << endl;

	/*
	ON_TextLog error_log;
	if (NurbsSurfaceCreate(filename_mynurbs_3dm, error_log))
	{
		cout << "create successfully" << endl;
	}
	else
	{
		cout << "create false!" << endl;
	}
	*/

	ON::End();
	return 0;
}