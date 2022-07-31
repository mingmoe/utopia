//===---------------------- guuid.hpp ---------------------===//
//
// this file is under the MIT License
// See https://opensource.org/licenses/MIT for license information.
// Copyright(c) 2020-2022 moe-org All rights reserved.
//
//===------------------------------------------------------===//

#pragma once

#include "utopia/exception.hpp"
#include <cctype>
#include <initializer_list>
#include <string>
#include <string_view>
#include <compare>
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
        static bool is_legal_name(std::string_view str) noexcept {
            if(str.size() == 0) {
                return false;
            }

            for(auto c : str) {
                if(std::isalnum(c) == 0 && c != '_') {
                    return false;
                }
            }
            return true;
        }

        /**
         * @brief 构造一个新的guuid
         * @param root guuid的root
         * @param namespaces guuid的namespaces
         * 
         * @note root和namespaces必须满足正则表达式[a-zA-Z0-9_]+
        */
        Guuid(std::string_view                        root,
              std::initializer_list<std::string_view> namespaces) {
            auto check = [](std::string_view str)
            {
                if(!is_legal_name(str)) {
                    throw IllegalArgumentException{ "illegal guuid name" };
                }
            };
            check(root);

            this->root = root;
            this->namespaces.reserve(namespaces.size());

            for(auto space : namespaces) {
                check(space);
                this->namespaces.emplace_back(space);
            }
        }

        std::string get_root() const {
            return this->root;
        }

        std::vector<std::string> get_namespaces() const {
            return this->namespaces;
        }

        std::string_view get_root_ref() const {
            return std::string_view{ this->root };
        }

        std::vector<std::string_view> get_namespace_ref() const {
            std::vector<std::string_view> refs;
            refs.reserve(namespaces.size());

            for(auto &r : namespaces) {
                refs.emplace_back(r);
            }

            return refs;
        }

        std::strong_ordering operator<=>(const Guuid& another) const {
            auto result = this->get_root_ref() <=> another.get_root_ref();

            if(result == std::strong_ordering::equal) {
                return this->get_namespace_ref() <=>
                       another.get_namespace_ref();
            }
            else {
                return result;
            }
        }

        bool operator==(const Guuid &another) const {
            return ((*this) <=> another) == std::strong_ordering::equal;
        }
    };

}   // namespace utopia::core
