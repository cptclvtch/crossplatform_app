#undef TITLE
#define TITLE "particle bunches"
POST_TITLE

#undef SUBTITLE
#define SUBTITLE "get_new_particle_bunch - Best Case Scenario"
cpu_particles* bunch = get_new_particle_bunch((vec3){0}, 10);
VERIFY_SINGLE_VALUE(bunch->array, !=, NULL)
VERIFY_SINGLE_VALUE(bunch->max_particle_index, ==, 10)
bunch->array[bunch->max_particle_index].p = (vec3){0}; //SEGFAULT check
COLLECT_FINDINGS
delete_particle_bunch(bunch);

#undef SUBTITLE
#define SUBTITLE "get_new_particle_bunch - 1 particle"
bunch = get_new_particle_bunch((vec3){0}, 0);
VERIFY_SINGLE_VALUE(bunch->array, !=, NULL)
COLLECT_FINDINGS
delete_particle_bunch(bunch);

//TODO integration? presets?