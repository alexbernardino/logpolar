// CHomography.h: interface for the CHomography class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(CHOMOGRAPHY_H)
#define CHOMOGRAPHY_H


#include <math.h>

#include "CQuadrangle.h"

// must link with MKL
extern "C" void	SGEBRD(int*, int*, float*, int*, float*, float*, float*, float*, float*, int*, int*);
extern "C" void SORGBR(char*, int*, int*, int*, float*, int*, float*, float*, int*, int* );
extern "C" void SBDSQR(char*, int*, int*, int*, int*, float*, float*, float*, int*, float*, int*, float*, int*, float*, int* );


class CHomography  
{
public:	

	bool Compute(CQuadrangle &Pin, CQuadrangle &Pout)
	{		
		A[0] = -Pin.x1;		A[9] = -Pin.y1;		A[18] = -1;		A[27] = 0;			A[36] = 0;			A[45] = 0;		A[54] = Pout.x1*Pin.x1;	A[63] = Pout.x1*Pin.y1;	A[72] = Pout.x1;
		A[1] = 0;			A[10] = 0;			A[19] = 0;		A[28] = -Pin.x1;	A[37] = -Pin.y1;	A[46] = -1;		A[55] = Pout.y1*Pin.x1;	A[64] = Pout.y1*Pin.y1;	A[73] = Pout.y1;
		A[2] = -Pin.x2;		A[11] = -Pin.y2;	A[20] = -1;		A[29] = 0;			A[38] = 0;			A[47] = 0;		A[56] = Pout.x2*Pin.x2;	A[65] = Pout.x2*Pin.y2;	A[74] = Pout.x2;
		A[3] = 0;			A[12] = 0;			A[21] = 0;		A[30] = -Pin.x2;	A[39] = -Pin.y2;	A[48] = -1;		A[57] = Pout.y2*Pin.x2;	A[66] = Pout.y2*Pin.y2;	A[75] = Pout.y2;
		A[4] = -Pin.x3;		A[13] = -Pin.y3;	A[22] = -1;		A[31] = 0;			A[40] = 0;			A[49] = 0;		A[58] = Pout.x3*Pin.x3;	A[67] = Pout.x3*Pin.y3;	A[76] = Pout.x3;
		A[5] = 0;			A[14] = 0;			A[23] = 0;		A[32] = -Pin.x3;	A[41] = -Pin.y3;	A[50] = -1;		A[59] = Pout.y3*Pin.x3;	A[68] = Pout.y3*Pin.y3;	A[77] = Pout.y3;
		A[6] = -Pin.x4;		A[15] = -Pin.y4;	A[24] = -1;		A[33] = 0;			A[42] = 0;			A[51] = 0;		A[60] = Pout.x4*Pin.x4;	A[69] = Pout.x4*Pin.y4;	A[78] = Pout.x4;
		A[7] = 0;			A[16] = 0;			A[25] = 0;		A[34] = -Pin.x4;	A[43] = -Pin.y4;	A[52] = -1;		A[61] = Pout.y4*Pin.x4;	A[70] = Pout.y4*Pin.y4;	A[79] = Pout.y4;
		A[8] = 0;			A[17] = 0;			A[26] = 0;		A[35] = 0;			A[44] = 0;			A[53] = 0;		A[62] = 0;				A[71] = 0;				A[80] = 0;	

		int m = 8, n = 9, lda = 9, lwork = (9+9)*64, info, nru = 0, ncc = 0;
		SGEBRD(&m, &n, A, &lda, D, E, TAUQ, TAUP, WORK, &lwork, &info);
		SORGBR("P", &n, &n, &m, A, &lda, TAUP, WORK, &lwork, &info );
		SBDSQR("L", &m, &n, &nru, &ncc, D, E, A, &lda, U, &lda, C, &lda, WORK, &info );
		
		float v99 = A[8*9+8];

		if(v99==0) 
			return false;

		q11 = A[0*9+8]/v99;q12 = A[1*9+8]/v99;q13 = A[2*9+8]/v99;q21 = A[3*9+8]/v99;
		q22 = A[4*9+8]/v99;q23 = A[5*9+8]/v99;q31 = A[6*9+8]/v99;q32 = A[7*9+8]/v99;
		q33 = A[8*9+8]/v99;


		return true;
	};

	bool Apply(float xin, float yin, float &xout, float &yout)
	{
		float denominator;

		denominator = q31*xin + q32*yin + q33;
		if(denominator == 0.0)
			return false;
		xout = (q11*xin+q12*yin+q13)/denominator;
		yout = (q21*xin+q22*yin+q23)/denominator;
		return true;
	};

