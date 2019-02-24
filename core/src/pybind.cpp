#include "BarcodeFormat.h"
#include "BinaryBitmap.h"
#include "BitArray.h"
#include "BitHacks.h"
#include "BitMatrix.h"
#include "BitSource.h"
#include "BitWrapperBinarizer.h"
#include "ByteArray.h"
#include "ByteMatrix.h"
#include "CharacterSet.h"
#include "CharacterSetECI.h"
#include "CustomData.h"
#include "DecodeHints.h"
#include "DecodeStatus.h"
#include "DecoderResult.h"
#include "DetectorResult.h"
#include "GenericGF.h"
#include "GenericGFPoly.h"
#include "GenericLuminanceSource.h"
#include "GlobalHistogramBinarizer.h"
#include "GridSampler.h"
#include "HybridBinarizer.h"
#include "LuminanceSource.h"
#include "MultiFormatReader.h"
#include "MultiFormatWriter.h"
#include "PerspectiveTransform.h"
#include "Reader.h"
#include "ReedSolomonDecoder.h"
#include "ReedSolomonEncoder.h"
#include "Result.h"
#include "ResultMetadata.h"
#include "ResultPoint.h"
#include "TextDecoder.h"
#include "TextEncoder.h"
#include "TextUtfEncoding.h"
#include "WhiteRectDetector.h"
#include "ZXBigInteger.h"
#include "ZXConfig.h"
#include "ZXContainerAlgorithms.h"
#include "ZXNullable.h"
#include "ZXNumeric.h"
#include "ZXStrConvWorkaround.h"
#include "ZXTestSupport.h"
#include "aztec/AZDecoder.h"
#include "aztec/AZDetector.h"
#include "aztec/AZDetectorResult.h"
#include "aztec/AZEncoder.h"
#include "aztec/AZEncodingState.h"
#include "aztec/AZHighLevelEncoder.h"
#include "aztec/AZReader.h"
#include "aztec/AZToken.h"
#include "aztec/AZWriter.h"
#include "datamatrix/DMBitMatrixParser.h"
#include "datamatrix/DMDataBlock.h"
#include "datamatrix/DMDecoder.h"
#include "datamatrix/DMDefaultPlacement.h"
#include "datamatrix/DMDetector.h"
#include "datamatrix/DMECB.h"
#include "datamatrix/DMECEncoder.h"
#include "datamatrix/DMEncoderContext.h"
#include "datamatrix/DMHighLevelEncoder.h"
#include "datamatrix/DMReader.h"
#include "datamatrix/DMSymbolInfo.h"
#include "datamatrix/DMSymbolShape.h"
#include "datamatrix/DMVersion.h"
#include "datamatrix/DMWriter.h"
#include "maxicode/MCBitMatrixParser.h"
#include "maxicode/MCDecoder.h"
#include "maxicode/MCReader.h"
#include "oned/ODCodabarReader.h"
#include "oned/ODCodabarWriter.h"
#include "oned/ODCode128Patterns.h"
#include "oned/ODCode128Reader.h"
#include "oned/ODCode128Writer.h"
#include "oned/ODCode39Reader.h"
#include "oned/ODCode39Writer.h"
#include "oned/ODCode93Reader.h"
#include "oned/ODCode93Writer.h"
#include "oned/ODEAN13Reader.h"
#include "oned/ODEAN13Writer.h"
#include "oned/ODEAN8Reader.h"
#include "oned/ODEAN8Writer.h"
#include "oned/ODEANManufacturerOrgSupport.h"
#include "oned/ODITFReader.h"
#include "oned/ODITFWriter.h"
#include "oned/ODMultiUPCEANReader.h"
#include "oned/ODRSS14Reader.h"
#include "oned/ODRSSExpandedReader.h"
#include "oned/ODReader.h"
#include "oned/ODRowReader.h"
#include "oned/ODUPCAReader.h"
#include "oned/ODUPCAWriter.h"
#include "oned/ODUPCEANCommon.h"
#include "oned/ODUPCEANExtensionSupport.h"
#include "oned/ODUPCEANReader.h"
#include "oned/ODUPCEReader.h"
#include "oned/ODUPCEWriter.h"
#include "oned/ODWriterHelper.h"
#include "oned/rss/ODRSSDataCharacter.h"
#include "oned/rss/ODRSSExpandedBinaryDecoder.h"
#include "oned/rss/ODRSSExpandedPair.h"
#include "oned/rss/ODRSSExpandedRow.h"
#include "oned/rss/ODRSSFieldParser.h"
#include "oned/rss/ODRSSFinderPattern.h"
#include "oned/rss/ODRSSGenericAppIdDecoder.h"
#include "oned/rss/ODRSSPair.h"
#include "oned/rss/ODRSSReaderHelper.h"
#include "pdf417/PDFBarcodeMetadata.h"
#include "pdf417/PDFBarcodeValue.h"
#include "pdf417/PDFBoundingBox.h"
#include "pdf417/PDFCodeword.h"
#include "pdf417/PDFCodewordDecoder.h"
#include "pdf417/PDFCompaction.h"
#include "pdf417/PDFDecodedBitStreamParser.h"
#include "pdf417/PDFDecoderResultExtra.h"
#include "pdf417/PDFDetectionResult.h"
#include "pdf417/PDFDetectionResultColumn.h"
#include "pdf417/PDFDetector.h"
#include "pdf417/PDFEncoder.h"
#include "pdf417/PDFHighLevelEncoder.h"
#include "pdf417/PDFModulusGF.h"
#include "pdf417/PDFModulusPoly.h"
#include "pdf417/PDFReader.h"
#include "pdf417/PDFScanningDecoder.h"
#include "pdf417/PDFWriter.h"
#include "qrcode/QRAlignmentPattern.h"
#include "qrcode/QRAlignmentPatternFinder.h"
#include "qrcode/QRBitMatrixParser.h"
#include "qrcode/QRCodecMode.h"
#include "qrcode/QRDataBlock.h"
#include "qrcode/QRDataMask.h"
#include "qrcode/QRDecoder.h"
#include "qrcode/QRDecoderMetadata.h"
#include "qrcode/QRDetector.h"
#include "qrcode/QRECB.h"
#include "qrcode/QREncodeResult.h"
#include "qrcode/QREncoder.h"
#include "qrcode/QRErrorCorrectionLevel.h"
#include "qrcode/QRFinderPattern.h"
#include "qrcode/QRFinderPatternFinder.h"
#include "qrcode/QRFinderPatternInfo.h"
#include "qrcode/QRFormatInformation.h"
#include "qrcode/QRMaskUtil.h"
#include "qrcode/QRMatrixUtil.h"
#include "qrcode/QRReader.h"
#include "qrcode/QRVersion.h"
#include "qrcode/QRWriter.h"
#include "textcodec/Big5MapTable.h"
#include "textcodec/Big5TextDecoder.h"
#include "textcodec/Big5TextEncoder.h"
#include "textcodec/GBTextDecoder.h"
#include "textcodec/GBTextEncoder.h"
#include "textcodec/JPTextDecoder.h"
#include "textcodec/JPTextEncoder.h"
#include "textcodec/KRHangulMapping.h"
// #include "textcodec/KRTextDecoder.h"
// #include "textcodec/KRTextEncoder.h"


