# Alyn


## Install the SW development ennviroment
* Install zephyr 3.4.0
```bash
cd /path/to/project/zephyr
west init -m https://github.com/zephyrproject-rtos/zephyr --mr v3.4.0
west update
```

* Download the projct
```bash
git clone github.com:yairgd/alyn.git
```

* Build & Compile
Set Zephyr environment variables
```bash
export ZEPHYR_BASE=/mnt/sdb1/yair/stm32/zephyrproject/zephyr
export ZEPHYR_TOOLCHAIN_VARIANT=zephyr
```

Create projects using west
```bash
west build -b nucleo_f429zi . -- -G"Unix Makefiles"  # For CMake project
west build -b nucleo_f429zi                           # For Ninja project
```

Alternatively, create projects directly with CMake
```bash
cmake -DBOARD=nucleo_f429zi -DOVERLAY_CONFIG=prj.conf -DDTC_OVERLAY_FILE=./boards/nucleo_f429zi.overlay .. # Add '-GNinja' to the CMake command to force Ninja build
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
