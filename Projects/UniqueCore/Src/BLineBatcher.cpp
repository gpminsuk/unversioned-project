#include "StdAfx.h"
#include "BLineBatcher.h"

#include "RResource.h"

BLineBatcher* GLineBatcher;

BLineBatcher::BLineBatcher(void) {
}

BLineBatcher::~BLineBatcher(void) {
}

void BLineBatcher::AddLine(TVector3 Point1, TVector3 Point2) {
    TLine Line;
    Line.Point1 = Point1;
    Line.Point2 = Point2;
    Lines.AddItem(Line);
}

void BLineBatcher::Clear() {
    Lines.Clear();
}
