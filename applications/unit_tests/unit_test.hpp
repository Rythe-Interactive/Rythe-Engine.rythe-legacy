#pragma once
#include <core/core.hpp>
#include "doctest.h"

namespace legion
{
    struct test_info
    {
        static bool isBenchMarking;
    };

    inline bool test_info::isBenchMarking;

    template <class T>
    inline L_ALWAYS_INLINE void DoNotOptimize(const T& value)
    {
        asm volatile("" : : "r,m"(value) : "memory");
    }

    template <class T>
    inline L_ALWAYS_INLINE void DoNotOptimize(T& value)
    {
#if defined(LEGION_CLANG)
        asm volatile("" : "+r,m"(value) : : "memory");
#else
        asm volatile("" : "+m,r"(value) : : "memory");
#endif
    }

    using CheckFunc = void(*)(const doctest::detail::Result&, doctest::assertType::Enum, cstring, int, const char*);
    using SubDomainFunc = bool(*)(cstring);

    struct test_data
    {
        static std::string* lastDomain;
        static cstring lastSubDomain;
        static std::vector<cstring> subdomainnames;
        static std::unordered_map<cstring, std::pair<size_type, long double>> subdomaintimes;

        static std::unordered_map<std::string, std::string>& GetTestDomains()
        {
            static std::unordered_map<std::string, std::string> domains;
            return domains;
        }

        static CheckFunc Check;
        static SubDomainFunc StartSubDomain;
    };

    inline L_ALWAYS_INLINE int SetTestDomain(const std::string& func, cstring domain)
    {
        test_data::GetTestDomains()[func] = std::string(domain);
        return 0;
    }

    struct SubDomainStack
    {
        static inline size_type items = 0;
        long double* time;
        time::stopwatch<long double> clck;

        SubDomainStack(bool benchmark) : time(nullptr)
        {
            items++;
            if (benchmark)
            {
                time = &(test_data::subdomaintimes[test_data::lastSubDomain].second);
                clck.start();
            }
        }

        ~SubDomainStack()
        {
            if (time)
            {
                *time += clck.end().milliseconds();
            }
            items--;
        }

        constexpr operator bool() const noexcept { return true; }
    };

    inline L_ALWAYS_INLINE void NoOpt(const doctest::detail::Result& value, doctest::assertType::Enum assrt, L_MAYBEUNUSED cstring file, L_MAYBEUNUSED int line, L_MAYBEUNUSED const char* expr)
    {
        if (!value.m_passed)
            DoNotOptimize(assrt);

        DoNotOptimize(value);
    }

    inline L_ALWAYS_INLINE void DoCheck(const doctest::detail::Result& value, doctest::assertType::Enum assrt, cstring file, int line, const char* expr)
    {
        doctest::detail::ResultBuilder _DOCTEST_RB(assrt, file, line, expr);
        _DOCTEST_RB.setResult(value);
        _DOCTEST_RB.log();
        _DOCTEST_RB.react();
    }

    inline L_ALWAYS_INLINE bool NoPrint(cstring val)
    {
        test_data::lastSubDomain = val;
        if (!test_data::subdomaintimes.count(val))
            test_data::subdomainnames.push_back(val);
        test_data::subdomaintimes.emplace(val, std::make_pair(SubDomainStack::items, static_cast<long double>(0)));
        return true;
    }

    inline L_ALWAYS_INLINE bool DoPrint(cstring val)
    {
        test_data::lastSubDomain = val;
        std::cout << '[' << (test_data::lastDomain ? *test_data::lastDomain : "unknown") << "] ";
        for (size_type i = 0; i <= SubDomainStack::items; i++)
            std::cout << "    ";
        std::cout << val << '\n';
        return false;
    }

    inline L_ALWAYS_INLINE std::string SanitizeFunctionName(cstring func)
    {
        std::string_view fname = func;
        auto end = fname.find_first_of('(');
        fname = fname.substr(0, end);
        auto start = fname.find_last_of(' ');
        return std::string(fname.substr(start + 1));
    }

