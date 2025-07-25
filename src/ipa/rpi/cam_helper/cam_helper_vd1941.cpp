/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (C) STMicroelectronics SA 2024
 *
 * Camera information for vd1941 sensor
 */

#include <assert.h>

#include "cam_helper.h"

using namespace RPiController;

class CamHelperVd1941 : public CamHelper
{
public:
	CamHelperVd1941();
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
 * VD1941 currently doesn't output metadata, so we have to use the "unicam parser"
 * which works by counting frames.
 */

CamHelperVd1941::CamHelperVd1941()
	: CamHelper({}, frameIntegrationDiff)
{
}

uint32_t CamHelperVd1941::gainCode(double gain) const
{
	return static_cast<uint32_t>(16.0 - 16.0 / gain);
}

double CamHelperVd1941::gain(uint32_t gainCode) const
{
	return static_cast<double>(16.0 / (16 - gainCode));
}

static CamHelper *create()
{
	return new CamHelperVd1941();
}

static RegisterCamHelper reg("vd1941", &create);
