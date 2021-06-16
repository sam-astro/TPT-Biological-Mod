#include "simulation/ElementCommon.h"

static int update(UPDATE_FUNC_ARGS);

void Element::Element_LRAD()
{
	Identifier = "DEFAULT_PT_LRAD";
	Name = "LRAD";
	Colour = PIXPACK(0xffff0f);
	MenuVisible = 1;
	MenuSection = SC_LIQUID;
	Enabled = 1;

	Advection = 0.6f;
	AirDrag = 0.01f * CFDS;
	AirLoss = 0.98f;
	Loss = 0.95f;
	Collision = 0.0f;
	Gravity = 0.1f;
	Diffusion = 0.00f;
	HotAir = 0.000f	* CFDS;
	Falldown = 2;

	Flammable = 0;
	Explosive = 0;
	Meltable = 0;
	Hardness = 0;

	Weight = 30;

	DefaultProperties.temp = 0.0f;
	HeatConduct = 70;
	Description = "Liquid Radon. Very cold.";

	Properties = TYPE_LIQUID | PROP_RADIOACTIVE | PROP_NEUTPASS;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = 90.1f;
	HighTemperatureTransition = PT_RADN;

	Update = &update;
}


static int update(UPDATE_FUNC_ARGS)
{
	int r = sim->photons[y][x];

	if (RNG::Ref().chance(1, 100) && RNG::Ref().chance(int(5.0f * sim->pv[y / CELL][x / CELL]), 1000))
	{
		sim->create_part(i, x, y, PT_NONE);
	}
	if (RNG::Ref().chance(1, 10000) && !parts[i].tmp)
	{
		int s = sim->create_part(-3, x, y, PT_NEUT);
		if (s >= 0)
		{
			parts[i].tmp++;

			parts[i].temp = ((parts[i].temp + parts[s].temp) + 600.0f) / 2.0f;
			parts[s].temp = parts[i].temp;
		}
	}

	if (r && RNG::Ref().chance(1, 100))
	{
		int s = sim->create_part(-3, x, y, PT_NEUT);
		if (s >= 0)
		{
			parts[i].temp = ((parts[i].temp + parts[ID(r)].temp + parts[ID(r)].temp) + 600.0f) / 3.0f;
			parts[i].tmp++;

			parts[ID(r)].temp = parts[i].temp;

			parts[s].temp = parts[i].temp;
			parts[s].vx = parts[ID(r)].vx;
			parts[s].vy = parts[ID(r)].vy;
		}
	}
	return 0;
}