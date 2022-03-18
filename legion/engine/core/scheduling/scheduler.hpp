#pragma once
#include <thread>
#include <unordered_map>

#include <core/engine/enginesubsystem.hpp>
#include <core/engine/engine.hpp>
#include <core/async/async.hpp>
#include <core/containers/pointer.hpp>
#include <core/containers/sparse_map.hpp>
#include <core/types/types.hpp>

#include <core/scheduling/process.hpp>
#include <core/scheduling/processchain.hpp>
#include <core/scheduling/clock.hpp>

namespace legion::core::scheduling
{
    const inline static std::thread::id invalid_thread_id = std::thread::id();

    class Scheduler : public EngineSubSystem<Scheduler>
    {
        AllowPrivateOnInit;
        AllowPrivateOnShutdown;
        SubSystemInstance(Scheduler);
    public:
        using chain_callback_type = typename ProcessChain::chain_callback_type;
        using chain_callback_delegate = typename ProcessChain::chain_callback_delegate;
        using frame_callback_type = chain_callback_type;
        using frame_callback_delegate = chain_callback_delegate;
        using thread_callback_type = void();
        using thread_callback_delegate = delegate<thread_callback_type>;

    private:
        static constexpr size_type reserved_threads = 1; // this, OS

        template<typename resource>
        using per_thread_map = std::unordered_map<std::thread::id, resource>;

        sparse_map<id_type, ProcessChain> m_processChains;

        multicast_delegate<frame_callback_type> m_onFrameStart;
        multicast_delegate<frame_callback_type> m_onFrameEnd;

        const size_type m_maxThreadCount = reserved_threads >= std::thread::hardware_concurrency() ? 0 : std::thread::hardware_concurrency() - reserved_threads;
        size_type m_availableThreads = m_maxThreadCount;

        per_thread_map<std::thread> m_threads;

        async::rw_lock_pair<async::job_queue> m_jobs;
        size_type m_jobPoolSize = 0;

        std::atomic<bool> m_exit = { false };
        std::atomic<bool> m_exitFromEvent = { false };
        std::atomic<bool> m_start = { false };
        int m_exitCode = 0;

        std::atomic<float> m_pollTime = { 0.1f };

        thread_local static Engine* m_currentEngineInstance;

        static multicast_delegate<thread_callback_type>& getOnThreadCreate();

        static void onInit();
        static void onShutdown();

        static void threadMain(Engine& engine, bool lowPower, std::string name);

        template<typename Function, typename... Args >
        L_NODISCARD static pointer<std::thread> createThread(Function&& function, Args&&... args);

        static void tryCompleteJobPool();

        static void doTick(Clock::span_type deltaTime);

    public:
        static void hookThreadToEngine(Engine& context);

        static pointer<Engine> currentEngineInstance();

        template<typename functor, typename... argument_types>
        L_NODISCARD static pointer<std::thread> reserveThread(functor&& function, argument_types&&... args);

        L_NODISCARD static pointer<std::thread> getThread(std::thread::id id);

        static size_type jobPoolSize() noexcept;

        template<typename Func>
        static auto queueJobs(size_type count, Func&& func);

        static int run(Engine& engine, bool lowPower = false, size_type minThreads = 0);

        static void exit(int exitCode);

        static bool isExiting();

        /**@brief Create a new process-chain.
         */
        template<size_type charc>
        static pointer<ProcessChain> createProcessChain(const char(&name)[charc]);

        /**@brief Create a new process-chain.
         */
        static pointer<ProcessChain> createProcessChain(cstring name);

        /**@brief Get pointer to a certain process-chain.
         */
        template<size_type charc>
        L_NODISCARD static pointer<ProcessChain> getChain(const char(&name)[charc]);

        /**@brief Get pointer to a certain process-chain.
         */
        L_NODISCARD static pointer<ProcessChain> getChain(id_type id);

        /**@brief Get pointer to a certain process-chain.
         */
        L_NODISCARD static pointer<ProcessChain> getChain(cstring name);

        static void subscribeToThreadCreate(const thread_callback_delegate& callback);

        static void subscribeToFrameStart(const frame_callback_delegate& callback);
        static void unsubscribeFromFrameStart(const frame_callback_delegate& callback);

        static void subscribeToFrameEnd(const frame_callback_delegate& callback);
        static void unsubscribeFromFrameEnd(const frame_callback_delegate& callback);

        static void subscribeToChainStart(id_type chainId, const chain_callback_delegate& callback);
        static void subscribeToChainStart(cstring chainName, const chain_callback_delegate& callback);

        static void unsubscribeFromChainStart(id_type chainId, const chain_callback_delegate& callback);
        static void unsubscribeFromChainStart(cstring chainName, const chain_callback_delegate& callback);

        static void subscribeToChainEnd(id_type chainId, const chain_callback_delegate& callback);
        static void subscribeToChainEnd(cstring chainName, const chain_callback_delegate& callback);

        static void unsubscribeFromChainEnd(id_type chainId, const chain_callback_delegate& callback);
        static void unsubscribeFromChainEnd(cstring chainName, const chain_callback_delegate& callback);

        /**@brief Hook a process to a certain chain.
         * @return bool True if succeeded, false if the chain doesn't exist.
         */
        template<size_type charc>
        static bool hookProcess(const char(&processChainName)[charc], Process& process);

        /**@brief Hook a process to a certain chain.
         * @return bool True if succeeded, false if the chain doesn't exist.
         */
        template<size_type charc>
        static bool hookProcess(const char(&processChainName)[charc], pointer<Process> process);

        /**@brief Hook a process to a certain chain.
         * @return bool True if succeeded, false if the chain doesn't exist.
         */
        static bool hookProcess(cstring chainName, Process& process);

        /**@brief Hook a process to a certain chain.
         * @return bool True if succeeded, false if the chain doesn't exist.
         */
        static bool hookProcess(cstring chainName, pointer<Process> process);

        /**@brief Unhook a process from a certain chain.
         * @return bool True if succeeded, false if the chain doesn't exist.
         */
        template<size_type charc>
        static bool unhookProcess(const char(&chainName)[charc], Process& process);

        /**@brief Unhook a process from a certain chain.
         * @return bool True if succeeded, false if the chain doesn't exist.
         */
        template<size_type charc>
        static bool unhookProcess(const char(&chainName)[charc], pointer<Process> process);

        /**@brief Unhook a process from a certain chain.
         * @return bool True if succeeded, false if the chain doesn't exist.
         */
        static bool unhookProcess(cstring chainName, Process& process);

        /**@brief Unhook a process from a certain chain.
         * @return bool True if succeeded, false if the chain doesn't exist.
         */
        static bool unhookProcess(cstring chainName, pointer<Process> process);

        /**@brief Unhook a process from a certain chain.
         * @return bool True if succeeded, false if the chain doesn't exist.
         */
        static bool unhookProcess(id_type chainId, pointer<Process> process);
    };

    ReportSubSystem(Scheduler);
}

#include <core/scheduling/scheduler.inl>
