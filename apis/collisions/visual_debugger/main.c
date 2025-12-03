#include "app_configuration.c"
#include "../../../api.c"

vec3 cam_pos = {-6,3,1};
rotor3 cam_rot = IDENTITY_ROTOR;

float rotation_speed = 0.05;
float movement_speed = 0.05;

enum
{
    O_POS,
    O_ROT,
    O_SCALE,
    CAM_POS,
    CAM_ROT,
    ASPECT_RATIO,
    VIEW_SIZE,
    FOCAL_POINT,
    COLOR,

    TEXTURES
};
shader gizmo_shader = (shader){0};
#include "vector_debug.c"

real debug_paused = fl2real(1);
real anim_speed = fl2real(1.0);
collision_pair p = (collision_pair){0};
collision_volume cv_a = (collision_volume){0};
collision_volume cv_b = (collision_volume){0};
vec3 v_a = (vec3){5,0.5,0};
vec3 v_b = (vec3){-5,-0.5,0};
rotor3 r_a = IDENTITY_ROTOR;
rotor3 r_b = IDENTITY_ROTOR;

void point_camera_at(vec3 point)
{
    vec3 direction = vec_subtract(point, cam_pos);
    
    cam_rot = rotor_from_vectors((vec3){1,0,0}, direction);
    vec3 up = vec_rotate_w_rotor((vec3){0,0,1}, cam_rot);
    rotor3 correction = rotor_from_vectors( vec_cross_product(up, direction),
                                            vec_cross_product((vec3){0,0,1}, direction));
    cam_rot = rotor_combine(cam_rot, correction);
}

void load_scene()
{
    declare_uniform(&gizmo_shader.stages[VERT], O_POS, "offset", G_VEC3);
    declare_uniform(&gizmo_shader.stages[VERT], O_ROT, "rotor", G_VEC4);
    declare_uniform(&gizmo_shader.stages[VERT], O_SCALE, "scale", G_VEC3);
    declare_uniform(&gizmo_shader.stages[VERT], CAM_POS, "cam_pos", G_VEC3);
    declare_uniform(&gizmo_shader.stages[VERT], CAM_ROT, "cam_rot", G_VEC4);
    declare_uniform(&gizmo_shader.stages[VERT], VIEW_SIZE, "view_size", G_VEC3);
    declare_uniform(&gizmo_shader.stages[FRAG], COLOR, "color", G_VEC3);
    load_shader_from_file(&gizmo_shader, "gizmo.vs", "gizmo.fs");

    cv_a.position = &v_a;
    cv_a.orientation = &r_a;
    cv_a.half_size = (vec3){1,1,1};
    cv_b.position = &v_b;
    cv_b.orientation = &r_b;
    cv_b.half_size = (vec3){1,1,1};

    p.members[0] = &cv_a;
    p.members[1] = &cv_b;
}

#include "developer_window.c"

void process_input()
{
    SDL_Event event;
    
    nk_input_begin(dev_ctx);
    while(SDL_PollEvent(&event))
    {
        if(SDL_GetWindowFromID(event.window.windowID) == dev_window)
        nk_sdl_handle_event(&event);

        if(event.type == SDL_QUIT || event.window.event == SDL_WINDOWEVENT_CLOSE)
        {
            running = 0;
            continue;
        }

        if(event.type == SDL_KEYDOWN)
        {
            const char* key = SDL_GetKeyName(event.key.keysym.sym);
            if(strcmp(key, "Q") == 0) running = 0;
            if(strcmp(key, "W") == 0) cam_pos = vec_add(cam_pos, vec_rotate_w_rotor((vec3){ movement_speed,0,0}, cam_rot));
            if(strcmp(key, "S") == 0) cam_pos = vec_add(cam_pos, vec_rotate_w_rotor((vec3){-movement_speed,0,0}, cam_rot));
            
            rotor3 yaw_rotation = rotor_from_scaled_axis_angle((vec3){0,0, rotation_speed});
            if(strcmp(key, "A") == 0) cam_pos = vec_rotate_w_rotor(cam_pos, yaw_rotation);
            if(strcmp(key, "D") == 0) cam_pos = vec_rotate_w_rotor(cam_pos, rotor_reverse(yaw_rotation));
            rotor3 pitch_rotation = rotor_from_scaled_axis_angle(vec_scalar_multiply(vec_normalize(vec_cross_product(cam_pos, (vec3){0,0,1})), rotation_speed));
            vec3 new_pitch_pos = cam_pos;
            if(strcmp(key, "Space") == 0) new_pitch_pos = vec_rotate_w_rotor(cam_pos, rotor_reverse(pitch_rotation));
            if(strcmp(key, "X") == 0)     new_pitch_pos = vec_rotate_w_rotor(cam_pos, pitch_rotation);
            if(m_abs(vec_dot_product(vec_normalize(new_pitch_pos), (vec3){0,0,1})) < fl2real(0.95)) cam_pos = new_pitch_pos;
        }
    }

    nk_input_end(dev_ctx);
}

