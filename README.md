# Alyn

## Build & Compile
```bash
export ZEPHYR_BASE=/mnt/sdb1/yair/stm32/zephyrproject/zephyr
export ZEPHYR_TOOLCHAIN_VARIANT=zephyr
west build -b stm32f4_disco  --  -G"Unix Makefiles"

cmake -GNinja -DBOARD=stm32f4_disco -DOVERLAY_CONFIG=prj.conf -DDTC_OVERLAY_FILE=app.overlay ..
```

## Vim and YouCompleteMe setup for Qt5 
To allow YouCompleteMe (YCM) to work fine with qt5 project we an use the plugin [YCM-Generator](https://github.com/rdnetto/YCM-Generator) to create the file *.ycm_extra_conf.py* and place it under the main directory of the project. 
```bash
cd  ~/.vim/bundle/YCM-Generator
./config_gen.py ~/the/project/main/directory/
```

## Optinal boards
[NUCLEO-L4R5ZI](https://www.digikey.co.il/he/products/detail/stmicroelectronics/NUCLEO-L4R5ZI/7807685)
[NUCLEO-H745ZI-Q](https://www.digikey.co.il/he/products/detail/stmicroelectronics/NUCLEO-H745ZI-Q/10498890?s=N4IgTCBcDaIHIFUDCAZAogeQLQAkDsALAKwBaAklgIogC6AvkA)

[NUCLEO-F429ZI](https://www.digikey.co.il/he/products/detail/stmicroelectronics/NUCLEO-F429ZI/5806777?s=N4IgTCBcDaIHIFUDCAZAogeQLQDEAsYAnAFoCSIAugL5A)

[NUCLEO-F439ZI](https://www.digikey.co.il/he/products/detail/stmicroelectronics/NUCLEO-F439ZI/9520178?s=N4IgTCBcDaIHIFUDCAZAogeQLQDEAsAzAJwBaAkiALoC%2BQA)
[Zephyr Supported boards](https://docs.zephyrproject.org/latest/boards/arm/nucleo_f429zi/doc/index.html)

[NUCLEO-F767ZI](https://www.digikey.co.il/he/products/detail/stmicroelectronics/NUCLEO-F767ZI/6004740?s=N4IgTCBcDaIHIFUDCAZAogeQLQDEDsAbHgFoCSIAugL5A)
