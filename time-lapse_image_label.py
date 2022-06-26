from PIL import Image
from PIL import ImageFont
from PIL import ImageDraw
from os import walk
import os
from datetime import datetime
from datetime import timedelta

IN_DIR = "./RAW"
OUT_DIR = "./LABELED/"

def get_modification_date(filename):
    t = os.path.getmtime(filename)
    return datetime.fromtimestamp(t)


def get_days_hours_minutes(td):
    return td.days, td.seconds//3600, (td.seconds//60) % 60


if __name__ == "__main__":
    file_list = []

    for (dirpath, dirnames, filenames) in walk(IN_DIR):
        for f in filenames:
            if f.endswith(".JPG"):
                file_list.append(os.path.join(dirpath, f))

    print("Total photos to process " + str(len(file_list)))
    print("File names: ")
    print(file_list)

    # clock settings
    photo_time_interval = timedelta(minutes=6)
    clock_margin_bottom = 300
    clock_margin_left = 100
    clock_font = ImageFont.truetype("arial.ttf", 150)
    clock_color = "#1aff1a"
    photo_count = len(file_list)
    photo_counter = 1
    time_counter = timedelta(days=0, hours=0, minutes=0, seconds=0)

    # scale settings
    scale_length = 1350
    scale_margin_right = 100
    scale_margin_bottom = 250
    scale_width = 25
    scale_color = "#FBFCFC"

    for file in file_list:
        print("Processing " + str(file) + "     " + str(photo_counter))
        img = Image.open(file)
        draw = ImageDraw.Draw(img)

        width, height = img.size

        # draw clock
        days, hours, minutes = get_days_hours_minutes(time_counter)
        draw.text((clock_margin_left, height - clock_margin_bottom),
                  "D:" + "{:02d}".format(days) + " H:" + "{:02d}".format(hours)
                  + " M:" + "{:02d}".format(minutes), font=clock_font, fill=clock_color)

        # draw scale
        draw.line((width - scale_margin_right,
                   height - scale_margin_bottom,
                   width - scale_margin_right - scale_length,
                   height - scale_margin_bottom),
                  fill=scale_color,
                  width=scale_width)

        img.save(OUT_DIR + "L_106_" + str(photo_counter).zfill(5) + ".jpg", format='JPEG', quality=95, subsampling=0, dpi=(350, 350))

        print(get_days_hours_minutes(time_counter))
        time_counter += photo_time_interval
        photo_counter += 1
