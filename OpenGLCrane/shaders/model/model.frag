#version 330 core

struct Material {

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 highlight;
    float shininess;    
}; 

struct Light {

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 position;
};

in FragData {

    vec3 frag_pos;  
    vec3 frag_norm;
} fs_in;

out vec4 color;  

uniform Material material;
uniform Light light;
uniform vec3 view_pos;

void main()
{
    const vec3 F0 = vec3(0.08); // basic coeficient (0.04 glass, 0.08 metal)

    vec3 ambientColor = material.ambient;
    vec3 diffuseColor = material.diffuse;

    vec3 unit_frag_norm = normalize(fs_in.frag_norm);
    vec3 view_dir = normalize(view_pos - fs_in.frag_pos);


    //
    // ambient light
    //
    vec3 ambient = light.ambient * ambientColor;

    //
    // diffuse light
    //
    vec3 light_dir = normalize(light.position - fs_in.frag_pos);  

    float diffuse_mag = max(dot(unit_frag_norm, light_dir), 0.0);
    vec3 diffuse = light.diffuse * (diffuse_mag * diffuseColor * material.highlight);

    //
    // specular light
    //
    vec3 halfway_dir = normalize(light_dir + view_dir);
    float spec = pow(max(dot(unit_frag_norm, halfway_dir), 0.0), material.shininess);

    //vec3 fresnelFactor = F0 + (1.0 - F0) * pow(1.0 - max(dot(unit_frag_norm, view_dir), 0.0), 5.0);
    //
    // Fresnel-Schlick Approximation
    //
    vec3 fresnelFactor = F0 + (1.0 - F0) * pow(max(1.0 - dot(unit_frag_norm, view_dir), 0.0001), 5.0);

    vec3 specular = mix(spec * material.specular, fresnelFactor, 0.5) * light.specular;


    
    vec3 result = ambient + diffuse + specular;


    color = vec4(result, 1.0);
}
