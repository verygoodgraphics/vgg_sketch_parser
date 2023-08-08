from common import create_collection, resource_path

def test_group():
    relation = create_collection(f'{resource_path}/object/group.sketch')

    def check_group(name, value):
        obj = relation[name]
        assert len(obj["childObjects"]) == len(value)
        for x, y in zip(obj["childObjects"], value):
            assert x["class"] == y[0]
            assert x["name"] == y[1]
    check_group("Group", [
        ["group", "a0_empty"],
        ["image", "a1_img"],
        ["text", "a2_text"],
        ["path", "a3_path"],
        ["group", "a7_group"]
    ])
    check_group("a0_empty", [])
    check_group("a7_group", [["path", "a4_union"]])
