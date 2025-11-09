//#include "idctutils.h"
//#include <cmath>
//#include <random>
//
////const int N = 8;
////const double MY_PI = 3.14159265358979323846;
//
//int my_round(double x) { return static_cast<int>(x + (x >= 0 ? 0.5 : -0.5)); }
//
//void idct8x8(const double input[N][N], double output[N][N]) {
//    for (int x = 0; x < N; ++x)
//        for (int y = 0; y < N; ++y) {
//            double sum = 0.0;
//            for (int u = 0; u < N; ++u)
//                for (int v = 0; v < N; ++v) {
//                    double Cu = (u == 0) ? 1.0 / sqrt(2.0) : 1.0;
//                    double Cv = (v == 0) ? 1.0 / sqrt(2.0) : 1.0;
//                    sum += Cu * Cv * input[u][v]
//                        * cos((MY_PI / N) * (x + 0.5) * u)
//                        * cos((MY_PI / N) * (y + 0.5) * v);
//                }
//            output[x][y] = sum * 0.25;
//        }
//}
//
//void generate_encoded_video(std::vector<std::vector<std::vector<std::vector<std::vector<double>>>>>& encoded_data,
//    int num_frames, int blocks_h, int blocks_w, int channels) {
//    std::mt19937 rng(12345);
//    std::uniform_real_distribution<double> DCTval(-128, 128);
//
//    for (int f = 0; f < num_frames; ++f) {
//        encoded_data[f].resize(blocks_h, std::vector<std::vector<std::vector<double>>>(blocks_w, std::vector<std::vector<double>>(channels, std::vector<double>(N * N, 0.0))));
//        for (int by = 0; by < blocks_h; ++by)
//            for (int bx = 0; bx < blocks_w; ++bx)
//                for (int ch = 0; ch < channels; ++ch)
//                    for (int i = 0; i < N * N; ++i) {
//                        if (i == 0)
//                            encoded_data[f][by][bx][ch][i] = ((bx + by + ch + f) % 32) * 8 - 128;
//                        else
//                            encoded_data[f][by][bx][ch][i] = DCTval(rng) * ((ch == 0) ? 0.3 : 1.0);
//                    }
//    }
//}
