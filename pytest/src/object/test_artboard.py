from common import analyze
import src.attr.test_color as test_color

def test_artboard():
    out = analyze('pytest/resource/object/artboard.sketch')

    assert out['artboard'][0]['hasBackgroundColor']
    assert not out['artboard'][1]['hasBackgroundColor']
    test_color.check_color(out['artboard'][0]['backgroundColor'], 1, 0, 0, 1)
