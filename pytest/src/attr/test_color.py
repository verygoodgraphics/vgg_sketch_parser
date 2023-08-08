import common

def check_color(color, r, g, b, a):
    common.check_float_equal(color["red"], r)
    common.check_float_equal(color["green"], g)
    common.check_float_equal(color["blue"], b)
    common.check_float_equal(color["alpha"], a)

def test_color():
    out = common.analyze(f'{common.resource_path}/attr/color.sketch')

    objs = out["artboard"][0]["layers"][0]["childObjects"]

    # 不含透明度的对象
    obj = objs[0]
    common.check_name(obj, 'aa')
    check_color(obj["style"]["fills"][0]["color"], 
                0.7608695652173914, 0.1744432661717921, 0.1744432661717921, 1.0)
    
    # 含透明度的对象
    obj = objs[1]
    common.check_name(obj, 'bb')
    check_color(obj["style"]["fills"][0]["color"], 
                0.1744432661717921, 0.7608695652173914, 0.7485669155870641, 0.5)
