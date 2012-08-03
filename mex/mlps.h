#ifndef MLPS_H
#define MLPS_H

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
	void invmap_matlab(double *log, double *fov, double *cart)
	{
		_invmap_columnfirstorder(log, fov, cart);
	}
};

#endif // MLPS_H