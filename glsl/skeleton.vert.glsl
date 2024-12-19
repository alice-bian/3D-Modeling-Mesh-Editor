#version 150
// ^ Change this to version 130 if you have compatibility issues

//This is a vertex shader. While it is called a "shader" due to outdated conventions, this file
//is used to apply matrix transformations to the arrays of vertex data passed to it.
//Since this code is run on your GPU, each vertex is transformed simultaneously.
//If it were run on your CPU, each vertex would have to be processed in a FOR loop, one at a time.
//This simultaneous transformation allows your program to run much faster, especially when rendering
//geometry with millions of vertices.

const int NUM_JOINTS_INFLUENCING_VERTEX = 2;

uniform mat4 u_Model;       // The matrix that defines the transformation of the
                            // object we're rendering. In this assignment,
                            // this will be the result of traversing your scene graph.

uniform mat4 u_ModelInvTr;  // The inverse transpose of the model matrix.
                            // This allows us to transform the object's normals properly
                            // if the object has been non-uniformly scaled.

uniform mat4 u_ViewProj;    // The matrix that defines the camera's transformation.
                            // We've written a static matrix for you to use for HW2,
                            // but in HW3 you'll have to generate one yourself

in vec4 vs_Pos;             // The array of vertex positions passed to the shader

in vec4 vs_Nor;             // The array of vertex normals passed to the shader

in vec4 vs_Col;             // The array of vertex colors passed to the shader.

out vec3 fs_Pos;
out vec4 fs_Nor;            // The array of normals that has been transformed by u_ModelInvTr. This is implicitly passed to the fragment shader.
out vec4 fs_Col;            // The color of each vertex. This is implicitly passed to the fragment shader.

// Each Vertex is influenced by two Joints.
// in_weights[0] is the first Joint's influence weight, and
// in_weights[1] is the second Joint's influence weight.
in vec2 in_weights;

// Indicates the IDs of the Joints that influence this Vertex.
// A Joint's ID corresponds to the index in the mat4 arrays below
// that stores that Joint's bind matrix and current transformation matrix.
in ivec2 in_jointIDs;

// All of the bind matrices of the skeleton.
// Each element in the array is one Joint's bind matrix.
uniform mat4 ua_bindMats[100];

// All of the current transformation matrices of the skeleton.
// Each element in the array is one Joint's current transformation matrix.
uniform mat4 ua_jointMats[100];

void main()
{
    fs_Col = vs_Col;                         // Pass the vertex colors to the fragment shader for interpolation

    mat3 invTranspose = mat3(u_ModelInvTr);
    fs_Nor = vec4(invTranspose * vec3(vs_Nor), 0);          // Pass the vertex normals to the fragment shader for interpolation.
                                                            // Transform the geometry's normals by the inverse transpose of the
                                                            // model matrix. This is necessary to ensure the normals remain
                                                            // perpendicular to the surface after the surface is transformed by
                                                            // the model matrix.


    vec4 modelposition = u_Model * vs_Pos;   // Temporarily store the transformed vertex positions for use below
    fs_Pos = modelposition.xyz;

    vec4 pos_sum = vec4(0, 0, 0, 0);
    for (int i = 0; i < NUM_JOINTS_INFLUENCING_VERTEX; i++) {
        mat4 bind_matrix = ua_bindMats[in_jointIDs[i]];
        mat4 overall_transformation_matrix = ua_jointMats[in_jointIDs[i]];
        pos_sum += in_weights[i] * overall_transformation_matrix * bind_matrix * vs_Pos;
    }
    gl_Position = u_ViewProj * u_Model * pos_sum;
}
