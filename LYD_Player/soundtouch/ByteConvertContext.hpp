#ifndef _Byte_Convert_Context_hpp_
#define _Byte_Convert_Context_hpp_


class ByteConvertContext
{
public:
	//sample size
	unsigned short bits_per_sample;
	char* writeBuffer = nullptr;

public:
	bool open(unsigned short sampleSize)
	{
		close();
		bits_per_sample = sampleSize;
		return true;
	}

	void close()
	{
		if (writeBuffer == nullptr) return;
		delete writeBuffer;
		writeBuffer = nullptr;
	}

	//all memory clear with caller
	char* write(const float *buffer, int numElems, int* outsize)
	{
		int numBytes;
		int bytesPerSample;

		if (numElems == 0) return nullptr;

		bytesPerSample = bits_per_sample / 8;
		numBytes = numElems * bytesPerSample;
		if (writeBuffer == nullptr)
		{
			writeBuffer = new char[(numBytes + 7 + 15) & -8];
			if (writeBuffer == nullptr) return nullptr;
		}
		//char *temp = new char[(numBytes + 7 + 15) & -8];
		

		switch (bytesPerSample)
		{
			case 1:
			{
				unsigned char *temp2 = (unsigned char *)writeBuffer;
				for (int i = 0; i < numElems; i++)
				{
					temp2[i] = (unsigned char)saturate(buffer[i] * 128.0f + 128.0f, 0.0f, 255.0f);
				}
				break;
			}
		case 2:
			{
				short *temp2 = (short *)writeBuffer;
				for (int i = 0; i < numElems; i++)
				{
					short value = (short)saturate(buffer[i] * 32768.0f, -32768.0f, 32767.0f);
					temp2[i] = _swap16(value);
				}
			}
			break;
		case 3:
			{
				char *temp2 = (char *)writeBuffer;
				for (int i = 0; i < numElems; i++)
				{
					int value = saturate(buffer[i] * 8388608.0f, -8388608.0f, 8388607.0f);
					*((int*)temp2) = _swap32(value);
					temp2 += 3;
				}
			}
			break;
		case 4:
			{
				int *temp2 = (int *)writeBuffer;
				for (int i = 0; i < numElems; i++)
				{
					int value = saturate(buffer[i] * 2147483648.0f, -2147483648.0f, 2147483647.0f);
					temp2[i] = _swap32(value);
				}
			}
			break;
		default:
			break;
		}

		*outsize = numBytes;
		return writeBuffer;
	}

	//all memory clear with caller
	int read(float *buffer, int maxElems, char* pcm, int pcmSize)
	{
		unsigned int afterDataRead;
		int numBytes;
		int numElems;
		int bytesPerSample;

		bytesPerSample = bits_per_sample / 8;
		if ((bytesPerSample < 1) || (bytesPerSample > 4))
		{
			return -1;
		}

		numBytes = maxElems * bytesPerSample;

		// read raw data into temporary buffer
		char *temp = pcm;
		numBytes = pcmSize;

		numElems = numBytes / bytesPerSample;

		// swap byte ordert & convert to float, depending on sample format
		switch (bytesPerSample)
		{
		case 1:
			{
				unsigned char *temp2 = (unsigned char*)temp;
				double conv = 1.0 / 128.0;
				for (int i = 0; i < numElems; i++)
				{
					buffer[i] = (float)(temp2[i] * conv - 1.0);
				}
			}
			break;
		case 2:
			{
				short *temp2 = (short*)temp;
				double conv = 1.0 / 32768.0;
				for (int i = 0; i < numElems; i++)
				{
					short value = temp2[i];
					buffer[i] = (float)(_swap16(value) * conv);
				}
			}
			break;
		case 3:
			{
				char *temp2 = (char *)temp;
				double conv = 1.0 / 8388608.0;
				for (int i = 0; i < numElems; i++)
				{
					int value = *((int*)temp2);
					value = _swap32(value) & 0x00ffffff;             // take 24 bits
					value |= (value & 0x00800000) ? 0xff000000 : 0;  // extend minus sign bits
					buffer[i] = (float)(value * conv);
					temp2 += 3;
				}
			}
			break;
		case 4:
			{
				int *temp2 = (int *)temp;
				double conv = 1.0 / 2147483648.0;

				for (int i = 0; i < numElems; i++)
				{
					int value = temp2[i];
					buffer[i] = (float)(_swap32(value) * conv);
				}
			}
			break;
		}
		return numElems;
	}

private:
	inline int _swap32(int &dwData)
	{
		return dwData;
	}

	inline short _swap16(short &wData)
	{
		return wData;
	}

	inline int saturate(float fvalue, float minval, float maxval)
	{
		if (fvalue > maxval)
		{
			fvalue = maxval;
		}
		else if (fvalue < minval)
		{
			fvalue = minval;
		}
		return (int)fvalue;
	}
};

#endif // !_Byte_Convert_Context_hpp_
