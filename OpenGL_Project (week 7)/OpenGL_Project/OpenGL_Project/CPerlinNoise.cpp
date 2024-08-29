// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) Media Design School
//
// File Name : CPerlinNoise.cpp
// Description : Creates perlin noise. NPG blends four colours based on height. Outputs a raw file to make a heightmap. 
// Author : Daniel West
// Mail : daniel.west@mds.ac.nz

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
    uint8_t* pixelsRGBA = new uint8_t[_ScreenWidth * _ScreenHeight * 3];
    int index = 0;
    int indexRGBA = 0;

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
            // black white heightmap
            pixels[index++] = (uint8_t)(noise);

            // Apply smooth blending for colors based on the noise value
            float r, g, b;

            // blue
            if (noise < 32)
            {
                r = 252;
                g = 244;
                b = 52;
            }
            else if (noise < 64)
            {
                // mix the previous colour
                float t = (noise - 32) / 32.0f;
                r = glm::mix(252, 255, t);
                g = glm::mix(244, 255, t);
                b = glm::mix(52, 255, t);
            }
            // pink
            else if (noise < 128)
            {
                // mix the previous colour
                float t = (noise - 64) / 64.0f;
                r = glm::mix(255, 156, t);
                g = glm::mix(255, 89, t);
                b = glm::mix(255, 209, t);
            }
            // white
            else
            {
                float t = (noise - 128) / 68.0f;
                r = glm::mix(156, 44, t);
                g = glm::mix(89, 44, t);
                b = glm::mix(209, 4, t);
            }
            // dark blue
            //else
            //{
            //    float t = (noise - 128) / 68.0f;
            //    r = glm::mix(128, 69, t);
            //    g = glm::mix(128, 240, t);
            //    b = glm::mix(128, 20, t);
            //}

            pixelsRGBA[indexRGBA++] = (uint8_t)glm::clamp(r, 0.0f, 255.0f);
            pixelsRGBA[indexRGBA++] = (uint8_t)glm::clamp(g, 0.0f, 255.0f);
            pixelsRGBA[indexRGBA++] = (uint8_t)glm::clamp(b, 0.0f, 255.0f);
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

    // create coloured jpg
    stbi_write_jpg("Resources/Textures/Noise/COLOURED.jpg", _ScreenWidth, _ScreenHeight, 3, pixelsRGBA, 100);
    // create black white jpg
    stbi_write_jpg(("Resources/Textures/Noise/" + std::to_string(m_iSeed) + ".jpg").c_str(), _ScreenWidth, _ScreenHeight, 1, pixels, 100);

    delete[] pixelsRGBA;
    delete[] pixels;
}

CPerlinNoise::~CPerlinNoise()
{
}

//void CPerlinNoise::AnimateTransition(GLuint texture1ID, GLuint texture2ID, int frameCount, const std::string& outputDirectory)
//{
//    int channels = 4; // Assuming RGBA format
//
//    // Retrieve dimensions of the first texture
//    glBindTexture(GL_TEXTURE_2D, texture1ID);
//    GLint texture1Width, texture1Height;
//    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &texture1Width);
//    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &texture1Height);
//
//    // Retrieve dimensions of the second texture
//    glBindTexture(GL_TEXTURE_2D, texture2ID);
//    GLint texture2Width, texture2Height;
//    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &texture2Width);
//    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &texture2Height);
//
//    // Determine the minimum dimensions for blending
//    int blendWidth = std::min(texture1Width, texture2Width);
//    int blendHeight = std::min(texture1Height, texture2Height);
//
//    size_t textureSize = blendWidth * blendHeight * channels;
//
//    // Allocate space for the pixel data of both textures
//    uint8_t* texture1 = new uint8_t[textureSize];
//    uint8_t* texture2 = new uint8_t[textureSize];
//    uint8_t* blendedTexture = new uint8_t[textureSize];
//
//    // Bind and read the first texture from GPU to CPU
//    glBindTexture(GL_TEXTURE_2D, texture1ID);
//    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture1);
//
//    // Bind and read the second texture from GPU to CPU
//    glBindTexture(GL_TEXTURE_2D, texture2ID);
//    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture2);
//
//    // Generate Perlin noise map
//    float low = 0.0f;
//    float increment = 1.0f / frameCount;
//    float high = increment;
//
//    float** perlinNoise = GeneratePerlinNoise(blendWidth, blendHeight, 9);
//    AdjustLevels(perlinNoise, blendWidth, blendHeight, 0.2f, 0.8f);
//
//    // Create animation frames
//    for (int frame = 0; frame < frameCount; frame++)
//    {
//        // Adjust noise levels for the current frame
//        float** blendMask = AdjustLevels(perlinNoise, blendWidth, blendHeight, low, high);
//
//        int index = 0;
//        for (int Col = 0; Col < blendHeight; Col++)
//        {
//            for (int Row = 0; Row < blendWidth; Row++)
//            {
//                float blendValue = blendMask[Row][Col];
//
//                for (int c = 0; c < channels; c++)
//                {
//                    blendedTexture[index] = (uint8_t)glm::mix(texture1[index], texture2[index], blendValue);
//                    index++;
//                }
//            }
//        }
//
//        // Save the blended texture as a PNG
//        std::string outputPath = outputDirectory + "/blend_animation_" + std::to_string(frame) + ".png";
//        stbi_write_png(outputPath.c_str(), blendWidth, blendHeight, channels, blendedTexture, blendWidth * channels);
//
//        // Update levels for the next frame
//        low = high;
//        high += increment;
//    }
//
//    // Cleanup
//    delete[] texture1;
//    delete[] texture2;
//    delete[] blendedTexture;
//    FreeArray(perlinNoise, blendWidth);
//}

float** CPerlinNoise::AdjustLevels(float** image, int width, int height, float low, float high)
{
    float** newImage = GetEmptyArray(width, height);

    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            float col = image[i][j];

            if (col <= low)
            {
                newImage[i][j] = 0.0f;
            }
            else if (col >= high)
            {
                newImage[i][j] = 1.0f;
            }
            else
            {
                newImage[i][j] = (col - low) / (high - low);
            }
        }
    }

    return newImage;
}

float** CPerlinNoise::GetEmptyArray(int width, int height)
{
    float** array = new float* [width];
    for (int i = 0; i < width; i++)
    {
        array[i] = new float[height];
        std::fill(array[i], array[i] + height, 0.0f);
    }
    return array;
}

void CPerlinNoise::FreeArray(float** array, int width)
{
    for (int i = 0; i < width; i++)
    {
        delete[] array[i];
    }
    delete[] array;
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
}
