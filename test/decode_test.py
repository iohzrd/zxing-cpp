import os
import unittest
from PIL import Image
from ZXingCore import (
    BarcodeFormat,
    BarcodeFormatFromString,
    BarcodeFormatToString,
    DecodeHints,
    GenericLuminanceSource,
    GlobalHistogramBinarizer,
    HybridBinarizer,
    MultiFormatReader,
    TextUtfEncoding,
)

hints = DecodeHints()
hints.setPossibleFormats([])
hints.setShouldTryHarder(True)
hints.setShouldTryRotate(True)
reader = MultiFormatReader(hints)


class MainTest(unittest.TestCase):
    pass


def fn(caseImagePath):
    im = Image.open(caseImagePath).convert("L")
    width, height = im.size
    im = im.tobytes()
    source = GenericLuminanceSource(width, height, im, width)
    binarizer = HybridBinarizer(source)
    result = reader.read(binarizer)
    return result.text()


for fol in os.listdir("./blackbox/"):
    if os.path.isdir("./blackbox/{}".format(fol)):
        for fil in os.listdir("./blackbox/{}".format(fol)):
            if fil.endswith(".txt"):
                case = fil.split(".")[0]
                caseTxt = open("./blackbox/{}/{}".format(fol, fil), "r")
                caseValue = caseTxt.readline()
                caseTxt.close()
            if fil.endswith(".png"):
                caseImagePath = "./blackbox/{}/{}".format(fol, fil)
                testmethodname = "test_fn_{0}".format(fil)
                testmethod = lambda self: self.assertEqual(fn(caseImagePath), caseValue)
                setattr(MainTest, testmethodname, testmethod)

if __name__ == "__main__":
    unittest.main()
