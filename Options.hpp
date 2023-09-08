
#define ENABLE_AA

// If this is uncommented, use Blinn-Phong instead of classic Phong model
#define BLINN_PHONG

// If this is uncommented, render bounding volumes instead of meshes
//#define RENDER_BOUNDING_VOLUMES

const int MAX_DEPTH = 5;

/* MULTI-SAMPLING */

const uint AA_SAMPLER_GRID = 3;

// Note: This has no effect when AA is enabled; we will take one sample per subpixel
const uint DOF_SAMPLER_GRID = 3;

const uint AREA_LIGHT_SAMPLER_GRID = 2;

const uint GLOSSY_SAMPLES = 4;

/* ATMOSPHERE */

const uint ATMOSPHERE_PRIMARY_SAMPLES = 8;
const uint ATMOSPHERE_LIGHT_SAMPLES = 16;

const double SUN_LIGHT_DISTANCE = 1e6;
