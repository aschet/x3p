/***************************************************************************
 *   Copyright by Georg Wiora (NanoFocus AG) 2008                          *
 *                                                                         *
 *   This file is part of the openGPS (R)[TM] software library.            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License (LGPL)    *
 *   as published by the Free Software Foundation; either version 3 of     *
 *   the License, or (at your option) any later version.                   *
 *   for detail see the files "licence_LGPL-3.0.txt" and                   *
 *   "licence_GPL-3.0.txt".                                                *
 *                                                                         *
 *   openGPS is distributed in the hope that it will be useful,            *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU Lesser General Public License for more details.                   *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 *                                                                         *
 *   The name "openGPS" and the logo are registered as                     *
 *   European trade mark No. 006178354 for                                 *
 *   Physikalisch Technische Bundesanstalt (PTB)                           *
 *   http://www.ptb.de/                                                    *
 *                                                                         *
 *   More information about openGPS can be found at                        *
 *   http://www.opengps.eu/                                                *
 ***************************************************************************/

 // Mex file to import X3P files to matlab

#include "X3PUtilities.h"

#include "mex.h"
#include <strstream>
#include <limits>
#include <opengps/cxx/opengps.hxx>
#include <opengps/iso5436_2.h>
#include <opengps/cxx/iso5436_2.hxx>
#include <opengps/cxx/iso5436_2_handle.hxx>
#include <opengps/cxx/iso5436_2_xsd.hxx>
#include <opengps/cxx/point_iterator.hxx>
#include <opengps/cxx/point_vector.hxx>
#include <opengps/cxx/data_point.hxx>
#include <opengps/cxx/string.hxx>
#include <opengps/cxx/info.hxx>
#ifdef _WIN32
#include <tchar.h>
#endif

using namespace std;
using namespace OpenGPS::Schemas::ISO5436_2;

