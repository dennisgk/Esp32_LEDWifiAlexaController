# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.24

# compile C with /Users/denniskountouris/.espressif/tools/xtensa-esp32-elf/esp-2022r1-RC1-11.2.0/xtensa-esp32-elf/bin/xtensa-esp32-elf-gcc
C_DEFINES = -DMBEDTLS_CONFIG_FILE=\"mbedtls/esp_config.h\"

C_INCLUDES = -I/Users/denniskountouris/Desktop/esp-idf/projects/lcd-wifi-controller-simple/build/config -I/Users/denniskountouris/Desktop/esp-idf/components/esp_common/include -I/Users/denniskountouris/Desktop/esp-idf/components/newlib/platform_include -I/Users/denniskountouris/Desktop/esp-idf/components/freertos/FreeRTOS-Kernel/include -I/Users/denniskountouris/Desktop/esp-idf/components/freertos/esp_additions/include/freertos -I/Users/denniskountouris/Desktop/esp-idf/components/freertos/FreeRTOS-Kernel/portable/xtensa/include -I/Users/denniskountouris/Desktop/esp-idf/components/freertos/esp_additions/include -I/Users/denniskountouris/Desktop/esp-idf/components/esp_hw_support/include -I/Users/denniskountouris/Desktop/esp-idf/components/esp_hw_support/include/soc -I/Users/denniskountouris/Desktop/esp-idf/components/esp_hw_support/include/soc/esp32 -I/Users/denniskountouris/Desktop/esp-idf/components/esp_hw_support/port/esp32/. -I/Users/denniskountouris/Desktop/esp-idf/components/esp_hw_support/port/esp32/private_include -I/Users/denniskountouris/Desktop/esp-idf/components/heap/include -I/Users/denniskountouris/Desktop/esp-idf/components/log/include -I/Users/denniskountouris/Desktop/esp-idf/components/soc/include -I/Users/denniskountouris/Desktop/esp-idf/components/soc/esp32/. -I/Users/denniskountouris/Desktop/esp-idf/components/soc/esp32/include -I/Users/denniskountouris/Desktop/esp-idf/components/hal/esp32/include -I/Users/denniskountouris/Desktop/esp-idf/components/hal/include -I/Users/denniskountouris/Desktop/esp-idf/components/hal/platform_port/include -I/Users/denniskountouris/Desktop/esp-idf/components/esp_rom/include -I/Users/denniskountouris/Desktop/esp-idf/components/esp_rom/include/esp32 -I/Users/denniskountouris/Desktop/esp-idf/components/esp_rom/esp32 -I/Users/denniskountouris/Desktop/esp-idf/components/esp_system/include -I/Users/denniskountouris/Desktop/esp-idf/components/esp_system/port/soc -I/Users/denniskountouris/Desktop/esp-idf/components/esp_system/port/include/private -I/Users/denniskountouris/Desktop/esp-idf/components/xtensa/include -I/Users/denniskountouris/Desktop/esp-idf/components/xtensa/esp32/include -I/Users/denniskountouris/Desktop/esp-idf/components/lwip/include -I/Users/denniskountouris/Desktop/esp-idf/components/lwip/include/apps -I/Users/denniskountouris/Desktop/esp-idf/components/lwip/include/apps/sntp -I/Users/denniskountouris/Desktop/esp-idf/components/lwip/lwip/src/include -I/Users/denniskountouris/Desktop/esp-idf/components/lwip/port/esp32/include -I/Users/denniskountouris/Desktop/esp-idf/components/lwip/port/esp32/include/arch -I/Users/denniskountouris/Desktop/esp-idf/components/ulp/ulp_common/include -I/Users/denniskountouris/Desktop/esp-idf/components/ulp/ulp_common/include/esp32 -I/Users/denniskountouris/Desktop/esp-idf/components/driver/include -I/Users/denniskountouris/Desktop/esp-idf/components/driver/deprecated -I/Users/denniskountouris/Desktop/esp-idf/components/driver/esp32/include -I/Users/denniskountouris/Desktop/esp-idf/components/esp_pm/include -I/Users/denniskountouris/Desktop/esp-idf/components/esp_ringbuf/include -I/Users/denniskountouris/Desktop/esp-idf/components/esp_adc/include -I/Users/denniskountouris/Desktop/esp-idf/components/esp_adc/interface -I/Users/denniskountouris/Desktop/esp-idf/components/esp_adc/esp32/include -I/Users/denniskountouris/Desktop/esp-idf/components/esp_adc/deprecated/include -I/Users/denniskountouris/Desktop/esp-idf/components/efuse/include -I/Users/denniskountouris/Desktop/esp-idf/components/efuse/esp32/include -I/Users/denniskountouris/Desktop/esp-idf/components/esp_http_client/include -I/Users/denniskountouris/Desktop/esp-idf/components/esp_http_server/include -I/Users/denniskountouris/Desktop/esp-idf/components/http_parser -I/Users/denniskountouris/Desktop/esp-idf/components/bootloader_support/include -I/Users/denniskountouris/Desktop/esp-idf/components/bootloader_support/bootloader_flash/include -I/Users/denniskountouris/Desktop/esp-idf/components/nvs_flash/include -I/Users/denniskountouris/Desktop/esp-idf/components/spi_flash/include -I/Users/denniskountouris/Desktop/esp-idf/components/esp_wifi/include -I/Users/denniskountouris/Desktop/esp-idf/components/esp_event/include -I/Users/denniskountouris/Desktop/esp-idf/components/esp_phy/include -I/Users/denniskountouris/Desktop/esp-idf/components/esp_phy/esp32/include -I/Users/denniskountouris/Desktop/esp-idf/components/esp_netif/include -I/Users/denniskountouris/Desktop/esp-idf/components/app_update/include -I/Users/denniskountouris/Desktop/esp-idf/components/wpa_supplicant/include -I/Users/denniskountouris/Desktop/esp-idf/components/wpa_supplicant/port/include -I/Users/denniskountouris/Desktop/esp-idf/components/wpa_supplicant/esp_supplicant/include -I/Users/denniskountouris/Desktop/esp-idf/components/esp_serial_slave_link/include -I/Users/denniskountouris/Desktop/esp-idf/components/sdmmc/include -I/Users/denniskountouris/Desktop/esp-idf/components/esp-tls -I/Users/denniskountouris/Desktop/esp-idf/components/esp-tls/esp-tls-crypto -I/Users/denniskountouris/Desktop/esp-idf/components/mbedtls/port/include -I/Users/denniskountouris/Desktop/esp-idf/components/mbedtls/mbedtls/include -I/Users/denniskountouris/Desktop/esp-idf/components/mbedtls/mbedtls/library -I/Users/denniskountouris/Desktop/esp-idf/components/mbedtls/esp_crt_bundle/include -I/Users/denniskountouris/Desktop/esp-idf/components/esp_https_ota/include

C_FLAGS = -mlongcalls -Wno-frame-address  -ffunction-sections -fdata-sections -Wall -Werror=all -Wno-error=unused-function -Wno-error=unused-variable -Wno-error=deprecated-declarations -Wextra -Wno-unused-parameter -Wno-sign-compare -Wno-enum-conversion -gdwarf-4 -ggdb -Og -fmacro-prefix-map=/Users/denniskountouris/Desktop/esp-idf/projects/lcd-wifi-controller-simple=. -fmacro-prefix-map=/Users/denniskountouris/Desktop/esp-idf=/IDF -fstrict-volatile-bitfields -Wno-error=unused-but-set-variable -fno-jump-tables -fno-tree-switch-conversion -DconfigENABLE_FREERTOS_DEBUG_OCDAWARE=1 -std=gnu17 -Wno-old-style-declaration -D_GNU_SOURCE -DIDF_VER=\"v5.0-dev-4723-g30e8f19f5a\" -DESP_PLATFORM -D_POSIX_READER_WRITER_LOCKS

