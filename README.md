# TPT-Biological-Mod
Mod for the game "The powder toy" that introduces some biological elements

This is all of the source code, but the build version is in <code>MostRecentBuild/powder.exe</code>

> I do not own the original code for this, and the only things I created are the elements for this mod.

## Features
* BLD - Blood, absorbs oxygen. Will transfer oxygen to FLE, FLE2, and moves inside BLV
* AVOL - Aveoli, acts like lungs and converts oxygen to co2. Gets more oxygen than blood, then transfers to nearby blood
* BLV - Blood vessel, BLVI will transfer into it and it will carry throughout
* BLVO - Blood vessel out, an output for blood vessel
* BLVI - Blood vessel in, collects blood to put in blood vessel
* BLDC - Blood clot, solid version of blood
* FLE - Flesh, uses oxygen. It will grow a protective layer of skin on it if it has enough blood, and if it doesn't have any skin then it will lose blood by bleeding
* FLE2 - Innard flesh, for use with organs and internals bacause it does not bleed but also doesn't grow skin
* BONM - Bone marrow, slowly creates blood. If it is touching a blood vessel (BLV) then it will transfer directly to that
* SKIN - Skin, grows on flesh
* NERV - Basically METL, just burns instead of melts. Conducts.
* DFLE - Dead flesh, solid flesh that no longer works. Anything that doesn't get enough oxygen from blood will turn into this
* RFLE - Rotten flesh, powdered dead flesh, DFLE will turn into this as it rots
* BIOV - Biological virus, any organic objects will absorb this. If an object gets too concentrated with BIOV, then it will begin to rot
* WBLD - White blood cells, removes blood clots (BLDC) and removes viruses (BIOV) from biological objects
* WBCG - White blood cell gland, if it is supplied with blood (BLD) then it will slowly produce white blood cells (WBLD)
