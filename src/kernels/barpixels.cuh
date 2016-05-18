
	
class PixelRGBA {
public:
	__device__ PixelRGBA(): r_(0), g_(0), b_(0), a_(0) { }
	__device__ PixelRGBA(
		unsigned char r, unsigned char g,
		unsigned char b, unsigned char a = 255):
		r_(r), g_(g), b_(b), a_(a) { }

private:
	unsigned char r_, g_, b_, a_;
};

friend PixelRGBA operator+(const PixelRGBA const PixelRGBA&);

__device__ 
PixelRGBA operator+(const PixelRGBA& p1, const PixelRGBA& p2)
{
	return PixelRGBA(p1.r_ + p2.r_, p1.g_ + p2.g_,
	p1.b_ + p2.b_, p1.a_ + p2.a_);
}