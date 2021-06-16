#include "simulation/ElementCommon.h"
#include <iostream>
#include <random>

static int update(UPDATE_FUNC_ARGS);
static int graphics(GRAPHICS_FUNC_ARGS);

void Element::Element_WBCG()
{
	srand(time(NULL));

	Identifier = "DEFAULT_PT_WBCG";
	Name = "WBCG";
	Colour = PIXPACK(0xed7777);
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
	PhotonReflectWavelengths = 0xed7777;

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
	int r, rx, ry, trade;

	for (rx = -1; rx < 2; rx++)
		for (ry = -1; ry < 2; ry++)
			for (trade = 0; trade < 8; trade++)
			{
				r = pmap[y + ry][x + rx];
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

				if (RNG::Ref().chance(1, 20000) && parts[i].tmp > 50)
				{
					rx = RNG::Ref().between(-2, 2);
					ry = RNG::Ref().between(-2, 2);
					r = pmap[y + ry][x + rx];

					if (!TYP(r))
					{
						int w = sim->create_part(-3, x + rx, y + ry, PT_WBLD);
						parts[i].tmp -= 40;
					}
					else if (TYP(r) == PT_BLV)
						parts[ID(r)].flags += 6;
				}
			}

	if (parts[i].tmp > 100)
		parts[i].tmp = 100;
	return 0;
}

static int graphics(GRAPHICS_FUNC_ARGS)
{
	int tmp = cpart->tmp;
	int tmp2 = cpart->tmp2;
	int infectedAmount = cpart->pavg[0];
	*colr = (int)restrict_flt(((tmp * 10) / 2), 20, 237) + tmp2;
	*colg = (int)restrict_flt(((tmp * 10) / 8), 20, 119) + tmp2 + infectedAmount * 20;
	*colb = (int)restrict_flt(119, 20, 119) + tmp2;
	return 0;
}
