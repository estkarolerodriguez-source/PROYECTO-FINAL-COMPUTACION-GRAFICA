// Use the bundled full stb header if available in librerias/include/stb
#define STB_IMAGE_IMPLEMENTATION
#ifdef _MSC_VER
#include "../librerias/include/stb/stb_image.h"
#else
#include "librerias/include/stb/stb_image.h"
#endif
