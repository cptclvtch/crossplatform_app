#undef TITLE
#define TITLE "phys_world"
POST_TITLE

phys_world world_a = (phys_world){0};

#undef SUBTITLE
#define SUBTITLE "add_point - Best Case Scenario"
phys_point* point_a = add_point(&world_a, (vec3){0});
VERIFY_SINGLE_VALUE(point_a, !=, NULL)
COLLECT_FINDINGS

#undef SUBTITLE
#define SUBTITLE "add_point - NULL world"
point_a = add_point(NULL, (vec3){0});
VERIFY_SINGLE_VALUE(point_a, ==, NULL)
COLLECT_FINDINGS