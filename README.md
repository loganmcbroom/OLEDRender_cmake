This is all the code I use to generate OLED animations for keyboard screens. This wasn't made for other people to use so there is no manual and there could be mistakes, but it does make some pretty sweet animations. 
It can render using uv surfaces and level surfaces using ray marching. If you want to add a new object you will need a normal equation along with the surface equation. 
If you're not familiar with multivariable calculus, represent your surface equation in the form g(x,y,z)=0, then use symbolab or a similar online solver to find the "gradient" of g. That will be the normal equation.

If you just want to use some premade animations check out the animations folder.
