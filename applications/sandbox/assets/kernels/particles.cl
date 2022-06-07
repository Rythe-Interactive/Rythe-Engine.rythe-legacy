ulong gridIdx(const float3 pos, const float3 bounds)
{
    return pos.x+(pos.y*bounds.x)+(pos.z*bounds.x*bounds.y);
}

kernel void reset_spatial_grid(global ulong* gridCells)
{
    int i = get_global_id(0);
    gridCells[i] = -1;
}

kernel void spatial_grid(global ulong* gridCells, global const float4* positions, const float3 cellDims, const float4 bounds)
{
    int i = get_global_id(0);
    float3 pos = positions[i].xyz;
    ulong idx = gridIdx(pos, bounds.xyz);
    if(gridCells[idx] == (ulong)(-1))
        gridCells[idx] = i;
}

kernel void boundary_detection(global ulong* gridCells, global float4* positions, const float4 bounds)
{
    int i = get_global_id(0);
    float3 pos = positions[i].xyz;
}

kernel void collision_detection(global const ulong* gridCells, global float4* positions, const float4 bounds)
{
    int i = get_global_id(0);
    float3 pos = positions[gridCells[i]].xyz;
    ulong idx = gridIdx(pos, bounds.xyz);
   
    // positions[gridCells[idx]].xyz = (float3)(2.f);
    // __attribute__((opencl_unroll_hint(3)))
    for(int x = -1; x >= 1; x++)
    {
        positions[gridCells[idx]].xyz = (float3)(2.f);
        for(int y = -1; y >= 1; y++)
        {
            for(int z = -1; z >= 1; z++)
            {
                ulong n_idx = gridIdx(pos + (float3)(x,y,z), bounds.xyz);
                //positions[gridCells[n_idx]].xyz = (float3)(x,y,z);
                // ulong n_idx = gridIdx(pos + (float3)(x,y,z), bounds);
                // if(n_idx != (ulong)(-1) && n_idx != idx)
                // {
                //     float3 n_pos = positions[n_idx].xyz; 
                //     float3 axis = normalize(pos-n_pos);
                //     //positions[idx].xyz = (float3)(2.f);
                //     positions[idx].xyz += 5.f * axis;
                //     positions[n_idx].xyz -= 5.f * axis;
                // }
            }
        }
    }
        
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