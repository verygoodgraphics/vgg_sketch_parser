from common import analyze

def test_artboard():
    out = analyze('pytest/resource/object/overflow_version_97.sketch')

    assert 1 == out['artboard'][0]['overflow']
    assert 2 == out['artboard'][1]['overflow']
