from common import analyze, resource_path

def test_artboard():
    out = analyze(f'{resource_path}/object/overflow_version_97.sketch')

    assert 1 == out['artboard'][0]['overflow']
    assert 2 == out['artboard'][1]['overflow']
