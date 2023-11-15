from common import create_collection, analyze, check_float_equal, resource_root
import src.attr.test_color as test_color
import src.object.test_mask as test_mask

def check_override_value(obj, index, object_id, override_name, override_value = None):
    attr = obj[index]

    if override_value is not None:
        if isinstance(attr['overrideValue'], float):
            check_float_equal(attr['overrideValue'], override_value)
        else:
            assert attr['overrideValue'] == override_value

    if isinstance(object_id, list):
        assert attr['objectId'] == object_id
    else:
        assert len(attr['objectId']) == 1
        assert attr['objectId'][0] == object_id

    assert attr['overrideName'] == override_name

def check_color_override(obj, index, r, g, b, a, object_id, override_name):
    attr = obj[index]
    test_color.check_color(attr['overrideValue'], r, g, b, a)

    if isinstance(object_id, list):
        assert attr['objectId'] == object_id
    else:
        assert len(attr['objectId']) == 1
        assert attr['objectId'][0] == object_id

    assert attr['overrideName'] == override_name

def test_symbol_master_num():
    out = analyze(f'{resource_root}/96/object/symbol/symbol_master_number.sketch')
    assert len(out['frames'][0]['childObjects']) == 3
    assert out['frames'][0]['childObjects'][0]['name'] == 'Master-Path'
    assert out['frames'][0]['childObjects'][1]['name'] == 'Master-Image'
    assert out['frames'][0]['childObjects'][2]['name'] == 'Master-Text'

def test_symbol_position():
    out = analyze(f'{resource_root}/96/object/symbol/symbol_position.sketch')
    assert len(out['frames']) == 2

    objs = out['frames'][0]['childObjects']
    assert len(objs) == 4
    assert objs[0]['name'] == 'instance_in_artboard'
    assert objs[0]['class'] == 'symbolInstance'
    assert objs[1]['shape']['subshapes'][0]['subGeometry']['name'] == 'Master-Path'
    assert objs[1]['shape']['subshapes'][0]['subGeometry']['class'] == 'symbolInstance'
    assert objs[2]['childObjects'][0]['name'] == 'Master-Text'
    assert objs[2]['childObjects'][0]['class'] == 'symbolInstance'
    assert objs[3]['name'] == 'Master_Nest'
    assert objs[3]['class'] == 'symbolInstance'
    objs = out['frames'][1]['childObjects']
    assert len(objs) == 6
    assert objs[0]['name'] == 'Master-Path'
    assert objs[0]['class'] == 'symbolMaster'
    assert objs[1]['name'] == 'Master-Image'
    assert objs[1]['class'] == 'symbolMaster'
    assert objs[2]['name'] == 'Master-Text'
    assert objs[2]['class'] == 'symbolMaster'        
    assert objs[3]['class'] == 'path'        
    assert objs[4]['class'] == 'path'        
    assert objs[5]['name'] == 'Master_Nest'      
    assert objs[5]['class'] == 'symbolMaster'    
    assert objs[5]['childObjects'][0]['name'] == 'Master-Text'
    assert objs[5]['childObjects'][0]['class'] == 'symbolInstance'

def test_allow_override_off():
    out = create_collection(f'{resource_root}/96/object/symbol/allow_override_off.sketch')
    assert len(out['page1_master_path']['overrideValues']) == 0
    assert len(out['page1_master_image']['overrideValues']) == 0
    assert len(out['page1_master_text']['overrideValues']) == 0
    assert len(out['page1_Master_Nest_change_3_attr']['overrideValues']) == 0
    assert len(out['page1_Master_Nest_change_text_content_and_symbol']['overrideValues']) == 0
    assert len(out['page1_master_nest_change_symbol_and_image']['overrideValues']) == 0
    assert len(out['page1_Master_Nest_Double_change_color']['overrideValues']) == 0
    assert len(out['page1_Master_Nest_Triple_change_symbol_and_color_and_text']['overrideValues']) == 0
    assert len(out['Master_Nest_Text_change_3_attr']['overrideValues']) == 0
    assert len(out['Master_Nest_Path_change_3_attr']['overrideValues']) == 0
    assert len(out['Master_Nest_change_fill_color_and_text_size_color']['overrideValues']) == 0
    assert len(out['Master_Nest_Double_change_symbol_and_color']['overrideValues']) == 0

