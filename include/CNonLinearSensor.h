#ifndef __CNONLINEARSENSOR_H_
#define __CNONLINEARSENSOR_H_


class CNonLinearSensor 
{
protected:
	long m_sensor_lines;
	long m_sensor_columns;
public:
	CNonLinearSensor()
		: m_sensor_lines(0)
		, m_sensor_columns(0)
	{
	}

	virtual ~CNonLinearSensor()
	{
	}

public:
	int get_sensor_lines(long *pVal)
	{
		// TODO: Add your implementation code here
		*pVal = m_sensor_lines;
		return 0;
	}

	int put_sensor_lines(long newVal)
	{
		// TODO: Add your implementation code here
		m_sensor_lines = newVal;
		return 0;
	}

	int get_sensor_columns(long *pVal)
	{
		// TODO: Add your implementation code here
		*pVal = m_sensor_columns;
		return 0;
	}

	int put_sensor_columns(long newVal)
	{
		// TODO: Add your implementation code here
		m_sensor_columns = newVal;
		return 0;
	}

	virtual int map_coordinates(float x, float y, float* u, float* v ) = 0;
    virtual int invmap_coordinates(float u, float v, float* x, float* y) = 0;				
};

#endif //__CNonLinearSensor_H_
