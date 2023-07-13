from common import create_collection

def test_image():
    relation = create_collection('pytest/resource/object/image.sketch')

    obj = relation["fill replace"]
    assert obj["fillReplacesImage"]

    obj = relation["style"]
    assert 'fillReplacesImage' not in obj or not obj["fillReplacesImage"]
