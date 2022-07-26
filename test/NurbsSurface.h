#pragma once


#include "..\..\opennurbs-7.x\opennurbs_public.h"
#include<iostream>
#include<fstream>

using namespace std;


#define INTERNAL_INITIALIZE_MODEL(model) Internal_SetExampleModelProperties(model,OPENNURBS__FUNCTION__,__FILE__)



inline static void Internal_SetExampleModelProperties(
	ONX_Model& model,
	const char* function_name,
	const char* source_file_name
)
{
	const bool bHaveFunctionName = (nullptr != function_name && 0 != function_name[0]);
	if (!bHaveFunctionName)
		function_name = "";

	const bool bHaveFileName = (nullptr != source_file_name && 0 != source_file_name[0]);
	if (!bHaveFileName)
		source_file_name = "";

	model.m_sStartSectionComments = "This was file created by OpenNURBS toolkit example code.";

	// set application information
	const ON_wString wide_function_name(function_name);
	const ON_wString wide_source_file_name(source_file_name);
	model.m_properties.m_Application.m_application_name
		= bHaveFunctionName
		? ON_wString::FormatToString(L"OpenNURBS toolkit Example: %ls() function", static_cast<const wchar_t*>(wide_function_name))
		: ON_wString(L"OpenNURBS Examples");

	model.m_properties.m_Application.m_application_URL = L"http://www.opennurbs.org";
	model.m_properties.m_Application.m_application_details
		= bHaveFileName
		? ON_wString::FormatToString(L"Opennurbs examples are in the file %ls.", static_cast<const wchar_t*>(wide_source_file_name))
		: ON_wString::FormatToString(L"Opennurbs examples are example_*.cpp files.");

	// some notes
	if (bHaveFunctionName && bHaveFileName)
	{
		model.m_properties.m_Notes.m_notes
			= ON_wString::FormatToString(
				L"This .3dm file was made with the OpenNURBS toolkit example function %s() defined in source code file %ls.",
				static_cast<const wchar_t*>(wide_function_name),
				static_cast<const wchar_t*>(wide_source_file_name));
		model.m_properties.m_Notes.m_bVisible = model.m_properties.m_Notes.m_notes.IsNotEmpty();
	}

	// set revision history information
	model.m_properties.m_RevisionHistory.NewRevision();
}




inline static bool Internal_WriteExampleModel(
	const ONX_Model& model,
	const wchar_t* filename,
	ON_TextLog& error_log
)
{
	int version = 0;

	// writes model to archive
	return model.Write(filename, version, &error_log);
}




