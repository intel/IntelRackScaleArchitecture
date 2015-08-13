/*!
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
 *
 * @file lvm_api.hpp
 * @brief C++ wrapper for liblvm2app
 * */

#ifndef PSME_STORAGE_LVM_API_HPP
#define PSME_STORAGE_LVM_API_HPP

#include "storage_config.hpp"

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <iterator>

#ifdef LVM2APP_FOUND
#if __cplusplus >= 201103L
extern "C" {
#endif
#include <lvm2app.h>
#if __cplusplus >= 201103L
}
#endif
#endif

namespace agent {
namespace storage {
namespace lvm {

using std::vector;
using std::string;

/*!
 * @brief C++ wrapper for liblvm2app
 */
class LvmAPI {
public:
    class PhysicalVolume {
        string m_name{};
        float m_capacity_gb{};
        string m_volume_group{};
        static constexpr const bool m_is_protected = false;
        string m_status{};
        string m_health{};

    public:
        /*! Default constructor */
        PhysicalVolume() {}

        /*! Default copy constructor */
        PhysicalVolume(const PhysicalVolume&) = default;

        /*! Default assignment operator */
        PhysicalVolume& operator=(const PhysicalVolume&) = default;

        /*!
         * @brief Get physical volume name
         * @return Physical volume name
         * */
        const string& get_name() const {
            return m_name;
        }

        /*!
         * @brief Set physical volume name
         * @param[in] name Physical volume name
         * */
        void set_name(const string& name) {
            m_name = name;
        }

        /*!
         * @brief Get physical volume capacity in GB
         * @return Physical volume capacity in GB
         * */
        float get_capacity_gb() const {
            return m_capacity_gb;
        }

        /*!
         * @brief Set physical volume capacity in GB
         * @param[in] capacity_gb Physical volume capacity in GB
         * */
        void set_capacity_gb(float capacity_gb) {
            m_capacity_gb = capacity_gb;
        }

        /*!
         * @brief Get physical volume's volume_group
         * @return Physical volume's volume_group
         * */
        const string& get_volume_group() const {
            return m_volume_group;
        }

        /*!
         * @brief Set physical volume's volume_group
         * @param[in] volume_group Physical volume's volume_group
         * */
        void set_volume_group(const string& volume_group) {
            m_volume_group = volume_group;
        }

        /*!
         * @brief Get information about physical volume protection
         * @return True if physical volume is read only
         * */
        bool get_protection_status() const {
            return m_is_protected;
        }

        /*!
         * @brief Get physical volume health
         * @return Physical volume health
         * */
        const string& get_health() const {
            return m_health;
        }

        /*!
         * @brief Set physical volume health
         * @param[in] health Physical volume health
         * */
        void set_health(const string& health) {
            m_health = health;
        }

        /*!
         * @brief Get physical volume status
         * @return Physical volume status
         * */
        const string& get_status() const {
            return m_status;
        }

        /*!
         * @brief Set physical volume status
         * @param[in] status Physical volume status
         * */
        void set_status(const string& status) {
            m_status = status;
        }

    };

    class LogicalVolume {
        string m_name{};
        float m_capacity_gb{};
        string m_volume_group{};
        bool m_is_protected{};
        bool m_is_snapshot{};
        string m_status{};
        string m_health{};

    public:
        /*! Default constructor */
        LogicalVolume() {}

        /*! Default copy constructor */
        LogicalVolume(const LogicalVolume&) = default;

        /*! Default assignment operator */
        LogicalVolume& operator=(const LogicalVolume&) = default;

        /*!
         * @brief Get logical volume name
         * @return Logical volume name
         * */
        const string& get_name() const {
            return m_name;
        }

        /*!
         * @brief Set logical volume name
         * @param[in] name Logical volume name
         * */
        void set_name(const string& name) {
            m_name = name;
        }

        /*!
         * @brief Get logical volume capacity in GB
         * @return Logical volume capacity in GB
         * */
        float get_capacity_gb() const {
            return m_capacity_gb;
        }

