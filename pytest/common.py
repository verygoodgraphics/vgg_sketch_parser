import subprocess
import jsonschema
import json


resource_root = 'C:/work/res/data/sketch/pytest'
#resource_path = f'{resource_root}/88_1'
resource_path = f'{resource_root}/96'

with open("pytest/vgg-format.json") as f:
    str_schema = f.read()
    schema = json.loads(str_schema)

def analyze(filename):
    '''
    加载指定的文件, 进行解析, 并校验解析结果是否符合 vgg-format

    :param filename 被解析的文件
    :return 解析出的 json 数据
    '''
    result = subprocess.Popen(f'pytest/sketch_parser_tool.exe {filename} pytest/output').wait()
    assert result == 0

    try:
        f = open('pytest/output/design.json', "rb")
        data = f.read()
        f.close()
    except:
        assert False
    
    assert data
    out = json.loads(data)
    jsonschema.validate(out, schema)

    return out

def check_float_equal(a, b):
    assert abs(a - b) <= 0.000001

def check_name(obj, name):
    assert obj["name"] == name

def create_collection(file_name):
    out = analyze(file_name)

    relation = {}
    for frame in out["frames"]:
        __collection(frame, relation)
    
    return relation    

def __collection(obj, out):
    '''
    要求输入的 name 是唯一的

    :return dict[name: object]
    '''

    assert obj["name"] not in out

    if obj["class"] != "frame":
        out[obj["name"]] = obj

    if 'childObjects' in obj:
        for item in obj['childObjects']:
            __collection(item, out) 
    
    if 'shape' in obj:
        for item in obj["shape"]["subshapes"]:
            if item["subGeometry"]["class"] != "contour":
                __collection(item["subGeometry"], out)

def check_equal(a, b):
    if isinstance(a, (tuple, list, set, dict)):
        assert len(a) == len(b)
        for x, y in zip(a, b, strict = True):
            check_equal(x, y)
    else:
        if isinstance(a, float) or isinstance(b, float):
            check_float_equal(a, b)
        else:
            assert a == b
