#version 150

in vec4 var_position;
in vec3 var_normal_in_world;
in vec3 var_normal_in_cam;
in vec2 var_texture_coord;
in vec3 var_color;

out vec4 out_color;

uniform sampler2D texture;
uniform int renderType;
uniform float objectId;

vec3 float2color(float val)
{
    vec3 c = vec3(1.0,1.0,1.0);

    if(val < 0.25)
    {
        c.r = 0;
        c.g = 4 * val;
    }
    else if(val < 0.5)
    {
        c.r = 0;
        c.b = 1 + 4*(0.25-val);
    }
    else if(val < 0.75)
    {
        c.r = 4*(val-0.5);
        c.b = 0;
    }
    else
    {
        c.g = 1 + 4*(0.75 - val);
        c.b = 0;
    }
    return c;
}

void main()
{
    vec3 light_dir = normalize(vec3(0, 0, 1));
    float cos_term = max(dot(light_dir, var_normal_in_cam), 0.3);

    // normal color
    if(renderType == 0)
        out_color = vec4((var_normal_in_cam+1)/2., 1)*cos_term;

    // texture render
    if(renderType == 1)
        out_color = texture2D(texture, var_texture_coord)*cos_term;

    // color render
    if(renderType == 2)
        out_color = vec4(var_color.xyz, 1)*cos_term;

    // id render
    if(renderType == 3)
        out_color = vec4(float2color(objectId), 1);
}
