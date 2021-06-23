# TPT-Biological-Mod
Mod for the game "The powder toy" that introduces some biological elements

This is all of the source code, but the build version is in <code>MostRecentBuild/powder.exe</code>

Make sure to download updates from here often, since I am always making this mod better and fixing bugs

I do not own the original code for this, and the only things I created are the elements for this mod.

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

### Using the Source code
<h2><span class="mw-headline" id="Required_environment_setup">Required environment setup</span></h2>
<ul><li> install Git (<a rel="nofollow" class="external text" href="https://git-scm.com/">get it here</a>)
<ul><li> options should be left as is</li></ul></li>
<li> install Python (<a rel="nofollow" class="external text" href="https://www.python.org/">get it here</a>)
<ul><li> it is recommended to allow the installer to add Python to PATH and to disable the path length limit (the latter is offered near the end of the installation process)</li></ul></li>
<li> open an elevated command prompt (search for "cmd" the Start Menu, right-click the most sensible result, click "Run as administrator") and execute the following commands</li></ul>
<pre>python -m pip install --upgrade pip
pip install meson ninja
</pre>
<ul><li> install Visual Studio (<a rel="nofollow" class="external text" href="https://visualstudio.microsoft.com/vs/community/">get it here</a>)
<ul><li> select the Desktop Development workload</li>
<li> you really only need "MSVC" and "Windows 10 SDK", so you can uncheck everything else in the list on the right</li></ul></li>
<li> find "x64 Native Tools Command Prompt for VS" (or similar, hereafter referred to as "VS prompt") in the Start Menu and execute the following commands
<ul><li> it is recommended to pin the resulting window to your taskbar; you will be using this a lot to build TPT</li></ul></li></ul>
<pre>cd /d [wherever you keep your repositories]
git clone <a rel="nofollow" class="external free" href="https://github.com/The-Powder-Toy/The-Powder-Toy">https://github.com/The-Powder-Toy/The-Powder-Toy</a>
</pre>
<h2><span class="mw-headline" id="Building_for_the_first_time">Building for the first time</span></h2>
<ul><li> open a VS prompt (see above) and execute the following commands</li></ul>
<pre>cd /d [wherever you keep your repositories]
cd The-Powder-Toy
meson build-debug
cd build-debug
ninja
</pre>
<ul><li> you may see a few warnings throughout all of this, but no errors (if you do at any stage, do not try to skip it; ask us about it <a rel="nofollow" class="external text" href="https://powdertoy.co.uk/Discussions/Categories/Topics.html?Category=5">on the forum</a> instead)
<ul><li> if you are not sure, run Ninja again; if it says "no work to do", everything worked fine</li></ul></li>
<li> at this point, running TPT from the prompt should be possible</li></ul>
<pre>powder.exe
</pre>
<h2><span class="mw-headline" id="Using_the_Visual_Studio_IDE">Using the Visual Studio IDE</span></h2>
<p>The method above does not let you use the 'Visual' part of Visual Studio, the IDE. Although Meson has limited support for this use case, if for some reason you wish to use the IDE, you can ask Meson to generate a build site that uses Visual Studio instead of Ninja.
</p>
<ul><li> open a VS prompt (see above) and execute the following commands</li></ul>
<pre>cd /d [wherever you keep your repositories]
cd The-Powder-Toy
meson --backend=vs -Dbackend_startup_project=powder build-debug-vs
</pre>
<ul><li> you will not need the VS dev prompt beyond this point</li></ul>
<p>This will generate a build site with a Visual Studio solution inside (<code>the-powder-toy.sln</code>). You can use this as any other solution.
</p>
