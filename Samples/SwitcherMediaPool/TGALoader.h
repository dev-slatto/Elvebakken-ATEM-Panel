/* -LICENSE-START-
** Copyright (c) 2012 Blackmagic Design
**
** Permission is hereby granted, free of charge, to any person or organization
** obtaining a copy of the software and accompanying documentation covered by
** this license (the "Software") to use, reproduce, display, distribute,
** execute, and transmit the Software, and to prepare derivative works of the
** Software, and to permit third-parties to whom the Software is furnished to
** do so, all subject to the following:
** 
** The copyright notices in the Software and this entire statement, including
** the above license grant, this restriction and the following disclaimer,
** must be included in all copies of the Software, in whole or in part, and
** all derivative works of the Software, unless such copies or derivative
** works are solely in the form of machine-executable object code generated by
** a source language processor.
** 
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
** FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
** SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
** FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
** ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
** DEALINGS IN THE SOFTWARE.
** -LICENSE-END-
*/

#pragma once

#include <string>

enum TGAImageDescriptor
{
	// we only support alignment flags in the image descriptor
	kTGAImageDescriptor_AlphaMask = 0x0F,		// number of bits per pixel reserved for the attribute/alpha channel
	kTGAImageDescriptor_RightToLeft = (1 << 4), // pixel rows are ordered right to left
	kTGAImageDescriptor_TopToBottom = (1 << 5), // pixel rows are ordered top to bottom
	kTGAImageDescriptor_SupportMask = kTGAImageDescriptor_AlphaMask | kTGAImageDescriptor_RightToLeft | kTGAImageDescriptor_TopToBottom
};

// A very basic TGA loader and parser that works with some TGA files
class TGALoader
{
public:
	TGALoader(std::string path); // path to the TGA file that is to be loaded and parsed
	~TGALoader();

	bool	valid();				// true if the image was loaded and successfully parsed, false otherwise

	// the following methods will return valid data only if valid() returns true

	void*	imageData();			// image data
	int		imageDataSize();		// image data size in bytes
	int		width();				// width of the image in pixels
	int		height();				// height of the image in pixels
	int		bitsPerPixel();			// number of bits per pixel
	int		imageDescriptorFlags();	// additional image attributes, see TGAImageDescriptor enum

private:

	// remove padding in structs
	#pragma pack(push, 1)

	// compulsory TGA header 
	struct TGAHeader
	{
		unsigned char		idLength;			// 1 byte
		unsigned char		colourMapType;		// 1 byte
		unsigned char		imageTypeCode;		// 1 byte
		unsigned short int	colourMapOrigin;	// 2 bytes
		unsigned short int	colourMapLength;	// 2 bytes
		unsigned char		colourMapDepth;		// 1 byte
		unsigned short int	xOrigin;			// 2 bytes
		unsigned short int	yOrigin;			// 2 bytes
		unsigned short int	width;				// 2 bytes
		unsigned short int	height;				// 2 bytes
		unsigned char		bitsPerPixel;		// 1 byte
		unsigned char		imageDescriptor;	// 1 byte
	};

	// optional TGA footer, version 2.0
	struct TGAFooter
	{
		unsigned int		extensionOffset;	// 4 bytes
		unsigned int		developerOffset;	// 4 bytes
		char				signature[16];		// 16 bytes "TRUEVISION-XFILE"
		unsigned char		eof[2];				// 2 bytes { 0x2E, NULL }
	};

	// restore padding in structs
	#pragma pack(pop)

	bool			mValid;
	unsigned char*	mImageData;
	int				mImageDataSize;
	int				mWidth;
	int				mHeight;
	int				mBitsPerPixel;
	int				mImageDescriptorFlags;
};
