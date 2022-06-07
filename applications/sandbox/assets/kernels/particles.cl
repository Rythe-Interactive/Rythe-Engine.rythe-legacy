kernel void spatial_grid(global int* gridCells, global const float4* positions, const float4 cellDims)
{
    int i = get_global_id(0);
    int idx = positions[i].x+(positions[i].y*2)+(positions[i].z*2*2);
    gridCells[idx] = i;
}

kernel void vector_add(global float4* positions, global float4* velocities, const float dt, const float4 center)
{
	int i = get_global_id(0);
	positions[i] += velocities[i]*dt;
    velocities[i] += (center-positions[i])*dt;
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

kernel void constrain_positions(global uint* living, global float4* positions, const float radius)
{
    int i = get_global_id(0);
    if(living[i] == 0)
        return;
    
    float3 pos = positions[i].xyz;
    float dist = length(pos);
    if(dist > radius)
    {
        positions[i] = (float4)(normalize(pos) * radius, 0.0f);
    }
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

    float y = sin((float)(i/3600))*(1+(i%radius));
    float x = cos((float)(i/3600))*(1+(i%radius));
    positions[i] = (float4)(x,0.f,y,0.f);
}

kernel void init_pos_spherical(global float4* positions, global float4* spawnDirs, const int dirCount, const float radius)
{

	int i = get_global_id(0);

    positions[i] = spawnDirs[i%dirCount]*radius;
}

kernel void init_pos_cube(global float4* positions, const int width, const int height, const int depth)
{

	int i = get_global_id(0);
    float4 midpoint = (float4)(width/2.f, height/2.f, depth/2.f, 0.0f);
    float4 pos = (float4)(i%width,(i/width)%height,(i/(width*height))%depth,0.0f);
    positions[i] = pos-midpoint;
}

kernel void init_pos(global float4* positions, const float4 position, const int start, const int end)
{
	int i = get_global_id(0);
    if(start+i > start+end)
        return;

    positions[start+i] = position;
}

kernel void init_rot(global float4* rotations, const float4 direction, const int start, const int end) 
{
	int i = get_global_id(0);
    if(start+i > start+end)
        return;
        
    rotations[i] = direction;
}

kernel void init_scale(global float4* scales, const float4 scale, const int start, const int end)
{
	int i = get_global_id(0);
    if(start+i > start+end)
        return;
        
    scales[i] = scale;
}