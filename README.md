# Alyn

Developing an immersive game for children with special needs set in a pool environment is a multifaceted endeavor that requires a comprehensive approach encompassing hardware integration, software development, and a keen understanding of the unique requirements of the target audience. This project aims to leverage cutting-edge technology, including the STM32 microcontroller unit (MCU), ZephyrOS, and Lua scripting, to create a captivating and accessible gaming experience.

## Hardware Integration with STM32 MCU
The STM32 MCU serves as the central nervous system of the gaming system, providing the necessary processing power and connectivity to interact with various peripherals. Through its versatile GPIO pins, UART, SPI, and I2C interfaces, the STM32 MCU seamlessly interfaces with components such as a screen display, keyboard, and LED integration.
The choice of STM32 MCU is strategic due to its reliability, efficiency, and widespread adoption in embedded systems development. Its low-power consumption and robust performance make it ideal for powering portable gaming devices tailored for children with special needs. By harnessing the capabilities of the STM32 MCU, developers can ensure optimal performance and responsiveness, crucial for delivering an engaging gaming experience.

## Operating System Support with ZephyrOS
ZephyrOS, an open-source real-time operating system, serves as the software foundation for the gaming platform. With its modular architecture and extensive device support, ZephyrOS provides a conducive environment for developing embedded applications with stringent performance requirements.
One of the key advantages of ZephyrOS is its scalability, allowing developers to tailor the operating system to suit the specific needs of the gaming platform. By leveraging ZephyrOS's rich set of features, including preemptive multitasking, device drivers, and networking protocols, developers can focus on implementing game logic and user interactions without being encumbered by low-level system concerns.
Additionally, ZephyrOS offers robust support for debugging and testing, essential for ensuring the reliability and stability of the gaming platform. Through its integrated development environment (IDE) and comprehensive documentation, developers can streamline the development process and expedite time-to-market for the immersive game.

## Scripting Capabilities with Lua Engine
To empower users with greater control over the gaming experience, the project incorporates a Lua engine for scripting game logic and interactions. Lua's lightweight and flexible nature make it an ideal choice for embedding scripting capabilities into the gaming platform, enabling users to customize gameplay elements to suit their preferences and abilities.
By exposing a rich set of Lua APIs for interacting with the game world, developers can empower users to create custom levels, modify game mechanics, and implement accessibility features tailored to the needs of children with special needs. This extensibility not only enhances the replay value of the game but also fosters a sense of ownership and agency among players, promoting engagement and immersion.
Furthermore, Lua's simplicity and ease of use make it accessible to developers of all skill levels, facilitating rapid prototyping and iteration. Whether implementing complex game mechanics or fine-tuning visual effects, Lua provides a versatile and expressive scripting language that empowers developers to bring their creative vision to life.


## Install the SW development ennviroment
* setup zephyr local path
```bash
export PROJECT=/home/yair/alyn/zephyr-3.4.99/
export PROJECT=//mnt/sdb1/yair/stm32/zephyrproject/
```

* install west tool
```bash
python3 -m venv $PROJECT/.venv
source $PROJECT/.venv/bin/activate
pip install pyelftools
pip install west
```

* Install zephyr 3.4.0
```bash
cd $PROJECT
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
export ZEPHYR_BASE=${PROJECT}/zephyr
export ZEPHYR_TOOLCHAIN_VARIANT=zephyr
```

Create projects using west
```bash
west build -b nucleo_f429zi -d  nucleo_f429zi . -- -G"Unix Makefiles"  # For CMake project
west build -b nucleo_f429zi -d  nucleo_f429zi                          # For Ninja project
```

Alternatively, create projects directly with CMake
```bash
mkdir nucleo_f429zi
cd nucleo_f429zi
cmake -DBOARD=nucleo_f429zi -DOVERLAY_CONFIG=prj.conf -DDTC_OVERLAY_FILE=./boards/nucleo_f429zi.overlay .. # Add '-GNinja' to the CMake command to force Ninja build
```
or, to build for linux execution
```bash
mkdir native_posix_64
cd native_posix_64
cmake -DBOARD=native_posix_64 -DOVERLAY_CONFIG=prj.conf -DDTC_OVERLAY_FILE=./boards/native_posix_64.overlay ..
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
