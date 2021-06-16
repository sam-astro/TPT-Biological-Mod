#include "simulation/ElementCommon.h"

static int update(UPDATE_FUNC_ARGS);
static int graphics(GRAPHICS_FUNC_ARGS);

void Element::Element_SKIN()
{
	Identifier = "DEFAULT_PT_SKIN";
	Name = "SKIN";
	Colour = PIXPACK(0xffd387);
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
	PhotonReflectWavelengths = 0xffd387;

	Weight = 100;

	DefaultProperties.tmp = 100;

	HeatConduct = 75;
	Description = "Skin, grows on normal flesh and protects it";

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
	int r, rx, ry, trade, np;

	int maxtmp = 100;

	if (RNG::Ref().chance(1, 3))
		parts[i].tmp--;

	for (rx = -1; rx < 2; rx++)
		for (ry = -1; ry < 2; ry++)
			if (BOUNDS_CHECK && (rx || ry))
			{
				if (parts[i].life <= maxtmp)
					goto trade;
			}
trade:
	for (trade = 0; trade < 30; trade++)
	{
		rx = RNG::Ref().between(-10, 10);
		ry = RNG::Ref().between(-10, 10);
		if (BOUNDS_CHECK && (rx || ry))
		{
			r = pmap[y + ry][x + rx];
			if (!r)
				continue;
			if (TYP(r) == PT_SKIN && (parts[i].tmp > parts[ID(r)].tmp) && parts[i].tmp > 0)//diffusion
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
	for (trade = 0; trade < 30; trade++)
	{
		rx = RNG::Ref().between(-3, 3);
		ry = RNG::Ref().between(-3, 3);
		if (BOUNDS_CHECK && (rx || ry))
		{
			r = pmap[y + ry][x + rx];
			if (!r)
				continue;
			if (TYP(r) == PT_BLD)
			{
				//parts[i].type = PT_NONE;
			}
		}
	}

	if (parts[i].flags > 0 && parts[i].pavg[0] > 0)
	{
		parts[i].flags--;
		parts[i].pavg[0]--;
	}

	if (parts[i].tmp <= 0)
		parts[i].type = PT_NONE;

	if (parts[i].tmp > 100)
		parts[i].tmp = 100;
	return 0;
}

static int graphics(GRAPHICS_FUNC_ARGS)
{

	return 0;
}
