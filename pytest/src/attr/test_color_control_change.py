from common import check_float_equal, analyze, resource_path

def check_color_control_change(attr, isEnabled, brightness, contrast, hue, saturation):
    assert attr["isEnabled"] == isEnabled
    check_float_equal(attr["brightness"], brightness)
    check_float_equal(attr["contrast"], contrast)
    check_float_equal(attr["hue"], hue)
    check_float_equal(attr["saturation"], saturation)

def test_color_control_change():
    out = analyze(f'{resource_path}/attr/color_control_change.sketch')
    objs = out['artboard'][0]['layers'][0]['childObjects']    

    check_color_control_change(objs[0]["imageAdjust"]["instance"],
                               True, 
                               -0.379746835443038, 
                               2.89873417721519, 
                               1.193009868451821, 
                               1.379746835443038)
    check_color_control_change(objs[1]["imageAdjust"]["instance"],
                               True, 0, 1, 0, 1)
