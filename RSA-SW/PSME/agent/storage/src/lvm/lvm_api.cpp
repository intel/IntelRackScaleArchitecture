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
 * @file lvm_api.cpp
 *
 * @brief C++ wrapper for Liblvm
 * */
#include "lvm_api.hpp"
#include "agent-framework/logger_ext.hpp"
#include "agent-framework/exceptions/exception.hpp"


using namespace agent::storage::lvm;

#ifdef LVM2APP_FOUND
#define KB_TO_GB 1024 / 1024 / 1024

bool LvmAPI::remove_logical_volume(const char *vg_name, const char *lv_name) {

    if (nullptr == vg_name || nullptr == lv_name) {
        log_error(GET_LOGGER("lvm"), "Could not read function parameters.");
        return false;
    }

    vg_t vg_handle = nullptr;
    lv_t lv_handle = nullptr;
    lvm_t lvm_handle = nullptr;
    int ret = 0;

    lvm_handle = lvm_init(nullptr);
    if (nullptr == lvm_handle) {
        log_error(GET_LOGGER("lvm"), "Could not open handle to LVM.");
        return false;
    }

    vg_handle = lvm_vg_open(lvm_handle, vg_name, WRITE_MODE, flags);
    if (nullptr == vg_handle) {
        log_error(GET_LOGGER("lvm"), "Could not open volume group: "
                << lvm_errmsg(lvm_handle));
        lvm_quit(lvm_handle);
        return false;
    }

    lv_handle = lvm_lv_from_name(vg_handle, lv_name);
    if (nullptr == lv_handle) {
        log_error(GET_LOGGER("lvm"), "Could not open logical volume: "
                << lvm_errmsg(lvm_handle));
        lvm_vg_close(vg_handle);
        lvm_quit(lvm_handle);
        return false;
    }

    ret = lvm_vg_remove_lv(lv_handle);
    if (0 != ret) {
        log_error(GET_LOGGER("lvm"), "Could not remove logical volume: "
                << lvm_errmsg(lvm_handle));
        lvm_vg_close(vg_handle);
        lvm_quit(lvm_handle);
        return false;
    }

    ret = lvm_vg_write(vg_handle);
    if (0 != ret) {
        log_error(GET_LOGGER("lvm"), "Could not save changes to volume group: "
                << lvm_errmsg(lvm_handle));
        lvm_vg_close(vg_handle);
        lvm_quit(lvm_handle);
        return false;
    }

    lvm_vg_close(vg_handle);
    lvm_quit(lvm_handle);
    return true;
}

bool LvmAPI::create_snapshot(const char *vg_name, const char *lv_name, const char *snapshot_name, uint64_t size_bytes) {

    if (nullptr == vg_name || nullptr == lv_name || nullptr == snapshot_name) {
        THROW(agent_framework::exceptions::LvmError, "lvm",
            "Could not read create parameters");
    }

    if (0 >= size_bytes) {
        THROW(agent_framework::exceptions::LvmError, "lvm",
            "Snapshot size must be above 0.");
    }

    vg_t vg_handle = nullptr;
    lvm_t lvm_handle = nullptr;
    lv_t lv_handle = nullptr, snapshot_handle = nullptr;
    lvm_handle = lvm_init(nullptr);

    if (nullptr == lvm_handle) {
        THROW(agent_framework::exceptions::LvmError, "lvm",
            "Could not open handle to LVM");
    }

    vg_handle = lvm_vg_open(lvm_handle, vg_name, WRITE_MODE, flags);
    if (nullptr == vg_handle) {
        lvm_quit(lvm_handle);
        THROW(agent_framework::exceptions::LvmError, "lvm",
            "Could not open volume group");
    }

    lv_handle = lvm_lv_from_name(vg_handle, lv_name);
    if (nullptr == lv_handle) {
        lvm_vg_close(vg_handle);
        lvm_quit(lvm_handle);
        THROW(agent_framework::exceptions::LvmError, "lvm",
            "Could not open logical volume");
    }

    snapshot_handle = lvm_lv_snapshot(lv_handle, snapshot_name, size_bytes);
    if (nullptr == snapshot_handle) {
        log_error(GET_LOGGER("lvm"), "Could not create snapshot: "
                << lvm_errmsg(lvm_handle));
        lvm_vg_close(vg_handle);
        lvm_quit(lvm_handle);
        THROW(agent_framework::exceptions::LvmError, "lvm",
            "Could not create snapshot");
    }

    lvm_vg_close(vg_handle);
    lvm_quit(lvm_handle);
    return true;
}

