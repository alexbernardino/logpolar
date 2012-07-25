#ifndef __CLOGPOLARCAM_H_
#define __CLOGPOLARCAM_H_

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "CCam.h"
#include "CNonLinearSensor.h"

class CLogPolarCam : public CCam, public CNonLinearSensor
{
private:
	float m_fRhoMin;
	float m_fLogFact;
	int   m_iFoveaPixels;
	float m_fLogShift;
	float m_fFirstRho;
public:
	CLogPolarCam() 
		: pi(3.14159265358979323846f)
		, m_fRhoMin(0.0f) //not initialized
		, m_fLogFact(0.0f)
		, m_fLogShift(0.0f)
		, m_fFirstRho(0.0f)
		, m_bValidData(false)
	{
	}

	virtual ~CLogPolarCam()
	{
	}

	
private:
	// Private Helper Variables	
	float pi;	
	typedef struct list_elem
	{
		 int	cart_index;			//index pixel in cartesian image
		 struct list_elem *next;
	}
	tLPSListElem;
	int *m_num_pix_in_cell;	  //accumulator - number of cartesian pixel falling in log-polar cell
	int m_iNumPixels;         //total number of used cartesian pixels

	int *m_pix_index_array;   //array of indexes of cartesian pixels corresponding to log-polar cell
							  //it is organized sequentially. Size is m_iNumPixels
	int *m_fovea_index_array; //array of indexes of cartesian pixels corresponding to fovea pixels
							  //it is organized sequentially. Size is m_iFoveaPixels

	tLPSListElem **m_pix_index_list; //temporary structure to build m_pix_index_array; dynamically built
	bool m_bValidData;

protected:
	template <class inType, class outType>
	void _logmap(inType *cart, outType *log, outType *fov)
	{
		int i, j, k, index, np, counter = 0;		
		float temp;
		for(i = 0; i < m_sensor_lines; i++) {
			for( j = 0; j < m_sensor_columns; j++) {
				np = m_num_pix_in_cell[ i*m_sensor_columns + j ];
				if(np == 0)
					np = 1;					
				temp = 0.0f;
				for(k = 0; k < np; k++) {
					index = m_pix_index_array[ counter++ ];
					temp += cart[ index ];
				}
				log[ i*m_sensor_columns + j ] = (outType)(temp/np);				
			}
		}

		if( fov != NULL )
		{
			for(i = 0; i < m_iFoveaPixels; i++ )
			{
				index = m_fovea_index_array[i];
				fov[i] = (outType)(cart[index]);
			}
		}
	}

	template <class inType, class outType>
	void _invmap(inType *log, inType *fov, outType *cart)
	{
		int i, j, k, index, np, counter=0;
		for(i = 0; i < m_sensor_lines; i++) {
			for(j = 0; j < m_sensor_columns; j++) {
				np = m_num_pix_in_cell[ i*m_sensor_columns + j ];
				if(np == 0)
					np = 1;
				for(k = 0; k < np; k++) {
					index = m_pix_index_array[ counter++ ];
					cart[ index ] = (outType)log[ i*m_sensor_columns + j ];			
				}
			}
		}
		if( fov != NULL )
		{
			for( i = 0; i < m_iFoveaPixels; i++ )
			{
				index = m_fovea_index_array[i];
				cart[index] = (outType)(fov[i]);
			}
		}
	}

	//OVERLOADING FOR INTERLACED COLOR IMAGES
	template <class inType, class outType>
	void _logmap(inType *cart, outType *log, outType *fov, int nChannels)
	{
		int i, j, k, c, index, np, counter = 0;
		float temp[10];  
		ATLASSERT(nChannels <= 10);
		for(i = 0; i < m_sensor_lines; i++) {
			for(j = 0; j < m_sensor_columns; j++) {
				np = m_num_pix_in_cell[ i*m_sensor_columns + j ];
				if(np == 0)
					np = 1;					
				for(c = 0; c < nChannels; c++) {
					temp[c] = 0.0;
				}
				for(k = 0; k < np; k++) {
					index = m_pix_index_array[ counter++ ];						
					for(c = 0; c < nChannels; c++) {
						temp[c] += cart[ index*nChannels + c ];
					}						
				}
				for(c = 0; c < nChannels; c++) {
					log[ (i*m_sensor_columns+j)*nChannels + c ] = (outType)(temp[c]/np);
				}
			}
		}

		if( fov != NULL )
		{
			for(i = 0; i < m_iFoveaPixels; i++ )
			{
				index = m_fovea_index_array[i];
				//fov[i] = cart[index];
				for(c = 0; c < nChannels; c++) {
					fov[i*nChannels+c] = cart[ index*nChannels + c ];
				}						
			}
		}
	}
	