//得到Nurbs曲面的信息
inline void GetNurbsSurfaceInfo(const ON_Brep* brep,int count,const wchar_t* filename, ON_TextLog& error_log)
{
	unsigned int num_surface = brep->m_S.Count();
	//std::cout << "num_surface = " << num_surface << std::endl;

	ONX_Model model;

	INTERNAL_INITIALIZE_MODEL(model);

	for (auto s_index = 0; s_index < num_surface; s_index++)
	{

		if (brep->m_S[s_index]->HasNurbForm() )    //稍作修改只打出一个曲面先count==1 && s_index==1 &&
		{
			ON_NurbsSurface nurbs_surface;


			if (brep->m_S[s_index]->GetNurbForm(nurbs_surface) && s_index == 0)
			{
				//给出了u方向的nurbs阶与v方向的nurbs阶
				int order0 = nurbs_surface.m_order[0];
				int order1 = nurbs_surface.m_order[1];
				std::cout << "order0=" << order0 << ",order1=" << order1 << std::endl << endl;

				/*
				if (nurbs_surface.IsClosed(0))
					cout << "the surface in u dir is closed" << endl;
				else
					cout << "the surface in u dir isn't closed" << endl;

				if (nurbs_surface.IsClosed(1))
					cout << "the surface in v dir is closed" << endl;
				else
					cout << "the surface in v dir isn't closed" << endl;
				*/


				//knot
				const double* u_array;
				const double* v_array;

				int u_size = nurbs_surface.KnotCount(0);
				int v_size = nurbs_surface.KnotCount(1);

				u_array = nurbs_surface.Knot(0);
				v_array = nurbs_surface.Knot(1);


				//u方向的节点向量
				cout << "the knot vector of u dir is:" << nurbs_surface.KnotCount(0) + 2 << endl;

				cout << "u=[" << u_array[0] << " ";
				for (int i = 0; i < u_size; i++)
					cout << u_array[i] << " ";
				cout << u_array[u_size - 1] << "];" << endl;


				//v方向的节点向量
				cout << endl << "the knot vector of v dir is:" << nurbs_surface.KnotCount(1) + 2 << endl;

				cout << "v=[" << v_array[0] << " ";
				for (int i = 0; i < v_size; i++)
					cout << v_array[i] << " ";
				cout << v_array[v_size - 1] << "];" << endl << endl;


				//cvnum
				auto m = nurbs_surface.m_cv_count[0];
				auto n = nurbs_surface.m_cv_count[1];

				cout << endl << "the num of cv in u dir:" << m << endl;
				cout << "the num of cv in v dir:" << n << endl;

				//cout << "the control points is:" << endl;
				cout << "cv_point=[";
				for (int i = 0; i < m; i++)
				{
					for (int j = 0; j < n; j++)
					{
						ON_4dPoint cv_point;
						if (nurbs_surface.GetCV(i, j, cv_point))
							printf("%lf,%lf,%lf,%lf\n", cv_point[0], cv_point[1], cv_point[2], cv_point[3]);
					}
					if (i != m - 1)
					{
						cout << ";" << endl;
					}
				}
				cout << "];" << endl << endl << endl;

				cout << "/////////" << endl << endl << endl;


			    model.AddModelGeometryComponent(&nurbs_surface, nullptr);

				//auto onsurface_point = nurbs_surface.PointAt(u_array[m-1], 5);
				//printf("点值为:(%f,%f,%f)\n", onsurface_point.x, onsurface_point.y, onsurface_point.z);
				//auto normal_point = nurbs_surface.NormalAt(u_array[m-1], 5);
				//printf("法向为:(%f,%f,%f)\n", normal_point.x, normal_point.y, normal_point.z);
				//cout << endl << endl << endl;
			}

			/*
			auto onsurface_point = nurbs_surface.PointAt(0.0,5.0/1024.0);
			printf("点值为:(%f,%f,%f)\n",onsurface_point.x, onsurface_point.y, onsurface_point.z);
			auto normal_point = nurbs_surface.NormalAt(0.0, 5.0/1024.0);
			printf("法向为:(%f,%f,%f)\n",normal_point.x, normal_point.y, normal_point.z);
			cout << endl << endl << endl;
			*/

			
		}
        
	}

	Internal_WriteExampleModel(model, filename, error_log);

	cout << "///////////////////////////////////////////////////////////////" << endl;
}




// 得到一个Brep中将Nurbs转化为Bezier之后的曲面,count记录是第几个brep
inline void GetNurbsToBezierSurfaceInfo(const ON_Brep* brep, ofstream& outfile_cv, ofstream& outfile_order,int count)
{
	unsigned int num_surface = brep->m_S.Count();

	for (auto s_index = 0; s_index < num_surface; s_index++)
	{

		if (brep->m_S[s_index]->HasNurbForm() && count==1 && s_index==1)    //稍作修改只打出一个曲面先count==1 && s_index==1 &&
		{
			ON_NurbsSurface nurbs_surface;


			if (brep->m_S[s_index]->GetNurbForm(nurbs_surface))
			{
				//给出了u方向的nurbs阶与v方向的nurbs阶
				int order0 = nurbs_surface.m_order[0];
				int order1 = nurbs_surface.m_order[1];
				//std::cout << "order0=" << order0 << ",order1=" << order1 << std::endl << endl;


				//knot
				const double* u_array;
				const double* v_array;

				int u_size = nurbs_surface.KnotCount(0);
				int v_size = nurbs_surface.KnotCount(1);

				u_array = nurbs_surface.Knot(0);
				v_array = nurbs_surface.Knot(1);


				//cvnum
				auto m = nurbs_surface.m_cv_count[0];
				auto n = nurbs_surface.m_cv_count[1];

				

				outfile_order << count << " " << s_index << endl;
				outfile_cv << count << " " << s_index << endl;

				int count_m = -1;
				for (int span_idx0 = 0; span_idx0 <= m - order0; span_idx0++)
				{
					if (u_array[span_idx0 + order0 - 2] < u_array[span_idx0 + order0 - 1])
					{
						count_m++;
						int count_n = -1;
						for (int span_idx1 = 0; span_idx1 <= n - order1; span_idx1++)
						{
							if (v_array[span_idx1 + order1 - 2] < v_array[span_idx1 + order1 - 1])
							{
								count_n++;
								//outfile_order << count_m << " " << count_n << endl;
								outfile_cv << count_m << " " << count_n << endl;

								ON_BezierSurface my_bezier;
								nurbs_surface.ConvertSpanToBezier(span_idx0, span_idx1, my_bezier);


								//将


								auto u_order = my_bezier.m_order[0];
								auto v_order = my_bezier.m_order[1];
								if (count_m == 0 && count_n == 0)
								{
									//cout << "u方向的阶为:" << u_order << endl;
									//cout << "v方向的阶为:" << v_order << endl;

									outfile_order << u_order << " " << v_order << endl << endl;
								}

								//cout << endl << "此时Bezier的控制点为:" << endl;
								for (int i = 0; i < u_order; i++)
								{
									for (int j = 0; j < v_order; j++)
									{
										ON_4dPoint cv;
										if (my_bezier.GetCV(i, j, cv))
										{
											//printf("(%lf,%lf,%lf,%lf)\n", cv[0], cv[1], cv[2], cv[3]);
											outfile_cv << cv[0] << " " << cv[1] << " " << cv[2] << " " << cv[3] << endl;
										}
									}
								}
							}
						}
					}
				}
				outfile_cv << endl;


			}
		}
	}
}