def test_allow_override_on_but_every_item_off():
    out = create_collection(f'{resource_root}/96/object/symbol/allow_override_on_but_every_item_off.sketch')
    assert len(out['page1_master_path']['overrideValues']) == 0
    assert len(out['page1_master_image']['overrideValues']) == 0
    assert len(out['page1_master_text']['overrideValues']) == 0
    assert len(out['page1_Master_Nest_change_3_attr']['overrideValues']) == 0
    assert len(out['page1_Master_Nest_change_text_content_and_symbol']['overrideValues']) == 0
    assert len(out['page1_master_nest_change_symbol_and_image']['overrideValues']) == 0
    assert len(out['page1_Master_Nest_Double_change_color']['overrideValues']) == 0
    assert len(out['page1_Master_Nest_Triple_change_symbol_and_color_and_text']['overrideValues']) == 0
    assert len(out['Master_Nest_Text_change_3_attr']['overrideValues']) == 0
    assert len(out['Master_Nest_Path_change_3_attr']['overrideValues']) == 0
    assert len(out['Master_Nest_change_fill_color_and_text_size_color']['overrideValues']) == 0
    assert len(out['Master_Nest_Double_change_symbol_and_color']['overrideValues']) == 0

def check_allow_override_on_and_every_item_on(file_name):
    out = create_collection(file_name)

    obj = out['page1_master_path']['overrideValues']
    assert len(obj) == 8
    check_color_override(obj, 0, 0.05882352941176471, 0.05882352941176471, 0.05882352941176471, 0.5,
                         'FB099C6B-9E82-43D9-84EE-909F6D602498', 'style.fills.3.color')
    check_color_override(obj, 1, 0.9411764705882353, 0.9411764705882353, 0.9411764705882353, 0.5,
                         'FB099C6B-9E82-43D9-84EE-909F6D602498', 'style.fills.0.color')
    check_color_override(obj, 2, 1, 0, 0, 1,
                         'FB099C6B-9E82-43D9-84EE-909F6D602498', 'style.borders.0.color')
    check_color_override(obj, 3, 0, 0, 0, 1,
                         'FB099C6B-9E82-43D9-84EE-909F6D602498', 'style.shadows.1.color')    
    check_color_override(obj, 4, 1, 1, 1, 0.5,
                         'FB099C6B-9E82-43D9-84EE-909F6D602498', 'style.shadows.0.color')    
    check_color_override(obj, 5, 1, 1, 0, 0.5,
                         'FB099C6B-9E82-43D9-84EE-909F6D602498', 'style.shadows.3.color')  
    check_color_override(obj, 6, 0, 0, 1, 0.5,
                         'FB099C6B-9E82-43D9-84EE-909F6D602498', 'style.shadows.2.color')  
    check_override_value(obj, 7, 'FB099C6B-9E82-43D9-84EE-909F6D602498', 'style.fills.2.pattern.instance.imageFileName',
                 'resources/4e3a88994fb339824d20deaa85ebf1bad7d7b46f.png')

    obj = out['page1_master_image']['overrideValues']
    assert len(obj) == 3
    check_color_override(obj, 0, 0.7333333333333333, 0.6666666666666666, 0.8, 0.7,
                         'A5C629DE-9983-48B6-94B6-3C08557C841F', 'style.fills.0.color')  
    check_color_override(obj, 1, 1, 0, 1, 0.5,
                         'A5C629DE-9983-48B6-94B6-3C08557C841F', 'style.borders.0.color')          
    check_override_value(obj, 2, 'A5C629DE-9983-48B6-94B6-3C08557C841F', 'imageFileName',
                 'resources/4e3a88994fb339824d20deaa85ebf1bad7d7b46f.png')

    obj = out['page1_master_text']['overrideValues']
    assert len(obj) == 10
    check_color_override(obj, 0, 0.9782608695652174, 0.01908801696712621, 0.01908801696712621, 0.5,
                         '95C02DB7-5EC9-4A61-97B1-0FFBE0C30E83', 'attr.*.fills.*.color')      
    check_override_value(obj, 1, '95C02DB7-5EC9-4A61-97B1-0FFBE0C30E83', 'attr.*.size', 50)
    check_override_value(obj, 2, '95C02DB7-5EC9-4A61-97B1-0FFBE0C30E83', 'attr.*.underline', 1)
    check_override_value(obj, 3, '95C02DB7-5EC9-4A61-97B1-0FFBE0C30E83', 'attr.*.linethrough', False)
    check_override_value(obj, 4, '95C02DB7-5EC9-4A61-97B1-0FFBE0C30E83', 'attr.*.horizontalAlignment', 2)
    check_override_value(obj, 5, '95C02DB7-5EC9-4A61-97B1-0FFBE0C30E83', 'content', 'new world')
    check_override_value(obj, 6, 'F052DA2D-69E8-4F54-AB92-F19335E9486F', 'attr.*.underline', 0)
    check_override_value(obj, 7, 'F052DA2D-69E8-4F54-AB92-F19335E9486F', 'attr.*.linethrough', True)
    check_override_value(obj, 8, '985FEC76-D8E5-4EBF-B119-F26E367F7BC6', 'attr.*.underline', 1)
    check_override_value(obj, 9, '985FEC76-D8E5-4EBF-B119-F26E367F7BC6', 'attr.*.linethrough', False)

    obj = out['Master_Nest_Path_change_3_attr']['overrideValues']
    assert len(obj) == 3
    check_color_override(obj, 0, 0, 0, 0, 0.5,
                         'FB099C6B-9E82-43D9-84EE-909F6D602498', 'style.fills.3.color')      
    check_color_override(obj, 1, 1, 0, 1, 1,
                         'FB099C6B-9E82-43D9-84EE-909F6D602498', 'style.borders.0.color')   
    check_color_override(obj, 2, 1, 1, 0, 1,
                         'FB099C6B-9E82-43D9-84EE-909F6D602498', 'style.shadows.0.color')   

    obj = out['Master_Nest_Text_change_3_attr']['overrideValues']
    assert len(obj) == 3
    check_override_value(obj, 0, '985FEC76-D8E5-4EBF-B119-F26E367F7BC6', 'content', 'b1b')
    check_override_value(obj, 1, 'F052DA2D-69E8-4F54-AB92-F19335E9486F', 'content', 'h123')
    check_override_value(obj, 2, '95C02DB7-5EC9-4A61-97B1-0FFBE0C30E83', 'content', 'abcdefg')

    obj = out['page1_Master_Nest_change_3_attr']['overrideValues']
    assert len(obj) == 3
    check_override_value(obj, 0, ['0B8DCD1B-9BEA-4DC3-88B9-D6A0BC5DD9C3', '95C02DB7-5EC9-4A61-97B1-0FFBE0C30E83'], 'content', 'double change')
    check_override_value(obj, 1, ['651675C7-452D-48D3-A84A-A6CF6796E3B3', 'FB099C6B-9E82-43D9-84EE-909F6D602498'], 'style.fills.2.pattern.instance.imageFileName', 'resources/4e3a88994fb339824d20deaa85ebf1bad7d7b46f.png')
    check_color_override(obj, 2, 0, 1, 0, 1, '31757F0E-A99C-447D-BE5C-BF84EC724F0B', 'style.fills.0.color')
   
    obj = out['page1_Master_Nest_change_text_content_and_symbol']['overrideValues']
    assert len(obj) == 6
    check_override_value(obj, 0, ['0B8DCD1B-9BEA-4DC3-88B9-D6A0BC5DD9C3', '95C02DB7-5EC9-4A61-97B1-0FFBE0C30E83'], 'content', 'hello world1')
    check_override_value(obj, 1, ['0B8DCD1B-9BEA-4DC3-88B9-D6A0BC5DD9C3', 'F052DA2D-69E8-4F54-AB92-F19335E9486F'], 'content', 'hello 2')
    check_override_value(obj, 2, ['0B8DCD1B-9BEA-4DC3-88B9-D6A0BC5DD9C3', '985FEC76-D8E5-4EBF-B119-F26E367F7BC6'], 'content', 'hello 3')
    check_color_override(obj, 3, 0, 0, 1, 0.5, '31757F0E-A99C-447D-BE5C-BF84EC724F0B', 'style.fills.0.color')
    check_override_value(obj, 4, '651675C7-452D-48D3-A84A-A6CF6796E3B3', 'masterId', 'E6687A9D-5F3C-49F7-90BB-F19DBC9A956C')
    check_override_value(obj, 5, ['651675C7-452D-48D3-A84A-A6CF6796E3B3', '95C02DB7-5EC9-4A61-97B1-0FFBE0C30E83'], 'content', 'symbol-change')
    
    obj = out['page1_master_nest_change_symbol_and_image']['overrideValues']
    check_override_value(obj, 0, '651675C7-452D-48D3-A84A-A6CF6796E3B3', 'masterId', '87621494-BE66-4837-9F83-EE3D6390C1D6')
    check_override_value(obj, 1, ['651675C7-452D-48D3-A84A-A6CF6796E3B3', 'A5C629DE-9983-48B6-94B6-3C08557C841F'], 'imageFileName', 'resources/4e3a88994fb339824d20deaa85ebf1bad7d7b46f.png')

    obj = out['Master_Nest_change_fill_color_and_text_size_color']['overrideValues']
    assert len(obj) == 4
    check_override_value(obj, 0, ['0B8DCD1B-9BEA-4DC3-88B9-D6A0BC5DD9C3', '95C02DB7-5EC9-4A61-97B1-0FFBE0C30E83'], 'attr.*.size', 30)
    check_color_override(obj, 1, 1, 0, 0, 1, ['651675C7-452D-48D3-A84A-A6CF6796E3B3', 'FB099C6B-9E82-43D9-84EE-909F6D602498'], 'style.fills.3.color')
    check_color_override(obj, 2, 1, 0, 0, 0.8, ['0B8DCD1B-9BEA-4DC3-88B9-D6A0BC5DD9C3', '95C02DB7-5EC9-4A61-97B1-0FFBE0C30E83'], 'attr.*.fills.*.color')
    check_color_override(obj, 3, 0, 1, 0, 1, '31757F0E-A99C-447D-BE5C-BF84EC724F0B', 'style.fills.0.color')

    obj = out['page1_Master_Nest_Double_change_color']['overrideValues']
    check_color_override(obj, 0, 0, 1, 1, 1, '77077B65-EB8F-4D66-A533-44224D8720C7', 'style.fills.0.color')
    check_color_override(obj, 1, 1, 0, 1, 0.5, ['D4568B7D-9CCF-4284-98AB-4E7D2A4C8DED', '0B8DCD1B-9BEA-4DC3-88B9-D6A0BC5DD9C3', 'F052DA2D-69E8-4F54-AB92-F19335E9486F'], 'attr.*.fills.*.color')
    check_color_override(obj, 2, 1, 0, 0, 1, ['D4568B7D-9CCF-4284-98AB-4E7D2A4C8DED', '0B8DCD1B-9BEA-4DC3-88B9-D6A0BC5DD9C3', '985FEC76-D8E5-4EBF-B119-F26E367F7BC6'], 'attr.*.fills.*.color')

    obj = out['Master_Nest_Double_change_symbol_and_color']['overrideValues']
    check_override_value(obj, 0, ['D4568B7D-9CCF-4284-98AB-4E7D2A4C8DED', '651675C7-452D-48D3-A84A-A6CF6796E3B3'], 'masterId', 'E6687A9D-5F3C-49F7-90BB-F19DBC9A956C')
    check_color_override(obj, 1, 0, 0, 0, 1, '77077B65-EB8F-4D66-A533-44224D8720C7', 'style.fills.0.color')
    check_color_override(obj, 2, 1, 0, 1, 1, ['D4568B7D-9CCF-4284-98AB-4E7D2A4C8DED', '651675C7-452D-48D3-A84A-A6CF6796E3B3', '95C02DB7-5EC9-4A61-97B1-0FFBE0C30E83'], 'attr.*.fills.*.color')

    obj = out['page1_Master_Nest_Triple_change_symbol_and_color_and_text']['overrideValues']
    check_override_value(obj, 0, ['74FC2703-A058-442A-941B-54990FADFAB5', 'D4568B7D-9CCF-4284-98AB-4E7D2A4C8DED', '0B8DCD1B-9BEA-4DC3-88B9-D6A0BC5DD9C3'], 'masterId', '59B5C63E-4B01-4D1F-8D5B-459FE06E638C')
    check_color_override(obj, 1, 0, 0, 0, 1, ['74FC2703-A058-442A-941B-54990FADFAB5', 'D4568B7D-9CCF-4284-98AB-4E7D2A4C8DED', '0B8DCD1B-9BEA-4DC3-88B9-D6A0BC5DD9C3', 'FB099C6B-9E82-43D9-84EE-909F6D602498'], 'style.fills.3.color')
    check_override_value(obj, 2, ['74FC2703-A058-442A-941B-54990FADFAB5', 'D4568B7D-9CCF-4284-98AB-4E7D2A4C8DED', '651675C7-452D-48D3-A84A-A6CF6796E3B3', '95C02DB7-5EC9-4A61-97B1-0FFBE0C30E83'], 'content', 'happy life')

