#include "simulation/ElementCommon.h"

static int update(UPDATE_FUNC_ARGS);
static int graphics(GRAPHICS_FUNC_ARGS);

void Element::Element_BLVI()
{
	Identifier = "DEFAULT_PT_BLVI";
	Name = "BLVI";
	Colour = PIXPACK(0xb03c3c);
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
	PhotonReflectWavelengths = 0xb03c3c;

	Weight = 100;

	DefaultProperties.tmp = 100;

	HeatConduct = 75;
	Description = "Blood vessel input, clooects blood and transfers to any connected blood vessels";

	Properties = TYPE_SOLID | PROP_NEUTPENETRATE | PROP_LIFE_DEC | PROP_LIFE_KILL_DEC;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = 1200.0f;
	HighTemperatureTransition = PT_FIRE;

	Update = &update;
	Graphics = &graphics;
}

constexpr float ADVECTION = 0.1f;

static int update(UPDATE_FUNC_ARGS)
{
	int r, rx, ry, trade, np;

	int maxtmp = 100;

	if (RNG::Ref().chance(1, 500))
		parts[i].tmp--;

	for (rx = -1; rx < 2; rx++)
		for (ry = -1; ry < 2; ry++)
			if (BOUNDS_CHECK && (rx || ry))
			{
				if (parts[i].life <= maxtmp)
					goto trade;
			}
trade:
	for (trade = 0; trade < 4; trade++)
	{
		rx = RNG::Ref().between(-2, 2);
		ry = RNG::Ref().between(-2, 2);
		if (BOUNDS_CHECK && (rx || ry))
		{
			r = pmap[y + ry][x + rx];
			if (!r)
				continue;
			if ((TYP(r) == PT_BLVI || TYP(r) == PT_BLV) && (parts[i].tmp > parts[ID(r)].tmp) && parts[i].tmp > 0)//diffusion
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

				int infectedDiff = parts[i].pavg[0] - parts[ID(r)].pavg[0];
				if (infectedDiff == 1)
				{
					parts[ID(r)].pavg[0]++;
					parts[i].pavg[0]--;
				}
				else if (infectedDiff > 1)
				{
					parts[ID(r)].pavg[0] += infectedDiff / 2;
					parts[i].pavg[0] -= infectedDiff / 2;
				}

				int whiteBloodCellDiff = parts[i].flags - parts[ID(r)].flags;
				if (whiteBloodCellDiff == 1)
				{
					parts[ID(r)].flags++;
					parts[i].flags--;
				}
				else if (whiteBloodCellDiff > 1)
				{
					parts[ID(r)].flags += whiteBloodCellDiff / 2;
					parts[i].flags -= whiteBloodCellDiff / 2;
				}
			}
		}
	}

	if (parts[i].flags > 0 && parts[i].pavg[0] > 0)
	{
		parts[i].flags--;
		parts[i].pavg[0]--;
	}

	if (parts[i].tmp <= 0)
		parts[i].type = PT_RFLE;

	if (parts[i].tmp > 100)
		parts[i].tmp = 100;
	return 0;
}

static int graphics(GRAPHICS_FUNC_ARGS)
{
	int tmp = cpart->tmp;
	int infectedAmount = cpart->pavg[0];
	*colr = (int)restrict_flt((tmp * 10) / 3, 20, 176);
	*colg = (int)restrict_flt((tmp * 10) / 16, 20, 60) + infectedAmount * 20;
	*colb = (int)restrict_flt(60, 20, 60);

	return 0;
}