        /*!
         * @brief Set logical volume capacity in GB
         * @param[in] capacity_gb Logical volume capacity in GB
         * */
        void set_capacity_gb(float capacity_gb) {
            m_capacity_gb = capacity_gb;
        }

        /*!
         * @brief Get logical volume's volume_group
         * @return Logical volume's volume_group
         * */
        const string& get_volume_group() const {
            return m_volume_group;
        }

        /*!
         * @brief Set logical volume's volume_group
         * @param[in] volume_group Logical volume's volume_group
         * */
        void set_volume_group(const string& volume_group) {
            m_volume_group = volume_group;
        }

        /*!
         * @brief Get information about logical volume protection
         * @return True if logical volume is read only
         * */
        bool get_protection_status() const {
            return m_is_protected;
        }

        /*!
         * @brief Set information about logical volume protection
         * @param[in] protection_status should be set to true if logical volume is read only
         * */
        void set_protection_status(bool protection_status) {
            m_is_protected = protection_status;
        }

        /*!
         * @brief Get logical volume health
         * @return Logical volume health
         * */
        const string& get_health() const {
            return m_health;
        }

        /*!
         * @brief Set logical volume health
         * @param[in] health Logical volume health
         * */
        void set_health(const string& health) {
            m_health = health;
        }

        /*!
         * @brief Get logical volume status
         * @return Logical volume status
         * */
        const string& get_status() const {
            return m_status;
        }

        /*!
         * @brief Set logical volume status
         * @param[in] status Logical volume status
         * */
        void set_status(const string& status) {
            m_status = status;
        }

        /*!
         * @brief Get logical volume snapshot status
         * @return Logical volume snapshot status
         * */
        bool get_snapshot_status() const {
            return m_is_snapshot;
        }

        /*!
         * @brief Set logical volume snapshot status
         * @param[in] snapshot_status Logical volume snapshot status
         * */
        void set_snapshot_status(bool snapshot_status) {
            m_is_snapshot = snapshot_status;
        }

    };

    class VolumeGroup {
        string m_name{};
        float m_capacity_gb{};
        bool m_is_protected{};
        string m_status{};
        string m_health{};

    public:
        /*! Physical Volumes of this Volume Group */
        std::vector<PhysicalVolume> physical_volumes;

        /*! Logical Volumes of this Volume Group */
        std::vector<LogicalVolume> logical_volumes;

        /*! Default constructor */
        VolumeGroup() : physical_volumes(), logical_volumes() {}

        /*! Default copy constructor */
        VolumeGroup(const VolumeGroup&) = default;

        /*! Default assignment operator */
        VolumeGroup& operator=(const VolumeGroup&) = default;

        /*!
         * @brief Get volume group name
         * @return Volume group name
         * */
        const string& get_name() const {
            return m_name;
        }

        /*!
         * @brief Set volume group name
         * @param[in] name Volume group name
         * */
        void set_name(const string& name) {
            m_name = name;
        }

        /*!
         * @brief Get volume group capacity in GB
         * @return Volume group capacity in GB
         * */
        float get_capacity_gb() const {
            return m_capacity_gb;
        }

        /*!
         * @brief Set volume group capacity in GB
         * @param[in] capacity_gb Volume group capacity in GB
         * */
        void set_capacity_gb(float capacity_gb) {
            m_capacity_gb = capacity_gb;
        }


        /*!
         * @brief Get information about volume group protection
         * @return True if volume group is read only
         * */
        bool get_protection_status() const {
            return m_is_protected;
        }

        /*!
         * @brief Set information about volume group protection
         * @param[in] protection_status should be set to true if volume group is read only
         * */
        void set_protection_status(bool protection_status) {
            m_is_protected = protection_status;
        }

        /*!
         * @brief Get volume group health
         * @return Volume group health
         * */
        const string& get_health() const {
            return m_health;
        }

        /*!
         * @brief Set volume group health
         * @param[in] health Volume group health
         * */
        void set_health(const string& health) {
            m_health = health;
        }

        /*!
         * @brief Get volume group status
         * @return Volume group status
         * */
        const string& get_status() const {
            return m_status;
        }

