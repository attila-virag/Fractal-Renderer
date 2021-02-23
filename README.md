# Fractal-Renderer

# Background

This project evolved organically over many years and incrementally became more organized. I used to write fractal generating and rendering code on-and-off mainly using c++.
At times what came out was legtimately presentable but often the images were just so so. The point really was to have some fun and try to code everything from scratch. 

Since calculating a fractal is a computationally involved thing and could take a long time it soon became apparent to separate the data generation part from the rendering 
and coloring parts. That way I can re-color the same data set without waiting to re-generate. This is the basic principle in this program: we have an exploratory preview 
mode which generates a 500 by 500 image that is used to launch a data set calculation. The data set gets saved to file and can be loaded to overlay different color schemes and 
generate images from.

# Coloring

Coloring is done using color interpolation. The general color function takes a value range 0-1 as input and returns and R,G,B value. The R,G,B values are taken from a pallette
that can be generated or loaded from file. The palette can be either cyclic or one way. For a cyclic palette the color at 0 is the same as the color at 1.

# Normalization

The coloring function can be applied to the whole image for a parameter of interest but the image data has to be normalized first. We can apply different normalization
strategies some of which can apply data smoothing - i.e. elevating the low ranges of the data to be nearer the high ranges. If the parameter that we are coloring on is 
continous we shall get smooth changing colors regardless but if the parameter is not continous (as in Mandelbrot set escape iteration count) we may need to apply some 
smoothing to make the colors come out better. 





