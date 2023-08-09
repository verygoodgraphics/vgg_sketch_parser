from common import analyze, resource_path
import src.attr.test_color as test_color

def test_artboard():
    out = analyze(f'{resource_path}/object/artboard.sketch')

    test_color.check_color(out['frames'][0]['style']['fills'][0]['color'], 1, 0, 0, 1)
    assert len(out['frames'][1]['style']['fills']) == 0
