# DonkeyHill
NodeMCU Esp8266 RGB LED Control Via Unreal Engine DMX, using Artnet.

# What is it good for?
I always liked to do things on my own way,
keeping the price of a project as low
as possible. It's a great way to understand
how the tools around us work.

This project aims to offer a very cheap, DIY
style RGB light, which can be controlled
wirelessly, using any wifi network, from
Unreal Engine 5 via the built in DMX plugin.

Of course there are pre-built products that can
be used for similar usecases, but this light
can be built for a very low price, it is extremely
customisable, and it has the joy of building
something functional.

The main use case for me was to make a light
That matches the color of my Unreal Engine
composite background for home-scale virtual
production, like puppeteering before a green
screen, or make an explainer video.

#How does it work?

This setup uses a NodeMCU Esp8266, to drive
an RGB led chip using 3 MOSFET with pulse-width
modulation. Since the led needs cooling,
there's a dht11 sensor watching the temperature
of the led, controls a 12vdc fan using pwm,
And shuts off the whole thing if it gets too hot.

The code running on the NodeMCU is able to be
configured and store the wifi credentials, and
if everything goes well, it starts to work
automatically once powered on. And tells you
all the important informations using an oled display.

#What Hardware do I need?

this part will be full of pictures and connection diagrams.
