# Fractal-Projects
This project intends to collect various fractal generating code I was messing around over the years in C++.
My idea for now is to make an algorithm agnostic pixel processor that is multithreaded. Ideally this could be used 
with any algorithm for image genrating or processing as long as the algorithms passed in are pixel-by-pixel independent (i.e each pixel can be treated as a separate problem). I plan to apply this to some versions of the Julia set and Mandelbrot set and maybe look into 
incorporating fern fractals depending on how things go.
Along the way this is a good exersize to parctive some C++11 such as multithreading, unique pointers etc.
