/// Usage: closelps(<handle>)
/// Arguments: handle - pointer to the logpolar sensor (previously created with openlps) 

#include "mex.h"
#include "mlps.h" 

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    // check correct number of arguments
    if( (nlhs!=0) || (nrhs!=1) )
		mexErrMsgTxt("Please use as: [<periphery_pixels>, <fovea_pixels>] = logmap(<input_img>)\n");
		
	// check correct handle type
	if(!mxIsUint64(prhs[0]))
		mexErrMsgTxt("Invalid handle\n");
	if(mxIsEmpty(prhs[0]))
		mexErrMsgTxt("Invalid handle\n");
	
	//get handle
	unsigned long long *pointer = (unsigned long long*)mxGetData(prhs[0]);
	CMatlabLogPolarCam *cam = (CMatlabLogPolarCam *)pointer[0];
	if( cam->id != CMATLABLOGPOLARCAMHANDLEID )
		mexErrMsgTxt("Invalid object\n");
		
	//deallocate memory
	cam->id = NULL;
	cam->destroy_logpolar_sensor();
	delete cam;
	
    return;
}