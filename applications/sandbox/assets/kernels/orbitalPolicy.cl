kernel void init_pos(global float4* positions, const int start, const int end)
{
	int i = get_global_id(0);
    int idx = start+i;
    if(idx > start+end)
        return;

    float3 baseDir = (float3)(cos(idx%6*45.f),0.f,sin(idx%6*45.f));
    float3 norm = fast_normalize(baseDir);
    positions[idx].xyz = norm * 4 + norm * ((idx%26));
}

kernel void init_vel(global const float4* positions, global float4* velocitys, const float gravForce, const float c_mass, const int start, const int end)
{
	int i = get_global_id(0);
    int idx = start+i;
    if(idx >= start+end)
        return;

    float3 pos = positions[idx].xyz;
    float3 up = (float3)(0.f,1.f,0.f);
    float r = fast_length(pos);
    float speed = sqrt((gravForce*c_mass)/r);
    velocitys[idx].xyz = fast_normalize(cross(pos,up))*speed;
}

kernel void update_orbits(global const uint* living, global float4* positions, global float4* velocitys, const float gravForce, const float c_mass, const float dt)
{
	int i = get_global_id(0);
    if(living[i] == 0)
        return;

    float3 pos = positions[i].xyz;
    float r2 = pow(fast_length(pos),2);
    float force = (gravForce*c_mass)/r2;

    velocitys[i].xyz += -pos*rsqrt(r2)*force*dt;
    positions[i].xyz += velocitys[i].xyz*dt;
} 