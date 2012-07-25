// CQuadrangle.h: interface for the CQuadrangle class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(CQUADRANGLE_H)
#define CQUADRANGLE_H

class CQuadrangle  
{
public:
	virtual ~CQuadrangle()
	{
	};

	CQuadrangle()	: x1(data[0][0])
					, y1(data[0][1])
					, x2(data[1][0])
					, y2(data[1][1])
					, x3(data[2][0])
					, y3(data[2][1])
					, x4(data[3][0])
					, y4(data[3][1])
	{
	};

/*
	CQuadrangle(RECT &rect)
					: x1(data[0][0])
					, y1(data[0][1])
					, x2(data[1][0])
					, y2(data[1][1])
					, x3(data[2][0])
					, y3(data[2][1])
					, x4(data[3][0])
					, y4(data[3][1])
	{
		SetQuadrangle(rect);
	};
*/
	CQuadrangle(float X1, float Y1, float X2, float Y2, float X3, float Y3, float X4, float Y4)
					: x1(data[0][0])
					, y1(data[0][1])
					, x2(data[1][0])
					, y2(data[1][1])
					, x3(data[2][0])
					, y3(data[2][1])
					, x4(data[3][0])
					, y4(data[3][1])
	{
		SetQuadrangle( X1, Y1, X2, Y2, X3, Y3, X4, Y4);
	};

	bool IsConcave()
	{
		float x2_x1 = x2-x1;
		float x3_x2 = x3-x2;
		float x4_x3 = x4-x3;
		float x1_x4 = x1-x4;
		float y2_y1 = y2-y1;
		float y3_y2 = y3-y2;
		float y4_y3 = y4-y3;
		float y1_y4 = y1-y4;

		if((x2_x1*y3_y2 - x3_x2*y2_y1) <= 0)
			return true;

		if((x3_x2*y4_y3 - x4_x3*y3_y2) <= 0)
			return true;

		if((x4_x3*y1_y4 - x1_x4*y4_y3) <= 0)
			return true;

		if((x1_x4*y2_y1 - x2_x1*y1_y4) <= 0)
			return true;

		return false;
	};

	bool ConvertToCenteredCoordinates(const CQuadrangle &quad, int width, int height )
	{
		x1 = quad.x1 - width/2.0f; y1 = - quad.y1 + height/2.0f;
		x2 = quad.x2 - width/2.0f; y2 = - quad.y2 + height/2.0f;
		x3 = quad.x3 - width/2.0f; y3 = - quad.y3 + height/2.0f;
		x4 = quad.x4 - width/2.0f; y4 = - quad.y4 + height/2.0f;

		return true;
	};

	bool ConvertToImageCoordinates(const CQuadrangle &quad, int width, int height )
	{
		x1 = quad.x1 + width/2.0f; y1 = - quad.y1 + height/2.0f;
		x2 = quad.x2 + width/2.0f; y2 = - quad.y2 + height/2.0f;
		x3 = quad.x3 + width/2.0f; y3 = - quad.y3 + height/2.0f;
		x4 = quad.x4 + width/2.0f; y4 = - quad.y4 + height/2.0f;

		return true;
	};
	

	float MinHorzCoord()
	{
		float m = x1;
		if(x2 < m)
			m = x2;
		if(x3 < m)
			m = x3;
		if(x4 < m)
			m = x4;

		return m;
	}

	float MaxHorzCoord()
	{
		float m = x1;
		if(x2 > m)
			m = x2;
		if(x3 > m)
			m = x3;
		if(x4 > m)
			m = x4;

		return m;
	}

	float MinVertCoord()
	{
		float m = y1;
		if(y2 < m)
			m = y2;
		if(y3 < m)
			m = y3;
		if(y4 < m)
			m = y4;

		return m;
	}

	float MaxVertCoord()
	{
		float m = y1;
		if(y2 > m)
			m = y2;
		if(y3 > m)
			m = y3;
		if(y4 > m)
			m = y4;

		return m;
	}



	inline void SetQuadrangle(float X1, float Y1, float X2, float Y2, float X3, float Y3, float X4, float Y4)
	{
		x1 = X1;		y1 = Y1;
		x2 = X2;		y2 = Y2;
		x3 = X3;		y3 = Y3;
		x4 = X4;		y4 = Y4;
	}

	/*
	inline void SetQuadrangle(RECT &rect)
	{
		x1 = (float)rect.left;		y1 = (float)rect.top;
		x2 = (float)rect.right;		y2 = (float)rect.top;
		x3 = (float)rect.right;		y3 = (float)rect.bottom;
		x4 = (float)rect.left;		y4 = (float)rect.bottom;
	}
	*/

	inline float& operator[](int i)				{return data[i/2][i%2];};
	inline const float& operator[](int i)	const	{return data[i/2][i%2];};

	inline const CQuadrangle& operator= (const CQuadrangle& quad)	
	{
		if(&quad != this) //check for self assignment			
		{ 
			for(int i = 0; i < 8; i++) 
				(*this)[i] = quad[i];
		}
		return *this;  //enables x = y = z;
	};

	inline const CQuadrangle& operator+= (const CQuadrangle& quad)	
	{
		for(int i = 0; i < 8; i++) 
			(*this)[i] += quad[i];
		return *this; //enables cascaded calls
	};
	
	float &x1, &y1, &x2, &y2, &x3, &y3, &x4, &y4;
	float data[4][2];
};

#endif // !defined(CQUADRANGLE_H)
