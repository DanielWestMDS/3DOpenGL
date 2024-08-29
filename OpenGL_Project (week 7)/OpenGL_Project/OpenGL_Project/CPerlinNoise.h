// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) Media Design School
//
// File Name : CPerlinNoise.h
// Description : creates random noise based on a seed made from the time. Noise is stored as a png and a raw file to be loaded to a heightmap 
// Author : Daniel West
// Mail : daniel.west@mds.ac.nz

#pragma once

#include "CCamera.h"

class CPerlinNoise
{

public:
	CPerlinNoise(int _ScreenWidth, int _ScreenHeight);
	~CPerlinNoise();

	//void AnimateTransition(GLuint texture1ID, GLuint texture2ID, int frameCount, const std::string& outputDirectory);

	int GetSeed() { return m_iSeed; };

private:

	double RandomValue(int _x, int _y);
	double Smooth(int _x, int _y);
	double LinearInterpolate(double _p1, double _p2, double _fract);
	double CosineInterpolate(double _p1, double _p2, double _fract);
	double SmoothInterpolate(double _x, double _y);
	double TotalNoisePerPoint(int _x, int _y);

	// for animation
	float** AdjustLevels(float** image, int width, int height, float low, float high);
	float** GetEmptyArray(int width, int height);
	void FreeArray(float** array, int width);

	int m_iSeed;
};