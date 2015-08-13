/*!
 * @section LICENSE
 *
 * @copyright
 * Copyright (c) 2015 Intel Corporation
 *
 * @copyright
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * @copyright
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * @copyright
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * @section DESCRIPTION
 *
 * @file core/dto/collections_dto.hpp
 * @brief Collections response transfer object declaration.
 *
 * */

#ifndef PSME_CORE_DTO_COLLECTIONS_DTO_HPP
#define PSME_CORE_DTO_COLLECTIONS_DTO_HPP

#include "response_dto.hpp"
#include <string>

namespace Json {
class Value;
}

/*! PSME namespace */
namespace psme {
/*! Core namespace */
namespace core {
/*! Dto namespace */
namespace dto {

/*! Collections transfer object */
class CollectionsDTO {
public:
    /*! Copy constructor */
    CollectionsDTO(const CollectionsDTO &) = default;

    /*! Assigment constructor */
    CollectionsDTO& operator=(const CollectionsDTO &) = default;

    /*! Default constructor */
    CollectionsDTO() {}

    /*! Collection entry class */
    class Collection {
    public:
        /*! Constructor name and type are mandatory fields */
        Collection(const std::string& name, const std::string& type)
            : m_name(name), m_type(type) {}

        /*!
         * @brief Set collection entry name
         * @param name Entry name
         */
        void set_name(const std::string& name);

        /*!
         * @brief Get collection entry name
         * @return Entry name
         */
        const std::string& get_name() const;

        /*!
         * @brief Set collection entry type
         * @param type Entry type
         */
        void set_type(const std::string& type);

        /*!
         * @brief Get collection entry type
         * @return Entry type
         */
        const std::string& get_type() const;

        /*!
         * @brief Set collection entry slot mask
         * @param slot_mask Entry slot mask
         */
        void set_slot_mask(const std::string& slot_mask);

        /*!
         * @brief Get collection entry slot mask
         * @return Entry slot mask
         */
        const std::string& get_slot_mask() const;
    private:
        std::string m_name{};
        std::string m_type{};
        std::string m_slot_mask{};
    };

    class Response : ResponseDTO {
        using collections_t = std::vector<Collection>;
        collections_t m_collections{};
    public:
        /*! Copy constructor */
        Response(const Response &) = default;

        /*! Assigment constructor */
        Response& operator=(const Response &) = default;

        /*! Default constructor */
        Response() : ResponseDTO(){}

        /*!
         * Desarializes collection data response object from JSON.
         *
         * @param response Response JSON
         * */
        void to_object(const Json::Value& response);

        /*!
         * @brief Add entry to collection
         * @param entry Collection entry object
         */
        void add_collection(const Collection& entry);

        /*!
         * @brief Set collections
         * @param collections Collection array
         */
        void set_collections(const collections_t& collections);

        /*!
         * @brief Get collections
         * @return Collection array
         */
        const collections_t& get_collections() const;

        virtual ~Response();
    };

    virtual ~CollectionsDTO();
};

}
}
}

#endif /* PSME_CORE_DTO_COLLECTIONS_DTO_HPP */