def test_allow_override_on_and_every_item_on():
    check_allow_override_on_and_every_item_on(f'{resource_root}/96/object/symbol/allow_override_on_and_every_item_on.sketch')

def test_allow_override_on_and_every_item_on_only_change_organization_form():
    out = check_allow_override_on_and_every_item_on(f'{resource_root}/96/object/symbol/allow_override_on_and_every_item_on_only_change_organization_form.sketch')

def test_background():
    out = create_collection(f'{resource_root}/96/object/symbol/background.sketch')
    test_color.check_color(out['Master-Path']['style']['fills'][0]['color'], 0.5452103216684341, 0.7536231884057971, 0.3088016967126193, 1.0)
    check_override_value(out['page1_master_path']['overrideValues'], 0, '59B5C63E-4B01-4D1F-8D5B-459FE06E638C', 'style.fills', [])
    check_override_value(out['Master_Nest_Path_change_3_attr']['overrideValues'], 0, '59B5C63E-4B01-4D1F-8D5B-459FE06E638C', 'style.fills', [])
    check_override_value(out['page1_master_image']['overrideValues'], 0, '87621494-BE66-4837-9F83-EE3D6390C1D6', 'style.fills', [])

def test_symbol_mask():
    relation = test_mask.get_relation(f'{resource_root}/96/object/symbol/mask.sketch')
    test_mask.judge(relation, 'outline_mask', ['outline_mask'], [], 1)
    test_mask.judge(relation, 'be_masked', ['outline_mask'], [], 0)
    test_mask.judge(relation, 'outline_mask_in_master', ['outline_mask_in_master'], [], 1)
    test_mask.judge(relation, 'obj_in_master_be_masked', ['outline_mask_in_master'], [], 0)
    test_mask.judge(relation, 'Master-outline_mask', [], [], 0)
    test_mask.judge(relation, 'mask_stop_by_master', [], [], 0)
    test_mask.judge(relation, 'outline_mask_for_complex_test', ['outline_mask_for_complex_test'], [], 1)
    test_mask.judge(relation, 'alpha_mask_in_master', ['outline_mask_for_complex_test'], ['alpha_mask_in_master'], 2)
    test_mask.judge(relation, 'Rectangle_double_mask', ['outline_mask_for_complex_test'], ['alpha_mask_in_master'], 0)
    test_mask.judge(relation, 'Group_in_master', ['outline_mask_for_complex_test'], [], 0)
    test_mask.judge(relation, 'master_alpha_mask', [], [], 0)
    test_mask.judge(relation, 'Master-outline_mask_as_instance', [], [], 0)
    test_mask.judge(relation, 'normal_obj', [], [], 0)
    test_mask.judge(relation, 'master_nest', [], [], 0)

    test_mask.judge(relation, 'page1_instance_as_mask', ['page1_instance_as_mask'], [], 1)
    test_mask.judge(relation, 'page1_obj_be_masked', ['page1_instance_as_mask'], [], 0)
    test_mask.judge(relation, 'page1_instance_be_masked', ['page1_instance_as_mask'], [], 0)
    test_mask.judge(relation, 'page1_instance_ignore_mask', [], [], 0)

