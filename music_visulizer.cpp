#include "complex"
#include "ft_functions.h"
#include "raylib.h"
#include <bits/stdc++.h>
#include <vector>

#define ARR_LEN(xs) sizeof(xs) / sizeof(xs[0])

uint64_t global_frames[512] = {0};
int global_frame_count = 512;

void Callback(void *bufferData, unsigned int frames) {
  if (frames > ARR_LEN(global_frames))
    frames = ARR_LEN(global_frames);
  memcpy(global_frames, bufferData, frames * sizeof(uint64_t));
};

CLITERAL(Color)
InBetweenColor(CLITERAL(Color) colorA, CLITERAL(Color) colorB, float distance) {
  CLITERAL(Color) res;
  res.a = (colorA.a * distance + colorB.a * (1 - distance));
  res.b = (colorA.b * distance + colorB.b * (1 - distance));
  res.g = (colorA.g * distance + colorB.g * (1 - distance));
  res.r = (colorA.r * distance + colorB.r * (1 - distance));
  return res;
}

int main(int argc, char *argv[]) {
  const int screenHeight = 1000;
  const int screenWidth = 1600;
  const int rectanglesNum = 132;
  const int playBarHeight = 30;
  const int maxRecHeight = screenHeight - playBarHeight;
  const double gravity = 0.25;
  float volume = 0.5;

  InitWindow(screenWidth, screenHeight, "music frequency visualizer");
  InitAudioDevice();
  Music music;
  if (argc > 1) {
    music = LoadMusicStream(argv[1]);
  } else {
    music = LoadMusicStream("NEFFEX-Cold-❄️_Copyright-Free.mp3");
  }
  PlayMusicStream(music);
  double timePlayed;
  CLITERAL(Color) backgroundColor = CLITERAL(Color){41, 115, 178, 255};
  CLITERAL(Color) highColor = CLITERAL(Color){242, 239, 231, 255};
  CLITERAL(Color) lowColor = CLITERAL(Color){72, 172, 140, 255};
  SetTargetFPS(60);
  AttachAudioStreamProcessor(music.stream, Callback);
  SetAudioStreamVolume(music.stream, volume);

  std::vector<std::pair<double, double>> heights((rectanglesNum / 2) - 2,
                                                 {0, 0});

  while (!WindowShouldClose()) {

    UpdateMusicStream(music);
    std::vector<double> rectangles(rectanglesNum);
    std::vector<std::complex<float>> wav_data(512);
    for (int i = 0; i < 512; i++) {
      float *samples = (float *)&global_frames[i];
      wav_data[i] = std::complex<float>(samples[0], 0);
    }
    double rms = 0;
    for (int i = 0; i < wav_data.size(); i++) {
      float window = 0.5f * (1 - cos(2 * M_PI * i / (wav_data.size() - 1)));
      wav_data[i] *= window;
      rms += sqrt(wav_data[i].real() * wav_data[i].real());
    }
    rms /= wav_data.size();

    std::vector<std::complex<float>> frequency = fft(wav_data);

    int pointsPerRectangle =
        ceil((float)global_frame_count / (rectanglesNum - 4));

    double maxRec = 0;

    for (int i = 0; i < 512; i++) {
      rectangles[i / pointsPerRectangle] +=
          pow(pow(frequency[i].real(), 2) + pow(frequency[i].imag(), 2), 0.5);
    }

    // printf("scale: %lf \n", scale);-
    for (int i = 0; i < (rectanglesNum / 2) - 2; i++) {
      rectangles[i] = log2(rectangles[i] * 20);
      if (rectangles[i] > maxRec) {
        maxRec = (rectangles[i] < 7) ? rectangles[i] : 7;
      }
      // if (rectangles[i] < 0) {
      //   std::cout << "it happens" << std::endl;
      // }
    }
    if (IsKeyPressed(KEY_SPACE)) {
      if (IsMusicStreamPlaying(music))
        PauseMusicStream(music);
      else
        ResumeMusicStream(music);
    }
    if (IsKeyPressed(KEY_UP) && volume < 1) {
      volume += 0.1;
    } else if (IsKeyPressed(KEY_DOWN) && volume > 0) {
      volume -= 0.1;
    }
    SetAudioStreamVolume(music.stream, volume);

    timePlayed = GetMusicTimePlayed(music) / GetMusicTimeLength(music);

    if (timePlayed > 1.0f)
      timePlayed = 1.0f;

    double maxHeight = 0;
    for (int i = 0; i < heights.size(); i++) {
      if (heights[i].first > rectangles[i]) {
        heights[i].second -= gravity;
      } else {
        heights[i].second += (rectangles[i] - heights[i].first) / 2.f;
      }
      heights[i].first += heights[i].second;
      heights[i].first = std::max(heights[i].first, 0.0);
      maxHeight = std::max(maxHeight, heights[i].first);
    }
    double scale = (double)5 * maxRecHeight * rms / maxHeight;
    BeginDrawing();

    ClearBackground(backgroundColor);
    for (int i = 0; i < heights.size(); i++) {
      CLITERAL(Color)
      tempColor = InBetweenColor(highColor, lowColor,
                                 (float)i / ((rectanglesNum / 2) - 2));
      DrawRectangle(2 * i * screenWidth / rectanglesNum,
                    maxRecHeight - (heights[i].first * scale),
                    2 * 0.95 * screenWidth / rectanglesNum,
                    heights[i].first * scale, tempColor);
    }
    DrawRectangle(0, maxRecHeight, screenWidth, playBarHeight, GRAY);
    DrawRectangle(0, maxRecHeight, floor((float)timePlayed * screenWidth),
                  playBarHeight, WHITE);
    DrawRectangleLines(0, maxRecHeight, floor((float)timePlayed * screenWidth),
                       playBarHeight, BLACK);
    DrawRectangle(2 * ((rectanglesNum / 2) - 2) * screenWidth / rectanglesNum,
                  maxRecHeight * (1 - volume), 4 * screenWidth / rectanglesNum,
                  volume * maxRecHeight, WHITE);

    EndDrawing();
  }
  UnloadMusicStream(music);
  CloseAudioDevice();
  CloseWindow();
}