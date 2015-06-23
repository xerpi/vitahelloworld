## vitahelloworld
Homebrew sample for the PSVita.

## Compiling

In order two compile this sample you need to download:
* [devkitARM] (http://sourceforge.net/projects/devkitpro/files/devkitARM)
* [psp2sdk] (https://github.com/173210/psp2sdk)
* [vita-toolchain] (https://github.com/vitasdk/vita-toolchain)

First you need to install devkitARM, on my OS I've installed it to:
```
	/opt/devkitpro/devkitARM
```
To install it you can follow this tutorial: http://devkitpro.org/wiki/Getting_Started/devkitARM

Once you got it installed, first setup an environment variable called PSP2SDK, for example by adding to the ~/.bashrc:
```
	export PSP2SDK=$DEVKITPRO/psp2
	export PATH=$PATH:$PSP2SDK/bin
```

Then you need to compile and install psp2sdk:
```
	$ cd psp2sdk
	$ export PATH=$DEVKITARM/bin:$PATH
	$ sh autogen.sh --prefix=$PSP2SDK
	$ cd tools && make install && cd ..
	$ cd src && make install
```

Now we need to install vita-toolchain:
```
	$ cd vita-toolchain
	$ make
	$ cp vita-libs-gen $(PSP2SDK)/bin
	$ cp vita-elf-create $(PSP2SDK)/bin
```

Now you can compile this homebrew by typing:
```
	$ make
```

The resulting file will be called: vitahelloworld.velf

## Credits
Thanks to yifanlu for Rejuvenate and UVLoader :D
Thanks to 173210 and everybody who contributed to psp2sdk.
Thanks to Cirne and everybody who contributed to vita-toolchain.
Also thanks to everybody who has helped me on #vitadev :P
