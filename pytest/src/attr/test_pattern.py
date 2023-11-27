from common import check_float_equal, check_name, analyze, resource_path, check_equal

def check_pattern(pattern, fillType, imageFileName, matrix = [1, 0, 0, 1, 0, 0]):
    assert pattern["fillType"] == fillType
    assert pattern["imageFileName"].endswith(imageFileName)
    for x, y in zip(pattern["matrix"], matrix):
        check_float_equal(x, y)
    assert pattern["imageTileMirrored"] == False

def check_pattern_fill(pattern, image_filename, rotation):
    check_equal(pattern['class'], 'patternImageFill')
    check_equal(pattern['rotation'], rotation)
    check_equal(pattern['imageFileName'], image_filename)

def check_pattern_fit(pattern, image_filename, rotation):
    check_equal(pattern['class'], 'patternImageFit')
    check_equal(pattern['rotation'], rotation)
    check_equal(pattern['imageFileName'], image_filename)

def check_pattern_tile(pattern, image_filename, rotation, scale, mirror = False, mode = 0):
    check_equal(pattern['class'], 'patternImageTile')
    check_equal(pattern['rotation'], rotation)
    check_equal(pattern['imageFileName'], image_filename)
    check_equal(pattern['scale'], scale)
    check_equal(pattern.get('mirror', False), mirror)
    check_equal(pattern.get('mode', 0), mode)

def check_pattern_crop(pattern, image_filename, matrix):
    check_equal(pattern['class'], 'patternImageStretch')
    check_equal(pattern['matrix'], matrix)
    check_equal(pattern['imageFileName'], image_filename)

def test_pattern():
    out = analyze(f'{resource_path}/attr/pattern.sketch')
    objs = out['frames'][0]['childObjects']

    obj = objs[0]
    check_name(obj, "fill_with_opacity")
    pattern = obj["style"]["fills"][0]["pattern"]["instance"]
    check_pattern_fill(pattern, "resources/853732577995ec08625706620b0235b0184b90e8.pdf", 0)
    check_float_equal(obj["style"]["fills"][0]["contextSettings"]["opacity"], 0.5)

    obj = objs[1]
    check_name(obj, 'fit')
    pattern = obj["style"]["fills"][0]["pattern"]["instance"]
    check_pattern_fit(pattern, "resources/853732577995ec08625706620b0235b0184b90e8.pdf", 0)

    obj = objs[2]
    check_name(obj, 'stretch')
    pattern = obj["style"]["fills"][0]["pattern"]["instance"]
    check_pattern_crop(pattern, "resources/853732577995ec08625706620b0235b0184b90e8.pdf", [1, 0, 0, 1, 0, 0])

    obj = objs[3]
    check_name(obj, 'tile')
    pattern = obj["style"]["fills"][0]["pattern"]["instance"]
    check_pattern_tile(pattern, "resources/853732577995ec08625706620b0235b0184b90e8.pdf", 0, 1)    

    obj = objs[4]
    check_name(obj, 'tile_scale')
    pattern = obj["style"]["fills"][0]["pattern"]["instance"]
    check_pattern_tile(pattern, "resources/853732577995ec08625706620b0235b0184b90e8.pdf", 0, 2)   

    obj = objs[5]
    check_name(obj, 'fill-img0')
    pattern = obj["style"]["fills"][0]["pattern"]["instance"]
    check_pattern_fill(pattern, "resources/6b8e9bb0b00a35e4939b3cdd31db5af8d1db8ab9.png", 0)    

    obj = objs[6]
    check_name(obj, 'fill_img1')
    pattern = obj["style"]["fills"][0]["pattern"]["instance"]
    check_pattern_fill(pattern, "resources/acf8882c350632bb72770bbf67ac7f5861fd067f.png", 0)    
