#include <complex>
#include <bits/stdc++.h>
#include "ft_functions.hpp"

float pi = acos(-1);

std::complex<float> euler(float angle)
{
    float real = cos(angle);
    float img = sin(angle);
    std::complex<float> out(real, img);
    return out;
}

std::vector<std::complex<float>> dft(std::vector<std::complex<float>> polynomial)
{
    long long degree = polynomial.size();
    std::vector<std::complex<float>> points(degree);
    for (int i = 0; i < degree; i++)
    {
        std::complex<float> temp = euler(-2 * pi * i / degree);
        points[i] = temp;
    }
    std::vector<std::complex<float>> result(degree);
    int j = 0;
    for (std::complex<float> point : points)
    {
        std::complex<float> temp(0, 0);
        for (int i = 0; i < degree; i++)
        {
            temp += polynomial[i] * pow(point, i);
        }
        result[j] = temp;
        j++;
    }
    return result;
}

std::vector<std::complex<float>> inv_dft(std::vector<std::complex<float>> polynomial)
{
    long long degree = polynomial.size();
    std::vector<std::complex<float>> points(degree);
    for (int i = 0; i < degree; i++)
    {
        std::complex<float> temp = euler(2 * pi * i / degree);
        points[i] = temp;
    }
    std::vector<std::complex<float>> result(degree);
    int j = 0;
    for (std::complex<float> point : points)
    {
        std::complex<float> temp(0, 0);
        for (int i = 0; i < degree; i++)
        {
            temp += polynomial[i] * pow(point, i);
        }

        result[j] = temp / (std::complex<float>)degree;
        j++;
    }
    return result;
}

std::vector<std::complex<float>> fft(std::vector<std::complex<float>> polynomial)
{
    long long degree = polynomial.size();
    float base_2 = log2(degree);
    if (base_2 != ceil(base_2))
    {
        std::vector<std::complex<float>> temp(pow(2, ceil(base_2)), std::complex<float>(0, 0));
        for (long long i = 0; i < degree; i++)
        {
            temp[i] = polynomial[i];
        }
        degree = pow(2, ceil(base_2));
        polynomial = temp;
    }
    if (degree == 1)
    {
        return polynomial;
    }
    std::vector<std::complex<float>> output(degree, std::complex<float>(0, 0));
    std::vector<std::complex<float>> even(degree / 2);
    std::vector<std::complex<float>> odd(degree / 2);
    for (long long i = 0; i < degree; i++)
    {
        if (i & 1)
        {
            odd[i / 2] = polynomial[i];
        }
        else
        {
            even[i / 2] = polynomial[i];
        }
    }
    std::vector<std::complex<float>> ret_even = fft(even);
    std::vector<std::complex<float>> ret_odd = fft(odd);
    std::vector<std::complex<float>> points(degree / 2);
    for (int i = 0; i < degree / 2; i++)
    {
        std::complex<float> temp = euler(2 * pi * i / degree);
        points[i] = temp;
    }
    for (long long i = 0; i < degree / 2; i++)
    {
        output[i] = ret_even[i] + points[i] * ret_odd[i];
        output[i + degree / 2] = ret_even[i] - points[i] * ret_odd[i];
    }
    return output;
}

std::vector<std::complex<float>> inv_fft(std::vector<std::complex<float>> polynomial)
{
    long long degree = polynomial.size();
    float base_2 = log2(degree);
    if (base_2 != ceil(base_2))
    {
        std::vector<std::complex<float>> temp(pow(2, ceil(base_2)), std::complex<float>(0, 0));
        for (long long i = 0; i < degree; i++)
        {
            temp[i] = polynomial[i];
        }
        degree = pow(2, ceil(base_2));
        polynomial = temp;
    }
    if (degree == 1)
    {
        return polynomial;
    }
    std::vector<std::complex<float>> output(degree, std::complex<float>(0, 0));
    std::vector<std::complex<float>> even(degree / 2);
    std::vector<std::complex<float>> odd(degree / 2);
    for (long long i = 0; i < degree; i++)
    {
        if (i & 1)
        {
            odd[i / 2] = polynomial[i];
        }
        else
        {
            even[i / 2] = polynomial[i];
        }
    }
    std::vector<std::complex<float>> ret_even = inv_fft(even);
    std::vector<std::complex<float>> ret_odd = inv_fft(odd);
    std::vector<std::complex<float>> points(degree / 2);
    for (int i = 0; i < degree / 2; i++)
    {
        std::complex<float> temp = euler(-2 * pi * i / degree);
        points[i] = temp;
    }
    for (long long i = 0; i < degree / 2; i++)
    {
        output[i] = (ret_even[i] + points[i] * ret_odd[i]) / (std::complex<float>)2;
        output[i + degree / 2] = (ret_even[i] - points[i] * ret_odd[i]) / (std::complex<float>)2;
    }
    return output;
}
