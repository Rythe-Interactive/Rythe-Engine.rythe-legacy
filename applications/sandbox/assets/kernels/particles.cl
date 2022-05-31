
__kernel void vector_add(__global const float4* A, __global const float4* B,__global float4* C, const float dt) {

	int i = get_global_id(0);

	C[i] = A[i] + B[i]*dt;
}

__kernel void init_vel(__global float4* A, const unsigned long start, const unsigned long count) {

	int i = get_global_id(0);

    if(i > count)
        return;

    ulong seed = start + i;
    seed = (seed * 0x5DEECE66DL + 0xBL) & ((1L << 48) - 1);
    float x = seed >> 42;
    seed = start + i;
    seed = (seed * 0x5DEECE66DL + 0xBL) & ((1L << 48-i) - 1);
    float y = seed >> 42;
    seed = start + i;
    seed = (seed * 0x5DEECE66DL + 0xBL) & ((1L << 48-i-i) - 1);
    float z = seed >> 42;

    A[start+i] = (float4)(1.0+x,1.0+y,1.0+z,0.0);
}

__kernel void init_pos(__global float4* A, const unsigned long start, const unsigned long count) {

	int i = get_global_id(0);

    if(i > count)
        return;

    A[start+i] = (float4)(1.0,1.0,1.0,0.0);
}

__kernel void init_rot(__global float4* A, const float4 direction,const unsigned long start, const unsigned long count) {

	int i = get_global_id(0);

    if(i > count)
        return;

    A[start+i] = direction;
}

__kernel void init_scale(__global float4* A, const unsigned long start, const unsigned long count) {

	int i = get_global_id(0);

    if(i > count)
        return;

    A[start+i] = (float4)(1.0,1.0,1.0,1.0);
}

__kernel void particle_matrix_composer(__global const uint* living, 
__global const float4* posBuffer, 
__global const float4* rotBuffer, 
__global const float4* scaleBuffer, 
__global float4* rowX, 
__global float4* rowY, 
__global float4* rowZ, 
__global float4* rowW,
const float4 parentX, 
const float4 parentY, 
const float4 parentZ, 
const float4 parentW)
{
    int i = get_global_id(0);
    
    if(living[i] == 0)
        return;

    float qxx = rotBuffer[i].x*rotBuffer[i].x;
    float qyy = rotBuffer[i].y*rotBuffer[i].y;
    float qzz = rotBuffer[i].z*rotBuffer[i].z;

    float qxz = rotBuffer[i].x*rotBuffer[i].z;
    float qxy = rotBuffer[i].x*rotBuffer[i].y;
    float qyz = rotBuffer[i].y*rotBuffer[i].z;

    float qwx = rotBuffer[i].w*rotBuffer[i].x;
    float qwy = rotBuffer[i].w*rotBuffer[i].y;
    float qwz = rotBuffer[i].w*rotBuffer[i].z;

    float3 rotX = (float3)(1.0-2.0*(qyy+qzz),2.0*(qxy+qwz),2.0*(qxz-qwy));
    // rotX.x = 1.0-2.0*(qyy+qzz);
    // rotX.y = 2.0*(qxy+qwz);
    // rotX.z = 2.0*(qxz-qwy);

    float3 rotY = (float3)(2.0*(qxy-qwz),1.0-2.0*(qxx+qzz),2.0*(qyz+qwz));
    // rotY.x = 2.0*(qxy-qwz); 
    // rotY.z = 1.0-2.0*(qxx+qzz);
    // rotY.y = 2.0*(qyz+qwz);

    float3 rotZ = (float3)(2.0*(qxz+qwy),2.0*(qyz-qwx),1.0-2.0*(qxx+qyy));
    // rotZ.x = 2.0*(qxz+qwy);
    // rotZ.y = 2.0*(qyz-qwx);
    // rotZ.z = 1.0-2.0*(qxx+qyy);
    
    float4 localX = (float4)(rotX*scaleBuffer[i].x,0.0);
    float4 localY = (float4)(rotY*scaleBuffer[i].y,0.0);
    float4 localZ = (float4)(rotZ*scaleBuffer[i].z,0.0);
    posBuffer[i].w = 1.0;
    float4 localW = posBuffer[i];

    rowX[i] = localX;
    rowY[i] = localY;
    rowZ[i] = localZ;
    rowW[i] = localW;
}

