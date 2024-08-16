# read raw file from Sony .arw
#
# 2024.4.1 by mingtao
#


import numpy as np
import rawpy
import os
import json
import matplotlib.pyplot as plt


def read_files_name(path=None, tag=None):

    if path == None:
        files_name = os.listdir()
    else:
        files_name = os.listdir(path)

    res_name = []

    for file in files_name:
        if tag == None:
            if file[-3:] == 'raw':
                res_name.append(file)
        else:
            if file[-len(tag):] == tag:
                res_name.append(file)

    return res_name


def pattern_mapping(pattern, left, top):

    # R G R G      G R G R      G B G B     B G B G
    # G B G B      B G B G      R G R G     G R G R
    # R G R G      G R G R      G B G B     B G B G
    # G B G B      B G B G      R G R G     G R G R

    left = left % 2
    top = top % 2

    if pattern.upper() == 'RGGB':
        if left == 0 and top == 0:
            cropped_pattern = 'RGGB'
        elif left == 0 and top == 1:
            cropped_pattern = 'GBRG'
        elif left == 1 and top == 0:
            cropped_pattern = 'GRBG'
        elif left == 1 and top == 1:
            cropped_pattern = 'BGGR'
        else:
            print('Wrong!')

    elif pattern.upper() == 'GRBG':
        if left == 1 and top == 0:
            cropped_pattern = 'RGGB'
        elif left == 1 and top == 1:
            cropped_pattern = 'GBRG'
        elif left == 0 and top == 0:
            cropped_pattern = 'GRBG'
        elif left == 0 and top == 1:
            cropped_pattern = 'BGGR'
        else:
            print('Wrong!')

    elif pattern.upper() == 'GBRG':
        if left == 0 and top == 1:
            cropped_pattern = 'RGGB'
        elif left == 0 and top == 0:
            cropped_pattern = 'GBRG'
        elif left == 1 and top == 1:
            cropped_pattern = 'GRBG'
        elif left == 1 and top == 0:
            cropped_pattern = 'BGGR'
        else:
            print('Wrong!')

    elif pattern.upper() == 'BGGR':
        if left == 1 and top == 1:
            cropped_pattern = 'RGGB'
        elif left == 1 and top == 0:
            cropped_pattern = 'GBRG'
        elif left == 0 and top == 1:
            cropped_pattern = 'GRBG'
        elif left == 0 and top == 0:
            cropped_pattern = 'BGGR'
        else:
            print('Wrong!')

    else:
        print("Wrong bayer pattern")

    if cropped_pattern == 'RGGB':
        cropped_pattern = 0
    elif cropped_pattern == 'GRBG':
        cropped_pattern = 1
    elif cropped_pattern == 'GBRG':
        cropped_pattern = 2
    elif cropped_pattern == 'BGGR':
        cropped_pattern = 3

    return cropped_pattern


def get_cropped_pattern(raw, left, top):

    pa = raw.color_desc
    pa = pa.decode('ascii')

    # get cropped bayer pattern
    raw_pattern = raw.raw_pattern.flatten()
    bayer_pattern = ""
    for i in raw_pattern:
        bayer_pattern += pa[i]

    cropped_pattern = pattern_mapping(bayer_pattern, left, top)

    return cropped_pattern


def get_raw(raw, target_height, target_width, save_path):

    half_target_height = target_height >> 1
    half_target_width = target_width >> 1

    raw_img = raw.raw_image

    raw_img = np.array(raw_img)

    if raw.sizes.flip != 0:
        raw_img = raw_img.T
        raw_img = raw_img[::-1]

    height, width = raw_img.shape

    center_x = width >> 1
    center_y = height >> 1

    left_x = center_x - half_target_width
    right_x = center_x + half_target_width

    top_y = center_y - half_target_height
    bottom_y = center_y + half_target_height

    target_area = raw_img[top_y:bottom_y, left_x:right_x]

    target_area.tofile(save_path)

    bayer_pattern = get_cropped_pattern(raw, left_x, top_y)

    # print(bayer_pattern, left_x, top_y)
    return bayer_pattern


def save_params(raw, bayer_pattern, height, width, path):

    params = {}
    params['common'] = {}
    params['blc'] = {}
    params['awb'] = {}

    params['common']['bayer_pattern'] = bayer_pattern
    params['common']['white_level'] = raw.camera_white_level_per_channel[0]
    params['common']['img_height'] = height
    params['common']['img_width'] = width

    params['blc']['black_level'] = raw.black_level_per_channel

    params['awb']['gain'] = raw.camera_whitebalance

    with open(path, 'w') as f:
        json.dump(params, f)

    f.close()


def save_raw(dir_path, target_height, target_width, tag):

    files_name = read_files_name(dir_path, tag)

    assert len(files_name) != 0, "Please check target file."

    for name in files_name:

        raw_path = dir_path + '/' + name
        save_path = dir_path + '/' + name[:-(len(tag) + 1)] + '_' + str(target_width) + 'x' + str(target_height) + '.raw'
        param_path = dir_path + '/' + name[:-(len(tag) + 1)] + '.json'

        raw = rawpy.imread(raw_path)
        pattern = get_raw(raw, target_height, target_width, save_path)
        save_params(raw, pattern, target_height, target_width, param_path)


if __name__ == '__main__':

    target_height = 1080
    target_width = 1920

    path = '../test_image/RAW/Sony_A74'
    tag = 'ARW'

    save_raw(path, target_height, target_width, tag)
