#include "StdAfx.h"
#include "BDriver.h"

BDriver *GDriver = 0;
RRenderTarget *GBackBuffer = 0;

BDriver::BDriver() :
	CurrentVertexDecl(0) {

}

BDriver::~BDriver() {

}
