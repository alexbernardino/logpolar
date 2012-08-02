/// Usage: [periph, fovea] = logmap(input)
/// Arguments: input - 2D array
///            output - 2D array

#include "mex.h"
#include "CLogPolarCam.h" 

class CMySensor : public CLogPolarCam
{
public:
	void logmap_matlab(double *cart, double *log, double *fov)
	{
		_logmap_columnfirstorder(cart, log, fov);
	}
};

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    // check correct number of arguments
    if( (nlhs!=2) || (nrhs!=1) )
		mexErrMsgTxt("Please use as: [<periphery_pixels>, <fovea_pixels>] = logmap(<input_img>)\n");
		
	// check correct number of dimensions
    if( mxGetNumberOfDimensions(prhs[0]) != 2)
		mexErrMsgTxt("Input matrix must be 2-dimensional\n");
		
	// check if data type is double
	
	if(!mxIsDouble(prhs[0]))
		mexErrMsgTxt("Input matrix must be of type double\n");
		
	// check valid sizes
	const mwSize* psz_in =  mxGetDimensions(prhs[0]);
	int rows, cols;
	rows = psz_in[0];
	cols = psz_in[1];
	if( ( rows < 2) || ( cols < 2) )
		mexErrMsgTxt("Input matrix should be proper (lines and columns bigger than 1) \n");
		
	//Create log polar sensor
	CMySensor cam;
	cam.put_image_frame_columns(cols);
	cam.put_image_frame_lines(rows);
	cam.put_sensor_lines(64);
	cam.put_sensor_columns(32);
	cam.put_rho_min(5);
	cam.create_logpolar_sensor_columnfirstorder();
	long fov_pix;
	cam.get_fovea_pix(&fov_pix);
	
	plhs[1] = mxCreateDoubleMatrix(1,fov_pix, mxREAL);
	plhs[0] = mxCreateDoubleMatrix(64, 32, mxREAL);
	
	double *pin, *pout_periph, *pout_fov;
	
	pin = mxGetPr(prhs[0]);
	pout_periph = mxGetPr(plhs[0]);
	pout_fov = mxGetPr(plhs[1]);
	
	cam.logmap_matlab(pin, pout_periph, pout_fov);
	
    return;
}