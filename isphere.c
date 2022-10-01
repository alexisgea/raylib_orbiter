#include "raylib.h"
#include "math.h"

Vector3 GetNormalizedVector3(float x, float y, float z);
void CreateIcosphereVertices();


// void Vector3Normalize(Vector3 v) {
//     float length = sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);
//     if (length == 0.0f) length = 1.0f;
//     float ilength = 1.0f/length;
//     v.x *= ilength;
//     v.y *= ilength;
//     v.z *= ilength;
// }



Mesh GenIcosphereMesh(int subdivision) {

    Mesh mesh = { 0 };
    if(subdivision > 5) TRACELOG(LOG_WARNING, "MESH: icosphere subdivision too high, max 5.");
    else {

        int startVerticesCount = 12;
        int totalVerticesCount = startVerticesCount; // todo calculate final amount
        int startTrianglesCount = 20;
        int totalTrianglesCount = startTrianglesCount; // todo calculate final amount

        mesh.vertices = (float *)RL_MALLOC(totalVerticesCount*3*sizeof(float));
        mesh.texcoords = (float *)RL_MALLOC(totalVerticesCount*2*sizeof(float));
        mesh.normals = (float *)RL_MALLOC(totalVerticesCount*3*sizeof(float));

        mesh.vertexCount = totalVerticesCount*3;
        mesh.triangleCount = totalVerticesCount;

        CreateStartingVertices(&mesh.vertices, totalVerticesCount);


        for (int k = 0; k < mesh.vertexCount; k++)
        {
            // mesh.vertices[k*3] = sphere->points[sphere->triangles[k]*3];
            // mesh.vertices[k*3 + 1] = sphere->points[sphere->triangles[k]*3 + 1];
            // mesh.vertices[k*3 + 2] = sphere->points[sphere->triangles[k]*3 + 2];

            // mesh.normals[k*3] = sphere->normals[sphere->triangles[k]*3];
            // mesh.normals[k*3 + 1] = sphere->normals[sphere->triangles[k]*3 + 1];
            // mesh.normals[k*3 + 2] = sphere->normals[sphere->triangles[k]*3 + 2];

            // mesh.texcoords[k*2] = sphere->tcoords[sphere->triangles[k]*2];
            // mesh.texcoords[k*2 + 1] = sphere->tcoords[sphere->triangles[k]*2 + 1];
        }

    }

    return mesh;
}



Vector3 GetNormalizedVector3(float x, float y, float z) {
    float length = sqrtf(x*x + y*y + z*z);
    if (length == 0.0f) length = 1.0f;
    float ilength = 1.0f/length;
    x *= ilength;
    y *= ilength;
    z *= ilength;
    return (Vector3){x, y, z};
}

void SetVertice(int vertId, Vector3 vert, float *vertices, int length) {
    if(length < vertId*3+3) TRACELOG(LOG_ERROR, "MESH: icosphere vertices array too small.");

    vertices[vertId*3] = vert.x;
    vertices[vertId*3 + 1] = vert.y;
    vertices[vertId*3 + 2] = vert.z;
}

// create the 12 starting vertices of a icosahedron
void CreateStartingVertices(float *vertices, int length) {
    if(length < 12) TRACELOG(LOG_ERROR, "MESH: icosphere vertices array too small.");

    const int count = 12;
    Vector3 verts[count] = {0};
    
    const float t = (1.0f + Sqrtf(5.0f)) / 2.0f;

    verts[0] = GetNormalizedVector3(-1.0f,  t,  0.0f);
    verts[1] = GetNormalizedVector3(1.0f,  t,  0.0f);
    verts[2] = GetNormalizedVector3(-1.0f,  -t,  0.0f);
    verts[3] = GetNormalizedVector3(1.0f,  -t,  0.0f);

    verts[4] = GetNormalizedVector3(0.0f,  -1.0f,  t);
    verts[5] = GetNormalizedVector3(0.0f,  1.0f,  t);
    verts[6] = GetNormalizedVector3(0.0f,  -1.0f,  -t);
    verts[7] = GetNormalizedVector3(0.0f,  1.0f,  -t);

    verts[8] = GetNormalizedVector3(t,  0.0f,  -1.0f);
    verts[9] = GetNormalizedVector3(t,  0.0f,  1.0f);
    verts[10] = GetNormalizedVector3(-t,  0.0f,  -1.0f);
    verts[11] = GetNormalizedVector3(-t,  0.0f,  1.0f);

    for(int i = 0; i < count; i++) {
        SetVertice(i, verts[i], &vertices, length);
    }
}

void CreateIcosphereTriangles() {

}

// // create the 20 staring triangle faces of the icosahedron
// public static  List<int> CreateIcosphereFaces() {
//     List<int> startingFaces = new List<int>();

//     // 5 faces around point 0
//     AddTriangle(startingFaces,0, 11, 5);
//     AddTriangle(startingFaces,0, 5, 1);
//     AddTriangle(startingFaces,0, 1, 7);
//     AddTriangle(startingFaces,0, 7, 10);
//     AddTriangle(startingFaces,0, 10, 11);

//     // 5 adjacent faces 
//     AddTriangle(startingFaces,1, 5, 9);
//     AddTriangle(startingFaces,5, 11, 4);
//     AddTriangle(startingFaces,11, 10, 2);
//     AddTriangle(startingFaces,10, 7, 6);
//     AddTriangle(startingFaces,7, 1, 8);

//     // 5 faces around point 3
//     AddTriangle(startingFaces,3, 9, 4);
//     AddTriangle(startingFaces,3, 4, 2);
//     AddTriangle(startingFaces,3, 2, 6);
//     AddTriangle(startingFaces,3, 6, 8);
//     AddTriangle(startingFaces,3, 8, 9);

//     // 5 adjacent faces 
//     AddTriangle(startingFaces,4, 9, 5);
//     AddTriangle(startingFaces,2, 4, 11);
//     AddTriangle(startingFaces,6, 2, 10);
//     AddTriangle(startingFaces,8, 6, 7);
//     AddTriangle(startingFaces,9, 8, 1);

//     return startingFaces;
// }