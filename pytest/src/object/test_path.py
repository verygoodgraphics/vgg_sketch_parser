from common import create_collection, check_float_equal, resource_path

def test_path():
    relation = create_collection(f'{resource_path}/object/path.sketch')

    def check_radius(name, value: list):
        obj = relation[name]
        assert len(obj["shape"]["subshapes"]) == 1

        points = obj["shape"]["subshapes"][0]["subGeometry"]["points"]
        assert len(points) == len(value)

        for x, y in zip(points, value):
            check_float_equal(x["radius"], y)
    check_radius("a0", [0, 0, 0, 0])
    check_radius("a1", [30, 30, 30, 30])
    check_radius("a2", [30, 40, 57, 50])

    def check_cornerStyle(name, value):
        obj = relation[name]
        assert len(obj["shape"]["subshapes"]) == 1

        points = obj["shape"]["subshapes"][0]["subGeometry"]["points"]
        for x in points:
            assert x["cornerStyle"] == value
    check_cornerStyle("a3", 0)
    check_cornerStyle("a4", 0)
    check_cornerStyle("a5", 2)
    check_cornerStyle("a6", 3)
    check_cornerStyle("a7", 1)

    def check_curve_point(name, value):
        obj = relation[name]
        assert len(obj["shape"]["subshapes"]) == 1
        
        points = obj["shape"]["subshapes"][0]["subGeometry"]["points"]      
        assert len(points) == len(value)

        for x, y in zip(points, value):
            check_float_equal(x["point"][0], y[0][0])
            check_float_equal(x["point"][1], y[0][1])

            if y[1] is not None:
                check_float_equal(x["curveFrom"][0], y[1][0])
                check_float_equal(x["curveFrom"][1], y[1][1])
            
            if y[2] is not None:
                check_float_equal(x["curveTo"][0], y[2][0])
                check_float_equal(x["curveTo"][1], y[2][1])                
    check_curve_point("a8", 
                      [
                          ([0, -79], None, None), 
                          ([58, 0], None, None), 
                          ([113, -79], None, None), 
                      ])
    check_curve_point("a9", 
                      [
                          (
                            [3.4929477679246195, -79.04206189709915], 
                            [-12.13420505463115, -56.26350015574669], 
                            [19.120100590480387, -101.82062363845161]
                          ), 

                          (
                            [57.492947767924605, -0.04206189709920237], 
                            [160.49294776792468, -0.04206189709924948], 
                            [28.03345853984262, 1.7828622143749069]
                          ), 

                          (
                            [116.49294776792463, -86.04206189709915], 
                            None, None
                          )
                      ])    
    
    def check_contour(name, value):
        obj = relation[name]
        assert len(obj["shape"]["subshapes"]) == 1
        assert obj["shape"]["subshapes"][0]["subGeometry"]["closed"] == value
    check_contour("a10", False)    
    check_contour("a11", True)    
    
    def check_boolean(name, value):
        obj = relation[name]
        for x, y in zip(obj["shape"]["subshapes"], value):
            assert x["booleanOperation"] == y
    check_boolean("union", [0, 0])
    check_boolean("sub", [1, 1])
    check_boolean("interset", [2, 2])
    check_boolean("diff", [3, 3])
    check_boolean("none", [0, 4, 4])

    def check_subGeometry(name, value):
        obj = relation[name]
        for x, y in zip(obj["shape"]["subshapes"], value):
            assert x["subGeometry"]["class"] == y[0]
            assert x["subGeometry"]["name"] == y[1]
    check_subGeometry("subGeometry",
                      (["path", "a22"],
                      ["text", "a23_text"],
                      ["image", "a24_img"]))
    
    def check_winding_rule(name, value):
        obj = relation[name]
        assert obj["shape"]["windingRule"] == value
    check_winding_rule("a25", 1)
    check_winding_rule("a26", 0)

    # def check_path_type(name, value):
    #     obj = relation[name]
    #     assert obj["pathType"] == value
    # check_path_type("a_28_Rectangle", 0)
    # check_path_type("a_29_Oval", 3)
    # check_path_type("a_30_Rectangle", 0)
    # check_path_type("a_31_Line", 6)
    # check_path_type("a_32_Line 2", 6)
    # check_path_type("a_33_Triangle", 4)
    # check_path_type("a_34_Star", 5)
    # check_path_type("a_35_Polygon", 4)
    # check_path_type("a_36_Path", 6)
    # check_path_type("a_37_Path 2", 6)

    # ignore object
    assert 'a_27_slice_ignore' not in relation
    assert 'a_38_Hotspot_ignore' not in relation