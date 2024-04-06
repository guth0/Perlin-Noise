#pragma once
#include "PerlinNoise.hpp"
#include <iostream>

class PerlinField
{
public:
    // float *data;
    uint16_t width;
    uint16_t height;

    PerlinField(const uint_fast32_t &seed_)
    {
        perlin = siv::PerlinNoise{seed_};
    }

    // ~PerlinField() // destructor, don't explicitily call, it will delete itself
    // {
    //     delete[] data;
    // }

    void setIntensity(int i)
    {
        field_intensitiy = i;
    }

    void setOctaves(int o)
    {
        octaves = o;
    }

    void setSize(const uint16_t &w, const uint16_t &h)
    {
        width = w;
        height = h;

        // data = new float[w * h];
    }

    void generateField(float *data)
    {
        for (int x = 0; x < width; x++)
        {
            for (int y = 0; y < height; y++)
            {
                data[(x * height) + y] = coefficent * perlin.octave2D_01((x * field_intensitiy), (y * field_intensitiy), octaves);
            }
        }

        std::cout << data[0] << "" << std::endl;
    }

private:
    siv::PerlinNoise perlin;
    int field_intensitiy;
    int octaves;
};