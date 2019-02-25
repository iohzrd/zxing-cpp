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


def create_test_fn(caseImgPath, caseTxtValue):
    def do_test_expected(self):
        im = Image.open(caseImgPath).convert("L")
        width, height = im.size
        im = im.tobytes()
        source = GenericLuminanceSource(width, height, im, width)
        binarizer = HybridBinarizer(source)
        result = reader.read(binarizer)
        self.assertEqual(result.text(), caseTxtValue)

    return do_test_expected


class TestSequence(unittest.TestCase):
    maxDiff = None


for fol in os.listdir("./blackbox/"):
    if os.path.isdir("./blackbox/{}".format(fol)):
        for fil in os.listdir("./blackbox/{}".format(fol)):
            if fil.endswith(".txt"):
                caseTxtPath = os.path.abspath("./blackbox/{}/{}".format(fol, fil))
                caseImgPath = os.path.abspath(caseTxtPath.replace(".txt", ".png"))
                if os.path.exists(caseTxtPath) and os.path.exists(caseImgPath):
                    caseTxtFile = open(caseTxtPath, "r")
                    caseTxtValue = caseTxtFile.read()
                    caseTxtFile.close()
                    testmethodname = (
                        "test_fn_{}_{}".format(fol, fil)
                        .replace(".txt", "")
                        .replace("-", "_")
                    )
                    testmethod = create_test_fn(caseImgPath, caseTxtValue)
                    setattr(TestSequence, testmethodname, testmethod)


if __name__ == "__main__":
    unittest.main()