	template <class inType, class outType>
	void _invmap(inType *log, inType *fov, outType *cart, int nChannels)
	{
		int i, j, k, c, index, np, counter = 0;
		for(i = 0; i < m_sensor_lines; i++) {
			for(j = 0; j < m_sensor_columns; j++) {
				np = m_num_pix_in_cell[ i*m_sensor_columns + j ];
				if(np == 0)
					np = 1;
				for(k = 0; k < np; k++) {
					index = m_pix_index_array[ counter++ ];
					for(c = 0; c < nChannels; c++) {
						cart[ index*nChannels + c ] = (outType)log[ (i*m_sensor_columns+j)*nChannels + c ];			
					}
				}
			}
		}

		if( fov != NULL )
		{
			for( i = 0; i < m_iFoveaPixels; i++ )
			{
				index = m_fovea_index_array[i];
				for(c = 0; c < nChannels; c++) {
					cart[index*nChannels + c] = (outType)(fov[i*nChannels+c]);
				}
			}
		}
	}

private:
	bool compute_encoder_parameters(float sensor_height, float sensor_width, long *angles, long *eccentr, float *rmin, float *logfact, float *logshift, float *firstrho)
	{
		float dim_image;
		int k = 0, m = 0, r = 0, s = 0;

		if( sensor_height <= 0.0f )  return false;  //nothing to do 
		if( sensor_width <= 0.0f )	 return false;

		if(sensor_width < sensor_height) 
			dim_image = sensor_width;
		else 
			dim_image = sensor_height;

		if( *angles <= 0  )	  
			*angles = 64;   // assume a default value

		if( *eccentr > 0 )	 m = 1;
		if( *rmin > 0 )   	 r = 1;
		if( *logfact > 0 )	 k = 1;
		if( *logshift > 0 )  s = 1;

		if( !k && !r && !m && !s)		 //nothing is specified assume logshift = 0;
		{ 
			(*logshift) = 0;
			(*logfact) = 1+2.0f*pi/(*angles);				// ideal value
			(*rmin) = 1/(2*(float)tan(pi/(*angles)));      // ideal value
			(*eccentr) = (int)(log(dim_image/2.0f/(*rmin))/log((*logfact)));
		}
		if( !k && !r && !m && s)		 //logshift is specified - assume rhomin = 0;
		{ 
			(*logfact) = 1+2.0f*pi/(*angles);				// ideal value
			(*rmin) = 0;
			(*eccentr) = (int)(log((dim_image/2.0f+(*logshift))/(*logshift))/log((*logfact)));
		}
		if( !k && r && !m && !s)		 //only min radius is specified
		{
			(*logshift) = 0;
			(*logfact) = 1+2.0f*pi/(*angles);        // ideal value
			(*eccentr) = (int)(log(dim_image/2.0f/(*rmin))/log((*logfact)));
		}
		if( !k && r && !m && !s)		 //min radius and logshift specified
		{
			(*logfact) = 1+2.0f*pi/(*angles);        // ideal value
			(*eccentr) = (int)(log((dim_image/2.0f+(*logshift))/((*rmin)+(*logshift)))/log((*logfact)));
		}
		if( !k && r && m && !s)		//log-fact and logshift not specified 
		{
			(*logshift = 0);
			(*logfact) = (float)exp( log(dim_image/2.0f/(*rmin))/(*eccentr) );
		}
		if( !k && r && m && s)		//only log-fact not specified 
		{
			(*logfact) = (float)exp( log((dim_image/2.0f+(*logshift))/((*rmin)+(*logshift)))/(*eccentr) );
		}
		if( k && !r && !m && !s)		//only log-fact is specified
		{
			(*logshift) = 0;
			(*rmin) = 1/(2*(float)tan(pi/(*angles)));      // ideal value
			(*eccentr) = (int)(log(dim_image/2.0f/(*rmin))/log((*logfact)));
		}
		if( k && !r && !m && !s)		//only log-fact and logshift specified
		{
			(*rmin) = 0;
			(*eccentr) = (int)(log((dim_image/2.0f+(*logshift))/(*logshift))/log((*logfact)));
		}
		if( k && !r && m && !s)		//only min radius is not specified
		{
			(*logshift) = 0;
			(*rmin) = (dim_image/2.0f)/(float)pow((*logfact), (*eccentr));
		}
		if( k && !r && m && s)		//only min radius is not specified
		{
			(*rmin) = (dim_image/2.0f + (*logshift)*(1-(float)pow((*logfact), (*eccentr))))/(float)pow((*logfact), (*eccentr));
		}
		if( k && r && !m && !s)		//eccentr and logshift not specified 
		{
			(*logshift) = 0;
			(*eccentr) = (int)(log(dim_image/2.0/(*rmin))/log((*logfact)));
		}
		if( k && r && !m && s)		//only eccentr not specified 
		{
			(*eccentr) = (int)(log((dim_image/2.0f+(*logshift))/(((*rmin)+(*logshift))))/log((*logfact)));
		}
		if( !k && !r && m && !s)		//only eccentr is specified
		{
			// FOR BALANCED_CELLS
			//(*logfact) = 1+2.0*M_PI/(*angles);
			//(*rmin) = dim_image/2.0/pow((*logfact), (*eccentr));
			// FOR BALANCED INNER RADIUS
			(*logshift) = 0;
			(*rmin) = 1/(2*(float)tan(pi/(*angles)));      // ideal value
			(*logfact) = (float)exp( log(dim_image/2.0/(*rmin))/(*eccentr) );
		}
		if( !k && !r && m && s)		//eccentr and logshift specified
		{
			// FOR BALANCED_CELLS
			//(*logfact) = 1+2.0*M_PI/(*angles);
			//(*rmin) = dim_image/2.0/pow((*logfact), (*eccentr));
			// FOR BALANCED INNER RADIUS
			(*rmin) = 0;
			(*logfact) = (float)exp( log((dim_image/2.0 + (*logshift))/(*logshift))/(*eccentr) );
		}
		if( k & r & m )		   //everything is specified ? Unexpected !
		{
			return false;
		}
		*firstrho = ((*rmin) + (*logshift))*(*logfact) - (*logshift);
		return true;
	}
	bool compute_log_fact(float *log_fact)
	{		
		*log_fact = 0;
		return compute_encoder_parameters(m_f_image_plane_height, m_f_image_plane_width, &m_sensor_lines, &m_sensor_columns, &m_fRhoMin, log_fact, &m_fLogShift, &m_fFirstRho);
	}

public:

