#pragma once

#include <cmath>
#include <vector>
#include <random>

class PerlinNoise {
public:
    explicit PerlinNoise(unsigned int seed) {
        permutation.resize(512);
        std::iota(permutation.begin(), permutation.begin() + 256, 0);

        std::mt19937 rng(seed);
        std::shuffle(permutation.begin(), permutation.begin() + 256, rng);

        // Duplicate the permutation to avoid overflow
        for (int i = 0; i < 256; ++i)
            permutation[256 + i] = permutation[i];
    }

    // Fractal noise (Perlin-style) between -1 and 1
    float noise(float x, int octaves = 4, float persistence = 0.5f) const {
        float total = 0;
        float frequency = 1.0f;
        float amplitude = 1.0f;
        float maxValue = 0;

        for (int i = 0; i < octaves; ++i) {
            total += singleNoise(x * frequency) * amplitude;
            maxValue += amplitude;

            amplitude *= persistence;
            frequency *= 2;
        }

        float result = total / maxValue;
        return std::clamp(result, -1.0f, 1.0f);
    }

private:
    std::vector<int> permutation;

    static float fade(float t) {
        // 6t^5 - 15t^4 + 10t^3
        return t * t * t * (t * (t * 6 - 15) + 10);
    }

    static float lerp(float a, float b, float t) {
        return a + t * (b - a);
    }

    static float grad(int hash, float x) {
        int h = hash & 15;
        float grad = 1.0f + (h & 7); // Gradient between 1 and 8
        if (h & 8) grad = -grad;
        return grad * x;
    }

    float singleNoise(float x) const {
        int xi = static_cast<int>(std::floor(x)) & 255;
        float xf = x - std::floor(x);

        float u = fade(xf);

        int a = permutation[xi];
        int b = permutation[xi + 1];

        float left = grad(a, xf);
        float right = grad(b, xf - 1);

        return lerp(left, right, u);
    }
};