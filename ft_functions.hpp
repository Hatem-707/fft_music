#pragma once
#include <complex>
#include <vector>

std::complex<float> euler(float angle);
std::vector<std::complex<float>> dft(std::vector<std::complex<float>> polynomial);
std::vector<std::complex<float>> inv_dft(std::vector<std::complex<float>> polynomial);
std::vector<std::complex<float>> fft(std::vector<std::complex<float>> polynomial);
std::vector<std::complex<float>> inv_fft(std::vector<std::complex<float>> polynomial);