# 测试数据丢失
# def test_layer_style():
#     out = create_collection(f'{resource_root}/96/object/symbol/layerStyle.sketch')

#     obj = out['ins_rect_no_change']['overrideValues']
#     assert not obj

#     obj = out['ins_rect_change_two_color']['overrideValues']
#     assert len(obj) == 2 
#     check_color_override(obj, 0, 0, 0, 1, 0.5, ['6D67C953-AB4E-47DB-9CDC-30EE282D69B0'], 'style.fills.1.color')
#     check_color_override(obj, 1, 1, 0, 1, 0.5, ['6D67C953-AB4E-47DB-9CDC-30EE282D69B0'], 'style.fills.0.color')

#     obj = out['ins_round_rect_no_change']['overrideValues']
#     check_override_value(obj, 0, ['A081F952-2746-44ED-A442-2A6A5704489D'], 'style.fills', [])

#     obj = out['ins_round_rect_change_one_color']['overrideValues']
#     check_override_value(obj, 0, ['A081F952-2746-44ED-A442-2A6A5704489D'], 'style.fills', [])
#     check_color_override(obj, 1, 0, 0, 0, 0.5, ['B4874969-3965-4036-96C0-FF61C9038E17'], 'style.fills.0.color')

#     obj = out['ins_oval_change_layerStyle']['overrideValues']
#     check_override_value(obj, 0, ['8905A07C-C9F1-4DE1-AD7D-96FA184FB15B'], 'style', 'referenced_style_DBE84A11-1415-4356-9640-FDB6EBCE398B')

