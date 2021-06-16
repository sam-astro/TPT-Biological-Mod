#include "simulation/ElementCommon.h"

static int update(UPDATE_FUNC_ARGS);

void Element::Element_RADN()
{
	Identifier = "DEFAULT_PT_RADN";
	Name = "RADN";
	Colour = PIXPACK(0x849e72);
	MenuVisible = 1;
	MenuSection = SC_NUCLEAR;
	Enabled = 1;
	
	Advection = 2.0f;
	AirDrag = 0.00f * CFDS;
	AirLoss = 0.99f;
	Loss = 0.30f;
	Collision = -0.1f;
	Gravity = 0.3f;
	Diffusion = 1.0f;
	HotAir = 0.000f	* CFDS;
	Falldown = 1;

	Flammable = 0;
	Explosive = 0;
	Meltable = 1000;
	Hardness = 0;
	PhotonReflectWavelengths = 0x849e72;

	Weight = 10;

	DefaultProperties.temp = R_TEMP + 4.0f + 273.15f;
	HeatConduct = 251;
	Description = "Radon, a gas that decays into neutrons";

	Properties = TYPE_GAS | PROP_NEUTPASS | PROP_RADIOACTIVE;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = 90.0f;
	LowTemperatureTransition = PT_LRAD;
	HighTemperature = ITH;
	HighTemperatureTransition = NT;

	Update = &update;
}

constexpr int COOLDOWN = 1;
constexpr int LIMIT = 50;

static int update(UPDATE_FUNC_ARGS)
{
	int r = sim->photons[y][x];
	
	if (RNG::Ref().chance(1, 1000))
	{
		sim->create_part(i, x, y, PT_NEUT);
	}

	if (parts[i].tmp2 >= 10)
	{
		sim->part_change_type(i, x, y, PT_PLUT);
		parts[i].temp = (parts[i].temp + 600.0f) / 2.0f;
		return 1;
	}
	if (TYP(r) == PT_PROT)
	{
		parts[i].tmp2++;
		sim->kill_part(ID(r));
	}
	if (parts[i].temp < 388.15f)
	{
		parts[i].temp += 0.2f;
	}
	return 0;
}