bool LvmAPI::create_clone(const char *vg_name, const char *lv_name, const char *clone_name, uint64_t size_bytes) {

    if (nullptr == vg_name || nullptr == lv_name || nullptr == clone_name) {
        THROW(agent_framework::exceptions::LvmError, "lvm",
            "Could not read create parameters");
    }

    vg_t vg_handle = nullptr;
    lvm_t lvm_handle = nullptr;
    lv_t lv_handle = nullptr, clone_handle = nullptr;
    lvm_handle = lvm_init(nullptr);

    if (nullptr == lvm_handle) {
        THROW(agent_framework::exceptions::LvmError, "lvm",
            "Could not open handle to LVM");
    }

    vg_handle = lvm_vg_open(lvm_handle, vg_name, WRITE_MODE, flags);
    if (nullptr == vg_handle) {
        lvm_quit(lvm_handle);
        THROW(agent_framework::exceptions::LvmError, "lvm",
            "Could not open volume group");
    }

    lv_handle = lvm_lv_from_name(vg_handle, lv_name);
    if (nullptr == lv_handle) {
        lvm_vg_close(vg_handle);
        lvm_quit(lvm_handle);
        THROW(agent_framework::exceptions::LvmError, "lvm",
            "Could not open logical volume");
    }

    if (size_bytes < lvm_lv_get_size(lv_handle)) {
        lvm_vg_close(vg_handle);
        lvm_quit(lvm_handle);
        THROW(agent_framework::exceptions::LvmError, "lvm",
            "Could not create clone size is smaller than source size.");
    }

    clone_handle = lvm_vg_create_lv_linear(vg_handle, clone_name, size_bytes);
    if (nullptr == clone_handle) {
        lvm_vg_close(vg_handle);
        lvm_quit(lvm_handle);
        THROW(agent_framework::exceptions::LvmError, "lvm",
            "Could not create clone.");
    }

    lvm_vg_close(vg_handle);
    lvm_quit(lvm_handle);

    return true;
}

void LvmAPI::m_discover_physical_volumes(LvmAPI::VolumeGroup& volume_group, vg_t& vg_handle, lvm_t& lvm_handle) {
    struct dm_list* pvnames = nullptr;
    struct lvm_pv_list* pv_list = nullptr;
    pvnames = lvm_vg_list_pvs(vg_handle);
    if (nullptr == pvnames) {
        log_error(GET_LOGGER("lvm"), "Could not open physical volumes list: "
                << lvm_errmsg(lvm_handle));
        lvm_list_pvs_free(pvnames);
        return;
    }

    dm_list_iterate_items(pv_list, pvnames) {
        PhysicalVolume physical_volume;
        physical_volume.set_name(lvm_pv_get_name(pv_list->pv));
        physical_volume.set_capacity_gb(static_cast<float> (lvm_pv_get_size(pv_list->pv)) / KB_TO_GB);
        lvm_property_value pv_prop = lvm_pv_get_property(pv_list->pv, pv_attr_property);
        if (pv_prop.value.string[pv_status_health_attr] == *PV_STATUS_HEALTH_CRITICAL) {
            physical_volume.set_health(health_critical);
            physical_volume.set_status(state_disabled);
        } else {
            physical_volume.set_health(health_ok);
            physical_volume.set_status(state_enabled);
        }
        physical_volume.set_volume_group(lvm_vg_get_name(vg_handle));

        log_debug(GET_LOGGER("lvm"), "Found physical volume " << physical_volume.get_name());
        log_debug(GET_LOGGER("lvm"), "Capacity: " << physical_volume.get_capacity_gb());
        log_debug(GET_LOGGER("lvm"), "Is protected: " << physical_volume.get_protection_status());
        log_debug(GET_LOGGER("lvm"), "Health: " << physical_volume.get_health());
        log_debug(GET_LOGGER("lvm"), "Status: " << physical_volume.get_status());
        log_debug(GET_LOGGER("lvm"), "Volume group: " << physical_volume.get_volume_group());

        volume_group.physical_volumes.push_back(physical_volume);
    }
    lvm_list_pvs_free(pvnames);
}

