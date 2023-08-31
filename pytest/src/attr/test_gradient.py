from common import check_float_equal, check_name, analyze, resource_path
import src.attr.test_color as test_color

def check_gradient_from_or_to(gradient, check_from: bool, pt: list):
    attr = "from" if check_from else "to"

    for x, y in zip(gradient[attr], pt):
        check_float_equal(x, y)

def check_stops_basic(stop, position, midPoint):
    check_float_equal(stop["position"], position)
    check_float_equal(stop["midPoint"], midPoint)

def test_gradient():
    out = analyze(f'{resource_path}/attr/gradient.sketch')
    objs = out['frames'][0]['childObjects']

    obj = objs[0]
    check_name(obj, "line_opacity")
    gradient = obj["style"]["fills"][0]["gradient"]["instance"]
    assert gradient["class"] == "gradientLinear"
    check_gradient_from_or_to(gradient, True, [0.19615521767606037, -0.087144455020446543])
    check_gradient_from_or_to(gradient, False, [0.89720617706798489, 1.11999853029441])
    stops = gradient["stops"]
    assert len(stops) == 3
    test_color.check_color(stops[0]["color"], 0.3456608531204033, 0.8883248730964468, 0.444327038570593, 1)
    test_color.check_color(stops[1]["color"], 0.01449275362318841, 0.0144220572640509, 0.0144220572640509, 0.8)
    test_color.check_color(stops[2]["color"], 0.7608695652173914, 0.1744432661717921, 0.1744432661717921, 0.5)
    check_stops_basic(stops[0], 0, 0.5)
    check_stops_basic(stops[1], 0.4685314685314685, 0.5)
    check_stops_basic(stops[2], 1, 0.5)

    obj = objs[1]
    check_name(obj, "radial")
    gradient = obj["style"]["fills"][0]["gradient"]["instance"]
    assert gradient["class"] == "gradientRadial"
    check_gradient_from_or_to(gradient, True, [0.5, 0.43595083861599271])
    check_gradient_from_or_to(gradient, False, [0.5, 1.4359508386159927])
    stops = gradient["stops"]
    assert len(stops) == 3
    test_color.check_color(stops[0]["color"], 0.06891659065572117, 0, 0.9855072463768116, 1)
    check_stops_basic(stops[1], 0.5244755244755245, 0.5)

    obj = objs[2]
    check_name(obj, "angular_0")
    gradient = obj["style"]["fills"][0]["gradient"]["instance"]
    assert gradient["class"] == "gradientAngular"    
    check_gradient_from_or_to(gradient, True, [0.5, 0.5])
    check_gradient_from_or_to(gradient, False, [1.0, 0.5])
    stops = gradient["stops"]
    assert len(stops) == 2
    test_color.check_color(stops[0]["color"], 1, 1, 1, 1)
    check_stops_basic(stops[0], 0.1337361562091488, 0.5)

    obj = objs[3]
    check_name(obj, "angular_1")
    gradient = obj["style"]["fills"][0]["gradient"]["instance"]
    assert gradient["class"] == "gradientAngular"    
    check_gradient_from_or_to(gradient, True, [0.5, 0.5])
    #check_gradient_from_or_to(gradient, False, [1.8584070796460177, 0.5])
    check_gradient_from_or_to(gradient, False, [1.0, 0.5])