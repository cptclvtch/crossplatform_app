#undef TITLE
#define TITLE "phys_virtual_spring"
POST_TITLE

#undef SUBTITLE
#define SUBTITLE "get_virtual_spring - NULL points"
phys_virtual_spring* spring_a = get_virtual_spring(NULL,NULL,0,0);
VERIFY_SINGLE_VALUE(spring_a, ==, NULL)
COLLECT_FINDINGS

#undef SUBTITLE
#define SUBTITLE "get_virtual_spring - Best Case Scenario"
phys_point a = (phys_point){0};
phys_point b = (phys_point){0};
spring_a = get_virtual_spring(&a,&b, 0,0);
VERIFY_SINGLE_VALUE(spring_a, !=, NULL)
VERIFY_SINGLE_VALUE(spring_a->k, ==, 1.0)
COLLECT_FINDINGS