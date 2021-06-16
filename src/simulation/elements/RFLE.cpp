#include "simulation/ElementCommon.h"

static int update(UPDATE_FUNC_ARGS);
static int graphics(GRAPHICS_FUNC_ARGS);

void Element::Element_RFLE()
{
	Identifier = "DEFAULT_PT_RFLE";
	Name = "RFLE";
	Colour = PIXPACK(0xc541919);
	MenuVisible = 1;
	MenuSection = SC_BIOLOGICAL;
	Enabled = 1;

	Advection = 0.0f;
	AirDrag = 0.00f * CFDS;
	AirLoss = 0.97f;
	Loss = 0.70f;
	Collision = 0.0f;
	Gravity = 0.1f;
	Diffusion = 0.3f;
	HotAir = 0.000f * CFDS;
	Falldown = 1.1f;

	Flammable = 1;
	Explosive = 0;
	Meltable = 0;
	Hardness = 12;
	PhotonReflectWavelengths = 0xc541919;

	DefaultProperties.tmp = 100;

	Weight = 100;

	HeatConduct = 75;
	Description = "Deforms and disappears under pressure.";

	Properties = TYPE_PART | PROP_NEUTPENETRATE | PROP_LIFE_DEC | PROP_LIFE_KILL_DEC;

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
	parts[i].type = PT_RFLE;
	return 0;
}

static int graphics(GRAPHICS_FUNC_ARGS)
{
	int tmp = cpart->tmp;
	*colr = (int)restrict_flt((84 - tmp * 20), 20, 84);
	*colg = (int)restrict_flt((25 - tmp * 20), 20, 25);
	*colb = (int)restrict_flt((25 - tmp * 20), 20, 25);

	return 0;
}
