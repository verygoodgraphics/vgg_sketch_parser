from common import check_float_equal, check_name, analyze, resource_path
import src.attr.test_color as test_color
import src.attr.test_context_settings as test_context_settings

def check_shadow_basic(shadow, isEnabled, offsetX, offsetY, blur, spread, inner, showBehindTransparentAreas = None):
    '''
    检查除了 color contextSettings 之外的属性
    '''
    assert shadow["isEnabled"] == isEnabled
    check_float_equal(shadow["offsetX"], offsetX)
    check_float_equal(shadow["offsetY"], offsetY)
    check_float_equal(shadow["blur"], blur)
    check_float_equal(shadow["spread"], spread)
    assert shadow["inner"] == inner
    if showBehindTransparentAreas is not None:
        assert shadow["showBehindTransparentAreas"] == showBehindTransparentAreas
    else:
        assert 'showBehindTransparentAreas' not in shadow

def test_shadow():
    out = analyze(f'{resource_path}/attr/shadow.sketch')
    objs = out['frames'][0]['childObjects']

    # zero shadow
    obj = objs[0]
    check_name(obj, 'zero')
    assert not obj["style"]["shadows"]

    # one shadow
    obj = objs[1]
    check_name(obj, 'one')
    check_shadow_basic(obj["style"]["shadows"][0], True, 10.1, 20.1, 10, -1.5, False)
    test_color.check_color(obj["style"]["shadows"][0]["color"], 0, 0, 0, 0.5)
    
    # two shadow
    obj = objs[2]
    check_name(obj, 'two')
    check_shadow_basic(obj["style"]["shadows"][0], True, 10, -20, 10.2, 11.5, False)
    test_color.check_color(obj["style"]["shadows"][0]["color"], 0.2500327526838662, 0.1009544008483563, 0.8623188405797102, 0.5)
    check_shadow_basic(obj["style"]["shadows"][1], True, 5, 20, 40, 30.1, False)
    test_color.check_color(obj["style"]["shadows"][1]["color"], 0.5447184134352745, 0.8623188405797102, 0.1177801343230824, 0.5)
    
    # three shadow with disable
    obj = objs[3]
    assert len(obj["style"]["shadows"]) == 3
    assert obj["style"]["shadows"][0]["isEnabled"]
    assert not obj["style"]["shadows"][1]["isEnabled"]
    assert obj["style"]["shadows"][2]["isEnabled"]

    # context-setting
    obj = objs[4]
    check_name(obj, "context-settings")
    test_context_settings.check_context_settings(obj["style"]["shadows"][0]["contextSettings"], 1, 1.0)
    test_context_settings.check_context_settings(obj["style"]["shadows"][1]["contextSettings"], 3, 1.0)

def test_inner_shadow():
    out = analyze(f'{resource_path}/attr/shadow_inner.sketch')
    objs = out['frames'][0]['childObjects']

    # zero shadow
    obj = objs[0]
    check_name(obj, 'zero')
    assert not obj["style"]["shadows"]

    def judge(shadow, size):
        assert len(shadow) == size
        for item in shadow:
            assert item["inner"] == True

    # one shadow
    obj = objs[1]
    check_name(obj, 'one')
    judge(obj["style"]["shadows"], 1)
    
    # two shadow
    obj = objs[2]
    check_name(obj, 'two')
    judge(obj["style"]["shadows"], 2)        

    # three shadow with disable
    obj = objs[3]
    check_name(obj, 'three with disable')
    judge(obj["style"]["shadows"], 3)  

    # context-setting
    obj = objs[4]
    check_name(obj, 'context-setting')
    judge(obj["style"]["shadows"], 2)  