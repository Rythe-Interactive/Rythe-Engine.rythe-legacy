__kernel void vector_add(__global const float3* A, __global const float3* B,__global float3* C) {

	int i = get_global_id(0);

	// C[i] = (float3)(A[i] + B[i]);
    C[i] = (float3)(A[i].x,A[i].y,A[i].z);
}

__kernel void init_vel(__global float4* A, const unsigned long start, const unsigned long count) {

	int i = get_global_id(0);

    if(i > count)
        return;

    A[start+i] = (float4)(0.0,0.0,0.0,0.0);
}

__kernel void init_pos(__global float4* A, const unsigned long start, const unsigned long count) {

	int i = get_global_id(0);

    if(i > count)
        return;

    A[start+i] = (float4)(1.0,0.0,0.0,0.0);
}
