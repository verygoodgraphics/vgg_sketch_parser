from common import analyze, resource_path

def test_artboard():
    out = analyze(f'{resource_path}/object/overflow_version_97.sketch')

    assert 1 == out['frames'][0]['overflow']
    assert 2 == out['frames'][1]['overflow']