void LvmAPI::m_discover_logical_volumes(LvmAPI::VolumeGroup& volume_group, vg_t& vg_handle, lvm_t& lvm_handle) {
    struct dm_list* lvnames = nullptr;
    struct lvm_lv_list* lv_list = nullptr;
    lvnames = lvm_vg_list_lvs(vg_handle);
    if (nullptr == lvnames) {
        log_error(GET_LOGGER("lvm"), "Could not open logical volumes list: "
                << lvm_errmsg(lvm_handle));
        return;
    }

    dm_list_iterate_items(lv_list, lvnames) {
        lv_t lv_handle = lv_list->lv;

        lvm_property_value lv_prop = lvm_lv_get_property(lv_handle, lv_attr_property);
        //to avoid showing snapshots twice
        if (lv_prop.value.string[lv_type_attr] == *LV_VIRTUAL_TYPE) continue;

        LogicalVolume logical_volume;
        logical_volume.set_capacity_gb(static_cast<float> (lvm_lv_get_size(lv_handle)) / KB_TO_GB);
        logical_volume.set_name(lvm_lv_get_name(lv_handle));
        logical_volume.set_protection_status(lv_prop.value.string[lv_rw_attr] == *WRITE_MODE ? false : true);
        logical_volume.set_health(lv_prop.value.string[lv_health_attr] == *HEALTH_CRITICAL ? health_critical : health_ok);
        logical_volume.set_status(lv_prop.value.string[lv_status_attr] == *STATE_ACTIVE ? state_enabled : state_disabled);
        logical_volume.set_snapshot_status(lv_prop.value.string[lv_type_attr] == *LV_SNAPSHOT_TYPE ? true : false);
        logical_volume.set_volume_group(lvm_vg_get_name(vg_handle));

        log_debug(GET_LOGGER("lvm"), "Found logical volume " << logical_volume.get_name());
        log_debug(GET_LOGGER("lvm"), "Capacity: " << logical_volume.get_capacity_gb());
        log_debug(GET_LOGGER("lvm"), "Is protected: " << logical_volume.get_protection_status());
        log_debug(GET_LOGGER("lvm"), "Health: " << logical_volume.get_health());
        log_debug(GET_LOGGER("lvm"), "Status: " << logical_volume.get_status());
        log_debug(GET_LOGGER("lvm"), "Snapshot: " << logical_volume.get_snapshot_status());
        log_debug(GET_LOGGER("lvm"), "Volume group: " << logical_volume.get_volume_group());

        volume_group.logical_volumes.push_back(logical_volume);
    }
}

void LvmAPI::discover_volume_groups_structure(vector<LvmAPI::VolumeGroup>& volume_groups) {

    lvm_t handle = nullptr;
    handle = lvm_init(nullptr);

    if (nullptr == handle) {
        log_error(GET_LOGGER("lvm"), "Could not open handle to LVM.");
        return;
    }

    vg_t vg = nullptr;
    struct dm_list* vgnames = nullptr;
    struct lvm_str_list* str_list = nullptr;

    vgnames = lvm_list_vg_names(handle);
    if (nullptr == vgnames) {
        log_error(GET_LOGGER("lvm"), "Could not open volume group list: "
                << lvm_errmsg(handle));
        lvm_quit(handle);
        return;
    }

    dm_list_iterate_items(str_list, vgnames) {
        vg = lvm_vg_open(handle, str_list->str, READ_MODE, flags);
        if (nullptr == vg) {
            log_error(GET_LOGGER("lvm"), "Could not open volume group: "
                    << lvm_errmsg(handle));
            lvm_quit(handle);
            return;
        }
        VolumeGroup volume_group;
        volume_group.set_name(str_list->str);
        volume_group.set_capacity_gb(static_cast<float> (lvm_vg_get_size(vg)) / KB_TO_GB);
        lvm_property_value vg_prop = lvm_vg_get_property(vg, vg_attr_property);
        volume_group.set_protection_status(vg_prop.value.string[vg_rw_attr] == *WRITE_MODE ? false : true);
        if (vg_prop.value.string[vg_status_health_attr] == *HEALTH_CRITICAL) {
            volume_group.set_health(health_critical);
            volume_group.set_status(state_disabled);
        } else {
            volume_group.set_health(health_ok);
            volume_group.set_status(state_enabled);
        }

        log_debug(GET_LOGGER("lvm"), "Found volume group " << volume_group.get_name());
        log_debug(GET_LOGGER("lvm"), "Capacity: " << volume_group.get_capacity_gb());
        log_debug(GET_LOGGER("lvm"), "Is protected: " << volume_group.get_protection_status());
        log_debug(GET_LOGGER("lvm"), "Health: " << volume_group.get_health());
        log_debug(GET_LOGGER("lvm"), "Status: " << volume_group.get_status());

        m_discover_physical_volumes(volume_group, vg, handle);
        m_discover_logical_volumes(volume_group, vg, handle);

        lvm_vg_close(vg);
        volume_groups.push_back(volume_group);
    }
    lvm_quit(handle);

}

#endif
