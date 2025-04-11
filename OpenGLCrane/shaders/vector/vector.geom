#version 330 core
layout(points) in;
layout(triangle_strip, max_vertices = 128) out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

in VS_OUT {
    vec3  start;
    vec3  direction;
    vec4  color;
    float length;
} gs_in[];

out vec4 fragColor;

vec3 ortho(vec3 v) {
    // ortogonale vector to v
    return normalize(abs(v.x) > abs(v.z) ? vec3(-v.y, v.x, 0.0)
                                         : vec3(0.0, -v.z, v.y));
}

void emitBox(vec3 center, vec3 dir, float length, float width, float height)
{

    vec3 up = ortho(dir);
    vec3 right = normalize(cross(dir, up));

    vec3 halfRight = right * (width * 0.5);
    vec3 halfUp = up * (height * 0.5);
    vec3 halfForward = normalize(dir) * (length * 0.5);

    // 8 cube vertixes
    vec3 corners[8];

    int i = 0;

    for (int dx = -1; dx <= 1; dx += 2)
        for (int dy = -1; dy <= 1; dy += 2)
            for (int dz = -1; dz <= 1; dz += 2)
                corners[i++] = center + dx * halfRight + dy * halfUp + dz * halfForward;

    // face indexes
    const int faces[36] = int[](
        0, 1, 2,  2, 1, 3,  // bottom
        4, 6, 5,  5, 6, 7,  // top
        0, 4, 1,  1, 4, 5,  // front
        2, 3, 6,  6, 3, 7,  // back
        0, 2, 4,  4, 2, 6,  // left
        1, 5, 3,  3, 5, 7   // right
    );

    // triangle emmision
    for (int i = 0; i < 36; i++) {
        gl_Position = projection * view * model * vec4(corners[faces[i]], 1.0);
        fragColor = gs_in[0].color; // vec4(0.7, 0.7, 1.0, 1.0);

        EmitVertex();

        if (i % 3 == 2)
            EndPrimitive();
    }
}

void emitArrowhead(vec3 tip, vec3 dir, float length, float baseSize)
{
    vec3 up = ortho(dir);
    vec3 right = normalize(cross(dir, up));

    vec3 baseCenter = tip - normalize(dir) * length;
    vec3 baseCorners[4];
    baseCorners[0] = baseCenter + up * baseSize + right * baseSize;
    baseCorners[1] = baseCenter + up * baseSize - right * baseSize;
    baseCorners[2] = baseCenter - up * baseSize - right * baseSize;
    baseCorners[3] = baseCenter - up * baseSize + right * baseSize;

    for (int i = 0; i < 4; i++) {

        gl_Position = projection * view * model * vec4(baseCorners[i], 1.0);
        fragColor = vec4(1.0, 0.4, 0.2, 1.0);
        EmitVertex();
        gl_Position = projection * view * model * vec4(tip, 1.0);
        fragColor = vec4(1.0, 0.4, 0.2, 1.0);
        EmitVertex();
        gl_Position = projection * view * model * vec4(baseCorners[(i + 1) % 4], 1.0);
        fragColor = vec4(1.0, 0.4, 0.2, 1.0);
        EmitVertex();
        EndPrimitive();
    }
}

void main()
{


    vec3 start = gs_in[0].start;
    vec3 dir = normalize(gs_in[0].direction);
    float length = gs_in[0].length;

    float bodyLength;
    float headLength;

    if (length > 5.0f)
    {
        bodyLength = length - 5.0f;
        headLength = 5.0f;
    }
    else
    {
        bodyLength = 0.1f;
        headLength = length;
    }

    float thickness = 0.5f;

    // box in the begining 
    emitBox(start, dir, 0.1, thickness, thickness);

    // arrow body
    vec3 bodyCenter = start + dir * (bodyLength * 0.5);
    emitBox(bodyCenter, dir, bodyLength, thickness, thickness);

    // arrow
    vec3 arrowTip = start + dir * length;
    emitArrowhead(arrowTip, dir, headLength, thickness * 1.5f);
}
