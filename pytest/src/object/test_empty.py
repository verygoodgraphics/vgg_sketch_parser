from common import analyze, resource_path
import src.attr.test_color as test_color

def test_artboard():
    out = analyze(f'{resource_path}/object/empty.sketch')

    assert out['fileType'] == 1
    assert not out['frames']
    #assert not out['symbolMaster']
