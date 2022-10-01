//===-------------------------- world.hpp -------------------------===//
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
/// 负责实现游戏地图相关的数据结构。
//===--------------------------------------------------------------===//

#pragma once

#include "utopia/entity.hpp"
#include "utopia/guuid.hpp"
#include "utopia/serializable.hpp"
#include <array>
#include <cstdint>
#include <map>
#include <memory>
#include <optional>
#include <set>
#include <vector>

namespace utopia::core {

    /**
     * @brief 坐标系单位
    */
    using pos_t = std::int64_t;

    struct FlatPosition {
        pos_t x;
        pos_t y;
    };

    struct Position : public FlatPosition {
        pos_t z;
    };

    struct WorldPosition : public Position {
        /**
         * @brief stand for the id of the world
        */
        pos_t id;
    };

    /**
     * @brief 地图方块
    */
    class Block : public Hashable {
        std::vector<std::shared_ptr<Entity>>           entities{};

        std::map<Guuid, std::shared_ptr<Serializable>> datas{};

        bool                                           collidable{ false };
        bool                                           blockable{ false };

      public:

        void add(std::shared_ptr<Entity> entity) {
            if(this->blockable && blockable) {
                throw RuntimeException{
                    "add a blockable to another blockable!"
                };
            }
            else {
                this->blockable = entity->is_blockable();
            }

            this->blockable = this->blockable || entity->is_collidable();

            this->entities.push_back(entity);
        }

        bool is_collidable() const noexcept {
            return this->collidable;
        }

        bool is_blockable() const noexcept {
            return this->blockable;
        }

        /**
         * @brief 删除一个与参数相等的实体（调用operator==())
        */
        void remove(std::shared_ptr<Entity> entity) {
            for(auto begin = entities.begin(), end = entities.end();
                begin != end;
                begin++) {
                if(begin->get()->operator==(*entity.get())) {
                    entities.erase(begin);
                    return;
                }
            }
        }

        /**
         * @brief 删除所有与参数相等的实体（调用operator==())
        */
        void remove_all(std::shared_ptr<Entity> entity) {
            for(auto begin = entities.begin(), end = entities.end();
                begin != end;
                begin++) {
                if(begin->get()->operator==(*entity.get())) {
                    entities.erase(begin);
                    begin = entities.begin();
                    end   = entities.end();
                }
            }
        }

        /**
         * @brief 删除所有具有相同id的实体
        */
        void remove_all(Guuid id) {
            for(auto begin = entities.begin(), end = entities.end();
                begin != end;
                begin++) {
                if(begin->get()->get_id().operator==(id)) {
                    entities.erase(begin);
                    begin = entities.begin();
                    end   = entities.end();
                }
            }
        }

        /**
         * @brief 获取方块所有实体的列表
        */
        std::vector<std::shared_ptr<Entity>> get_all() const {
            return this->entities;
        }

        /**
         * @brief 获取所有实体的id
         * @return 实体的id
        */
        std::set<Guuid> get_all_id() const {
            std::set<Guuid> ids{};
            for(auto &entity : this->entities) {
                ids.insert(entity->get_id());
            }
            return ids;
        }

        virtual void get_hash(Hasher &hasher) const override {
            for(auto &entity : entities) {
                entity->get_hash(hasher);
            }
        }
    };

    /**
     * @brief 地区区域
    */
    struct Area {
        static constexpr const pos_t X_BLOCKS_COUNT = 32;
        static constexpr const pos_t Y_BLOCKS_COUNT = 32;

      private:

        /**
         * @brief 以左下角为原点
        */
        using SingleFloor =
            std::array<std::array<std::shared_ptr<Block>, Y_BLOCKS_COUNT>,
                       X_BLOCKS_COUNT>;

        SingleFloor                  zero_floor{};

        std::map<pos_t, SingleFloor> other_floor{};

        /**
         * @brief 初始化某个地图层
        */
        void init_floor(SingleFloor &floor) {
            for(auto &x : floor) {
                for(auto &y : x) {
                    if(y.get() == nullptr) {
                        y = std::make_shared<Block>();
                    }
                }
            }
        }

      public:

        Area() {
            init_floor(zero_floor);
        }

        /**
         * @brief 检查坐标是否在area范围内
         * @return 如果在范围内返回true
        */
        static inline bool check_in_range(Position p) noexcept {
            if(p.x >= X_BLOCKS_COUNT) {
                return false;
            }
            if(p.y >= Y_BLOCKS_COUNT) {
                return false;
            }
            return true;
        }

