from common import check_float_equal, check_name, analyze

def check_blur(blur, type, isEnabled, center, saturation, radius = None, motionAngle = None):
    assert blur['type'] == type
    assert blur['isEnabled'] == isEnabled
    
    assert len(blur['center']) == len(center)
    for x, y in zip(blur['center'], center):
        check_float_equal(x, y)

    assert blur['saturation'] == saturation

    if radius is not None:
        check_float_equal(blur['radius'], radius)
    else:
        assert 'radius' not in blur

    if motionAngle is not None:
        check_float_equal(blur['motionAngle'], motionAngle)
    else:
        assert 'motionAngle' not in blur    

def test_blur():
    out = analyze('pytest/resource/attr/blur.sketch')
    objs = out['artboard'][0]['layers'][0]['childObjects']

    def check(id, name, type, isEnabled, center, saturation, radius = None, motionAngle = None):
        check_name(objs[id], name)
        assert len(objs[id]['style']['blurs']) == 1
        check_blur(objs[id]['style']['blurs'][0], type, isEnabled, center, saturation, radius, motionAngle)

    # Gaussian
    check(0, 'a0', 0, True, [0.5, 0.5], 1.0, 10.0, 0.0)

    # Motion
    check(1, 'a1', 1, True, [0.5, 0.5], 1.0, 10.0, 71.11111111111111)

    # Zoom
    check(2, "a2", 2, True, [0.94326241134751809, 0.91200000000000014], 1, 10, 0)

    # Background
    check(3, "a3", 3, True, [0.5, 0.5], 1.530864197530864, 33, 0)

    # isEnabled is false
    check(4, "a4", 0, False, [0.5, 0.5], 1, 10, 0)
