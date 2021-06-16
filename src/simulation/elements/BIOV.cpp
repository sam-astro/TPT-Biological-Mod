#include "simulation/ElementCommon.h"

int Element_BIOV_update(UPDATE_FUNC_ARGS);
static int graphics(GRAPHICS_FUNC_ARGS);

void Element::Element_BIOV()
{
	Identifier = "DEFAULT_PT_BIOV";
	Name = "BIOV";
	Colour = PIXPACK(0x899146);
	MenuVisible = 1;
	MenuSection = SC_BIOLOGICAL;
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

	Weight = 7;

	DefaultProperties.temp = 72.0f + 273.15f;
	HeatConduct = 251;
	Description = "Bio Virus. Dangerous to flesh and biological life";

	Properties = TYPE_LIQUID | PROP_DEADLY;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = 305.0f;
	LowTemperatureTransition = NT;
	HighTemperature = 673.0f;
	HighTemperatureTransition = NT;

	DefaultProperties.pavg[1] = 250;

	Update = &Element_BIOV_update;
	Graphics = &graphics;
}

int Element_BIOV_update(UPDATE_FUNC_ARGS)
{
	//pavg[0] measures how many frames until it is cured (0 if still actively spreading and not being cured)
	//pavg[1] measures how many frames until it dies
	int r, rx, ry, rndstore = RNG::Ref().gen();
	if (parts[i].pavg[0])
	{
		parts[i].pavg[0] -= (rndstore & 0x1) ? 0 : 1;
		//has been cured, so change back into the original element
		if (!parts[i].pavg[0])
		{
			sim->part_change_type(i, x, y, parts[i].tmp2);
			parts[i].tmp2 = 0;
			parts[i].pavg[0] = 0;
			parts[i].pavg[1] = 0;
		}
		return 0;
		//cured virus is never in below code
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

				if (TYP(r) != PT_BIOV && (TYP(r) == PT_BLD))
				{
					if (!(rndstore & 0x7))
					{
						parts[ID(r)].pavg[1]++;

						parts[i].type = PT_NONE;
					}
					rndstore >>= 3;
				}
				else if (TYP(r) != PT_BIOV && (TYP(r) == PT_FLE2 || TYP(r) == PT_BLVI || TYP(r) == PT_BONM || TYP(r) == PT_FLE))
				{
					parts[ID(r)].pavg[0]++;

					parts[i].type = PT_NONE;
				}
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
