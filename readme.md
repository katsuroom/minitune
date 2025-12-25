# minitune

Mini music player with customizable playlist support.

Made using Raylib v5.5 with Raygui v4.0.
- Raylib: uses commit **11429b4** for `SeekMusicStream()` fix (issue #4521), built from source.

## How it Works
- Load music file into a Music stream instance.
- Use Raylib's `AttachAudioStreamProcessor()` to read samples from audio buffer.
- Samples are 32-bit floats in range (-1, 1)

### Volume Visualization
- Take root mean square (RMS) of samples.
$$
\text{RMS} = \sqrt{\frac{1}{N} \sum_{i=1}^N {x_i}^2}
$$
- Convert to decibels, set minimum to -40 dB.
$$
\text{dB} = 20 * log_{10}(\text{RMS})
$$

### Frequency Visualization
- Fast Fourier Transform on samples.
- FFT code from: https://rosettacode.org/wiki/Fast_Fourier_transform#C