    template<typename Callable, typename... Args>
    inline L_ALWAYS_INLINE void Benchmark_IMPL(const std::string& func, size_type n, Callable c, Args&&... args)
    {
        if (this_engine::cliargs()["nobenchmark"])
            return;

        test_data::subdomaintimes.clear();
        test_data::subdomainnames.clear();
        test_data::lastDomain = &test_data::GetTestDomains()[func];
        std::cout << '[' << *test_data::lastDomain << "] Running benchmark: " << n << " cases\n";
        test_data::Check = &NoOpt;
        test_data::StartSubDomain = &NoPrint;
        test_info::isBenchMarking = true;
        time::stopwatch<long double> clck;
        clck.start();
        for (size_type i = 0; i < n; i++)
        {
            std::invoke(c, std::forward<Args>(args)...);
        }
        auto elapsed = clck.end();

        if (test_data::subdomainnames.size())
        {
            long double total = 0;
            for (auto& name : test_data::subdomainnames)
            {
                auto& [indent, time] = test_data::subdomaintimes[name];
                std::cout << '[' << *test_data::lastDomain << "] ";
                for (size_type i = 0; i <= indent; i++)
                    std::cout << "    ";
                std::cout << name << ": " << time << "ms total " << time / static_cast<long double>(n) << "ms on average\n";
                total += time;
            }
            test_data::subdomaintimes.clear();

            std::cout << '[' << *test_data::lastDomain << "]     " << total / static_cast<long double>(n) << "ms on average\n";
            std::cout << '[' << *test_data::lastDomain << "]     " << total << "ms total\n";
        }
        else
        {
            std::cout << '[' << *test_data::lastDomain << "]     " << elapsed.milliseconds() / static_cast<long double>(n) << "ms on average\n";
            std::cout << '[' << *test_data::lastDomain << "]     " << elapsed.milliseconds() << "ms total\n";
        }
    }

    template<typename Callable, typename... Args>
    inline L_ALWAYS_INLINE void Test_IMPL(const std::string& func, Callable c, Args&&... args)
    {
        test_data::lastDomain = &test_data::GetTestDomains()[func];
        std::cout << '[' << *test_data::lastDomain << "] Running test\n";
        test_data::Check = &DoCheck;
        test_data::StartSubDomain = &DoPrint;
        test_info::isBenchMarking = false;
        std::cout << '[' << *test_data::lastDomain << "] Iteration 1\n";
        std::invoke(c, std::forward<Args>(args)...);
        std::cout << '[' << *test_data::lastDomain << "] Iteration 2\n";
        std::invoke(c, std::forward<Args>(args)...);
    }

#define WRAP_IN_TRY(x)                                                                              \
    try {                                                                                           \
        x;                                                                                          \
    } catch(...) { legion::log::error("{}", doctest::translateActiveException().c_str()); }

#define L_CHECK_IMPL(assert_type, ...)                                                                                                                              \
    do {                                                                                                                                                            \
        byte _DATA_BUFF[sizeof(doctest::detail::Result)];                                                                                                           \
        doctest::detail::Result* _DOCTEST_RES = reinterpret_cast<doctest::detail::Result*>(&_DATA_BUFF[0]);                                                         \
        if(!test_info::isBenchMarking)                                                                                                                              \
        {                                                                                                                                                           \
            DOCTEST_CLANG_SUPPRESS_WARNING_WITH_PUSH("-Woverloaded-shift-op-parentheses")                                                                           \
            WRAP_IN_TRY(new(_DOCTEST_RES) doctest::detail::Result(doctest::detail::ExpressionDecomposer(doctest::assertType::assert_type) << __VA_ARGS__))          \
            DOCTEST_CLANG_SUPPRESS_WARNING_POP                                                                                                                      \
        }                                                                                                                                                           \
        test_data::Check(*_DOCTEST_RES, doctest::assertType::assert_type, __FILE__, __LINE__, #__VA_ARGS__);                                                        \
    } while(false)

#define L_CHECK(...) L_CHECK_IMPL(DT_CHECK, __VA_ARGS__)

#define L_REQUIRE(...) L_CHECK_IMPL(DT_REQUIRE, __VA_ARGS__)

#define Benchmark_N(n, ...) legion::Benchmark_IMPL(legion::SanitizeFunctionName(__FULL_FUNC__), n, __VA_ARGS__)
#define Benchmark(...) legion::Benchmark_IMPL(legion::SanitizeFunctionName(__FULL_FUNC__), 10000, __VA_ARGS__)

#define Test(...) legion::Test_IMPL(legion::SanitizeFunctionName(__FULL_FUNC__), __VA_ARGS__)

#define LEGION_FUNCNAME_IMPL(f) #f

#define LEGION_REGISTER_TEST(f, domain)                                                                             \
            DOCTEST_GLOBAL_NO_WARNINGS(DOCTEST_ANONYMOUS(_DOCTEST_ANON_VAR_)) =                                     \
            legion::SetTestDomain(LEGION_FUNCNAME_IMPL(f), domain);                                                 \
            DOCTEST_GLOBAL_NO_WARNINGS_END()                                                                        \
            static void f();                                                                                        \
            DOCTEST_REGISTER_FUNCTION(DOCTEST_EMPTY, f, domain)                                                     \
            static void f()

#define LEGION_TEST(domain) LEGION_REGISTER_TEST(DOCTEST_ANONYMOUS(_DOCTEST_ANON_FUNC_), domain)

#define LEGION_SUBTEST(subdomain) if(const SubDomainStack& DOCTEST_ANONYMOUS(_DOCTEST_ANON_SUBCASE_) DOCTEST_UNUSED = test_data::StartSubDomain(subdomain))

}
