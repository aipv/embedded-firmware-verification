set(EFV_ROOT ${CMAKE_CURRENT_LIST_DIR}/../../../..)

idf_component_register(
    SRCS 
        "${EFV_ROOT}/core/src/efv_tweetnacl_wrapper.c"
        "${EFV_ROOT}/core/src/efv_ed25519.c"
        "efv_boot_verify.c"
    INCLUDE_DIRS
        "${EFV_ROOT}/core/include"
        "${EFV_ROOT}/ext/tweetnacl"
         "."
    REQUIRES bootloader_support spi_flash
)

target_compile_options(${COMPONENT_LIB} PRIVATE -O2)