        /**
         * @brief 获取指定坐标的方块
         * @param pos 坐标
         * @return 方块
        */
        std::optional<std::shared_ptr<Block>> get_block(Position pos) noexcept {
            std::shared_ptr<Block> block{};

            if(pos.x >= X_BLOCKS_COUNT || pos.y >= Y_BLOCKS_COUNT ||
               pos.x < 0 || pos.y < 0) {
                return std::nullopt;
            }


            if(pos.z == 0) {
                block = zero_floor.at(pos.x).at(pos.y);
            }
            else {
                auto found = other_floor.find(pos.z);

                if(found == other_floor.cend()) {
                    auto p = std::pair(pos.z, SingleFloor{});

                    init_floor(p.second);

                    block = p.second.at(pos.x).at(pos.y);

                    other_floor.insert(std::move(p));
                }
                else {
                    block = found->second.at(pos.x).at(pos.y);
                }
            }

            return std::make_optional(block);
        }
    };

    /**
     * @brief 世界
    */
    struct World {

        /**
         * @brief X轴区域数量
        */
        static constexpr const pos_t X_AREA_COUNT = 32;

        /**
         * @brief Y轴区域数量
        */
        static constexpr const pos_t Y_AREA_COUNT = 32;

        /**
         * @brief 地图中心距离原点的x区域数量.
         * 另一个说法是，X的负半轴有几个区域。
        */
        static constexpr const std::ptrdiff_t CENTER_X = 16;

        /**
         * @brief 地图中心距离原点的y区域数量
         * 另一个说法是，Y的负半轴有几个区域。
        */
        static constexpr const std::ptrdiff_t CENTER_Y = 16;

      private:

        // 以左下角为原点
        std::array<std::array<std::shared_ptr<Area>, Y_AREA_COUNT>,
                   X_AREA_COUNT>
            areas{};

        /**
         * @brief 根据地图原点和中心标准化索引
         * @param x x轴索引，带正负值
         * @param y y轴索引，带正负值
         * @return 可用的直接索引
        */
        static constexpr inline std::tuple<std::size_t, std::size_t>
            standardization(std::ptrdiff_t x, std::ptrdiff_t y) {
            auto xs = static_cast<std::size_t>(x + CENTER_X - 1);
            auto ys = static_cast<std::size_t>(y + CENTER_Y - 1);

            return std::make_tuple(xs, ys);
        }

        /**
         * @brief 根据区域索引获取区域
         * @param x x轴索引
         * @param y y轴
         * @return 如果索引超出范围，则返回nullopt
        */
        std::optional<std::shared_ptr<Area>>
            get_area_by_index(std::ptrdiff_t x, std::ptrdiff_t y) {
            auto [xi, yi] = standardization(x, y);

            if(xi >= this->areas.size() || yi >= this->areas.at(xi).size()) {
                return std::nullopt;
            }

            return std::make_optional(areas.at(x).at(y));
        }

        const Guuid world_id;


      public:

        World(Guuid id) : world_id(id) {
            for(auto &x : areas) {
                for(auto &y : x) {
                    if(y.get() == nullptr) {
                        y = std::make_shared<Area>();
                    }
                }
            }
        }

        std::optional<std::shared_ptr<Block>> get_block(Position pos) {
            auto x_index = static_cast<std::ptrdiff_t>(
                std::floor(pos.x / Area::X_BLOCKS_COUNT));
            auto y_index = static_cast<std::ptrdiff_t>(
                std::floor(pos.y / Area::Y_BLOCKS_COUNT));

            auto block = get_area_by_index(x_index, y_index);

            if(!block.has_value()) {
                throw OutOfRangeException{ "position out of range" };
            }

            auto p = Position{};

            if(pos.x < 0)
                pos.x = -pos.x;
            if(pos.y < 0)
                pos.y = -pos.y;

            p.x = pos.x % Area::X_BLOCKS_COUNT;
            p.y = pos.y % Area::Y_BLOCKS_COUNT;
            p.z = pos.z;

            return block.value()->get_block(p);
        }


        /**
         * @brief 获取世界的id
         * @return 世界id
        */
        inline Guuid get_id() const noexcept {
            return this->world_id;
        }
    };

    /**
     * @brief 世界管理器
    */
    class WorldManager {
        std::map<Guuid, std::shared_ptr<World>> worlds{};

      public:

        /**
         * @return 如果世界id已经存在，则返回false。插入成功返回true
        */
        bool insert(std::shared_ptr<World> new_world) {
            if(worlds.find(new_world->get_id()) != worlds.cend()) {
                return false;
            }

            this->worlds.insert(std::make_pair(new_world->get_id(), new_world));
            return true;
        }

        std::optional<std::shared_ptr<World>> get_world(Guuid id) {
            auto it = worlds.find(id);

            if(it != worlds.end()) {
                return (*it).second;
            }
            else {
                return std::nullopt;
            }
        }

        std::vector<std::shared_ptr<World>> get_all_worlds() {
            std::vector<std::shared_ptr<World>> opts{};
            for(auto &it : worlds) {
                opts.push_back(it.second);
            }
            return opts;
        }
    };

    // 游戏地图构成:
    // WorldManager -> World -> Areas -> Blocks
    // 对应获取的坐标:
    // WorldPosition(with world id) -> Position(3D) -> FlatPotition(2D)

}   // namespace utopia::core
