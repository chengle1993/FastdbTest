#pragma once

namespace idb
{
	struct precision_x
	{
		double PRECISION1;
		double PRECISION2;
		double PRECISION3;
		double PRECISION4;
		double PRECISION5;
		double PRECISION6;

		precision_x()
		{
			PRECISION1 = 0.1;
			PRECISION2 = 0.01;
			PRECISION3 = 0.001;
			PRECISION4 = 0.0001;
			PRECISION5 = 0.00001;
			PRECISION6 = 0.000001;
		}

	}precision;	
}