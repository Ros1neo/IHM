#version 150

uniform mat4 mat_view;
uniform mat4 mat_proj;
uniform mat4 mat_obj;

in vec3 vtx_position;
in vec3 vtx_normal;
in vec2 vtx_texture_coord;
in vec3 vtx_color;

out vec4 var_position;
out vec3 var_normal_in_world;
out vec3 var_normal_in_cam;
out vec2 var_texture_coord;
out vec3 var_color;

void main()
{
    mat4 normalMatrix   = transpose(inverse(mat_view *  mat_obj));
    var_position        = mat_obj * vec4(vtx_position,1);
    var_normal_in_world = normalize(vtx_normal);
    var_normal_in_cam   = normalize((normalMatrix * vec4(var_normal_in_world,1)).xyz);
    var_texture_coord   = vtx_texture_coord;
    var_color           = vtx_color;
    gl_Position         = mat_proj * mat_view *  mat_obj * vec4(vtx_position,1);
}
