#include "simulation/ElementCommon.h"

static int update(UPDATE_FUNC_ARGS);

void Element::Element_CURE()
{
	Identifier = "DEFAULT_PT_CURE";
	Name = "CURE";
	Colour = PIXPACK(0x70d4d2);
	MenuVisible = 1;
	MenuSection = SC_BIOLOGICAL;
	Enabled = 1;

	Advection = 0.6f;
	AirDrag = 0.01f * CFDS;
	AirLoss = 0.98f;
	Loss = 0.95f;
	Collision = 0.0f;
	Gravity = 0.1f;
	Diffusion = 0.00f;
	HotAir = 0.000f * CFDS;
	Falldown = 2;

	Flammable = 0;
	Explosive = 0;
	Meltable = 0;
	Hardness = 2;

	Weight = 30;

	DefaultProperties.temp = R_TEMP - 2.0f + 273.15f;
	DefaultProperties.tmp = 20;
	HeatConduct = 29;
	Description = "Cure, reverts dead and rotting flesh and too much could make the organism make invincible";

	Properties = TYPE_LIQUID | PROP_NEUTPASS;

	LowPressure = -4.0f;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = 250.0f;
	LowTemperatureTransition = PT_BLDC;
	HighTemperature = ITH;
	HighTemperatureTransition = NT;

	Update = &update;
}

static int update(UPDATE_FUNC_ARGS)
{
	int r, rx, ry;

	for (rx = -1; rx < 2; rx++)
		for (ry = -1; ry < 2; ry++)
			if (BOUNDS_CHECK && (rx || ry))
			{
				r = pmap[y + ry][x + rx];

				if (TYP(r) == PT_FLE)
					sim->kill_part(i);

				if (TYP(r) == PT_DFLE || TYP(r) == PT_RFLE)
				{
					if (parts[ID(r)].tmp < 100)
					{
						parts[ID(r)].type = PT_FLE2;
						parts[ID(r)].tmp += 3;
						parts[ID(r)].pavg[1] += 1;
						parts[ID(r)].pavg[0] = 0;
						sim->kill_part(i);
					}
				}
			}
	return 0;
}
