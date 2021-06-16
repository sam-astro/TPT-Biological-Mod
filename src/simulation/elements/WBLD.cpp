#include "simulation/ElementCommon.h"

static int update(UPDATE_FUNC_ARGS);

void Element::Element_WBLD()
{
	Identifier = "DEFAULT_PT_WBLD";
	Name = "WBLD";
	Colour = PIXPACK(0xfefefe);
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

	Weight = 18;

	DefaultProperties.temp = R_TEMP - 2.0f + 273.15f;
	DefaultProperties.tmp = 20;
	HeatConduct = 29;
	Description = "White blood cells, kills infection and clears blood clots.";

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

	if(parts[i].tmp <= 0)
		sim->kill_part(i);

	for (rx = -1; rx < 2; rx++)
		for (ry = -1; ry < 2; ry++)
			if (BOUNDS_CHECK && (rx || ry))
			{
				r = pmap[y + ry][x + rx];
				if (TYP(r) == PT_BLDC)
				{
					if (parts[ID(r)].tmp < 100)
					{
						parts[ID(r)].tmp += 1;
						parts[i].tmp--;
					}
				}
				if (TYP(r) == PT_BIOV)
				{
					parts[i].tmp--;
					sim->kill_part(ID(r));
				}
				if (TYP(r) == PT_BLVI || TYP(r) == PT_FLE || TYP(r) == PT_FLE2)
				{
					if (parts[ID(r)].flags < 2)
					{
						parts[ID(r)].flags++;
						sim->kill_part(i);
					}
				}
			}
	return 0;
}
