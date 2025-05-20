/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (C) STMicroelectronics SA 2025
 *
 * cam_helper_vd55g0.cpp - camera information for vd55g0 sensor
 */

#include <assert.h>

#include "cam_helper.h"

using namespace RPiController;

class CamHelperVd55g0 : public CamHelper
{
public:
	CamHelperVd55g0();
	uint32_t gainCode(double gain) const override;
	double gain(uint32_t gainCode) const override;

private:
	/*
	 * Smallest difference between the frame length and integration time,
	 * in units of lines.
	 */
	static constexpr int frameIntegrationDiff = 4;
};

/*
 * OV9281 doesn't output metadata, so we have to use the "unicam parser" which
 * works by counting frames.
 */

CamHelperVd55g0::CamHelperVd55g0()
	: CamHelper({}, frameIntegrationDiff)
{
}

uint32_t CamHelperVd55g0::gainCode(double gain) const
{
	return static_cast<uint32_t>(32.0 - 32.0 / gain);
}

double CamHelperVd55g0::gain(uint32_t gainCode) const
{
	return static_cast<double>(32.0 / (32 - gainCode));
}

static CamHelper *create()
{
	return new CamHelperVd55g0();
}

static RegisterCamHelper reg("vd55g0", &create);
