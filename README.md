# DonkeyHill Firefly
NodeMCU Esp8266 RGB LED Control Via Unreal Engine DMX, using Artnet.

# What is it good for?
I always liked to do things on my own way, keeping the price of a project as low as possible. It's a great way to understand how the tools around us work.

This project aims to offer a very cheap, DIY style RGB light, which can be controlled wirelessly, using any wifi network, from Unreal Engine 5 via the built in DMX plugin.

Of course there are pre-built products that can be used for similar usecases, but this light can be built for a very low price, it is extremely customisable, and it has the joy of building something functional. Keep in mind that this project is not for professional, big corpo film production use, but a nice and cheap solution for hobbyists who like to build things.

The main use case for me was to make a light that matches the color of my Unreal Engine composite background for home-scale virtual production, like puppeteering before a green screen, or make an explainer video.

# How does it work?

This setup uses a NodeMCU Esp8266, to drive an RGB led chip using 3 MOSFET with pulse-width modulation. Since the led needs cooling, there's a dht11 sensor watching the temperature of the led, controls a 12vdc fan using pwm, And shuts off the whole thing if it gets too hot.

The code running on the NodeMCU is able to be configured and store the wifi credentials, and if everything goes well, it starts to work automatically once powered on. And it tells you all the important informations using an oled display.

# What Hardware do I need?

