from common import check_float_equal, analyze, resource_path
import src.attr.test_color as test_color

def check_isEnabled(border, value):
    assert border["isEnabled"] == value

def check_tickness(border, value):
    check_float_equal(border["thickness"], value)

def check_fillType(border, value):
    check_float_equal(border["fillType"], value)

def check_position(border, value):
    assert border["position"] == value

def check_style(border, value):
    assert border["style"] == value

def check_dashedPattern(border, value):
    assert len(border["dashedPattern"]) == len(value)
    for x, y in zip(border["dashedPattern"], value):
        check_float_equal(x, y)

def check_dashedOffset(border, value):
    check_float_equal(border["dashedOffset"], value)     

def check_lineJoinStyle(border, value):
    assert border["lineJoinStyle"] == value

def check_lineCapStyle(border, value):
    assert border["lineCapStyle"] == value

def check_MarkerType(border, start: bool, value):
    tmp = "startMarkerType" if start else "endMarkerType"
    assert border[tmp] == value

def test_border():
    out = analyze(f'{resource_path}/attr/border.sketch')

    def get_objs(id):
        return out['artboard'][id]['layers'][0]['childObjects']

    def get_border(obj, border_id):
        return obj["style"]["borders"][border_id]

    # isEnabled color thickness style(solid)
    objs = get_objs(0)
    assert not objs[0]["style"]["borders"]
    assert len(objs[1]["style"]["borders"]) == 2
    border = get_border(objs[1], 0)
    check_isEnabled(border, True)
    test_color.check_color(border["color"], 0.9710144927536232, 0.0189466242488512, 0.0189466242488512, 1)
    check_tickness(border, 10)
    border = get_border(objs[1], 1)
    check_isEnabled(border, True)
    test_color.check_color(border["color"], 0.06019794980558496, 0.9492753623188406, 0.3710641779570729, 1)
    check_tickness(border, 5)
    assert len(objs[2]["style"]["borders"]) == 2
    check_isEnabled(get_border(objs[2], 0), True)
    check_isEnabled(get_border(objs[2], 1), False)
    check_style(get_border(objs[2], 0), 0)
    
    # fillType gradient
    objs = get_objs(1)
    check_fillType(get_border(objs[0], 0), 0)
    check_fillType(get_border(objs[1], 0), 1)
    check_fillType(get_border(objs[2], 0), 1)
    check_fillType(get_border(objs[3], 0), 1)
    assert get_border(objs[1], 0)["gradient"]["instance"]["class"] == "gradientLinear"
    assert get_border(objs[2], 0)["gradient"]["instance"]["class"] == "gradientRadial"
    assert get_border(objs[3], 0)["gradient"]["instance"]["class"] == "gradientAngular"

    # border-position
    objs = get_objs(2)
    check_position(get_border(objs[0], 0), 1)
    check_position(get_border(objs[1], 0), 0)
    check_position(get_border(objs[2], 0), 2)

    # context-setting
    objs = get_objs(3)
    assert get_border(objs[0], 0)["contextSettings"]["blendMode"] == 3
    assert get_border(objs[0], 1)["contextSettings"]["blendMode"] == 1

    # style dashedPattern dashedOffset
    objs = get_objs(4)
    check_style(get_border(objs[0], 0), 2)
    check_dashedPattern(get_border(objs[0], 0), [10, 20])
    check_dashedOffset(get_border(objs[0], 0), 0)

    # lineJoinStyle
    objs = get_objs(5)
    check_lineJoinStyle(get_border(objs[0], 0), 0)
    check_lineJoinStyle(get_border(objs[1], 0), 1)
    check_lineJoinStyle(get_border(objs[2], 0), 2)

    # lineCapStyle
    objs = get_objs(6)
    check_lineCapStyle(get_border(objs[0], 0), 0)
    check_lineCapStyle(get_border(objs[1], 0), 1)
    check_lineCapStyle(get_border(objs[2], 0), 2)

    # startMarkerType endMarkerType
    objs = get_objs(7)
    check_MarkerType(get_border(objs[0], 0), True, 0)
    check_MarkerType(get_border(objs[1], 0), True, 4)
    check_MarkerType(get_border(objs[2], 0), True, 5)
    check_MarkerType(get_border(objs[3], 0), True, 7)
    check_MarkerType(get_border(objs[4], 0), True, 6)
    check_MarkerType(get_border(objs[5], 0), True, 1)
    check_MarkerType(get_border(objs[6], 0), True, 3)
    check_MarkerType(get_border(objs[7], 0), True, 2)
    check_MarkerType(get_border(objs[0], 0), False, 2)
    check_MarkerType(get_border(objs[1], 0), False, 2)
    check_MarkerType(get_border(objs[2], 0), False, 2)
    check_MarkerType(get_border(objs[3], 0), False, 2)
    check_MarkerType(get_border(objs[4], 0), False, 2)
    check_MarkerType(get_border(objs[5], 0), False, 2)
    check_MarkerType(get_border(objs[6], 0), False, 2)
    check_MarkerType(get_border(objs[7], 0), False, 2)
    check_MarkerType(get_border(objs[8], 0), True, 6)
    check_MarkerType(get_border(objs[8], 0), False, 6)
    check_MarkerType(get_border(objs[9], 0), True, 5)
    check_MarkerType(get_border(objs[9], 0), False, 5)    
    check_MarkerType(get_border(objs[10], 0), True, 0)
    check_MarkerType(get_border(objs[10], 0), False, 0)
    check_MarkerType(get_border(objs[11], 0), True, 2)
    check_MarkerType(get_border(objs[11], 0), False, 2)
    check_MarkerType(get_border(objs[12], 0), True, 1)
    check_MarkerType(get_border(objs[12], 0), False, 1)
    check_MarkerType(get_border(objs[13], 0), True, 7)
    check_MarkerType(get_border(objs[13], 0), False, 7)
    check_MarkerType(get_border(objs[14], 0), True, 3)
    check_MarkerType(get_border(objs[14], 0), False, 3)
    check_MarkerType(get_border(objs[15], 0), True, 4)
    check_MarkerType(get_border(objs[15], 0), False, 4)     
    objs = get_objs(8)
    check_MarkerType(get_border(objs[0], 0), True, 4)
    check_MarkerType(get_border(objs[0], 0), False, 5)
    check_MarkerType(get_border(objs[1], 0), True, 7)
    check_MarkerType(get_border(objs[1], 0), False, 2)
    check_MarkerType(get_border(objs[2], 0), True, 3)
    check_MarkerType(get_border(objs[2], 0), False, 6)

    # 备注: miterLimit flat 不测