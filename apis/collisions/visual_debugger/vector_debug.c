vec3 a = (vec3){fl2real(1.0),fl2real(1.0),fl2real(1.0)};
vec3 x = (vec3){fl2real(1.0),fl2real(0.0),fl2real(0.0)};
vec3 y = (vec3){fl2real(0.0),fl2real(1.0),fl2real(0.0)};
vec3 z = (vec3){fl2real(0.0),fl2real(0.0),fl2real(1.0)};
vec3 neg = (vec3){fl2real(-1.0),fl2real(-1.0),fl2real(-1.0)};
vec3 zero = (vec3){fl2real(0.0),fl2real(0.0),fl2real(0.0)};
vec3 two = (vec3){fl2real(2.0),fl2real(2.0),fl2real(2.0)};

void draw_vector(vec3 v, vec3 offset, vec3 color)
{
    glBindVertexArray(g_mesh[GIZMO_DIAGONAL_LINE].vertex_array);
    set_uniform(gizmo_shader.stages[VERT].uniforms[O_POS].id, 3f, offset.x, offset.y, offset.z);
    set_uniform(gizmo_shader.stages[VERT].uniforms[O_ROT].id, 4f, 0, 0, 0, 1);
    set_uniform(gizmo_shader.stages[VERT].uniforms[O_SCALE].id, 3f, v.x, v.y, v.z);
    set_uniform(gizmo_shader.stages[FRAG].uniforms[COLOR].id, 3f, color.x, color.y, color.z);
    glDrawArrays(GL_LINES, 0,2);
}

vec3 draw_vector_addition(vec3 a, vec3 color_a, vec3 b, vec3 color_b, vec3 offset)
{
    vec3 result = vec_add(a,b);

    draw_vector(a, offset, color_a);
    draw_vector(b, vec_add(offset, a), color_b);

    draw_vector(result, offset, vec_unit_cube_fit(vec_add(color_a, color_b)));

    return result;
}