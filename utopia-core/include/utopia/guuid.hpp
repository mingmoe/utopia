//===---------------------- guuid.hpp ---------------------===//
//
// this file is under the MIT License
// See https://opensource.org/licenses/MIT for license information.
// Copyright(c) 2020-2022 moe-org All rights reserved.
//
//===------------------------------------------------------===//

#pragma once

#include "utopia/constexpr.hpp"
#include "utopia/exception.hpp"
#include "utopia/hasher.hpp"
#include "utopia/template.hpp"
#include <atomic>
#include <cctype>
#include <chrono>
#include <compare>
#include <ctime>
#include <fmt/core.h>
#include <initializer_list>
#include <random>
#include <string>
#include <string_view>
#include <thread>
#include <vector>

namespace utopia::core {

    /**
     * @brief Global Universally Unique Identifier
    */
    class Guuid {
        std::string              root;

        std::vector<std::string> namespaces;

      public:

        /**
         * @brief 检查字符串是否可以作为root或者namespace
         * @param str 要检查的字符串
         * @return 如果合法，返回true，否则返回false
        */
        constexpr static bool is_legal_name(std::string_view str) noexcept {
            if(str.size() == 0) {
                return false;
            }

            for(auto c : str) {
                if(utopia::core::isalnum(c) == 0 && c != '_') {
                    return false;
                }
            }
            return true;
        }

        /**
         * @brief 构造一个新的guuid
         * @param root guuid的root
         * @param begin guuid的namespaces迭代器的begin
         * @param end guuid的namespaces迭代器的end
         * 
         * @note root和namespaces必须满足正则表达式[a-zA-Z0-9_]+
        */
        template<class IT>
        constexpr Guuid(std::string_view root, IT begin, IT end) {
            auto check = [](const std::string_view &str)
            {
                if(!is_legal_name(str)) {
                    throw IllegalArgumentException{ "illegal guuid name" };
                }
            };
            check(root);
            this->root = root;

            while(begin != end) {
                check(*begin);
                this->namespaces.emplace_back(*begin);
                begin++;
            }
        }

        /**
         * @brief 构造一个新的guuid
         * @param root guuid的root
         * @param namespaces guuid的namespaces
         * 
         * @note root和namespaces必须满足正则表达式[a-zA-Z0-9_]+
        */
        constexpr Guuid(std::string_view                        root,
                        std::initializer_list<std::string_view> namespaces) :
            Guuid(root, namespaces.begin(), namespaces.end()) {}

        /**
         * @brief 构造一个新的guuid
         * @param root guuid的root
         * @param namespaces guuid的namespaces
         * 
         * @note root和namespaces必须满足正则表达式[a-zA-Z0-9_]+
        */
        constexpr Guuid(std::string_view              root,
                        std::vector<std::string_view> namespaces) :
            Guuid(root, namespaces.cbegin(), namespaces.cend()) {}

        constexpr inline std::string get_root() const {
            return this->root;
        }

        constexpr inline std::vector<std::string> get_namespaces() const {
            return this->namespaces;
        }

        constexpr inline std::string_view get_root_ref() const {
            return std::string_view{ this->root };
        }

        constexpr inline std::vector<std::string_view>
            get_namespace_ref() const {
            std::vector<std::string_view> refs;
            refs.reserve(namespaces.size());

            for(auto &r : namespaces) {
                refs.emplace_back(r);
            }

            return refs;
        }

        constexpr inline std::strong_ordering
            operator<=>(const Guuid &another) const {
            auto result = this->get_root_ref() <=> another.get_root_ref();

            if(result == std::strong_ordering::equal) {
                return this->get_namespace_ref() <=>
                       another.get_namespace_ref();
            }
            else {
                return result;
            }
        }

        constexpr inline bool operator==(const Guuid &another) const {
            return ((*this) <=> another) == std::strong_ordering::equal;
        }

        /**
         * @brief 转换为字符串
         * @return 字符串表示形式
        */
        inline std::string to_string() {
            std::stringstream ss{};

            ss << this->get_root_ref() << ":";

            for(auto &str : this->get_namespace_ref()) {
                ss << str << ":";
            }

            auto str = ss.str();

            str.erase(str.size() - 1);
            return str;
        }

        /**
         * @brief 根据已有的guuid生成一个独特的guuid。
         * 例如: 给出game:worlds:id会返回game:worlds:id:XXXXXXXXXX等。
         * @param origin 已有的guuid
         * @return 独特的新的guuid
        */
        static inline Guuid unique(Guuid origin) {
            static std::atomic<std::size_t>         unique_id{ 0 };
            static const time_t                     t  = time(nullptr);
            static const auto                      *tm = std::localtime(&t);
            thread_local utopia::core::XXHashHasher hasher{};
            thread_local std::random_device         rd{};
            thread_local std::thread::id t_id = std::this_thread::get_id();
            thread_local std::hash<std::thread::id> id_hasher{};

            // write time
            hasher.write(tm->tm_isdst);
            hasher.write(tm->tm_year);
            hasher.write(tm->tm_mon);
            hasher.write(tm->tm_yday);
            hasher.write(tm->tm_mday);
            hasher.write(tm->tm_wday);
            hasher.write(tm->tm_hour);
            hasher.write(tm->tm_min);
            hasher.write(tm->tm_sec);

            // write origin
            hasher.write(origin.get_root_ref());
            for(auto &s : origin.get_namespace_ref()) {
                hasher.write(s);
            }

            // write some others
            hasher.write(rd());
            hasher.write(id_hasher.operator()(t_id));
            hasher.write(unique_id.fetch_add(1));

            // generate
            auto id = hasher.workout();
            hasher.reset();

            auto unique_str = fmt::format("{:X}", id);
            auto copied     = origin.get_namespace_ref();
            copied.emplace_back(std::move(unique_str));

            return Guuid(origin.get_root_ref(), copied);
        }

        /**
         * @brief 获取一个新的，独立的guuid
         * @return 一个新的guuid
        */
        static inline Guuid new_unique() {
            return unique(Guuid{ "unique_id", { "unique" } });
        }
    };

}   // namespace utopia::core