void mexFunction(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[])
{
	wstring SyntaxHelp(
		_T("Call Syntax:\n")
		_T("  [z,x,y,pinfo,meta] = openX3P(filename[,...])\n")
		_T("    x     - Array of x coordinates in meter\n")
		_T("    y     - Array of y coordinates in meter\n")
		_T("    z     - Array of z coordinates in meter\n")
		_T("    pinfo - Info about data organisation\n")
		_T("    meta  - Meta data structure of the file\n")
		_T("  [z, x, y] = openX3P(filename)\n")
		_T("  [z, x, y, pinfo] = openX3P(filename)\n")
		_T("  [z, x, y, pinfo, meta] = openX3P(filename)\n\n")
		_T("  Additional key-value-pairs can be specified after the last argument:\n")
		_T("    'VendorSpecificFilename' - The exact (case sensitive) filename\n")
		_T("            (without path portion) of a file that was packet into the\n")
		_T("            x3p file as a vendor specific extension. This argument is\n")
		_T("            mandatory for reading the vendorspecific extension.\n")
		_T("    'VendorSpecificTargetFile' - The path for the destination where the\n")
		_T("            unpacked file should be stored to. Default is the current\n")
		_T("            directory and the original filename.\n")
		_T("    'VendorSpecificURI' - if a specific URI has been used you have to\n")
		_T("            specify it here. The default extension for MATLAB written\n")
		_T("            x3p-files is '") OGPS_VEXT_URI _T("'\n")

		OGPS_LICENSETEXT);
	SyntaxHelp.append(GetX3P_Dll_ID());

	mxArray* outMatrixX{};
	mxArray* outMatrixY{};
	mxArray* outMatrixZ{};

	// check for proper number of arguments
	if ((nrhs < 1) || (nrhs > 7))
	{
		ostringstream msg;
		msg << "openX3P was called with " << nrhs << " input arguments!" << endl
			<< SyntaxHelp << ends;
		mexErrMsgIdAndTxt("openGPS:openX3P:nrhs", msg.str().c_str());
	}
	// Check number of output arguments
	if ((nlhs < 3) || (nlhs > 5))
	{
		ostringstream msg;
		msg << "openX3P was called with " << nlhs << " output arguments!" << endl
			<< SyntaxHelp << ends;
		mexErrMsgIdAndTxt("openGPS:openX3P:nlhs", msg.str().c_str());
	}

	// Check for metadata requested
	bool hasMeta{};
	if (nlhs == 5)
		hasMeta = true;

	// Check for point info structure requested
	bool hasPointInfo{};
	if (nlhs >= 4)
		hasPointInfo = true;

	// Get filename
	auto inFileName = prhs[0];

	// make sure the filename argument is string
	if (!mxIsChar(inFileName) ||
		mxGetNumberOfElements(inFileName) < 1) {
		mexErrMsgIdAndTxt("openGPS:openX3P:notString", "Input must be a file name string");
	}

	std::wstring FileNameL(ConvertMtoWStr(inFileName));

	// Flags for keyword existence
	bool bHasVendorFilename{};
	bool bHasVendorURI{};
	bool bHasVendorTarget{};

	const size_t cKeywordOffset{ 1 };
	const size_t inNKeywords{ nrhs - cKeywordOffset };

	// Keyword arguments
	const mxArray** inKeywords{};
	const mxArray* inVendorFilename{};
	const mxArray* inVendorURI{};
	const mxArray* inVendorTarget{};
	// std strings for keyword arguments
	std::wstring strVFilename;
	std::wstring strVURI{ OGPS_VEXT_URI };
	std::wstring strVTarget;

	// Parse keyword arguments
	if (inNKeywords > 0)
	{
		inKeywords = &prhs[cKeywordOffset];
		// Parse keys
		for (size_t i = 0; i < inNKeywords; i++)
		{
			wstring key(ConvertMtoWStr(inKeywords[i]));
			transform(key.begin(), key.end(), key.begin(), ::tolower);

			// check Vendor specific filename
			if (key == _T("vendorspecificfilename"))
			{
				if (inNKeywords <= i + 1)
				{
					mexErrMsgIdAndTxt("openGPS:writeX3P:missingArgument", "'VendorSpecificFilename' keyword not followd by an argument");
				}
				inVendorFilename = inKeywords[++i];
				if (!mxIsChar(inVendorFilename) ||
					mxGetNumberOfElements(inVendorFilename) < 1) {
					mexErrMsgIdAndTxt("openGPS:writeX3P:notString", "'VendorSpecificFilename' argument must be a string");
				}
				strVFilename = std::wstring(ConvertMtoWStr(inVendorFilename));
				bHasVendorFilename = true;
			}
			// check rotation matrix
			else if (key == _T("vendorspecifictargetfile"))
			{
				if (inNKeywords <= i + 1)
				{
					mexErrMsgIdAndTxt("openGPS:writeX3P:missingArgument", "'VendorSpecificTargetFile' keyword not followd by an argument");
				}
				inVendorTarget = inKeywords[++i];
				if (!mxIsChar(inVendorTarget) ||
					mxGetNumberOfElements(inVendorTarget) < 1) {
					mexErrMsgIdAndTxt("openGPS:writeX3P:notString", "'VendorSpecificTargetFile' argument must be a string");
				}
				strVTarget = std::wstring(ConvertMtoWStr(inVendorTarget));
				bHasVendorTarget = true;
			}
			else if (key == _T("vendorspecificuri"))
			{
				if (inNKeywords <= i + 1)
				{
					mexErrMsgIdAndTxt("openGPS:writeX3P:missingArgument", "'VendorSpecificURI' keyword not followd by an argument");
				}
				inVendorURI = inKeywords[++i];
				if (!mxIsChar(inVendorURI) ||
					mxGetNumberOfElements(inVendorURI) < 1) {
					mexErrMsgIdAndTxt("openGPS:writeX3P:notString", "'VendorSpecificURI' argument must be a string");
				}
				strVURI = std::wstring(ConvertMtoWStr(inVendorURI));
				bHasVendorURI = true;
			}
			else
			{
				ostringstream msg;
				msg << "Warning: unknown keyword argument on position " << cKeywordOffset + i << "!" << endl << ends;
				mexWarnMsgIdAndTxt("openGPS:writeX3P:IllegalArgument", msg.str().c_str());
			}
		}
	}

	// Create default arguments for optional arguments if needed
	if (bHasVendorFilename)
	{
		// Check for existence of target file
		if (!bHasVendorTarget)
		{
			// copy vendor filename to target file
			strVTarget = strVFilename;
		}
	}

	auto handle = ogps_OpenISO5436_2(FileNameL.c_str(), nullptr);

	if (ogps_HasError())
	{
		ostringstream msg;
		msg << "Error opening X3P-file name \"" << FileNameL << "\"!" << endl
			<< wstring(ogps_GetErrorMessage()) << endl
			<< wstring(ogps_GetErrorDescription()) << endl
			<< ends;
		mexErrMsgIdAndTxt("openGPS:openX3P:FileOpen", msg.str().c_str());
		return;
	}

	if (!handle)
		return;

	// Check for vendor specific extension
	if (bHasVendorFilename)
	{
		if (!ogps_GetVendorSpecific(handle, strVURI.c_str(), strVFilename.c_str(), strVTarget.c_str()))
		{
			ostringstream msg;
			msg << "Error reading vendor specific extension with URI \"" << strVURI << "\"," << endl
				<< "file name \"" << strVFilename << "\"," << endl
				<< "and destination file name \"" << strVTarget << "\"." << endl
				<< "Please check wether you used the correct URI and filename." << endl;

			if (ogps_HasError())
			{
				msg << "X3P-library reported the following error:" << endl
					<< wstring(ogps_GetErrorMessage()) << endl
					<< wstring(ogps_GetErrorDescription()) << endl;
			}

			msg << ends;
			mexWarnMsgIdAndTxt("openGPS:openX3P:VendorSpecificExtension",
				msg.str().c_str());
		}
	}

	size_t npoints{};
	if (ogps_IsMatrix(handle))
	{
		size_t su{}, sv{}, sw{};
		ogps_GetMatrixDimensions(handle, &su, &sv, &sw);
		npoints = su * sv * sw;

		mwSize dims[3] = { su, sv, sw };

		outMatrixX = mxCreateNumericArray(3, dims, mxDOUBLE_CLASS, mxREAL);
		outMatrixY = mxCreateNumericArray(3, dims, mxDOUBLE_CLASS, mxREAL);
		outMatrixZ = mxCreateNumericArray(3, dims, mxDOUBLE_CLASS, mxREAL);
	}
	else
	{
		npoints = ogps_GetListDimension(handle);

		outMatrixX = mxCreateDoubleMatrix(npoints, 1, mxREAL);
		outMatrixY = mxCreateDoubleMatrix(npoints, 1, mxREAL);
		outMatrixZ = mxCreateDoubleMatrix(npoints, 1, mxREAL);
	}

	plhs[0] = outMatrixZ;
	plhs[1] = outMatrixX;
	plhs[2] = outMatrixY;

	auto ptrX{ mxGetDoubles(outMatrixX) };
	auto ptrY{ mxGetDoubles(outMatrixY) };
	auto ptrZ{ mxGetDoubles(outMatrixZ) };

	auto vector = ogps_CreatePointVector();
	auto iterator = ogps_CreateNextPointIterator(handle);

	for (size_t i = 0; i < npoints; ++i)
	{
		ogps_MoveNextPoint(iterator);
		ogps_GetCurrentCoord(iterator, vector);

		if (ogps_HasError())
		{
			break;
		}

		// A transformed coordinate is allways valid, but some of the components may be set to NaN
		// So we can allways call GetXYZ here.
		OGPS_Double x{}, y{}, z{};
		ogps_GetXYZ(vector, &x, &y, &z);
		*(ptrX++) = x;
		*(ptrY++) = y;
		*(ptrZ++) = z;
	}

	if (ogps_HasNextPoint(iterator))
	{
		ostringstream msg;
		msg << "File \"" << FileNameL << "\" was expected to contain exactly "
			<< npoints << " data points, but it seems to have more!" << endl
			<< ends;
		mexErrMsgIdAndTxt("openGPS:openX3P:FileRead", msg.str().c_str());
	}

	ogps_FreePointIterator(&iterator);
	ogps_FreePointVector(&vector);

	if (hasPointInfo)
		plhs[3] = GetPointInfoStructure(handle);

	if (hasMeta)
	{
		const auto document = ogps_GetDocument(handle);
		if (ogps_HasError())
		{
			ostringstream msg;
			msg << "Could not access XML document in file \"" << FileNameL << "\"!"
				<< endl << ends;
			mexErrMsgIdAndTxt("openGPS:openX3P:XMLDocument", msg.str().c_str());
		}

		const auto& r2opt = document->Record2();

		// Check for presence of meta data
		if (!r2opt.present())
		{
			ostringstream msg;
			msg << "File \"" << FileNameL << "\" does not contain meta data Record2!"
				<< endl << ends;
			mexWarnMsgIdAndTxt("openGPS:openX3P:XMLDocument:Record2", msg.str().c_str());
			return;
		}

		const mwSize nelem{ 10 };
		const char* fieldnames[nelem] = { "Date","Creator","Instrument_Manufacturer",
			"Instrument_Model","Instrument_Serial","Instrument_Version",
			"CalibrationDate","ProbingSystem_Type","ProbingSystem_Identification","Comment" };
		plhs[4] = mxCreateStructMatrix(1, 1, nelem, &(fieldnames[0]));

		const auto& r2 = r2opt.get();

		// Data set creation date
		{
			std::wstringstream ss;
			ss << r2.Date();
			mxSetField(plhs[4], 0, "Date", ConvertWtoMStr(ss.str()));
		}

		if (r2.Creator().present())
		{
			if (r2.Creator().get().length() < 1)
			{
				mexWarnMsgIdAndTxt("openGPS:openX3P:XMLDocument:Record2:Creator",
					"Data set creator name is present, but empty!");
				mxSetField(plhs[4], 0, "Creator", mxCreateString("No data set creator specified"));
			}
			else
			{
				mxSetField(plhs[4], 0, "Creator", ConvertWtoMStr(r2.Creator().get()));
			}
		}
		else
			mexWarnMsgIdAndTxt("openGPS:openX3P:XMLDocument:Record2:Creator",
				"Data set was created anonymously.");

		// Measuring instrument
		mxSetField(plhs[4], 0, "Instrument_Manufacturer", ConvertWtoMStr(r2.Instrument().Manufacturer()));
		mxSetField(plhs[4], 0, "Instrument_Model", ConvertWtoMStr(r2.Instrument().Model()));
		mxSetField(plhs[4], 0, "Instrument_Serial", ConvertWtoMStr(r2.Instrument().Serial()));
		mxSetField(plhs[4], 0, "Instrument_Version", ConvertWtoMStr(r2.Instrument().Version()));

		// Calibration
		{
			std::wstringstream ss;
			ss << r2.CalibrationDate();
			mxSetField(plhs[4], 0, "CalibrationDate", ConvertWtoMStr(ss.str()));
		}

		// Probing system type
		mxSetField(plhs[4], 0, "ProbingSystem_Type", ConvertWtoMStr(r2.ProbingSystem().Type()));
		mxSetField(plhs[4], 0, "ProbingSystem_Identification", ConvertWtoMStr(r2.ProbingSystem().Identification()));

		// Extract and print information
		if (r2.Comment().present())
		{
			if (r2.Comment().get().length() < 1)
			{
				mexWarnMsgIdAndTxt("openGPS:openX3P:XMLDocument:Record2:Comment",
					"Data set has an empty comment!");
			}
			else
			{
				mxSetField(plhs[4], 0, "Comment", ConvertWtoMStr(r2.Comment().get()));
			}
		}
		else
			mexWarnMsgIdAndTxt("openGPS:openX3P:XMLDocument:Record2:Comment",
				"Data set contains no comment.");
	}

	ogps_CloseISO5436_2(&handle);
}
