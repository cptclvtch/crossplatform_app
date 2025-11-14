#include <stdio.h>

#define INCLUDE_IMPLEMENTATION
#include "../api.c"

#ifndef FRAME_COUNT
#define FRAME_COUNT 20
#endif
void simple_loop(bvh_bt_node* scene)
{
    collision_list* collisions = create_collision_list();

    uint32_t frame = FRAME_COUNT;
    while(frame)
    {
        //perform collision calculations
        gather_contacts(scene, collisions, NULL);

        //move randomly


        frame--;
    }

    clear_collision_list(collisions);
    
    printf("Simple loop finished.\n");
}

void custom_collision_combos(bvh_bt_node* scene, collision_list* list)
{
    bvh_bt_cross_traversal(scene->child[0], scene->child[1], _self_collision_test, _self_collision_func, list);
    //lets say one side is static volumes, no need to check between static objects, skip it
    bvh_bt_sibling_traversal(scene->child[1], _self_collision_test, _self_collision_func, list);
}

void custom_combo_loop(bvh_bt_node* scene)
{
    collision_list* collisions = create_collision_list();

    uint32_t frame = FRAME_COUNT;
    while(frame)
    {
        //perform collision calculations
        gather_contacts(scene, collisions, custom_collision_combos);

        //update based on contacts?


        frame--;
    }

    clear_collision_list(collisions);

    printf("Combo loop finished.\n");
}

void multi_threaded_loop(bvh_bt_node* scene)
{
    //create multiple collision lists


}

#define NO_OF_VOLUMES 5
collision_volume* my_volumes[NO_OF_VOLUMES];

bvh_bt_node* scene = NULL;

vec3* get_random_vec();
void load_scene()
{
    uint32_t i = 0;
    for(; i < NO_OF_VOLUMES; i++)
    {
        vec3* rand_pos = get_random_vec();
        uint8_t shape = SPHERE;

        rotor3* new_rotor = (rotor3*)calloc(1, sizeof(rotor3));
        *new_rotor = IDENTITY_ROTOR;

        my_volumes[i] = create_primitive_collision_volume(shape, (vec3){1,1,1}, rand_pos, new_rotor);
        
        scene = bvh_insert(scene, create_bvh_bt_node(bvh_data_from_volume(my_volumes[i])));
    }
}

void unload_scene();

int main()
{
    //create your collision volumes
    load_scene();

    //run loop
    simple_loop(scene);

    custom_combo_loop(scene);

    // multi_threaded_loop(bvh);

    //cleanup
    free_bvh_bt_node(scene);
    unload_scene();
}
//----------------------------------------------------

int8_t seed = 0;
vec3* get_random_vec()
{
    vec3* result = (vec3*)calloc(1, sizeof(vec3));

    #define rand fl2real(seed = seed*17 + 111)/13.0
    *result = (vec3){rand,rand,rand};
    #undef rand

    return result;
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