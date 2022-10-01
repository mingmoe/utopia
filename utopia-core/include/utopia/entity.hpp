//===------------------------- entity.hpp -------------------------===//
//   Copyright (C) 2021-2022 mingmoe(me@kawayi.moe)(https://kawayi.moe)
//
//   This program is free software: you can redistribute it and/or modify
//   it under the terms of the GNU Affero General Public License as published by
//   the Free Software Foundation, either version 3 of the License, or
//   (at your option) any later version.
//
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU Affero General Public License for more details.
//
//   You should have received a copy of the GNU Affero General Public License
//===--------------------------------------------------------------===//
/// \file
/// 这个文件声明了一个实体接口。
//===--------------------------------------------------------------===//

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

        /**
         * @brief 实体是否可碰撞。
         * 一个方块内不能有两个可碰撞的实体共存。
        */
        virtual bool is_collidable() = 0;

        /**
         * @brief 实体是否可阻挡。
         * 一个方块内不能有多个可阻挡实体共存。
        */
        virtual bool is_blockable() = 0;
    };


}   // namespace utopia::core
