/// Usage: cart_img = invmap(handle, periph_img, fov_pix)
/// Arguments:input:  handle - pointer to the logpolar sensor (previously created with openlps) 
///           		  periph_image - matrix with image in logpolar coordinates
///                   fovea_pix - array with the fovea pixels (not topologically organized)
///           output: cart_img - the reconstructed cartesian image (retinal image)

#include "mex.h"
#include "mlps.h" 


void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    // check correct number of arguments
    if( (nlhs!=1) || (nrhs!=3) )
		mexErrMsgTxt("Please use as: <cart_img> = invmap(<sensor_handle>, <log_img>, <fov_pix>)\n");
	
	// check correct handle type
	if(!mxIsUint64(prhs[0]))
		mexErrMsgTxt("Invalid handle\n");
	if(mxIsEmpty(prhs[0]))
		mexErrMsgTxt("Invalid handle\n");
	
	//get handle
	unsigned long long *pointer = (unsigned long long *)mxGetData(prhs[0]);
	CMatlabLogPolarCam *cam = (CMatlabLogPolarCam *)pointer[0];
	if( cam->id != CMATLABLOGPOLARCAMHANDLEID )
		mexErrMsgTxt("Invalid object\n");
	
	//get sensor parameters
	long cart_lines, cart_cols, log_lines, log_cols, fov_pix;
	cam->get_image_frame_lines(&cart_lines);
	cam->get_image_frame_columns(&cart_cols);
    cam->get_sensor_lines(&log_lines);
	cam->get_sensor_columns(&log_cols);
	cam->get_fovea_pix(&fov_pix);
	
	// check input data
    if( mxGetNumberOfDimensions(prhs[1]) != 2)
		mexErrMsgTxt("Input logpolar image must be 2-dimensional\n");
		
	if( mxGetNumberOfDimensions(prhs[2]) != 1)
		mexErrMsgTxt("Input fovea pixels must be 1-dimensional array \n");
		
		
	// check if data type is double
	if(!mxIsDouble(prhs[1]) || !mxIsDouble(prhs[2]))
		mexErrMsgTxt("Input data must be of type double\n");
		
	// check coherent sizes
	const mwSize* psz_in1 =  mxGetDimensions(prhs[1]);
	int rows, cols;
	rows = psz_in1[0];
	cols = psz_in1[1];
	if( ( rows != log_lines) || ( cols !=  log_cols) )
		mexErrMsgTxt("Input logpolar image must have dimensions coherent with log polar sensor specification\n");
	
	const mwSize* psz_in2 =  mxGetDimensions(prhs[2]);
	int sz;
	sz = psz_in[0];
	if( sz != fov_pix ) )
		mexErrMsgTxt("Input fovea array must have dimensions coherent with log polar sensor specification\n");
	
	
	plhs[0] = mxCreateDoubleMatrix(cart_lines, cart_cols, mxREAL);
	
	double *pin_log, *pin_fov, *pout;
	
	pin_log = mxGetPr(prhs[1]);
	pin_fov = mxGetPr(prhs[2]);
	pout = mxGetPr(plhs[0]);
		
	cam->invmap_matlab(pin_log, pin_fov, pout);
	
    return;
}