#include "raylib.h"
#include "complex"
#include <bits/stdc++.h>
#include "ft_functions.h"

int main()
{
    const int screenWidth = 800;
    const int screenHeight = 450;
    int sample_size = 6;
    long long dft_times[sample_size];
    long long fft_times[sample_size];
    long long inputSize[sample_size];
    long long max_time = -1;
    for (int i = 0; i < sample_size; i++)
    {
        std::vector<std::complex<float>> data(10 * (pow(2, i)));
        for (long long j = 0; j < 10 * (pow(2, i)); j++)
        {
            data[j] = std::complex<float>(j, 0);
        }
        clock_t initial_time;
        clock_t final_time;
        initial_time = clock();
        std::vector<std::complex<float>> trans = dft(data);
        final_time = clock();
        long long interval_dft = final_time - initial_time;
        initial_time = clock();
        std::vector<std::complex<float>> trans_fast = fft(data);
        final_time = clock();
        long long interval_fft = final_time - initial_time;
        dft_times[i] = interval_dft;
        fft_times[i] = interval_fft;
        if (interval_dft > max_time)
        {
            max_time = interval_dft;
        }
        if (interval_fft > max_time)
        {
            max_time = interval_fft;
        }
        std::cout << interval_dft << " " << interval_fft << "\n";
        inputSize[i] = 10 * (pow(2, i));
    }
    InitWindow(screenWidth, screenHeight, "DFT vs FFT");
    SetTargetFPS(60);
    double scale = (double)screenHeight / (double)max_time;
    std::cout << scale << "\n";
    for (int i = 0; i < sample_size; i++)
    {
        dft_times[i] = (long long)(dft_times[i] * scale);
        std::cout << dft_times[i] << " ";
        fft_times[i] = (long long)(fft_times[i] * scale);
        std::cout << fft_times[i] << "\n";
    }
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        for (int i = 0; i < sample_size; i++)
        {
            DrawRectangle(2 * i * screenWidth / (2 * sample_size), screenHeight - dft_times[i], 0.8 * screenWidth / (2 * sample_size), dft_times[i], GREEN);
            DrawRectangle((2 * i + 1) * screenWidth / (2 * sample_size), screenHeight - fft_times[i], 0.8 * screenWidth / (2 * sample_size), fft_times[i], RED);
        }

        EndDrawing();
    }

    CloseWindow();
}