#     obj = out['ins_oval_change_layerStyle_fillcolor']['overrideValues']
#     check_override_value(obj, 0, ['8905A07C-C9F1-4DE1-AD7D-96FA184FB15B'], 'style', 'referenced_style_DBE84A11-1415-4356-9640-FDB6EBCE398B')
#     check_color_override(obj, 1, 0.43937787203959, 0.7971014492753623, 0.504418522446094, 0.5, ['8905A07C-C9F1-4DE1-AD7D-96FA184FB15B'], 'style.fills.1.color')

#     obj = out['ins_oval_change_layerStyle_contextSettings']['overrideValues']
#     check_override_value(obj, 0, ['8905A07C-C9F1-4DE1-AD7D-96FA184FB15B'], 'style', 'referenced_style_DBE84A11-1415-4356-9640-FDB6EBCE398B')

#     obj = out['ins_oval_change_layerStyle_contextSettings_fillcolor']['overrideValues']
#     check_override_value(obj, 0, ['8905A07C-C9F1-4DE1-AD7D-96FA184FB15B'], 'style', 'referenced_style_DBE84A11-1415-4356-9640-FDB6EBCE398B')
#     check_color_override(obj, 1, 0.2544361965358785, 0.855072463768116, 0.2880382254719476, 0.5, ['8905A07C-C9F1-4DE1-AD7D-96FA184FB15B'], 'style.fills.1.color')

