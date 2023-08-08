from common import create_collection, resource_path

def test_image():
    relation = create_collection(f'{resource_path}/object/image.sketch')

    obj = relation["fill replace"]
    assert obj["fillReplacesImage"]

    obj = relation["style"]
    assert 'fillReplacesImage' not in obj or not obj["fillReplacesImage"]
