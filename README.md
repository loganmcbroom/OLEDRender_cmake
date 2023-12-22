This is all the code I use to generate OLED animations for keyboard screens. This wasn't made for other people to use so there is no manual and there could be mistakes, but it does make some pretty sweet animations. 
It can render using uv surfaces and level surfaces using ray marching. If you want to add a new object you will need a normal equation along with the surface equation. 
If you're not familiar with multivariable calculus, represent your surface equation in the form g(x,y,z)=0, then use symbolab or a similar online solver to find the "gradient" of g. That will be the normal equation.

If you just want to use some premade animations check out the animations folder. Just make sure your MCU has enough memory for them, cheaper ones may only have room for a few frames, but the premade animations are in the hundreds of frames. Assuming you're using qmk you will need to fiddle with the following settings in your config.h:

1) I2C1_CLOCK_SPEED. I have this set to 320000, I think this is the only one that matters.
2) OLED_UPDATE_INTERVAL I have this set to 0, but I'm not sure if it matters.
3) OLED_UPDATE_PROCESS_LIMIT I have this at 16. I probably don't need it that high.

You can probably turn the limit back down to 1, but I found these to work for me on a 32x128 oled screen.


