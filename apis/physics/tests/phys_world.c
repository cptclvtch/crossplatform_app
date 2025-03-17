#undef TITLE
#define TITLE "phys_world"
POST_TITLE

#undef SUBTITLE
#define SUBTITLE "setup_phys_world - Best Case Scenario"
phys_world world_a = (phys_world){0};
setup_phys_world(&world_a);
VERIFY_SINGLE_VALUE(world_a.points.delete_func, ==, NULL)
VERIFY_SINGLE_VALUE(world_a.particle_bunches.delete_func, ==, &delete_particle_bunch)
// VERIFY_SINGLE_VALUE(world_a.force_generators.delete_func, ==, &delete_force_generator)
VERIFY_SINGLE_VALUE(world_a.virtual_springs.delete_func, ==, &delete_virtual_spring)
COLLECT_FINDINGS

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