void update_app()
{
    point_camera_at((vec3){0});

    p.members[0]->position->x += anim_speed*debug_paused*dt/300;
    if(p.members[0]->position->x > 5) p.members[0]->position->x = -5;
    p.members[1]->position->x -= anim_speed*debug_paused*dt/300;
    if(p.members[1]->position->x < -5) p.members[1]->position->x = 5;

    collision_detect(&p);
}

void render_collision_test()
{
    uint8_t i;
    //contacts
    if(p.type != NO_COLLISION)
    for(i = 0; i < p.contact_count; i++)
    {
        draw_vector(p.points[i].normal, p.points[i].point, (vec3){1,0,0});
    }

    vec3 pos_a = *(p.members[1]->position);
    vec3 pos_b = *(p.members[0]->position);
    rotor3 rot_b = *(p.members[0]->orientation);

    vec3 relative = vec_subtract(pos_a, pos_b);
    draw_vector(relative, pos_b, (vec3){0,1,1});
    relative = vec_rotate_w_rotor(relative, rotor_reverse(rot_b));
    
    vec3 closest = vec_max(relative, vec_reverse(p.members[0]->half_size));
    closest = vec_min(closest, p.members[0]->half_size);

    vec3 normal = vec_subtract(closest, relative);
    draw_vector_addition(vec_rotate_w_rotor(closest, rot_b), (vec3){1,0,1}, vec_rotate_w_rotor(normal, rot_b), (vec3){1,1,1}, *(p.members[0]->position));
    
    //meshes
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    for(i = 0; i < 2; i++)
    {
        collision_volume v = *(p.members[i]);
        uint16_t vertex_array = g_mesh[GIZMO_SPHERE].vertex_array;
        uint16_t vertex_count = g_mesh[GIZMO_SPHERE].vertex_count;
        switch(v.type)
        {
            case SPHERE:
            vertex_array = g_mesh[GIZMO_SPHERE].vertex_array;
            vertex_count = g_mesh[GIZMO_SPHERE].vertex_count;
            break;
            case BOX:
            vertex_array = g_mesh[GIZMO_CENTERED_CUBE].vertex_array;
            vertex_count = g_mesh[GIZMO_CENTERED_CUBE].vertex_count;
            break;
        }
        
        glBindVertexArray(vertex_array);
        
        set_uniform(gizmo_shader.stages[VERT].uniforms[O_POS].id, 3f, v.position->x, v.position->y, v.position->z);
        set_uniform(gizmo_shader.stages[VERT].uniforms[O_ROT].id, 4f, v.orientation->T.x, v.orientation->T.y, v.orientation->T.z, v.orientation->II);
        vec3 scale = v.dimensions;
        if(v.type == SPHERE)    scale = (vec3){v.radius, v.radius, v.radius};
        if(v.type == BOX)       scale = vec_scalar_multiply(v.half_size, 2);
        set_uniform(gizmo_shader.stages[VERT].uniforms[O_SCALE].id, 3f, scale.x, scale.y, scale.z);
        
        vec3 color = i == 0? (vec3){1.0,0.1,0.1} : (vec3){0.1,1.0,0.1};
        set_uniform(gizmo_shader.stages[FRAG].uniforms[COLOR].id, 3f, color.x, color.y, color.z);
        glDrawArrays(GL_TRIANGLES, 0, vertex_count);
    }
    glDisable(GL_BLEND);
}

void render_output()
{
    use_shader(&gizmo_shader);

    set_uniform(gizmo_shader.stages[VERT].uniforms[VIEW_SIZE].id, 3f, 16.0, 9.0, 2.0);
    set_uniform(gizmo_shader.stages[VERT].uniforms[CAM_POS].id, 3f, cam_pos.x, cam_pos.y, cam_pos.z);
    set_uniform(gizmo_shader.stages[VERT].uniforms[CAM_ROT].id, 4f, cam_rot.T.x, cam_rot.T.y, cam_rot.T.z, cam_rot.II);

    //Clear
    clear_screen(COLOR_BIT | DEPTH_BIT);

    render_collision_test();

    //UI
    clear_screen(DEPTH_BIT);
    draw_vector(x, (vec3){0}, (vec3){1,0,0});
    draw_vector(y, (vec3){0}, (vec3){0,1,0});
    draw_vector(z, (vec3){0}, (vec3){0,0,1});

    dev_window_loop();
    
    //Display
    SDL_GL_SwapWindow(window);
}

int main()
{
    setup_app();

    load_scene();

    dev_window_setup();
    
    app_loop();
}