//利用opennurbs中的knotInsertion做曲面转化,但是不太成功
//因为opennurbs节点插入算法规定knot(order-2)<input<knot(m-1)
inline void NurbsKnotInsertionToBezier(ON_NurbsSurface& new_nurbs_surface, ON_NurbsSurface nurbs_surface)
{


	new_nurbs_surface = nurbs_surface;


	//给出了u方向的nurbs阶与v方向的nurbs阶
	int order0 = nurbs_surface.m_order[0];
	int order1 = nurbs_surface.m_order[1];
	//std::cout << "order0=" << order0 << ",order1=" << order1 << std::endl << endl;


	//knot
	const double* u_array;
	const double* v_array;

	int u_size = nurbs_surface.KnotCount(0);
	int v_size = nurbs_surface.KnotCount(1);

	u_array = nurbs_surface.Knot(0);
	v_array = nurbs_surface.Knot(1);


	//cvnum
	auto m = nurbs_surface.m_cv_count[0];
	auto n = nurbs_surface.m_cv_count[1];


	//向u方向插入
	int temp = order0 - 2;
	for (int i = order0 - 1; i < m - 1; i++)
	{
		if (fabs(u_array[i] - u_array[order0 - 2]) < 1e-10 || fabs(u_array[i] - u_array[m - 1]) < 1e-10)
		{
			continue;
		}

		if (fabs(u_array[i] - u_array[temp] < 1e-10))
		{
			continue;
		}

		int multi = nurbs_surface.KnotMultiplicity(0, i);

		if (multi < order0 - 1)
		{
			if (!new_nurbs_surface.InsertKnot(0, u_array[i], order0 - 1))
			{
				cout << "fail to insert u knot!" << endl;
				break;
			}
			temp = i;
		}
	}

	//向v方向插入
	temp = order1 - 2;
	for (int i = order1 - 1; i < n - 1; i++)
	{

		if (fabs(v_array[i] - v_array[order1 - 2]) < 1e-10 || fabs(v_array[i] - v_array[n - 1]) < 1e-10)
		{
			continue;
		}

		if (fabs(v_array[i] - v_array[temp] < 1e-10))
		{
			continue;
		}

		int multi = nurbs_surface.KnotMultiplicity(1, i);

		if (multi < order1 - 1)
		{
			if (!new_nurbs_surface.InsertKnot(1, v_array[i], order1 - 1))
			{
				cout << "fail to insert v knot!" << endl;
				break;
			}
			temp = i;
		}
	}


	// new knot
	const double* new_u_array;
	const double* new_v_array;
	int new_u_size = new_nurbs_surface.KnotCount(0);
	int new_v_size = new_nurbs_surface.KnotCount(1);
	//u_array = new double[u_size];
	//v_array = new double[v_size];
	new_u_array = new_nurbs_surface.Knot(0);
	new_v_array = new_nurbs_surface.Knot(1);
	cout << "the new knot vector of u dir is:" << new_nurbs_surface.KnotCount(0) + 2 << endl;
	cout << "u=[" << new_u_array[0] << " ";
	for (int i = 0; i < new_u_size; i++)
		cout << new_u_array[i] << " ";
	cout << new_u_array[new_u_size - 1] << "];" << endl;
	cout << endl << "the new knot vector of v dir is:" << new_nurbs_surface.KnotCount(1) + 2 << endl;
	cout << "v=[" << new_v_array[0] << " ";
	for (int i = 0; i < new_v_size; i++)
		cout << new_v_array[i] << " ";
	cout << new_v_array[new_v_size - 1] << "];" << endl << endl;

	//new_cvpoint
	auto new_m = new_nurbs_surface.m_cv_count[0];
	auto new_n = new_nurbs_surface.m_cv_count[1];

	cout << endl << "the num of newcv in u dir:" << new_m << endl;
	cout << "the num of newcv in v dir:" << new_n << endl;

	//cout << "the control points is:" << endl;
	cout << "new_cv_point=[";
	for (int i = 0; i < new_m; i++)
	{
		for (int j = 0; j < new_n; j++)
		{
			ON_4dPoint cv_point;
			if (new_nurbs_surface.GetCV(i, j, cv_point))
				printf("%lf,%lf,%lf ", cv_point[0], cv_point[1], cv_point[2]);
		}
		if (i != new_m - 1)
		{
			cout << ";" << endl;
		}
	}
	cout << "];" << endl << endl << endl;

	// 验证插入正确性
	double input_u = new_u_array[order0 - 1];
	double input_v = new_v_array[order1 - 1];
	auto output = nurbs_surface.PointAt(input_u, input_v);
	auto new_output = new_nurbs_surface.PointAt(input_u, input_v);
	auto output_diff = nurbs_surface.NormalAt(input_u, input_v);
	auto new_output_diff = new_nurbs_surface.NormalAt(input_u, input_v);
	printf("(%.9lf,%.9lf,%.9lf),(%.9lf,%.9lf,%.9lf)\n", output[0], output[1], output[2], new_output[0], new_output[1], new_output[2]);
	printf("(%.9lf,%.9lf,%.9lf),(%.9lf,%.9lf,%.9lf)\n", output_diff[0], output_diff[1], output_diff[2], new_output_diff[0], new_output_diff[1], new_output_diff[2]);
}



