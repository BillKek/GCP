#ifndef GCP_IOH
#define GCP_IOH
#include <fstream>
#include <sstream>

//������� � ������ �������
class GPC_IO_ERROR
{
};

class GCP_IO
{
	private:
		std::fstream fs;
		int _iMode;
	public:
		SDL_RWops *io;
		const static int GCP_IO_INPUT = 0;
		const static int GCP_IO_OUTPUT = 1;	
		const static int GCP_IO_OUTPUTTXT = 2;	
		GCP_IO(string filename, int mode){
			switch(mode)
			{
				case GCP_IO_INPUT:
					io = SDL_RWFromFile(filename.c_str(), "rb"); break;
				case GCP_IO_OUTPUT:			
					io = SDL_RWFromFile(filename.c_str(), "wb"); break;
				case GCP_IO_OUTPUTTXT:			
					io = SDL_RWFromFile(filename.c_str(), "w"); break;
			}
			_iMode = mode;
		}

		~GCP_IO()
		{
			io->close(io);
			//delete io;
		}

		void read(void *type, size_t size, int count = 1)
		{
			io->read(io,type,size,count);
		}
		void write(const void *type, size_t size, int count = 1)
		{
			io->write(io,type,size,count);
		}


		int readInt()
		{			
			int value;
			readInt(&value);
			return value;
		}

		unsigned int readUInt()
		{			
			unsigned int value;
			readInt(&value);
			return value;
		}

		double readDouble()
		{		
			double value;
			readDouble(&value);
			return value;	
		}

		string readString()
		{
			string value;
			readString(&value);
			return value;
		}

		void readInt(int *value)
		{			
			io->read(io,value,sizeof(int),1);
		}

		void readInt(unsigned int *value)
		{			
			io->read(io,value,sizeof(unsigned int),1);
		}

		void readDouble(double *value)
		{			
			io->read(io,value,sizeof(double),1);		
		}

		void readString(string *value)
		{
			char str[256];
			io->read(io,&str,sizeof(str),1);
			*value = str;
		}

		void writeInt(int value)
		{
			io->write(io,&value,sizeof(int),1);
		}

		void writeDouble(double value)
		{
			io->write(io,&value,sizeof(double),1);
			
		}
		void writeString(string value)
		{			
			char str[256];
			strncpy_s(str,value.c_str(),255);	

			if(GCP_IO_OUTPUTTXT == _iMode)
			{
				size_t len = SDL_strlen(str);				
				if(SDL_RWwrite(io, str, 1, len) != len)
					throw GPC_IO_ERROR();
			}
			else
				io->write(io,&str,sizeof(str),1);						
		}

		void writeln()
		{
			char str = '\n';
			SDL_RWwrite(io, &str, 1, 1);
		}

		void close()
		{
			io->close(io);
		}
};

#endif