

extern "C" 
__global__ 
void mandelbrot ( 
	uchar4 *ptr, 
	unsigned int width, 
	unsigned int height, 
	int time) 
{


	int iter=48;
	// map from threadIdx/BlockIdx to pixel position
    float x = threadIdx.x + blockIdx.x * blockDim.x;
    float y = threadIdx.y + blockIdx.y * blockDim.y;
    int offset = x + y * blockDim.x * gridDim.x;
    
	  float cx = 3.0f * (x / width-0.5f);
	  float cy = 2.0f * (y / height-0.5f);

	  int i;
    float zx = cx;
    float zy = cy;
		float dx = 0.0f;
		float dy = 0.0f; 
    cx *=  cos(time/1000.0f);
    cy *=  sin(time/1000.0f);
	  for(i=0; i<iter; i++) {
      dx = (zx * zx - zy * zy) + cx + dx;
      dy = (zy * zx + zx * zy) + cy + dy;
		  if((dx * dx + dy * dy) > 4.0f) break;
		  zx = dx;
		  zy = dy;
	  }

	  unsigned char val = 255.0f*float(i==iter?0:i)/iter;
    
    ptr[offset].x = min(255,3*val/2);
    ptr[offset].y = (val>100 ? min(255,3*(val-100)) : 0);
    ptr[offset].z = (val>127 ? min(255,2*(val-127)) : 0);
    ptr[offset].w = 255;
}

