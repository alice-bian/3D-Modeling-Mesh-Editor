# 3D Modeling Mesh Editor
A mesh editor for 3D modeling and rigging, much like the modeling capabilities of Blender and Maya.

The mesh is made from three component classes: Vertex, Face, and Half-Edge. Meshes are built by connecting vertices in groups of threes.

Meshes are rigged via joints that use a weighted distribution to control vertices around them, with more control over closer vertices.

I have created three test cases using JSON files to store joint data, which my program is able to build a mesh over. The user can modify a rigged mesh through various GUI actions like moving, rotating, and scaling joints. They can also subdivide a mesh, which I implemented using the Catmull-Clark algorithm. There are also three shaders implemented using GLSL (flat, Lambert, and skeleton) to view the meshes in various forms.

This project is run on Qt Creator.
