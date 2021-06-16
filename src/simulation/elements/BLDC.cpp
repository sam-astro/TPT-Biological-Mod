#include "simulation/ElementCommon.h"

static int update(UPDATE_FUNC_ARGS);
static int graphics(GRAPHICS_FUNC_ARGS);

void Element::Element_BLDC()
{
	Identifier = "DEFAULT_PT_BLDC";
	Name = "BLDC";
	Colour = PIXPACK(0xbd0404);
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
	PhotonReflectWavelengths = 0xbd0404;

	Weight = 100;

	DefaultProperties.tmp = 100;

	HeatConduct = 75;
	Description = "Deforms and disappears under pressure.";

	Properties = TYPE_SOLID | PROP_LIFE_DEC | PROP_LIFE_KILL_DEC;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = -4.0f;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = 273.0f;
	HighTemperatureTransition = PT_BLD;

	Update = &update;
	Graphics = &graphics;
}

constexpr float ADVECTION = 0.1f;

static int update(UPDATE_FUNC_ARGS)
{
	if (parts[i].tmp > 1)
		parts[i].type = PT_BLD;
	return 0;
}

static int graphics(GRAPHICS_FUNC_ARGS)
{

	return 0;
}
