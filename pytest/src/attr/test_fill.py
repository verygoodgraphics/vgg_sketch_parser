from common import analyze, resource_path

def check_isEnabled(fill, value):
    assert fill["isEnabled"] == value

def check_fillType(fill, value):
    assert fill["fillType"] == value

def test_fill():
    out = analyze(f'{resource_path}/attr/fill.sketch')

    def get_objs(id):
        return out['artboard'][id]['layers'][0]['childObjects']

    def get_fill(obj, fill_id):
        return obj["style"]["fills"][fill_id]

    # isEnabled 
    objs = get_objs(0)
    assert not objs[1]["style"]["fills"]
    check_isEnabled(get_fill(objs[0], 0), True)
    check_isEnabled(get_fill(objs[0], 1), True)
    check_isEnabled(get_fill(objs[2], 0), True)
    check_isEnabled(get_fill(objs[3], 0), False)
    check_isEnabled(get_fill(objs[3], 1), True)
    check_isEnabled(get_fill(objs[3], 2), True)

    # fillType
    objs = get_objs(1)
    check_fillType(get_fill(objs[0], 0), 0)
    check_fillType(get_fill(objs[1], 0), 1)
    check_fillType(get_fill(objs[2], 0), 1)
    check_fillType(get_fill(objs[3], 0), 1)
    check_fillType(get_fill(objs[4], 0), 2)
    check_fillType(get_fill(objs[5], 0), 2)
    check_fillType(get_fill(objs[6], 0), 2)
    check_fillType(get_fill(objs[7], 0), 2)
    check_fillType(get_fill(objs[8], 0), 2)
    check_fillType(get_fill(objs[9], 0), 2)

    # context-setting
    objs = get_objs(2)
    assert get_fill(objs[0], 0)["contextSettings"]["blendMode"] == 4
    assert get_fill(objs[0], 1)["contextSettings"]["blendMode"] == 1