	bool Apply(CQuadrangle &Pin, CQuadrangle& Pout)
	{
		Apply(Pin.x1, Pin.y1, Pout.x1, Pout.y1);
		Apply(Pin.x2, Pin.y2, Pout.x2, Pout.y2);
		Apply(Pin.x3, Pin.y3, Pout.x3, Pout.y3);
		Apply(Pin.x4, Pin.y4, Pout.x4, Pout.y4);
		return true;
	};

	virtual ~CHomography()
	{
	};


	CHomography()	: q11(data[0][0])
					, q12(data[0][1])
					, q13(data[0][2])
					, q21(data[1][0])
					, q22(data[1][1])
					, q23(data[1][2])
					, q31(data[2][0])
					, q32(data[2][1])
					, q33(data[2][2])
	{
	};
	

	CHomography(float Q11, float Q12, float Q13, float Q21, float Q22, float Q23, float Q31, float Q32, float Q33)
					: q11(data[0][0])
					, q12(data[0][1])
					, q13(data[0][2])
					, q21(data[1][0])
					, q22(data[1][1])
					, q23(data[1][2])
					, q31(data[2][0])
					, q32(data[2][1])
					, q33(data[2][2])
	{
		SetHomography( Q11, Q12, Q13, Q21, Q22, Q23, Q31, Q32, Q33);
	}

	inline void SetHomography(float Q11, float Q12, float Q13, float Q21, float Q22, float Q23, float Q31, float Q32, float Q33)
	{
		q11 = Q11;	
		q12 = Q12;	
		q13 = Q13;	
		q21 = Q21;	
		q22 = Q22;
		q23 = Q23;
		q31 = Q31;
		q32 = Q32;
		q33 = Q33;
	}

	inline void SetIdentity()
	{
		q11 = 1;	
		q12 = 0;	
		q13 = 0;	
		q21 = 0;	
		q22 = 1;
		q23 = 0;
		q31 = 0;
		q32 = 0;
		q33 = 1;
	}


	inline float& operator[](int i)					{return data[i/3][i%3];};
	inline const float& operator[](int i)	const	{return data[i/3][i%3];};

	inline const CHomography& operator= (const CHomography& h)	
	{
		if(&h != this) //check for self assignment			
		{ 
			for(int i = 0; i < 9; i++) 
				(*this)[i] = h[i];
		}
		return *this;  //enables x = y = z;
	};

	inline void mult(const CHomography& h1, const CHomography& h2)	
	{
		q11 = h1.q11*h2.q11+h1.q12*h2.q21+h1.q13*h2.q31;
		q12 = h1.q11*h2.q12+h1.q12*h2.q22+h1.q13*h2.q32;
		q13 = h1.q11*h2.q13+h1.q12*h2.q23+h1.q13*h2.q33;

		q21 = h1.q21*h2.q11+h1.q22*h2.q21+h1.q23*h2.q31;
		q22 = h1.q21*h2.q12+h1.q22*h2.q22+h1.q23*h2.q32;
		q23 = h1.q21*h2.q13+h1.q22*h2.q23+h1.q23*h2.q33;

		q31 = h1.q31*h2.q11+h1.q32*h2.q21+h1.q33*h2.q31;
		q32 = h1.q31*h2.q12+h1.q32*h2.q22+h1.q33*h2.q32;
		q33 = h1.q31*h2.q13+h1.q32*h2.q23+h1.q33*h2.q33;
	};

	bool Inverse(const CHomography& h)
	{
		float a = h.q22*h.q33;
		float b = h.q23*h.q32;
		float c = h.q12*h.q33;
		float d = h.q13*h.q32;
		float e = h.q12*h.q23;
		float f = h.q13*h.q22;
		float num11 = a - b;
		float num12 = d - c;
		float num13 = e - f;
		float num21 = h.q23*h.q31 - h.q21*h.q33;
		float num22 = h.q11*h.q33 - h.q13*h.q31;
		float num23 = h.q13*h.q21 - h.q11*h.q23;
		float num31 = h.q21*h.q32 - h.q22*h.q31;
		float num32 = h.q12*h.q31 - h.q11*h.q32;
		float num33 = h.q11*h.q22 - h.q12*h.q21;
		float den = h.q11*num11 + h.q21*num12 + h.q31*num13;

		if( den != 0)
		{
			q11 = num11/den;
			q12 = num12/den;
			q13 = num13/den;
			q21 = num21/den;
			q22 = num22/den;
			q23 = num23/den;
			q31 = num31/den;
			q32 = num32/den;
			q33 = num33/den;
			return true;
		}
		else
		{
			return false;
		}
	};

