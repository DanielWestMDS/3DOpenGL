#pragma once

class CPerlinNoise
{

public:
	CPerlinNoise(int _ScreenWidth, int _ScreenHeight);
	~CPerlinNoise();

	int GetSeed() { return m_iSeed; };

private:

	double RandomValue(int _x, int _y);
	double Smooth(int _x, int _y);
	double LinearInterpolate(double _p1, double _p2, double _fract);
	double CosineInterpolate(double _p1, double _p2, double _fract);
	double SmoothInterpolate(double _x, double _y);
	double TotalNoisePerPoint(int _x, int _y);

	int m_iSeed;
};