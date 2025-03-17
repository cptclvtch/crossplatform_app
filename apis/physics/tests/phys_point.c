#undef TITLE
#define TITLE "phys_point"
POST_TITLE

#undef SUBTITLE
#define SUBTITLE "integrate_phys_point - Best Case Scenario"
phys_point p = (phys_point){0};
integrate_phys_point(&p, 0.0);
VERIFY_SINGLE_VALUE(p.p.x, ==, 0.0)
COLLECT_FINDINGS

//TODO test more stuff?