	virtual int map_coordinates(float x, float y, float *u, float *v)
	{			
		/*if(!m_bValidData)
		{
			static_cast<_DERIVED_ *>(this)->Error("Object not created");
			return -1;
		}*/

		float rho = (float)sqrt(x*x + y*y);
		float theta = (float)atan2(y,x);
		float csi = (float)log((rho+m_fLogShift)/(m_fRhoMin+m_fLogShift))/(float)log(m_fLogFact);
		float eta = theta/2.0f/pi*m_sensor_lines;

		if(eta < 0.0f)
			eta += m_sensor_lines;
		
		if(eta >= m_sensor_lines) 
			eta -=  m_sensor_lines;
		
		*u = csi;
		*v = eta;

		return 0;
	}

	virtual int invmap_coordinates(float u, float v, float *x, float *y)
	{		
		/*if(!m_bValidData)
		{
			static_cast<_DERIVED_ *>(this)->Error("Object not created");
			return -1;
		}*/

		float rho = (m_fRhoMin+m_fLogShift)*(float)pow(m_fLogFact,u) - m_fLogShift;
		float theta = v*2.0f*pi/m_sensor_lines;
		*x = rho*(float)cos(theta);
		*y = rho*(float)sin(theta);

		return 0;
	}

	/*
	int load_logpolar_sensor(BSTR filename)
	{
		destroy_logpolar_sensor();
		USES_CONVERSION;
		FILE *fp = _tfopen(OLE2T(filename),_T("rb"));
		if( !fp ){
			//static_cast<_DERIVED_ *>(this)->Error("File not found");
			return -1;
		}

		int ints[6];
		float floats[2];
		if( fread(ints, sizeof(int), 6, fp) != 6) {
			//static_cast<_DERIVED_ *>(this)->Error("Invalid file format");
			return -1; 
		}

		long temp;
		temp = ints[0];
		put_image_frame_lines(temp);
		temp = ints[1];
		put_image_frame_columns(temp);
		temp = ints[2];
		put_sensor_lines(temp);
		temp = ints[3];
		put_sensor_columns(temp);

		m_iNumPixels = ints[4];
		m_iFoveaPixels = ints[5];

		if( fread(floats, sizeof(float), 4, fp) != 4){
			static_cast<_DERIVED_ *>(this)->Error("Invalid file format");
			return -1;
		}
		
		m_fRhoMin = floats[0];
		m_fLogFact = floats[1];
		m_fLogShift = floats[2];
		m_fFirstRho = floats[3];

		// constructing sparse matrices
		m_pix_index_array = (int*)calloc(m_iNumPixels, sizeof(int));
		if(m_pix_index_array == NULL) {
			static_cast<_DERIVED_ *>(this)->Error("Out of Memory");
			return E_OUTOFMEMORY; //memory error
		}
		if( fread(m_pix_index_array, sizeof(int), m_iNumPixels, fp) != (unsigned int)m_iNumPixels ){
			free(m_pix_index_array);
			static_cast<_DERIVED_ *>(this)->Error("Invalid file format");
			return -1; 
		}
		
		m_fovea_index_array = (int*)calloc(m_iFoveaPixels, sizeof(int));
		if(m_fovea_index_array == NULL) {
			free(m_pix_index_array);
			static_cast<_DERIVED_ *>(this)->Error("Out of Memory");
			return E_OUTOFMEMORY; //memory error
		}

		if( fread(m_fovea_index_array, sizeof(int), m_iFoveaPixels, fp) != (unsigned int)m_iFoveaPixels ){
			free(m_pix_index_array);
			free(m_fovea_index_array);
			static_cast<_DERIVED_ *>(this)->Error("Invalid file format");
			return -1; 
		}

		m_bValidData = true;

		return 0;
	}
	*/

