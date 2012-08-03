/// Usage: [periph_img, fovea_pix] = logmap(handle, input_img)
/// Arguments:input:  handle - pointer to the logpolar sensor (previously created with openlps) 
///           output: periph_image - matrix with image in logpolar coordinates
///                   fovea_pix - array with the fovea pixels (not topologically organized)

#include "mex.h"
#include "mlps.h" 


void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    // check correct number of arguments
    if( (nlhs!=2) || (nrhs!=2) )
		mexErrMsgTxt("Please use as: [<periph_img>, <fovea_pix>] = logmap(<sensor_handle>, <input_img>)\n");
	
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
	
	// check image
    if( mxGetNumberOfDimensions(prhs[1]) != 2)
		mexErrMsgTxt("Input matrix must be 2-dimensional\n");
		
	// check if data type is double
	if(!mxIsDouble(prhs[1]))
		mexErrMsgTxt("Input matrix must be of type double\n");
		
	// check coherent sizes
	const mwSize* psz_in =  mxGetDimensions(prhs[1]);
	int rows, cols;
	rows = psz_in[0];
	cols = psz_in[1];
	if( ( rows != cart_lines) || ( cols !=  cart_cols) )
		mexErrMsgTxt("Input matrix must have dimensions coherent with log polar sensor specification\n");
	
	plhs[0] = mxCreateDoubleMatrix(log_lines, log_cols, mxREAL);
	plhs[1] = mxCreateDoubleMatrix(1,fov_pix, mxREAL);
	
	double *pin, *pout_periph, *pout_fov;
	
	pin = mxGetPr(prhs[1]);
	pout_periph = mxGetPr(plhs[0]);
	pout_fov = mxGetPr(plhs[1]);
	
	cam->logmap_matlab(pin, pout_periph, pout_fov);
	
    return;
}