	//
	// SetProjectiveParams
	//
	// Computes an homography from a special parameterization:
	//
	// tx_percent - horizontal translation as percentage of image width
	// ty_percent - vertical translation as percentage of image height
	// theta_degree - rotation in degrees around the center of the coordinate system
	// log2_scale_percent - scale change in percentage (100% = double size; 50% = half size)
	// log2_compr_percent - aspect ratio change in percentage (100% = doubles horiz. dim. and halves vert. dim.)
	// shear_percent - shear in percentage of image size (100% = shears equivalent to image size)
	// inv_vanish_x_percent - horz. curl. in percentage of the inverse of the vanish. point (100% = vanish. point touches image border)
	// inv_vanish_y_percent - vert. curl. in percentage of the inverse of the vanish. point (100% = vanish. point touches image border)

	bool SetProjectiveParams(
		float tx_percent, 
		float ty_percent, 
		float theta_degree, 
		float log2_scale_percent, 
		float log2_compr_percent, 
		float shear_percent, 
		float inv_vanish_x_percent, 
		float inv_vanish_y_percent,
		float image_width,
		float image_height )
	{
		const float local_pi = 3.14159265358979323846f;
		float sh, cp, mg, tx, ty, cx, cy, theta_rad, sth, cth;

		theta_rad = theta_degree*local_pi/180.0f;
		sth = (float)sin(theta_rad); 
		cth = (float)cos(theta_rad);
 		cp = (float)pow(2.0f, log2_compr_percent/100.0f);
		mg = (float)pow(2.0f, log2_scale_percent/100.0f);
		sh = shear_percent/100.0f;		
		tx = tx_percent/100.0f*image_width;
		ty = ty_percent/100.0f*image_height;		
		cx = inv_vanish_x_percent/100.0f/image_width; 
		cy = inv_vanish_y_percent/100.0f/image_height;

		q11 = mg*cth*cp + tx*cx;
		q12 = mg*cth*sh - mg*sth/cp+tx*cy;
		q13 = tx;
		q21 = mg*sth*cp + ty*cx;
		q22 = mg*sth*sh + mg*cth/cp+ty*cy;
		q23 = ty;
		q31 = cx;
		q32 = cy;
		q33 = 1.0f;

		return true;
	};


	//
	// GetProjectiveParams
	//
	// Decomposed the homography into a special parameterization:
	//
	// tx_percent - horizontal translation as percentage of image width
	// ty_percent - vertical translation as percentage of image height
	// theta_degree - rotation in degrees around the center of the coordinate system
	// log2_scale_percent - scale change in percentage (100% = double size; 50% = half size)
	// log2_compr_percent - aspect ratio change in percentage (100% = doubles horiz. dim. and halves vert. dim.)
	// shear_percent - shear in percentage of image size (100% = shears equivalent to image size)
	// inv_vanish_x_percent - horz. curl. in percentage of the inverse of the vanish. point (100% = vanish. point touches image border)
	// inv_vanish_y_percent - vert. curl. in percentage of the inverse of the vanish. point (100% = vanish. point touches image border)

	bool GetProjectiveParams(
		float* tx_percent, 
		float* ty_percent, 
		float* theta_degree, 
		float* log2_scale_percent, 
		float* log2_compr_percent, 
		float* shear_percent, 
		float* inv_vanish_x_percent, 
		float* inv_vanish_y_percent,
		float image_width,
		float image_height )
	{

		const float local_pi = 3.14159265358979323846f;
		
		float srk11 = q11 - q13*q31;
		float srk12 = q12 - q13*q32;
		float srk21 = q21 - q23*q31;
		float srk22 = q22 - q23*q32;

		float mg = (float)sqrt(srk11*srk22 - srk21*srk12);
		if( mg == 0 )
			return false;

		float den = (float)sqrt(srk11*srk11 + srk21*srk21);
		float sth = srk21/den;
		float cth = srk11/den;

		float cp = srk11/mg/cth;
		float sh = (srk12*cp + mg*sth)/srk11;

		float theta_rad = (float)atan2(sth, cth);
		
		*theta_degree = theta_rad*180.0f/local_pi;
		*log2_compr_percent = 100.0f*(float)log(cp)/(float)log(2.0);	
		*log2_scale_percent = 100.0f*(float)log(mg)/(float)log(2.0);
		*shear_percent = sh*100.0f;
 		*tx_percent = 100.0f*q13/image_width;
		*ty_percent = 100.0f*q23/image_height;		
		*inv_vanish_x_percent = q31*100.0f*image_width; 
		*inv_vanish_y_percent = q32*100.0f*image_height;

		return true;
	};

	
	float &q11, &q12, &q13, &q21, &q22, &q23, &q31, &q32, &q33;
	float data[3][3];

private:
	float A[9*9], D[9], E[9], TAUQ[9], TAUP[9], WORK[(9+9)*64], VT[9*9], U[9*9], C[9*9];
};

#endif // !defined(CHOMOGRAPHY_H)
