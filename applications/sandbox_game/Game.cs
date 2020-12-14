using Legion;
using Legion.Attributes;

[GameSystem]
public class Game
{
    private Entity entity;

    public void Init()
    {

        entity = Entity.CreateEntity();
        Log.Info($"Entity with id {entity.Id}");

        var bytes = new AssetView("assets://bigint.txt").ReadFile();


        Log.Info("Game has been initialized! {0}",bytes.Length);
    }


    private int m_counter = 0;

    public void Update(float dt)
    {

        if (m_counter > 100)
        {
            Log.Info("Game has been updated 100 times");
            Log.Info($"Delta Time of this frame: {dt}");
            m_counter = 0;

           
            EventBus.Emit(new ExitEvent());
        }

        m_counter++;
    }

    [OnEvent]
    public void OnNotifyOne(NotifyOne no)
    {

    }
}

