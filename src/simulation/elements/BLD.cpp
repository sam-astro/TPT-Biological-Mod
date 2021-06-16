#include "simulation/ElementCommon.h"

static int update(UPDATE_FUNC_ARGS);
static int graphics(GRAPHICS_FUNC_ARGS);

void Element::Element_BLD()
{
	Identifier = "DEFAULT_PT_BLD";
	Name = "BLD";
	Colour = PIXPACK(0xd40f0f);
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
	DefaultProperties.tmp = 0;
	HeatConduct = 29;
	Description = "Blood, absorbs and deposits oxygen, and flows through living things";

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
	Graphics = &graphics;
}

static int update(UPDATE_FUNC_ARGS)
{
	//if (parts[i].tmp <= 0)
		//parts[i].type = PT_BLDC;

	int r, rx, trade, ry;

	for (rx = -2; rx < 3; rx++)
		for (ry = -2; ry < 3; ry++)
			if (BOUNDS_CHECK && (rx || ry))
			{
				r = pmap[y + ry][x + rx];
				if (TYP(r) == PT_FLE2)
				{
					if (parts[ID(r)].tmp < 100 && parts[i].tmp > 5)
					{
						parts[ID(r)].tmp += 5;
						parts[i].tmp -= 5;
						parts[ID(r)].pavg[0] = parts[i].pavg[0];
					}
				}
				if (TYP(r) == PT_FLE || TYP(r) == PT_WBCG)
				{
					if (parts[ID(r)].tmp < 100)
					{
						parts[ID(r)].tmp += 1;
						parts[i].tmp -= 1;
						parts[ID(r)].pavg[0] = parts[i].pavg[0];
					}
				}
				if (TYP(r) == PT_BLVI)
				{
					if (parts[ID(r)].tmp < 100)
					{
						parts[ID(r)].tmp += 1;
						parts[ID(r)].pavg[0] = parts[i].pavg[0];
						sim->kill_part(i);
					}
				}
				//if (TYP(r) == PT_O2)
				//{
				//	if (parts[i].tmp < 100)
				//	{
				//		parts[i].tmp += 4;
				//		parts[ID(r)].type = PT_CO2;
				//	}
				//}
				goto trade;
			}

trade:
	for (trade = 0; trade < 6; trade++)
	{
		rx = RNG::Ref().between(-2, 2);
		ry = RNG::Ref().between(-2, 2);
		if (BOUNDS_CHECK && (rx || ry))
		{
			r = pmap[y + ry][x + rx];
			if (!r)
				continue;
			if ((TYP(r) == PT_BLD) && (parts[i].tmp > parts[ID(r)].tmp) && parts[i].tmp > 0)//diffusion
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
			}
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
	*colr = (int)restrict_flt(((tmp * 10) / 2), 20, 255) + tmp2;
	*colg = (int)restrict_flt(((tmp * 10) / 30), 0, 17) + tmp2 + infectedAmount * 20;
	*colb = (int)restrict_flt(0, 0, 20) + tmp2;

	return 0;
}
