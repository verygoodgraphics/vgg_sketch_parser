from common import check_float_equal, check_name, analyze

def check_pattern(pattern, fillType, imageFileName, matrix = [1, 0, 0, 1, 0, 0]):
    assert pattern["fillType"] == fillType
    assert pattern["imageFileName"].endswith(imageFileName)
    for x, y in zip(pattern["matrix"], matrix):
        check_float_equal(x, y)
    assert pattern["imageTileMirrored"] == False

def test_pattern():
    out = analyze('pytest/resource/attr/pattern.sketch')
    objs = out['artboard'][0]['layers'][0]['childObjects']

    obj = objs[0]
    check_name(obj, "fill_with_opacity")
    pattern = obj["style"]["fills"][0]["pattern"]["instance"]
    check_pattern(pattern, 1, "853732577995ec08625706620b0235b0184b90e8.pdf")
    check_float_equal(obj["style"]["fills"][0]["contextSettings"]["opacity"], 0.5)

    obj = objs[1]
    check_name(obj, 'fit')
    pattern = obj["style"]["fills"][0]["pattern"]["instance"]
    check_pattern(pattern, 3, "853732577995ec08625706620b0235b0184b90e8.pdf")

    obj = objs[2]
    check_name(obj, 'stretch')
    pattern = obj["style"]["fills"][0]["pattern"]["instance"]
    check_pattern(pattern, 2, "853732577995ec08625706620b0235b0184b90e8.pdf")

    obj = objs[3]
    check_name(obj, 'tile')
    pattern = obj["style"]["fills"][0]["pattern"]["instance"]
    check_pattern(pattern, 0, "853732577995ec08625706620b0235b0184b90e8.pdf")    

    obj = objs[4]
    check_name(obj, 'tile_scale')
    pattern = obj["style"]["fills"][0]["pattern"]["instance"]
    check_pattern(pattern, 0, "853732577995ec08625706620b0235b0184b90e8.pdf", [2, 0, 0, 2, 0, 0])        

    obj = objs[5]
    check_name(obj, 'fill-img0')
    pattern = obj["style"]["fills"][0]["pattern"]["instance"]
    check_pattern(pattern, 1, "6b8e9bb0b00a35e4939b3cdd31db5af8d1db8ab9.png")    

    obj = objs[6]
    check_name(obj, 'fill_img1')
    pattern = obj["style"]["fills"][0]["pattern"]["instance"]
    check_pattern(pattern, 1, "acf8882c350632bb72770bbf67ac7f5861fd067f.png")    
