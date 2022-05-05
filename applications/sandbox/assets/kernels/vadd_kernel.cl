__kernel void vector_add(__global const float3* A, __global const float3* B,__global float3* C) {

	int i = get_global_id(0);

	C[i] = A[i] + B[i];
}
