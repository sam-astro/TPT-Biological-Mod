#include "simulation/ElementCommon.h"
#include <iostream>
#include <random>

static int update(UPDATE_FUNC_ARGS);
static int graphics(GRAPHICS_FUNC_ARGS);

void Element::Element_AVOL()
{
	srand(time(NULL));

	Identifier = "DEFAULT_PT_AVOL";
	Name = "AVOL";
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
	DefaultProperties.tmp2 = 0;

	HeatConduct = 75;
	Description = "Aveoli, gathers oxygen from the air and transfers to blood.";

	Properties = TYPE_SOLID | PROP_LIFE_DEC | PROP_LIFE_KILL_DEC;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = 600.0f;
	HighTemperatureTransition = PT_FIRE;

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

	int r, rx, ry, trade, np;

	int maxtmp = 100;

	for (rx = -1; rx < 2; rx++)
		for (ry = -1; ry < 2; ry++)
			if (BOUNDS_CHECK && (rx || ry))
				goto trade;
trade:
	for (trade = 0; trade < 12; trade++)
	{
		rx = RNG::Ref().between(-3, 3);
		ry = RNG::Ref().between(-3, 3);
		if (BOUNDS_CHECK && (rx || ry))
		{
			r = pmap[y + ry][x + rx];
			if (!r)
				continue;
			if ((TYP(r) == PT_AVOL) && (parts[i].tmp > parts[ID(r)].tmp) && parts[i].tmp > 0)//diffusion
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
			}
			if (TYP(r) == PT_O2)
			{
				if (parts[i].tmp < 100)
				{
					parts[i].tmp += 12;
					parts[ID(r)].type = PT_CO2;
				}
			}
			if (TYP(r) == PT_BLD)
			{
				if (parts[ID(r)].tmp < 100 && parts[i].tmp > 17)
				{
					parts[ID(r)].tmp += 12;
					parts[i].tmp -= 12;
				}
			}
		}
	}

	if (parts[i].flags > 0 && parts[i].pavg[0] > 0)
	{
		parts[i].flags--;
		parts[i].pavg[0]--;
	}

	if(parts[i].pavg[1] > 0)
		if (RNG::Ref().chance(1, 100))
			parts[i].pavg[1]--;

	if (parts[i].tmp > 100)
		parts[i].tmp = 100;
	return 0;
}

static int graphics(GRAPHICS_FUNC_ARGS)
{
	int tmp = cpart->tmp;
	int tmp2 = cpart->tmp2;
	int infectedAmount = cpart->pavg[0];
	*colr = (int)restrict_flt(((tmp * 10) / 3), 20, 176) + tmp2;
	*colg = (int)restrict_flt(((tmp * 10) / 16), 20, 60) + tmp2 + infectedAmount * 20;
	*colb = (int)restrict_flt(60, 20, 60) + tmp2;

	return 0;
}