	/*
	int save_logpolar_sensor(BSTR filename)
	{
		if(!m_bValidData){
			static_cast<_DERIVED_ *>(this)->Error("Sensor not created yet");
			return -1;
		}

		USES_CONVERSION;
		FILE *fp = _tfopen(OLE2T(filename),_T("wb"));
		if( !fp ){
			static_cast<_DERIVED_ *>(this)->Error("Cannot open file");
			return -1;
		}

		int ints[6];
		float floats[4];

		ints[0] = m_i_image_frame_lines;
		ints[1] = m_i_image_frame_columns;
		ints[2] = m_sensor_lines;
		ints[3] = m_sensor_columns;

		ints[4] = m_iNumPixels;
		ints[5] = m_iFoveaPixels;
		
		floats[0] = m_fRhoMin;
		floats[1] = m_fLogFact;
		floats[2] = m_fLogShift;
		floats[3] = m_fFirstRho;


		if( fwrite(ints, sizeof(int), 6, fp) != 6) {
			static_cast<_DERIVED_ *>(this)->Error("Cannot write file");
			return -1; 
		}

		
		if( fwrite(floats, sizeof(float), 4, fp) != 4){
			static_cast<_DERIVED_ *>(this)->Error("Cannot write file");
			return -1;
		}
		
		
		if( fwrite(m_pix_index_array, sizeof(int), m_iNumPixels, fp) != (unsigned int)m_iNumPixels ){
			static_cast<_DERIVED_ *>(this)->Error("Cannot write file");
			return -1; 
		}
		

		if( fwrite(m_fovea_index_array, sizeof(int), m_iFoveaPixels, fp) != (unsigned int)m_iFoveaPixels ){
			static_cast<_DERIVED_ *>(this)->Error("Cannot write file");
			return -1; 
		}

		return 0;
	}
	*/


