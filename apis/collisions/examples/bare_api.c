#include <stdio.h>

#define INCLUDE_IMPLEMENTATION
#include "../api.c"

void simple_loop(binary_tree* scene)
{
    collision_list* collisions = get_new_collision_list();

    uint32_t frame = 1000;
    while(frame)
    {
        //perform collision calculations
        gather_contacts(scene, collisions, NULL);

        //move randomly


        frame--;
    }

    clear_collision_list(collisions);
}

void custom_collision_combos(binary_tree* scene, collision_list* list)
{
    void* custom_data = (void*)list;
    binary_tree_cross_traversal(scene->child[0], scene->child[1], _self_collision_test, _self_collision_func, custom_data);
    //lets say one side is static volumes, no need to check between static objects, skip it
    binary_tree_sibling_traversal(scene->child[1], _self_collision_test, _self_collision_func, custom_data);
}

void custom_combo_loop(binary_tree* scene)
{
    collision_list* collisions = get_new_collision_list();

    uint32_t frame = 1000;
    while(frame)
    {
        //perform collision calculations
        gather_contacts(scene, collisions, custom_collision_combos);

        //update based on contacts?


        frame--;
    }

    clear_collision_list(collisions);
}

void multi_threaded_loop(binary_tree* scene)
{
    //create multiple collision lists


}

#define NO_OF_VOLUMES 5
collision_volume* my_volumes[NO_OF_VOLUMES];
void load_scene();
void unload_scene();

int main()
{
    //create your collision volumes
    load_scene();

    //build BVH
    binary_tree* bvh = NULL;
    uint32_t i = 0;
    for(i = 0; i < NO_OF_VOLUMES; i++)
        bvh = bvh_insert(bvh, binary_tree_new(bvh_data_from_volume(my_volumes[i])));

    //run loop
    simple_loop(bvh);

    custom_combo_loop(bvh);

    multi_threaded_loop(bvh);

    //cleanup
    binary_tree_delete(bvh);
    unload_scene();
}
//----------------------------------------------------


vec3* get_random_vec()
{
    vec3* result = (vec3*)calloc(1, sizeof(vec3));

    *result = (vec3){0,0,0}; //TODO switch to random

    return result;
}

void load_scene()
{
    uint32_t i = 0;
    for(; i < NO_OF_VOLUMES; i++)
    {
        vec3* rand_pos = get_random_vec();
        rotor3* new_rotor = (rotor3*)calloc(1, sizeof(rotor3));
        *new_rotor = IDENTITY_ROTOR;
        uint8_t shape = SPHERE; //TODO switch to random

        my_volumes[i] = create_primitive_collision_volume(shape, (vec3){1,1,1}, rand_pos, new_rotor);
    }
}

void unload_scene()
{
    uint32_t i = 0;
    for(; i < NO_OF_VOLUMES; i++)
    {
        free(my_volumes[i]->position);
        free(my_volumes[i]->orientation);
        free_collision_volume(my_volumes[i]);
    }
}