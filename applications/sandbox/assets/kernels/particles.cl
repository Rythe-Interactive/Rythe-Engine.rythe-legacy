struct life_time
{
    float age;
    float max;
};

__kernel void emit(__global bool* living/*, __global struct life_time* lifeTime, const float minLife, const float maxLife*/)
{
    int idx = get_global_id(0);

    // struct life_time life;
    // life.age = 0.0f;
    // life.max = lifeTime[idx].max;
    // lifeTime[idx] = life;

    living[idx] = true;
}

__kernel void maintain(__global bool* living, __global struct life_time* lifeTime, const float timeStep)
{
    int idx = get_global_id(0);
    lifeTime[idx].age += timeStep;
    living[idx] = lifeTime[idx].age < lifeTime[idx].max;
}

__kernel void movement(__global const float3* velocity, __global float3* position)
{
    int idx = get_global_id(0);
    float3 newPos = (float3)(position[idx] + velocity[idx] * .02f);
    position[idx] = newPos;
}