#     obj = out['ins_star_no_change']['overrideValues']
#     assert not obj

#     obj = out['ins_star_change_layerStyle']['overrideValues']
#     assert not obj

# 测试数据丢失
# def test_foreign_layer_style():
#     out = create_collection(f'{resource_root}/96/object/symbol/foreign_layer_style.sketch')

#     obj = out['ins_change_layerStyle']['overrideValues']
#     check_override_value(obj, 0, ['603D9000-FE01-41F2-860D-A76FBD662A45'], 'style', 'referenced_style_0E4F4A2E-CAD7-41DA-9A6E-A513ECD2F063')

#     obj = out['ins_change_layerStyle_fillColor']['overrideValues']
#     check_override_value(obj, 0, ['603D9000-FE01-41F2-860D-A76FBD662A45'], 'style', 'referenced_style_0E4F4A2E-CAD7-41DA-9A6E-A513ECD2F063')
#     check_color_override(obj, 1, 0.1538461538461533, 0, 1, 1, ['603D9000-FE01-41F2-860D-A76FBD662A45'], 'style.fills.0.color')

# 测试数据丢失
# def test_text_style():
#     references = analyze(f'{resource_root}/96/object/symbol/text_style.sketch')['references']
#     out = create_collection(f'{resource_root}/96/object/symbol/text_style.sketch')
    
#     def check_text_attr_item(attr, attr_name, attr_value):
#         if isinstance(attr[attr_name], float):
#             check_float_equal(attr[attr_name], attr_value)
#         else:
#             assert attr[attr_name] == attr_value

#     obj = out['ins_abc_no_change']['overrideValues']
#     assert not obj

