/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (C) STMicroelectronics SA 2024
 *
 * Camera information for vd1943 sensor
 */

#include <assert.h>

#include "cam_helper.h"

using namespace RPiController;

class CamHelperVd1943 : public CamHelper
{
public:
	CamHelperVd1943();
	uint32_t gainCode(double gain) const override;
	double gain(uint32_t gainCode) const override;

private:
	/*
	 * Smallest difference between the frame length and integration time,
	 * in units of lines.
	 */
	/*
	 * TODO : need characterization from application/validation team
	 */
	static constexpr int frameIntegrationDiff = 100;
};

/*
 * VD1943 currently doesn't output metadata, so we have to use the "unicam parser"
 * which works by counting frames.
 */

CamHelperVd1943::CamHelperVd1943()
	: CamHelper({}, frameIntegrationDiff)
{
}

uint32_t CamHelperVd1943::gainCode(double gain) const
{
	return static_cast<uint32_t>(16.0 - 16.0 / gain);
}

double CamHelperVd1943::gain(uint32_t gainCode) const
{
	return static_cast<double>(16.0 / (16 - gainCode));
}

static CamHelper *create()
{
	return new CamHelperVd1943();
}

static RegisterCamHelper reg("vd1943", &create);
