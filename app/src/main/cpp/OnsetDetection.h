#ifndef ANIMALS_AS_METER_ONSETDETECTION_H
#define ANIMALS_AS_METER_ONSETDETECTION_H

#include "NE10.h"
#include "Window.h"
#include <array>
#include <cmath>
#include <cstddef>

namespace btrack {
enum OnsetDetectionFunctionType
{
    EnergyEnvelope,
    EnergyDifference,
    SpectralDifference,
    SpectralDifferenceHWR,
    PhaseDeviation,
    ComplexSpectralDifference,
    ComplexSpectralDifferenceHWR,
    HighFrequencyContent,
    HighFrequencySpectralDifference,
    HighFrequencySpectralDifferenceHWR
};

class OnsetDetectionFunction {
public:
	OnsetDetectionFunction()
        : OnsetDetectionFunction(ComplexSpectralDifferenceHWR);

	explicit OnsetDetectionFunction(OnsetDetectionFunctionType t);

	~OnsetDetectionFunction();

	float calculate_sample(std::vector<float>& buffer);

private:
    static constexpr std::size_t FrameSize = 1024;
    static constexpr std::size_t HopSize = 512;
	void perform_FFT();
    float energy_envelope();
    float energy_difference();
    float spectral_difference();
    float spectral_difference_hwr();
    float phase_deviation();
	float complex_spectral_difference_hwr();
    float complex_spectral_difference();
    float high_frequency_content();
    float high_frequency_spectral_difference();
    float high_frequency_spectral_difference_hwr();

	OnsetDetectionFunctionType t;
	static constexpr Window<FrameSize> window = stompbox::window::get_window<FrameSize, WindowType>();

	std::array<ne10_fft_cpx_float32_t, FrameSize> complexOut = {};
    ne10_fft_r2c_cfg_float32_t fftCfg;

	std::array<float, FrameSize> frame = {};
	std::array<float, FrameSize> magSpec = {};
	std::array<float, FrameSize> prevMagSpec = {};
	std::array<float, FrameSize> phase = {};
	std::array<float, FrameSize> prevPhase = {};
	std::array<float, FrameSize> prevPhase2 = {};

	float prevEnergySum;
};
} // namespace btrack

#endif // STOMPBOX_ONSETDETECTION_H