#include "3DLSM_calibrator.h"

double calibrationParameterMatrix[4][3] = {
    /* calibration matrix */
    {0.9899004243231164,    -0.004861795267232943,  -0.02359372089701752},
    {0.013368527647434196,  1.006797542908049,      -0.012675866615768527},
    {0.001168835273746666,  0.010437258284570846,   0.9787753807124044},
    {-37.37080025465076,    26.624201044402923,     -27.7085963508693}
};

void calibrate(int* acc3D)
{ // adjust incoming accelerometer data
    int outBuffer[3] = {0,0,0};
    for (int j = 0; j < 3; j++) {
        for (int k = 0; k < 3; k++) {
            outBuffer[j] += acc3D[k]*calibrationParameterMatrix[k][j];
            if (k==2) {
                outBuffer[j] += calibrationParameterMatrix[3][j];
            }
        }
    }
    for (int i = 0; i < 3; i++) {
        acc3D[i] = outBuffer[i];
    }
}
