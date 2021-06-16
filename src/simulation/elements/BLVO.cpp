#include "simulation/ElementCommon.h"
#include <iostream>
#include <random>

static int update(UPDATE_FUNC_ARGS);
static int graphics(GRAPHICS_FUNC_ARGS);

void Element::Element_BLVO()
{
	srand(time(NULL));

	Identifier = "DEFAULT_PT_BLVO";
	Name = "BLVO";
	Colour = PIXPACK(0xb03c3c);
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
	PhotonReflectWavelengths = 0xb03c3c;

	Weight = 100;

	DefaultProperties.tmp = 100;

	HeatConduct = 75;
	Description = "Deforms and disappears under pressure.";

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
	int r, rx, ry, trade, np;

	if (RNG::Ref().chance(1, 500))
		parts[i].tmp--;

	int maxtmp = 100;

	for (rx = -1; rx < 2; rx++)
		for (ry = -1; ry < 2; ry++)
			for (trade = 0; trade < 10; trade++)
			{
				if (RNG::Ref().chance(1, 150))
				{
					rx = RNG::Ref().between(-5, 5);
					ry = RNG::Ref().between(-5, 5);
					r = pmap[y + ry][x + rx];
					if (BOUNDS_CHECK && (rx || ry))
					{
						if (TYP(r) == PT_BLVO && (parts[i].tmp > parts[ID(r)].tmp) && parts[i].tmp > 0)//diffusion
						{
							int difference = parts[i].tmp - parts[ID(r)].tmp;
							if (difference == 1)
							{
								parts[ID(r)].tmp++;
								parts[i].tmp--;
							}
							else if (difference > 1)
							{
								parts[ID(r)].tmp += difference / 2;
								parts[i].tmp -= difference / 2;
							}

							int infectedDiff = parts[i].pavg[0] - parts[ID(r)].pavg[0];
							if (infectedDiff == 1)
							{
								parts[ID(r)].pavg[0]++;
								parts[i].pavg[0]--;
							}
							else if (infectedDiff > 1)
							{
								parts[ID(r)].pavg[0] += infectedDiff / 2;
								parts[i].pavg[0] -= infectedDiff / 2;
							}

							int whiteBloodCellDiff = parts[i].flags - parts[ID(r)].flags;
							if (whiteBloodCellDiff == 1)
							{
								parts[ID(r)].flags++;
								parts[i].flags--;
							}
							else if (whiteBloodCellDiff > 1)
							{
								parts[ID(r)].flags += whiteBloodCellDiff / 2;
								parts[i].flags -= whiteBloodCellDiff / 2;
							}
						}
					}

					if (parts[i].tmp > 1 && !TYP(r))
					{
						int w = sim->create_part(-3, x + rx, y + ry, PT_BLD);
						parts[w].pavg[0] = parts[i].pavg[0];
						parts[i].pavg[0] = 0;
						//sim->part_change_type(r, x + rx, y + ry, PT_BLD);
						parts[i].tmp -= 6;
					}
					if (parts[i].flags > 0 && !TYP(r))
					{
						int w = sim->create_part(-3, x + rx, y + ry, PT_WBLD);
						parts[i].flags--;
					}
				}
			}

	if (parts[i].flags > 0 && parts[i].pavg[0] > 0)
	{
		parts[i].flags--;
		parts[i].pavg[0]--;
	}

	if (parts[i].tmp <= 0)
		if (RNG::Ref().chance(1, 1000))
			parts[i].type = PT_DFLE;

	if (parts[i].tmp > 100)
		parts[i].tmp = 100;
	return 0;
}

static int graphics(GRAPHICS_FUNC_ARGS)
{
	int tmp = cpart->tmp;
	int infectedAmount = cpart->pavg[0];
	*colr = (int)restrict_flt((tmp * 10) / 3, 20, 176);
	*colg = (int)restrict_flt((tmp * 10) / 16, 20, 60) + infectedAmount * 20;
	*colb = (int)restrict_flt(60, 20, 60);

	return 0;
}
