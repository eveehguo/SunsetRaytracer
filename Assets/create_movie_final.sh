ffmpeg -framerate 30 -start_number 0 -i final/%d.png -c:v libx264 -vf "fps=30,format=yuv420p" final.mp4
