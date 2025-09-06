/*Mesh considerations:
- cache face normals
- store edges (origin and direction)*/

vec3 minmax_separating_axis_test(real min_a, real max_a, real min_b, real max_b)
{
    real min = (min_a > min_b)? min_a : min_b;
    real max = (max_a < max_b)? max_a : max_b;

    return (vec3){max-min, min, max};
}
vec3 halfsize_separating_axis_test(real center_a, real half_a, real center_b, real half_b)
{
    return minmax_separating_axis_test(center_a - half_a, center_a + half_a, center_b - half_b, center_b + half_b);
}

linked_list_node* pick_test_axes(collision_volume* a, collision_volume* b)
{
    if(a->type != MESH || b->type != MESH) return NULL;

    //face combinations
    

    //edge combinations

    //prune parallels

    return NULL;
}