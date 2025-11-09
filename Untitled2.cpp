#include <opencv2/opencv.hpp>
#include <iostream>
#include <math.h>
constexpr int N = 8; //size of the block
constexpr double PI = 3.14159265358979323846;

double block[N][N] = { 0 };

double quant = 2.0;//qunatization value

void idct8x8(const double in[N][N], double out[N][N]) {
    for (int x = 0; x < N; ++x) {
        for (int y = 0; y < N; ++y) {
            double sum = 0.0;

            for (int u = 0; u < N; u++) {
                for (int v = 0; v < N; v++) {
                    double Cu = (u == 0) ? 1.0 / sqrt(2.0) : 1.0;
                    double Cv = (v==0) ? 1.0/sqrt(2.0) :1.0;
                    sum += Cu * Cv * in[u][v]
                        * cos((PI / N) * (x + 0.5) * u)
                        * cos((PI / N) * (y + 0.5) * v);

                }
            }
            out[x][y] = sum * 0.25;
        
        }
    }
}
int main() {
   

    //for the input coefficients we will be filling just 0,0 to each values.
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            block[i][j] = 0.0;
        }
    }
    //modifying some values here like this insdie the input.
            block[0][0] = 50;//strong DC
            block[0][1] = 30;//low frequency horizontal mode
            block[1][1] = -20;//low frequncy verical mode
            block[5][0] = 70; 
                //block[5][5] =120;
            //firstly we take the input encoded vlaues right and then
            // we multiply by quantization parameter for that block
            //that block matrix will be our input which we pass to the computational fuinction
            double dequant[N][N];
            for (int i = 0; i < N; i++)  {
                for (int j = 0; j <N; j++) {
                    dequant[i][j] = block[i][j] * quant;//scaling factor is quant

                }
            }

            double out[N][N];
            idct8x8(dequant, out);

            cv::Mat show(N, N, CV_8UC3);
            for (int i = 0; i < N; ++i) {
                for (int j = 0; j < N; ++j) {
                    int p = static_cast<int>(out[i][j]);
                    if (p < 0) p = 0;
                    if (p > 255)p = 255;
                    show.at<cv::Vec3b>(i, j) = static_cast<cv::Vec3b>(p,p,p);

                }
            }

            cv::Mat big;
            cv::resize(show, big, cv::Size(620, 620), 0, 0, cv::INTER_NEAREST);

        cv::imshow(
            "IDCT macroblock", 
            big
        );
        cv::waitKey(0);
        return 0;
    

}
 