//===------------------------ resource.hpp ------------------------===//
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

#pragma once

#include "utopia/exception.hpp"
#include "utopia/stream.hpp"
#include <fstream>

namespace utopia::core {

    /**
     * @brief 资源类型
    */
    enum class ResourceType {
        /**
         * @brief 文件
        */
        File,
        /**
         * @brief 网络文件
        */
        Net,
        /**
         * @brief 程序自己构造的资源
        */
        Customize
    };

    /**
     * @brief 负责管理资源，如文件等。
    */
    struct ResourceManager {
        ResourceManager()  = delete;
        ~ResourceManager() = delete;
    };

    /**
     * @brief 资源句柄的抽象类
    */
    struct ResourceHandle {
        virtual ~ResourceHandle() = default;

        /**
         * @brief 打开资源流
        */
        virtual Stream OpenResource() = 0;

        /**
         * @brief 打开只读资源流
        */
        virtual InputStream     OpenReadOnlyStream() const        = 0;

        ResourceHandle         &operator=(const ResourceHandle &) = delete;
        virtual ResourceHandle &operator=(ResourceHandle &&)      = default;
    };

    /**
     * @brief 文件资源实现
    */
    class FileResourceHandle : public ResourceHandle {
        std::fstream fs;

      public:

        FileResourceHandle(const std::string &path) {
            fs.open(path);

            if(fs.fail()) {
                throw IOException{ "failed to open file" };
            }
        }
    };


}   // namespace utopia::core
