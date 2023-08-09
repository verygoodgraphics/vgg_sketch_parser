from common import check_float_equal, check_name, analyze, resource_path

def check_id(obj, value):
    assert obj["id"] == value

def check_visible(obj, value):
    assert obj["visible"] == value

def check_isLocked(obj, value):
    assert obj["isLocked"] == value

def check_rect(rect, x, y, w, h):
    check_float_equal(rect["x"], x)
    check_float_equal(rect["y"], y)
    check_float_equal(rect["width"], w)
    check_float_equal(rect["height"], h)

def check_matrix(obj, value):
    assert len(obj["matrix"]) == 6
    for x, y in zip(obj["matrix"], value):
        check_float_equal(x, y)

def test_basic_attr():
    out = analyze(f'{resource_path}/object/basic_attr.sketch')
    
    def get_objs(id):
        return out['frames'][id]['childObjects']
        
    # id name
    def check(obj, id, name):
        check_name(obj, name)
        check_id(obj, id)
    objs = get_objs(0)
    check(objs[0], '11A21950-58F1-41DF-8BD2-5985703E1B3D', 'Group')
    check(objs[0]['childObjects'][0], 'FB29CCC1-15B8-4779-B3F4-C134AA8D3789', 'Rectangle')
    check(objs[1], '7947572A-7B41-47B7-B038-6EA36CE77CE2', 'hello')
    check(objs[2], 'F98B6E01-3AEC-46D6-83BF-6E1A60DD5613', '莱纳')
    check(objs[3], '1CB033C0-41E8-454C-9E0A-860B40843F59', 'Type something')
    check(objs[4], '0977FF27-520C-4610-9914-44ABF320C558', 'cc')
    check(objs[4]['shape']['subshapes'][0]["subGeometry"], '02AFCE0F-D613-4711-A752-90C68A86BCD0', 'aa')
    check(objs[4]['shape']['subshapes'][1]["subGeometry"], '102CD06F-505F-404F-AF9B-A742F7A95A0D', 'bb')

    # visible
    objs = get_objs(1)
    check_visible(objs[0], True)
    check_visible(objs[1], False)
    check_visible(objs[2], True)
    check_visible(objs[2]["childObjects"][0], False)
    check_visible(objs[2]["childObjects"][1], True)
    check_visible(objs[3], True)
    check_visible(objs[3]['shape']['subshapes'][0]["subGeometry"], False)
    check_visible(objs[3]['shape']['subshapes'][1]["subGeometry"], True)

    # isLocked
    objs = get_objs(2)
    check_isLocked(objs[0], True)
    check_isLocked(objs[1], False)
    check_isLocked(objs[2], True)
    check_isLocked(objs[2]["childObjects"][0], True)
    check_isLocked(objs[2]["childObjects"][1], False)
    check_isLocked(objs[3], True)
    check_isLocked(objs[3]['shape']['subshapes'][0]["subGeometry"], True)
    check_isLocked(objs[3]['shape']['subshapes'][1]["subGeometry"], True)

    # bound frame matrix
    def check(obj, frame_x, frame_y, bound_w, bound_h, matrix, frame_w = None, frame_h = None, bound_x = 0, bound_y = 0):
        if frame_w is None:
            frame_w = bound_w
        if frame_h is None:
            frame_h = bound_h

        check_rect(obj["bounds"], bound_x, bound_y, bound_w, bound_h)
        check_rect(obj["frame"], frame_x, frame_y, frame_w, frame_h)
        check_matrix(obj, matrix)

        a = matrix[0]
        b = matrix[1]
        c = matrix[2]
        d = matrix[3]
        tx = matrix[4]
        ty = matrix[5]

        def calc(x, y):
            x1 = a * x + c * y + tx
            y1 = b * x + d * y + ty
            return x1, y1
        
        p0 = calc(bound_x, bound_y)
        p1 = calc(bound_w + bound_x, bound_y)
        p2 = calc(bound_w + bound_x, bound_y - bound_h)
        p3 = calc(bound_x, bound_y - bound_h)

        x_calc = [p0[0], p1[0], p2[0], p3[0]]
        y_calc = [p0[1], p1[1], p2[1], p3[1]]
        x_start = min(x_calc)
        x_stop = max(x_calc)
        y_start = max(y_calc)
        y_stop = min(y_calc)
        check_float_equal(frame_x, x_start)
        check_float_equal(frame_y, y_start)
        check_float_equal(frame_w, x_stop - x_start)
        check_float_equal(frame_h, y_start - y_stop)
    objs = get_objs(3)
    check(objs[0], 14, -14, 482, 357, [1, 0, 0, 1, 14, -14])
    check(objs[1], 518, -14, 482, 357, [-1, 0, 0, 1, 1000, -14])
    check(objs[2], 21, -404, 482, 357, [1, 0, 0, -1, 21, -761])
    check(objs[3], 524, -404, 482, 357, [-1, 0, 0, -1, 1006, -761])
    check(objs[4], 76.03787799238975, -794.4144664914695, 482, 357,
          [ 0.8660254082502548, -0.49999999226497954, 0.49999999226497954,
            0.8660254082502548, 254.53787523098745, -794.4144664914695 ],
            595.9242440152204, 550.1710670170612)
    check(objs[5], 765.0378779923897, -783.4144664914695, 482, 357, 
          [ -0.8660254082502548, -0.49999999226497954, -0.49999999226497954,
            0.8660254082502548, 1182.4621247690125, -783.4144664914695 ],
            595.9242440152204, 550.1710670170612)
    check(objs[6], 114.03787799238972, -1498.4144664914697, 482, 357, 
          [ 0.8660254082502548, 0.49999999226497954, 0.49999999226497954,
            -0.8660254082502548, 292.5378752309874, -2048.5855335085307 ],
            595.9242440152204, 550.171067017061)
    check(objs[7], 822.0378779923897, -1498.4144664914697, 482, 357, 
          [ -0.8660254082502548, 0.49999999226497954, -0.49999999226497954,
            -0.8660254082502548, 1239.4621247690125, -2048.5855335085307 ],
            595.9242440152204, 550.171067017061)
    check(objs[8], 192.65036066916753, -2132.602734318598, 785.9621223120497, 637.5855345755223,
          [ -0.9848077535291953, -0.17364817473495014, -0.17364817473495014,
            0.9848077535291953, 966.6719527023358, -2132.602734318598 ],
            884.7371563496152, 764.380065938326)
    check(objs[8]['childObjects'][0], 3.04439566889414e-07, -87.41446649146945, 482, 357, 
          [ 0.8660254082502548, -0.49999999226497954, 0.49999999226497954,
            0.8660254082502548, 178.49999754303727, -87.41446649146945 ], 
            595.9242440152204, 550.171067017061)
    check(objs[9], 174.80351078652296, -2980.7416655078887, 790.9621223120497, 550.1710691510445,
          [ 0.9396926228224405, -0.3420201377303428, 0.3420201377303428,
            0.9396926228224405, 362.97309563281317, -2980.7416655078887 ],
            931.4308561149041, 787.5166689842226)
    check(objs[9]['shape']['subshapes'][1]["subGeometry"], 
          3.04439566889414e-07, -1.066991615061852e-06, 482, 357,
          [ 0.8660254082502548, -0.49999999226497954, 0.49999999226497954,
            0.8660254082502548, 178.49999754303727, -1.066991615061852e-06 ],
            595.9242440152204, 550.171067017061)
    # 备注: group shape_group 中两个图片的左上角的点的正确性, 通过手动计算保证过了, 只要上述验证满足, 其结果就是对的

    # defalut_artboard bound frame matrix
    artboard_obj = out['frames'][4]
    check(artboard_obj, -1180.9999992684716, -4077.000000987401, 1361.9999992684716, 
          843.9999990125989, [1, 0, 0, 1, 0, 0], None, None, -1180.9999992684716, -4077.000000987401)

    objs = get_objs(4)
    check(objs[0]['childObjects'][0], 3.04439566889414e-07, -87.41446649146945, 482, 357, 
          [ 0.8660254082502548, -0.49999999226497954, 0.49999999226497954,
            0.8660254082502548, 178.49999754303727, -87.41446649146945 ],
            595.9242440152204, 550.171067017061)
    check(objs[0], -1180.9999992684716, -4077.000000987401, 785.9621223120497, 637.5855345755223, 
          [ -0.9848077535291953, -0.17364817473495014, -0.17364817473495014,
            0.9848077535291953, -406.9784072353034, -4077.000000987401 ], 
            884.7371563496151, 764.380065938326)
    # 备注: 图片左上角的二重变换, 手动验证过了, 只要上述验证满足, 其结果就是对的

