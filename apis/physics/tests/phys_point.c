#undef TITLE
#define TITLE "phys_point"
POST_TITLE

#undef SUBTITLE
#define SUBTITLE "linear_integration - Best Case Scenario"
phys_point particle = (phys_point){0};
linear_integration(&particle, 0.0);
VERIFY_SINGLE_VALUE(particle.p.x, ==, 0.0)
COLLECT_FINDINGS

//TODO test more stuff?