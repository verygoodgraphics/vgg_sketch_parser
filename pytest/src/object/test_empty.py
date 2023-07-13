from common import analyze
import src.attr.test_color as test_color

def test_artboard():
    out = analyze('pytest/resource/object/empty.sketch')

    assert out['fileType'] == 1
    assert not out['artboard']
    assert not out['symbolMaster']
