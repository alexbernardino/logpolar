#ifndef CLOGPOLARMAPPER_H
#define CLOGPOLARMAPPER_H

class CLogPolarMapper : public CLogPolarCam
{
private:
	VARTYPE m_cur_type;
	byte *m_log_byte;
	byte *m_fov_byte;
	float *m_log_float;
	float *m_fov_float;
public:
	CLogPolarMapper() 
	{
		m_bRequiresSave = TRUE;
		m_cur_type = VT_UI1;
		m_log_byte = 0;
		m_fov_byte = 0;
		m_log_float = 0;
		m_fov_float = 0;
	};
	virtual ~CLogPolarMapper()
	{
		destroy_logpolar_mapper();
	};
	int create_logpolar_mapper();
	int destroy_logpolar_mapper();

	int compute_warp_field_2( CHomography * h, CImg * wf2);
	int compute_warp_field_1(CHomography * h, CImg * wf1);
	int get_fov(CImg * fov);
	int get_log(CImg * log);
	int invmap(CImg * log, CImg * fov, CImg *cart);
	int logmap(CImg * cart);

	int compute_warp_fields(/*[in]*/ float *h, /*[out]*/ float *wf1, /*[out]*/ float *wf2);
	int logmap8U(/*[in]*/ byte * cart, /*[in, out]*/ byte * log, /*[in, out]*/ byte * fov);
	int invmap8U(/*[in]*/ byte * log, /*[in]*/ byte * fov, /*[in, out]*/ byte * cart);
	int logmap8U_C3(/*[in]*/ byte * cart, /*[in, out]*/ byte * log, /*[in, out]*/ byte * fov);
	int invmap8U_C3(/*[in]*/ byte * log, /*[in]*/ byte * fov, /*[in,out]*/ byte * cart);
	int logmap8U_AC4(/*[in]*/ byte * cart, /*[in, out]*/ byte * log, /*[in, out]*/ byte * fov);
	int invmap8U_AC4(/*[in]*/ byte * log, /*[in]*/ byte * fov, /*[in,out]*/ byte * cart);
	int logmap32F(/*[in]*/ float * cart, /*[in, out]*/ float * log, /*[in, out]*/ float * fov);
	int invmap32F(/*[in]*/ float * log, /*[in]*/ float * fov, /*[in, out]*/ float * cart);
	int logmap32F_C3(/*[in]*/ float * cart, /*[in, out]*/ float * log, /*[in, out]*/ float * fov);
	int invmap32F_C3(/*[in]*/ float * log, /*[in]*/ float * fov, /*[in,out]*/ float * cart);
	int logmap32F_AC4(/*[in]*/ float * cart, /*[in, out]*/ float * log, /*[in, out]*/ float * fov);
	int invmap32F_AC4(/*[in]*/ float * log, /*[in]*/ float * fov, /*[in,out]*/ float * cart);
}

#endif //CLOGPOLARMAPPER_H