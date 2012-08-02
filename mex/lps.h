#ifndef LPS_H
#define LPS_H

//Define a logpolar_cam object for use with matlab

#include "CLogPolarCam.h" 

//Arbitrary code for checking handle validity
#define CMATLABLOGPOLARCAMHANDLEID 45235307

class CMatlabLogPolarCam : public CLogPolarCam
{
public:
    int id;
	void logmap_matlab(double *cart, double *log, double *fov)
	{
		_logmap_columnfirstorder(cart, log, fov);
	}
};

#endif