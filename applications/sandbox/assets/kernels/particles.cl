//Initialization Functions
kernel void init_pos(global float4* positions, const float4 position, const int start, const int end)
{
	int i = get_global_id(0);
    int idx = start+i;
    if(idx > start+end)
        return;

    positions[idx] = position;
}

kernel void init_rot(global float4* rotations, const float4 direction, const int start, const int end) 
{
	int i = get_global_id(0);
    int idx = start+i;
    if(idx > start+end)
        return;
        
    rotations[idx] = direction;
}

kernel void init_scale(global float4* scales, const float4 scale, const int start, const int end)
{
	int i = get_global_id(0);
        int idx = start+i;
    if(idx > start+end)
        return;
        
    scales[idx] = scale;
}

kernel void init_vel(global float4* velocity, global const float3* directions) {

	int i = get_global_id(0);

    velocity[i] = (float4)(directions[i], 0.0f);
}

kernel void init_velField(global float4* positions, global float4* vectorField, const float4 center)
{
    int i = get_global_id(0);

    float4 pos = positions[i];
    float4 r = (float4)(center-pos);
    float4 w = (float4)(0.f,5.f,0.f,0.f);
    vectorField[i] = r + cross(r,w);
}

kernel void init_pos_disk(global float4* positions, const int radius)
{
	int i = get_global_id(0);

}

kernel void init_pos_cube(global float4* positions, const int width, const int height, const int depth)
{
	int i = get_global_id(0);

    float4 midpoint = (float4)(width/2.f, height/2.f, depth/2.f, 0.0f);
    float4 pos = (float4)(i%width,(i/width)%height,(i/(width*height))%depth,0.0f);
    positions[i] = pos-midpoint;
}


//Position integration functions
kernel void euler_integration(global uint* living, global float4* positions, global float4* velocities, const float dt)
{
	int i = get_global_id(0);
    if(living[i] == 0)
        return;

	positions[i] += velocities[i]*dt;
}

kernel void verlet_integration(global uint* living, global float4* positions, global float4* prevPositions, const float4 acceleration, const float dt)
{
    int i = get_global_id(0);
    if(living[i] == 0)
        return;
        
    float4 velocity = positions[i] - prevPositions[i];
    prevPositions[i] = positions[i];
    positions[i] += velocity + acceleration * dt * dt;
}



