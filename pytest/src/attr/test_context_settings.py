from common import check_float_equal, check_name, analyze, resource_path

def check_context_settings(cs, blendMode, opacity, isolateBlending = False, transparencyKnockoutGroup = 0):
    assert cs["blendMode"] == blendMode
    check_float_equal(cs["opacity"], opacity)
    assert cs["isolateBlending"] == isolateBlending
    assert cs["transparencyKnockoutGroup"] == transparencyKnockoutGroup

def test_context_settings():
    out = analyze(f'{resource_path}/attr/context_settings.sketch')
    objs = out['frames'][0]['childObjects']

    def get(obj):
        return (obj["style"]["fills"][0]["contextSettings"],
                obj["style"]["borders"][0]["contextSettings"],
                obj["contextSettings"])

    # fill 和 border 上的 context-setting
    obj = objs[0]
    fill_cs, border_cs, global_cs = get(obj)
    check_name(obj, '0_fill_border')
    check_context_settings(fill_cs, 6, 1.0)
    check_context_settings(border_cs, 4, 1.0)
    check_context_settings(global_cs, 0, 1.0)

    # 全局的 context-setting
    obj = objs[1]
    fill_cs, border_cs, global_cs = get(obj)
    check_name(obj, '1_only_global')
    check_context_settings(fill_cs, 0, 1.0)
    check_context_settings(border_cs, 0, 1.0)
    check_context_settings(global_cs, 3, 0.5)

    # fill 和 border 以及 global 上的 context-setting
    obj = objs[2]
    fill_cs, border_cs, global_cs = get(obj)
    check_name(obj, '2_both')
    check_context_settings(fill_cs, 6, 1.0)
    check_context_settings(border_cs, 4, 1.0)
    check_context_settings(global_cs, 3, 1)

    # blend-mode
    objs = out['frames'][1]['childObjects']
    assert len(objs) == 18
    for x, y in zip(objs, range(len(objs))):
        assert x["contextSettings"]["blendMode"] == y