#include </usr/local/include/python3.6/pybind11/pybind11.h>
#include </usr/local/include/python3.6/pybind11/stl.h>
#include </usr/local/include/python3.6/pybind11/complex.h>

// #include </usr/local/include/python3.6/pybind11/attr.h>
// #include </usr/local/include/python3.6/pybind11/buffer_info.h>
// #include </usr/local/include/python3.6/pybind11/cast.h>
// #include </usr/local/include/python3.6/pybind11/chrono.h>
// #include </usr/local/include/python3.6/pybind11/detail/class.h>
// #include </usr/local/include/python3.6/pybind11/detail/common.h>
// #include </usr/local/include/python3.6/pybind11/detail/descr.h>
// #include </usr/local/include/python3.6/pybind11/detail/init.h>
// #include </usr/local/include/python3.6/pybind11/detail/internals.h>
// #include </usr/local/include/python3.6/pybind11/detail/typeid.h>
// #include </usr/local/include/python3.6/pybind11/embed.h>
// #include </usr/local/include/python3.6/pybind11/eval.h>
// #include </usr/local/include/python3.6/pybind11/functional.h>
// #include </usr/local/include/python3.6/pybind11/iostream.h>
// #include </usr/local/include/python3.6/pybind11/numpy.h>
// #include </usr/local/include/python3.6/pybind11/operators.h>
// #include </usr/local/include/python3.6/pybind11/options.h>
// #include </usr/local/include/python3.6/pybind11/pytypes.h>
// #include </usr/local/include/python3.6/pybind11/stl_bind.h>

namespace py = pybind11;


