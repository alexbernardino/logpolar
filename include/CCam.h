#ifndef __CCAM_H_
#define __CCAM_H_

#include "CHomography.h"
#include "CImg.h"

class CCam 
{
protected:
	long  m_i_image_frame_columns;
	long  m_i_image_frame_lines;
	float m_f_image_plane_width;
	float m_f_image_plane_height;	
	float m_f_pixel_x_size;
	float m_f_pixel_y_size;	
	float m_f_principal_point_x;
	float m_f_principal_point_y;
	float m_f_focal_length;
	float m_f_skew;
	long  m_i_color_channels;
	long  m_i_color_mode;
public:
	CCam()
		: m_f_pixel_x_size(1.0f)					//default
		, m_f_pixel_y_size(1.0f)					//default
		, m_f_image_plane_width(0.0f)				//uninitialized
		, m_f_image_plane_height(0.0f)				//uninitialized
		, m_i_image_frame_lines(0)					//uninitialized
		, m_i_image_frame_columns(0)				//uninitialized
		, m_f_focal_length(1.0f)					//default
		, m_f_principal_point_x(0.0f)				//default
		, m_f_principal_point_y(0.0f)				//default
		, m_f_skew(0.0f)							//default
		, m_i_color_channels(1)						//default
		, m_i_color_mode(COLOR_MODE_INTERLEAVED)	//default
	{		
	}

	virtual ~CCam()
	{
	}

	
	inline void PixelFromImageFrame(CHomography *H)
	{
		H->SetHomography(1.0f, 0.0f, -m_i_image_frame_columns/2.0f,
						 0.0f, -1.0f, m_i_image_frame_lines/2.0f,
						 0.0f, 0.0f, 1.0f);
	}

	inline void ImageFrameFromPixel(CHomography *H)
	{
		H->SetHomography(1.0f, 0.0f, m_i_image_frame_columns/2.0f,
						 0.0f, -1.0f, m_i_image_frame_lines/2.0f,
						 0.0f, 0.0f, 1.0f);
	}

	inline void ImagePlaneFromPixel(CHomography *H)
	{
		H->SetHomography(m_f_pixel_x_size, 0.0f, 0.0f, 
						 0.0f, m_f_pixel_y_size, 0.0f,
						 0.0f, 0.0f, 1.0f);
	}

	inline void PixelFromImagePlane(CHomography *H)
	{
		H->SetHomography(1/m_f_pixel_x_size, 0.0f, 0.0f, 
						 0.0f, 1/m_f_pixel_y_size, 0.0f,
						 0.0f, 0.0f, 1.0f);
	}

	inline void ImagePlaneFromMetricProjection(CHomography *H)
	{
		H->SetHomography(1.0f, m_f_skew, m_f_principal_point_x,
										0.0f, 1.0f, m_f_principal_point_y,
										0.0f, 0.0f, 1.0f);
	}

	inline void MetricProjectionFromImagePlane(CHomography *H)
	{
		H->SetHomography(1.0f, -m_f_skew, m_f_skew*m_f_principal_point_y - m_f_principal_point_x,
										0.0f, 1.0f, -m_f_principal_point_y,
										0.0f, 0.0f, 1.0f);
	}

	inline void ImagePlaneFromWorld(CHomography *H)
	{
		H->SetHomography(m_f_focal_length, m_f_focal_length*m_f_skew, m_f_focal_length*m_f_principal_point_x,
										0.0f, m_f_focal_length, m_f_focal_length*m_f_principal_point_y,
										0.0f, 0.0f, 1.0f);
	}


	int get_image_frame_lines(long *pVal)
	{
		// TODO: Add your implementation code here
		*pVal = m_i_image_frame_lines;
		return 0;
	}

	int put_image_frame_lines(long newVal)
	{
		// TODO: Add your implementation code here
		if( (m_i_image_frame_lines == 0) && (m_f_image_plane_height == 0.0f)) //uninitialized
			m_f_image_plane_height = (float)newVal;

		m_i_image_frame_lines = newVal;

		//update pixel size dimension
		if(m_i_image_frame_lines != 0)
			m_f_pixel_y_size = m_f_image_plane_height/(float)m_i_image_frame_lines;
		return 0;
	}

	int get_image_frame_columns(long *pVal)
	{
		// TODO: Add your implementation code here
		*pVal = m_i_image_frame_columns;
		return 0;
	}

	int put_image_frame_columns(long newVal)
	{
		// TODO: Add your implementation code here

		if((m_i_image_frame_columns == 0) && (m_f_image_plane_width == 0.0f)) //uninitialized
			m_f_image_plane_width = (float)newVal;

		m_i_image_frame_columns = newVal;

		//update pixel size dimension
		if(m_i_image_frame_columns != 0)
			m_f_pixel_x_size = m_f_image_plane_width/(float)m_i_image_frame_columns;
		return 0;
	}

