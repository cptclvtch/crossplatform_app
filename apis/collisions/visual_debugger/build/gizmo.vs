#version 310 es
#define NEAR_CLIP 0.05
#define FAR_CLIP 10000.0

layout (location = 0) in vec3 v_pos;

uniform vec3 offset;
uniform vec4 rotor;
uniform vec3 scale;
uniform vec3 cam_pos;
uniform vec4 cam_rot;
uniform vec3 view_size;

void rotate_w_rotor(inout vec3 a, vec4 r)
{
    vec3 a_II = a * r.w;
    vec3 a_T  = cross(a, r.xyz);
    vec3 v = a_II + a_T;

    float trivec = dot(r.xyz, a);

    a.x =  v.x*r.w + v.y*r.z - v.z*r.y + trivec*r.x;
    a.y = -v.x*r.z + v.y*r.w + v.z*r.x + trivec*r.y;
    a.z =  v.x*r.y - v.y*r.x + v.z*r.w + trivec*r.z;
}

void main()
{
    vec3 v = v_pos;
    v *= scale;
    rotate_w_rotor(v, rotor);
    v += offset;
    v -= cam_pos;

    vec4 c_rot = cam_rot; c_rot.xyz *= -1.0;
    rotate_w_rotor(v, c_rot);

    gl_Position.xyzw = v.yzxx;

    //ndc stretch
    gl_Position.xy *= view_size.z;
    gl_Position.x *= -1.0*(view_size.y/view_size.x);
    gl_Position.z = (gl_Position.z - NEAR_CLIP)/FAR_CLIP*gl_Position.z;
}