PYBIND11_MODULE(ZXingCore, m) {
	m.def("BarcodeFormatFromString", [](const std::string& str) {
			return ZXing::BarcodeFormatFromString(str);
		});


	// BarcodeFormat
	py::class_<ZXing::BarcodeFormat>(m, "BarcodeFormat")
		.def(py::init<>());


	// ByteArray
	py::class_<ZXing::ByteArray>(m, "ByteArray")
		.def(py::init<>());


	// DecodeHints
	py::class_<ZXing::DecodeHints>(m, "DecodeHints")
		.def(py::init<>())
		.def("setShouldTryHarder", &ZXing::DecodeHints::setShouldTryHarder)
		.def("setShouldTryRotate", &ZXing::DecodeHints::setShouldTryRotate)
		.def("possibleFormats", &ZXing::DecodeHints::possibleFormats)
		// .def("setPossibleFormats", &ZXing::DecodeHints::setPossibleFormats);
		.def("setPossibleFormats", [](ZXing::DecodeHints &self, const std::vector<ZXing::BarcodeFormat>& formats) {
			self.setPossibleFormats(formats);
		});


	// GenericLuminanceSource
	py::class_<ZXing::GenericLuminanceSource, std::shared_ptr<ZXing::GenericLuminanceSource>>(m, "GenericLuminanceSource")
		.def(py::init<int, int, const void*, int, int, int, int, int>())
		.def(py::init<int, int, char*, int>());
		// .def("rotated", &ZXing::GenericLuminanceSource::rotated)
		// .def("cropped", &ZXing::GenericLuminanceSource::cropped);


	// GlobalHistogramBinarizer
	py::class_<ZXing::GlobalHistogramBinarizer, std::shared_ptr<ZXing::GlobalHistogramBinarizer>>(m, "GlobalHistogramBinarizer")
		.def(py::init<std::shared_ptr<const ZXing::LuminanceSource>, bool>());


	// HybridBinarizer
	py::class_<ZXing::HybridBinarizer, std::shared_ptr<ZXing::HybridBinarizer>>(m, "HybridBinarizer")
		.def(py::init<std::shared_ptr<ZXing::LuminanceSource>>())
		.def(py::init<std::shared_ptr<ZXing::LuminanceSource>, bool>())
		.def(py::init<std::shared_ptr<ZXing::GenericLuminanceSource>>())
		.def(py::init<std::shared_ptr<ZXing::GenericLuminanceSource>, bool>())
		.def("newInstance", &ZXing::HybridBinarizer::newInstance);
		// .def("newInstance", [](ZXing::HybridBinarizer &self, std::shared_ptr<ZXing::GenericLuminanceSource> &source) {
		// 	auto ls = (std::shared_ptr<ZXing::LuminanceSource>)source;
		// 	auto bm = self.newInstance(ls);
		// 	return bm;
		// });


	// MultiFormatReader
	py::class_<ZXing::MultiFormatReader>(m, "MultiFormatReader")
		.def(py::init<ZXing::DecodeHints>())
		// .def("read", &ZXing::MultiFormatReader::read);
		.def("read", [](ZXing::MultiFormatReader &self, ZXing::HybridBinarizer &hb) {
			return ZXing::Result (self.read(hb));
		});


	// Result
	py::class_<ZXing::Result>(m, "Result")
		.def(py::init<ZXing::Result>())
		.def("metadata", [](ZXing::Result &self) {
			return self.metadata();
		})
		.def("addResultPoints", &ZXing::Result::addResultPoints)
		.def("format", &ZXing::Result::format)
		.def("isValid", &ZXing::Result::isValid)
		.def("rawBytes", &ZXing::Result::rawBytes)
		.def("resultPoints", &ZXing::Result::resultPoints)
		.def("setFormat", &ZXing::Result::setFormat)
		// .def("setResultPoints", &ZXing::Result::setResultPoints)
		// .def("setText", &ZXing::Result::setText)
		.def("status", &ZXing::Result::status)
		.def("text", &ZXing::Result::text);


	// TextUtfEncoding
	py::class_<ZXing::TextUtfEncoding>(m, "TextUtfEncoding")
		.def_static("AppendUtf16", &ZXing::TextUtfEncoding::AppendUtf16)
		.def_static("AppendUtf8", &ZXing::TextUtfEncoding::AppendUtf8)
		.def_static("ToUtf8", [](const std::wstring& str) {
			return ZXing::TextUtfEncoding::ToUtf8(str);
		})
		.def_static("ToUtf8", [](const std::wstring& str, std::string& utf8) {
			return ZXing::TextUtfEncoding::ToUtf8(str);
		});

}