	int get_image_plane_width(float *pVal)
	{
		// TODO: Add your implementation code here
		*pVal = m_f_image_plane_width;
		return 0;
	}

	int put_image_plane_width(float newVal)  
	{
		// TODO: Add your implementation code here
		m_f_image_plane_width = newVal;

		//update pixel size dimension
		if(m_i_image_frame_columns != 0)
			m_f_pixel_x_size = m_f_image_plane_width/(float)m_i_image_frame_columns;
		return 0;
	}

	int get_image_plane_height(float *pVal)
	{
		// TODO: Add your implementation code here
		*pVal = m_f_image_plane_height;
		return 0;
	}

	int put_image_plane_height(float newVal)  
	{
		// TODO: Add your implementation code here
		m_f_image_plane_height = newVal;

		//update pixel size dimension
		if(m_i_image_frame_lines != 0)
			m_f_pixel_y_size = m_f_image_plane_height/(float)m_i_image_frame_lines;
		return 0;
	}


	////////////////////////////////////////////////
	////////////////////////////////////////////////
	// Pixel Size parameters cannot be user changed
	// They are determined by image plane and image array sizes

	int get_pixel_x_size(float *pVal)
	{
		// TODO: Add your implementation code here
		*pVal = m_f_pixel_x_size;
		return 0;
	}

	int put_pixel_x_size(float newVal) 
	//should not be called by the user
	//only for persistence
	{
		// TODO: Add your implementation code here
		m_f_pixel_x_size = newVal;		
		return 0;
	}

	int get_pixel_y_size(float *pVal)
	{
		// TODO: Add your implementation code here
		*pVal = m_f_pixel_y_size;
		return 0;
	}

	int put_pixel_y_size(float newVal) 
	//should not be used externally
	//only for persistence
	{
		// TODO: Add your implementation code here
		m_f_pixel_y_size = newVal;		
		return 0;
	}		

	int get_focal_length(float *pVal)
	{
		// TODO: Add your implementation code here
		*pVal = m_f_focal_length;
		return 0;
	}

	int put_focal_length(float newVal)
	{
		// TODO: Add your implementation code here
		m_f_focal_length = newVal;
		return 0;
	}

	
	int get_principal_point_x(float *pVal)
	{
		// TODO: Add your implementation code here
		*pVal = m_f_principal_point_x;
		return 0;
	}

	int put_principal_point_x(float newVal)
	{
		// TODO: Add your implementation code here
		m_f_principal_point_x = newVal;
		return 0;
	}

	int get_principal_point_y(float *pVal)
	{
		// TODO: Add your implementation code here
		*pVal = m_f_principal_point_y;
		return 0;
	}

	int put_principal_point_y(float newVal)
	{
		// TODO: Add your implementation code here
		m_f_principal_point_y = newVal;
		return 0;
	}

	int get_skew(float *pVal)
	{
		// TODO: Add your implementation code here
		*pVal = m_f_skew;
		return 0;
	}

	int put_skew(float newVal)
	{
		// TODO: Add your implementation code here
		m_f_skew = newVal;
		return 0;
	}


	///////////////////////////////////////////////////////////////
	/// Operations

	int image_frame_to_pixel(float col, float lin, float *px, float *py)
	{		
		// center and invert y axis
		
		*px = col - m_i_image_frame_columns/2.0f;
		*py = m_i_image_frame_lines/2.0f - lin;
		return 0;
	}

	int pixel_to_image_frame(float px, float py, float *col, float *lin)
	{		
		*col = px + m_i_image_frame_columns/2.0f;
		*lin = m_i_image_frame_lines/2.0f - py;
		return 0;
	}


	int image_plane_to_pixel(float xi, float yi, float *px, float *py)
	{	
		*px = xi/m_f_pixel_x_size;
		*py = yi/m_f_pixel_y_size;
		return 0;
	}

	int pixel_to_image_plane(float px, float py, float *xi, float *yi)
	{			
		*xi = m_f_pixel_x_size*px;
		*yi = m_f_pixel_y_size*py;		
		return 0;
	}

	
	int metric_projection_to_image_plane(float x, float y, float *xi, float *yi)
	{				
		*xi = x + m_f_principal_point_x + m_f_skew*y;
		*yi = y + m_f_principal_point_y;
		return 0;
	}

	int image_plane_to_metric_projection(float xi, float yi, float *x, float *y)
	{	
		*y = yi - m_f_principal_point_y;
		*x = xi + m_f_principal_point_x - m_f_skew*(*y);
		return 0;
	}

	int get_color_channels(long *pVal) 
	{
		*pVal = m_i_color_channels;
		return 0;
	}

	int put_color_channels(long newVal) 
	{
		m_i_color_channels = newVal;
		return 0;
	}

	int get_color_mode(long *pVal) 
	{
		*pVal = m_i_color_mode;
		return 0;
	}

	int put_color_mode(long newVal) 
	{
		m_i_color_mode = newVal;
		return 0;
	}
};

#endif //__CCam_H_
