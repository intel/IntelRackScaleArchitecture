package com.intel.rsa.common.types;

/**
 * Interface of StorageController asset.
 */
public enum StorageControllerInterface implements ConvertableEnum {
    PCIE("PCIe"),
    SAS("SAS"),
    SATA("SATA");

    private String value;

    StorageControllerInterface(String value) {
        this.value = value;
    }

    @Override
    public String getValue() {
        return value;
    }

    @Override
    public String toString() {
        return getValue();
    }
}
