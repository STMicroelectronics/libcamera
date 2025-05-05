/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (C) STMicroelectronics SA 2024
 *
 * cam_helper_vd1941.cpp - camera information for vd1941 sensor
 */

#include <assert.h>
#include <cmath>
#include <stddef.h>

#include "cam_helper.h"

using namespace RPiController;
using libcamera::utils::Duration;
using namespace std::literals::chrono_literals;

class CamHelpervd1941 : public CamHelper
{
public:
	CamHelpervd1941();
	uint32_t gainCode(double gain) const override;
	double gain(uint32_t gainCode) const override;
	uint32_t exposureLines(const Duration exposure, const Duration lineLength) const override;
	Duration exposure(uint32_t exposureLines, const Duration lineLength) const override;

private:
	/*
	 * Smallest difference between the frame length and integration time,
	 * in units of lines.
	 */
	/*
	 * TODO : need characterization from application/validation team
	 */
	static constexpr int frameIntegrationDiff = 100;
	/*
	 * minExposureLines is dependant of Shutter mode :
	 *  - 4 in Global Shutter
	 *  - 2 in Rolling Shutter
	 */
	static constexpr uint32_t minExposureLines = 4;
	/*
	 * Line length is currently fixed to 3372.
	 * With a default pixel clock of 93.75 MHz, the corresponding line time is 8.992 µs.
	 */
	static constexpr Duration timePerLine = 8.992us;
};

/*
 * vd1941 currently doesn't output metadata, so we have to use the "unicam parser"
 * which works by counting frames.
 */

CamHelpervd1941::CamHelpervd1941()
	: CamHelper({}, frameIntegrationDiff)
{
}

uint32_t CamHelpervd1941::gainCode(double gain) const
{
	return static_cast<uint32_t>(16.0 - 16.0 / gain);
}

double CamHelpervd1941::gain(uint32_t gainCode) const
{
	return 16.0 / (16.0 - static_cast<double>(gainCode));
}

uint32_t CamHelpervd1941::exposureLines(const Duration exposure,
					[[maybe_unused]] const Duration lineLength) const
{
	return std::max<uint32_t>(minExposureLines, exposure / timePerLine);
}

Duration CamHelpervd1941::exposure(uint32_t exposureLines,
				   [[maybe_unused]] const Duration lineLength) const
{
	return std::max<uint32_t>(minExposureLines, exposureLines) * timePerLine;
}

static CamHelper *create()
{
	return new CamHelpervd1941();
}

static RegisterCamHelper reg("vd1941", &create);
