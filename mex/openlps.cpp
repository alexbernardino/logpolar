/// Usage: handle = openlps( cart_lines, cart_cols, log_lines, log_cols, rho_min )
/// Arguments: input - cart_lines: number of lines of the cartesian images
///                    cart_cols: number of columns of the cartesian images
///                    log_lines: number of lines of the logpolar images
///                    log_cols: number of coluns of the logpolar images
///                    rho_min: radius of the blind spot 
///            output - handle with the object pointer

#include "mex.h"
#include "mlps.h" 

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    // check correct number of arguments
    if( (nlhs!=1) || (nrhs!=5) )
		mexErrMsgTxt("Please use as: <handle> = openlps( <cart_lines>, <cart_cols>, <log_lines>, <log_cols>, <rho_min> )\n");
		
	// check correct number of dimensions
    if( mxIsEmpty(prhs[0]) || mxIsEmpty(prhs[1]) || mxIsEmpty(prhs[2]) || mxIsEmpty(prhs[3]) || mxIsEmpty(prhs[4]) )
		mexErrMsgTxt("Invalid argument: empty array");
		
	
	double cart_lines = mxGetScalar(prhs[0]);
	double cart_cols = mxGetScalar(prhs[1]);
	double log_lines = mxGetScalar(prhs[2]);
	double log_cols = mxGetScalar(prhs[3]);
	double rho_min =  mxGetScalar(prhs[4]);
		
	// check valid range
	if( ((int)cart_lines < 1) || ((int)cart_cols < 1) || ((int)log_lines < 1) || ((int)log_cols < 1) || (rho_min < 1) )
		mexErrMsgTxt("Error: Argument must be positive\n");
			
	CMatlabLogPolarCam *cam = new CMatlabLogPolarCam();
	if( cam == NULL)
		mexErrMsgTxt("Error: Cannot create object\n");
		
	cam->id = CMATLABLOGPOLARCAMHANDLEID;
	cam->put_image_frame_columns(cart_cols);
	cam->put_image_frame_lines(cart_lines);
	cam->put_sensor_lines(log_lines);
	cam->put_sensor_columns(log_cols);
	cam->put_rho_min(rho_min);
	cam->create_logpolar_sensor_columnfirstorder();
	
	
	plhs[0] = mxCreateNumericMatrix(1,1, mxUINT64_CLASS, mxREAL);
	unsigned long long *pointer = (unsigned long long*)mxGetData(plhs[0]);
	pointer[0] = (unsigned long long)cam;
	
    return;
}