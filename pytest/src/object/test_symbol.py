from common import create_collection, analyze, check_float_equal, resource_root
import src.attr.test_color as test_color
import src.object.test_mask as test_mask

def check_override_value(obj, index, object_id, override_name, override_value):
    attr = obj[index]
    if isinstance(attr['overrideValue'], float):
        check_float_equal(attr['overrideValue'], override_value)
    else:
        assert attr['overrideValue'] == override_value
    assert attr['objectId'] == object_id
    assert attr['overrideName'] == override_name

def check_color_override(obj, index, r, g, b, a, object_id, override_name):
    attr = obj[index]
    test_color.check_color(attr['overrideValue'], r, g, b, a)
    assert attr['objectId'] == object_id
    assert attr['overrideName'] == override_name

def test_symbol_master_num():
    out = analyze(f'{resource_root}/96/object/symbol/symbol_master_number.sketch')
    assert len(out['artboard'][0]['layers'][0]['childObjects']) == 3
    assert out['artboard'][0]['layers'][0]['childObjects'][0]['name'] == 'Master-Path'
    assert out['artboard'][0]['layers'][0]['childObjects'][1]['name'] == 'Master-Image'
    assert out['artboard'][0]['layers'][0]['childObjects'][2]['name'] == 'Master-Text'

def test_symbol_position():
    out = analyze(f'{resource_root}/96/object/symbol/symbol_position.sketch')
    assert len(out['artboard']) == 2

    objs = out['artboard'][0]['layers'][0]['childObjects']
    assert len(objs) == 4
    assert objs[0]['name'] == 'instance_in_artboard'
    assert objs[0]['class'] == 'symbolInstance'
    assert objs[1]['shape']['subshapes'][0]['subGeometry']['name'] == 'Master-Path'
    assert objs[1]['shape']['subshapes'][0]['subGeometry']['class'] == 'symbolInstance'
    assert objs[2]['childObjects'][0]['name'] == 'Master-Text'
    assert objs[2]['childObjects'][0]['class'] == 'symbolInstance'
    assert objs[3]['name'] == 'Master_Nest'
    assert objs[3]['class'] == 'symbolInstance'
    objs = out['artboard'][1]['layers'][0]['childObjects']
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
    check_override_value(obj, 7, 'FB099C6B-9E82-43D9-84EE-909F6D602498', 'style.fills.2.pattern.imageFileName',
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
    check_override_value(obj, 0, '95C02DB7-5EC9-4A61-97B1-0FFBE0C30E83', 'content', 'double change')
    check_override_value(obj, 1, 'FB099C6B-9E82-43D9-84EE-909F6D602498', 'style.fills.2.pattern.imageFileName', 'resources/4e3a88994fb339824d20deaa85ebf1bad7d7b46f.png')
    check_color_override(obj, 2, 0, 1, 0, 1, '31757F0E-A99C-447D-BE5C-BF84EC724F0B', 'style.fills.0.color')

    obj = out['page1_Master_Nest_change_text_content_and_symbol']['overrideValues']
    assert len(obj) == 6
    check_override_value(obj, 0, '95C02DB7-5EC9-4A61-97B1-0FFBE0C30E83', 'content', 'hello world1')
    check_override_value(obj, 1, 'F052DA2D-69E8-4F54-AB92-F19335E9486F', 'content', 'hello 2')
    check_override_value(obj, 2, '985FEC76-D8E5-4EBF-B119-F26E367F7BC6', 'content', 'hello 3')
    check_color_override(obj, 3, 0, 0, 1, 0.5, '31757F0E-A99C-447D-BE5C-BF84EC724F0B', 'style.fills.0.color')
    check_override_value(obj, 4, '651675C7-452D-48D3-A84A-A6CF6796E3B3', 'masterId', 'E6687A9D-5F3C-49F7-90BB-F19DBC9A956C')
    check_override_value(obj, 5, '95C02DB7-5EC9-4A61-97B1-0FFBE0C30E83', 'content', 'symbol-change')
    
    obj = out['page1_master_nest_change_symbol_and_image']['overrideValues']
    check_override_value(obj, 0, '651675C7-452D-48D3-A84A-A6CF6796E3B3', 'masterId', '87621494-BE66-4837-9F83-EE3D6390C1D6')
    check_override_value(obj, 1, 'A5C629DE-9983-48B6-94B6-3C08557C841F', 'imageFileName', 'resources/4e3a88994fb339824d20deaa85ebf1bad7d7b46f.png')

    obj = out['Master_Nest_change_fill_color_and_text_size_color']['overrideValues']
    assert len(obj) == 4
    check_override_value(obj, 0, '95C02DB7-5EC9-4A61-97B1-0FFBE0C30E83', 'attr.*.size', 30)
    check_color_override(obj, 1, 1, 0, 0, 1, 'FB099C6B-9E82-43D9-84EE-909F6D602498', 'style.fills.3.color')
    check_color_override(obj, 2, 1, 0, 0, 0.8, '95C02DB7-5EC9-4A61-97B1-0FFBE0C30E83', 'attr.*.fills.*.color')
    check_color_override(obj, 3, 0, 1, 0, 1, '31757F0E-A99C-447D-BE5C-BF84EC724F0B', 'style.fills.0.color')

    obj = out['page1_Master_Nest_Double_change_color']['overrideValues']
    check_color_override(obj, 0, 0, 1, 1, 1, '77077B65-EB8F-4D66-A533-44224D8720C7', 'style.fills.0.color')
    check_color_override(obj, 1, 1, 0, 1, 0.5, 'F052DA2D-69E8-4F54-AB92-F19335E9486F', 'attr.*.fills.*.color')
    check_color_override(obj, 2, 1, 0, 0, 1, '985FEC76-D8E5-4EBF-B119-F26E367F7BC6', 'attr.*.fills.*.color')

    obj = out['Master_Nest_Double_change_symbol_and_color']['overrideValues']
    check_override_value(obj, 0, '651675C7-452D-48D3-A84A-A6CF6796E3B3', 'masterId', 'E6687A9D-5F3C-49F7-90BB-F19DBC9A956C')
    check_color_override(obj, 1, 0, 0, 0, 1, '77077B65-EB8F-4D66-A533-44224D8720C7', 'style.fills.0.color')
    check_color_override(obj, 2, 1, 0, 1, 1, '95C02DB7-5EC9-4A61-97B1-0FFBE0C30E83', 'attr.*.fills.*.color')

    obj = out['page1_Master_Nest_Triple_change_symbol_and_color_and_text']['overrideValues']
    check_override_value(obj, 0, '0B8DCD1B-9BEA-4DC3-88B9-D6A0BC5DD9C3', 'masterId', '59B5C63E-4B01-4D1F-8D5B-459FE06E638C')
    check_color_override(obj, 1, 0, 0, 0, 1, 'FB099C6B-9E82-43D9-84EE-909F6D602498', 'style.fills.3.color')
    check_override_value(obj, 2, '95C02DB7-5EC9-4A61-97B1-0FFBE0C30E83', 'content', 'happy life')

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
    check_override_value(obj, 1, 'B65FED22-13D9-4EEF-9A1F-258D897A5602',
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
