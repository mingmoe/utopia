//===--------------------- entity.hpp ---------------------===//
//
// this file is under the MIT License
// See https://opensource.org/licenses/MIT for license information.
// Copyright(c) 2020-2022 moe-org All rights reserved.
//
//===------------------------------------------------------===//
/// \file
/// 这个文件声明了一个实体接口。
//===------------------------------------------------------===//

#pragma once

#include "utopia/guuid.hpp"
#include "utopia/hasher.hpp"
#include <memory>

namespace utopia::core {

    /**
     * @brief stand for a game entity
    */
    class Entity : public Hashable {
        Guuid id;

      public:

        Entity(const Guuid entity_id) : id(entity_id) {}

        virtual ~Entity()                          = default;

        virtual Entity &operator=(const Entity &)  = 0;
        virtual Entity &operator=(const Entity &&) = 0;

        /**
         * @brief 比较两个实体是否"等价"
         * @param another 另一个要比较的实体
         * @return 如果等级，则返回true
         * @note 此函数用于比较实体是否"等价"，即所有属性都应该相同。
         * 如果要比较两种实体是否是同一种，则应该比较两种实体的id是否相同。
        */
        virtual bool operator==(const Entity &another) const = 0;

        /**
         * @brief 获取实体id，id应该是唯一的而且不变的。
        */
        virtual Guuid get_id() const noexcept {
            return id;
        }
    };


}   // namespace utopia::core
