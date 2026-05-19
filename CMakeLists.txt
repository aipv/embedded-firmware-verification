cmake_minimum_required(VERSION 3.10)
project(sbc C)

set(CMAKE_C_STANDARD 99)

# ==== find libsodium ====
find_package(PkgConfig REQUIRED)
pkg_check_modules(SODIUM REQUIRED libsodium)

# ================================ include ================================

# ================================ library ================================
add_library(bss STATIC ext/bss/bss_utils.c)
target_include_directories(bss PUBLIC ${CMAKE_SOURCE_DIR}/ext/bss)

add_library(core STATIC core/src/efv_ed25519.c core/src/efv_tweetnacl_wrapper.c)
target_include_directories(core PUBLIC ${CMAKE_SOURCE_DIR}/core/include ${CMAKE_SOURCE_DIR}/ext/tweetnacl)

# ================================ test ================================
add_executable(test_efv_sha256 core/test/test_efv_sha256.c core/src/efv_sha256.c)
target_include_directories(test_efv_sha256 PUBLIC ${CMAKE_SOURCE_DIR}/core/include)

add_executable(test_efv_ed25519 core/test/test_efv_ed25519.c)
target_link_libraries(test_efv_ed25519 core)

# ================================ tools ================================
add_executable(sodium_keypair_create tools/sodium_keypair_create.c)
target_link_libraries(sodium_keypair_create bss ${SODIUM_LIBRARIES})

add_executable(sodium_payload_sign tools/sodium_payload_sign.c)
target_link_libraries(sodium_payload_sign bss ${SODIUM_LIBRARIES})

add_executable(sodium_image_create tools/sodium_image_create.c)
target_include_directories(sodium_image_create PUBLIC ${CMAKE_SOURCE_DIR}/core/include)
target_link_libraries(sodium_image_create bss ${SODIUM_LIBRARIES})

add_executable(sodium_signature_verify tools/sodium_signature_verify.c)
target_link_libraries(sodium_signature_verify bss ${SODIUM_LIBRARIES})

add_executable(tweetnacl_signature_verify tools/tweetnacl_signature_verify.c)
target_include_directories(tweetnacl_signature_verify PUBLIC ${CMAKE_SOURCE_DIR}/ext/tweetnacl)
target_link_libraries(tweetnacl_signature_verify bss)

add_executable(efv_signature_verify tools/efv_signature_verify.c)
target_link_libraries(efv_signature_verify bss core)

add_executable(efv_image_verify tools/efv_image_verify.c core/src/efv_sha256.c)
target_link_libraries(efv_image_verify bss core)

add_executable(check_payload_hash tools/check_payload_hash.c core/src/efv_sha256.c)
target_include_directories(check_payload_hash PUBLIC ${CMAKE_SOURCE_DIR}/core/include)
target_link_libraries(check_payload_hash bss ${SODIUM_LIBRARIES})