	int destroy_logpolar_sensor()
	{
		if(m_bValidData)		{
			free(m_num_pix_in_cell);
			free(m_pix_index_array);
			free(m_fovea_index_array);
		}
		m_bValidData = false;
		return 0;
	}

	int create_logpolar_sensor()
	{
		float xi, yi, xm, ym, cr, ca;
		int i,j; //general integer for-loop indexes		
		int dr, da, dx, dy;
		tLPSListElem *ptr, *temp, *fovea_list = NULL;
		
			
		if(m_bValidData)
			destroy_logpolar_sensor();

		m_fLogFact = -1;
		if( compute_encoder_parameters(m_f_image_plane_height, m_f_image_plane_width,
				&m_sensor_lines, &m_sensor_columns, &m_fRhoMin, &m_fLogFact, &m_fLogShift, &m_fFirstRho)==0)	
		{
			//static_cast<_DERIVED_ *>(this)->Error("Invalid Object Properties");
			return -1;
		}

		// memory allocation
		m_num_pix_in_cell = (int*)calloc(m_sensor_lines*m_sensor_columns, sizeof(int));
		if(m_num_pix_in_cell == NULL) {
			//static_cast<_DERIVED_ *>(this)->Error("Out of Memory");
			return -1; //memory error
		}
				
		m_pix_index_list = (tLPSListElem **)calloc( m_sensor_lines*m_sensor_columns, sizeof(tLPSListElem*) );
		if( m_pix_index_list == NULL )	{
			//static_cast<_DERIVED_ *>(this)->Error("Out of Memory");
			return -1;  //memory error
		}

		m_iNumPixels = 0;
		m_iFoveaPixels = 0;
		for( i = 0; i < m_sensor_lines*m_sensor_columns; i++)		{
			m_num_pix_in_cell[i] = 0;
			m_pix_index_list[i] = NULL;
		}

		// filling luts		
		for( i = 0 ; i < m_i_image_frame_lines; i++ )		{
			for( j = 0; j < m_i_image_frame_columns ; j++ )			{				
				//measurements in the center of the pixel
				image_frame_to_pixel(j+0.5f,i+0.5f, &xm, &ym);			
				pixel_to_image_plane(xm, ym, &xm, &ym);							
				map_coordinates(xm, ym, &cr, &ca ); 
				dr = (int)cr;
				da = (int)ca;
				if(dr < 0)    //belongs to fovea
				{
					m_iFoveaPixels++;
					ptr = (tLPSListElem*)calloc(1,sizeof(tLPSListElem));
					if( ptr == NULL )	{	 //out of memory
						//static_cast<_DERIVED_ *>(this)->Error("Out of Memory");
						return -1;
					}
					ptr->cart_index = i*m_i_image_frame_columns + j;
					// insert at beginning of fovea list
					ptr->next = fovea_list;
					fovea_list = ptr;
				}

				if( (dr >= 0) && (dr < m_sensor_columns))
				{
					m_iNumPixels++;
					m_num_pix_in_cell[ da*m_sensor_columns + dr ] += 1;
					ptr = (tLPSListElem*)calloc(1,sizeof(tLPSListElem));
					if( ptr == NULL )	{	 //out of memory
						//static_cast<_DERIVED_ *>(this)->Error("Out of Memory");
						return -1;
					}
					ptr->cart_index = i*m_i_image_frame_columns + j;
					// insert at beginning of list
					ptr->next = m_pix_index_list[ da*m_sensor_columns + dr];
					m_pix_index_list[da*m_sensor_columns + dr] = ptr;
				}
			}
		}
		//dealing with empty cells
		//bool use_left, use_top; // flags for deciding interpolating pixels
		for( i = 0 ; i < m_sensor_lines ; i++ ) {
			for( j = 0 ; j < m_sensor_columns ; j++ )  {
				if( m_num_pix_in_cell[ i*m_sensor_columns + j ] == 0) {
					//compute the closest cartesian pixel
					cr = j+0.5f;
					ca = i+0.5f;
					invmap_coordinates( cr , ca , &xm , &ym );
					image_plane_to_pixel(xm, ym, &xi, &yi);											
					pixel_to_image_frame(xm, ym, &xi, &yi);

					dx = (int)xi;
					dy = (int)yi;
					
					ptr=(tLPSListElem*)calloc(1,sizeof(tLPSListElem));
					if( ptr == NULL ) {  //out of memory
						//static_cast<_DERIVED_ *>(this)->Error("Out of Memory");
						return -1;
					}
					ptr->cart_index = dy*m_i_image_frame_columns + dx;
					ptr->next = m_pix_index_list[ i*m_sensor_columns + j ];
					m_pix_index_list[ i*m_sensor_columns + j] = ptr;
					m_iNumPixels++;	 
				}
			}
		}		
		// constructing sparse matrices
		m_pix_index_array = (int*)calloc(m_iNumPixels, sizeof(int));
		if(m_pix_index_array == NULL) {
			//static_cast<_DERIVED_ *>(this)->Error("Out of Memory");
			return -1; //memory error
		}

		int counter = 0;
		for(i = 0; i < m_sensor_lines; i++) {
			for(j = 0; j < m_sensor_columns; j++) {		
				ptr = m_pix_index_list[ i*m_sensor_columns + j ];
				while(ptr != NULL) {
					m_pix_index_array[counter++] = ptr->cart_index;
					temp = ptr;
					ptr = ptr->next;
					free(temp);
				}	
			}
		}
		free(m_pix_index_list);

		m_fovea_index_array = (int*)calloc(m_iFoveaPixels, sizeof(int));
		if(m_fovea_index_array == NULL) {
			//static_cast<_DERIVED_ *>(this)->Error("Out of Memory");
			return -1; //memory error
		}

		ptr = fovea_list;
		for( i = 0; i < m_iFoveaPixels; i++ )
		{
			m_fovea_index_array[i] = ptr->cart_index;
			temp = ptr;
			ptr = ptr->next;
			free(temp);
		}

		m_bValidData = true;
		
		return 0;
	}

	
	int get_rho_min(float *pVal)
	{
		// TODO: Add your implementation code here
		*pVal = m_fRhoMin;
		return 0;
	}

