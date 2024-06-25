/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (C) STMicroelectronics SA 2023
 *
 * cam_helper_vd56g3.cpp - camera information for vd56g3 sensor
 */

#include <assert.h>

#include "cam_helper.h"

using namespace RPiController;

class CamHelperVd56g3 : public CamHelper
{
public:
	CamHelperVd56g3();
	uint32_t gainCode(double gain) const override;
	double gain(uint32_t gainCode) const override;
	void getDelays(int &exposureDelay, int &gainDelay,
		       int &vblankDelay, int &hblankDelay) const override;

private:
	/*
	 * Smallest difference between the frame length and integration time,
	 * in units of lines.
	 */
	static constexpr int frameIntegrationDiff = 60;
};

/*
 * VD56G3 currently doesn't output metadata, so we have to use the "unicam parser"
 * which works by counting frames.
 */

CamHelperVd56g3::CamHelperVd56g3()
	: CamHelper({}, frameIntegrationDiff)
{
}

uint32_t CamHelperVd56g3::gainCode(double gain) const
{
	return static_cast<uint32_t>(32.0 - 32.0 / gain);
}

double CamHelperVd56g3::gain(uint32_t gainCode) const
{
	return 32.0 / (32.0 - static_cast<double>(gainCode));
}

void CamHelperVd56g3::getDelays(int &exposureDelay, int &gainDelay,
				int &vblankDelay, int &hblankDelay) const
{
	/*
	 * vd56g3 is able to apply settings to next frame, if they arrive 20 lines
	 * before VSYNC, so to ensure predictable behavior add one more frame.
	 */
	exposureDelay = 2;
	gainDelay = 2;
	vblankDelay = 2;
	hblankDelay = 2;
}

static CamHelper *create()
{
	return new CamHelperVd56g3();
}

static RegisterCamHelper reg("vd56g3", &create);
