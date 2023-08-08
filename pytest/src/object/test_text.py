from common import create_collection, check_float_equal, resource_path
import src.attr.test_fill as test_fill
import src.attr.test_color as test_color
import src.attr.test_context_settings as test_context_settings

def test_text():
    relation = create_collection(f'{resource_path}/object/text.sketch')

    def check_attr_item(name, attr_name, attr_value):
        obj = relation[name]
        assert len(obj['attr']) == len(attr_value)
        for x, y in zip(obj['attr'], attr_value):
            if isinstance(y, float):
                check_float_equal(x[attr_name], y)
            else:
                assert x[attr_name] == y

    assert relation["a0_content"]["content"] == "莱纳👨‍👩‍👧‍👦🌈🏳️‍🌈123"
    check_attr_item('a0_content', 'length', [2, 19, 3])

    check_attr_item('a1_font_size', 'size', [20, 60])
    check_attr_item('a1_font_size', 'length', [4, 5])

    check_attr_item('a2_font_name', 'name', ['Trattatello', 'PingFangSC-Regular'])
    check_attr_item('a2_font_name', 'length', [4, 5])

    check_attr_item('a3_font_subname', 'name', ['PingFangSC-Regular', 'PingFangSC-Semibold'])
    check_attr_item('a3_font_subname', 'length', [4, 5])

    fill_0 = relation['a4_font_color']['attr'][0]['fills'][0]
    fill_1 = relation['a4_font_color']['attr'][1]['fills'][0]
    test_fill.check_isEnabled(fill_0, True)
    test_fill.check_isEnabled(fill_1, True)
    test_fill.check_fillType(fill_0, 0)
    test_fill.check_fillType(fill_1, 0)
    test_color.check_color(fill_0["color"], 0.04358430540827151, 0.09668468682254507, 0.9927536231884058, 0.5)
    test_color.check_color(fill_1["color"], 0.9565217391304348, 0, 0, 1)
    check_attr_item('a4_font_color', 'length', [4, 5])    

    check_attr_item('a5_character', 'letterSpacing', [50, 0])
    check_attr_item('a5_character', 'length', [4, 5])

    check_attr_item('a6_line', 'lineSpace', [50, 100, 200])
    check_attr_item('a6_line', 'length', [11, 11, 21])

    def check_paragraphSpacing(name, attr_value):
        obj = relation[name]
        assert len(obj['attr']) == len(attr_value)
        for x, y in zip(obj['attr'], attr_value):
            check_float_equal(x["textParagraph"]["paragraphSpacing"], y)
    check_paragraphSpacing('a7_paragraph', [0, 50, 100, 0])
    check_attr_item('a7_paragraph', 'length', [11, 11, 11, 10])

    assert relation["a8_auto_width"]["frameMode"] == 1
    assert relation["a9_auto_height"]["frameMode"] == 2
    assert relation["a10_fixsize"]["frameMode"] == 0

    check_attr_item('a11_align_h', 'horizontalAlignment', [0, 2, 1, 3])
    check_attr_item('a11_align_h', 'length', [11, 11, 11, 10])    
    
    assert relation["a12_align_v"]["verticalAlignment"] == 0
    assert relation["a13_align_v"]["verticalAlignment"] == 1
    assert relation["a14_align_v"]["verticalAlignment"] == 2

    def check_obj_style_fill_type(name, fill_type):
        obj = relation[name]
        assert len(obj['style']['fills']) == 1
        fill = obj['style']['fills'][0]
        test_fill.check_isEnabled(fill, True)
        test_fill.check_fillType(fill, fill_type)
    check_obj_style_fill_type('a15_fill', 1)
    check_obj_style_fill_type('a16_fill', 1)
    check_obj_style_fill_type('a_17_fill', 1)
    check_obj_style_fill_type('a_18_fill', 0)
    check_obj_style_fill_type('a_19_fill_pattern', 2)
    check_obj_style_fill_type('a_20_fill_pattern', 2)

    check_obj_style_fill_type('szn_tmp_test_0', 1)
    test_context_settings.check_context_settings(relation['szn_tmp_test_0']["attr"][0]["fills"][0]["contextSettings"], 0, 1)
    
    obj = relation['szn_tmp_test_1']
    check_attr_item('szn_tmp_test_1', 'length', [4, 5])
    assert len(obj['attr'][0]["fills"]) == 1
    assert len(obj['style']['fills']) == 2

    fill_0 = obj['style']['fills'][0]
    fill_1 = obj['style']['fills'][1]
    test_fill.check_isEnabled(fill_0, True)
    test_fill.check_isEnabled(fill_1, False)
    test_color.check_color(fill_0['color'], 0.4312062628075353, 0.05259809119830333, 0.8985507246376812, 0.5)
    test_color.check_color(fill_1['color'], 0.9130434782608695, 0.02226935312831391, 0.02226935312831391, 0.01)
    assert len(obj['attr'][0]["fills"]) == 1
    assert len(obj['attr'][1]["fills"]) == 1
    fill_0 = obj['attr'][0]['fills'][0]
    fill_1 = obj['attr'][1]['fills'][0]
    test_fill.check_isEnabled(fill_0, True)
    test_fill.check_isEnabled(fill_1, True)
    test_color.check_color(fill_0['color'], 0.9565217391304348, 0, 0, 0.5)
    test_color.check_color(fill_1['color'], 0.9565217391304348, 0, 0, 1)

    obj = relation['szn_tmp_test_2']
    check_attr_item('szn_tmp_test_2', 'length', [4, 5])
    assert len(obj['style']["fills"]) == 2
    fill_0 = obj['style']['fills'][0]
    fill_1 = obj['style']['fills'][1]
    test_fill.check_isEnabled(fill_0, True)
    test_fill.check_isEnabled(fill_1, True)
    test_color.check_color(fill_0['color'], 0.4312062628075353, 0.05259809119830333, 0.8985507246376812, 0.5)
    test_color.check_color(fill_1['color'], 1, 0.02926829268292686, 0.02926829268292686, 0.1)
    assert len(obj['attr'][0]["fills"]) == 1
    assert len(obj['attr'][1]["fills"]) == 1
    fill_0 = obj['attr'][0]['fills'][0]
    fill_1 = obj['attr'][1]['fills'][0]
    test_fill.check_isEnabled(fill_0, True)
    test_fill.check_isEnabled(fill_1, True)
    test_color.check_color(fill_0['color'], 0.9565217391304348, 0, 0, 0.5)
    test_color.check_color(fill_1['color'], 0.9565217391304348, 0, 0, 1)

    obj = relation['a_21_multiple_fill_blend_mode']
    test_color.check_color(obj['style']['fills'][0]["color"], 0.9492753623188406, 0.0324142806645458, 0.0324142806645458, 0.5)
    test_color.check_color(obj['style']['fills'][1]["color"], 0.2372245993666991, 0.05326970661010962, 0.9927536231884058, 0.6)
    test_context_settings.check_context_settings(obj['style']['fills'][0]["contextSettings"], 0, 1)
    test_context_settings.check_context_settings(obj['style']['fills'][1]["contextSettings"], 2, 1)

    obj = relation['a_22_border']
    assert len(obj['style']['borders']) == 1

    check_attr_item('a_23_menu_bold', 'name', ['PingFangSC-Semibold', 'PingFangSC-Regular'])
    check_attr_item('a_23_menu_bold', 'length', [4, 5])

    check_attr_item('a_24_underline', 'underline', [1, 0])
    check_attr_item('a_23_menu_bold', 'length', [4, 5])

    assert('textOnPath' in relation['a_25_on_path'])

    # 备注: a26 ~ a29, kering 会影响 letterSpacing, 该结论来自手动验证

    # 备注: a30 ~ a32, ligature 不同值几乎没区别, 目前也未解析该字段

    check_attr_item('b0_default', 'baseline', [0])

    check_attr_item('b1_sup', 'baseline', [1, 0])
    check_attr_item('b1_sup', 'length', [4, 5])
    
    check_attr_item('b2_sub', 'baseline', [2, 0])
    check_attr_item('b2_sub', 'length', [4, 5])

    check_attr_item('b3_raise', 'baselineShift', [5, 0])
    check_attr_item('b3_raise', 'length', [4, 5])

    check_attr_item('b4_down', 'baselineShift', [-4, 0])
    check_attr_item('b4_down', 'length', [4, 5])    

    check_attr_item('a_33_upper', 'lettertransform', [2, 0])
    check_attr_item('a_33_upper', 'length', [4, 5])      

    check_attr_item('a_34_lower', 'lettertransform', [3, 0])
    check_attr_item('a_34_lower', 'length', [4, 5])   

    assert relation["a_35_list_type"]["content"] == "\t1.\tsomething1\n\t2.\tsomething2\n\t3.\tsomething3\n\t4.\tsomething4"
    assert relation["a_36_list_type"]["content"] == "\t• \tsomething1\n\t• \tsomething2\n\t• \tsomething3\n\t• \tsomething4"

    check_attr_item('a_38_through', 'linethrough', [True, False])
    check_attr_item('b3_raise', 'length', [4, 5])

    check_attr_item('a_39_not_menu_change', 'lettertransform', [2, 0])
    check_attr_item('a_39_not_menu_change', 'length', [4, 5])

    check_attr_item('a_40_not_menu_change', 'lettertransform', [3, 0])
    check_attr_item('a_40_not_menu_change', 'length', [4, 5])
