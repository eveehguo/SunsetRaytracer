ffmpeg -framerate 10 -start_number 0 -i obj7/%d.png -c:v libx264 -vf "fps=10,format=yuv420p" obj7.mp4
