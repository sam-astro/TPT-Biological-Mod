#include "simulation/ElementCommon.h"
#include <iostream>
#include <random>

int Element_GGOO_update(UPDATE_FUNC_ARGS);
static int graphics(GRAPHICS_FUNC_ARGS);

void Element::Element_GGOO()
{
	srand(time(NULL));

	Identifier = "DEFAULT_PT_GGOO";
	Name = "GGOO";
	Colour = PIXPACK(0x343434);
	MenuVisible = 1;
	MenuSection = SC_LIQUID;
	Enabled = 1;

	Advection = 0.6f;
	AirDrag = 0.01f * CFDS;
	AirLoss = 0.98f;
	Loss = 0.95f;
	Collision = 0.0f;
	Gravity = 0.1f;
	Diffusion = 0.3f;
	HotAir = 0.000f * CFDS;
	Falldown = 2;

	Flammable = 0;
	Explosive = 0;
	Meltable = 0;
	Hardness = 20;

	Weight = 3;

	DefaultProperties.temp = 72.0f + 273.15f;
	HeatConduct = 251;
	Description = "Grey Goo. Turns everything into grey goo. Docile when very cold, and killed when very hot. Eats faster as it gets warmer.";

	Properties = TYPE_LIQUID | PROP_DEADLY;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = 305.0f;
	LowTemperatureTransition = NT;
	HighTemperature = 2000.0f;
	HighTemperatureTransition = PT_NONE;

	DefaultProperties.pavg[1] = 250;

	Update = &Element_GGOO_update;
	Graphics = &graphics;
}

int Element_GGOO_update(UPDATE_FUNC_ARGS)
{
	//pavg[0] measures how many frames until it is cured (0 if still actively spreading and not being cured)
	//pavg[1] measures how many frames until it dies
	int r, rx, ry, rndstore = RNG::Ref().gen();
	int randomNumber = 1 + rand() % ((1000 + 1) - 1);

	if (parts[i].temp > randomNumber + 170 && parts[i].temp > 270)
	{
		for (rx = -1; rx < 2; rx++)
			for (ry = -1; ry < 2; ry++)
			{
				if (BOUNDS_CHECK && (rx || ry))
				{
					r = pmap[y + ry][x + rx];
					if (!r)
						continue;

					//transforms things into virus here
					if (TYP(r) != PT_GGOO && TYP(r) != PT_DMND)
					{
						if (!(rndstore & 0x7))
						{
							parts[ID(r)].tmp2 = TYP(r);
							if (parts[i].pavg[1])
								parts[ID(r)].pavg[1] = parts[i].pavg[1] + 1;
							else
								parts[ID(r)].pavg[1] = 0;
							if (parts[ID(r)].temp > 0.0f)
								sim->part_change_type(ID(r), x + rx, y + ry, PT_GGOO);
						}
						rndstore >>= 3;
					}
					//protons make GGOO last forever
					else if (TYP(sim->photons[y + ry][x + rx]) == PT_PROT)
					{
						parts[i].pavg[1] = 0;
					}
				}
				//reset rndstore only once, halfway through
				else if (!rx && !ry)
					rndstore = RNG::Ref().gen();
			}
		return 0;
	}

	//decrease pavg[1] so it slowly dies
	if (parts[i].pavg[1])
	{
		if (!(rndstore & 0x7) && --parts[i].pavg[1] <= 0)
		{
			sim->kill_part(i);
			return 1;
		}
		rndstore >>= 3;
	}

	for (rx = -1; rx < 2; rx++)
		for (ry = -1; ry < 2; ry++)
		{
			if (BOUNDS_CHECK && (rx || ry))
			{
				r = pmap[y + ry][x + rx];
				if (!r)
					continue;
			}
			//reset rndstore only once, halfway through
			else if (!rx && !ry)
				rndstore = RNG::Ref().gen();
		}
	return 0;
}

static int graphics(GRAPHICS_FUNC_ARGS)
{
	*pixel_mode |= PMODE_BLUR;
	*pixel_mode |= NO_DECO;
	return 1;
}