        /*!
         * @brief Set volume group status
         * @param[in] status Volume group status
         * */
        void set_status(const string& status) {
            m_status = status;
        }
    };

    /*!
     * @brief Discover volume groups with their physical and logical volumes
     * @param[in] volume_groups Vector of volume groups to be filled.
     * */
    void discover_volume_groups_structure(vector<VolumeGroup>& volume_groups);

    /*!
     * @brief Create snapshot clone of given logical volume
     * @param[in] vg_name Name of volume group of logical volume to be cloned
     * @param[in] lv_name Name of logical volume to be cloned
     * @param[in] snapshot_name Name for the cloned logical volume
     * @param[in] size_bytes Size in bytes for the cloned logical volume
     * @return Bool value with operation status
     * */
    bool create_snapshot(const char *vg_name, const char *lv_name, const char *snapshot_name, uint64_t size_bytes);

    /*!
     * @brief Create full clone of given logical volume. It takes time, so better run it in separate thread.
     * @param[in] vg_name Name of volume group of logical volume to be cloned
     * @param[in] lv_name Name of logical volume to be cloned
     * @param[in] clone_name Name for the cloned logical volume
     * @param[in] size_bytes Size in bytes for the cloned logical volume
     * @return Bool value with operation status
     * */
    bool create_clone(const char *vg_name, const char *lv_name, const char *clone_name, uint64_t size_bytes);

    /*!
     * @brief Remove given logical volume
     * @param[in] vg_name Name of volume group of logical volume to be removed
     * @param[in] lv_name Name of logical volume to be removed
     * @return Bool value with operation status
     * */
    bool remove_logical_volume(const char *vg_name, const char *lv_name);

private:
#ifdef LVM2APP_FOUND
    void m_discover_physical_volumes(LvmAPI::VolumeGroup& volume_group, vg_t& vg_handle, lvm_t& lvm_handle);
    void m_discover_logical_volumes(LvmAPI::VolumeGroup& volume_group, vg_t& vg_handle, lvm_t& lvm_handle);
#endif

    string m_boot_device{};
    static constexpr const char *health_ok = "OK";
    static constexpr const char *health_critical = "Critical";
    static constexpr const char *state_enabled = "Enabled";
    static constexpr const char *state_disabled = "Disabled";

    //default flag for lvm handle
    static constexpr uint32_t flags = 0;

    //lvm attribute properties for LV, VG, PV
    static constexpr const char *lv_attr_property = "lv_attr";
    static constexpr const char *vg_attr_property = "vg_attr";
    static constexpr const char *pv_attr_property = "pv_attr";

    //lvm attribute value for read-only permission
    static constexpr const char *READ_MODE = "r";
    //lvm attribute value for write permission
    static constexpr const char *WRITE_MODE = "w";
    //lv_attr bit for checking read-write permission
    static constexpr const short lv_rw_attr = 1;
    //vg_attr bit for checking read-write permission
    static constexpr const short vg_rw_attr = 0;

    //LV attribute value for active state
    static constexpr const char *STATE_ACTIVE = "a";
    //LV, VG attribute value which means they are missing PV
    static constexpr const char *HEALTH_CRITICAL = "p";
    //PV attribute value which means it misses PV segments
    static constexpr const char *PV_STATUS_HEALTH_CRITICAL = "m";
    //lv_attr bit for checking status
    static constexpr const short lv_status_attr = 4;
    //lv_attr bit for checking health
    static constexpr const short lv_health_attr = 8;
    //vg_attr bit for checking health and status
    static constexpr const short vg_status_health_attr = 3;
    //pv_attr bit for checking health and status
    static constexpr const short pv_status_health_attr = 2;

    //LV attribute value for snapshot
    static constexpr const char *LV_SNAPSHOT_TYPE = "s";
    //LV attribute value for virtual type
    static constexpr const char *LV_VIRTUAL_TYPE = "v";
    //lv_attr bit for checking type
    static constexpr const short lv_type_attr = 0;

};

}
}
}
#endif	/* PSME_STORAGE_LVM_API_HPP */

