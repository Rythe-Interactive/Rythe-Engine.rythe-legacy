using System;
using Legion.Attributes;

namespace Legion
{
    [GameSystem]
    class StubModule : GameSystemBase
    {
        public override void Init() => Log.Info("StubModule has been initialized!");


        private int i = 0;
        public override void Update(float dt)
        {
            if (i > 100)
            { 
                Log.Info("StubModule has been updated 100 times");
                Log.Info($"Delta Time of this frame: {dt}");
                i = 0;
            }

            i++;
        }

    }
}
