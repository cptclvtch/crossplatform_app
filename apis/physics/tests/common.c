#define TITLE "common code"
POST_TITLE

#undef SUBTITLE
#define SUBTITLE "get_inverse_mass - 2.0"
float im;
im = get_inverse_mass(2.0);
VERIFY_SINGLE_VALUE(im*1000, ==, 1/2.0*1000)
COLLECT_FINDINGS

// #undef SUBTITLE
// #define SUBTITLE "get_inverse_mass - 0.0"
// im = get_inverse_mass(0.0);
// VERIFY_SINGLE_VALUE(im*1000, ==, 0.9999999*1000)
// COLLECT_FINDINGS

// #undef SUBTITLE
// #define SUBTITLE "energy_conserved_mass_scaling"
// float s = energy_conserved_mass_scaling(1.0, 3.0 - 1.0);
// VERIFY_SINGLE_VALUE(s*1000, ==, 1.0/(2.0)*1000)
// COLLECT_FINDINGS

// #undef SUBTITLE
// #define SUBTITLE "energy_conserved_speed_scaling"
// s = energy_conserved_speed_scaling(1.0, 3.0 - 1.0);
// VERIFY_SINGLE_VALUE(s*1000, ==, 1.0/(2.0)*1000)
// COLLECT_FINDINGS

//FIXME figure out whats wrong