#     obj = out['ins_abc_change_textStyle']['overrideValues']
#     assert len(obj) == 2
#     check_override_value(obj, 0, ['54502DF5-31AF-43BF-873B-22C42B577FAB'], 'style', 'referenced_style_D100B848-557E-4EED-B127-BAE5792E2E16')
#     assert references[1]['id'] == 'D100B848-557E-4EED-B127-BAE5792E2E16'
#     attr = references[1]['fontAttr']
#     test_color.check_color(attr['fills'][0]['color'], 0.3373180372119927, 0.0505478967833157, 0.9420289855072463, 1.0)
#     check_text_attr_item(attr, 'fillUseType', 1)
#     check_text_attr_item(attr, 'horizontalAlignment', 2)
#     check_text_attr_item(attr, 'name', 'InaiMathi-Bold')
#     check_text_attr_item(attr, 'size', 72)
#     check_text_attr_item(attr, 'kerning', False)
#     check_text_attr_item(attr, 'letterSpacingValue', 73.5)
#     check_text_attr_item(attr, 'lineSpaceValue', 19)
#     check_text_attr_item(attr, 'underline', 0)
#     check_text_attr_item(attr, 'linethrough', True)
#     check_text_attr_item(attr, 'textCase', 1)
#     check_text_attr_item(attr, 'baselineShift', -1)
#     check_text_attr_item(attr, 'baseline', 0)
#     check_float_equal(attr['textParagraph']['paragraphSpacing'], 55)
#     check_override_value(obj, 1, ['54502DF5-31AF-43BF-873B-22C42B577FAB'], 'verticalAlignment', 1)

#     obj = out['ins_abc_change_textStyle_other_attr']['overrideValues']
#     check_override_value(obj, 0, ['54502DF5-31AF-43BF-873B-22C42B577FAB'], 'style', 'referenced_style_D100B848-557E-4EED-B127-BAE5792E2E16')
#     check_override_value(obj, 1, ['54502DF5-31AF-43BF-873B-22C42B577FAB'], 'verticalAlignment', 1)
#     check_color_override(obj, 2, 0.0505478967833157, 0.9420289855072463, 0.06301616375847571, 1, '54502DF5-31AF-43BF-873B-22C42B577FAB', 'attr.*.fills.*.color')
#     check_override_value(obj, 3, ['54502DF5-31AF-43BF-873B-22C42B577FAB'], 'attr.*.underline', 1)
#     check_override_value(obj, 4, ['54502DF5-31AF-43BF-873B-22C42B577FAB'], 'attr.*.linethrough', False)
#     check_override_value(obj, 5, ['54502DF5-31AF-43BF-873B-22C42B577FAB'], 'attr.*.horizontalAlignment', 1)
#     check_override_value(obj, 6, ['54502DF5-31AF-43BF-873B-22C42B577FAB'], 'attr.*.size', 50)

#     obj = out['ins_happy_change_textStyle']['overrideValues']
#     assert not obj

#     obj = out['ins_xyz_change_textStyle']['overrideValues']
#     assert len(obj) == 2
#     check_override_value(obj, 0, ['0008559D-E572-4DBA-8843-7109BC3AF7D8'], 'style', 'referenced_style_29FFAB7B-6F48-4279-B9A9-827484FA4E77')
#     assert references[3]['id'] == '29FFAB7B-6F48-4279-B9A9-827484FA4E77'
#     attr = references[3]['fontAttr']
#     check_text_attr_item(attr, 'underline', 1)
#     check_text_attr_item(attr, 'linethrough', False)
#     check_text_attr_item(attr, 'baseline', 2)
#     check_override_value(obj, 1, ['0008559D-E572-4DBA-8843-7109BC3AF7D8'], 'verticalAlignment', 1)

# 测试数据丢失
# def test_foreign_text_style():
#     out = create_collection(f'{resource_root}/96/object/symbol/foreign_text_style.sketch')
    
#     obj = out['ins_abc_no_change']['overrideValues']
#     assert not obj

#     obj = out['ins_abc_change_textStyle']['overrideValues']
#     check_override_value(obj, 0, ['7AED76A8-8047-470F-8AE8-E255509B3669'], 'style', 'referenced_style_8F151F36-743D-4005-A040-939720B272B6')
#     check_override_value(obj, 1, ['7AED76A8-8047-470F-8AE8-E255509B3669'], 'verticalAlignment', 0)