inline static bool NurbsSurfaceCreate(const wchar_t* filename, ON_TextLog& error_log)
{
	ONX_Model model;
	INTERNAL_INITIALIZE_MODEL(model);

	const int bIsRational = false;
	const int dim = 3;
	const int u_degree = 1;
	const int v_degree = 2;
	const int u_cv_count = 3;
	const int v_cv_count = 4;

	double u_knot[u_cv_count + u_degree - 1];
	double v_knot[v_cv_count + v_degree - 1];

	u_knot[0] = 0.0;
	u_knot[1] = 0.5;
	u_knot[2] = 1.0;

	v_knot[0] = v_knot[1] = 0.0;
	v_knot[2] = 0.5;
	v_knot[3] = v_knot[4] = 1.0;


	ON_3dPoint CV[u_cv_count][v_cv_count];

	CV[0][0] = ON_3dPoint(-14.753541, 3.100204, 12.569305);
	CV[0][1] = ON_3dPoint(-14.753541, 6.591857, 4.650172);
	CV[0][2] = ON_3dPoint(-14.753541, 7.137687, 15.883056);
	CV[0][3] = ON_3dPoint(-14.753541, 3.100204, 12.569305);

	CV[1][0] = ON_3dPoint(-9.534034, 4.312491, 9.703382);
	CV[1][1] = ON_3dPoint(-9.534034, 7.321024, 2.626922);
	CV[1][2] = ON_3dPoint(-9.534034, 8.321024, 11.626922);
	CV[1][3] = ON_3dPoint(-9.534034, 4.312491, 9.703382);

	CV[2][0] = ON_3dPoint(-14.753541, 3.100204, 12.569305);
	CV[2][1] = ON_3dPoint(-14.753541, 6.591857, 4.650172);
	CV[2][2] = ON_3dPoint(-14.753541, 7.137687, 15.883056);
	CV[2][3] = ON_3dPoint(-14.753541, 3.100204, 12.569305);


	ON_NurbsSurface nurbs_surface(dim, bIsRational,
		u_degree + 1, v_degree + 1,
		u_cv_count, v_cv_count);


	for (int i = 0; i < nurbs_surface.KnotCount(0); i++)
		nurbs_surface.SetKnot(0, i, u_knot[i]);

	for (int j = 0; j < nurbs_surface.KnotCount(1); j++)
		nurbs_surface.SetKnot(1, j, v_knot[j]);

	for (int i = 0; i < nurbs_surface.CVCount(0); i++) {
		for (int j = 0; j < nurbs_surface.CVCount(1); j++) {
			nurbs_surface.SetCV(i, j, CV[i][j]);
		}
	}

	model.AddModelGeometryComponent(&nurbs_surface, nullptr);

	return Internal_WriteExampleModel(model, filename, error_log);
}