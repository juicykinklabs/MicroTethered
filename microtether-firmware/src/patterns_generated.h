/* AUTOMATICALLY GENERATED FILE. DO NOT MODIFY DIRECTLY */
#pragma once
#include <Arduino.h>

static const double bytemax = 255.0;
static const double timingConversion = 1024.0;
static int k = 1;

void PP_SquarePulseSync(void(* fWrite)(uint8_t, double), void(* fDelay)(double)) {
	constexpr uint32_t LENGTH = 2;
	const uint8_t c1_x[LENGTH] = {0, 255, };
	const uint16_t durs[LENGTH] = {1024, 1024, };
	k = k % LENGTH;
	fWrite(1, c1_x[k] / bytemax);
	fWrite(2, c1_x[k] / bytemax);
	fWrite(3, c1_x[k] / bytemax);
	fDelay(durs[k] / timingConversion);
	k += 1;
}
void PP_ExponentialWaveSync(void(* fWrite)(uint8_t, double), void(* fDelay)(double)) {
	constexpr uint32_t LENGTH = 14;
	const uint8_t c1_x[LENGTH] = {0, 76, 153, 216, 242, 255, 242, 216, 204, 165, 127, 89, 63, 51, };
	const uint16_t durs[LENGTH] = {614, 256, 256, 256, 256, 256, 256, 256, 307, 358, 358, 358, 358, 307, };
	k = k % LENGTH;
	fWrite(1, c1_x[k] / bytemax);
	fWrite(2, c1_x[k] / bytemax);
	fWrite(3, c1_x[k] / bytemax);
	fDelay(durs[k] / timingConversion);
	k += 1;
}
void PP_RampSync(void(* fWrite)(uint8_t, double), void(* fDelay)(double)) {
	constexpr uint32_t LENGTH = 21;
	const uint8_t c1_x[LENGTH] = {0, 12, 25, 38, 51, 63, 76, 89, 102, 114, 127, 140, 153, 165, 178, 191, 204, 211, 226, 234, 255, };
	const uint16_t durs[LENGTH] = {512, 153, 122, 122, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 122, 133, 143, 153, 133, };
	k = k % LENGTH;
	fWrite(1, c1_x[k] / bytemax);
	fWrite(2, c1_x[k] / bytemax);
	fWrite(3, c1_x[k] / bytemax);
	fDelay(durs[k] / timingConversion);
	k += 1;
}
void PP_PlateauSync(void(* fWrite)(uint8_t, double), void(* fDelay)(double)) {
	constexpr uint32_t LENGTH = 18;
	const uint8_t c1_x[LENGTH] = {0, 12, 25, 38, 51, 63, 76, 89, 102, 114, 127, 140, 153, 165, 178, 191, 204, 255, };
	const uint16_t durs[LENGTH] = {768, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 122, 2560, };
	k = k % LENGTH;
	fWrite(1, c1_x[k] / bytemax);
	fWrite(2, c1_x[k] / bytemax);
	fWrite(3, c1_x[k] / bytemax);
	fDelay(durs[k] / timingConversion);
	k += 1;
}
void PP_SineSync(void(* fWrite)(uint8_t, double), void(* fDelay)(double)) {
	constexpr uint32_t LENGTH = 20;
	const uint8_t c1_x[LENGTH] = {0, 38, 76, 114, 153, 178, 204, 229, 242, 247, 255, 247, 242, 229, 204, 178, 153, 114, 76, 38, };
	const uint16_t durs[LENGTH] = {112, 51, 51, 51, 51, 51, 51, 51, 51, 51, 81, 51, 51, 51, 51, 51, 51, 51, 51, 51, };
	k = k % LENGTH;
	fWrite(1, c1_x[k] / bytemax);
	fWrite(2, c1_x[k] / bytemax);
	fWrite(3, c1_x[k] / bytemax);
	fDelay(durs[k] / timingConversion);
	k += 1;
}
void PP_SquarePulsePhases(void(* fWrite)(uint8_t, double), void(* fDelay)(double)) {
	constexpr uint32_t LENGTH = 5;
	const uint8_t c1_x[LENGTH] = {255, 0, 0, 255, 255, };
	const uint8_t c2_x[LENGTH] = {0, 0, 255, 255, 255, };
	const uint8_t c3_x[LENGTH] = {0, 255, 255, 0, 0, };
	const uint16_t durs[LENGTH] = {307, 307, 307, 307, 307, };
	k = k % LENGTH;
	fWrite(1, c1_x[k] / bytemax);
	fWrite(2, c2_x[k] / bytemax);
	fWrite(3, c3_x[k] / bytemax);
	fDelay(durs[k] / timingConversion);
	k += 1;
}
void PP_SinePhases(void(* fWrite)(uint8_t, double), void(* fDelay)(double)) {
	constexpr uint32_t LENGTH = 20;
	const uint8_t c1_x[LENGTH] = {0, 38, 76, 114, 153, 178, 204, 229, 242, 247, 255, 247, 242, 229, 204, 178, 153, 114, 76, 38, };
	const uint8_t c2_x[LENGTH] = {204, 229, 242, 247, 255, 247, 242, 229, 204, 178, 153, 114, 76, 38, 0, 38, 76, 114, 153, 178, };
	const uint8_t c3_x[LENGTH] = {229, 204, 178, 153, 114, 76, 38, 0, 38, 76, 114, 153, 178, 204, 229, 242, 247, 255, 247, 242, };
	const uint16_t durs[LENGTH] = {102, 51, 51, 51, 51, 51, 51, 102, 51, 51, 81, 51, 51, 51, 102, 51, 51, 51, 51, 51, };
	k = k % LENGTH;
	fWrite(1, c1_x[k] / bytemax);
	fWrite(2, c2_x[k] / bytemax);
	fWrite(3, c3_x[k] / bytemax);
	fDelay(durs[k] / timingConversion);
	k += 1;
}

void (*loadedPatterns[]) (void(*f)(uint8_t, double), void(* fDelay)(double)) = {
	PP_SquarePulseSync,
	PP_ExponentialWaveSync,
	PP_RampSync,
	PP_PlateauSync,
	PP_SineSync,
	PP_SquarePulsePhases,
	PP_SinePhases,
};
