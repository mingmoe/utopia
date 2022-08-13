//===---------------------- serializable.hpp ----------------------===//
//   Copyright (C) 2021-2022 mingmoe(me@kawayi.moe)(https://blog.kawayi.moe)
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
/// 声明一个可序列化接口。实现这个接口要求同时实现反序列化。
/// 这个接口和游戏存档有关，输出和输入二进制格式。
//===------------------------------------------------------===//

#pragma once

#include "utopia/stream.hpp"

namespace utopia::core {

    /**
     * @brief 可序列化和反序列化接口
    */
    struct Serializable {
        virtual void serialization(OutputStream &output) const = 0;
        virtual void deserialize(InputStream &input)           = 0;
    };

    /**
     * @brief 反序列化工厂
     * @tparam T 产生的对象类型
    */
    template<class T>
    struct DeserializationFactory {
        /**
         * @brief 反序列化一个对象
         * @param from_origin 对象的输入流
         * @return 反序列化的对象
        */
        virtual T create(InputStream& from_origin) = 0;
    };

    /**
     * @brief 序列化工厂
     * @tparam T 需要序列化的类型
    */
    template<class T>
    struct SerializationFactory {
        /**
         * @brief 序列化对象
        */
        virtual void save(T& obj,OutputStream& output) = 0;
    };

    // 关于Serializable和XXXFactory的区别:
    // Serializable更适用于独立的，解耦的对象，如可以随意创建的XmlDom
    // Factory更适合集中统一管理的对象，如Entity等

}   // namespace utopia::core
