# User can override these variables as needed:
CC = g++
LD := $(CC)
CFLAGS =
LDFLAGS =
ARCH =
MARCH = native
MCPU =
MTUNE =
MFLAGS = -mssse3 -msahf -mfpmath=sse,387
OFLAGS = -O3 -fstrict-aliasing -ftree-vectorize -fomit-frame-pointer -flto
WFLAGS = -Wall -Wno-multichar
INC = /opt/local/include /opt/local/include/SDL /usr/include
LIBDIR = /usr/lib64
LIB = libSDLmain.a libSDL_net.so libSDL_mixer.so libSDL_ttf.so libSDL_image.so libSDL.so libGLEW.a libGLU.so libGL.so
DEF =
EXE = xwingrev
VERSION = 0.1.3
GAMEDIR = /Games/X-Wing Revival
SERVERDIR = /srv/xwingrev
SERVERUSER = xwingrev
BUILD_FINALIZE =
INSTALL_FINALIZE =
MAC_APP = X-Wing Revival
MAC_FRAMEWORKS = OpenGL Cocoa AudioUnit AudioToolbox IOKit Carbon
MAC_INSTALL_NAME_TOOL = /opt/local/bin/install_name_tool
MAC_BUNDLE_LIBS = /opt/local/lib/gcc49/libstdc++.6.dylib /opt/local/lib/gcc49/libgcc_s.1.dylib /opt/local/lib/libgcc/libstdc++.6.dylib /opt/local/lib/libgcc/libgcc_s.1.dylib
MAC_PPC_ARCH = ppc
MAC_PPC_MCPU = G3
MAC_PPC_MTUNE = G5
MAC_I32_ARCH = pentium4
MAC_I32_MARCH = nocona
MAC_I32_MTUNE = nocona
MAC_I64_ARCH = x86_64
MAC_I64_MARCH = core2
MAC_I64_MTUNE = core2


