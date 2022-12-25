import imageio.v2 as imageio
import glob
import os
import sys
jpg_dir = 'image_dir'
output_dir = 'image_dir'
import re

_nsre = re.compile('([0-9]+)')
def natural_sort_key(s):
    return [int(text) if text.isdigit() else text.lower()
            for text in re.split(_nsre, s)]   



images = []
# for file in glob.glob(os.path.join(jpg_dir,"*.jpg")):
#     images.append(imageio.imread(file))
# imageio.mimsave(os.path.join(output_dir,'ball_simulation.gif'), images)
a = glob.glob(os.path.join(jpg_dir,"*.jpg"))
print(a)
a.sort(key=natural_sort_key)
print(a)

with imageio.get_writer(os.path.join(output_dir,'ball_simulation.gif'), mode='I',duration = 0.1) as writer:
    for file in a:
        image = imageio.imread(file)
        writer.append_data(image)