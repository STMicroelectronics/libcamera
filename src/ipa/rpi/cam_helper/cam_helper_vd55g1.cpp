/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (C) 2021, Raspberry Pi Ltd
 *
 * cam_helper_vd55g1.cpp - camera information for vd55g1 sensor
 */

#include <assert.h>

#include "cam_helper.h"

using namespace RPiController;

class CamHelperVd55g1 : public CamHelper
{
public:
	CamHelperVd55g1();
	uint32_t gainCode(double gain) const override;
	double gain(uint32_t gainCode) const override;
	void getDelays(int &exposureDelay, int &gainDelay,
		       int &vblankDelay, int &hblankDelay) const override;

private:
	/*
	 * Smallest difference between the frame length and integration time,
	 * in units of lines.
	 */
	static constexpr int frameIntegrationDiff = 4;
};

/*
 * VD5511 doesn't output metadata, so we have to use the "unicam parser" which
 * works by counting frames.
 */

CamHelperVd55g1::CamHelperVd55g1()
	: CamHelper({}, frameIntegrationDiff)
{
}

uint32_t CamHelperVd55g1::gainCode(double gain) const
{
	return static_cast<uint32_t>(gain * 16.0);
}

double CamHelperVd55g1::gain(uint32_t gainCode) const
{
	return static_cast<double>(gainCode) / 16.0;
}

void CamHelperVd55g1::getDelays(int &exposureDelay, int &gainDelay,
				int &vblankDelay, int &hblankDelay) const
{
	/* The driver appears to behave as follows: */
	exposureDelay = 2;
	gainDelay = 2;
	vblankDelay = 2;
	hblankDelay = 2;
}

static CamHelper *create()
{
	return new CamHelperVd55g1();
}

static RegisterCamHelper reg("vd55g1", &create);
