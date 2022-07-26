#pragma once

#include "..\..\opennurbs-7.x\opennurbs_public.h"
#include<iostream>
#include"NurbsSurface.h"

using namespace std;



// 获得一个brep上的trim信息
inline void GetTrimInformation(const ON_Brep* brep, int count, const wchar_t* filename, ON_TextLog& error_log)
{

	unsigned int num_trim = brep->m_T.Count();
	//std::cout << "num_trim = " << num_trim << std::endl;

	//ONX_Model model;

	//INTERNAL_INITIALIZE_MODEL(model);

	for (auto t_index = 0; t_index < num_trim; t_index++)
	{
		const ON_BrepTrim& trim = brep->m_T[t_index];

		//trim曲线属于对应brep上哪一个曲面
		int trimfromSurfaceIdx = trim.SurfaceIndexOf();
		cout << trimfromSurfaceIdx << endl;

		//std::cout << "m_type = " << trim.m_type << std::endl;

		if (trim.Brep()) {
			//std::cout << "HAS BREP" << std::endl;
		}
		else {
			//std::cout << "HAS NO BREP" << std::endl;
		}


		ON_NurbsCurve trim_nurbs_curve;
		if (trim.NurbsCurve(&trim_nurbs_curve)) {
			// 控制点个数
			int num_cv = trim_nurbs_curve.CVCount();
			std::cout << "Able to get trim as NURBS curve : num_cv = " << num_cv << std::endl;


			// 阶数
			int order = trim_nurbs_curve.Order();
			cout << "the order is:" << order << endl;

			// 节点向量数
			int knotsize = trim_nurbs_curve.KnotCount();
			cout << "the number of the knot is:" << knotsize + 2 << endl;


			auto udir = trim_nurbs_curve.Knot();
			cout << "knot=[" << udir[0] << " ";
			for (int i = 0; i < knotsize; i++)
			{
				cout << udir[i] << " ";
			}
			cout << udir[knotsize - 1] << "];" << endl;

			cout << "cv_point=[";
			for (auto tcv_index = 0; tcv_index < num_cv; tcv_index++)
			{
				if (trim_nurbs_curve.IsRational())
				{
					ON_4dPoint cv;

					trim_nurbs_curve.GetCV(tcv_index, cv);
					//printf("Rational CV[%f,%f,%f,%f,%f]\n", cv.x, cv.y, cv.z, cv.w);

					printf("%lf,%lf,%lf,%lf\n", cv.x, cv.y, cv.z, cv.w);
					//std::cout << boost::format("Rational CV [%f,%f,%f,%f]") % cv.x % cv.y % cv.z % cv.w << std::endl;
				}
				else
				{
					ON_3dPoint cv;
					trim_nurbs_curve.GetCV(tcv_index, cv);
					//printf("Non Rational CV[%f,%f,%f]\n", cv.x, cv.y, cv.z);
					printf("%lf,%lf,%lf\n", cv.x, cv.y, cv.z);
					//std::cout << boost::format("Non-rational CV [%f,%f,%f]") % cv.x % cv.y % cv.z << std::endl;

				}
			}

			//auto test = trim_nurbs_curve.PointAt(-3.4567);
			//printf("此时在%f处取值为:[%f,%f,%f]\n", 0.0,test.x,test.y,test.z);
			//auto test_diff = trim_nurbs_curve.TangentAt(-3.4567);
			//printf("此时在%f处取值为:[%f,%f,%f]\n\n", 0.0, test_diff.x, test_diff.y, test_diff.z);
			cout << endl << endl << endl;


			//model.AddModelGeometryComponent(&trim, nullptr);
		}
	}

	//Internal_WriteExampleModel(model, filename, error_log);
}