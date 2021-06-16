#include "simulation/ElementCommon.h"
#include <iostream>
#include <random>

static int update(UPDATE_FUNC_ARGS);
static int graphics(GRAPHICS_FUNC_ARGS);

void Element::Element_BONM()
{
	srand(time(NULL));

	Identifier = "DEFAULT_PT_BONM";
	Name = "BONM";
	Colour = PIXPACK(0xffffff);
	MenuVisible = 1;
	MenuSection = SC_BIOLOGICAL;
	Enabled = 1;

	Advection = 0.0f;
	AirDrag = 0.00f * CFDS;
	AirLoss = 0.97f;
	Loss = 0.50f;
	Collision = 0.0f;
	Gravity = 0.0f;
	Diffusion = 0.00f;
	HotAir = 0.000f * CFDS;
	Falldown = 0;

	Flammable = 1;
	Explosive = 0;
	Meltable = 0;
	Hardness = 12;
	PhotonReflectWavelengths = 0xffffff;

	Weight = 100;

	DefaultProperties.tmp = 100;

	HeatConduct = 75;
	Description = "Bone marrow, create blood. Deposits directly into blood vessel";

	Properties = TYPE_SOLID | PROP_LIFE_DEC | PROP_LIFE_KILL_DEC | PROP_BLOODPASS;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = 1200.0f;
	HighTemperatureTransition = NT;

	Update = &update;
	Graphics = &graphics;
}

constexpr float ADVECTION = 0.1f;

static int update(UPDATE_FUNC_ARGS)
{
	if (parts[i].tmp2 == 0)
		for (int w = 0; w < 100; w++)
			if (RNG::Ref().chance(1, 10))
				parts[i].tmp2++;

	int r, rx, ry, trade;

	for (rx = -1; rx < 2; rx++)
		for (ry = -1; ry < 2; ry++)
			for (trade = 0; trade < 8; trade++)
			{
				if (RNG::Ref().chance(1, 200))
				{
					rx = RNG::Ref().between(-2, 2);
					ry = RNG::Ref().between(-2, 2);
					r = pmap[y + ry][x + rx];

					if (!TYP(r))
						int w = sim->create_part(-3, x + rx, y + ry, PT_BLD);
					else if (TYP(r) == PT_BLV)
						parts[ID(r)].tmp += 6;
				}
			}

	if (parts[i].tmp > 100)
		parts[i].tmp = 100;
	return 0;
}

static int graphics(GRAPHICS_FUNC_ARGS)
{
	int tmp2 = cpart->tmp2;

	*colr = 230 + tmp2 * 2;
	*colg = 230 + tmp2 * 2;
	*colb = 230 + tmp2 * 2;

	return 0;
}