INCLUDES = Sources Sources/Game Sources/UI $(wildcard ../RaptorEngine/*) $(INC)
LIBRARIES = $(foreach lib,$(LIB),$(LIBDIR)/$(lib))
SOURCES = $(wildcard Sources/*.cpp) $(wildcard Sources/*/*.cpp) $(wildcard ../RaptorEngine/*/*.cpp)
HEADERS = $(wildcard Sources/*.h) $(wildcard Sources/*/*.h) $(wildcard ../RaptorEngine/*/*.h)
EXE_INSTALL = $(EXE)
BINDIR = $(GAMEDIR)
XFLAGS =


UNAME = $(shell uname)
ifeq ($(UNAME), Darwin)
# Begin Mac OS X section.

ifndef ARCH
# Unless ARCH is defined, Macs should make 32-bit universal binaries by default.
ARCH = $(MAC_PPC_ARCH) $(MAC_I32_ARCH)
endif

# Don't use default march=native on Macs.
MARCH =

# Use MacPorts lib directory.
LIBDIR = /opt/local/lib

# Always suggest 10.4 Tiger as the minimum Mac OS X version.
XFLAGS += -mmacosx-version-min=10.4

ifeq (,$(findstring $(MAC_I64_ARCH),$(ARCH)))
# When building without i64 target, use the 10.4 Tiger universal SDK and ppc/i32 MacPorts libs.
XFLAGS += -isysroot /Developer/SDKs/MacOSX10.4u.sdk
LIBDIR = /Developer/SDKs/MacOSX10.4u.sdk/opt/local/lib
endif

KERNEL_VERSION = $(shell uname -r)
ifneq (8.11.0,$(KERNEL_VERSION))
ifneq ($(MAC_I64_ARCH),$(ARCH))
# When building on 10.5+ with any non-i64 target, use Apple gcc 4.0 and disable stack protector.
CC = /Developer/usr/bin/g++-4.0
LD = /Developer/usr/bin/g++-4.0
OFLAGS += -fno-stack-protector
endif
else
# When building on 10.4, specify gcc 4.0.
CC = g++-4.0
LD = g++-4.0
endif

GCC_VERSION = $(shell $(CC) -dumpversion)
GCC_VERSION_MAJOR = $(word 1,$(subst ., ,$(GCC_VERSION)))
GCC_VERSION_MINOR = $(word 2,$(subst ., ,$(GCC_VERSION)))
ifneq (,$(findstring 4.9.,$(GCC_VERSION)))
# When using GCC 4.9, don't let the linker use compact unwind.
XFLAGS += -Wl,-no_compact_unwind
else
ifeq (5,$(GCC_VERSION_MAJOR))
# When using GCC 5, don't let the linker use compact unwind.
XFLAGS += -Wl,-no_compact_unwind
endif
endif

ifneq (,$(findstring -4.0,$(CC)))
# When using gcc 4.0, don't use link-time optimization (it's not supported).
OFLAGS := $(filter-out -flto,$(OFLAGS))
endif

ifeq ($(MAC_PPC_ARCH),$(ARCH))
MFLAGS =
else
# Specify SSE3/SSSE3 when building for anything other than ppc.
ifeq (,$(findstring -4.0,$(CC)))
# On gcc > 4.0, use SSSE3.
MFLAGS = -mssse3
ifneq (,$(findstring $(MAC_I64_ARCH),$(ARCH)))
# For i64 on gcc > 4.0, enable SAHF instruction.
MFLAGS += -msahf
endif
else
# On gcc 4.0, use SSE3.
MFLAGS = -msse3
endif
ifeq (,$(findstring $(MAC_PPC_ARCH),$(ARCH)))
# Specify mfpmath=both if there's no ppc target.
MFLAGS += -mfpmath=sse,387
endif
endif

# Override LIB specified above for Linux.
LIB = libSDL_net.a libSDL_ttf.a libSDL_image.a libSDL_mixer.a libSDL.a libSDLmain.a libGLEW.a libmikmod.a libsmpeg.a libflac.a libpng.a libfreetype.a libvorbisfile.a libvorbis.a libogg.a libtiff.a libXrandr.a libXrender.a libXext.a libX11.a libxcb.a libXdmcp.a libXau.a libjpeg.a libbz2.a liblzma.a libz.a

# Macs don't have .so files, so replace with .a files.
LIBRARIES := $(patsubst %.so,%.a,$(LIBRARIES))

# Add frameworks to Mac linker line.
LIBRARIES += $(foreach framework,$(MAC_FRAMEWORKS),-framework $(framework))

# Macs must pad install names so install_name_tool can make them longer.
XFLAGS += -headerpad_max_install_names

# Fix dynamic library paths.
BUILD_FINALIZE += $(foreach lib,$(MAC_BUNDLE_LIBS),$(MAC_INSTALL_NAME_TOOL) -change "$(lib)" "@executable_path/$(notdir $(lib))" "$(EXE)";)

ifdef MAC_APP
# Create application package.
EXE_INSTALL = $(MAC_APP)
BINDIR = $(GAMEDIR)/$(MAC_APP).app/Contents/MacOS
INSTALL_FINALIZE += mkdir -p "$(GAMEDIR)/$(MAC_APP).app/Contents/Resources"; cp -p "Info.plist" "$(GAMEDIR)/$(MAC_APP).app/Contents/"; rsync -ax --exclude=".*" "English.lproj" "$(GAMEDIR)/$(MAC_APP).app/Contents/Resources/"; cp -p "xwing128.icns" "$(GAMEDIR)/$(MAC_APP).app/Contents/Resources/";
endif

ifdef MAC_BUNDLE_LIBS
# Include necessary dylibs.
INSTALL_FINALIZE += $(foreach lib,$(MAC_BUNDLE_LIBS),cp -p "$(lib)" "$(BINDIR)/"; chmod 644 "$(BINDIR)/$(notdir $(lib))";)
endif

# End Mac OS X section.
endif


# Generate an object build directory based on the target architecture(s).
NULL :=
SPACE := $(NULL) $(NULL)
ifdef ARCH
ARCHDIR = arch-$(subst $(SPACE),-,$(ARCH))/
else
ARCHDIR = arch-native/
endif
OBJECTS = $(patsubst Sources/%.cpp,build/$(ARCHDIR)XWing/%.o,$(patsubst ../RaptorEngine/%.cpp,build/$(ARCHDIR)RaptorEngine/%.o,$(SOURCES)))
OBJDIRS = $(dir $(OBJECTS))

# Build a list of architecture flags.
AFLAGS =
ifdef ARCH
AFLAGS = $(foreach arch,$(ARCH),-arch $(arch))
endif
ifdef MARCH
AFLAGS += -march=$(MARCH)
endif
ifdef MCPU
AFLAGS += -mcpu=$(MCPU)
endif
ifdef MTUNE
AFLAGS += -mtune=$(MTUNE)
endif


default: $(SOURCES) $(HEADERS) $(EXE)

$(EXE): $(OBJECTS)
	$(LD) $(AFLAGS) $(MFLAGS) $(OFLAGS) $(WFLAGS) $(XFLAGS) $(LDFLAGS) $(OBJECTS) $(LIBRARIES) -o $@
	-$(BUILD_FINALIZE)

build/$(ARCHDIR)XWing/%.o: Sources/%.cpp $(HEADERS) $(foreach objdir,$(OBJDIRS),$(objdir).dir)
	$(CC) -c $(AFLAGS) $(MFLAGS) $(OFLAGS) $(WFLAGS) $(XFLAGS) $(CFLAGS) $(foreach inc,$(INCLUDES),-I$(inc)) $(foreach def,$(DEF),-D$(def)) $< -o $@

build/$(ARCHDIR)RaptorEngine/%.o: ../RaptorEngine/%.cpp $(HEADERS) $(foreach objdir,$(OBJDIRS),$(objdir).dir)
	$(CC) -c $(AFLAGS) $(MFLAGS) $(OFLAGS) $(WFLAGS) $(XFLAGS) $(CFLAGS) $(foreach inc,$(INCLUDES),-I$(inc)) $(foreach def,$(DEF),-D$(def)) $< -o $@

build/%/.dir:
	mkdir -p $(dir $@)
	touch $@

objects: $(SOURCES) $(HEADERS) $(OBJECTS)

clean:
	rm -rf build/arch-* "$(EXE)" "$(EXE)"_*

install:
	mkdir -p "$(BINDIR)"
	cp "$(EXE)" "$(BINDIR)/$(EXE_INSTALL)"
	cp -p build/Debug/README.txt "$(GAMEDIR)/"
	rsync -ax --exclude=".*" build/Debug/Fonts "$(GAMEDIR)/"
	rsync -ax --exclude=".*" build/Debug/Textures "$(GAMEDIR)/"
	rsync -ax --exclude=".*" build/Debug/Models "$(GAMEDIR)/"
	rsync -ax --exclude=".*" build/Debug/Shaders "$(GAMEDIR)/"
	rsync -ax --exclude=".*" build/Debug/Sounds "$(GAMEDIR)/"
	rsync -ax --exclude=".*" build/Debug/Music "$(GAMEDIR)/"
	rsync -ax --exclude=".*" build/Debug/Screensaver "$(GAMEDIR)/"
	rsync -ax --exclude=".*" build/Debug/Tools "$(GAMEDIR)/"
	rsync -ax --exclude=".*" build/Debug/Docs "$(GAMEDIR)/"
	chmod ugo+x "$(BINDIR)/$(EXE_INSTALL)"
	$(INSTALL_FINALIZE)

uninstall:
	rm -r "$(GAMEDIR)"

play:
	cd "$(GAMEDIR)"; "$(BINDIR)/$(EXE_INSTALL)"

server-install:
	mkdir -p "$(SERVERDIR)"
	cp xwingctl "$(SERVERDIR)/"
	-"$(SERVERDIR)/xwingctl" stop
	cp "$(EXE)" "$(SERVERDIR)/$(EXE)-$(VERSION)"
	cd "$(SERVERDIR)" && ln -sf "$(EXE)-$(VERSION)" "$(EXE)"
	-chown -R $(SERVERUSER):wheel "$(SERVERDIR)"
	-chmod 775 "$(SERVERDIR)/$(EXE)-$(VERSION)"
	-ln -sf "$(SERVERDIR)/xwingctl" /usr/local/bin/xwingctl
	-"$(SERVERDIR)/xwingctl" start

ppc:
	make objects ARCH="$(MAC_PPC_ARCH)" MCPU="$(MAC_PPC_MCPU)" MTUNE="$(MAC_PPC_MTUNE)"
	make EXE="$(EXE)_ppc" ARCH="$(MAC_PPC_ARCH)" MARCH="$(MAC_PPC_MARCH)" MTUNE="$(MAC_PPC_MTUNE)"

i32:
	make objects ARCH="$(MAC_I32_ARCH)" MARCH="$(MAC_I32_MARCH)" MTUNE="$(MAC_I32_MTUNE)"
	make EXE="$(EXE)_i32" ARCH="$(MAC_I32_ARCH)" MARCH="$(MAC_I32_MARCH)" MTUNE="$(MAC_I32_MTUNE)"

i64:
	make objects ARCH="$(MAC_I64_ARCH)" MARCH="$(MAC_I64_MARCH)" MTUNE="$(MAC_I64_MTUNE)" CC="/opt/local/bin/g++"
	make EXE="$(EXE)_i64" ARCH="$(MAC_I64_ARCH)" MARCH="$(MAC_I64_MARCH)" MTUNE="$(MAC_I64_MTUNE)" CC="/opt/local/bin/g++"

universal:
	make ppc EXE="$(EXE)"
	make i32 EXE="$(EXE)"
	make i64 EXE="$(EXE)"
	make lipo EXE="$(EXE)"

lipo: $(EXE)_ppc $(EXE)_i32 $(EXE)_i64
	lipo $(EXE)_ppc $(EXE)_i32 $(EXE)_i64 -create -output $(EXE)
