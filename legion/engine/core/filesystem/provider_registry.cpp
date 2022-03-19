#include "provider_registry.hpp"
#include <algorithm>
#include <core/containers/iterator_tricks.hpp>
#include <core/platform/platform.hpp>
#include <core/logging/logging.hpp>
#include <core/filesystem/basic_resolver.hpp>

namespace legion::core::filesystem
{
    struct provider_registry::driver {
        std::unordered_multimap<domain,std::unique_ptr<resolver>>* m_domain_resolver_map;
        driver()
        {
            //pointer to pointer to implementation ?
            m_domain_resolver_map = new std::unordered_multimap<domain, std::unique_ptr<resolver>>;
#if defined(LEGION_WINDOWS)
            LPSTR buffer = new char[512];
            auto charCount = GetLogicalDriveStringsA(512,buffer);
            if (!charCount)
            {
                log::error(GetLastError());
            }
            else
            {
                std::string driveName;
                for (DWORD i = 0; i < charCount; i++)
                {
                    if (buffer[i] == '\0')
                    {
                        driveName += '\\';
                        m_domain_resolver_map->emplace(driveName, std::make_unique<basic_resolver>(driveName));
                        driveName.clear();
                    }
                    else
                    {
                        driveName += buffer[i];
                    }
                }
            }
            delete[] buffer;
#elif defined(LEGION_LINUX)
#endif
        }

        ~driver()
        {
            delete m_domain_resolver_map;
        }
    };

    provider_registry::driver& provider_registry::get_driver()
    {
        static provider_registry::driver registry;
       
        return registry;
    }

    std::unordered_set<provider_registry::domain> provider_registry::domains()
    {
        //get map driver
        static auto& driver = get_driver();

        std::unordered_set<domain> _domains;

        for(auto& key : keys_only(*driver.m_domain_resolver_map))
        {
            //unordered_sets are unique by default no need to worry about duplicates
            _domains.insert(key);   
        }
        return _domains;
    }

    bool provider_registry::has_domain(domain d)
    {
        //get map driver
        static auto& driver = get_driver();

        return driver.m_domain_resolver_map->find(d) != driver.m_domain_resolver_map->end();

        
    }

    void provider_registry::domain_add_resolver(domain d, resolver_ptr r)
    {
        //get map driver
        static auto& driver = get_driver();

        //insert a resolver
        auto itr = driver.m_domain_resolver_map->emplace(strpath_manip::localize(d),std::unique_ptr<resolver>(r));
        itr->second->set_identifier(d);
    }

    std::vector<provider_registry::resolver_ptr> provider_registry::domain_get_resolvers(domain d)
    {
        //get map driver
        static auto& driver = get_driver();
        std::vector<resolver_ptr> resolvers;

        //get range for domains
        const auto& iterator_pair = driver.m_domain_resolver_map->equal_range(d);

        for(auto& [_,value] : pair_range(iterator_pair))
        {
            resolvers.emplace_back(value.get());
        }
        return resolvers;
    }

    provider_registry::resolver_sentinel provider_registry::resolver_sentinel::operator++() const
    {
        return domain_get_next_resolver(*this);
    }

    provider_registry::resolver_sentinel provider_registry::resolver_sentinel::operator--() const
    {
        return domain_get_prev_resolver(*this);
    }

    provider_registry::resolver_ptr provider_registry::resolver_sentinel::operator*() const
    {
        return domain_get_resolver_at(*this);
    }

    provider_registry::resolver_ptr provider_registry::resolver_sentinel::operator->() const
    {
        return domain_get_resolver_at(*this);
    }

    provider_registry::resolver_sentinel provider_registry::domain_get_first_resolver(domain d)
    {
        if(has_domain(d))
        {
            return resolver_sentinel{0,d};
        } else {
            return resolver_sentinel{nullptr};
        }
    }

    provider_registry::resolver_sentinel provider_registry::domain_get_prev_resolver(const resolver_sentinel& iterator)
    {
        if(iterator.index == 0){
            return resolver_sentinel{nullptr};
        } else {
            return resolver_sentinel{iterator.index-1,iterator.inspected_domain};
        }
    }

    provider_registry::resolver_sentinel provider_registry::domain_get_next_resolver(const resolver_sentinel& iterator)
    {
        //get map driver
        static auto& driver = get_driver();
        
        if(iterator.index >= driver.m_domain_resolver_map->count(iterator.inspected_domain)){
            return resolver_sentinel{nullptr};
        } else {
            return resolver_sentinel{iterator.index+1,iterator.inspected_domain};
        }
    }

    provider_registry::resolver_sentinel provider_registry::domain_get_resolver_end()
    {
        return resolver_sentinel{nullptr};
    }

    provider_registry::resolver_ptr provider_registry::domain_get_resolver_at(const resolver_sentinel& iterator)
    {
        //check against sentinel value
        if(iterator == resolver_sentinel{nullptr}) return nullptr;
        
        //get map driver
        static auto& driver = get_driver();

        //get range of domains
        auto real_iterator = driver.m_domain_resolver_map->find(iterator.inspected_domain);

        if(!checked_next(real_iterator,driver.m_domain_resolver_map->end(),iterator.index))
        {
            return nullptr;
        }

        return real_iterator->second.get();
    }




    bool operator==(const provider_registry::resolver_sentinel& lhs, const provider_registry::resolver_sentinel& rhs)
    {
        if (lhs.index == provider_registry::resolver_sentinel::sentinel_value && rhs.index == provider_registry::
            resolver_sentinel::sentinel_value) return true;
        if (lhs.index == provider_registry::resolver_sentinel::sentinel_value || rhs.index == provider_registry::
            resolver_sentinel::sentinel_value) return false;
        return lhs.index == rhs.index && rhs.inspected_domain == lhs.inspected_domain;
    }

}