	int get_fovea_pix(long *pVal)
	{
		// TODO: Add your implementation code here
		*pVal = m_iFoveaPixels;
		return 0;
	}

	int put_rho_min(float newVal)
	{
		// TODO: Add your implementation code here
		if(newVal <= 0.0f)
			return 1;

		m_fRhoMin = newVal;

		//recompute log-fact
		compute_log_fact(&m_fLogFact);
		//static_cast<_DERIVED_ *>(this)->SetDirty(TRUE);
		return 0;
	}

	int get_log_fact(float *pVal)
	{
		// TODO: Add your implementation code here
		*pVal = m_fLogFact;
		return 0;
	}

	int put_log_fact(float newVal)
	{
		// TODO: Add your implementation code here	
		m_fLogFact = newVal;	
		//static_cast<_DERIVED_ *>(this)->SetDirty(TRUE);
		return 0;
	}

	int get_log_shift(float *pVal)
	{
		// TODO: Add your implementation code here
		*pVal = m_fLogShift;
		return 0;
	}

	int put_log_shift(float newVal)
	{
		// TODO: Add your implementation code here	
		m_fLogShift = newVal;	
		//recompute log-fact
		compute_log_fact(&m_fLogFact);
		//static_cast<_DERIVED_ *>(this)->SetDirty(TRUE);
		return 0;
	}

	int get_first_rho(float *pVal)
	{
		// TODO: Add your implementation code here
		*pVal = m_fFirstRho;
		return 0;
	}

	int put_first_rho(float newVal)
	{
		// TODO: Add your implementation code here	
		m_fFirstRho = newVal;	
		//static_cast<_DERIVED_ *>(this)->SetDirty(TRUE);
		return 0;
	}
};

#endif //__CLOGPOLARCAM_H_

