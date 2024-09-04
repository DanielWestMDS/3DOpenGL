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
	/// <summary>
	/// Constructor for perlin noise. Creates a raw file for the height map and png that blends 4 colours based on height
	/// </summary>
	/// <param name="_ScreenWidth"></param>
	/// <param name="_ScreenHeight"></param>
	CPerlinNoise(int _ScreenWidth, int _ScreenHeight);

	/// <summary>
	/// destructor
	/// </summary>
	~CPerlinNoise();

	/// <summary>
	/// getter for the seed
	/// </summary>
	/// <returns></returns>
	int GetSeed() { return m_iSeed; };

private:

	/// <summary>
	/// uses the seed and coordinates and maths to produce a random seeming value
	/// </summary>
	/// <param name="_x"></param>
	/// <param name="_y"></param>
	/// <returns></returns>
	double RandomValue(int _x, int _y);

	/// <summary>
	/// Takes 8 surrounding neighbours and weighs them to make the centre value closer
	/// </summary>
	/// <param name="_x"></param>
	/// <param name="_y"></param>
	/// <returns></returns>
	double Smooth(int _x, int _y);

	/// <summary>
	/// direct interpolation between points
	/// </summary>
	/// <param name="_p1"></param>
	/// <param name="_p2"></param>
	/// <param name="_fract"></param>
	/// <returns></returns>
	double LinearInterpolate(double _p1, double _p2, double _fract);

	/// <summary>
	/// calculates value based on the value of other data points. Uses cosine for a smoother interpolation
	/// </summary>
	/// <param name="_p1"></param>
	/// <param name="_p2"></param>
	/// <param name="_fract"></param>
	/// <returns></returns>
	double CosineInterpolate(double _p1, double _p2, double _fract);

	/// <summary>
	/// bilinear interpolation
	/// </summary>
	/// <param name="_x"></param>
	/// <param name="_y"></param>
	/// <returns></returns>
	double SmoothInterpolate(double _x, double _y);

	/// <summary>
	/// returns a value ranging from -1 to 1
	/// </summary>
	/// <param name="_x"></param>
	/// <param name="_y"></param>
	/// <returns></returns>
	double TotalNoisePerPoint(int _x, int _y);

	int m_iSeed;
};