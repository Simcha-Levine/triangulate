if meson compile -C builddir; then
    echo "_"
    echo "_"
    echo "_"
    echo "_"
    ./builddir/triangulate
else 
    echo "-sory"
fi