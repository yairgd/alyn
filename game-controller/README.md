# alyn
#!/bin/bash
export ZEPHYR_BASE=/mnt/sdb1/yair/stm32/zephyrproject/zephyr
export ZEPHYR_TOOLCHAIN_VARIANT=zephyr
west build -b stm32f4_disco  --  -G"Unix Makefiles"

cmake -GNinja -DBOARD=stm32f4_disco -DOVERLAY_CONFIG=prj.conf -DDTC_OVERLAY_FILE=app.overlay ..
