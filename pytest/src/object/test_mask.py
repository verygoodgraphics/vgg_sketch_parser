from common import analyze, resource_path

def collection_info(obj, out):
    '''
    要求输入的 name 是唯一的

    :return dict[name: (id, outline_mask, alphamask, maskType)]
    '''

    assert obj["name"] not in out

    if obj["class"] != "frame":
        out[obj['name']] = (obj["id"], obj["outlineMaskBy"], obj["alphaMaskBy"], obj["maskType"])

    try:
        for item in obj['childObjects']:
            collection_info(item, out)
    except:
        pass
    
    try:
        for item in obj["shape"]["subshapes"]:
            if item["subGeometry"]["class"] != "contour":
                collection_info(item["subGeometry"], out)
    except:
        pass

def judge(relation, name, outline_mask_name = [], alpha_mask_name = [], maskType = 0):
    assert name in relation

    obj_self = relation[name]
    assert obj_self[3] == maskType

    obj_self_outline_mask = obj_self[1]
    assert len(obj_self_outline_mask) == len(outline_mask_name)
    for item in outline_mask_name:
        assert relation[item][0] in obj_self_outline_mask

    obj_self_alpha_mask = obj_self[2]
    assert len(obj_self_alpha_mask) == len(alpha_mask_name)
    for item in alpha_mask_name:
        is_find = False
        for tmp in obj_self_alpha_mask:
            if relation[item][0] == tmp["id"]:
                is_find = True
                assert tmp["crop"]
                assert not tmp["alphaType"]
                break
        assert is_find

def get_relation(file_name):
    out = analyze(file_name)

    relation = {}
    for frame in out["frames"]:
        collection_info(frame, relation)
    
    return relation

def test_outline_mask():
    relation = get_relation(f'{resource_path}/object/mask/outline_mask.sketch')

    judge(relation, 'a0', ['a0'], [], 1)
    judge(relation, 'a1', ['a0'])
    
    judge(relation, 'a2', ['a2'], [], 1)
    judge(relation, 'a3', ['a2'])
    judge(relation, 'a4', ['a4'], [], 1)
    judge(relation, 'a5', ['a4'])

    judge(relation, 'a6_stop_by_artboard')

    judge(relation, 'a7')
    judge(relation, 'a8', ['a8'], [], 1)
    judge(relation, 'a9_ignore_mask')

    judge(relation, 'a10_mask_in_group')
    judge(relation, 'a11', ['a11'], [], 1)
    judge(relation, 'a12', ['a11'])
    judge(relation, 'a13')

def test_alpha_mask():
    relation = get_relation(f'{resource_path}/object/mask/alpha_mask.sketch')

    judge(relation, 'a0_alpha_mask', [], ['a0_alpha_mask'], 2)
    judge(relation, 'a1', [], ['a0_alpha_mask'])
    judge(relation, 'a2_outline_mask', ['a2_outline_mask'], [], 1)
    judge(relation, 'a3', ['a2_outline_mask'], [])
    judge(relation, 'a4_alpha_mask', [], ['a4_alpha_mask'], 2)
    judge(relation, 'a5_ignore mask')
    judge(relation, 'a11_alpha_mask', [], ['a11_alpha_mask'], 2)

    judge(relation, 'a6_alpha_mask', [], ['a6_alpha_mask'], 2)
    judge(relation, 'a7', [], ['a6_alpha_mask'])
    judge(relation, 'a8')
    judge(relation, 'a9_out_of_group')

    judge(relation, 'a10_stop_by_artboard')

def test_compound_mask():
    relation = get_relation(f'{resource_path}/object/mask/compound_mask.sketch')

    judge(relation, 'a0_outline_mask', ['a0_outline_mask'], [], 1)

    judge(relation, 'a1_outline_mask', ['a0_outline_mask', 'a1_outline_mask'], [], 1)
    judge(relation, 'a2', ['a0_outline_mask', 'a1_outline_mask'])
    judge(relation, 'a3', ['a0_outline_mask'])

    judge(relation, 'a4_alpha_mask', ['a0_outline_mask'], ['a4_alpha_mask'], 2)
    judge(relation, 'a5', ['a0_outline_mask'], ['a4_alpha_mask'])
    judge(relation, 'a6', ['a0_outline_mask'])

    judge(relation, 'a7_alpha_mask', ['a0_outline_mask'], ['a7_alpha_mask'], 2)
    judge(relation, 'a8_alpha_mask', ['a0_outline_mask'], ['a7_alpha_mask', 'a8_alpha_mask'], 2)
    judge(relation, 'a9', ['a0_outline_mask'], ['a7_alpha_mask', 'a8_alpha_mask'])
    judge(relation, 'a10', ['a0_outline_mask'], ['a7_alpha_mask'])
    judge(relation, 'a11', ['a0_outline_mask'])


    judge(relation, 'b0_outline_mask', ['b0_outline_mask'], [], 1)

    judge(relation, 'b1_outline_mask', ['b0_outline_mask', 'b1_outline_mask'], [], 1)
    judge(relation, 'b2', ['b0_outline_mask', 'b1_outline_mask'])
    judge(relation, 'b3', ['b0_outline_mask'])

    judge(relation, 'b4_alpha_mask', ['b0_outline_mask'], ['b4_alpha_mask'], 2)
    judge(relation, 'b5', ['b0_outline_mask'], ['b4_alpha_mask'])
    judge(relation, 'b6', ['b0_outline_mask'])

    judge(relation, 'b7_alpha_mask', ['b0_outline_mask'], ['b7_alpha_mask'], 2)
    judge(relation, 'b8_alpha_mask', ['b0_outline_mask'], ['b7_alpha_mask', 'b8_alpha_mask'], 2)
    judge(relation, 'b9', ['b0_outline_mask'], ['b7_alpha_mask', 'b8_alpha_mask'])
    judge(relation, 'b10', ['b0_outline_mask'], ['b7_alpha_mask'])
    judge(relation, 'b11', ['b0_outline_mask'])

def test_group_shape_group_mask():
    relation = get_relation(f'{resource_path}/object/mask/grou_shape_group_mask.sketch')

    judge(relation, 'a0', ['a2_group_as_outline_mask'])
    judge(relation, 'a1', ['a2_group_as_outline_mask'])
    judge(relation, 'a2_group_as_outline_mask', ['a2_group_as_outline_mask'], [], 1)
    judge(relation, 'a3', ['a2_group_as_outline_mask'])
    judge(relation, 'a4_group')

    judge(relation, 'a5')
    judge(relation, 'a6')
    judge(relation, 'a7_shape_group_as_mask', ['a7_shape_group_as_mask'], [], 1)
    judge(relation, 'Rectangle', ['a7_shape_group_as_mask'])
