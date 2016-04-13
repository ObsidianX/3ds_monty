import _img


def load_png(_file, background=False):
    png = _img._PngLoader(_file)

    if background:
        png.load_in_background()
        return png

    return png.load_all()


def load_jpeg(_file, background=False):
    jpeg = _img._JpegLoader(_file)

    if background:
        jpeg.load_in_background()
        return jpeg

    return jpeg.load_all()