![TERVRAJZ-RGBLAMPA](https://user-images.githubusercontent.com/60850001/218952777-49bcdf8b-97c2-4c55-9624-259ecf9abea8.jpg)

The whole circuit starts with an AC-DC step-down converter, which can be plugged in a power outlet, and gives a nice 12V dc output. The current configuration uses a 10w RGB led, so the current of the AC-DC step-down converter is only 2A. This can be changed to a bigger power source if you are going to use a bigger led. 

![5366_4](https://user-images.githubusercontent.com/60850001/218955039-35cf1262-b02e-4244-be1e-996f187917c5.jpg)

The next part is a DC-DC step-down converter, which reduces the voltage from 12V to 5V, so we can power up our NodeMcu, the converter has a maximum output current of 3A, so it's a bit overkill for the NodeMcu, this part can be changed to every DC-DC step-down converter, or even a 7805 IC. Be aware, that we need two of this component, since the RED channel usually does not work on the same voltage as the blue and green Channel, so we have to reduce the 12vdc for the led, in this case to 6V. If you Use more powerful LEDs, the voltages will be higher too.

![dc_dc_konverter_mini_step_down_3A_1](https://user-images.githubusercontent.com/60850001/218955410-c4def351-6830-4746-9555-a9803a5a550a.jpg)

The following thing is optional, Especially if you use stronger a led chip, which is a 250W boost constant current module. As the name suggests, this module is capable of MAX 250W output, but it has the option of set the current, and even step-up convert 12V to even 50V.

![7289_3](https://user-images.githubusercontent.com/60850001/218956705-02b63cc1-d166-425f-b73f-099c7a742367.jpg)

Next comes the ESP8266 CH340 NodeMcu V3 Wireless module. It is possible that the code works with other types of modules, such as esp32, but I haven't tested it since I currently don't own any of other boards. This module is a well-known, very popular microcontroller, which can be programmed using Arduino IDE. It is capable of genereating PWM signals, and reiciving inputs from various sensors. Not to mention it can be connected to WiFi networks aswell. Basically it's the brain of the whole light.

![4582_5](https://user-images.githubusercontent.com/60850001/218957657-e6bfcb2d-ab11-428b-91d9-321bba3dcb6d.jpg)

There are three components which will be connected to the NodeMCU board. The first one is a 128x32 oled display. This will be responsible of telling crucial information to the user such as wifi connection status, temperature readings, and received inputs from the ArtNet protocol. This part is also optional if you find it too pricy, and I will provide a version of the code without the display.

![aa321](https://user-images.githubusercontent.com/60850001/218958315-63fbced7-8442-4052-b970-e2af83d55f67.jpg)

The second component connected to the NodeMCU is a DHT-11 Temperature (and humidity) sensor, I had this laying around at home, and I knew how to use one. If you ask that why don't you use a thermistor or a 18b20 temp sensor, well, it's about the same price as a dht11.

![szenzor_paratartalom_DHT11_panelen_1](https://user-images.githubusercontent.com/60850001/218959256-d46be714-d1c9-4dae-9333-55da03e1454e.jpg)

The third component is the real deal: it's a IRF 540 module - 4 Channel Opto Isolated DC Switch. Bacially it acts as a faucet, but instead of water, it uses electricity. Since the NodeMCU can not output the power the LED needs, we need this MOSFET to control the brightness of the LED. There are four mosfets on this board, 3 for each color of the LED (Red, Green, Blue), and one for the DC Fan cooling the system. It is an absolute overkill, it can drive 33Amps of current, if you find it too pricy, there are other MOSFETs you can use, even without a board. Be aware that if you use it with a more powerful LED, you really should use heatsinks.

![4515_1](https://user-images.githubusercontent.com/60850001/218960952-ce1f55e3-c8de-4456-bf43-6d3feee770a7.jpg)

The Led will give off a lot of heat, even this 10w led! Not to mention if you use a stronger led, there will be a LOT more heat. So we need some active cooling, in this case, a 12VDC fan. The bigger the better, since smaller ones can be loud. You can even use your old PC fan laying around somewhere. If you are using a simple, not PWM controllable fan, then you need to connect it to the 4th mosfet, if you are using a PWM controllable version, in theory, you can connect it directly to one of the digital outputs on the NodeMCU.

![7467_1](https://user-images.githubusercontent.com/60850001/218962417-3cf28bf6-b892-48e3-bf8b-a8dbf01914da.jpg)

The LED. there are many types of these components, you can even use a led strip, or solder a bunch of leds on a breadboard instead of an smd led chip, but this prototype is currently using a 10w version. Be aware, that different LED's have different characteristics, and powerul led chips require more voltage and they give off way more heat. The sad thing is that if you want to keep the price low, you have to make compromises. Cheap RGB LED's usually give off different light intensity on each color, usually the blue is the strongest, next comes the green, and the red is very weak compared to the other two. What does that mean? Well, if you want to get a true white color light, you have to adjust the light intensity of the blue and green channels to the red channel. You can find the currently used method later in this description.

![7363_1](https://user-images.githubusercontent.com/60850001/218964136-382cf1b7-80e2-44bb-baab-0d44fbe8f854.jpg)

Heatsink, and other cooling components. For the Mosfets, I used 11x11x5mm aluminium heatsinks, and for the LED, well I used an old processor heatsink laying around in my house. There are many options, so it's on you what suits your configuration the best.

![7134_1](https://user-images.githubusercontent.com/60850001/218965414-185d0ac7-7237-4354-bb13-c3a0f7f009f1.jpg)

The current version of this light needs to be configured to be able to connect to your wifi network, but I did'nt want to upload a new code every time I bring this lamp somewhere else, so there is two methods to set up the connection:
- The first one is when the light can not connect to the WiFi network, after 10 attempt, it will automatically switch to Config Mode, where you need to connect it to your PC with an USB cable, and give it the required data such as SSID, Password, Ip, Gateway, and Subnet Mask. I made a cute little program that can be downloaded, and it helps you to set up your light. Currently Windows only, I don't have acces to Mac or Linux systems yet, but if you are a developer who would like to help me, I would be grateful. (Will be uploaded soon)
- The second one is for manually going to config mode, pressing a button. It's basically the same as the previous method, but it allows you to connect to another wifi any time. (It's currently not included in the code, be patient!)

# What Software Do I need?

-Arduino IDE
https://wiki-content.arduino.cc/en/software

-The config app from the releases page

- Unreal Engine 4, or 5. But i suggest that you have this already since you are here ;)
https://www.unrealengine.com/en-US/download

//In theory, the light also works with dmx control software that use the Artnet protocol///

# Step by step assembly

There will be something here soon.

# Setting up the software

Still making the tutorial, be patient please :)

# License

soon.
