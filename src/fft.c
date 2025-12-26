#include <stdio.h>
#include <math.h>
#include <complex.h>

#define PI 3.14159265358979323846

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