#     obj = out['ins_abc_change_textStyle_color']['overrideValues']
#     check_override_value(obj, 0, ['7AED76A8-8047-470F-8AE8-E255509B3669'], 'style', 'referenced_style_8F151F36-743D-4005-A040-939720B272B6')
#     check_override_value(obj, 1, ['7AED76A8-8047-470F-8AE8-E255509B3669'], 'verticalAlignment', 0)
#     check_color_override(obj, 2, 0.9710144927536232, 0, 0, 1, '7AED76A8-8047-470F-8AE8-E255509B3669', 'attr.*.fills.*.color')

#     obj = out['ins_xyz_change_textStyle']['overrideValues']
#     assert not obj

# 测试数据丢失
# def test_group_tint():
#     out = create_collection(f'{resource_root}/96/object/symbol/group_tint.sketch')
    
#     obj = out['ins_a_change_group_tint_shadow_color']['overrideValues']
#     assert len(obj) == 2
#     check_override_value(obj, 0, ['8AF9B558-6B13-4551-A16B-9A740182869E'], 'style.fills', None)
#     test_color.check_color(obj[0]['overrideValue'][0]['color'], 0.9565217391304348, 0.8847377436985073, 0.02332979851537648, 1)
#     check_color_override(obj, 1, 0.8188405797101449, 0.2486549152508856, 0.223683280311064, 1, 
#                          '8AF9B558-6B13-4551-A16B-9A740182869E', 'style.shadows.0.color')    

#     obj = out['ins_b_change_group_tint_shadow_color']['overrideValues']
#     assert len(obj) == 1
#     check_color_override(obj, 0, 0.02797202797202836, 0, 1, 1, 
#                          '18312955-5762-4B3B-BFEC-3DD8972C9E5E', 'style.shadows.0.color')    


def test_88_1_allow_override_on_and_every_item_on():
    out = create_collection(f'{resource_root}/88_1/object/symbol/allow_override_on_and_every_item_on.sketch')

    obj = out['master_nest_image_change_image']['overrideValues']
    assert len(obj) == 1
    check_override_value(obj, 0, '57C16C94-6CE8-4AEE-A464-5CB35902151B',
                         'imageFileName', 
                         'resources/55694ff666106cc599ea92740298541e33a1e249.png')
    
    obj = out['page1_path']['overrideValues']
    assert len(obj) == 0
    assert len(out['page1_path']['style']['shadows']) == 1
    
    obj = out['page1_image']['overrideValues']
    assert len(obj) == 1
    check_override_value(obj, 0, '57C16C94-6CE8-4AEE-A464-5CB35902151B',
                         'imageFileName', 
                         'resources/ccac3ee91b4909f6218af0a21d546c4114ba96e3.png')

    obj = out['page1_text']['overrideValues']
    assert len(obj) == 1
    check_override_value(obj, 0, 'B65FED22-13D9-4EEF-9A1F-258D897A5602',
                         'content', 
                         'new_content')
    assert len(out['page1_text']['style']['fills']) == 1

    obj = out['page1_master_nest']['overrideValues']
    assert len(obj) == 2
    check_override_value(obj, 0, 'F0E0CB99-09A6-4299-BD9C-289BD68D9D78',
                         'masterId', 
                         'A2DBFC7E-97AF-411F-B3F8-2C6DBCAF24F2')    
    check_override_value(obj, 1, ['F0E0CB99-09A6-4299-BD9C-289BD68D9D78', 'B65FED22-13D9-4EEF-9A1F-258D897A5602'],
                         'content', 
                         'double')  
    
def test_88_1_allow_override_on_but_every_item_off():
    out = create_collection(f'{resource_root}/88_1/object/symbol/allow_override_on_but_every_item_off.sketch')
    assert len(out['master_nest_image_change_image']['overrideValues']) == 0
    assert len(out['page1_path']['overrideValues']) == 0
    assert len(out['page1_image']['overrideValues']) == 0
    assert len(out['page1_text']['overrideValues']) == 0
    assert len(out['page1_master_nest']['overrideValues']) == 0

def test_88_1_allow_override_off():
    out = create_collection(f'{resource_root}/88_1/object/symbol/allow_override_off.sketch')
    assert len(out['master_nest_image_change_image']['overrideValues']) == 0
    assert len(out['page1_path']['overrideValues']) == 0
    assert len(out['page1_image']['overrideValues']) == 0
    assert len(out['page1_text']['overrideValues']) == 0
    assert len(out['page1_master_nest']['overrideValues']) == 0
