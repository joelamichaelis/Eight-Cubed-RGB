# Custom 8x8x8 RGB LED Cube

Check out the companion repo: https://github.com/joelamichaelis/8x8x8_RGB_LED_CUBE
It appears that repo was made first.
It appears that I made this repo to try and migrate away from that once since it has a lot of messy firmware builds.

This repository contains my custom 8x8x8 RGB LED cube design/build, which I worked on from late 2018 through early 2020.
This is primarily intended as an archived engineering showcase of the custom hardware, PCB, and firmware work rather than as a polished build guide.

## Context

This is one of my first hobby electronics designs that I consider worth showcasing.

I stopped developing this project years ago. Prior to this 2026 documentation update, the repository contained only a standalone Keil firmware project with no other context.

The firmware is set up as a Keil MDK 5 project, exported from STM32CubeMX. I used Keil because it was the embedded development environment I was familiar with from college at the time. I like Keil, but in later projects, I have generally moved toward STM32CubeIDE and other completely free VS Code/GCC-based workflows. I have not migrated this repository.

The original RGB LED cube was built with a classmate for a one-semester, one-credit senior design class in Fall 2018, shortly before I graduated with my BSEE that December.

After graduating, I was left with a prototype-grade 8x8x8 RGB LED cube wired to prototype-grade electronics on protoboards and hastily connected to a rough Arduino sketch.

I wanted to keep developing the project, which led me to design custom electronics on a custom PCB, paired with custom STM32 firmware that I architected and wrote myself.

I named the custom PCBA the **motherboard** because it is the core electronics piece of the design. The motherboard combines the following circuitry onto a custom PCBA:

- Power supply
- LED power load switches
- STM32F103 Blue Pill microcontroller
- PWM drivers
- LCD
- Capacitive touch scroll wheel and keypad
- Unused ESP8266 RF module
- Supporting level-shifting circuitry

I wanted to gain real embedded system design experience, so I decided to move away from Arduino and use an STM32F103 on a Blue Pill microcontroller platform instead.

At the time, STM32F103 Blue Pill boards were abundant and inexpensive, around $20 for a 10-pack on eBay. The ones I used appeared to be genuine, or at least not poor-quality clones, which is different from what I observed after the COVID-era supply chain disruption.

## Physical Build

The physical build closely followed Steve Manley's RGB LED cube guides.

The key dependencies this project has from Steve's build are his mechanical build and assembly instructions, along with the cube's 9x9-inch base PCB.

The electronics, firmware implementation, and other design choices may have had some inspiration from Steve's and other people's designs, but my intent was to create my own design from scratch.

- Steve Manley's YouTube instructional series: https://www.youtube.com/playlist?app=desktop&list=PL2eohiNrVbjwBBmdMBNiG6VZEHkuPX1jt
- Steve Manley's Dropbox share, where his files and base PCBA have been made publicly available: https://www.dropbox.com/scl/fo/yy7fa9qwqz1qt9lec915d/ANIrxRMUm1n4fHg-VZvry4I?rlkey=tztry4fiox118r8166pro4mfg&e=1&dl=0

This part of the project is not the highlight of this repository, but it is still worth documenting. Search the repository for supplemental photos.

My classmate created the mechanical jigs and helped with other parts of the build. I remember spending many nights and evenings bending and trimming individual LEDs with a jig, soldering those LEDs into 1x8 strips with bus wire using a second jig, and then soldering those 1x8 strips into 8x8 vertical panels with a third jig.

The 8x8 vertical panels were then soldered onto a base PCB we ordered. That base PCB adapts the cube's 192 soldered connections into 8 sets of 3x 1x8 header groupings.

Each TLC5940 current PWM driver used in this design controls 16 channels, so 12 daisy-chained PWM drivers map to all of the 1x8 headers.

## Motherboard

Our original senior design implementation required a jumbled mess of custom ribbon cables. The motherboard I designed eliminated the need for those ribbon cables and avoided having prototype electronics crammed under the cube.

Instead, the motherboard cleanly mates female 1x8 headers to the LED cube base's male 1x8 headers.

I also have some 3D-printed mechanical base parts that I intend to include in the repository if possible.

## Project Status / Known Issues

This project is in a functionally mature state with some known bugs, but it is no longer under active development.

Because this work was done several years ago, some files may be rough, incomplete, or not organized to the standard I would use for a current project. The goal of this repository is to preserve and share the project rather than present it as an actively maintained product or polished reference design.

I stopped developing the project after eventually getting bored, wanting to work on other things, or perhaps taking a break to move and then never taking the project out of its tub again.

I consider the core firmware driver and architecture logic to be architecturally mature.

I built a framework in the firmware to accept 3D video, but stopped short of creating a PC program or other tool that would allow animation files to be written and loaded via an SD card or other means.

I quickly learned that the STM32F103's 32K/64K flash was acceptable for simple, algorithmically defined animations, but inadequate for importing and running user-defined custom animations. This led me to realize that I should add an SD card to load and run externally defined animations, although I do not believe I fully developed that feature.

There is a Motherboard Rev2 in this repository. It appears to be functionally coherent, but I do not think I ever ordered those boards. At the time, I was still thinking through other changes I wanted to make, such as adding an SD card so non-hardcoded animations could be loaded without reflashing the LED cube.

As noted on the Rev1 schematic, there are a couple of level-shifter circuits that do not work with the discrete implementation because they are used with high-speed switching signals and the bandwidth of my level-shifter design is insufficient. I reworked my board to fix this.

The Rev2 design, which I never ordered, also addresses this by moving away from the component-heavy discrete level-shifting scheme and using the TXS0108 level-shifter IC, which I have come to prefer.

I remember there being some sort of startup transient that I never fixed. If I had all 8 of the 8x8 LED vertical layers connected to their FET load switches during power-up, all 512 LEDs would light up dimly white-ish. I believe this was some sort of startup transient inducing a brownout. If I ever resurrect development on this project, this would be a high-priority issue to address.

Due to the startup transient bug mentioned above, I made a dummy 8x8 LED array that I used for development without the persistence-of-vision effect of multiplexing all 8 layers.

I spent a lot of time experimenting with different 2D animations and making them accessible through the LCD menu, which can be navigated using the capacitive touch scroll wheel / touchpad.

I remember realizing that the capacitive touch scroll wheel is less useful than the keypad, and that only one of the two is really needed to navigate a menu. I also remember realizing that while it was fun to use this project to experiment with capacitive touch inputs, that part was unnecessary. When my hands got slightly sweaty, it was hard to get the capacitive touch inputs to behave the way I wanted.

Mechanical buttons would have been simpler, although the capacitive touch implementation was fun to experiment with.

A fimrware architectural design doc is missing & warranted.

## Repository Contents

Depending on what has been uploaded, this repository may include:

- PCB design files
- Firmware source code
- Hardware notes
- Photos of the completed cube
- Supporting documentation or design references
- 3D-printed mechanical base parts, if available

## Notes

This repository is provided as-is. It may be useful for reference, inspiration, or as an example of a custom RGB LED cube hardware project, but it should not be assumed to be a drop-in build guide without reviewing the design files and code.

## License

No license has been specified yet.
