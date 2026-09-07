<img width="400" alt="Screenshot from 2026-08-31 13-50-42" src="https://github.com/user-attachments/assets/f0ac657a-2aeb-47ba-b5f3-095151d217a2" />
<img width="400" alt="Screenshot from 2026-08-31 16-01-34" src="https://github.com/user-attachments/assets/6bb2917a-74cc-44b3-8bcb-29e63211cc7f" />


Have you ever thought about how computers fill polygons?

in modern computer graphics everything is made out of triangles

and the only thing the computer knows to fill.

So you need a triangulation algorithm to split any polygon into triangles.

I implemented in c++ the 'ear clipping triangulation' algorithm to visualize the process.

The algorithm is built on the theorem that for every polygon convex or concave there's at least two corners(ears) that can be removed leaving a valid polygon that doesn't intersect itself.

By looping through the vertices and whenever you find an 'ear' create from it a triangle and remove it from the remaining polygon and continue to loop you end up with a list of triangles that represent the polygon.

The interesting challenges i encountered were how to determine if a corner is convex or concave or in other words is the triangle inside or outside the shape.

In trigonometry there's no angle above 180 instead we say that the angle is flipped in 3d by 180 degrees, this is represented by the value of the cross product that is 1 if the angle is facing one direction and \-1 for the other.

This means we can distinguish between convex and concave corners by calculating the cross product and getting \-1 and 1 accordingly.

But there's still a problem in some shapes \-1 means convex and 1 means concave while in other shapes its the opposite.

The reason is that the computer always checks the vertices in the same order so sometimes the vertices order is clockwise and sometimes counterclockwise.

The way you check if the order is clockwise or counterclockwise is actually quite simple

you calculate the signed area of the vertices in the vertices order

if the result is negative it means the top points of the shape that have the bigger area because they are higher, have a negative area because they are ordered from right to left thus the shapes order direction is counterclockwise.
