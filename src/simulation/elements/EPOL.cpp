#include "simulation/ElementCommon.h"
#include <iostream>
#include <random>

static int update(UPDATE_FUNC_ARGS);
static int graphics(GRAPHICS_FUNC_ARGS);

void Element::Element_EPOL()
{
	srand(time(NULL));

	Identifier = "DEFAULT_PT_EPOL";
	Name = "EPOL";
	Colour = PIXPACK(0xadff14);
	MenuVisible = 1;
	MenuSection = SC_NUCLEAR;
	Enabled = 1;

	Advection = 0.4f;
	AirDrag = 0.01f * CFDS;
	AirLoss = 0.99f;
	Loss = 0.95f;
	Collision = 0.0f;
	Gravity = 0.4f;
	Diffusion = 0.00f;
	HotAir = 0.000f	* CFDS;
	Falldown = 1;

	Flammable = 0;
	Explosive = 0;
	Meltable = 0;
	Hardness = 0;
	PhotonReflectWavelengths = 0xadff14;

	Weight = 90;

	DefaultProperties.temp = 388.15f;
	HeatConduct = 251;
	Description = "Epolonium, highly radioactive. Decays into NEUT and heats up. Is stable at sub-zero";

	Properties = TYPE_PART|PROP_NEUTPASS|PROP_RADIOACTIVE|PROP_DEADLY;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = 1800.0f;
	HighTemperatureTransition = PT_MEPL;

	Update = &update;
	Graphics = &graphics;
}
constexpr int LIMIT = 100000;

static int update(UPDATE_FUNC_ARGS)
{
	if (parts[i].temp > 300)
	{
		int randomNumber = 1 + rand() % ((10000 + 1) - 1);
		int randNumTwo = 1 + rand() % ((30 + 1) - 1);

		if (parts[i].temp > randomNumber + 430)
		{
			if (randNumTwo == 2)
				parts[i].temp += 100.0f;
			else if (randNumTwo == 3)
			{
				parts[i].type = PT_NONE;
				sim->create_part(-3, x, y, PT_NEUT);
			}
			else if (randNumTwo == 4)
			{
				parts[i].type = PT_NONE;
				sim->create_part(-3, x, y, PT_PLSM);
			}
		}

		int amount = clamp_flt(((parts[i].temp - 273) / 7 + 1) - 1, 2, 500);
		if (amount <= 2)
			amount = 3;
		int changingHeat = 1 + rand() % amount;
		parts[i].temp += changingHeat;
	}
	return 0;
}

static int graphics(GRAPHICS_FUNC_ARGS)
{
	if (cpart->tmp >= LIMIT)
	{
		*colr = 0x70;
		*colg = 0x70;
		*colb = 0x70;
	}
	else
		*pixel_mode |= PMODE_GLOW;

	return 0;
}
