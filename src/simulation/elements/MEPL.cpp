#include "simulation/ElementCommon.h"
#include <iostream>
#include <random>

static int update(UPDATE_FUNC_ARGS);
static int graphics(GRAPHICS_FUNC_ARGS);

void Element::Element_MEPL()
{
	srand(time(NULL));

	Identifier = "DEFAULT_PT_MEPL";
	Name = "MEPL";
	Colour = PIXPACK(0xE05010);
	MenuVisible = 0;
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
	Falldown = 2;

	Flammable = 0;
	Explosive = 0;
	Meltable = 0;
	Hardness = 2;
	PhotonReflectWavelengths = 0xadff14;

	Weight = 90;

	DefaultProperties.temp = 1900.15f;
	HeatConduct = 251;
	Description = "Molten epolonium, highly radioactive and hot. Decays into NEUT and heats up.";

	Properties = TYPE_LIQUID |PROP_NEUTPASS|PROP_RADIOACTIVE|PROP_DEADLY;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = 1799.0f;
	LowTemperatureTransition = PT_EPOL;
	HighTemperature = 1800.0f;
	HighTemperatureTransition = NT;

	Update = &update;
	Graphics = &graphics;
}
constexpr int LIMIT = 100000;

static int update(UPDATE_FUNC_ARGS)
{
	if (parts[i].temp > 300)
	{
		int randomNumber = 1 + rand() % ((10000 + 1) - 1);
		int randNumTwo = 1 + rand() % ((1000 + 1) - 1);

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
	*colr = cpart->life * 2 + 0xE0;
	*colg = cpart->life * 1 + 0x50;
	*colb = cpart->life / 2 + 0x10;
	if (*colr > 255) *colr = 255;
	if (*colg > 192) *colg = 192;
	if (*colb > 128) *colb = 128;
	*firea = 40;
	*firer = *colr;
	*fireg = *colg;
	*fireb = *colb;
	*pixel_mode |= FIRE_ADD;
	*pixel_mode |= PMODE_BLUR;
	//Returning 0 means dynamic, do not cache
	return 0;
}
