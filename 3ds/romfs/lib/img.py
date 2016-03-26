from _img import *


def load_png(_file):
    png = _PngLoader(_file)

    while png.load_chunk():
        yield None

    p = png.finish()
    del png

    yield p


def load_jpeg(_file):
    jpeg = _JpegLoader(_file)

    while jpeg.load_chunk():
        yield None

    j = jpeg.finish()
    del jpeg

    yield j
