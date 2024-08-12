#define _USE_MATH_DEFINES
#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "CPerlinNoise.h"
#include <math.h>
#include <time.h>
#include <stdint.h>
#include <glm.hpp>
#include <iostream>
#include <string>
#include <fstream>

CPerlinNoise::CPerlinNoise(int _ScreenWidth, int _ScreenHeight)
{
    m_iSeed = (int)time(NULL);

    uint8_t* pixels = new uint8_t[_ScreenWidth * _ScreenHeight];
    int index = 0;

    // create the noise data
    for (unsigned int Col = 0; Col < _ScreenHeight; Col++)
    {
        for (unsigned int Row = 0; Row < _ScreenWidth; Row++)
        {
            // returns -1 to 1 value for each point
            float noise = (float)TotalNoisePerPoint(Row, Col);

            // shift bell curve so noise isn't gray
            noise *= 3.0f;

            // normalize noise from -1 -> 1 to 0 -> 1
            noise = (noise + 1) * 0.5f;
            // scale to 255
            noise = noise * 255.0f;

            noise = glm::clamp(noise, 0.0f, 255.0f);
            pixels[index++] = (uint8_t)(noise);
        }
    }

    // save data to a file
    std::string SaveFilePath = "Resources/Textures/Noise/";
    std::ofstream rawFile(SaveFilePath + ".raw", std::ios_base::binary);
    if (rawFile)
    {
        rawFile.write((char*)&pixels[0], (std::streamsize)((int)_ScreenWidth * (int)_ScreenHeight));
        rawFile.close();
    }

    // create jpg
    stbi_write_jpg(("Resources/Textures/Noise/" + std::to_string(m_iSeed) + ".jpg").c_str(), _ScreenWidth, _ScreenHeight, 1, pixels, 100);
}

CPerlinNoise::~CPerlinNoise()
{
}

double CPerlinNoise::RandomValue(int _x, int _y)
{
    double randomValue;
    int noise = _x + _y * m_iSeed;
    noise = (noise << 13) ^ noise;

    int T = (noise * (noise * noise * 15731 * 789221) + 1376312589) & 0x7fffffff;
    randomValue = 1.0f - (double)T * 0.93132257461548515625e-9;
    return randomValue;
}

double CPerlinNoise::Smooth(int _x, int _y)
{
    double Corners = (RandomValue(_x - 1, _y - 1) + RandomValue(_x + 1, _y - 1) + RandomValue(_x - 1, _y + 1) + RandomValue(_x + 1, _y + 1)) / 16.0f;
    double Sides = (RandomValue(_x - 1, _y) + RandomValue(_x + 1, _y) + RandomValue(_x, _y - 1) + RandomValue(_x, _y + 1)) / 16.0f;
    double Center = (RandomValue(_x, _y)) / 4.0f;
    return Corners + Sides + Center;
}

double CPerlinNoise::LinearInterpolate(double _p1, double _p2, double _fract)
{
    return (_p1 * (1 - _fract) + _p2 * _fract);
}

double CPerlinNoise::CosineInterpolate(double _p1, double _p2, double _fract)
{
    double fract2 = (1 - cos(_fract * M_PI)) / 2;
    return (_p1 * (1 - fract2) + _p2 * fract2);
}

double CPerlinNoise::SmoothInterpolate(double _x, double _y)
{
    int truncatedX = (int)_x;
    int truncatedY = (int)_y;

    // get the value after the decimal
    double fractX = _x - (double)truncatedX;
    double fractY = _y - (double)truncatedY;

    // smoothing
    double v1 = Smooth(truncatedX, truncatedY);
    double v2 = Smooth(truncatedX + 1, truncatedY);
    double v3 = Smooth(truncatedX, truncatedY + 1);
    double v4 = Smooth(truncatedX + 1, truncatedY + 1);

    // interpolates
    double interpolate1 = CosineInterpolate(v1, v2, (double)fractX);
    double interpolate2 = CosineInterpolate(v3, v4, (double)fractX);

    double bilinearInterpolation = CosineInterpolate(interpolate1, interpolate2, fractY);
    return bilinearInterpolation;
}

double CPerlinNoise::TotalNoisePerPoint(int _x, int _y)
{
    int octaves = 6;
    float wavelength = 255.0f;
    float gain = 0.5f;
    float lacunarity = 2.0f;

    float maxValue = 0.0f;
    double total = 0.0f;

    for (int i = 0; i < octaves; i++)
    {
        float frequency = (float)pow(lacunarity, i) / wavelength;
        float amplitude = (float)pow(gain, i);
        maxValue += amplitude;

        total += SmoothInterpolate(_x * frequency, _y * frequency) * amplitude;
    }

    return (total);

    //// circle
    //int octaves = 4;
    //float wavelength = 12.0f;
    //float gain = 0.5f;
    //float lacunarity = 2.0f;

    //int ScreenX = _x - 256;
    //int ScreenY = _y - 256;

    //double total = 0.0f;
    //for (int i = 0; i < octaves; i++)
    //{
    //    float frequency = (float)pow(lacunarity, i) / wavelength;
    //    float amplitude = (float)pow(gain, i);

    //    float valX = glm::pow(((float)ScreenX / wavelength) - 0.5f, 2.0f);
    //    float valY = glm::pow(((float)ScreenY / wavelength) - 0.5f, 2.0f);
    //    float Sqroot = std::sqrt(valX + valY);

    //    total += glm::sin(Sqroot) + 
    //        (SmoothInterpolate(_x * frequency, _y * frequency) * amplitude);
    //}

    //return total;
}
