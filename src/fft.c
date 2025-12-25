#include <stdio.h>
#include <math.h>
#include <complex.h>

double PI;

void _fft(double complex buf[], double complex out[], int n, int step)
{
	if (step < n) {
		_fft(out, buf, n, step * 2);
		_fft(out + step, buf + step, n, step * 2);

		for (int i = 0; i < n; i += 2 * step) {
			double complex t = cexp(-I * PI * i / n) * out[i + step];
			buf[i / 2]     = out[i] + t;
			buf[(i + n)/2] = out[i] - t;
		}
	}
}

void fft(double complex buf[], int n)
{
	double complex out[n];
	for (int i = 0; i < n; i++)
        out[i] = buf[i];

	_fft(buf, out, n, 1);
}

// int main()
// {
//     int N = 8;
// 	PI = atan2(1, 1) * 4;

//     double complex samples[N];

//     for(int i = 0; i < N; ++i) {
//         double t = (double)i / N;
//         samples[i] = sin(2*PI*t*1) + sin(2*PI*t*2);
//     }

// 	fft(samples, N);

//     // print dft result
//     for(int i = 0; i < N; ++i) {
//         // printf("[%d] %f\t%f\n", i, out_sin[i], out_cos[i]);
//         printf("[%d] %lf + %lfi\n", i, creal(samples[i]), cimag(samples[i]));
//     }

// 	return 0;
// }