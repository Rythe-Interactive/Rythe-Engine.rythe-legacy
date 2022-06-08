kernel void init_vel(global float4* velocitys, global const float4* initForce, const int start, const int end)
{
	int i = get_global_id(0);
    int idx = start+i;
    if(idx > end)
        return;

    velocitys[idx].xyz = initForce[i].xyz;
}

kernel void grav_update(global const uint* living, global float4* positions, global float4* velocitys, const float4 gravity, const float dt)
{
	int i = get_global_id(0);
    if(living[i] == 0)
        return;

    velocitys[i].xyz += gravity.xyz*dt;
    positions[i].xyz += velocitys[i].xyz*dt;
}